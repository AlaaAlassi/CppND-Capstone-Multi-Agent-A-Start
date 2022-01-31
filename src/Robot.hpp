#pragma once
#include "Cartesian2DPoint.hpp"
#include "CellData.hpp"
#include <deque>
#include <thread>

using namespace std;
class Robot
{
public:
    Robot(int id, shared_ptr<CellData> cell) : _id(id), _parkingCell(cell){
        _position.x = cell->cartesianPosition.x;
        _position.y = cell->cartesianPosition.y;
    };
    Robot(int id, shared_ptr<CellData> cell, double radius) : _id(id), _parkingCell(cell), _radius(radius){
        _position.x = cell->cartesianPosition.x;
        _position.y = cell->cartesianPosition.y;
    };
    bool goalReached = false;
    std::mutex mtx;

    bool trackNextPathPoint()
    {
        if (isBusy())
        {
            auto goal = _path.back()->cartesianPosition;
            double stepingDistance = 0.5;
            setGoal(goal);
            goalReached = false;
            auto n = std::round(this->distanceToPoint(goal) / stepingDistance);
            for (int i = 0; i < n; i++)
            {
                double cos_heading = (goal.y - _position.y) / distanceToPoint(goal);
                double sin_heading = (goal.x - _position.x) / distanceToPoint(goal);
                double dy = cos_heading * stepingDistance;
                double dx = sin_heading * stepingDistance;
                step(dx, dy);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            goalReached = true;
            std::lock_guard<std::mutex> lck(mtx);
            _path.pop_back();
        }
        return true;
    }

    void step(double dx, double dy)
    {
        _position.x = _position.x + dx;
        _position.y = _position.y + dy;
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

    Cartesian2DPoint getGoal()
    {
        return _goal;
    };

    double getRadius()
    {
        return _radius;
    };

    void setGoal(Cartesian2DPoint g)
    {
        _goal = g;
    };

    void appendCellToPath(std::shared_ptr<CellData> cell)
    {
        _path.push_front(cell);
    }

    Cartesian2DPoint getPosition()
    {
        std::lock_guard<std::mutex> lck(mtx);
        return _position;
    };

    int getID()
    {
        std::lock_guard<std::mutex> lck(mtx);
        return _id;
    }

    std::deque <std::shared_ptr<CellData>> getPath()
    {
        return _path;
    };

    bool isNotBusy()
    {
        return _path.empty();
    }

    bool isBusy()
    {
        return !_path.empty();
    }

private:
    Cartesian2DPoint _position;
    int _id = 0;
    Cartesian2DPoint _goal = Cartesian2DPoint(_position.x, _position.y);
    double _radius = 10;
    std::deque <std::shared_ptr<CellData>> _path;
    shared_ptr<CellData> _parkingCell;
};