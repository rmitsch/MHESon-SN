/* 
 * File:   Definitions.h
 * Author: RM
 *
 * Created on 02. August 2013, 15:40
 * 
 * Contains list of imports, typedefs, macros and other stuff 
 * considered useful and used in every or at least most files.
 * 
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#include "Protocol.h"
#include "ParametrizationSettings.h"

typedef unsigned int    uint;
using namespace std;

/**
 * Custom sort operator, used for preparing DBLP raw data. 
 * Inspired by http://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair.
 */
struct DBLP_Edge_SortPred 
{
    bool operator()(const std::pair<uint, uint> &left, const std::pair<uint, uint> &right) 
    {
        if (left.first == right.first) {
            return left.second < right.second;
        }
        
        return left.first < right.first;
    }
    
    bool operator()(const std::pair<string, string> &left, const std::pair<string, string> &right) 
    {
        if (left.first == right.first) {
            return left.second < right.second;
        }
        
        return left.first < right.first;
    }
};

#endif	/* DEFINITIONS_H */

