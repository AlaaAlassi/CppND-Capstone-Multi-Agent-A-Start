#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "Map.hpp"
#include "Robot.hpp"

class Graphics
{
public:
    Graphics(int windowLength,int windowWidth,Map &map);

    // getters / setters
    void setRobots(std::deque<std::shared_ptr<Robot>> robot) { _robots = robot; };

    // typical behaviour methods
    void run();
    void loadBackgroundImg();
        // typical behaviour methods
    void drawRobots();
    void drawCell(std::shared_ptr<CellData> cell, cv::Mat & image);
    std::deque<std::shared_ptr<Robot>> _robots;

private:
    // member variables
    std::string _windowName;
    std::vector<cv::Mat> _images;
    cv::Mat _mapImage;
    std::mutex mtx;
    int _windowLength;
    int _windowWidth;
};

#endif