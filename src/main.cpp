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
    std::vector<CellData> map = Warehouse::getDefultMap();

    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);
    auto rob1 = std::make_shared<Robot>(map.at(0).cartesianPosition);
    Graphics A = Graphics(windowLength, windowWidth, map);
    A._robots.push_back(rob1);
    A.loadBackgroundImg();
    std::thread simulationThread(&Graphics::simulate, &A);
    Cartesian2DPoint goal;
    for (auto const &cell : map)
    {
        goal = cell.cartesianPosition;
        double stepDistance = 1;
        std::future<bool> ftr = std::async(std::launch::async, &Robot::trackGoalPosition,rob1, goal, stepDistance);
        ftr.get();
    }
    std::cout << "Goal reached, distance error: " << rob1->distanceToPoint(goal) << std::endl;
    //wait for the user to press any key:
    simulationThread.join();
    waitKey(0);
    return 0;
}