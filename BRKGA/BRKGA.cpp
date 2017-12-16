#include "BRKGA.hpp"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <climits>

void BRKGA::initializePopulation(int numIndividuals, int chrLength) {

	this->numIndividuals = numIndividuals;
	this->chrLength = chrLength;

	srand(time(NULL));
	//Set the number of Individuals
	this->Population = vector<Individual>(numIndividuals);

	//For each individual, initialize randomly its chromosome array
	//Their fitness are uncalculated (-1) and their solutions are empty
	for (int i = 0; i < numIndividuals; i++) {
		Population[i].chromosome = vector<float>(chrLength);
		Population[i].fitness = INT_MAX;
		for (int j = 0; j < chrLength; j++)
			Population[i].chromosome[j] = ((float)rand() / (RAND_MAX));
	}
}

void BRKGA::createPopulation(vector<Individual> elite, vector<Individual> crossover, vector<Individual> mutants) {

	this->Population.clear();
	vector<Individual> result;

	result.insert(result.end(), elite.begin(), elite.end());
	result.insert(result.end(), crossover.begin(), crossover.end());
	result.insert(result.end(), mutants.begin(), mutants.end());

	//this->Population = result;
	this->Population.insert(this->Population.begin(), result.begin(), result.end());
}

bool compareByFitness(const Individual &a, const Individual &b)
{
    return a.fitness < b.fitness;
}

vector< vector<Individual> > BRKGA::classifyIndividuals(int numElite) {

	//Vector to store elite and nonElite vectors
	vector< vector<Individual> > classification(2);

	//Sorting the population by the fitness
	sort(this->Population.begin(), this->Population.end(), compareByFitness);

	//Storing the elite and nonElite individuals separately
	classification[0].insert(classification[0].end(), Population.begin(), Population.begin()+numElite);
	classification[1].insert(classification[1].end(), Population.begin() + numElite, Population.end());
	return classification;
}
	
vector<Individual> BRKGA::generateMutantIndividuals(int numMutants, int chrLength) {

	srand(time(NULL));

	vector<Individual> mutants(numMutants);
	for (int i = 0; i < numMutants; i++) {
		mutants[i].chromosome = vector<float>(chrLength);
		mutants[i].fitness = INT_MAX;
		for (int j = 0; j < chrLength; j++)
			mutants[i].chromosome[j] = ((float)rand() / (RAND_MAX));
	}
	return mutants;
}
	
vector<Individual> BRKGA::doCrossover(vector<Individual> elite, vector<Individual> nonElite, float ro, int numCrossover) {

	srand(time(NULL));
	vector<Individual> crossover;
	
	for (int i = 0; i < numCrossover; i++) {
		int indexElite = rand() % elite.size();
		int indexNonElite = rand() % nonElite.size();
		vector<float> chrElite = elite[indexElite].chromosome;
		vector<float> chrNonElite = nonElite[indexNonElite].chromosome;
		Individual crossed;
		crossed.chromosome = vector<float> (chrElite.size());
		crossed.fitness = INT_MAX;
		vector<float> rnd(chrElite.size());
		for (int j = 0; j < chrElite.size(); j++) {
			rnd[j] = ((float)rand() / (RAND_MAX));
			if (rnd[j] <= ro) crossed.chromosome[j] = chrElite[j];
			else crossed.chromosome[j] = chrNonElite[j];
		}
		crossover.insert(crossover.end(), crossed);
	}

	return crossover;
}

Individual BRKGA::getBestFitness() {

	sort(this->Population.begin(), this->Population.end(), compareByFitness);
	return this->Population[0];
}

void BRKGA::printPopulation() {

	cout << "*Properties of the population*" << endl;

	for(int i=0; i<numIndividuals; i++){
		cout << "*Individual " << i << "*" << endl;
		
		cout << "Chromosome:" << endl << "[ ";
		for(int j=0; j<chrLength; j++)
			cout <<  Population[i].chromosome[j] << ", ";
		cout << " ]" << endl;
		
		cout << "Solution:" << "[ " << endl;
		for(int j=0; j<Population[i].solution.size(); j++)
			cout << boost::any_cast<float>(Population[i].solution[j]) << ",  ";
		cout << " ]" << endl;
	
		cout << "Fitness:" << Population[i].fitness << endl;
	}
}

void BRKGA::printIndividual(Individual ind) {

	cout << "Chromosome:" << endl;
	cout << "[ ";
	for(int i=0; i<chrLength; i++)
		cout << ind.chromosome[i] << ",  ";
	cout << "]" << endl;
	
	cout << "Solution:" << endl;
	cout << "[ ";
	for(int i=0; i<ind.solution.size(); i++)
		cout << boost::any_cast<float>(ind.solution[i]) << ",  ";
	cout << " ]" << endl;
	
	cout << "Fitness:" << ind.fitness << endl;
}

void BRKGA::printIndividualSolution(Individual ind) {

        cout << "[ ";
        for(int i=0; i<ind.solution.size(); i++)
                cout << boost::any_cast<float>(ind.solution[i]) << ",  ";
        cout << " ]" << endl;
}

void BRKGA::printIndividualFitness(Individual ind) {

	cout << ind.fitness;
}

