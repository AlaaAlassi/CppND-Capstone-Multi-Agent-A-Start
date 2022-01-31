#include "CellData.hpp"
#include "Robot.hpp"
#include "Map.hpp"
#include <deque>

using namespace std;

bool compareFvalue(shared_ptr<CellData> c1, shared_ptr<CellData> c2)
{
    return ((c1->Hvalue + c1->Gvalue) > (c2->Hvalue + c2->Gvalue));
}
class Planner
{
public:
    Planner(Map *map) : mapHandler(map){};
    void planPath(shared_ptr<Robot> robot, pair<shared_ptr<CellData>, shared_ptr<CellData>> task, int t0)
    {
        _task = task;
        _robot = robot;
        //create a random path to simulate a planner
        std::cout << "robot #" << robot->getID() << " is planning" << std::endl;
        int timeStamp = t0;
        auto currentCell = robot->getParkingCell();
        currentCell->setTimeStamp(timeStamp);
        currentCell->visited= true;
        Q.push_back(currentCell);
        while (!Q.empty())
        {
            addNeighbors(currentCell);
            // sort open list
            std::sort(Q.begin(), Q.end(), compareFvalue);
            // take the cheapest cell
            currentCell = Q.back();
            Q.pop_back();
            //robot->appendCellToPath(currentCell,0,0);
            // break if goal reached
            if (currentCell->distanceTo(_task.first) <= 1e-7)
            {
                constructFoundPath(currentCell);
                break;
            }
            this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };

    void addNeighbors(shared_ptr<CellData> currentCell)
    {
        auto neighbors = mapHandler->getNeighbours(currentCell);
        for (auto neighbor : neighbors)
        {
            neighbor->Hvalue = neighbor->distanceTo(_task.first);
            neighbor->Gvalue = currentCell->Gvalue + neighbor->distanceTo(currentCell);
            if (!neighbor->visited && neighbor->value != CellValue::occupied && !neighbor->isReserverd())
            {
                neighbor->setParent(currentCell);
                Q.push_back(neighbor);
                neighbor->visited = true;
            }
        }
    }

    double CalculateHValue(shared_ptr<CellData> cell)
    {
        return cell->distanceTo(_task.first);
    }

    void constructFoundPath(shared_ptr<CellData> cell){
        while (cell->getParentCell() != nullptr)
        {
            _robot->appendCellToPath(cell,cell->getTimeStamp(),_robot->getParkingCell()->getTimeStamp());
            cell = cell->getParentCell();
        }

    }

private:
    pair<shared_ptr<CellData>, shared_ptr<CellData>> _task;
    std::mutex mtx;
    Map *mapHandler;
    vector<shared_ptr<CellData>> Q;
    shared_ptr<Robot> _robot=nullptr;
    int _t0;
};