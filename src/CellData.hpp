#pragma once
#include <memory>
#include <deque>
#include "Cartesian2DPoint.hpp"
#include <iostream>
#include <algorithm>

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
    void reserveCell(int timeStamp)
    {
        visitHistory.push_front(timeStamp);
    }
    void clearPassedTimeStampsSince(int timeStamp)
    {
        std::sort(visitHistory.begin(),visitHistory.end(), std::greater<int>());
        auto it = std::find(visitHistory.begin(), visitHistory.end(), timeStamp);
        if (it != visitHistory.end())
        {
            visitHistory.erase(it,visitHistory.end());
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
    void printVisitHistory (){
        std::cout<< "row " << rowIndex << " " <<"col "<<  columnsIndex <<std::endl;
        for(auto t:visitHistory){
            std::cout<< t <<std::endl;
        }
    }

private:
    std::deque<int> visitHistory;
};
