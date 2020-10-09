#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>

class Graph
{
public:
	int** graph;
	long* distances_OMP;
	long* distances_sequential;
	bool* visited;
	int size;
	int start;

	Graph(int size, int start, int** array);
	~Graph();

	static Graph* readFromFile(std::string file);
	void writeToFile(std::string file);

	void prepare();
	void prepareOMP();
	void Dijkstra();
	void DijkstraOMP(int num_threads);
	void DijkstraOMPChunks(int chunk);
	bool compareDistances();
};