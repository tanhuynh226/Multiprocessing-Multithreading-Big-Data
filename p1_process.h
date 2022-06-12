#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <vector>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>

void create_processes_and_sort(std::vector<std::string> class_names, int num_processes, int num_threads);

#endif
