#ifndef __DATA_1__
#define __DATA_1__

#include <vector>

#define NUM_TASKS 100
#define NUM_COMPUTERS 4

float TasksArray[NUM_TASKS] = { 67, 83, 53, 58, 76, 89, 70, 72, 51, 93, 50, 72, 68, 50, 57, 55, 76, 91, 76, 92, 52, 59, 92, 91, 85, 67, 85, 93, 67, 91, 58, 57, 92, 70, 88, 82, 81, 63, 98, 100, 65, 50, 55, 52, 70, 94, 66, 72, 78, 62, 54, 58, 68, 61, 69, 63, 79, 81, 89, 64, 53, 50, 64, 53, 85, 64, 81, 88, 79, 85, 76, 72, 54, 60, 66, 57, 65, 61, 50, 80, 92, 80, 60, 100, 75, 54, 56, 71, 80, 60, 58, 58, 54, 96, 87, 90, 95, 71, 90, 50};
vector<float> Tasks(TasksArray, TasksArray + sizeof(TasksArray) / sizeof(float));

float ComputersArray[NUM_COMPUTERS] = { 2938, 1766, 2628, 4578};
vector<float> Computers(ComputersArray, ComputersArray + sizeof(ComputersArray) / sizeof(float));

float CostArray[NUM_COMPUTERS] = { 20, 16, 19, 26};
vector<float> Cost(CostArray, CostArray + sizeof(CostArray) / sizeof(float));

#endif
