#ifndef __DECODER_nurses__
#define __DECODER_nurses__

#include "BRKGA.hpp"
#include <vector>
#include <utility>

class DECODER_nurses {

public:

				int getChromosomeLength(int nNurses);
				void decode(vector<Individual> &population, int totalHours, int nNurses, int maxHours, int minHours, int maxConsecHours, int maxPresenceHours, vector<float> demandH, Individual bestIndividual);
				std::pair< vector<float>, float> decoder_nurses(int nNurses, int totalHours, int maxHours, int minHours, int maxConsecHours, int maxPresenceHours, vector<float> demandH, vector<float> chromosome, vector<float> bestChromosome);

};

#endif

