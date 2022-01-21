#include "Warehouse.hpp"

std::vector<CellData> Warehouse::getDefultMap()
{
    std::vector<CellData> map;
    double cellSize = 35; //meters/cell
    int xSize = 35;
    int ySize = 21;
    for (int j = 0; j < ySize; j++)
    {
        for (int i = 0; i < xSize; i++)
        {
            int index = i + xSize * (j);
            map.emplace_back(j, i, index, cellSize);
        }
    }
    for (auto &cell : map)
    {
        if (cell.rowIndex == 2 || cell.rowIndex == 6 || cell.rowIndex == 10 || cell.rowIndex == 14 || cell.rowIndex == 18)
        {
            cell.value = CellValue::occupied;
        }
        if (cell.columnsIndex == 1 || cell.columnsIndex == 2 || cell.columnsIndex == 4 || cell.columnsIndex == 5 ||
            cell.columnsIndex == 29 || cell.columnsIndex == 30 || cell.columnsIndex == 32 || cell.columnsIndex == 33)
        {
            cell.value = CellValue::delivary;
        }
        if (cell.rowIndex == 0 || cell.rowIndex == 20)
        {
            cell.value = CellValue::emptey;
        }
        if (cell.rowIndex % 2 > 0)
        {
            cell.value = CellValue::delivary;
        }
        if (cell.columnsIndex == 0 || cell.columnsIndex == 3 || cell.columnsIndex == 6 || cell.columnsIndex == 17 ||
            cell.columnsIndex == 27 || cell.columnsIndex == 28 || cell.columnsIndex == 31 || cell.columnsIndex == 34)
        {
            cell.value = CellValue::emptey;
        }
    }
    return map;
}
