#pragma once
struct Cartesian2DPoint
{
   Cartesian2DPoint(): x(0),y(0){}
   Cartesian2DPoint(double x,double y): x(x),y(y){}
   double x;
   double y;
};