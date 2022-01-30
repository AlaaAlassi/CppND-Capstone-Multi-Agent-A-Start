#include <utility>
#include "CellData.hpp"

CellData::CellData(unsigned int row, unsigned int col, double cellSize)
    : rowIndex(row), columnsIndex(col)
{
    cartesianPosition.y = rowIndex * cellSize +  cellSize * 0.5;
    cartesianPosition.x = columnsIndex * cellSize + cellSize * 0.5;
    double p1_x = cartesianPosition.x - cellSize*0.5;
    double p1_y = cartesianPosition.y - cellSize*0.5;
    double p2_x = cartesianPosition.x + cellSize*0.5;
    double p2_y = cartesianPosition.y + cellSize*0.5;
    Cartesian2DPoint p1(p1_x,p1_y);
    Cartesian2DPoint p2(p2_x,p2_y);
    corners = std::make_unique<std::pair<Cartesian2DPoint,Cartesian2DPoint>>(p1,p2);
    indexCoordinates = std::make_pair(rowIndex,columnsIndex);
}