#include "../../include/uneviedefourmi/Features/Ants.h"
#include <iostream>
#include <stack>
#include <chrono>
#include <unordered_map>
#include <vector>
using namespace std;

static vector<string> optimalPath; // Stocke le chemin optimal trouvé par la première fourmi
static unordered_map<string, int> roomOccupancy; // Suivi de l'occupation des salles
static vector<Ants*> pendingAnts; // Liste des fourmis en attente d'entrer dans une salle

Ants::Ants(int id, const string& startRoom) : id(id), currentRoom(startRoom), reachedSd(false) {
    cout << "Fourmi " << id << " créée dans " << startRoom << "!" << endl;
    roomOccupancy[startRoom]++; // Augmenter l'occupation de la salle de départ
}

void Ants::moveDFS(const unordered_map<string, vector<string>>& graph, unordered_map<string, int>& roomCapacity, int step) {
    auto startTime = chrono::high_resolution_clock::now();
    cout << "\n--------- Étape " << step << " ---------" << endl;

    if (!optimalPath.empty()) { // Si un chemin optimal a déjà été trouvé
        for (const auto& room : optimalPath) {
            if (roomOccupancy[currentRoom] > 0) {
                roomOccupancy[currentRoom]--; // Libérer l'ancienne salle
            }

            if (roomOccupancy[room] < roomCapacity[room]) { // Vérifie si la salle a encore de la place
                roomOccupancy[room]++; // Augmenter l'occupation de la nouvelle salle
                currentRoom = room;
                cout << "Fourmi " << id << " déplacée vers " << room << " (" << roomOccupancy[room] << "/" << roomCapacity[room] << ")" << endl;

                if (room == "Sd") {
                    reachedSd = true; // Marquer comme arrivée uniquement si la fourmi atteint Sd
                    return;
                }
            } else {
                cout << "Fourmi " << id << " attend pour entrer dans " << room << " (Capacité max atteinte)" << endl;
                pendingAnts.push_back(this); // Ajouter à la liste des fourmis en attente
                return;
            }
        }
    } else { // Première fourmi explore le chemin optimal
        stack<string> stack;
        unordered_map<string, string> cameFrom;
        stack.push(currentRoom);
        cameFrom[currentRoom] = "";

        vector<string> visitedRooms;
        bool found = false;

        while (!stack.empty() && !found) {
            string room = stack.top();
            stack.pop();
            visitedRooms.push_back(room);
            cout << "Fourmi " << id << " visite " << room << endl;

            for (const string& neighbor : graph.at(room)) {
                if (cameFrom.find(neighbor) == cameFrom.end()) { // Si la salle n'a pas encore été visitée
                    cameFrom[neighbor] = room;
                    stack.push(neighbor);

                    if (neighbor == "Sd") { // Si on a trouvé la sortie
                        found = true;
                        break;
                    }
                }
            }
        }

        if (found) {
            string room = "Sd";
            while (!room.empty()) {
                optimalPath.insert(optimalPath.begin(), room);
                room = cameFrom[room];
            }
            cout << "Chemin optimal trouvé par la première fourmi : ";
            for (const auto& r : optimalPath) {
                cout << r << " -> ";
            }
            cout << "FIN" << endl;
        }
    }

    // Vérifier si des fourmis en attente peuvent avancer maintenant
    vector<Ants*> remainingAnts;
    for (auto* ant : pendingAnts) {
        if (roomOccupancy[ant->currentRoom] < roomCapacity[ant->currentRoom]) {
            roomOccupancy[ant->currentRoom]++;
            cout << "Fourmi " << ant->id << " peut maintenant entrer dans " << ant->currentRoom << " (" << roomOccupancy[ant->currentRoom] << "/" << roomCapacity[ant->currentRoom] << ")" << endl;
            ant->moveDFS(graph, roomCapacity, step + 1);
        } else {
            remainingAnts.push_back(ant); // La fourmi doit encore attendre
        }
    }
    pendingAnts = remainingAnts;

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;
    cout << "Temps d'exécution pour la fourmi " << id << " : " << duration.count() << " secondes" << endl;
}