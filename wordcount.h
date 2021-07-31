//  wordcount.h
//  CS570 a03
//
//  Created by Jacqueline Gronotte and Jesus Lopez on 2/17/20.

#ifndef wordcount_h
#define wordcount_h

using namespace std;

/* Struct containing the CurrentStatus, InitialValue and TerminationValue. CurrentStatus is
 a pointer to a long that represents the current status of the computation being tracked
 within the file. InitialValue is the starting value for the computation. TerminationValue
 is the value at which the computation is complete. We can assume that the TerminationValue
 will always be greater than or equal to the CurrentStatus, where CurrentStatus will always
 be greater than or equal to the InitialValue.
*/
typedef struct {
    long *CurrentStatus;
    long InitialValue;
    long TerminationValue;
} PROGRESS_STATUS;

/*
Functions that prints the progress bar to the console.
*/
void print_hyphens(int hyphens);

/*
The function progress_monitor monitors the progress of a task in a separate thread. The
function takes in a single argument which is a pointer to the struct we defined above. In
this function, we output the progress bar, which is displayed as hyphens ("-"), represnting
the number of bytes we have processed in the file.
*/
void *progress_monitor(void *var);

/*
The byteCounter function takes in a single argument of the file and returns the number of
bytes found in the file.
*/
long byteCounter(const char* file); //Counts the number of bytes in a file

/*
The function word_count returns a long integer with the number of words in the given file.
The function spawns the progress_monitor thread with a populated PROGRESS_STATUS structure
as the argument. We read in the file one character at a time and update the number of bytes
processed and count the number of words found.
*/
long word_count(string file, long filesize); //Counts the number of words in a file.

#endif /* wordcount_h */
