#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../../include/uneviedefourmi/Features/AntsBfs.h"

using namespace std;

AntBfs::AntBfs(AntHill &antHill) : antHill(antHill) {}

void AntBfs::simulate() {
    unordered_map<string, int> roomOccupancy;
    unordered_map<string, vector<string>> adjacencyList;
    unordered_map<int, unordered_map<string, int>> visitedRooms;
    unordered_map<int, int> stuckTime; // Pour compter le temps où une fourmi est bloquée dans une salle
    queue<pair<int, string>> ants;

    // Construire la liste d'adjacence
    for (const auto &[room1, room2] : antHill.getConnections()) {
        adjacencyList[room1].push_back(room2);
        adjacencyList[room2].push_back(room1);
    }

    // Initialisation des salles et des fourmis
    for (const auto &[room, capacity] : antHill.getRooms()) {
        roomOccupancy[room] = 0;
    }
    for (int i = 1; i <= antHill.getNumAnts(); ++i) {
        ants.push({i, "Sv"}); // Ajoute les fourmis dans le vestibule initialement
        roomOccupancy["Sv"]++;
        visitedRooms[i]["Sv"] = 1;
    }

    int timeStep = 0;
    int stuckThreshold = 5; // Nombre de tours avant qu'une fourmi soit considérée comme bloquée

    while (!ants.empty()) {
        int size = ants.size();
        bool anyMoved = false;

        for (int i = 0; i < size; ++i) {
            auto [antId, currentRoom] = ants.front();
            ants.pop();

            if (currentRoom == "Sd") {
                cout << "Fourmi " << antId << " est arrivée dans Sd !" << endl;
                continue;
            }

            bool moved = false;
            vector<string> unvisitedNeighbors; // Pour stocker les voisins non visités

            // Essayer de déplacer la fourmi vers une salle voisine
            for (const auto &nextRoom : adjacencyList[currentRoom]) {
                int capacity = antHill.getRoomCapacity(nextRoom);

                // Si la salle est déjà visitée trop souvent, on l'évite
                if (visitedRooms[antId][nextRoom] > 1) continue;

                // Si la salle suivante n'est pas pleine, la fourmi peut avancer
                if (nextRoom == "Sd" || roomOccupancy[nextRoom] < capacity) {
                    // Mise à jour des salles
                    roomOccupancy[currentRoom]--;
                    roomOccupancy[nextRoom]++;
                    visitedRooms[antId][nextRoom]++;

                    cout << "Fourmi " << antId << " avance de " << currentRoom << " vers " << nextRoom << endl;
                    ants.push({antId, nextRoom}); // Ajouter la fourmi à la file pour qu'elle explore
                    moved = true;
                    anyMoved = true;
                    break;  // Sortir dès qu'une fourmi a bougé
                }

                // Si la salle est pleine ou bloquée, on l'ajoute à la liste de tentatives
                unvisitedNeighbors.push_back(nextRoom);
            }

            // Si aucune salle n'est accessible, la fourmi attend
            if (!moved && !unvisitedNeighbors.empty()) {
                cout << "Fourmi " << antId << " reste bloquée dans " << currentRoom << endl;

                // Incrémenter le compteur de tours bloqués
                stuckTime[antId]++;

                // Si la fourmi reste bloquée trop longtemps, on l'aide à revenir en arrière
                if (stuckTime[antId] >= stuckThreshold) {
                    // Vérifier si elle peut essayer de revenir dans une salle précédente
                    bool hasBacktracked = false;
                    for (const auto &prevRoom : adjacencyList[currentRoom]) {
                        if (visitedRooms[antId][prevRoom] == 1) { // Si la salle a été visitée une seule fois
                            ants.push({antId, prevRoom}); // La fourmi revient en arrière
                            visitedRooms[antId][prevRoom]++;
                            stuckTime[antId] = 0;  // Réinitialiser le compteur de temps bloqué
                            hasBacktracked = true;
                            break;
                        }
                    }

                    // Si la fourmi n'a pas pu revenir en arrière, on la garde dans la même salle pour le tour suivant
                    if (!hasBacktracked) {
                        ants.push({antId, currentRoom});
                    }
                } else {
                    // Si la fourmi est encore bloquée, on la garde dans la même salle
                    ants.push({antId, currentRoom});
                }
            }
        }

        // Si aucune fourmi n'a bougé pendant un tour entier, il peut y avoir une impasse
        if (!anyMoved) {
            cout << "Aucune fourmi n'a bougé pendant ce tour. La simulation est arrêtée." << endl;
            break;
        }

        timeStep++;
        cout << "--- Fin du tour " << timeStep << " ---\n";
    }

    cout << "Simulation terminée : toutes les fourmis ont atteint Sd !" << endl;
}
