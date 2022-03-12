#include <gtest/gtest.h>
#include <deque>
#include <vector>
#include <future>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../src/Map.hpp"
#include "../src/Graphics.hpp"
#include "../src/Planner.hpp"


using namespace std;


const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

TEST(CollisionTest,BasicTest){

    double cellSize = 35; //meters/cell
    size_t numberOfColumns = 5;
    size_t numberOfRows = 5;
    Map testMap = Map(numberOfColumns,numberOfRows,cellSize);
    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);
    auto rob1 = std::make_shared<Robot>(1, testMap.getCell(0, 0), testMap.getCellSize() * 0.5);
    auto rob2 = std::make_shared<Robot>(2, testMap.getCell(0, 4), testMap.getCellSize() * 0.5);
    deque<shared_ptr<Robot>> fleet{rob1, rob2};
    Graphics viewer = Graphics(windowLength, windowWidth, testMap);
    viewer.loadBackgroundImg();
    std::thread simulationThread(&Graphics::run, &viewer);
    viewer.setRobots(fleet);
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task1(testMap.getCell(0, 3), testMap.getCell(0, 0));
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task2(testMap.getCell(0, 1), testMap.getCell(0, 0));
    Planner multiAgentPlanner(&testMap);
    int t0 = 0;
    multiAgentPlanner.planPath(rob1, task1, t0);
    std::vector<std::future<shared_ptr<Robot>>> moveThread;
    moveThread.emplace_back(async(std::launch::async, &Robot::trackNextPathPoint, rob1));

    auto tic = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - tic);
    this_thread::sleep_for(chrono::milliseconds(1000 - elapsed.count() % 1000));
    auto tStamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - tic);
    t0 = t0 + tStamp.count();
    multiAgentPlanner.planPath(rob2, task2, t0);
    moveThread.emplace_back(async(std::launch::async, &Robot::trackNextPathPoint, rob2));

    /*for (int i = 0; i < moveThread.size(); i++)
        {
            if ((moveThread[i].wait_for(chrono::milliseconds(1))) == future_status::ready)
            {
                moveThread.erase(moveThread.begin() + i);
            };
        }*/

    rob1->getParkingCell()->printVisitHistory();

    ASSERT_EQ(rob1->getParkingCell()->columnsIndex,3);

    simulationThread.join();
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}