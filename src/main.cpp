#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include "Warehouse.hpp"
#include "Robot.hpp"
#include "Graphics.hpp"
#include "Planner.hpp"
#include <vector>
#include <deque>
#include <thread>
#include <future>

using namespace cv;

const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

int main(int argc, char **argv)
{
    // get visulize the warehouse
    Warehouse warehouse = Warehouse();
    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);
    Graphics viewer = Graphics(windowLength, windowWidth, warehouse._map);

    // construct dummy robots
    auto rob1 = std::make_shared<Robot>(1, warehouse._map._cells[0], warehouse._map.getCellSize() * 0.5);
    auto rob2 = std::make_shared<Robot>(2, warehouse._map._cells[34], warehouse._map.getCellSize() * 0.5);
    auto rob3 = std::make_shared<Robot>(3, warehouse._map._cells[36], warehouse._map.getCellSize() * 0.5);
    std::deque<std::shared_ptr<Robot>> busyRobots;
    busyRobots.push_back(rob1);
    busyRobots.push_back(rob2);
    busyRobots.push_back(rob3);
    viewer.setRobots(busyRobots);
    viewer.loadBackgroundImg();

    // run viewer thread
    std::thread simulationThread(&Graphics::run, &viewer);
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task(warehouse._map.getCell(17,26),warehouse._map.getCell(0,20));
    int t0 = 0;
    Planner MultiAgentPlanner(&(warehouse._map));
    MultiAgentPlanner.planPath(rob1,task,t0);
    t0 = t0+1;
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task2(warehouse._map.getCell(17,24),warehouse._map.getCell(0,20));
    MultiAgentPlanner.planPath(rob2,task2,t0);

    t0 = t0+1;
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task3(warehouse._map.getCell(17,25),warehouse._map.getCell(0,20));
    MultiAgentPlanner.planPath(rob3,task3,t0);


    // execution loop
    std::mutex mtx;
    while (!busyRobots.empty())
    {
        std::vector<std::future<bool>> ftrs;
        for (std::size_t i=0;i < busyRobots.size();i++)
        {
            if (busyRobots[i]->isBusy())
            {
                ftrs.emplace_back(std::async(std::launch::async, &Robot::trackNextPathPoint, busyRobots[i]));
            }else{
                std::cout << "robot #" <<  busyRobots[i]->getID() << " is done" <<std::endl;
                std::lock_guard lg(mtx);
                busyRobots.erase(busyRobots.begin()+i);
            }
        }
            for(auto &ftr:ftrs){
               ftr.wait();
            }
    }
    std::cout << "exit" <<std::endl;
    //wait for the user to press any key:
    simulationThread.join();
    waitKey(0);
    return 0;
}