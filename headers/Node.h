#ifndef NODE_H
#define NODE_H

#include <vector>

struct Node {
  std::vector<int> path;
  int cost;
  int bound;
  int level;
  int vertex;

  Node(std::vector<int> path, int cost, int bound, int level, int vertex);

  int calculateBound(const std::vector<std::vector<int>>& costMatrix, int n);
};

#endif 
