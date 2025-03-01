#include "../../include/uneviedefourmi/Features/AntHill.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

AntHill::AntHill() : numAnts(0) {}

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

                if (rooms.find(room1) == rooms.end()) {
                    rooms[room1] = 1;
                    roomNames.push_back(room1);
                }
                if (rooms.find(room2) == rooms.end()) {
                    rooms[room2] = 1;
                    roomNames.push_back(room2);
                }
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
            roomNames.push_back(room);
        }
    }
    file.close();
    constructLaplacianMatrix();
}

void AntHill::constructLaplacianMatrix() {
    int n = roomNames.size();
    laplacianMatrix.assign(n, vector<int>(n, 0));
    unordered_map<string, int> roomIndex;
    for (int i = 0; i < n; i++) {
        roomIndex[roomNames[i]] = i;
    }

    for (const auto &[room1, room2] : connections) {
        int i = roomIndex[room1];
        int j = roomIndex[room2];
        laplacianMatrix[i][i]++;
        laplacianMatrix[j][j]++;
        laplacianMatrix[i][j] = -1;
        laplacianMatrix[j][i] = -1;
    }
}

void AntHill::display() const {
    cout << "Nombre de fourmis: " << numAnts << endl;
    cout << "Salles et capacités: " << endl;
    for (const auto &[room, capacity] : rooms) {
        cout << "- " << room << " (Capacité: " << capacity << ")\n";
    }
    cout << "Connexions: " << endl;
    for (const auto &[room1, room2] : connections) {
        cout << "- " << room1 << " <--> " << room2 << endl;
    }
    displayLaplacianMatrix();
}

void AntHill::displayLaplacianMatrix() const {
    cout << "\nMatrice laplacienne :\n";
    int n = laplacianMatrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(3) << laplacianMatrix[i][j] << " ";
        }
        cout << endl;
    }
}