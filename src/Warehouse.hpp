#include "Map.hpp"
#include <vector>
class Warehouse
{
    public:
    double cellSize = 35; //meters/cell
    size_t numberOfColumns = 35;
    size_t numberOfRows = 21;
    Warehouse();
    Map _map = Map(numberOfColumns,numberOfRows,cellSize);
};