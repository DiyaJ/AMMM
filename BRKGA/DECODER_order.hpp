#ifndef __DECODER_order__
#define __DECODER_order__

#include "BRKGA.hpp"
#include <vector>
#include <utility>

class DECODER_order {

public:

        int getChromosomeLength(vector<float> Tasks, vector<float> Computers);
        void decode(vector<Individual> &population, vector<float> Tasks, vector<float> Computers, vector<float> Cost);
        std::pair< vector<float>, float> decoder_order(vector<float> Tasks, vector<float> Computers, vector<float> Cost, vector<float> chromosome);

};

#endif

