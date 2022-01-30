#include "CellData.hpp"
#include "Robot.hpp"
#include "Map.hpp"
#include <deque>

using namespace std;

class Planner
{
public:
    Planner(Map *map) : mapHandler(map){};
    void planPath(shared_ptr<Robot> robot, int t0)
    {
        //create a random path to simulate a planner
        std::cout << "robot #" << robot->getID() << " is planning" << std::endl;
        int timeStamp = t0;
        if (robot->getID() == 1)
        {
            for (int j = 0; j < 18; j++)
            {
                std::lock_guard lg(mtx);
                auto cell = mapHandler->_cells[j];
                if (!cell->isReserverd(timeStamp))
                {
                    robot->appendCellToPath(cell);
                    cell->reserveCell(timeStamp);
                }
                cell->clearPassedTimeStampsSince(t0);
                timeStamp = timeStamp + 1;
            }
        }
        else if (robot->getID() == 2)
        {
            for (int j = 34; j > 15; j--)
            {
                std::lock_guard lg(mtx);
                auto cell = mapHandler->getCell(0, j);
                if (!cell->isReserverd(timeStamp))
                {
                    robot->appendCellToPath(cell);
                    cell->reserveCell(timeStamp);
                }
                else
                {
                    auto nCell = mapHandler->getNeighbours(cell)[2];
                    robot->appendCellToPath(nCell);
                    cell->reserveCell(timeStamp);
                }
                cell->clearPassedTimeStampsSince(t0);
                timeStamp = timeStamp + 1;
            }
        }
    };

    void addNeighbors( shared_ptr<CellData> currentCell)
    {
        auto neighbors = mapHandler->getNeighbours(currentCell);
        for (auto neighbor : neighbors)
        {
            neighbor->Hvalue = CalculateHValue(neighbor);
            neighbor->Gvalue = currentCell->Gvalue + neighbor->distanceTo(currentCell);
            _openList.push_back(neighbor);
            neighbor->visited = true;
        }
    }

    double CalculateHValue(shared_ptr<CellData> cell){
        return cell->distanceTo(_task.first);
    }

private:
    pair<shared_ptr<CellData>, shared_ptr<CellData>> _task;
    std::mutex mtx;
    Map *mapHandler;
    vector<shared_ptr<CellData>> _openList;
};