#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>

using namespace cv;




const int MAX_MONITOR_LENGTH = 1080;
const int MAX_MONITOR_WIDTH = 1920;

/**
* @class CellData
* @brief Storage for cell information used during path coverage
*/
class CellData
{
public:
  /**
 * @brief  Constructor for a CellData objects
 * @param  i The index of the cell in the cost map
 * @param  x The x coordinate of the cell in the cost map
 * @param  y The y coordinate of the cell in the cost map
 * @return
 */
  CellData(unsigned int row, unsigned int col,unsigned int i)
  : rowIndex(row), columnsIndex(col), linearIndex(i)
  {
  }
  public:
  unsigned int linearIndex;
  unsigned int rowIndex;
  unsigned int columnsIndex;
  int value = 0;
};


int main(int argc, char **argv)
{
    //construct a grid
    std::vector<CellData> map;
   double gridResolution = 20; //meters/cell
   int xSize = 32;
   int ySize = 32;
for(int j=0; j<ySize;j++){
    for(int i=0;i<xSize;i++){
        int index = i + xSize*(j);
        map.emplace_back(i,j,index);
    }
}
    //create a gui window:
    namedWindow("Output", 1);

    //initialize a 120X350 matrix of black pixels:

    double aspectRatio =0.7;
    int windowWidth = int(aspectRatio*MAX_MONITOR_WIDTH);
    int windowLength = int(aspectRatio* MAX_MONITOR_LENGTH);

    Mat output = Mat::zeros(windowLength,windowWidth , CV_8UC3);

int  warehouseWidth = int((0.9 )* windowWidth);
int  warehouseLength = int((0.9 )* windowLength);
int thickness = 1;
int shift = 0;
int i =0;
for(auto const &cell:map){
    Point p1 = Point(cell.rowIndex*gridResolution,cell.columnsIndex*gridResolution);
    Point p2 = Point(p1.x+gridResolution, p1.y+gridResolution);
    rectangle(output,
             p1,
             p2,
              Scalar(80, 190, 175),
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