#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector> 
#include <iostream>

class Algorithm {
    private:
      // Fields
      std::vector<int> indexs; 
      std::vector<std::vector<int>> points;

      // Private  function
      int calculatePathCost(std::vector<int>& path);

    public:
      // Constructor
      Algorithm();
      Algorithm(std::vector<int> pointsNumber, std::vector<std::vector<int>> points);
      
      // Public function
      int bruteForce();
      int dynamicPrograming(int mask, int pos, std::vector<std::vector<int>>& dp, std::vector<std::vector<int>>& parent);
      int branchAndBound(std::vector<int>& optimalPath); 
      std::string printOptimalPath(int start, const std::vector<std::vector<int>>& parent);

      // Getters and Setters
      std::vector<std::vector<int>> getPoints();
      void setPoints(std::vector<std::vector<int>> points);
  
};
#endif 
