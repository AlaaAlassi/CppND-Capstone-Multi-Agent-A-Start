#include "CellData.hpp"
#include "Robot.hpp"
#include "Map.hpp"
#include <deque>

using namespace std;

class Planner
{
    public:
    Planner(shared_ptr<Robot> robot, pair<shared_ptr<CellData>, shared_ptr<CellData>> task, Map *map) : _robot(robot), _task(task),mapHandler(map){};
    void planPath()
    {
        //create a random path to simulate a planner
        if (_robot->getID() == 1)
        {
            for (int j = 0; j < 20; j++)
            {
                std::lock_guard lg(mtx);
                _robot->appendCellToPath(mapHandler->_cells[j]);
            }
        }
        else if(_robot->getID() == 2)
        {
            for (int j = 0; j < 5; j++)
            {
                std::lock_guard lg(mtx);
                _robot->appendCellToPath(mapHandler->getCell(j, 1));
            }
        }
    };

private:
    pair<shared_ptr<CellData>, shared_ptr<CellData>>_task;
    shared_ptr<Robot> _robot;
    std::mutex mtx;
    Map * mapHandler;
};