#ifndef ANTSDIJKSTRA_H
#define ANTSDIJKSTRA_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>
#include <queue>
#include <string>

struct Room {
    std::string name;
    int capacity;
    std::vector<std::pair<Room*, int>> neighbors;

    Room(std::string n, int c = 1) : name(n), capacity(c) {}
};

class AntsDijkstra {
private:
    std::unordered_map<std::string, Room*> rooms;
    int numAnts;

public:
    AntsDijkstra();  
    ~AntsDijkstra(); 

    void loadFromFile(const std::string &filename);
    void display() const;  
    std::vector<std::string> findShortestPath(const std::string &start, const std::string &end);
    void run(const std::string &filename, const std::string &start, const std::string &end);
};

#endif // ANTSDIJKSTRA_H