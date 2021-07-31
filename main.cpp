//  main.cpp
//  CS570 a03
//
//  Created by Jacqueline Gronotte and Jesus Lopez on 2/17/20.

#include <stdio.h>
#include <string>
#include <stdexcept>
#include "wordcount.h"

using namespace std;

/*
 Main function takes a command line argument of the filename to be counted. It calls
 word_count with the name of the file.
 */
int main(int argc, const char * argv[]) {
    if(!argv[1]) { //Throw exception if no file is passed in
        throw runtime_error("Error. No file specified.");
    }
    
    long num_bytes = byteCounter(argv[1]); //number of bytes found from file to be passed into word_count
    word_count(argv[1], num_bytes); //calls function word_count with filename and the number of bytes in the file
    return 0;
}
