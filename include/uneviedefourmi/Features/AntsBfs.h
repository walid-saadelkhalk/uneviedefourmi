#ifndef ANT_BFS_H
#define ANT_BFS_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "AntHill.h"

class AntBfs {
public:
    explicit AntBfs(AntHill &antHill);

    void simulate();

    // void simulateAnts();

private:
    AntHill &antHill;
};

#endif
