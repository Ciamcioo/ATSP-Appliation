#include "Node.h"
#include <algorithm>
#include <climits>

Node::Node(std::vector<int> path, int cost, int bound, int level, int vertex) {
   this -> path = path;
   this -> cost = cost;
   this -> bound = bound;
   this -> level = level;
   this -> vertex = vertex;
}

int Node::calculateBound(const std::vector<std::vector<int>>& costMatrix, int n) {
  int estimatedBound = cost;

  for (int i = 0; i < n; i++) {
    if (std::find(path.begin(), path.end(), i) == path.end()) {
      int minCost = INT_MAX;

      for (int j = 0; j < n; j++) {
        if (i != j && std::find(path.begin(), path.end(), j) == path.end())
            minCost = std::min(minCost, costMatrix[i][j]);
      }
      estimatedBound += minCost;
    }
  }

  return estimatedBound;
}

