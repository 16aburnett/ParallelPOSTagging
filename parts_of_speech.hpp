// Name:   Amy Burnett
// Course: CSCI 476 - Parallel Programming
// Date:   2020/04/27

// Functions for determining Parts of speech of text 

// ======================================================================================

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <thread>

// ======================================================================================

typedef int POS_t; 
typedef std::vector<POS_t> POSList_t;
typedef std::unordered_map<std::string, POSList_t> LookUpTable;

// ======================================================================================
// Namespace for Part of Speech determining functions and variables 

namespace POS {

    // ==================================================================================
    // Global Constants

    const std::string POS_FILENAME = "dictionary.txt";

    // ==================================================================================
    // Global constants - Part of Speech equivalents 

    const int UNKNOWN      = 0;
    const int NOUN         = 1; 
    const int VERB         = 2;
    const int ADJECTIVE    = 3;
    const int ADVERB       = 4;
    const int PRONOUN      = 5;
    const int PREPOSITION  = 6;
    const int CONJUNCTION  = 7;
    const int INTERJECTION = 8;
    const int DETERMINER   = 9;

    // ==================================================================================
    // Global variables                                    

    LookUpTable g_dictionary;

    // ==================================================================================
    // Initializes the global dictionary for matching words with Parts of Speech

    void
    init () {
        // load file
        std::ifstream file (POS_FILENAME);
        std::string line;

        // read and parse lines
        while (getline (file, line)) {

            // separate word and POS
            std::stringstream ss (line);
            std::string word;
            ss >> word; 

            POSList_t pos;
            
            POS_t p; 

            while (ss >> p) pos.push_back(p);

            // add word to dictionary
            // ignores duplicates
            // adds word and associated POS to list 
            for (int i = 0; i < pos.size(); ++i) {
                g_dictionary[word].push_back(pos[i]);
            }

        }
    }

    // ==================================================================================
    // Gives a string representation for a given POS

    std::string 
    to_string (POS_t pos) {
        if (pos == NOUN)
            return "noun";
        else if (pos == VERB)
            return "verb";
        else if (pos == ADJECTIVE)
            return "adjective";
        else if (pos == ADVERB)
            return "adverb";
        else if (pos == PRONOUN)
            return "pronoun";
        else if (pos == PREPOSITION)
            return "preposition";
        else if (pos == CONJUNCTION)
            return "conjunction";
        else if (pos == INTERJECTION)
            return "interjection";
        else if (pos == DETERMINER)
            return "determiner";
        else return "unknown";
    }

    // ==================================================================================
    // Looks up the part of speech for a single word 
    // Returns a POS_t for the part of speech 
    //    or POS::UNKNOWN if it cannot determine the part of speech

    POSList_t 
    look_up (std::string word) {
        // exists 
        if (g_dictionary.find (word) != g_dictionary.end()) {
            return g_dictionary[word];
        }
        // does not exist 
        else {
            return POSList_t (1, POS::UNKNOWN);
        }
    }

    // ==================================================================================   
    // Looks up the part of speech for all words in the given word vector
    //  and populates the pos vector with the respective parts of speech

    void 
    look_up_all (std::vector<std::string> const & words, std::vector<POSList_t> & pos) {

        // match words with classifications 
        // to get parts of speech
        for (size_t i = 0; i < words.size(); ++i) {
            // determine pos
            pos[i] = look_up (words[i]);

        }

    }

    // ==================================================================================
    // Looks up the part of speech for all words in a range for the given word vector
    //  and populates the pos vector with the respective parts of speech

    void     
    look_up_range (std::vector<std::string> const & words, std::vector<POSList_t> & pos, int low, int high) {

        // match words with classifications 
        // to get parts of speech
        for (size_t i = low; i < high; ++i) {
            // determine pos
            pos[i] = look_up (words[i]);

        }

    }

    // ==================================================================================
    // Looks up the part of speech for all words in the given word vector
    //  and populates the pos vector with the respective parts of speech
    // uses C++ threads to parallelize the process of determining parts of speech
    // num_threads number of threads are spawned

    void 
    look_up_all_parallel (std::vector<std::string> const & words, std::vector<POSList_t> & pos, unsigned num_threads) {

        std::vector<std::thread> threads;

        // create and run threads
        for (int i = 0; i < num_threads; ++i) {
            int low  = words.size() * i / num_threads;
            int high = words.size() * (i + 1) / num_threads;

            threads.emplace_back (POS::look_up_range, std::cref (words), std::ref (pos), low, high);
        }

        // wait for all threads
        for (auto& t : threads) {
            t.join();
        }

    }

    // ==================================================================================

}

// Removes punctuation 
// converts to lowercase
std::string
clean_word (std::string dirty) {

    std::string clean;
    // remove punctuation
    for (char c : dirty) {
        if (isalnum (c)) {
            // make lowercase
            clean += tolower (c);
        }
    }

    return clean;

}