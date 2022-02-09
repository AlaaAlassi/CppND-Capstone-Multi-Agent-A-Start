#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include "Warehouse.hpp"
#include "Robot.hpp"
#include "Graphics.hpp"
#include "Planner.hpp"
#include "GenericQueue.hpp"
#include <vector>
#include <deque>
#include <thread>
#include <future>

using namespace cv;

const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

void planningThread(shared_ptr<GenericQueue<shared_ptr<Robot>>> avialableRobots, shared_ptr<GenericQueue<shared_ptr<Robot>>> busyRobots, Map *map)
{
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task1(map->getCell(17, 18), map->getCell(0, 20));

    pair<shared_ptr<CellData>, shared_ptr<CellData>> task2(map->getCell(15, 16), map->getCell(0, 20));
    // MultiAgentPlanner.planPath(rob2,task2,t0);

    pair<shared_ptr<CellData>, shared_ptr<CellData>> task3(map->getCell(15, 18), map->getCell(0, 20));
    // MultiAgentPlanner.planPath(rob3,task3,t0);

    pair<shared_ptr<CellData>, shared_ptr<CellData>> task4(map->getCell(17, 16), map->getCell(0, 20));
    // MultiAgentPlanner.planPath(rob4,task4,t0);

    deque<pair<shared_ptr<CellData>, shared_ptr<CellData>>> tasks;
    tasks.push_back(task1);
    tasks.push_back(task2);
    tasks.push_back(task3);
    tasks.push_back(task4);
    Planner multiAgentPlanner(map);

    int t0 = 0;
    while (true)
    {
        if (!tasks.empty())
        {
            shared_ptr<Robot> rob = avialableRobots->receive();
            std::cout << "[Planning thread] recived robot #" << rob->getID() << std::endl;
            multiAgentPlanner.planPath(rob, tasks.front(), t0);
            tasks.pop_front();
            busyRobots->send(std::move(rob));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            t0 = t0 + 1;
        }
    }
}

int main(int argc, char **argv)
{
    // get visulize the warehouse
    Warehouse warehouse = Warehouse();
    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);
    Graphics viewer = Graphics(windowLength, windowWidth, warehouse._map);

    // construct dummy robots
    auto rob1 = std::make_shared<Robot>(1, warehouse._map.getCell(0, 0), warehouse._map.getCellSize() * 0.5);
    auto rob2 = std::make_shared<Robot>(2, warehouse._map.getCell(0, 34), warehouse._map.getCellSize() * 0.5);
    auto rob3 = std::make_shared<Robot>(3, warehouse._map.getCell(1, 0), warehouse._map.getCellSize() * 0.5);
    auto rob4 = std::make_shared<Robot>(4, warehouse._map.getCell(1, 34), warehouse._map.getCellSize() * 0.5);
    deque<shared_ptr<Robot>> fleet{rob1, rob2, rob3, rob4};
    viewer.setRobots(fleet);
    viewer.loadBackgroundImg();
    std::thread simulationThread(&Graphics::run, &viewer);
    shared_ptr<GenericQueue<shared_ptr<Robot>>> busyRobots = std::make_shared<GenericQueue<shared_ptr<Robot>>>();
    shared_ptr<GenericQueue<shared_ptr<Robot>>> availableRobots = std::make_shared<GenericQueue<shared_ptr<Robot>>>();

    for (auto robot:fleet)
    {
        availableRobots->send(std::move(robot));
    }

    std::thread pThread(&planningThread, availableRobots, busyRobots, &(warehouse._map));

    std::mutex mtx;
    fleet.clear();
    std::vector<std::future<bool>>  moveThread;
    while (true)
    {
        shared_ptr<Robot>  robot = busyRobots->receive();
        std::cout << "[Execution thread] recived robot #" << robot->getID() << std::endl;

         moveThread.emplace_back(async(std::launch::async,&Robot::trackNextPathPoint, robot));

                 std::cout << "robot #" << moveThread.back().valid() << " is done" << std::endl;

    }

    std::cout << "exit" << std::endl;
    // wait for the user to press any key:
    simulationThread.join();
    pThread.join();
    waitKey(0);
    return 0;
}