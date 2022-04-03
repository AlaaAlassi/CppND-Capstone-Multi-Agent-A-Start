# Multi-Agent-A-Star
A C++ implantation for Multi-Label A* Algorithm for Multi Agent Path finding

![](https://github.com/AlaaAlassi/Multi-Label-A-Star/blob/finish-up/assets/firstTestRuning.gif)

# Problem Definition
A fleet of robots has to fulfill a pool of tasks, the problem is solved when all tasks are fulfilled.
![](https://github.com/AlaaAlassi/Multi-Label-A-Star/blob/finish-up/assets/problemDefinition.png)
## Constraints:
* The robots should not collide with each other while navigating to their tasks. "A
collision occurs iff two agents occupy the same location at
the same timestep (called a vertex conflict) or traverse the same edge in opposite directions at
the same timestep (called a swapping conflict)" [1]
* The robots should navigate to the task using the shortest path when possible.
## Assumtions:
* The space (the map) is sedcrtices into equall equal size cells. 
* Time is discretized into timesteps. At each timestep, a robot can move to a neighboring cell on the map. 


# References 
[1] Li, Jiaoyang, et al. "Lifelong Multi-Agent Path Finding in Large-Scale Warehouses." AAMAS. 2020.
