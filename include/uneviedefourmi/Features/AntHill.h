#ifndef ANTHILL_H
#define ANTHILL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

class AntHill {
private:
  int numAnts;
  unordered_map<string, int> rooms;
  vector<pair<string, string>> connections;
  vector<vector<int>> laplacianMatrix;
  vector<string> roomNames; // Pour associer les indices aux noms des salles

public:
  AntHill();
  void loadFromFile(const string &filename);
  void buildLaplacianMatrix();
  void display() const;
  void displayLaplacianMatrix() const;
  void computeLaplacianMatrix();
  void generateLaplacianMatrix();
  void printLaplacianMatrix() const;
  void constructLaplacianMatrix();

  int getNumAnts() const { return numAnts; }
  const unordered_map<string, int>& getRooms() const { return rooms; }
  const vector<pair<string, string>>& getConnections() const { return connections; }
};

#endif // ANTHILL_H