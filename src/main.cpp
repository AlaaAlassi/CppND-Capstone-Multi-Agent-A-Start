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

    for(auto &cell:map){
        if (cell.rowIndex == 2 || cell.rowIndex == 6 || cell.rowIndex == 10 || cell.rowIndex == 14 || cell.rowIndex == 18){
            cell.value = CellValue::occupied;
        }
        if (cell.columnsIndex == 1 || cell.columnsIndex == 2 || cell.columnsIndex == 4 || cell.columnsIndex == 5 ||
            cell.columnsIndex == 29 || cell.columnsIndex == 30 || cell.columnsIndex == 32 || cell.columnsIndex == 33){
            cell.value = CellValue::delivary;
        }
        if (cell.rowIndex == 0 || cell.rowIndex == 20){
            cell.value = CellValue::emptey;
        }
        if (cell.rowIndex%2 > 0){
            cell.value = CellValue::delivary;
        }
        if (cell.columnsIndex == 0 || cell.columnsIndex == 3 || cell.columnsIndex == 6 || cell.columnsIndex == 17 ||
        cell.columnsIndex ==  27 || cell.columnsIndex ==  28 || cell.columnsIndex == 31 || cell.columnsIndex == 34){
            cell.value = CellValue::emptey;
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
        imshow("Output", output);
    }
    //wait for the user to press any key:
    waitKey(0);

    return 0;
}