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

// testing against collision happening due to robots swapping cells
TEST(CollisionTest,SwappingCellsTest){

    // environment setup
    double cellSize = 100;
    size_t numberOfColumns = 5;
    size_t numberOfRows = 5;
    Map testMap = Map(numberOfColumns,numberOfRows,cellSize);
    testMap.setCellValue(0,2,CellValue::occupied);
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
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task2(testMap.getCell(1, 0), testMap.getCell(0, 0));
    Planner multiAgentPlanner(&testMap);
    std::vector<std::future<shared_ptr<Robot>>> ExecutionThreads;

    // set initial time stamp
    int t0 = 0;

    // plan the path for rob1 to reach the task1
    multiAgentPlanner.planPath(rob1, task1, t0);

    //execute the planned path for rob1
    ExecutionThreads.emplace_back(async(std::launch::async, &Robot::trackPath, rob1));

    // wait for one sec
    this_thread::sleep_for(chrono::milliseconds(1000));

    // increment initial time stamp
    t0 = t0 + 1;

    // plan the path for rob2 to reach the task2
    multiAgentPlanner.planPath(rob2, task2, t0);

    // for this scenario, a collision happens if getCell(1,2) is reserved by rob1 and rob2
    EXPECT_EQ(testMap.getCell(1,2)->getVisitHistory().size(), 1);

    //execute the planned path for rob2
    ExecutionThreads.emplace_back(async(std::launch::async, &Robot::trackPath, rob2));

    // wait until all robots reach their destinations
        for (int i = 0; i < ExecutionThreads.size(); i++)
        {
            ExecutionThreads[i].wait();
        }

    viewer.exit();
    simulationThread.join();
}

// testing against collision happening due to robots landing on the same cell
TEST(CollisionTest,OccupyingSameCell){

 // environment setup
    double cellSize = 100;
    size_t numberOfColumns = 5;
    size_t numberOfRows = 5;
    Map testMap = Map(numberOfColumns,numberOfRows,cellSize);
    testMap.setCellValue(0,2,CellValue::occupied);
    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);
    auto rob1 = std::make_shared<Robot>(1, testMap.getCell(0, 1), testMap.getCellSize() * 0.5);
    auto rob2 = std::make_shared<Robot>(2, testMap.getCell(0, 4), testMap.getCellSize() * 0.5);
    deque<shared_ptr<Robot>> fleet{rob1, rob2};
    Graphics viewer = Graphics(windowLength, windowWidth, testMap);
    viewer.loadBackgroundImg();
    std::thread simulationThread(&Graphics::run, &viewer);
    viewer.setRobots(fleet);
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task1(testMap.getCell(0, 3), testMap.getCell(0, 0));
    pair<shared_ptr<CellData>, shared_ptr<CellData>> task2(testMap.getCell(1, 0), testMap.getCell(0, 0));
    Planner multiAgentPlanner(&testMap);
    std::vector<std::future<shared_ptr<Robot>>> ExecutionThreads;

    // set initial time stamp
    int t0 = 0;

    // plan the path for rob1 to reach the task1
    multiAgentPlanner.planPath(rob1, task1, t0);

    //execute the planned path for rob1
    ExecutionThreads.emplace_back(async(std::launch::async, &Robot::trackPath, rob1));

    // wait for one sec
    this_thread::sleep_for(chrono::milliseconds(1000));

    // increment initial time stamp
    t0 = t0 + 1;

    // plan the path for rob2 to reach the task2
    multiAgentPlanner.planPath(rob2, task2, t0);

    // for this scenario, a collision happens if getCell(1,2) is reserved by rob1 and rob2
    EXPECT_EQ(testMap.getCell(1,2)->getVisitHistory().size(), 1);

    //execute the planned path for rob2
    ExecutionThreads.emplace_back(async(std::launch::async, &Robot::trackPath, rob2));

    // wait until all robots reach their destinations
        for (int i = 0; i < ExecutionThreads.size(); i++)
        {
            ExecutionThreads[i].wait();
        }

    viewer.exit();
    simulationThread.join();
}
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}