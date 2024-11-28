#include "Algorithm.h"

#include <climits>
#include <algorithm>
#include <chrono>
#include <queue>

#include "Node.h"


struct Compare {
  bool operator()(const Node& a, const Node& b) {
      return a.bound > b.bound;
  }
};

// CONSTRUCTORS
Algorithm::Algorithm() {
   this -> indexs = {};
   this -> points = {{}};
}

Algorithm::Algorithm(std::vector<int> indexs, std::vector<std::vector<int>> points) {
  this -> indexs = indexs;
  this -> points = points; 
}

// FUNCTIONS
int Algorithm::calculatePathCost(std::vector<int>& path) {
  int cost = 0;

  for (size_t i = 0; i < path.size() -1; i++) {
      cost += points[path[i]][path[i+1]];
  
  }
  cost += points[path.back()][path[0]];

  return cost;
} 

int Algorithm::bruteForce() {
  int minCost = INT_MAX;
  int size = points.size();

  std::vector<int> path, pathToOutput;
  for (int i = 1; i < size; i++)
    path.push_back(i);
  
  do {
    std::vector<int> fullPath = {0};
    fullPath.insert(fullPath.end(), path.begin(), path.end());
    int currentCost = calculatePathCost(fullPath);
    minCost = std::min(currentCost, minCost);
    if (currentCost == minCost) 
      pathToOutput = fullPath; 
  } while (std::next_permutation(path.begin(), path.end()));
  
  //std::cout << "Optimal path: ";
  //for (int vertex : pathToOutput) {
  //  std::cout << vertex << " ";
  //}

  //std::cout << "0" << std::endl;
  return minCost;
}

int Algorithm::dynamicPrograming(int mask, int pos, std::vector<std::vector<int>>& dp, std::vector<std::vector<int>>& parent) {
    int size = points.size(); 

   
    if (mask == (1 << size) - 1) {
        return points[pos][0];
    }

    
    if (dp[mask][pos] != -1) {
        return dp[mask][pos]; 
    }

    int minCost = INT_MAX;

    for (int point = 0; point < size; point++) {
        if ((mask & (1 << point)) == 0) { 
            int newCost = points[pos][point] + dynamicPrograming(mask | (1 << point), point, dp, parent);
            if (newCost < minCost) {
                minCost = newCost; 
                parent[mask][pos] = point;
            }
        } 
    }

    return dp[mask][pos] = minCost;
}

std::string Algorithm::printOptimalPath(int start, const std::vector<std::vector<int>>& parent) {
    int mask = 1 << start;
    int pos = start;
    std::vector<int> path = {start}; 
  
    while (true) {
        int next = parent[mask][pos];
        if (next == -1) break;
        path.push_back(next);
        mask |= (1 << next);
        pos = next;
    }

 
    std::string result_path = "Optimal path: ";
    for (int vertex : path) {
        result_path += std::to_string(vertex) + " ";
    }
    result_path += "0\n"; 

    return result_path;
} 

int Algorithm::branchAndBound(std::vector<int>& optimalPath) {
  std::vector<std::vector<int>> costMatrix = points;
  int n = costMatrix.size();
  std::priority_queue<Node, std::vector<Node>, Compare> pq;

  Node root({0}, 0, 0, 0, 0);
  root.bound = root.calculateBound(costMatrix, n);
  pq.push(root);

  int minCost = INT_MAX;

  while(!pq.empty()) {
    Node node = pq.top();
    pq.pop();
    
    if (node.bound >= minCost) continue;

    for (int i = 0; i < n; i++) {
      if (std::find(node.path.begin(), node.path.end(), i) == node.path.end() && costMatrix[node.vertex][i] != -1) {
        Node child = node; 
        child.level = node.level + 1; 
        child.path.push_back(i);
        child.vertex = i;

        int lastVertex = node.vertex;
        child.cost += costMatrix[lastVertex][i];

        if (child.level == n - 1) {
          child.cost += costMatrix[child.vertex][0];
          if (child.cost < minCost) {
            minCost = child.cost;
            optimalPath = child.path;
          }
        } else {
          child.bound = child.calculateBound(costMatrix, n);
          if (child.bound < minCost) 
              pq.push(child);
        }
      }
    }
  }
  return minCost;
}



// GETTERS AND SETTERS
std::vector<std::vector<int>> Algorithm::getPoints() {
  return this -> points;
}

void Algorithm::setPoints(std::vector<std::vector<int>> points) {
  if (!points.empty())
        this -> points = points;
}
