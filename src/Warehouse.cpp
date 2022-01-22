#include "Warehouse.hpp"

Warehouse::Warehouse()
{
    for (auto &cell : _map._cells)
    {
        if (cell->rowIndex == 2 || cell->rowIndex == 6 || cell->rowIndex == 10 || cell->rowIndex == 14 || cell->rowIndex == 18)
        {
            cell->value = CellValue::occupied;
        }
        if (cell->columnsIndex == 1 || cell->columnsIndex == 2 || cell->columnsIndex == 4 || cell->columnsIndex == 5 ||
            cell->columnsIndex == 29 || cell->columnsIndex == 30 || cell->columnsIndex == 32 || cell->columnsIndex == 33)
        {
            cell->value = CellValue::delivary;
        }
        if (cell->rowIndex == 0 || cell->rowIndex == 20)
        {
            cell->value = CellValue::emptey;
        }
        if (cell->rowIndex % 2 > 0)
        {
            cell->value = CellValue::delivary;
        }
        if (cell->columnsIndex == 0 || cell->columnsIndex == 3 || cell->columnsIndex == 6 || cell->columnsIndex == 17 ||
            cell->columnsIndex == 27 || cell->columnsIndex == 28 || cell->columnsIndex == 31 || cell->columnsIndex == 34)
        {
            cell->value = CellValue::emptey;
        }
    }
}
