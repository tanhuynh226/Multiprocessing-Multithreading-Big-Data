#include "p1_threads.h"
using namespace std;

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function

vector<Student> students;

int part = 0;
int thread_count = 0;
int grades_per_thread; 
int offset;

void merge(int l, int m, int h)
{
    Student* left = new Student[m - l + 1];
    Student* right = new Student[h - m];

    int i, j;
    int n1 = m - l + 1;
    int n2 = h - m;
 
    // copy data to temp arrays
    for (i = 0; i < n1; i++)
        left[i] = students[i+l];
    for (i = 0; i < n2; i++)
        right[i] = students[i+m+1];
 
    // remerge temp arrays
    int k = l;
    i = j = 0;
    while (i < n1 && j < n2) {
        if (left[i].grade >= right[j].grade)
            students[k++] = left[i++];
        else
            students[k++] = right[j++];
    }
 
    // copy remaining elements of left[]
    while (i < n1) {
        students[k++] = left[i++];
    }
    // copy remaining elements of right[]
    while (j < n2) {
        students[k++] = right[j++];
    }
}
 
void mergeSort(int l, int h)
{
    // mid point
    int m = l + (h - l) / 2;
    if (l < h) {
        // first half
        mergeSort(l, m);
        // second half
        mergeSort(m + 1, h);
        // merge halves
        merge(l, m, h);
    }
}

void* mergeSort(void* arg){
    int thread_part = part++;
    
    // bounds of low and high
    int l = thread_part * grades_per_thread;
    int h = (thread_part + 1) * grades_per_thread - 1;
    int m = l + (h - l) / 2; // mid point

    if (thread_part == thread_count - 1)
        h += offset;

    if (l < h) {
        mergeSort(l, m);
        mergeSort(m + 1, h);
        merge(l, m, h);
    }
    return NULL;
}

void mergeSections(int num_threads, int aggregation) {
    for(int i = 0; i < num_threads; i += 2) {
        int l = i * (grades_per_thread * aggregation);
        int h = ((i + 2) * grades_per_thread * aggregation) - 1;
        int m = l + (grades_per_thread * aggregation) - 1;
        if (h >= students.size())
            h = students.size() - 1;
        merge(l, m, h); // remerge all the segments into one complete vector
    }
    if (num_threads / 2 >= 1) {
        mergeSections(num_threads / 2, aggregation * 2);
    }
}

void readCSV(string filename){
    ifstream fin;
    fin.open(("./input/" + filename + ".csv").c_str());
    string line;
    getline(fin, line); // skip first line
    while(getline(fin, line)) {
        // create an instance of a student to store studentID and grade
        Student* tempStudent = new Student;
        sscanf(line.c_str(), "%lu,%lf", &tempStudent->studentID, &tempStudent->grade);
        students.push_back(*tempStudent);
    }
    fin.close();
}

void writeCSV(string filename){
    ifstream fin;
    ofstream fout;

    // generate {filename}_sorted.csv file
    fout.open(("./output/" + filename + "_sorted.csv").c_str());
    fout << "Rank," << "Student ID," << "Grade" << "\n";
    for (int i = 0; i < students.size(); i++){
        fout << i+1 << ",";
        fout << students[i].studentID << ",";
        fout << setprecision(12) << students[i].grade << "\n";
    }
    fout.close();

    // generate {filename}_stats.csv file
    fin.open(("./output/" + filename + "_sorted.csv").c_str(),std::ios_base::in);
    fout.open(("./output/" + filename + "_stats.csv").c_str());
    fout << "Average," << "Median," << "Std. Dev" << "\n";
    fout << fixed << setprecision(3) << getMean(fin) << "," << getMedian(fin) << "," << getStdDev(fin);
    fout.close();
}

double getMean(istream& fin){
    // return to the first line of the file
    fin.clear(); 
    fin.seekg(0);

    double mean, sum = 0;
    long int rank; 
    long unsigned int studentID;
    double grade;
    string line;
    getline(fin, line);
    while(getline(fin, line)) {
        sscanf(line.c_str(), "%li,%lu,%lf", &rank, &studentID, &grade);
        sum += grade; // Add up all the grades
    }
    mean = sum / students.size(); // Divide by total number of students
    return mean;
}

double getMedian(istream& fin){
    // return to the first line of the file
    fin.clear();
    fin.seekg(0);

    int count = 0;
    double median;
    long int rank; 
    long unsigned int studentID;
    double grade;
    string line;
    getline(fin, line);
    if (students.size() % 2 != 0) {
        while(getline(fin, line) && count < students.size()/2) { // Iterate through lines until halfway if number of students is odd
            sscanf(line.c_str(), "%li,%lu,%lf", &rank, &studentID, &grade);
            median = grade;
            count++;
        }
    }   
    else {
        while(getline(fin, line) && count < (students.size()-1)/2) { // Iterate through lines until halfway if number of students is even
            sscanf(line.c_str(), "%li,%lu,%lf", &rank, &studentID, &grade);
            median = grade;
            count++;
        }
    }
    return median;
}

double getStdDev(istream& fin){
    double mean = getMean(fin);

    // return to the first line of the file after calling getMean
    fin.clear();
    fin.seekg(0);

    double stdDev = 0.0, temp;
    long int rank; 
    long unsigned int studentID;
    double grade;
    string line;

    getline(fin, line); // skip first line
    while(getline(fin, line)) {
        sscanf(line.c_str(), "%li,%lu,%lf", &rank, &studentID, &grade);
        stdDev += (grade - mean) * (grade - mean);
    }

    return sqrt(stdDev/students.size());
}

void runThread(string filename, int count, int num_threads){
    readCSV(filename);

    thread_count = num_threads;
    grades_per_thread = students.size() / num_threads;
    offset = students.size() % num_threads;

    // create and join threads
    vector<pthread_t> threads;
    pthread_t threadID;
    for (int i = 0; i < num_threads; i++){
        pthread_create(&threadID, NULL, mergeSort, (void*)(intptr_t)i);
        threads.push_back(threadID);
    }
    for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }
    part = 0; // reset part to 0 so that threads for later files can be created

    // merge back threads
    mergeSections(num_threads, 1);

    // write to output and clear students vector
    writeCSV(filename);
    students.clear();
    threads.clear();
}