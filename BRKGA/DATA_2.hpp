#ifndef __DATA_2__
#define __DATA_2__

#include <vector>

#define NUM_TASKS 35
#define NUM_COMPUTERS 4

float TasksArray[NUM_TASKS] = { 67, 83, 53, 58, 76, 89, 70, 72, 51, 93, 50, 72, 68, 50, 57, 55, 76, 91, 76, 92, 52, 59, 92, 91, 85, 67, 85, 93, 67, 91, 58, 57, 92, 70, 88};
vector<float> Tasks(TasksArray, TasksArray + sizeof(TasksArray) / sizeof(float));

float ComputersArray[NUM_COMPUTERS] = { 2938, 1766, 2628, 4578};
vector<float> Computers(ComputersArray, ComputersArray + sizeof(ComputersArray) / sizeof(float));

float CostArray[NUM_COMPUTERS] = { 20, 16, 19, 26};
vector<float> Cost(CostArray, CostArray + sizeof(CostArray) / sizeof(float));

#endif

