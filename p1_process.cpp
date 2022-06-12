#include "p1_process.h"
#include "p1_threads.h"

using namespace std;

// This file implements the multi-processing logic for the project

void create_processes_and_sort(vector<string> class_names, int num_processes, int num_threads) {
    int classes_per_process = max(class_names.size() / num_processes + (class_names.size() % num_processes > 0), 1ul);

    // Your implementation goes here, you will need to implement:
    // Splitting up work
    //   - Split up the input vector of classes into num_processes sublists
    //
    //   - Make sure all classes are included, remember integer division rounds down
    //
    // Creating child processes
    //   - Each child process will handle one of the sublists from above via process_classes
    //   
    // Waiting for all child processes to terminate

    // saveVariables(num_threads);
    string currentClass;
    pid_t pid;
    for (int i = 0; i < num_processes; i++) {
        pid = fork();
        if (pid == 0){
            printf("Child process is created. (pid: %d)\n", getpid());
            vector<string> subClasses;
            if(i < num_processes - 1) {
	            for(int j = 0; j < classes_per_process && i*classes_per_process+j < class_names.size(); j++)
	                subClasses.push_back(class_names[i*classes_per_process+j]);
            }
            else {
	            for(int j = 0; i*classes_per_process+j < class_names.size(); j++)
	                subClasses.push_back(class_names[i*classes_per_process+j]);
            }
            for (int k = 0; k < subClasses.size(); k++){
                currentClass = subClasses[k];
                runThread(currentClass, k, num_threads);
            }
            printf("Child process is terminated. (pid: %d)\n", getpid());
            exit(0);
        }
    }
    while(wait(NULL) > 0); // main process waits for all children to terminate
}
