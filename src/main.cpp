#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include "Warehouse.hpp"
#include "Robot.hpp"
#include "Graphics.hpp"
#include <vector>
#include <thread>
#include <future>


using namespace cv;

const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

int main(int argc, char **argv)
{
    //construct a grid
     Warehouse W = Warehouse();
    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);
    Graphics A = Graphics(windowLength, windowWidth,  W._map->_cells);
    auto rob1 = std::make_shared<Robot>(W._map->_cells[0]->cartesianPosition);
    auto rob2 = std::make_shared<Robot>(W._map->_cells[0]->cartesianPosition);
    A._robots.push_back(rob1);
    A._robots.push_back(rob2);
    A.loadBackgroundImg();
    std::thread simulationThread(&Graphics::simulate, &A);
    Cartesian2DPoint goal;
    Cartesian2DPoint goal2;
    for (auto const &cell : W._map->_cells)
    {
        goal = cell->cartesianPosition;
        goal2 = cell->cartesianPosition;
        double stepDistance = 1;
        std::future<bool> ftr = std::async(std::launch::async, &Robot::trackGoalPosition,rob1, goal, stepDistance);
        std::future<bool> ftr2 = std::async(std::launch::async, &Robot::trackGoalPosition,rob2, goal2, 0.5);
        ftr.get();
        ftr2.get();
    }
    std::cout << "Goal reached, distance error: " << rob1->distanceToPoint(goal) << std::endl;
    //wait for the user to press any key:
    simulationThread.join();
    waitKey(0);
    return 0;
}