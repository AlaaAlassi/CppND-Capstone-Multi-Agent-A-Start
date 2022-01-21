#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "CellData.hpp"
#include "Robot.hpp"

class Graphics
{
public:
    Graphics(int windowLength,int windowWidth,std::vector<std::shared_ptr<CellData>> map);

    // getters / setters
    void setRobots(std::vector<std::shared_ptr<Robot>> &robot) { _robots = robot; };

    // typical behaviour methods
    void simulate();
    void loadBackgroundImg();
        // typical behaviour methods
    void drawRobots();
    std::vector<std::shared_ptr<Robot>> _robots;

private:
    // member variables
    std::string _windowName;
    std::vector<cv::Mat> _images;
    cv::Mat _mapImage;
    double _cellSize;
    std::mutex mtx;
};

#endif