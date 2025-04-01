#include <iostream>
#include <queue>
#include <unordered_map>
#include <chrono>
#include <vector>
#include "../../include/uneviedefourmi/Features/AntsBfs.h"

using namespace std;

AntBfs::AntBfs(AntHill &antHill) : antHill(antHill) {}

/*
===============================================================================
 Algorithme de recherche en largeur (BFS) appliqué aux fourmis
----------------------------------------
 STRUCT:
----------------------------------------
- `queue<pair<int, string>> ants;`
  --> File FIFO contenant les fourmis sous la forme (ID, Salle actuelle).

- `unordered_map<string, vector<string>> adjacencyList;`
  --> Liste d'adjacence représentant les connexions entre les salles.

- `unordered_map<string, int> roomOccupancy;`
  --> Stocke le nombre de fourmis présentes dans chaque salle.

- `unordered_map<int, unordered_map<string, int>> visitedRooms;`
  --> Garde une trace des salles visitées par chaque fourmi pour éviter les boucles infinies.

- `unordered_map<int, int> stuckTime;`
  --> Enregistre combien de tours une fourmi est restée bloquée.

----------------------------------------
SIMULATION:
   - À chaque tour, chaque fourmi tente d'avancer vers une salle voisine non pleine.
   - Si une fourmi atteint "Sd", elle est retirée du processus.
   - Si une fourmi ne peut pas avancer, elle attend ou tente de revenir en arrière après 5 tours.
   - Si aucune fourmi ne bouge pendant un tour complet, la simulation s'arrête.
===============================================================================
*/


void AntBfs::simulate() {
    // Chrono du temps global
    auto startSimulation = chrono::high_resolution_clock::now();

    unordered_map<string, int> roomOccupancy;
    unordered_map<string, vector<string>> adjacencyList;
    unordered_map<int, unordered_map<string, int>> visitedRooms;
    unordered_map<int, int> stuckTime;
    queue<pair<int, string>> ants;

    for (const auto &[room1, room2] : antHill.getConnections()) {
        adjacencyList[room1].push_back(room2);
        adjacencyList[room2].push_back(room1);
    }

    for (const auto &[room, capacity] : antHill.getRooms()) {
        roomOccupancy[room] = 0;
    }
    for (int i = 1; i <= antHill.getNumAnts(); ++i) {
        ants.push({i, "Sv"});
        roomOccupancy["Sv"]++;
        visitedRooms[i]["Sv"] = 1;
    }

    int timeStep = 1;
    int stuckThreshold = 5;

    while (!ants.empty()) {
        // chrono pour le tour
        auto startTurn = chrono::high_resolution_clock::now();

        int size = ants.size();
        bool anyMoved = false;

        for (int i = 0; i < size; ++i) {
            auto [antId, currentRoom] = ants.front();
            ants.pop();

            if (currentRoom == "Sd") {
                cout << "Fourmi " << antId << " est arrivee dans Sd !" << endl;
                continue;
            }

            bool moved = false;
            vector<string> unvisitedNeighbors;

            for (const auto &nextRoom : adjacencyList[currentRoom]) {
                int capacity = antHill.getRoomCapacity(nextRoom);

                if (visitedRooms[antId][nextRoom] > 1) continue;

                if (nextRoom == "Sd" || roomOccupancy[nextRoom] < capacity) {
                    roomOccupancy[currentRoom]--;
                    roomOccupancy[nextRoom]++;
                    visitedRooms[antId][nextRoom]++;

                    cout << "Fourmi " << antId << " avance de " << currentRoom << " vers " << nextRoom << endl;
                    ants.push({antId, nextRoom});
                    moved = true;
                    anyMoved = true;
                    break;
                }

                unvisitedNeighbors.push_back(nextRoom);
            }


            if (!moved && !unvisitedNeighbors.empty()) {
                cout << "Fourmi " << antId << " reste bloquee dans " << currentRoom << endl;

                stuckTime[antId]++;

                if (stuckTime[antId] >= stuckThreshold) {
                    bool hasBacktracked = false;
                    for (const auto &prevRoom : adjacencyList[currentRoom]) {
                        if (visitedRooms[antId][prevRoom] == 2) {
                            ants.push({antId, prevRoom});
                            visitedRooms[antId][prevRoom]++;
                            stuckTime[antId] = 0;
                            hasBacktracked = true;
                            break;
                        }
                    }

                    if (!hasBacktracked) {
                        ants.push({antId, currentRoom});
                    }
                } else {
                    ants.push({antId, currentRoom});
                }
            }
        }


        auto endTurn = chrono::high_resolution_clock::now();
        chrono::duration<double> turnDuration = endTurn - startTurn;
        cout << "============================== Temps du tour " << timeStep << " : " << turnDuration.count() << " secondes.\n" << endl;

        if (!anyMoved) {
            cout << "Aucune fourmi n'a bouge pendant ce tour. La simulation est arretee." << endl;
            break;
        }

        timeStep++;
        cout << "--- Fin du tour " << timeStep << " ---\n";
    }

    auto endSimulation = chrono::high_resolution_clock::now();
    chrono::duration<double> simulationDuration = endSimulation - startSimulation;
    cout << "Temps total de la simulation : " << simulationDuration.count() << " secondes." << endl;

    cout << "\nSimulation terminee : toutes les fourmis ont atteint Sd !" << endl;
}
