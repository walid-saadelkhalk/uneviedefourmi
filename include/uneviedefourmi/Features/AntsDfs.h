#ifndef ANTS_H
#define ANTS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
using namespace std;

class Ants {
private:
    string currentRoom;
    bool reachedSd;
    int id;

public:
    Ants(int id, const string& startRoom);
    void moveDFS(const unordered_map<string, vector<string>>& graph, unordered_map<string, int>& roomCapacity, int step);
    static void displayFinalStats();
    string getCurrentRoom() const;
    bool hasReachedSd() const;
    int getId() const;
};

#endif // ANTS_H
