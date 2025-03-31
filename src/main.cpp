#include "../include/uneviedefourmi/Features/AntHill.h"
#include "../include/uneviedefourmi/Features/AntsDfs.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
using namespace std;

int main() {
    AntHill antHill;
    antHill.loadFromFile("../../assets/fourmiliere_5.txt");
    antHill.display();

    int numAnts = antHill.getNumAnts();
    unordered_map<string, vector<string>> graph;
    unordered_map<string, int> roomCapacity;

    for (const auto& room : antHill.getRooms()) {
        roomCapacity[room.first] = room.second;
    }

    for (const auto& conn : antHill.getConnections()) {
        graph[conn.first].push_back(conn.second);
        graph[conn.second].push_back(conn.first);
    }

    vector<Ants> ants;
    for (int i = 0; i < numAnts; i++) {
        ants.emplace_back(i + 1, "Sv");
    }

    int step = 1;
    cout << "Début du déplacement des fourmis..." << endl;
    for (auto& ant : ants) {
        ant.moveDFS(graph, roomCapacity, step++);
    }

    Ants::displayFinalStats();
    cout << "\nToutes les fourmis ont terminé leur déplacement." << endl;
    return 0;


}
