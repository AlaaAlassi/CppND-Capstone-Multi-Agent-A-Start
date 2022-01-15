#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include "Warehouse.hpp"
#include <vector>


using namespace cv;

const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

struct Robot{
    Robot(Cartesian2DPoint position): position(position){};
    Cartesian2DPoint position;
};


int main(int argc, char **argv)
{
    //construct a grid
    std::vector<CellData> map = Warehouse::getDefultMap();

    //create a gui window:
    namedWindow("Output", 1);

    //initialize a 120X350 matrix of black pixels:

    double aspectRatio = 0.7;
    int windowWidth = int(aspectRatio * MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio * MAX_MONITOR_LENGTH);

    Mat output = Mat::zeros(windowLength, windowWidth, CV_8UC3);

    int warehouseWidth = int((0.9) * windowWidth);
    int warehouseLength = int((0.9) * windowLength);
    int thickness = 1;
    int shift = 0;
    int i = 0;
    for (auto const &cell : map)
    {
        Point p1 = Point(cell.corners->first.x,cell.corners->first.y);
        Point p2 = Point(cell.corners->second.x,cell.corners->second.y);
        Point centerOfCell = Point(cell.cartesianPosition.x, cell.cartesianPosition.y);


        rectangle(output,
                  p1,
                  p2,
                  Scalar(200, 200, 200),
                  FILLED,
                  LINE_8,
                  shift = 0);

        if(cell.value == CellValue::occupied){
        rectangle(output,
                  p1,
                  p2,
                  Scalar(0, 0, 0),
                  FILLED,
                  LINE_8,
                  shift = 0);
        }else if(cell.value == CellValue::delivary){
             rectangle(output,
                  p1,
                  p2,
                  Scalar(100, 100, 100),
                  FILLED,
                  LINE_8,
                  shift = 0);
        }else{

        }
        rectangle(output,
                  p1,
                  p2,
                  Scalar(208, 126, 229),
                  thickness = 1,
                  LINE_8,
                  shift = 0);


        //display the image:
        //imshow("Output", output);
    }
    //Mat output2 = Mat::zeros(windowLength, windowWidth, CV_8UC3);
    Mat output3 = Mat::zeros(windowLength, windowWidth, CV_8UC3);
       for (auto const &cell : map)
{

Mat  output2 = Mat::zeros(windowLength, windowWidth, CV_8UC3);
    Robot rob1(cell.cartesianPosition);
     Point p3 = Point(rob1.position.x,rob1.position.y);
               circle(output2,
               p3,
               10,
               Scalar(0, 0, 255),
               FILLED,
               LINE_8);

    float opacity = 0.5;
    cv::addWeighted(output, opacity,output2, 1-opacity, 0, output3);
    imshow("Output", output3);
    //imshow("Output", output);

waitKey(33);
}

    //wait for the user to press any key:
    waitKey(0);

    return 0;
}