# pragma once
#include<memory>
#include "Cartesian2DPoint.hpp"

enum CellValue {emptey,occupied,delivary,pickup};
struct CellData
{
    CellData(unsigned int row, unsigned int col, unsigned int i, double cellSize);
    unsigned int linearIndex;
    unsigned int rowIndex;
    unsigned int columnsIndex;
    CellValue value = CellValue::emptey;
    double cellSize;
    Cartesian2DPoint cartesianPosition;
    std::unique_ptr<std::pair<Cartesian2DPoint,Cartesian2DPoint>> corners;
};
