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

public:
  AntHill();

  void loadFromFile(const string &filename);

  void display() const;

  int getRoomCapacity(const string &room) const;

  int getNumAnts() const { return numAnts; }
  const unordered_map<string, int>& getRooms() const { return rooms; }
  const vector<pair<string, string>>& getConnections() const { return connections; }
};

#endif // ANTHILL_H
