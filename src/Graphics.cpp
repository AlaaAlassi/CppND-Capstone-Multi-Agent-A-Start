#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Graphics.hpp"

using namespace cv;

Graphics::Graphics(int windowLength, int windowWidth, Map &map):_windowLength(windowLength),_windowWidth(windowWidth)
{
    _mapImage = Mat::zeros(_windowLength, _windowWidth, CV_8UC3);
    for (auto const &cell : map._cells)
    {
        drawCell(cell,_mapImage);
    }
}

void Graphics::run()
{
    while (!_done)
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
    std::lock_guard<std::mutex> lg(mtx);
    for (auto &robot : _robots)
    {
        RNG rng(robot->getID());
        int b = rng.uniform(0, 255);
        int g = rng.uniform(0, 255);
        int r = sqrt(255 * 255 - g * g - b * b); // ensure that length of color vector is always 255
        Scalar robotColor = cv::Scalar(b, g, r);

        if (robot->isBusy())
        {
            // show robot path
            for (auto const &cell : (robot->getPath()))
            {
                Point rg = Point(cell->cartesianPosition.x, cell->cartesianPosition.y);
                circle(_images.at(1), rg, robot->getRadius(), robotColor, 2, LINE_8);
            }

            // show robot task
            Point rg = Point(robot->getParkingCell()->cartesianPosition.x,robot->getParkingCell()->cartesianPosition.y);
            circle(_images.at(1), rg, robot->getRadius()*0.5, cv::Scalar(0, 0, 0), 2, LINE_8);
        }

        // show the robot
        Point rp = Point(robot->getPosition().x, robot->getPosition().y);
        circle(_images.at(1), rp, robot->getRadius(), robotColor, FILLED, LINE_8);
    }

    float opacity = 1;
    cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

    // display background and overlay image
    cv::imshow(_windowName, _images.at(2));
    cv::waitKey(10);
}

void Graphics::drawCell(std::shared_ptr<CellData> cell, cv::Mat &image)
{
    //Mat im0 = Mat::zeros(_windowLength, _windowWidth, CV_8UC3);
    Point p1 = Point(cell->corners->first.x, cell->corners->first.y);
    Point p2 = Point(cell->corners->second.x, cell->corners->second.y);
    Point centerOfCell = Point(cell->cartesianPosition.x, cell->cartesianPosition.y);
    int shift = 0;

    rectangle(image,
              p1,
              p2,
              Scalar(200, 200, 200),
              FILLED,
              LINE_8,
              shift);

    if (cell->value == CellValue::occupied)
    {
        rectangle(_mapImage,
                  p1,
                  p2,
                  Scalar(0, 0, 0),
                  FILLED,
                  LINE_8,
                  shift);
    }
    else if (cell->value == CellValue::delivery)
    {
        rectangle(image,
                  p1,
                  p2,
                  Scalar(100, 100, 100),
                  FILLED,
                  LINE_8,
                  shift);
    }

    int thickness = 1;
    rectangle(image,
              p1,
              p2,
              Scalar(208, 126, 229),
              thickness,
              LINE_8,
              shift);
}

void Graphics::exit(){
    _done = true;
}