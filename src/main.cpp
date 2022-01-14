#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CellData.hpp"
#include <vector>

using namespace cv;

const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

int main(int argc, char **argv)
{
    //construct a grid
    std::vector<CellData> map;
    double cellSize = 35; //meters/cell
    int xSize = 35;
    int ySize = 21;
    for (int j = 0; j < ySize; j++)
    {
        for (int i = 0; i < xSize; i++)
        {
            int index = i + xSize * (j);
            map.emplace_back(j, i, index, cellSize);
        }
    }

    int counter = 0;
    int shelfStart = 77;
    int shelfSize = 10;
    int horizentalSpaceBetweenShelfs = 1;
    int shelfPerRow = 0;
    int numberOfConstructedShelfs = 2;
    for(auto &cell:map){
        if (cell.linearIndex >= shelfStart){
            cell.value = CellValue::occupied;
            counter++;
            if(counter >= shelfSize){
                shelfStart = cell.linearIndex+ 1 + horizentalSpaceBetweenShelfs;
                counter = 0;
                shelfPerRow++;
            }
            if(shelfPerRow >= numberOfConstructedShelfs){
                cell.value = CellValue::emptey;
                numberOfConstructedShelfs= numberOfConstructedShelfs+2;
                shelfStart = cell.linearIndex+ 120;
            }
        }
    }

    for(auto &cell:map){
        if (cell.columnsIndex == 3){
            cell.value = CellValue::occupied;
        }
    }

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
        Point centerOfCell = Point(cell.cartesianPosition->x, cell.cartesianPosition->y);
        rectangle(output,
                  p1,
                  p2,
                  Scalar(80, 190, 175),
                  thickness = 1,
                  LINE_8,
                  shift = 0);

        if(cell.value == CellValue::occupied){
             circle(output,
               centerOfCell,
               5,
               Scalar(100, 100, 100),
               FILLED,
               LINE_8);
        }else{
               circle(output,
               centerOfCell,
               5,
               Scalar(14, 92, 144),
               FILLED,
               LINE_8);

        }


        //display the image:
        imshow("Output", output);
    }
    //wait for the user to press any key:
    waitKey(0);

    return 0;
}