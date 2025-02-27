#include "../include/uneviedefourmi/Features/AntHill.h"
#include "../include/uneviedefourmi/Features/AntsBfs.h"

int main() {
    AntHill antHill;
    antHill.loadFromFile("../../assets/fourmiliere_5.txt");
    // antHill.display();
    cout << "🔍 Vérification de la connectivité du graphe..." << endl;
    unordered_map<string, bool> visited;

    AntBfs antBfs(antHill);
    antBfs.simulate();

    return 0;
}
