#include<iostream>
#include <algorithm>
using namespace std;

#include "BRKGA.hpp"
//#include "DECODER_assignment.hpp"
#include "DECODER_order.hpp"
#include "CONFIGURATION.hpp"
//#include "DATA_1.hpp"
#include "DATA_2.hpp"

int main() {
	// initializations
	int numElite = (int)numIndividuals*eliteProp;
	int numMutants = (int)numIndividuals*mutantProp;
	int numCrossover = max(numIndividuals-numElite-numMutants,0);
	float ro = inheritanceProb;
	vector<Individual> evol;
	BRKGA brkga;
	//DECODER_assignment decoder;
	DECODER_order decoder;

	// Main body
	//int chrLength = decoder.getChromosomeLength(Tasks);
	int chrLength = decoder.getChromosomeLength(Tasks, Computers);

	brkga.initializePopulation(numIndividuals,chrLength);

	for (int i = 0; i < maxNumGen; i++) {
		cout << "Iteration " << i << endl;

		vector< vector<Individual> > classification;
		vector<Individual> elite;
		vector<Individual> nonElite;
		vector<Individual> mutants;
		vector<Individual> crossover;

		decoder.decode(brkga.Population, Tasks, Computers, Cost);
		evol.insert(evol.end(), brkga.getBestFitness());
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

	decoder.decode(brkga.Population, Tasks, Computers, Cost);
	Individual bestIndividual = brkga.getBestFitness();
	
	cout << "*Fitness of each generation*" << endl;
	for (int j=0; j<evol.size(); j++) { brkga.printIndividualFitness(evol[j]); cout << endl; }
	cout << "Properties of the best individual" << endl;
	brkga.printIndividual(bestIndividual);
}
