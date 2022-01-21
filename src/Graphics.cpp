#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Graphics.hpp"

using namespace cv;

Graphics::Graphics(int windowLength,int windowWidth,std::vector<CellData> &map){
    Mat im0 = Mat::zeros(windowLength, windowWidth, CV_8UC3);
    _cellSize = map.at(0).cellSize;
    for (auto const &cell : map)
    {
        Point p1 = Point(cell.corners->first.x,cell.corners->first.y);
        Point p2 = Point(cell.corners->second.x,cell.corners->second.y);
        Point centerOfCell = Point(cell.cartesianPosition.x, cell.cartesianPosition.y);
        int shift = 0;

        rectangle(im0,
                  p1,
                  p2,
                  Scalar(200, 200, 200),
                  FILLED,
                  LINE_8,
                  shift);

        if(cell.value == CellValue::occupied){
        rectangle(im0,
                  p1,
                  p2,
                  Scalar(0, 0, 0),
                  FILLED,
                  LINE_8,
                  shift);
        }else if(cell.value == CellValue::delivary){
             rectangle(im0,
                  p1,
                  p2,
                  Scalar(100, 100, 100),
                  FILLED,
                  LINE_8,
                  shift);
        }else{

        }
        int thickness = 1;
        rectangle(im0,
                  p1,
                  p2,
                  Scalar(208, 126, 229),
                  thickness,
                  LINE_8,
                  shift);
    }
    _mapImage = im0;
}

void Graphics::simulate()
{
while (true)
        {
            drawRobots();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
}

void Graphics::loadBackgroundImg()
{
    // create window
    _windowName = "Multi Agent A* Planner";
    cv::namedWindow(_windowName, cv::WINDOW_AUTOSIZE);

    // load image and create copy to be used for semi-transparent overlay
    cv::Mat background = _mapImage;
    _images.push_back(background);         // first element is the original background
    _images.push_back(background.clone()); // second element will be the transparent overlay
    _images.push_back(background.clone()); // third element will be the result image for display

}

void Graphics::drawRobots()
{
    // reset images
    _images.at(1) = _images.at(0).clone();
    _images.at(2) = _images.at(0).clone();

    // create overlay from all traffic objects
    for (auto &robot : _robots)
    {
        std::unique_lock<std::mutex> uLock(mtx);
        Point rp = Point(robot->position.x,robot->position.y);
        Point rg = Point(robot->getGoal().x,robot->getGoal().y);
        uLock.unlock();
        cv::Scalar robotColor = Scalar(0, 0, 255);
        cv::circle(_images.at(1), rp, _cellSize*0.5, robotColor, FILLED,LINE_8);
        cv::circle(_images.at(1), rg, _cellSize*0.5, robotColor, 2,LINE_8);
    }

    float opacity = 1;
    cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

    // display background and overlay image
    cv::imshow(_windowName, _images.at(2));
    cv::waitKey(10);
}