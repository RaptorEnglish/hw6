#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        std::vector<HASH_INDEX_T> a(k.size());
        for (size_t i = 0; i < k.size(); i++) {
            a[k.size() - i - 1] = letterDigitToNumber(k[i]);
        }

        // create blocks of 6 and insert into 'w' with padding
        size_t block_size = 6;
        size_t max_size = 5;
        std::vector<unsigned long long> w(max_size);
        for (size_t i = 0; i < max_size; i++) {
            for (size_t j = 0; j < block_size; j++) {
                w[w.size() - i - 1] += std::pow(36, j) * a[(i * block_size) + j];
            }
        }

        // multiply by random vector
        unsigned long long result = 0;
        for (size_t i = 0; i < w.size(); i++) {
            result += rValues[i] * w[i];
        }

        return result;


    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        HASH_INDEX_T index = 0;
        if (std::isalpha(letter)) {
            letter = std::tolower(letter);
            index = letter - 'a';
        } else if (std::isdigit(letter)) {
            index = (letter - '0') + ('z' - 'a') + 1;
        }
        return index;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
