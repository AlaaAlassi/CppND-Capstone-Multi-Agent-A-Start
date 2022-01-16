#pragma once
#include"Cartesian2DPoint.hpp"

struct Robot{
    Robot(Cartesian2DPoint position): position(position){};
    Cartesian2DPoint position;
};