#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "../../include/uneviedefourmi/Features/AntsBfs.h"

#include <unordered_set>

using namespace std;

AntBfs::AntBfs(AntHill &antHill) : antHill(antHill) {}

void AntBfs::simulate() {
    unordered_map<string, queue<int>> roomQueues;
    unordered_map<string, int> roomOccupancy;
    unordered_map<int, unordered_set<string>> visitedRooms;
    queue<pair<int, string>> ants;

    for (const auto &[room, capacity] : antHill.getRooms()) {
        roomOccupancy[room] = 0;
    }
    for (int i = 1; i <= antHill.getNumAnts(); ++i) {
        ants.push({i, "Sv"});
        roomOccupancy["Sv"]++;
        visitedRooms[i].insert("Sv");
    }

    int timeStep = 0;


    while (!ants.empty()) {
        int size = ants.size();

        for (int i = 0; i < size; ++i) {
            auto [antId, currentRoom] = ants.front();
            ants.pop();

            if (currentRoom == "Sd") {
                cout << "Fourmi " << antId << " est arrivée dans Sd !" << endl;
                continue;
            }

            bool moved = false;
            for (const auto &[room1, room2] : antHill.getConnections()) {
                string nextRoom;
                if (room1 == currentRoom) nextRoom = room2;
                else if (room2 == currentRoom) nextRoom = room1;
                else continue;

                if (visitedRooms[antId].count(nextRoom)) {
                    continue;
                }
                int capacity = antHill.getRoomCapacity(nextRoom);

                if (nextRoom == "Sd" || roomOccupancy[nextRoom] < capacity ) {
                if (currentRoom != "Sd" && roomOccupancy[currentRoom] > 0) {
                    roomOccupancy[currentRoom]--;
                }
                roomOccupancy[nextRoom]++;
                ants.push({antId, nextRoom});
                    visitedRooms[antId].insert(nextRoom);
                moved = true;
                cout << "Fourmi " << antId << " avance de " << currentRoom << " vers " << nextRoom << endl;
                break;
                }
            }

            if (!moved) {
                cout << "Fourmi " << antId << " est bloquée dans " << currentRoom << endl;
                ants.push({antId, currentRoom});
            }
        }
        timeStep++;
        cout << "--- Fin du tour " << timeStep << " ---\n";
    }
    cout << "Simulation terminée : toutes les fourmis ont atteint Sd !" << endl;
}
