# pragma once
#include<memory>

enum CellValue {emptey,occupied,delivary,pickup};
struct Cartesian2DPoint
{
   Cartesian2DPoint(double x,double y): x(x),y(y){}
   const double x;
   const double y;
};

struct CellData
{
    CellData(unsigned int row, unsigned int col, unsigned int i, double cellSize);
    unsigned int linearIndex;
    unsigned int rowIndex;
    unsigned int columnsIndex;
    CellValue value = CellValue::emptey;
    double cellSize;
    std::unique_ptr <Cartesian2DPoint> cartesianPosition;
};
