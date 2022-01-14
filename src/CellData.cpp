#include "CellData.hpp"

CellData::CellData(unsigned int row, unsigned int col, unsigned int i, double cellSize)
    : rowIndex(row), columnsIndex(col), linearIndex(i), cellSize(cellSize)
{
    double y = rowIndex * cellSize +  cellSize * 0.5;
    double x = columnsIndex * cellSize + cellSize * 0.5;
    cartesianPosition = std::make_unique<Cartesian2DPoint>(x, y);
}
