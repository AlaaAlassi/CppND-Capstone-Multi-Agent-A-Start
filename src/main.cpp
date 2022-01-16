#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include "Warehouse.hpp"
#include "Robot.hpp"
#include "Graphics.hpp"
#include <vector>

using namespace cv;

const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

int main(int argc, char **argv)
{
    //construct a grid
    std::vector<CellData> map = Warehouse::getDefultMap();
    //map.size();
    //initialize a 120X350 matrix of black pixels:

    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);

    auto rob1 = std::make_shared<Robot>(map.at(0).cartesianPosition);
    Graphics A = Graphics(windowLength,windowWidth, map);
    A._robots.push_back(rob1);
    for (auto const &cell : map)
    {
    rob1->position = cell.cartesianPosition;
    A.drawTrafficObjects();
    }

    //wait for the user to press any key:
    waitKey(0);

    return 0;
}