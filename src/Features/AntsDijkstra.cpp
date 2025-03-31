#include "../../include/uneviedefourmi/Features/AntsDijkstra.h"



#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

AntsDijkstra::AntsDijkstra() : numAnts(0) {}

AntsDijkstra::~AntsDijkstra() {
    for (auto &[name, room] : rooms) {
        delete room;
    }
}

void AntsDijkstra::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur : Impossible d'ouvrir " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        line.erase(remove(line.begin(), line.end(), '\r'), line.end()); 

        istringstream iss(line);

        if (line[0] == 'f') { 
            iss.ignore(2);
            iss >> numAnts;
            cout << "Nombre de fourmis: " << numAnts << endl;
        }
        else if (line.find('-') != string::npos) { 
            size_t pos = line.find('-');
            string room1 = line.substr(0, pos);
            string room2 = line.substr(pos + 1);

            room1.erase(room1.find_last_not_of(" \t\r\n") + 1);
            room2.erase(0, room2.find_first_not_of(" \t\r\n"));

            if (room1.empty() || room2.empty()) {
                cerr << "Erreur : lien mal formé -> " << line << endl;
                continue;
            }

            if (rooms.find(room1) == rooms.end()) rooms[room1] = new Room(room1);
            if (rooms.find(room2) == rooms.end()) rooms[room2] = new Room(room2);

            rooms[room1]->neighbors.emplace_back(rooms[room2], 1);
            rooms[room2]->neighbors.emplace_back(rooms[room1], 1);

            cout << "Ajout connexion : " << room1 << " <-> " << room2 << endl;
        }
        else { 
            string room;
            int capacity = 1;
            size_t bracePos = line.find('{');
            if (bracePos != string::npos) {
                room = line.substr(0, bracePos);
                capacity = stoi(line.substr(bracePos + 1));
            } else {
                room = line;
            }

            room.erase(room.find_last_not_of(" \t\r\n") + 1);
            room.erase(0, room.find_first_not_of(" \t\r\n"));

            if (room.empty()) {
                cerr << "Erreur : nom de salle vide -> " << line << endl;
                continue;
            }

            rooms[room] = new Room(room, capacity);
            cout << "Ajout salle : " << room << " (capacité: " << capacity << ")" << endl;
        }
    }

    file.close();
}

void AntsDijkstra::display() const {
    cout << "Nombre de fourmis: " << numAnts << endl;
    cout << "Salles (" << rooms.size() << "):" << endl;
    for (const auto &[name, room] : rooms) {
        cout << " - " << name << " (cap: " << room->capacity << ") -> ";
        for (const auto &[neighbor, weight] : room->neighbors) {
            cout << neighbor->name << "(" << weight << ") ";
        }
        cout << endl;
    }
}

vector<string> AntsDijkstra::findShortestPath(const string &start, const string &end) {
    if (rooms.find(start) == rooms.end() || rooms.find(end) == rooms.end()) {
        cerr << "Erreur : Une des salles n'existe pas !" << endl;
        return {};
    }

    unordered_map<string, int> distances;
    unordered_map<string, string> predecessors;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

    for (const auto &[name, _] : rooms) {
        distances[name] = numeric_limits<int>::max();
    }
    distances[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();

        if (current == end) break;
        if (dist > distances[current]) continue;

        for (const auto &[neighbor, weight] : rooms[current]->neighbors) {
            int newDist = dist + weight;
            if (newDist < distances[neighbor->name]) {
                distances[neighbor->name] = newDist;
                predecessors[neighbor->name] = current;
                pq.emplace(newDist, neighbor->name);
            }
        }
    }

    if (distances[end] == numeric_limits<int>::max()) {
        return {};
    }

    vector<string> path;
    string at = end;
    while (at != start) {
        path.push_back(at);
        at = predecessors[at];
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

void AntsDijkstra::run(const string &filename, const string &start, const string &end) {
    loadFromFile(filename);
    display();

    cout << "\nRecherche du plus court chemin entre " << start << " et " << end << endl;
    vector<string> path = findShortestPath(start, end);

    if (path.empty()) {
        cout << "Aucun chemin trouvé entre " << start << " et " << end << endl;
    } else {
        cout << "Chemin trouvé : ";
        for (const auto &room : path) {
            cout << room << " -> ";
        }
        cout << "Arrivé !" << endl;
    }
}
