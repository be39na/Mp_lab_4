#include <iostream>
#include <fstream>
#include <omp.h>
#include <string>
#include <chrono>
#include "GraphDijkstra.h"


using namespace std;

void genFile(int number, string nameFile)
{
	ofstream out;
	out.open(nameFile);
	if (!out)
		throw "Can not open file to generate";

	out << number << endl << rand() % number << endl;

	int** generated = new int*[number];
	for (auto i = 0; i < number; i++)
		generated[i] = new int[number];

	cout << "Generating matrix for size " << number << endl;
	for (auto i = 0; i < number; i++)
	{
		generated[i][i] = 0;
		for (auto j = i + 1; j < number; j++)
			generated[i][j] = generated[j][i] = rand() % 100;
	}
	cout << "Matrix generated" << endl;

	cout << "Writing file" << endl;
	for (auto i = 0; i < number; i++)
	{
		for (auto j = 0; j < number; j++)
			out << generated[i][j] << " ";
		out << endl;
		delete[] generated[i];
	}
	delete[] generated;
	cout << "File written" << endl;
	out.close();
}

bool fileExist(string name)
{

	std::ifstream in;
	in.open(name);
	if (!in) return false;
	return true;

}


int main()
{
	try
	{

		string name;
		cout << "Enter input file name" << endl;
		cin >> name;
		name = name + ".txt";
		int size = 0;

		if (!fileExist(name))
		{
			std::cout << "Could not find file -> Generate file\n";
			std::cout << "Enter size:";
			std::cin >> size;
			genFile(size,name);
		}
		


		Graph* graph = Graph::readFromFile(name);
		



		const auto max_threads = omp_get_max_threads();

		
		
		for (auto num_threads = 1; num_threads <= max_threads; num_threads++)
		{
							int chunk = graph->size / num_threads;
				auto start = std::chrono::steady_clock::now();
				graph->DijkstraOMPChunks(chunk);
				auto finish = std::chrono::steady_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
				cout << "time: " << duration.count() << " \tThreads: " << num_threads << "\n" << endl;;
				
		}
		
		name = "Output" + name;
		graph->writeToFile(name);
		delete graph;
	}
	catch (exception &ex)
	{
		cout << ex.what();
		return -1;
	}

	return 0;
}