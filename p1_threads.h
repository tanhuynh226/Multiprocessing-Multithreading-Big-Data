#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include "p1_process.h"
#include <vector>
#include <iomanip>
#include <cmath>

struct Student{
    long unsigned int studentID;
    double grade;
};

void merge(int l, int m, int h);
void mergeSort(int l, int h);
void* mergeSort(void* arg);
void mergeSections(int num_threads, int aggregation);
void readCSV(std::string filename);
void writeCSV(std::string filename);
double getMean(std::istream& fin);
double getMedian(std::istream& fin);
double getStdDev(std::istream& fin);
void runThread(std::string filename, int count, int num_threads);

#endif
