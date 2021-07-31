//  wordcount.cpp
//  CS570 a03
//
//  Created by Jacqueline Gronotte and Jesus Lopez on 2/17/20.

#include <iostream>
#include <cwctype>
#include <fstream>
#include <sched.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include "wordcount.h"
#define PROGRESS_BAR_SIZE 40 //The size of the progress bar will be 40 hyphens

using namespace std;

//======================================================================================

/*
 Functions that prints the progress bar to the console.
 */
void print_hyphens(int hyphens) {
    if(hyphens % 10 == 0) { //Print a + every 10th character
        cout << '+';
        cout.flush(); //Request that they be printed immediately
    } else {
        cout << '-';
        cout.flush(); //Request that they be printed immediately
    }
}

//======================================================================================

/*
 The function progress_monitor monitors the progress of a task in a separate thread. The
 function takes in a single argument which is a pointer to the struct we defined above. In
 this function, we output the progress bar, which is displayed as hyphens ("-"), represnting
 the number of bytes we have processed in the file.
 */
void *progress_monitor(void *var) {
    PROGRESS_STATUS my_progress = *(PROGRESS_STATUS *) var;
    long *current = my_progress.CurrentStatus;
    long initial = my_progress.InitialValue;
    long terminate = my_progress.TerminationValue;
    
    float total = terminate - initial; //Size of file
    int processed_hyphens = 0;
    int total_hyphens = 0; //Total amount of hyphens processed
    
    if(terminate == 0) { //If the file is empty
        cout << string(PROGRESS_BAR_SIZE, '-');
        cout.flush(); //Request that they be printed immediately
        pthread_exit(0);
    }
    
    while(total_hyphens < PROGRESS_BAR_SIZE) { //Loop to print 40 hyphens as the file is                                            //processed
        total_hyphens = (((*(current)-initial)/total) * PROGRESS_BAR_SIZE);
        if(total_hyphens > processed_hyphens) {
            print_hyphens(total_hyphens); //Call function to print hyphens
            processed_hyphens = total_hyphens;
        }
    }
    pthread_exit(0);
}

//======================================================================================

/*
 The byteCounter function takes in a single argument of the file and returns the number of
 bytes found in the file.
 */
long byteCounter(const char* file) {
    struct stat buf = {0};
    lstat(file, &buf); //lstat to retrieve the number of bytes in file
    long num_bytes = buf.st_size; //Total size in bytes of the buffer
    return num_bytes; //Return the number of bytes of the file passed in
}

//======================================================================================

/*
 The function word_count returns a long integer with the number of words in the given file.
 The function spawns the progress_monitor thread with a populated PROGRESS_STATUS structure
 as the argument. We read in the file one character at a time and update the number of bytes
 processed and count the number of words found.
 */
long word_count(string file, long filesize) {
    long total_words = 0;
    long curr = 0; //Current amount of bytes that you have read in
    long *current_bytes = &curr; //Pointer to a pointer that passes the value of current
                                 //bytes to current status
    
    PROGRESS_STATUS struct_pointer;
    struct_pointer.CurrentStatus = current_bytes;
    struct_pointer.InitialValue = 0;
    struct_pointer.TerminationValue = filesize;
     _t threadID; //Thread ID
    
    //Creates a new thread and starts progress monitor
    pthread_create(&threadID, NULL, &progress_monitor, (void *)&struct_pointer);
    
     ifstream myfile(file);
        if(myfile.is_open()) { //Checks for empty file
            char word;
            char prev;
            myfile.get(word);
            if(iswspace(word)) { //Checks if the first char is whitespace
                prev = ' '; //Set prev equal to whitespace that way we don't count
                            //whitespace as a word
                curr = curr + sizeof(char); //We've processed one byte of the file
            }
            else {
                prev = 'x'; //Set prev equal to a random char
                curr = curr + sizeof(char); //We've processed one byte of the file
            }
            while(myfile.get(word)) { //Reads each character from the file
                curr = curr + sizeof(char); //count for new line character
                if(iswspace(word) && !isspace(prev)) {
                    //If the current character is white space and previous character is not,
                    //increment total_words
                    total_words = total_words + 1;
                }
                prev = word; //Update the previous char
            }
            if(curr == filesize && !iswspace(word)) //Add one to total_word count if the
                                                    //last char is not a whitespace
                total_words = total_words + 1;
            myfile.close();
        }
        else {
            throw runtime_error("Could not open file."); //Error handling, if the file does not exist
        }
    
    pthread_join(threadID, NULL); //Wait for thread to finish
    cout << '\n' << "There are " << total_words << " words in " << file << "." << '\n';
    return total_words; //Return the total number of words in the file passed in
}
