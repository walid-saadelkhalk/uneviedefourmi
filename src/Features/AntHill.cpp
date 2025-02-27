#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "../../include/uneviedefourmi/Features/AntHill.h"

using namespace std;

AntHill::AntHill() {
}

void AntHill::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Impossible d'ouvrir " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);

        if (line[0] == 'f') {
            iss.ignore(2);
            iss >> numAnts;
        }

        else if (line.find('-') != string::npos) {
            string room1, room2;
            size_t pos = line.find('-');

            if (pos != string::npos) {
                room1 = line.substr(0, pos - 1);
                room2 = line.substr(pos + 2);
                connections.emplace_back(room1, room2);
            }
        }

        else if (isalpha(line[0])) {
            string room;
            int capacity = 1;

            size_t bracePos = line.find('{');
            if (bracePos != string::npos) {
                room = line.substr(0, bracePos - 1);
                capacity = stoi(line.substr(bracePos + 2));
            } else {
                room = line;
            }
            rooms[room] = capacity;
        }
    }

    file.close();
}

void AntHill::display() const {
    cout << "Nombre de fourmis: " << numAnts << endl;
    cout << "Salles et capacites: " << endl;
    for (const auto &[room, capacity] : rooms) {
        cout << "- " << room << " (Capacite: " << capacity << ")\n";
    }

    cout << "Connexions: " << endl;
    for (const auto &[room1, room2] : connections) {
        cout << "- " << room1 << " <--> " << room2 << endl;
    }
}

int AntHill::getRoomCapacity(const string &room) const {
    auto it = rooms.find(room);
    // cout << "🔎 getRoomCapacity() demande : " << room << " -> trouvé : " << it->second << endl;
    if (it != rooms.end()) {
        return it->second;
    }
    return 1;
}



