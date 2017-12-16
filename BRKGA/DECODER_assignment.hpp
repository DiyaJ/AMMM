#ifndef __DECODER_assignment__
#define __DECODER_assignment__

#include "BRKGA.hpp"
#include <vector>
#include <utility>

class DECODER_assignment {

public:

	int getChromosomeLength(vector<float> Tasks);
	void decode(vector<Individual> &population, vector<float> Tasks, vector<float> Computers, vector<float> Cost);
	std::pair< vector<float>, float> decoder_assignment(vector<float> Tasks, vector<float> Computers, vector<float> Cost, vector<float> chromosome);

};

#endif
