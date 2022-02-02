#pragma once
#include <memory>
#include <deque>
#include "Cartesian2DPoint.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;
enum CellValue
{
    emptey,
    occupied,
    delivary,
    pickup
};
struct CellData
{
    CellData(unsigned int row, unsigned int col, double cellSize);
    unsigned int rowIndex;
    unsigned int columnsIndex;
    CellValue value = CellValue::emptey;
    Cartesian2DPoint cartesianPosition;
    std::unique_ptr<std::pair<Cartesian2DPoint, Cartesian2DPoint>> corners;
    std::pair<int, int> indexCoordinates;
    bool visited = false;
    double Gvalue = 0;
    double Hvalue = std::numeric_limits<double>::max();


    void reserveCell()
    {
        visitHistory.push_front(_timeStamp);
    }
    void clearPassedTimeStampsSince(int timeStamp)
    {
        std::sort(visitHistory.begin(), visitHistory.end(), std::greater<int>());
        auto it = std::find(visitHistory.begin(), visitHistory.end(), timeStamp);
        if (it != visitHistory.end())
        {
            visitHistory.erase(it, visitHistory.end());
        }
    }
    bool isReserverd(int timeStamp)
    {
        bool found = false;
        if (!visitHistory.empty())
        {
            found = std::find(visitHistory.begin(), visitHistory.end(), timeStamp) != visitHistory.end();
        }
        return found;
    }
    void printVisitHistory()
    {
        std::cout << "row " << rowIndex << " "
                  << "col " << columnsIndex << std::endl;
        for (auto t : visitHistory)
        {
            std::cout << t << std::endl;
        }
    }
    void printIndices(){
        cout << "Row "<< indexCoordinates.first << " " <<"Col "<< indexCoordinates.second << endl;
    }

    double distanceTo(shared_ptr<CellData> other)
    {
        double x1 = this->cartesianPosition.x;
        double y1 = this->cartesianPosition.y;
        double x2 = other->cartesianPosition.x;
        double y2 = other->cartesianPosition.y;
        double x = x1 - x2;
        double y = y1 - y2;
        double dist;
        dist = pow(x, 2) + pow(y, 2);
        dist = sqrt(dist);
        return dist;
    }

    void setParent(shared_ptr<CellData> cell){
        _parentCell = cell;
        shared_ptr<CellData> c = _parentCell.lock();
        this->_timeStamp = c->_timeStamp +1;
    }

    void setTimeStamp(int t){
        _timeStamp = t;
    }

    shared_ptr<CellData> getParentCell(){
        return _parentCell.lock();
    }

    int getTimeStamp(){
        return _timeStamp;
    }
    void reset(){
        _parentCell.reset();
    }

private:
    std::deque<int> visitHistory;
    std::weak_ptr<CellData> _parentCell;
    int _timeStamp = 0;
};
