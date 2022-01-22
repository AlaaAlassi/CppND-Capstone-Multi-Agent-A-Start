#pragma once
#include "Cartesian2DPoint.hpp"
#include <thread>

class Robot
{
    public :
    Robot(int id,Cartesian2DPoint position) : _id(id), _position(position){};
    Robot(int id,Cartesian2DPoint position,double radius) :_id(id), _position(position),_radius(radius){};
    bool goalReached = false;
    std::mutex mtx;

    bool trackGoalPosition(Cartesian2DPoint goal, double d)
    {
        setGoal(goal);
        goalReached = false;
        auto n = std::round(this->distanceToPoint(goal) / d);
        for (int i = 0; i < n; i++)
        {
            double cos_heading = (goal.y - _position.y) / distanceToPoint(goal);
            double sin_heading = (goal.x - _position.x) / distanceToPoint(goal);
            double dy = cos_heading * d;
            double dx = sin_heading * d;
            step(dx, dy);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        goalReached = true;
        return true;
    }

    void step(double dx, double dy)
    {
        std::lock_guard<std::mutex> lck(mtx);
        _position.x = _position.x + dx;
        _position.y = _position.y + dy;
    }

    template <typename T>
    int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    double distanceToPoint(Cartesian2DPoint point)
    {
        double x1 = _position.x;
        double y1 = _position.y;
        double x2 = point.x;
        double y2 = point.y;
        return sqrt(pow(x2 - x1, 2) +
                    pow(y2 - y1, 2));
    }

    Cartesian2DPoint getGoal(){
        return _goal;
    };

    double getRadius(){
        return _radius;
    };

    void setGoal(Cartesian2DPoint g){
         _goal = g;
    };

    Cartesian2DPoint getPosition(){
        return _position;
    };

    int getID(){
        return _id;
    }

    private:
    Cartesian2DPoint _position;
    int _id = 0;
    Cartesian2DPoint _goal = Cartesian2DPoint(_position.x,_position.y);
    double _radius = 10;
};