#include "../../include/uneviedefourmi/Features/Ants.h"
#include <iostream>
#include <stack>
#include <chrono>
#include <unordered_map>
using namespace std;

static vector<string> optimalPath; // Stocke le chemin optimal trouvé par la première fourmi

Ants::Ants(int id, const string& startRoom) : id(id), currentRoom(startRoom), reachedSd(false) {
    cout << "Fourmi " << id << " créée dans " << startRoom << "!" << endl;
}

void Ants::moveDFS(const unordered_map<string, vector<string>>& graph, unordered_map<string, int>& roomCapacity, int step) {
    auto startTime = chrono::high_resolution_clock::now();

    if (!optimalPath.empty()) { // Si un chemin optimal a déjà été trouvé, on le suit directement
        cout << "\n--------- Étape " << step << " ---------" << endl;
        cout << "Chemin optimal emprunté par les autres fourmis : ";
        for (const auto& room : optimalPath) {
            cout << room << " -> ";
            currentRoom = room;
            cout << "Fourmi " << id << " déplacée vers " << room << endl;
        }
        cout << "FIN" << endl;
        reachedSd = true;
    } else {
        stack<string> stack;
        unordered_map<string, string> cameFrom;
        stack.push(currentRoom);
        cameFrom[currentRoom] = "";

        cout << "\n--------- Étape " << step << " ---------" << endl;
        bool found = false;
        vector<string> visitedRooms;

        while (!stack.empty() && !found) {
            string room = stack.top();
            stack.pop();
            visitedRooms.push_back(room);
            cout << "Fourmi " << id << " visite " << room << endl;

            for (const string& neighbor : graph.at(room)) {
                if (cameFrom.find(neighbor) == cameFrom.end()) { // Si on n'a pas encore visité cette salle
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

        cout << "\nToutes les salles visitées par la première fourmi : ";
        for (const auto& room : visitedRooms) {
            cout << room << " -> ";
        }
        cout << "FIN" << endl;
    }

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;
    cout << "Temps d'exécution pour la fourmi " << id << " : " << duration.count() << " secondes" << endl;
}