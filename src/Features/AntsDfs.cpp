#include "../../include/uneviedefourmi/Features/AntsDfs.h"
#include <iostream>
#include <stack>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <regex>
using namespace std;

static vector<string> optimalPath; // Stocke le chemin optimal trouvé par la première fourmi
static unordered_map<string, int> roomOccupancy; // Suivi de l'occupation des salles
static vector<Ants*> pendingAnts; // Liste des fourmis en attente d'entrer dans une salle
static vector<double> executionTimes;

Ants::Ants(int id, const string& startRoom) : id(id), currentRoom(startRoom), reachedSd(false) {
    cout << "Fourmi " << id << " créée dans " << startRoom << "!" << endl;
    roomOccupancy[startRoom]++; // Augmenter l'occupation de la salle de départ
}

void Ants::moveDFS(const unordered_map<string, vector<string>>& graph,
                   unordered_map<string, int>& roomCapacity,
                   int step)
{
    auto startTime = chrono::high_resolution_clock::now();
    cout << "\n--------- Étape " << step << " ---------" << endl;

    // CAS 1: Un chemin optimal existe déjà (fourmi n°2, n°3, etc.)
    if (!optimalPath.empty()) {
        for (const auto& room : optimalPath) {
            // Libère la salle actuelle
            if (roomOccupancy[currentRoom] > 0) {
                roomOccupancy[currentRoom]--;
            }

            // Vérifie la capacité de la salle vers laquelle on va
            if (roomOccupancy[room] < roomCapacity[room]) {
                roomOccupancy[room]++;
                currentRoom = room;

                cout << "Fourmi " << id << " déplacée vers " << room
                     << " (" << roomOccupancy[room] << "/"
                     << roomCapacity[room] << ")" << endl;

                // Arrêt si on arrive à Sd
                if (room == "Sd") {
                    reachedSd = true;
                    break;
                }
            } else {
                // Salle pleine -> on attend
                cout << "Fourmi " << id << " attend pour entrer dans "
                     << room << " (Capacité max atteinte)" << endl;
                pendingAnts.push_back(this);
                // On arrête pour le moment
                auto endTime = chrono::high_resolution_clock::now();
                chrono::duration<double> duration = endTime - startTime;
                executionTimes.push_back(duration.count());
                return;
            }
        }
    }
    // CAS 2: AUCUN chemin optimal n’existe encore (fourmi n°1)
    else {
        // --- Recherche du chemin par DFS ---
        stack<string> stack;
        unordered_map<string, string> cameFrom;
        stack.push(currentRoom);
        cameFrom[currentRoom] = "";

        bool found = false;

        while (!stack.empty() && !found) {
            string room = stack.top();
            stack.pop();

            cout << "Fourmi " << id << " visite " << room << endl;

            // Parcourir les voisins
            for (const string& neighbor : graph.at(room)) {
                if (cameFrom.find(neighbor) == cameFrom.end()) {
                    cameFrom[neighbor] = room;
                    stack.push(neighbor);

                    if (neighbor == "Sd") {
                        found = true;
                        break;
                    }
                }
            }
        }

        // --- Reconstruction du chemin si Sd trouvé ---
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

            // --- Faire AVANCER la première fourmi selon le chemin ---
            for (const auto& r : optimalPath) {
                // Libère la salle actuelle
                if (roomOccupancy[currentRoom] > 0) {
                    roomOccupancy[currentRoom]--;
                }

                // Vérifie la capacité de la salle suivante
                if (roomOccupancy[r] < roomCapacity[r]) {
                    roomOccupancy[r]++;
                    currentRoom = r;

                    cout << "Fourmi " << id << " déplacée vers " << r
                         << " (" << roomOccupancy[r] << "/"
                         << roomCapacity[r] << ")" << endl;

                    // Arrêt si on arrive à Sd
                    if (r == "Sd") {
                        reachedSd = true;
                        break;
                    }
                } else {
                    cout << "Fourmi " << id << " attend pour entrer dans "
                         << r << " (Capacité max atteinte)" << endl;
                    pendingAnts.push_back(this);

                    auto endTime = chrono::high_resolution_clock::now();
                    chrono::duration<double> duration = endTime - startTime;
                    executionTimes.push_back(duration.count());
                    return;
                }
            }
        }
        // Si on n'a pas trouvé Sd, le "found" reste faux, donc aucune
        // mise à jour d'optimalPath, ni de salles (cas improbable).
    }

    // --- Vérifier si des fourmis en attente peuvent avancer ---
    vector<Ants*> remainingAnts;
    for (auto* ant : pendingAnts) {
        // Si la salle où elle veut entrer est dispo
        if (roomOccupancy[ant->currentRoom] < roomCapacity[ant->currentRoom]) {
            roomOccupancy[ant->currentRoom]++;
            cout << "Fourmi " << ant->id << " peut maintenant entrer dans "
                 << ant->currentRoom << " ("
                 << roomOccupancy[ant->currentRoom] << "/"
                 << roomCapacity[ant->currentRoom] << ")" << endl;

            // On relance moveDFS pour cette fourmi en attente
            ant->moveDFS(graph, roomCapacity, step + 1);
        } else {
            remainingAnts.push_back(ant);
        }
    }
    pendingAnts = remainingAnts;

    // --- Calcul et enregistrement du temps d'exécution ---
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;
    executionTimes.push_back(duration.count());

    cout << "Temps d'exécution pour la fourmi " << id << " : "
         << duration.count() << " secondes" << endl;
}

void Ants::displayFinalStats() {
    int numArrived = roomOccupancy["Sd"];
    double totalTime = 0;
    for (double time : executionTimes) {
        totalTime += time;
    }
    cout << "\n===== Statistiques Finales =====" << endl;
    cout << "Nombre de fourmis arrivées en Sd : " << numArrived << endl;
    cout << "Temps total de déplacement : " << totalTime << " secondes" << endl;
}