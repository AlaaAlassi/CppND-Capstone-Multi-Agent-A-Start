#pragma once
#include"Cartesian2DPoint.hpp"

struct Robot{
    Robot(Cartesian2DPoint position): position(position){};
    Cartesian2DPoint position;

    void trackGoalPosition(Cartesian2DPoint goal,std::chrono::time_point<std::chrono::system_clock> t0){
        auto deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t0).count();
        double throttel = 1;
        double vx = sgn(goal.x - position.x)*throttel;
        double vy = sgn(goal.y - position.y)*throttel;
        position.x = vx*deltaT + position.x;
        position.y = vy*deltaT + position.y;
    }

        void trackGoalPosition(Cartesian2DPoint goal,double d){
            double cos_heading = (goal.y - position.y)/distanceToPoint(goal);
            double sin_heading = (goal.x - position.x)/distanceToPoint(goal);
            double dy = cos_heading*d;
            double dx = sin_heading*d;
            step(dx,dy);
        }

    void step(double dx,double dy){
        position.x = position.x+dx;
        position.y = position.y+dy;
    }

    template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
    }

    double distanceToPoint(Cartesian2DPoint point){
        double x1 = position.x;
        double y1 = position.y;
        double x2 = point.x;
        double y2 = point.y;
            return sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2));
    }
};