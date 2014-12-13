/* 
 * File:   RandomNumberGenerator.cpp
 * Author: RM
 * 
 * Created on 31. Juli 2013, 15:03
 */

#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator(long long seed)// : uniformFloatDistribution(RandomNumberGenerator::DISTRIBUTION_MIN, RandomNumberGenerator::DISTRIBUTION_MAX)
{
    generator.seed(seed);
}

RandomNumberGenerator::RandomNumberGenerator(const RandomNumberGenerator& orig)
{
}

RandomNumberGenerator::~RandomNumberGenerator()
{
}

float RandomNumberGenerator::rand()
{
    return uniformFloatDistribution(generator);
}

void RandomNumberGenerator::generateRandomNumbers(vector<float>& container)
{
    //cout << ":: Capacity of container for random numbers is " << container.capacity() << "." << endl;
    for (uint index = 0; index < container.capacity(); index++) {
        //container.push_back(uniformFloatDistribution(generator));
        container[index] = uniformFloatDistribution(generator);
    }
}