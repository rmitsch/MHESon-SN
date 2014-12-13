/* 
 * File:   main.cpp
 * Author: RM
 *
 * 
 * Created on 26. Juli 2013, 22:45
 * Last Change: 5th of December, 2013
 * 
 * SVN repository: https://code.google.com/p/mheson-sn/
 * Developed in line with the bachelor thesis XYZ (Supervision: MH).
 * 
 * Raphael Mitsch (a1006529)
 * University of Vienna
 * November 2013
 * 
 */

#include <cstdlib>
#include <chrono>
#include <ctime>

#include "Definitions.h"
#include "FileMetadata.h"
#include "InputManagement.h"
#include "CDHS_GMH.h"
#include "MHESonCore.h"


/*
 * 
 */
int main(int argc, char** argv)
{
    // Specify arguments
    const uint numberOfRuns                                             = 100;
    const uint lastPhase                                                = MHESonCoreMetadata::ALL;
    const FileMetadataConstants::FileSpecification fileSpecification    = FileMetadataConstants::DBLP_F;
    const GMHAlgorithm::GMHeuristic gmh                                 = GMHAlgorithm::NEIGHBOURHOOD;
    const GMHAlgorithm::PriceSettingMechanism psm                       = GMHAlgorithm::THIRD_QUARTILE;
  
    ParametrizationSettings::NUMBER_OF_ROUNDS = 10;
    ParametrizationSettings::calculatePostRoundProbability();
    
    // Run simulation with given specifications.
//     MHESonCore::run(fileSpecification, gmh, psm, numberOfRuns, lastPhase);
    
    // Run evaluation of MP distribution.
    MHESonCore::evaluateMPDistribution();
    
    return 0;
}