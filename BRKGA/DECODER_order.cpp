#include "DECODER_order.hpp"
#include <iostream>
#include <math.h>
#include <climits>

// Return the chromosome lenth
int DECODER_order::getChromosomeLength(vector<float> Tasks, vector<float> Computers) {
        return (Tasks.size() + Computers.size());
}

void DECODER_order::decode(vector<Individual> &population, vector<float> Tasks, vector<float> Computers, vector<float> Cost) {

        // For every individual of the population
        for (int i = 0; i < population.size(); i++) {
                // Decode its chromosome to get its solution and fitness
                std::pair< vector<float>, float> solutionFitness = decoder_order(Tasks, Computers, Cost, population[i].chromosome);
                // Clear the previous solution and assign the new one
                population[i].solution.clear();
                population[i].solution.insert(population[i].solution.begin(), solutionFitness.first.begin(), solutionFitness.first.end());;
                population[i].fitness = solutionFitness.second;
        }
}

bool compareByChromosome(const float &a, const float &b)
{
    return a < b;
}

bool compareByIndex(const std::pair<float,int> &a, const std::pair<float,int> &b)
{
    return a.first < b.first;
}


std::pair< vector<float>, float> DECODER_order::decoder_order(vector<float> Tasks, vector<float> Computers, vector<float> Cost, vector<float> chromosome) {

        std::pair< vector<float>, float> solutionFitness;
        solutionFitness.first = vector<float>(Tasks.size());

        vector<float> ComputersAux = Computers;

        float rem = 1.0;
        vector<int> used(Computers.size(), 0);

	vector<float> chr_task_unindexed(chromosome.begin(), chromosome.begin() + Tasks.size());
	vector<float> chr_computer_unindexed(chromosome.begin() + Tasks.size(), chromosome.end());
	vector<int> roundUsed(2); roundUsed[0]=1; roundUsed[1]=0;

	vector <std::pair<float, int> > chr_task_indexed(chr_task_unindexed.size());
	for (int i = 0; i < chr_task_indexed.size(); i++) chr_task_indexed[i] = std::make_pair(chr_task_unindexed[i], i);
	sort(chr_task_indexed.begin(), chr_task_indexed.end(), compareByIndex);
	
	vector <std::pair<float, int> > chr_computer_indexed(chr_computer_unindexed.size());
        for (int i = 0; i < chr_computer_indexed.size(); i++) chr_computer_indexed[i] = std::make_pair(chr_computer_unindexed[i], i);
        sort(chr_computer_indexed.begin(), chr_computer_indexed.end(), compareByIndex);


        for (int ii = 0; ii < chr_task_indexed.size(); ii++) {
		int i = chr_task_indexed[ii].second;
		int assigned = 0;
		for (int ru = 0; ru < roundUsed.size(); ru++) {
        		for (int jj = 0; jj < chr_computer_indexed.size(); jj++) {        
				int j = chr_computer_indexed[jj].second;
				if ((used[j] == roundUsed[ru]) && ((Computers[j] - Tasks[i]) < 0)) continue;
				if (used[j] == 0) used[j] = 1;
				solutionFitness.first[i] = j;
				Computers[j] -= Tasks[i];
				if (rem > (Computers[j] / ComputersAux[j])) rem = (Computers[j] / ComputersAux[j]);
				assigned = 1;
				break;
			}
			if (assigned) break;
		}
		if (!assigned) {
			solutionFitness.first.clear();
			solutionFitness.second = INT_MAX;
			return solutionFitness;
		}
        }

        float sum = 0.0;
        for (int i = 0; i < Cost.size(); i++)
                sum += Cost[i] * used[i];

        solutionFitness.second = sum + rem;
        return solutionFitness;
}

