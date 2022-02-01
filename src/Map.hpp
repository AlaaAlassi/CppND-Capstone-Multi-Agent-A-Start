#pragma once
#include "CellData.hpp"
#include <iostream>
#include <vector>

using namespace std;
class Map
{
public:
    Map(std::size_t sizeCol, std::size_t sizeRow, double cellSize) : _sizeCol(sizeCol), _sizeRow(sizeRow), _cellSize(cellSize)
    {
        for (int j = 0; j < sizeRow; j++)
        {
            for (int i = 0; i < sizeCol; i++)
            {
                _cells.emplace_back(std::make_shared<CellData>(j, i, cellSize));
            }
        }
    };
    std::shared_ptr<CellData> getCell(int i, int j)
    {
        if (cellIsValid(i, j))
        {
            int cellIndex = i * _sizeCol + j;
            return _cells[cellIndex];
        }
        else
        {
            cout << "Error[Class: Map][Method: getCell(i,j)]: Requst to acces a cell out of map bounds" << endl;
            exit(EXIT_FAILURE);
        }
    };
    void setCellValue(int i, int j, CellValue value)
    {
        auto cell = getCell(i, j);
        cell->value = value;
    };

    std::size_t getNumberOfColumns() { return _sizeCol; };
    std::size_t getNumberOfRows() { return _sizeRow; };
    double getCellSize() { return _cellSize; };
    bool cellIsValid(int i, int j)
    {
        bool isValid = true;
        if (i >= getNumberOfRows() ||
            j >= getNumberOfColumns())
        {
            isValid = false;
        }
        return isValid;
    }

    std::vector<std::shared_ptr<CellData>> getNeighbours(std::shared_ptr<CellData> cell)
    {
        std::vector<std::shared_ptr<CellData>> v;
        for (auto &it : searchWindow)
        {
            int i = it.first + cell->indexCoordinates.first;
            int j = it.second + cell->indexCoordinates.second;
            if (cellIsValid(i, j))
            {
                v.push_back(this->getCell(i, j));
            }
        }
        return v;
    }

    std::vector<std::shared_ptr<CellData>> _cells;

private:
    const std::vector<std::pair<int, int>> searchWindow{make_pair(0, 1),
                                                        make_pair(0, -1),
                                                        make_pair(1, 0),
                                                        make_pair(-1, 0)};
    std::size_t _sizeCol;
    std::size_t _sizeRow;
    double _cellSize;
};