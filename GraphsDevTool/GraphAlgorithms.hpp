#ifndef GRAPHALGORITHMS_HPP
#define GRAPHALGORITHMS_HPP

#include <iostream>
#include <climits>
#include <vector>
#include <list>

void printSolution(const std::vector<int>& distance, const std::vector<int>& parent, int src, int dest, std::list<int>& l);
std::list<int> bellmanCallabaMin(std::vector<std::vector<int>> graph, int V, int src, int dest);
std::list<int> bellmanCallabaMax(std::vector<std::vector<int>> graph, int V, int src, int dest);
#endif // !GRAPHALGORITHMS_HPP
