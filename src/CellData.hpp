# pragma once
#include<memory>

enum CellValue {emptey,occupied,delivary,pickup};
struct Cartesian2DPoint
{
    Cartesian2DPoint(): x(0),y(0){}
   Cartesian2DPoint(double x,double y): x(x),y(y){}
   double x;
   double y;
};

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
