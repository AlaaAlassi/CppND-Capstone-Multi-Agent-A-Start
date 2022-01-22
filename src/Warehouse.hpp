#include "CellData.hpp"
#include <vector>

class Map
{
    public:
    Map(std::size_t sizeCol, std::size_t sizeRow, double cellSize) : _sizeCol(sizeCol), _sizeRow(sizeRow)
    {
        for (int j = 0; j < sizeRow ; j++)
        {
            for (int i = 0; i < sizeCol ; i++)
            {
                _cells.emplace_back(std::make_shared<CellData>(j, i, cellSize));
            }
        }
    };
    std::shared_ptr<CellData> getCell(int i, int j)
    {
        int cellIndex = i * _sizeCol + j;
        return _cells[cellIndex];
    };
    void setCellValue(int i, int j, CellValue value){
        auto cell = getCell(i, j);
        cell->value = value;
    };

    std::size_t getNumberOfColumns(){return _sizeCol;};
    std::size_t getNumberOfRows(){return _sizeRow;};

    std::vector<std::shared_ptr<CellData>> _cells;
private:
    std::size_t _sizeCol;
    std::size_t _sizeRow;
};
class Warehouse
{
    public:
    double cellSize = 35; //meters/cell
    size_t numberOfColumns = 35;
    size_t numberOfRows = 21;
    Warehouse();
    Map _map = Map(numberOfColumns,numberOfRows,cellSize);
};