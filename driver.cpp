// Name:   Amy Burnett
// Course: CSCI 476 - Parallel Programming
// Date:   2020/04/27

// Main driver file for using the Parts of speech functions 
// Compares parallel performance versus serial performance 

// ======================================================================================

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

// ======================================================================================

#include "parts_of_speech.hpp"
#include "Timer.hpp"

// ======================================================================================

int 
main (int argc, char ** argv) {

    // ensure the proper number of parameters 
    if (argc != 3) {
        printf ("Usage %s <Filename> <num_threads>\n", argv[0]);
        return 0; 
    }

    // read in words to process
    std::ifstream file (argv[1]);
    std::vector<std::string> words; 
    if (file.is_open()) {
        std::string line;
        while (getline (file, line)) {
            // separate by words
            std::stringstream ss (line);
            std::string word;
            while ((ss >> word)) {
                // clean word 
                std::string clean = clean_word (word);
                // insert word 
                words.push_back (clean);
            }
        }
        file.close();
    } 

    printf ("Number of Words: %ld\n", words.size());

    // determine parts of speech
    std::vector<POSList_t> pos (words.size());
    std::vector<POSList_t> pos2 (words.size());

    // initialize POS context
    POS::init();

    // determine POS sequentially 
    Timer<> t_serial;
    POS::look_up_all (words, pos);
    t_serial.stop();

    // determine POS in parallel 
    Timer<> t_parallel;
    POS::look_up_all_parallel (words, pos2, atoi(argv[2]));
    t_parallel.stop();

    // print first 15 of results
    int cap = 15;
    printf ("displaying the first %d words with POS\n", cap);
    printf ("%15s (serial result) (parallel result)\n\n", "word");
    for (int i = 0; i < std::min((int)pos.size(), cap); ++i) {
        printf ("%15s ", (words[i]).c_str());
        printf ("(");
        for (int j = 0; j < pos[i].size(); ++j) {
            printf (" %s", POS::to_string(pos[i][j]).c_str());
        }
        printf (" )\t");
        printf("(");
        for (int j = 0; j < pos2[i].size(); ++j) {
            printf (" %s", POS::to_string(pos2[i][j]).c_str());
        }
        printf (" ) \n");
    }

    // print elapsed time
    printf ("Serial Time %.2f ms\n", t_serial.getElapsedMs());
    printf ("Parallel Time %.2f ms\n", t_parallel.getElapsedMs());

    // ensure serial and parallel match 
    int matches = 1;
    for (int i = 0; i < pos.size(); ++i) {
        if (pos[i].size() != pos2[i].size()) {
            matches = 0;
            break;
        }
        for (int j = 0; j < pos[i].size(); ++j) {
            if (pos[i][j] != pos2[i][j]) {
                matches = 0;
                break;
            }
        }
    }
    if (matches == 1)
        printf ("Serial and Parallel match? yes\n");
    else
        printf ("Serial and Parallel match? no\n");

    return 0;



}

// ======================================================================================
