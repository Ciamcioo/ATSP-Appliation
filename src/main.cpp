#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <limits.h>
#include <chrono>
#include "Option.h"
#include "Algorithm.h"

void mainMenu();
Option validateInput(std::string);
int convertStringInput(std::string);
std::vector<std::vector<int>> loadPointsFromFile(std::string path);
int performAction(Option input);
std::string loadPath();
void clearTerminal();
void buttonPress();
double getBruteForceCalculationTime(Algorithm& algo);
double getDynamicProgramingCalculationTime(Algorithm& algo);
double getBranchAndBoundCalculationTime(Algorithm& algo);

std::vector<int> indexs;
std::vector<int> distances;
std::vector<std::vector<int>> points;
Algorithm algo = Algorithm();
bool shouldBreak = true;

int main(int, char**){
  mainMenu();
}

void mainMenu() {
  std::string input;
  Option input_option;
  do {
    std::cout << "Application solving ATSP problem" << std::endl;
    std::cout << "1. Load data set\n";
    std::cout << "2. Solve the problem using brute force alogrithm\n";
    std::cout << "3. Solve the problem using dynamic programing\n";
    std::cout << "4. Solve the problem using branch and bound algorithm\n";
    std::cout << "5. Run all algorithms at once\n";
    std::cout << "6. Print data\n";
    std::cout << "7. Run tests\n";
    std::cout << "8. Close application\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Provide the number relevant to option\n> ";
    std::getline(std::cin, input);

    input_option = validateInput(input);
    performAction(input_option); 

    buttonPress();
    clearTerminal();
  } while (input_option != Option::EXIT);
}

Option validateInput(std::string strInput) {
  int input = convertStringInput(strInput);
  switch (input) {
      case 1: { return Option::LOAD_SET; }
      case 2: { return Option::BRUTE_FORCE_METHOD; }    
      case 3: { return Option::DYNAMIC_PROGRAMING; }
      case 4: { return Option::BRANCH_AND_BOUND; }
      case 5: { return Option::RUN_ALL_ALGORITHMS; }
      case 6: { return Option::PRINT_LOADED_DATA; }
      case 7: { return Option::RUN_TESTS; }
      case 8: { return Option::EXIT; }
      default: { return Option::INVALID_INPUT; }
  } 
}

int convertStringInput(std::string strInput) {
  int input = 0;
  try {
    input = std::stoi(strInput); 
  } catch (const std::invalid_argument& e) {
    std::cout << "Invalid input arguemnt\n";
  } catch(const std::out_of_range& e) {
    std::cout << "Argument out of range\n";
  } 
  return input;
}

int performAction(Option input) {
  switch (input) {

     case Option::LOAD_SET: {
        indexs.clear();
        std::string path = loadPath();
        points = loadPointsFromFile(path);
        if (!points.empty())
          std::cout << "Points loaded to memory" << std::endl;
        else
          std::cout << "Operation was unsucesfull" << std::endl;
        break;
     }

     case Option::RUN_ALL_ALGORITHMS: {
        algo = Algorithm(indexs, points);
        std::cout << "Running all algorithms...\n";
        shouldBreak = false;
     }

     case Option::BRUTE_FORCE_METHOD: {
        if (points[0].size() <= 20) { 
            std::cout << "------------ BRUTE FORCE ------------\n";
            algo = Algorithm(indexs,points);

            auto start = std::chrono::high_resolution_clock::now();
            int cost = algo.bruteForce();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::cout << "Calculating time: " << elapsed.count() << " ms\n";
            std::cout << "Minimal path cost: " + std::to_string(cost) << std::endl; 
        }
        if (shouldBreak )
            break;       
     }

     case Option::DYNAMIC_PROGRAMING: {
        std::cout << "------------ DYNAMIC PROGRAMING ------------\n";
        algo = Algorithm(indexs,points);
        int cost = INT_MAX;
        int size = points.size();
        std::vector<std::vector<int>> dp(1 << size, std::vector<int>(size, -1));
        std::vector<std::vector<int>> parent(1 << size, std::vector<int>(size, -1));

        auto start = std::chrono::high_resolution_clock::now();
        cost = algo.dynamicPrograming(1, 0, dp, parent); 
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << algo.printOptimalPath(0, parent);
        std::cout << "Calculating time: " << elapsed.count() << " ms\n";
        std::cout << "Minimal path cost: " << cost << std::endl;
        if (shouldBreak)
            break;       
     }

     case Option::BRANCH_AND_BOUND: {
        std::cout << "------------ BRANCH AND BOUND ------------\n";
        std::vector<int> path;
        int cost = INT_MAX;

        algo = Algorithm(indexs, points);
        auto start = std::chrono::high_resolution_clock::now();
        cost = algo.branchAndBound(path); 
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "Optimal path: ";
        for (int vertex : path)
            std::cout << vertex << " ";
        std::cout << "0\n";
        std::cout << "Calculating time: " << elapsed.count() << " ms"  << std::endl;
        std::cout << "Minimal path cost: " << cost << std::endl;
        shouldBreak = true;
        break;
     }

  
     case Option::PRINT_LOADED_DATA:{
        printf("Number of points %lu\n", indexs.size()); 
        for (size_t i = 0; i < points.size(); i++) {
          for (size_t j = 0; j < points.size(); j++) {
            std::cout << points[i][j] << " ";
          }
          std::cout << std::endl;
        }
        break;
     }

     case Option::RUN_TESTS: {
         double avgBruteForce = 0., avgDynamicPrograming = 0., avgBranchAndBound = 0.; 
         int size = points[0].size();
         int seed = size/5;

         for (int i = 0; i < 100; i++) {
             algo = Algorithm(indexs, points);
             if (size < 14)
                 avgBruteForce += getBruteForceCalculationTime(algo);   
             avgDynamicPrograming += getDynamicProgramingCalculationTime(algo);
             avgBranchAndBound += getBranchAndBoundCalculationTime(algo);

            for (int j = 0; j < seed; j++) {
              int newCost = rand()%100 + 1; 
              int x = rand()%size;
              int y = rand()%size;
              if (x != y) 
                points[x][y] = newCost;
            }

            std::cout << "Number. " << i << " Brute foce result: " << avgBruteForce/(1.0*(i+1)) <<  " "; 
            std::cout << "Dynamic programing result: " << avgDynamicPrograming/(1.0*(i+1)) << " "; 
            std::cout << "Branch and bound result: " << avgBranchAndBound/(1.0*(i+1)) << std::endl; 

         }

         std::cout << "Brute foce result: " << avgBruteForce/100.0 << std::endl; 
         std::cout << "Dynamic programing result: " << avgDynamicPrograming/100.0 << std::endl; 
         std::cout << "Branch and bound result: " << avgBranchAndBound/100.0 << std::endl; 
         break;
     }
                           

     case Option::EXIT:{
        std::cout << "Application is closing." << std::endl; 
        break;
     }

     case Option::INVALID_INPUT:{
        std::cout << "Invalid input" << std::endl;
        break;
     }

  }

  return 1;
}

std::vector<std::vector<int>> loadPointsFromFile(std::string path) {
  std::ifstream inputFile(path); 
  std::vector<std::vector<int>> matrix;
  
  if (!inputFile) {
    std::cout << "Unable to find file" << std::endl;
    return matrix;
  }

  int points_number = 0;
  inputFile >> points_number;
  if (points_number  <= 0) {
    std::cout << "Issue with the size of an array." << std::endl;
    return matrix;
  }

  for (int i = 0; i < points_number; i++ ) {
      std::vector<int> distances;
      for (int j = 0; j < points_number; j++) { 
        int distance;
        inputFile >> distance;
        distances.push_back(distance);
      }
      matrix.push_back(distances);
      if (i != 0)
          indexs.push_back(i);
  }

  inputFile.close();
  return matrix;
}

std::string loadPath() {
  std::string path;
  std::cout << "Provide path to file with tsp problem\n> ";
  std::cin >> path;
  return path;
  
}

void buttonPress() {
  std::cin.ignore();
  std::cout << "Press Enter to clear the terminal..." << std::endl;
  std::cin.get();
}

void clearTerminal() {
  #ifdef _WIN32
    std::system("cls");
  #else
    std::system("clear");
  #endif
}

double getBruteForceCalculationTime(Algorithm& algo) {
    algo = Algorithm(indexs, points);
    auto start = std::chrono::high_resolution_clock::now();
    algo.bruteForce();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

double getDynamicProgramingCalculationTime(Algorithm& algo) {
    int size = points.size();
    std::vector<std::vector<int>> dp(1 << size, std::vector<int>(size, -1));
    std::vector<std::vector<int>> parent(1 << size, std::vector<int>(size, -1));
    auto start = std::chrono::high_resolution_clock::now();
    algo.dynamicPrograming(1, 0, dp, parent); 
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}
double  getBranchAndBoundCalculationTime(Algorithm& algo) {
    std::vector<int> path;
    auto start = std::chrono::high_resolution_clock::now();
    algo.branchAndBound(path); 
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}
