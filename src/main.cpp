#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include "Warehouse.hpp"
#include "Robot.hpp"
#include "Graphics.hpp"
#include <vector>
#include <thread>

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
    auto goal = map[10].cartesianPosition;
    double stepDistance = 1;
    std::thread t2(&Robot::trackGoalPosition, rob1, goal,stepDistance);
    bool done = false;
    int limit = 1;
    int timer = 0;
    int counter = 10;
    while (!done || !(counter <= 0))
    {
        A.drawTrafficObjects();
        if(rob1->goalReached){
            done = true;
            counter--;
        }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Goal reached, distance error: " << rob1->distanceToPoint(goal) << std::endl;
    //wait for the user to press any key:
    waitKey(0);
    t2.join();

    return 0;
}