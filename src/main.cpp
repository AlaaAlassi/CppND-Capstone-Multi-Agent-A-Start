#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char **argv)
{

    //create a gui window:
    namedWindow("Output", 1);

    //initialize a 120X350 matrix of black pixels:
    double aspectRatio =0.9;
    double windowWidth = int(aspectRatio*1920);
    int windowLength = int(aspectRatio*1080);

    Mat output = Mat::zeros(windowLength,windowWidth , CV_8UC3);

int thickness = 1;
int shift = 0;
    rectangle(output,
              Point(15, 70),
              Point(2000, 2000),
              Scalar(0, 255, 0),
              thickness = 1,
              LINE_8,
              shift = 0);

        //display the image:
        imshow("Output", output);

    //wait for the user to press any key:
    waitKey(0);

    return 0;
}