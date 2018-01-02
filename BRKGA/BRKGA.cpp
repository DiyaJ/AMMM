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

vector<float> BRKGA::createChromosomeFromSolution(vector<float> inputSolution, int nNurses, int totalHours)
{
	//vector<float> Chromosome(nNurses + inputSolution.size(), 0);
	//vector<float> Chromosome(nNurses, 0);
	vector<float> Chromosome(2*nNurses, 0);
	int workingNurses = 0;
	vector<int> indexWorkingNurses(nNurses, 0);
	srand(time(NULL));
	
	/*
	for (int i=0; i < nNurses; i++) {
		for (int j=0; j<totalHours; j++) if(inputSolution[i*totalHours+j]) Chromosome[i] = 0.7 + (float)(0.3 * (rand()) / (RAND_MAX + 1.0));
	}
	*/
	/*
	for (int i=0; i < nNurses; i++) {
		for (int j=0; j<totalHours; j++) {
			if(inputSolution[i*totalHours+j]) {
				indexWorkingNurses[workingNurses] = i; 
				Chromosome[workingNurses] = 0.75 + (float)(0.25 * (rand()) / (RAND_MAX + 1.0)); 
				workingNurses++; 
				break;
			}
		}
	}
	for (int i=0; i < workingNurses; i++) cout << indexWorkingNurses[i] << " ";
	cout << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	
	for (int i=0; i < workingNurses; i++) {
		for (int j=0; j<totalHours; j++) {
			if (inputSolution[indexWorkingNurses[i]*totalHours+j] == 1) Chromosome[i*totalHours + j + nNurses] = 0.85;
			//cout << inputSolution[indexWorkingNurses[i]*totalHours+j] << " ";
		}
		//cout << endl;
	}
	*/
	/*
	for (int i=0; i < inputSolution.size(); i++) {
		if (inputSolution[i]) Chromosome[i+nNurses] = 0.85;
	}
	*/
	/*
	for (int i=0; i < nNurses; i++) cout << Chromosome[i] << " ";
	cout << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	for (int i=0; i < nNurses; i++) {
		for (int j=0; j<totalHours; j++)
			cout << Chromosome[i*totalHours + j + nNurses] << " ";
		cout << endl;
	}
	*/
	
	for(int k=0;k<nNurses;k++) { 
		float i=0.5;
    Chromosome[nNurses+k]=0;
    // assigning the second set of alleles
    for(int j=0;j<totalHours;j++) {
			if(inputSolution[k*totalHours + j]==1) {
				Chromosome[nNurses+k]+=i;
			}
			i=i/2;
		}
		// assigning the first set of alleles
		if(Chromosome[nNurses+k]==0) {
			Chromosome[k]=0.4;     //chosen 0.4 just because it is less than 0.5
		}
		else {
			Chromosome[k]=0.8;     //chosen 0.8 because it is greater than 0.5
		}
	}
	
	//#if 0
	// Decoding the chromosome
	 for (int k=0; k<nNurses;k++) { 
			//if the first set allele>0.5 , make the nurse work and decode the schedule from the second set allele
			if(Chromosome[k]>0.5) {
				workingNurses++;
				/*
				float i = 0.5;
				for (int j =0; j <totalHours; j++) {
					if(Chromosome[nNurses + k]>=i) {
						 //cout << "1 ";
						 Chromosome[nNurses + k] = Chromosome[nNurses + k]-i;
					}
					//else cout << "0 ";
				 	i=i/2;
				}*/
				//cout << endl;
			}
			//else cout << "All 0" << endl;
		}
	cout << "workingNurses: " << workingNurses << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	//#endif
	
	for (int i=0; i < nNurses; i++) cout << Chromosome[i] << " ";
	cout << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	for (int i=nNurses; i < 2*nNurses; i++) {
			cout << Chromosome[i] << " ";
	}
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	
	return Chromosome;
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

