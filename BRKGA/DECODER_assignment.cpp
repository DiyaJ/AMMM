#include "DECODER_assignment.hpp"
#include <iostream>
#include <math.h>
#include <climits>

// Return the chromosome lenth
int DECODER_assignment::getChromosomeLength(vector<float> Tasks) {
	return Tasks.size();
}

void DECODER_assignment::decode(vector<Individual> &population, vector<float> Tasks, vector<float> Computers, vector<float> Cost) {

	// For every individual of the population
	for (int i = 0; i < population.size(); i++) {
		// Decode its chromosome to get its solution and fitness
		std::pair< vector<float>, float> solutionFitness = decoder_assignment(Tasks, Computers, Cost, population[i].chromosome);
		// Clear the previous solution and assign the new one
		population[i].solution.clear();		
		population[i].solution.insert(population[i].solution.begin(), solutionFitness.first.begin(), solutionFitness.first.end());;		
		population[i].fitness = solutionFitness.second;
	}
}

std::pair< vector<float>, float> DECODER_assignment::decoder_assignment(vector<float> Tasks, vector<float> Computers, vector<float> Cost, vector<float> chromosome) {

	std::pair< vector<float>, float> solutionFitness;
	solutionFitness.first = vector<float>(Tasks.size());

	vector<float> ComputersAux = Computers;

	float rem = 1.0;
	float segment = 1 / (float)Computers.size();
	vector<int> used(Computers.size(), 0);
	
	for (int i = 0; i < Tasks.size(); i++) {
		int j = (int)floor(chromosome[i] / segment);
		if ((Computers[j] - Tasks[i]) < 0) {
			solutionFitness.first.clear();
			solutionFitness.second = INT_MAX;
			return solutionFitness;
		}
		
		if (used[j] == 0) used[j] = 1;
		solutionFitness.first[i] = j;
		Computers[j] -= Tasks[i];
		if (rem > (Computers[j] / ComputersAux[j])) rem = (Computers[j] / ComputersAux[j]);
		
	}
	
	float sum = 0.0;
	for (int i = 0; i < Cost.size(); i++)
		sum += Cost[i] * used[i];
		
	solutionFitness.second = sum + rem;

	return solutionFitness;
	
}

