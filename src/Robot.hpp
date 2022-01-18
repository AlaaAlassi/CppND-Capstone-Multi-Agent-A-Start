#pragma once
#include "Cartesian2DPoint.hpp"
#include <thread>

struct Robot
{
    Robot(Cartesian2DPoint position) : position(position){};
    Cartesian2DPoint position;
    bool goalReached = false;
    std::mutex mtx;

    void trackGoalPosition(Cartesian2DPoint goal, double d)
    {
        goalReached = false;
        auto n = std::round(this->distanceToPoint(goal) / d);
        for (int i = 0; i < n; i++)
        {
            double cos_heading = (goal.y - position.y) / distanceToPoint(goal);
            double sin_heading = (goal.x - position.x) / distanceToPoint(goal);
            double dy = cos_heading * d;
            double dx = sin_heading * d;
            step(dx, dy);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        goalReached = true;
    }

    void step(double dx, double dy)
    {
        std::lock_guard<std::mutex> lck(mtx);
        position.x = position.x + dx;
        position.y = position.y + dy;
    }

    template <typename T>
    int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    double distanceToPoint(Cartesian2DPoint point)
    {
        double x1 = position.x;
        double y1 = position.y;
        double x2 = point.x;
        double y2 = point.y;
        return sqrt(pow(x2 - x1, 2) +
                    pow(y2 - y1, 2));
    }
};