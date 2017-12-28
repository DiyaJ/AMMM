#include<iostream>
#include <algorithm>
#include <climits>
using namespace std;

#include "BRKGA.hpp"
//#include "DECODER_assignment.hpp"
//#include "DECODER_order.hpp"
#include "DECODER_nurses.hpp"
#include "CONFIGURATION.hpp"
//#include "DATA_1.hpp"
//#include "DATA_2.hpp"
#include "DATA_nurses1.hpp"

void printDemand(vector<float> demand_h)
{
	cout << "  "; 
	for(int i=0; i<demand_h.size(); i++) cout << demand_h[i] << "   ";
	cout << "  " << endl;
}

void printNursesHour(Individual ind)
{
	cout << "  "; 
	for(int i=0; i<TOTAL_HOURS; i++) {
		float sum = 0;
		for(int j=0; j<NNURSES; j++)
			sum += boost::any_cast<float>(ind.solution[(j *TOTAL_HOURS) + i]);
		cout << sum << " ";
	}
	cout << endl;
} 

void printSolution(Individual ind, int chrLength, vector<float> demand_h) 
{
	vector<float> workedNurses(demand_h.size(), 0);

/*
	cout << "Chromosome:" << endl;
	cout << "[ ";
	for(int i=0; i<chrLength; i++)
		cout << ind.chromosome[i] << ",  ";
	cout << "]" << endl;
*/
	cout << "Solution:" << endl;
	cout << "  "; 
	for(int i=0; i<TOTAL_HOURS; i++) cout << i%10 << " ";
	cout << "  " << endl;
	
	for(int i=0; i<TOTAL_HOURS; i++) {
		for(int j=0; j<NNURSES; j++)
			workedNurses[i] += boost::any_cast<float>(ind.solution[(j *TOTAL_HOURS) + i]);
	}
	
	for(int i=0; i<NNURSES; i++) {
		cout << "[ ";
		for(int j=0; j<TOTAL_HOURS; j++) {
			cout << boost::any_cast<float>(ind.solution[(i *TOTAL_HOURS) + j]) << " ";
		}
		cout << " ]" << endl;
	}
	
	cout << "Fitness:" << ind.fitness << endl << endl;
	
	cout << "nNurses:" << NNURSES << endl;
	cout << "totalHours: " << TOTAL_HOURS << endl;
	cout << "minHours: " << MINHOURS << endl;
	cout << "maxHours: " << MAXHOURS << endl;
	cout << "maxConsec: " << MAXCONSEC << endl;
	cout << "maxPresence: " << MAXPRESENCE << endl;
	
	for(int i=0; i<workedNurses.size(); i++) {
		cout << "demand[" << i << "]: " << demand_h[i] << " --------> workedNurses[" << i << "]: " << workedNurses[i];
		if (demand_h[i] > workedNurses[i]) cout << " X";
		cout << endl;
	}
}

int main() {
	// initializations
	int numElite = (int)numIndividuals*eliteProp;
	int numMutants = (int)numIndividuals*mutantProp;
	int numCrossover = max(numIndividuals-numElite-numMutants,0);
	float ro = inheritanceProb;
	vector<Individual> evol;
	Individual bestIndividual;
	bestIndividual.fitness = INT_MAX;
	BRKGA brkga;
	//DECODER_assignment decoder;
	DECODER_nurses decoder;

	// Main body
	//int chrLength = NNURSES * TOTAL_HOURS;
	//int chrLength = NNURSES + TOTAL_HOURS;
	//int chrLength = TOTAL_HOURS + (TOTAL_HOURS * NNURSES);
	int chrLength = NNURSES + (TOTAL_HOURS * NNURSES);
	//int chrLength = decoder.getChromosomeLength(Tasks, Computers);

	brkga.initializePopulation(numIndividuals,chrLength);
	brkga.Population[0].chromosome = brkga.createChromosomeFromSolution(GRASPSolution, NNURSES, TOTAL_HOURS);
	//brkga.Population[0].fitness = 121;
	//#if 0
	int i=0;
	float lastFitness = 0;
	//for (int i = 0; i < maxNumGen; i++) {
	while (i<1000) {
		//cout << "Iteration " << i << endl;
		cout << "bestIndividual.fitness: " << bestIndividual.fitness << endl;
		i++;
		if (lastFitness != bestIndividual.fitness) i = 0;
		lastFitness = bestIndividual.fitness;

		vector< vector<Individual> > classification;
		vector<Individual> elite;
		vector<Individual> nonElite;
		vector<Individual> mutants;
		vector<Individual> crossover;

		decoder.decode(brkga.Population, TOTAL_HOURS, NNURSES, MAXHOURS, MINHOURS, MAXCONSEC, MAXPRESENCE, demand_h, brkga.getBestFitness());
		evol.insert(evol.end(), brkga.getBestFitness());
		bestIndividual = brkga.getBestFitness();
/*		
		cout << "Print evol" << endl;
		for (int j=0; j<evol.size(); j++) {
			cout << "evol[" << j << "] = " << endl;
			brkga.printIndividual(evol[j]);
		}
*/		

    if (numElite > 0) {
        classification = brkga.classifyIndividuals(numElite);
				elite = classification[0];
				nonElite = classification[1];		
		}
    else {
			nonElite = brkga.Population;
		}
		
    if (numMutants > 0) mutants = brkga.generateMutantIndividuals(numMutants,chrLength);
    if (numCrossover > 0) crossover = brkga.doCrossover(elite,nonElite,ro,numCrossover);
    brkga.createPopulation(elite, crossover, mutants);
    
  }

	decoder.decode(brkga.Population, TOTAL_HOURS, NNURSES, MAXHOURS, MINHOURS, MAXCONSEC, MAXPRESENCE, demand_h, brkga.getBestFitness());
	bestIndividual = brkga.getBestFitness();
	cout << "*Fitness of each generation*" << endl;
	for (int j=0; j<evol.size(); j++) { brkga.printIndividualFitness(evol[j]); cout << endl; }
	cout << "Properties of the best individual" << endl;
	//brkga.printIndividual(bestIndividual);
	printSolution(bestIndividual, chrLength, demand_h);
	//#endif
}
