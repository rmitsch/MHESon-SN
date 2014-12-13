/* 
 * File:   RandomNumberGenerator.h
 * Author: RM
 *
 * Created on 31. Juli 2013, 15:03
 */

#ifndef RANDOMNUMBERGENERATOR_H
#define	RANDOMNUMBERGENERATOR_H

#include <random>
#include <vector>
#include <iostream>
#include "Definitions.h"

typedef mt19937 MTRNG;

/**
 * Adapted from http://stackoverflow.com/questions/7114043/random-number-generation-in-c11-how-to-generate-how-do-they-work.
 * and http://stackoverflow.com/questions/14009637/c11-random-numbers.
 * Uses <random> from C++11 to create uniform distributions.<br>
 * Currently configured for production of uniformly distributed random float values
 * between 0 and 1.<br>
 * Uses the Mersenne Twister with a popular choice of parameters as RNG.
 */
class RandomNumberGenerator
{
    public:
        RandomNumberGenerator(long long seed = mt19937::default_seed);
        RandomNumberGenerator(const RandomNumberGenerator& orig);
        virtual ~RandomNumberGenerator();
    
        /**
         * Returns next random float from random number generator.
         * @return 
         */
        float rand();
        
        /**
         * Generates container.capacity() random numbers.
         * @param container
         */
        void generateRandomNumbers(vector<float>& container);
        
    private:
        // Range limits for distribution
        static const uint DISTRIBUTION_MIN = 0;
        static const uint DISTRIBUTION_MAX = 1;
        
        // Seed
        uint    seed;
        // Random number generator
        MTRNG   generator;
        // Uniform distribution for float values between DISTRIBUTION_MIN and DISTRIBUTION_MAX
        uniform_real_distribution<float> uniformFloatDistribution   {  uniform_real_distribution<float>(
                                                                            RandomNumberGenerator::DISTRIBUTION_MIN, 
                                                                            RandomNumberGenerator::DISTRIBUTION_MAX)
                                                                    };

} ;

#endif	/* RANDOMNUMBERGENERATOR_H */

