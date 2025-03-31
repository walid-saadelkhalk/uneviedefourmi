#include "../include/uneviedefourmi/Features/AntHill.h"
#include "../include/uneviedefourmi/Features/AntsBfs.h"


void checkConnectivity(AntHill &antHill) {
    unordered_map<string, bool> visited;
    queue<string> toVisit;

    toVisit.push("Sd");
    visited["Sd"] = true;

    while (!toVisit.empty()) {
        string current = toVisit.front();
        toVisit.pop();

        for (const auto &[room1, room2] : antHill.getConnections()) {
            if (room1 == current && !visited[room2]) {
                visited[room2] = true;
                toVisit.push(room2);
            }
            if (room2 == current && !visited[room1]) {
                visited[room1] = true;
                toVisit.push(room1);
            }
        }
        cout << "Exploration de la salle : " << current << endl;
    }

    cout << "\n Vérification des salles atteignables : \n";
    for (const auto &[room, _] : antHill.getRooms()) {
        if (!visited[room]) {
            cout << " La salle " << room << " n'est PAS connectée à Sd !\n";
        } else {
            cout << "La salle " << room << " est connectée à Sd.\n";
        }
    }
}


int main() {
    AntHill antHill;
    antHill.loadFromFile("../../assets/fourmiliere_5.txt");
    // antHill.display();
    cout << "🔍 Vérification de la connectivité du graphe..." << endl;
    unordered_map<string, bool> visited;

    // checkConnectivity(antHill);

    AntBfs antBfs(antHill);
    antBfs.simulate();

    return 0;
}



