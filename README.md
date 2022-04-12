# Multi-Agent-A-Star
A C++ implantation for Multi-Agent A* Algorithm for Multi Agent Path planning.
<p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/firstTestRuning.gif" />
</p>

# Overview
This is the capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). In this project, I implemented a multi-agent A* path planner and an example application of the algorithm in an order fulfillment center. The puzzle is as follows;
* In a warehouse, A fleet of robots has to fulfill a pool of tasks, the problem is solved when all tasks are fulfilled. 
* The robots should not collide with each other nor should they collide with an obstacle in the warehouse while navigating to their task.
According to [1] "A collision occurs if two agents occupy the same location at
the same timestep (called a vertex conflict) or traverse the same edge in opposite directions at
the same timestep (called a swapping conflict)" 


The problem components are illustrated in the figure below 
<p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/problemDefinition.png" />
</p>

Assumptions have been made to simplify the problem 
* The space (the map of the warehouse) is discretized into equal size cells 
* Time is discretized into time steps. At each timestep, a robot can move to a neighboring cell on the map
* All robots should move at the same speed

# Compile and build the project
 ```
 mkdir build
 cd build 
 cmake ..
 make
 ```

# Collsiion Tests 
To validate if the algorithm is avoiding collisions two tests have been developed.To run the tests 
```
 ./test/collisionTest 
```

and the console will show that the tests are passing 
```
[==========] 2 tests from 1 test suite ran. (16328 ms total)
[  PASSED  ] 2 tests.
```

The first test examines what is called a swapping conflict, which is when two robots swap their locations. Or as expressed mathematically in [2] 

 P<sub>a</sub>(t) = P<sub>b</sub> (t+1) AND P<sub>b</sub>(t) = P<sub>a</sub> (t+1)
 
where *a* and *b* are distinct agents.

This collision case can be simulated if we comment out line 68 in Planner.hpp
 ```
 //&& !neighbor->isReserverd(currentCell->getTimeStamp())
 ```
 build the project and run the tests 
 ```
 ./test/collisionTest 
 ```
 you should be able to see the robots colliding as the animation shows below
 <p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/swapCellsTestFailed.gif" />
</p>


and the console will show that the test is failing 
```
[==========] 2 tests from 1 test suite ran. (14300 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CollisionTest.SwappingCellsTest
```

Now uncomment line 68 in Planner.hpp
 ```
 && !neighbor->isReserverd(currentCell->getTimeStamp())
 ```
 build the project and run the tests 
 
you should be able to see the robots are avoiding the collision as the animation shows below.

 <p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/firstTestRuning.gif" />
</p>


\
The second test examines what is called a vertex conflict, which is when two robots occupy the same location. Or as expressed mathematically in [2] 

 P<sub>a</sub>(t) = P<sub>b</sub> (t)
 
This collision case can be simulated if we comment out line 67 in Planner.hpp
 ```
 //&& !neighbor->isReserverd(NexttimeStamp)
 ```
 build the project and run the tests 

 you should be able to see the robots colliding as the animation shows below
 <p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/occupySameLocationTestFailing.gif" />
</p>

and the console will show that the second test is failing 
```
[==========] 2 tests from 1 test suite ran. (14278 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CollisionTest.OccupyingSameCell
```
Now uncomment line 68 in Planner.hpp
 ```
 && !neighbor->isReserverd(NexttimeStamp)
 ```
 build the project and run the tests 
 
you should be able to see the robots are avoiding the collision as the animation shows below
 <p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/occupySameLocationTestPassing.gif" />
</p>



# Warehouse Demo
Now that we have validated the Multi-Agent A* planner, we can utilize it in an order fulfillment center scenario, where we have a queue of tasks and a queue of robots, and the demo shows how the robots can fulfill all the tasks without colliding with each other or with the shelves in the warehouse.
run the demo 
```
cd build
./MAA-Star 
```
You should be able to see this on the screen below (this gif is 3X the actual speed, the robots in the demo are moving at a velocity of 1 cell/sec)
<p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/warehouseDemo3X.gif" />
</p>

## code structure
To figure below explains how the code components of the demo are connected together 
<p align="center">
  <img src="https://github.com/AlaaAlassi/CppND-Capstone-Multi-Agent-A-Start/blob/main/assets/Queue.drawio.png" />
</p>

* ```CellData.hpp/CellData.cpp``` CellData is a struct that represents the cells of the grid of the warehouse, and it stores the information about the cell that the planner needs 
such as the index, Cartesian position, and the value of the cell {emptey,occupied,delivery,pickup}
* ```Map.hpp``` The Map Class is composed of a vector of Objects of type CellData
* ```Warehouse.hpp/Warehouse.cpp``` The Warehouse is a Class that is used to generates the map.
* ```Robot.hpp``` The Robots (the colored circles) are objects of type Robot (class),  each object owns a shared pointer to the cellData object that the robot is parking at. In addition, it owns a queue of shared pointers of DataCell objects that represents the planned path. 
* ```Planner.hpp``` The multiAgentPlanner is an object of type planner (class). It needs a robot instance, task and pointer to the Map to plan a path for the robot. 
* ```GenericQueue.hpp``` The robots in the demo are circulating between the planning thread and the executing thread until all tasks in the queue are fulfilled. The planning thread blocks until a robot is available in the queue of the available robots, this queue is of type GenericQueue. 
* ```Graphics.hpp/Graphics.cpp``` The viewer is an object of type Graphics (class).  This viewer owns a shared pointer to the map and to all the instances of Robot Class. The viewer runs in parallel in the simulation thread.

## Remarks
* The algorthim is a simplified version of the multi-label A* algorithm mentioned in [2]
* The code is designed to be extensible, that is why the task object is defined as std::pair, because ideally, the robot should first navigate to a Pickup cell and then to a delivery cell or vice versa. However, I kept this feature outside the scope of this Capstone project.
 

# Rubric

__README (All Rubric Points REQUIRED)__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | A README with instructions is included with the project |The README is included with the project and has instructions for building/running the project. If any additional libraries are needed to run the project, these are indicated with cross-platform installation instructions. You can submit your writeup as markdown or pdf.| |
| :heavy_check_mark: | The README indicates which project is chosen. | The README describes the project you have built. The README also indicates the file and class structure, along with the expected behavior or output of the program. | |
| :heavy_check_mark: | The README includes information about each rubric point addressed. | The README indicates which rubric points are addressed. The README also indicates where in the code (i.e. files and line numbers) that the rubric points are addressed. | |

__Compiling and Testing (All Rubric Points REQUIRED)__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The submission must compile and run. | The project code must compile and run without errors. We strongly recommend using cmake and make, as provided in the starter repos. If you choose another build system, the code must compile on any reviewer platform. |

__Loops, Functions, I/O__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The project demonstrates an understanding of C++ functions and control structures.| A variety of control structures are used in the project. The project code is clearly organized into functions.| Every *.cpp file |
|  | The project reads data from a file and process the data, or the program writes data to a file. | The project reads data from an external file or writes data to a file as part of the necessary operation of the program.| |
| | The project accepts user input and processes the input.|The project accepts input from a user as part of the necessary operation of the program.|  |

__Object Oriented Programming__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The project uses Object Oriented Programming techniques. | The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. | All *.cpp and *.h files |
| :heavy_check_mark: | Classes use appropriate access specifiers for class members. | All class data members are explicitly specified as public, protected, or private.|Map Class Robot Class and CellData struct|
| :heavy_check_mark: | Class constructors utilize member initialization lists. | All class members that are set to argument values are initialized through member initialization lists.| Map Class Robot Class and CellData struct|
| | Classes abstract implementation details from their interfaces. | All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.| |
|  | Classes encapsulate behavior. | Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.| |
| | Classes follow an appropriate inheritance hierarchy. | Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.|  |
|  | Overloaded functions allow the same function to operate on different parameters. |  |
|  | Derived class functions override virtual base class functions. |One member function in an inherited class overrides a virtual base class member function.| |
| :heavy_check_mark: | Templates generalize functions in the project. | One function is declared with a template that allows it to accept a generic parameter.| GenericQueue.hpp|

__Memory Management__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| | The project makes use of references in function declarations. | At least two variables are defined as references, or two functions use pass-by-reference in the project code.||
| | The project uses destructors appropriately. | At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor. | |
| | The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. | The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.| |
| | The project follows the Rule of 5. | For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.| |
| :heavy_check_mark: | The project uses move semantics to move data, instead of copying it, where possible. | For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object. |main.cpp and GenericQueue Class |
| :heavy_check_mark: | The project uses smart pointers instead of raw pointers. | The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.| used in main.cpp, CellData struct, Robot Class, Planner Class |

__Concurrency__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The project uses multithreading. | The project uses multiple threads in the execution.| main.cpp|
| | A promise and future is used in the project. | A promise and future is used to pass data from a worker thread to a parent thread in the project code.| |
| :heavy_check_mark: | A mutex or lock is used in the project. | A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.|main.cpp and Robot Class|
| :heavy_check_mark: | A condition variable is used in the project. | A std::condition_variable is used in the project code to synchronize thread execution.| GenericQueue.hpp |

# References 
[1] Li, Jiaoyang, et al. "Lifelong Multi-Agent Path Finding in Large-Scale Warehouses." AAMAS. 2020.\
[2] Grenouilleau, Florian, Willem-Jan van Hoeve, and John N. Hooker. "A multi-label A* algorithm for multi-agent pathfinding." Proceedings of the International Conference on Automated Planning and Scheduling. Vol. 29. 2019.
