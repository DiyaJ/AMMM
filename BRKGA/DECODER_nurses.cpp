#include "DECODER_nurses.hpp"
#include <iostream>
#include <math.h>
#include <climits>
//#include <tuple>
#include <algorithm>
#include <iterator>

// Return the chromosome lenth
int DECODER_nurses::getChromosomeLength(int nNurses) {
        return nNurses;
}

void DECODER_nurses::decode(vector<Individual> &population, int totalHours, int nNurses, int maxHours, int minHours, int maxConsecHours, int maxPresenceHours, vector<float> demandH/*, Individual bestIndividual*/) {
int workingNurses=0;
	for (int i=0; i < nNurses; i++) if (population[0].chromosome[i] > 0.5) workingNurses++;
	//cout << "workingNurses: " << workingNurses << endl;
        // For every individual of the population
        for (int i = 0; i < population.size(); i++) {
                // Decode its chromosome to get its solution and fitness
                std::pair< vector<float>, float> solutionFitness = decoder_nurses(nNurses, totalHours, maxHours, minHours, maxConsecHours, maxPresenceHours, demandH, population[i].chromosome, i);
                // Clear the previous solution and assign the new one
                population[i].solution.clear();
                population[i].solution.insert(population[i].solution.begin(), solutionFitness.first.begin(), solutionFitness.first.end()); 	
                population[i].fitness = solutionFitness.second;
        }
}

bool compareByChromosome(const float &a, const float &b)
{
    return a < b;
}

bool compareByIndex(const std::pair<float,int> &a, const std::pair<float,int> &b)
{
    return a.first > b.first;
}

/*
bool compareByIndexTuple(const std::tuple<float, int, int> &a, const std::tuple<float, int, int> &b)
{
    return std::get<0>(a) > std::get<0>(b);
}
*/

#if 0
void makeNurseFeasible(int nurseError, vector<float> &Hours, vector<float> &workedHours, int nurse, int totalHours, int minHours, int maxConsecHours)
{
	int firstWorkedHour;	
	int accumulatedHours = 0;
	
	/* First worked hour */
	for (int i = 0; i < totalHours; i++) {
		if (Hours[i + nurse * totalHours] == 1) {
			firstWorkedHour = i;
			break;
		}
	}
	
	/* Accumulated worked hours */
	for (int i = nurse * totalHours; i < ((nurse * totalHours) + totalHours); i++) accumulatedHours += Hours[i];
	
	switch(nurseError) {
		case 1: 
		{
			int lastConsecHours = 0;
			for (int i=firstWorkedHour+1; i<totalHours; i++) {
				if (Hours[i + nurse * totalHours] == 0) {
					for (int i = ((nurse * totalHours) + i)-1; (i >= nurse * totalHours); i--) {
						lastConsecHours += Hours[i];
						if (Hours[i] == 0) break;
					}

					for (int j = ((nurse * totalHours) + i)+1; (j < (nurse * totalHours) + totalHours); j++) {
						lastConsecHours += Hours[j];
						if (Hours[j] == 0) break;
					}
				
					if (lastConsecHours < maxConsecHours) {				
						Hours[i + nurse * totalHours] = 1;
						workedHours[i]++;
						accumulatedHours++;
					}
				}
					if (accumulatedHours == minHours) break;
			}
			break;
		}
		default: break;
	}
}
#endif

void makeNurseWork(vector<float> &Hours, vector<float> &demandH, int nurse, int minHours, int maxHours, int maxConsecHours, int maxPresenceHours)
{
	int totalHours = demandH.size();
	int maxDemandHour = std::distance(demandH.begin(), std::max_element(demandH.begin(),demandH.end()));
	float demandLeft = 0;
	float demandRight = 0;
	int lastConsecHours = 0;
	
	Hours[(nurse * totalHours) + maxDemandHour] = 1;
	demandH[maxDemandHour] = max(0, (int)demandH[maxDemandHour]-1);
	
	for (int i = ((nurse * totalHours) + maxDemandHour)-1; (i >= nurse * totalHours); i--) {
		if (Hours[i] == 0) {
			demandLeft += demandH[i];
		}
	}

	for (int j = ((nurse * totalHours) + maxDemandHour)+1; (j < (nurse * totalHours) + totalHours); j++) {
		if (Hours[j] == 0) {
			demandRight += demandH[j];
		}
	}
	
	if (demandLeft > demandRight) {
		if (lastConsecHours == maxConsecHours);
	}
}

/* A function to check if a solution is correct */
bool checkFeasibilityAfter(vector<float> Hours, vector<int> nurseWorked, int nurse, int totalHours, int minHours)
{
	int accumulatedHours = 0;
	
	if (!nurseWorked[nurse]) return true;

	/* The Nurse reach the minHours */
	for (int i = nurse * totalHours; i < ((nurse * totalHours) + totalHours); i++) accumulatedHours += Hours[i];
	if (accumulatedHours < minHours) return false;
	
	
	/* The Nurse does not rest more than one hour */
	///*
	for (int i = nurse * totalHours; i < ((nurse * totalHours) + totalHours); i++) {
		if (Hours[i]) {
			int restingHours = 0;
			int j;
			for (j = i+1; j < ((nurse * totalHours) + totalHours); j++) {
				if (Hours[j] == 0) restingHours++;
				else break;
			}
			if (restingHours > 1 && j != ((nurse * totalHours) + totalHours)) return false;
		}
	}
	//*/
	
	/* Each demandH is achieved */
	
	return true;

}


bool checkDemandHours(vector<float> Hours, vector<float> demandH, int hour, int nurse, int nNurses)
{
	if ((Hours[hour] < demandH[hour]) && ((nNurses - nurse) <= (demandH[hour] - Hours[hour]))) return false;
	return true;
}

bool checkDemandHoursWeek(vector<float> Hours, vector<float> demandH)
{
	for (int i = 0; i < Hours.size(); i++) if (Hours[i] < demandH[i]) return false;
	//for (int i = 0; i < Hours.size(); i++) if (Hours[i] != 0) return false;
	return true;
}

/* A function to check if a nurse can work in an hour */
bool checkNurseFeasibility(vector<float> Hours, int hour, int nurse, int totalHours, int maxHours, int maxConsecHours, int maxPresenceHours) 
{
	bool feasible = true;
	
	int accumulatedHours = 0;
	int lastConsecHours = 0;
	int firstWorkedHour = -1;
	int lastWorkedHour = -1;
	
	/* Accumulated worked hours */
	/* Just to check until that hour */
	//for (int i = nurse * totalHours; i < ((nurse * totalHours) + hour); i++) accumulatedHours += Hours[i];
	/* Check all the hours */
	for (int i = nurse * totalHours; i < ((nurse * totalHours) + totalHours); i++) accumulatedHours += Hours[i];
	
	/* Last consecutive worked hours */
	/* Just to check until that hour */
	bool consecBefore = true;
	bool consecAfter = true;
	for (int i = ((nurse * totalHours) + hour)-1; (i >= nurse * totalHours); i--) {
		lastConsecHours += Hours[i];
		if (Hours[i] == 0) break;
	}

	for (int j = ((nurse * totalHours) + hour)+1; (j < (nurse * totalHours) + totalHours); j++) {
		lastConsecHours += Hours[j];
		if (Hours[j] == 0) break;
	}
	
	/* First worked hour */
	for (int i = 0; i < totalHours; i++) {
		if (Hours[i + nurse * totalHours] == 1) {
			firstWorkedHour = i;
			break;
		}
	}
	
	/* Last worked hour */
	for (int i = totalHours-1; i >= 0; i--) {
		if (Hours[i + nurse * totalHours] == 1) {
			lastWorkedHour = i;
			break;
		}
	}
	
	/* The Nurse doesn't exceed the maxHours */
	if (accumulatedHours >= maxHours) feasible = false; //{ cout << "hour: " << hour << " accumulatedHours: " << accumulatedHours << " maxHours: " << maxHours << endl; feasible = false; }
	
	/* The Nurse doesn't exceed the maxConsecHours */
	if (lastConsecHours >= maxConsecHours) feasible = false;
	
	/* The Nurse doesn't exceed the maxPresence */	
	if (firstWorkedHour != -1) {
		firstWorkedHour = std::min(firstWorkedHour, hour);
		lastWorkedHour = std::max(lastWorkedHour, hour);
		if ((lastWorkedHour - firstWorkedHour) >= maxPresenceHours) feasible = false;
	}
	
	return feasible;
}

std::pair< vector<float>, float> DECODER_nurses::decoder_nurses(int nNurses, int totalHours, int maxHours, int minHours, int maxConsecHours, int maxPresenceHours, vector<float> demandH, vector<float> chromosome, int ind) {

	std::pair< vector<float>, float> solutionFitness;
	//solutionFitness.first = vector<float>(nNurses + (nNurses * totalHours), 0);
	solutionFitness.first = vector<float>((nNurses * totalHours), 0);
	//solutionFitness.first = vector<float>(2 * nNurses, 0);
	
	vector<float> Hours(totalHours, 0);
	vector<int> nurseWorked(nNurses, 0);
	int nurseWorked2 = 0;
	int nurseWorked3 = 0;
	int hourOK = 0;
	bool feasible;
	bool incorrect = false;
	
	/* Dummy decoder where a nurse works if that chromosome is g.e. than 0.5 */
	#if 0
	for (int i = 0; i < totalHours; i++) {
		//For each hour
		for (int j = 0; j < nNurses; j++) {
			//For each nurse
			if (chromosome[i + (j*totalHours)] >= 0.5) {
				//Try to make this nurse work
				feasible = checkNurseFeasibility(solutionFitness.first, i, j, totalHours, maxHours, maxConsecHours, maxPresenceHours);
				if (feasible) {
					solutionFitness.first[i + (j*totalHours)] = 1;
					Hours[i]++;
				}
			}
			//else that nurse doesn't work
			else solutionFitness.first[i + (j*totalHours)] = 0;
		}
		if (Hours[i] < demandH[i]) {
			//cout << "Hours[" << i << "]: " << Hours[i] << " demandH[" << i << "]: " << demandH[i] << endl;
			solutionFitness.first.clear();
			solutionFitness.second = INT_MAX;
			return solutionFitness;
		}
	}
	#endif
	
	/* Decoder where nurses and hours are sorted by the chromosome (nNurses + Hours)*/
	#if 0
	// First we sort the nurses and hours and we store them with their indices
	vector<float> chr_nurse_unindexed(chromosome.begin(), chromosome.begin() + nNurses);
	vector<float> chr_hour_unindexed(chromosome.begin() + nNurses, chromosome.end());
	
	vector <std::pair<float, int> > chr_nurse_indexed(chr_nurse_unindexed.size());
	for (int i = 0; i < chr_nurse_indexed.size(); i++) chr_nurse_indexed[i] = std::make_pair(chr_nurse_unindexed[i], i);
	sort(chr_nurse_indexed.begin(), chr_nurse_indexed.end(), compareByIndex);
	
	vector <std::pair<float, int> > chr_hour_indexed(chr_hour_unindexed.size());
  for (int i = 0; i < chr_hour_indexed.size(); i++) chr_hour_indexed[i] = std::make_pair(chr_hour_unindexed[i], i);
  sort(chr_hour_indexed.begin(), chr_hour_indexed.end(), compareByIndex);

	for (int ii = 0; ii < chr_nurse_indexed.size(); ii++) {
		int i = chr_nurse_indexed[ii].second;
		int assigned = 0;
		for (int jj = 0; jj < chr_hour_indexed.size(); jj++) {       
			int j = chr_hour_indexed[jj].second; 
			//Try to make this nurse work
			feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
			if (feasible) {
				solutionFitness.first[j + (i*totalHours)] = 1;
				Hours[j]++;
			}
			//else that nurse doesn't work
			else {
				feasible = checkDemandHours(solutionFitness.first, demandH, j, ii, nNurses);
				if (feasible) solutionFitness.first[j + (i*totalHours)] = 0;
				else {
					//solutionFitness.first.clear();
					solutionFitness.second = INT_MAX;
					incorrect = true;
				}
			}
		}
	}
	#endif
	
	/* Decoder where nurses and hours are sorted by the chromosome individually (nNurses * Hours)(Option 1)*/
	#if 0
	// First we sort the nurses and hours and we store them with their indices
	vector<float> chr_nurse_unindexed(chromosome.begin(), chromosome.end());
	
	vector <std::tuple<float, int, int> > chr_nurse_indexed(chr_nurse_unindexed.size());
	for (int i = 0; i < chr_nurse_indexed.size(); i++) chr_nurse_indexed[i] = std::make_tuple(chr_nurse_unindexed[i], i/totalHours, i%totalHours);
	sort(chr_nurse_indexed.begin(), chr_nurse_indexed.end(), compareByIndexTuple);
	
	for (int ii = 0; ii < nNurses * totalHours; ii++) {
		int i = std::get<1>(chr_nurse_indexed[ii]);	// Nurse
		int j = std::get<2>(chr_nurse_indexed[ii]);	// Hour
		int assigned = 0;
			//Try to make this nurse work
    	if (std::get<0>(chr_nurse_indexed[ii]) > 0.5) {
				feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
				if (feasible && (Hours[j] < demandH[j])) {
					solutionFitness.first[j + (i*totalHours)] = 1;
					Hours[j]++;
					nurseWorked[i] = 1;
				}
			}
			//else that nurse doesn't work
			//if (checkDemandHoursWeek(Hours, demandH)) break;
	}
	
	for (int i = 0; i < nNurses; i++) {
		if (checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours) && nurseWorked[i]) nurseWorked2++;
		nurseWorked3 += nurseWorked[i];
	}
	
	for (int i = 0; i < Hours.size(); i++) if (Hours[i] >= demandH[i]) hourOK++;
	
	if (nurseWorked2 != nurseWorked3) {
			//solutionFitness.first.clear();
			//solutionFitness.second = INT_MAX;
			solutionFitness.second = (nNurses - nurseWorked2) + (totalHours - hourOK) + nNurses;
			return solutionFitness;
	}
	
	if (!checkDemandHoursWeek(Hours, demandH)) {
		//solutionFitness.first.clear();
    //solutionFitness.second = INT_MAX;
    solutionFitness.second = (totalHours - hourOK) + nNurses;
		return solutionFitness;
	}
	#endif
	
	/* Decoder where nurses and hours are sorted by the chromosome individually (nNurses * Hours)
	(Option 2, it does not matter order of nurses, for each nurse decide her scheduling)*/
	#if 0
    // First we sort the nurses and hours and we store them with their indices
    vector<float> chr_nurseHour_unindexed(chromosome.begin(), chromosome.end());
    
	vector <std::pair<float, int> > chr_nurseHour_indexed(nNurses * totalHours);
    for (int i = 0; i < chr_nurseHour_indexed.size(); i++) {
        int hour = i%totalHours;
        chr_nurseHour_indexed[i] = std::make_pair(chr_nurseHour_unindexed[i], hour);
    }

    for (int i = 0; i < nNurses; i++) {
        sort(chr_nurseHour_indexed.begin() + (i * totalHours), chr_nurseHour_indexed.begin() + (i * totalHours) + totalHours, compareByIndex);
        hourOK = 0;
        for (int jj=0; jj<totalHours; jj++) {
					int j = chr_nurseHour_indexed[jj + (i * totalHours)].second; // Hour
        	int assigned = 0;
            	//Try to make this nurse work
            	if (chr_nurseHour_indexed[jj + (i * totalHours)].first > 0.8) {
            		//MAYBE she will work in that hour
		          	feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
		          	if (feasible){// && (Hours[j] < demandH[j])) {
		              	solutionFitness.first[j + (i*totalHours)] = 1;
		              	Hours[j]++;
		              	nurseWorked[i] = 1;
		          	}
		          	// else that nurse doesn't work
	          	}
	          	// else the nurse does not work in that hour
				}
				
				for (int i = 0; i < Hours.size(); i++) if (Hours[i] >= demandH[i]) hourOK++;
				
				if (!checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) {
				    //solutionFitness.first.clear();
				    //solutionFitness.second = INT_MAX;
				    solutionFitness.second = (nNurses - nurseWorked2) + (totalHours - hourOK) + nNurses;
						return solutionFitness;
				}
				else nurseWorked2++;
				
				if (checkDemandHoursWeek(Hours, demandH)) break;
   }
   
	hourOK = 0;
	for (int i = 0; i < Hours.size(); i++) if (Hours[i] >= demandH[i]) hourOK++;
   
	if (!checkDemandHoursWeek(Hours, demandH)) {
		//solutionFitness.first.clear();
    //solutionFitness.second = INT_MAX;
    solutionFitness.second = (totalHours - hourOK) + nNurses;
		return solutionFitness;
	}
	#endif
	
	/* Decoder where nurses and hours are sorted by the chromosome individually (nNurses * Hours)
	(Option 3, like Option 2, but for each hour decide the working nurses)*/
	#if 0
    // First we sort the nurses and hours and we store them with their indices
    vector<float> chr_nurseHour_unindexed(chromosome.begin(), chromosome.end());
    
	vector <std::pair<float, int> > chr_nurseHour_indexed(nNurses * totalHours);
    for (int i = 0; i < chr_nurseHour_indexed.size(); i++) {
        int hour = i%nNurses;
        chr_nurseHour_indexed[i] = std::make_pair(chr_nurseHour_unindexed[i], hour);
    }

    for (int j=0; j<totalHours; j++) {
      sort(chr_nurseHour_indexed.begin() + (j * nNurses), chr_nurseHour_indexed.begin() + (j * nNurses) + nNurses, compareByIndex);
  		for (int ii = 0; ii < nNurses; ii++) {
					int i = chr_nurseHour_indexed[ii + (j * nNurses)].second; // Nurse
        	int assigned = 0;
            	//Try to make this nurse work
            	feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
            	if (feasible){// && (Hours[j] < demandH[j])) {
                	solutionFitness.first[j + (i*totalHours)] = 1;
                	Hours[j]++;
                	nurseWorked[i] = 1;
            	}
            	//else that nurse doesn't work
				}
   }
   
   for (int i=0; i < nNurses; i++) {
		 if (!checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) {
			  //solutionFitness.first.clear();
			  //solutionFitness.second = INT_MAX;
			  solutionFitness.second = (nNurses - nurseWorked2) + nNurses * nNurses;
				return solutionFitness;
			}
			else nurseWorked2++;
		}
   
	if (!checkDemandHoursWeek(Hours, demandH)) {
		//solutionFitness.first.clear();
    //solutionFitness.second = INT_MAX;
    solutionFitness.second = (nNurses - nurseWorked2) + nNurses + 10;
		return solutionFitness;
	}
	#endif

	/* Decoder where nurses and hours are sorted by the chromosome (nNurses + (Hours * nNurses))(Option 1)*/
	#if 0
	// First we sort the nurses and hours and we store them with their indices
	vector<float> chr_nurse_unindexed(chromosome.begin(), chromosome.begin() + nNurses);
	vector<float> chr_nurseHour_unindexed(chromosome.begin() + nNurses, chromosome.end());
	
	vector <std::pair<float, int> > chr_nurse_indexed(chr_nurse_unindexed.size());
	for (int i = 0; i < chr_nurse_indexed.size(); i++) chr_nurse_indexed[i] = std::make_pair(chr_nurse_unindexed[i], i);
	sort(chr_nurse_indexed.begin(), chr_nurse_indexed.end(), compareByIndex);
	
	vector <std::pair<float, int> > chr_nurseHour_indexed(nNurses * totalHours);
  	for (int i = 0; i < chr_nurseHour_indexed.size(); i++) {
		int hour = i%totalHours;
		chr_nurseHour_indexed[i] = std::make_pair(chr_nurseHour_unindexed[i], hour);
	}

	for (int ii = 0; ii < nNurses; ii++) {
		int i = chr_nurse_indexed[ii].second;     // Nurse
		sort(chr_nurseHour_indexed.begin() + (i * totalHours), chr_nurseHour_indexed.begin() + (i * totalHours) + totalHours, compareByIndex);
		for (int jj = 0; jj < totalHours; jj++) {
			int j = chr_nurseHour_indexed[jj + (i * totalHours)].second;     // Hour
			int assigned = 0;
			//Try to make this nurse work
			feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
			if (feasible) {
				solutionFitness.first[j + (i*totalHours)] = 1;
				Hours[j]++;
			}
			//else that nurse doesn't work
		}
	}
	feasible = checkDemandHoursWeek(Hours, demandH);
	if (!feasible) {
		//solutionFitness.first.clear();
		solutionFitness.second = INT_MAX;
		return solutionFitness;
	}
	for (int i = 0; i < nNurses; i++) {
		if (!checkFeasibilityAfter(solutionFitness.first, i, totalHours, minHours)) {
			//solutionFitness.first.clear();
			solutionFitness.second = INT_MAX;
			return solutionFitness;
		}
	}
	#endif
	
	/* Decoder where nurses and hours are sorted by the chromosome individually, but it decides if each nurse has to be considered
	(nNurses +(nNurses * Hours)) (Option 2, it does not matter order of nurses, for each nurse decide her scheduling)*/
	//#if 0
    // First we sort the nurses and hours and we store them with their indices
    vector<float> chr_nurses_unindexed(chromosome.begin(), chromosome.begin() + nNurses);
    vector<float> chr_nurseHour_unindexed(chromosome.begin() + nNurses, chromosome.end());
    int workingNurses = 0;
    
    for (int k=0; k<nNurses;k++) { 
			//if the first set allele>0.5 , make the nurse work and decode the schedule from the second set allele
			if(chr_nurses_unindexed[k]>0.5) {
				workingNurses++;
			}
		}
    //if (ind == 0) cout << "workingNurses: " << workingNurses << endl;
    
    /* 2*nNurses decoder, where a single chromosome determines the scheduling of a nurse */
    
    vector <std::pair<float, int> > chr_nurses_indexed(nNurses);
    for (int i = 0; i < chr_nurses_indexed.size(); i++) {
        chr_nurses_indexed[i] = std::make_pair(chr_nurses_unindexed[i], i);
    }
    sort(chr_nurses_indexed.begin(), chr_nurses_indexed.end(), compareByIndex);    

		vector <std::pair<float, int> > chr_nurseHour_indexed(nNurses * totalHours);
		//#if 0
    /* Decoding the chromosome */
    for (int k=0; k<nNurses;k++) { 
			//if the first set allele>0.5 , make the nurse work and decode the schedule from the second set allele
			if(chr_nurses_unindexed[k]>0.5) {
				//workingNurses++;
				float i = 0.5;
				for (int j =0; j <totalHours; j++) {
					if(chr_nurseHour_unindexed[k]>=i) {
						 chr_nurseHour_indexed[k * totalHours + j].first = 1;
						 chr_nurseHour_unindexed[k] = chr_nurseHour_unindexed[k]-i;
					}
					chr_nurseHour_indexed[k * totalHours + j].second = j;
			    //if (ind == 0) cout << chr_nurseHour_indexed[k * totalHours + j].first << " ";
				 	i=i/2;
				}
			}
			else {
				//for (int j =0; j <totalHours; j++) cout << "0 ";
			}
			//cout << endl;
		}
		//#endif
    if (ind == 0) cout << "workingNurses: " << workingNurses << endl;
    /*
		vector <std::pair<float, int> > chr_nurseHour_indexed(nNurses * totalHours);
    for (int i = 0; i < chr_nurseHour_indexed.size(); i++) {
        int hour = i%totalHours;
        chr_nurseHour_indexed[i] = std::make_pair(chr_nurseHour_unindexed[i], hour);
    }
    */
    
    /*
    vector <std::pair<float, int> > chr_nurses_indexed(nNurses);
    for (int i = 0; i < chr_nurses_indexed.size(); i++) {
        chr_nurses_indexed[i] = std::make_pair(chr_nurses_unindexed[i], i);
    }
    sort(chr_nurses_indexed.begin(), chr_nurses_indexed.end(), compareByIndex);
    
    for (int i=0; i<chr_nurses_indexed.size(); i++)
    	if (chr_nurses_indexed[i].first > 0.6) workingNurses++;
    	
  	if (ind == 0 || ind == 1) cout << "workingNurses: " << workingNurses << endl;
  	*/
    
    /*
    int j = 0;
    for (int i = 0; i < chr_nurses_indexed.size(); i++) {
    	if (chr_nurses_unindexed[i] > 0.6) {
    		workingNurses++;
      	chr_nurses_indexed[j] = std::make_pair(chr_nurses_unindexed[i], i);
      	j++;
    	}
    }
    */
    
    /*
    vector <std::pair<float, int> > bestChromosome_indexed(nNurses * totalHours);
    for (int i = 0; i < chr_nurseHour_indexed.size(); i++) {
        int hour = i%totalHours;
        bestChromosome_indexed[i] = std::make_pair(bestChromosome[i+nNurses], hour);
    }
    */
    
    int hourInfeasible = 0;
    int ii;
    bool found = false;
    int finalNurse;
  	srand(time(NULL));
  	//int stopWorkingNurse = rand() % workingNurses;
  	int random = rand() % 2;
  	int nurseOrder = 0;
  	int exceededHour = 0;

		//#if 0
    //for (ii = 0; ii < nNurses; ii++) {
    for (ii = 0; ii < workingNurses; ii++) {
    	// if chromosome of the nurse is g.t. 0.8 look if the nurse can work
    	/*
    	if (!found && (float)rand() / (RAND_MAX) > 0.7) {
    			found = true;
    			finalNurse = ii;
    	}
    	else {
    	*/
    	int i = chr_nurses_indexed[ii].second;
    	//int i = chr_nurses_indexed[nurseOrder].second;
    	vector<int> hourOrder(totalHours, 0);
    	//if (chr_nurses_indexed[ii].first > 0.7) {

			if (chr_nurses_indexed[ii].first) {
    		nurseOrder++;
    		/*
    		for (int j=0; j<totalHours; j++) {
    			chr_nurseHour_indexed[(i * totalHours) + j].first = chr_nurseHour_indexed[(i * totalHours) + j].first * (demandH[j] - Hours[j]) * 20;
    		}
    		*/
        sort(chr_nurseHour_indexed.begin() + (i * totalHours), chr_nurseHour_indexed.begin() + (i * totalHours) + totalHours, compareByIndex);
        hourOK = 0;
        
        for (int jj=0; jj<totalHours; jj++) {
					int j = chr_nurseHour_indexed[jj + (i * totalHours)].second; // Hour
					hourOrder[jj] = j;
        	int assigned = 0;
            	//Try to make this nurse work
            	if (chr_nurseHour_indexed[jj + (i * totalHours)].first != 0) {
            		//MAYBE she will work in that hour
            		
		          	feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
		          	if (feasible){// && (Hours[j] < demandH[j])) {
		              	solutionFitness.first[j + (i*totalHours)] = 1;
		              	if (demandH[j] <= Hours[j]) exceededHour++;
		              	Hours[j]++;
		              	nurseWorked[i] = 1;
		          	}
		          	// else that nurse doesn't work
		          	//else hourInfeasible++;
	          	}
	          	// else the nurse does not work in that hour
				}
				
				int hoursWorked = 0;
				//for (int ii = 0; ii < Hours.size(); ii++) hoursWorked += solutionFitness.first[ii + (i*totalHours)];
				
				if (!checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) {
					for (int jj=0; jj<totalHours; jj++) {
						if (solutionFitness.first[jj + (i*totalHours)]) {
							solutionFitness.first[jj + (i*totalHours)] = 0;
							Hours[jj]--;
							hourInfeasible++;
						}
					}
					//hourInfeasible++;
					nurseWorked[i] = 0;
				}
					/*
					for (int jj=totalHours-1; jj>=0; jj--) {
						int j = hourOrder[jj];
						if (solutionFitness.first[j + (i*totalHours)]) {
							solutionFitness.first[j + (i*totalHours)] = 0;
							Hours[j]--;
						}
						if (checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) { 
							if (jj != 0) feasible=true; 
							else feasible=false;
							break;
						}
					}
					if (feasible) nurseWorked[i] = 1;
					else nurseWorked[i] = 0;
					nurseWorked2++;
					*/
				//}
				//else nurseWorked2++;
				
				if (checkDemandHoursWeek(Hours, demandH)) break;
			}
			else nurseWorked2++;

   }
	//cout << "HOLA" << endl;
	hourOK = 0;
	for (int i = 0; i < Hours.size(); i++) if (Hours[i] >= demandH[i]) hourOK++;
	
	int hoursLeft = 0;
	int maxHoursLeft = 0;
	for (int i = 0; i < Hours.size(); i++) {
		hoursLeft += (demandH[i] - min(Hours[i], demandH[i]));
		if (maxHoursLeft < (demandH[i] - min(Hours[i], demandH[i]))) maxHoursLeft = (demandH[i] - min(Hours[i], demandH[i]));
	}
	
	nurseWorked2 = 0;
	for (int i=0; i<nNurses; i++) {
			if (checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) nurseWorked2++;
	}
	   	   
	if (!checkDemandHoursWeek(Hours, demandH)) {
		//solutionFitness.first.clear();
    //solutionFitness.second = INT_MAX;
    //solutionFitness.second = (totalHours - hourOK) + nNurses;
  	if (ind == 1) cout << "demandH not reached" << endl;
		solutionFitness.second = (totalHours - hourOK) * hoursLeft + hourInfeasible /*+ exceededHour*/ + nNurses;
		return solutionFitness;
	}
	
	for (int i=0; i<nNurses; i++) {
		if (!checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) {
			if (ind == 1) cout << "nurse " << i << " not feasible" << endl;
			solutionFitness.second = /*(nNurses - nurseWorked2) + (totalHours - hourOK) exceededHour*/ (workingNurses - nurseWorked2) + nNurses;
			return solutionFitness;
		}
	}
	
	//#endif
	
	/* Decoder where the chromosome indicates if a nurse has to work or not (nNurses)*/
	#if 0
  vector<float> chr_nurses_unindexed(chromosome.begin(), chromosome.end());
  
  int workingNurses = 0;
  for (int i=0; i<chr_nurses_unindexed.size(); i++)
    	if (chr_nurses_unindexed[i] > 0.7) workingNurses++;
    	
	for (int i=0; i<totalHours; i++) Hours[i] = demandH[i];
    	
	for (int ii = 0; ii < nNurses; ii++) {
    //for (ii = 0; ii < workingNurses; ii++) {
    	// if chromosome of the nurse is g.t. 0.8 look if the nurse can work
    	
    	if (chr_nurses_unindexed[ii] > 0.7) {
      	makeNurseWork(solutionFitness.first, Hours, ii, minHours, maxHours, maxConsecHours, maxPresenceHours);
      	nurseWorked[ii] = 1;
			}
			
			nurseWorked2++;
				
			if (checkDemandHoursWeek(Hours, demandH)) break;
   }
   
	hourOK = 0;
	for (int i = 0; i < Hours.size(); i++) if (Hours[i] >= demandH[i]) hourOK++;
	
	int hoursLeft = 0;
	for (int i = 0; i < Hours.size(); i++) hoursLeft+= (demandH[i] - min(Hours[i], demandH[i]));
	
	nurseWorked2 = 0;
	for (int i=0; i<nNurses; i++) {
			if (checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) nurseWorked2++;
	}
	
	   
	if (!checkDemandHoursWeek(Hours, demandH)) {
		//solutionFitness.first.clear();
    //solutionFitness.second = INT_MAX;
    //solutionFitness.second = (totalHours - hourOK) + nNurses;
		solutionFitness.second = (totalHours - hourOK) * hoursLeft + nNurses;
		return solutionFitness;
	}
	
	
	for (int i=0; i<nNurses; i++) {
			if (!checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) {
				solutionFitness.second = (nNurses - nurseWorked2) + (totalHours - hourOK) + nNurses;
				return solutionFitness;
			}
	}
	#endif

	/* Decoder where nurses and hours are sorted by the chromosome (totalHours + (Hours * nNurses))*/
	#if 0
	// First we sort the nurses and hours and we store them with their indices
	vector<float> chr_hour_unindexed(chromosome.begin(), chromosome.begin() + totalHours);
	vector<float> chr_nurseHour_unindexed(chromosome.begin() + nNurses, chromosome.end());
	
	vector <std::pair<float, int> > chr_hour_indexed(chr_hour_unindexed.size());
	for (int i = 0; i < chr_hour_indexed.size(); i++) chr_hour_indexed[i] = std::make_pair(chr_hour_unindexed[i], i);
	sort(chr_hour_indexed.begin(), chr_hour_indexed.end(), compareByIndex);
	
	vector <std::pair<float, int> > chr_nurseHour_indexed(nNurses * totalHours);
  	for (int i = 0; i < chr_nurseHour_indexed.size(); i++) {
		int nurse = i%nNurses;
		chr_nurseHour_indexed[i] = std::make_pair(chr_nurseHour_unindexed[i], nurse);
  	}
	//sort(chr_hour_indexed.begin(), chr_hour_indexed.end(), compareByIndex);
	for (int jj = 0; jj < totalHours; jj++) {
		int j = chr_hour_indexed[jj].second;     // Hour
		sort(chr_nurseHour_indexed.begin() + (j * nNurses), chr_nurseHour_indexed.begin() + (j * nNurses) + nNurses, compareByIndex);
			for (int ii = 0; ii < nNurses; ii++) {
				int i = chr_nurseHour_indexed[ii + (j * nNurses)].second;     // Nurse
				int assigned = 0;
				//Try to make this nurse work
				feasible = checkNurseFeasibility(solutionFitness.first, j, i, totalHours, maxHours, maxConsecHours, maxPresenceHours);
				if (feasible && (Hours[j] < demandH[j])) {
					solutionFitness.first[j + (i*totalHours)] = 1;
					Hours[j]++;
					nurseWorked[i] = 1;
				}
				//else that nurse doesn't work
			}
	}
	
	for (int i = 0; i < nNurses; i++) {
		if (!checkFeasibilityAfter(solutionFitness.first, nurseWorked, i, totalHours, minHours)) {
			//solutionFitness.first.clear();
			//solutionFitness.second = INT_MAX;
			solutionFitness.second = (nNurses - nurseWorked2) + nNurses * nNurses;
			return solutionFitness;
		}
		else nurseWorked2++;
	}
	
	feasible = checkDemandHoursWeek(Hours, demandH);
	if (!feasible) {
		//solutionFitness.first.clear();
		//solutionFitness.second = INT_MAX;
    solutionFitness.second = (nNurses - nurseWorked2) + nNurses + 10;
		return solutionFitness;
	}
	#endif

	int sum = 0;
	for (int i=0; i<nNurses; i++) sum += nurseWorked[i];
	solutionFitness.second = sum;
	return solutionFitness;
}

