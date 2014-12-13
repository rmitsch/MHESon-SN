/* 
 * File:   MHESonCore.h
 * Author: RM
 *
 * Created on 18. August 2013, 00:33
 * 
 * Administrates procedure and stores globally needed variables.
 * Replaces main() as central hub for governing the program / algorithm.
 * 
 */

#ifndef MHESONCORE_H
#define	MHESONCORE_H

#include <cstdlib>
#include <chrono>
#include <ctime>
#include <windows.h>

#include "Definitions.h"
#include "FileMetadata.h"
#include "InputManagement.h"
#include "Graph.h"
#include "AugmentedNeighbourhoodCDHS_GMH.h"
#include "CDHS_GMH.h"
#include "Chaos_GMH.h"
#include "Degrees_GMH.h"

namespace MHESonCoreMetadata
{
    enum Phases {PARSING = 1, IS_PICKING = 2, MP_CALCULATION = 4, GROUP_MAPPING = 8, ITERATIVE_OFFERING = 16, ALL = 32};
}

class MHESonCore
{
    public:
        MHESonCore();
        MHESonCore(const MHESonCore& orig);
        virtual ~MHESonCore();
    
        /**
         * PHASE 0: Preparations
         * Loads settings and makes arrangements for further steps.
         * @param fileSpecification Flag for file to be loaded.
         * @param heuristic Flag indicating which heuristic to use.
         */
        void init(  FileMetadataConstants::FileSpecification fileSpecification, 
                    GMHAlgorithm::GMHeuristic heuristic,
                    GMHAlgorithm::PriceSettingMechanism offerMechanism
                 );
        
        /**
         * PHASES 1 to 6
         * Runs actual algorithm, stops after specified phase.
         * @param lastPhaseIndex Stops simulation after passed phase index, if set.
         * @return Sum of revenues earned.
         */
        float run(const uint lastPhaseIndex = MHESonCoreMetadata::ALL);
        
        /**
         * Creates instance of MHESonCore with given arguments, runs it speci-
         * fied number of times.
         * @param fileSpecification Specifies which file is to be examined.
         * @param heuristic Group mapping heuristic to be applied.
         * @param offerMechanism Offer mechanism to be used.
         * @param numberOfRuns Number of iterations (for statistical purposes).
         * @param lastPhaseIndex Stops simulation after passed phase index, if set.
         * @return Total time elapsed.
         */
        static long long run(   FileMetadataConstants::FileSpecification fileSpecification, 
                                GMHAlgorithm::GMHeuristic heuristic,
                                GMHAlgorithm::PriceSettingMechanism offerMechanism,
                                const uint numberOfRuns,
                                const uint lastPhaseIndex = MHESonCoreMetadata::ALL
                            );
        
        /**
         * Examines sum and average of myopic prices in simulation with given
         * arguments. Stops run after third phase.
         * Varies IS entry probability from 0/0.1 to 1/0.9.
         * Determines MP average and sum after finishing phase 3; then selects
         * dataset-dependent best IS entry probability values.
         * @param fileSpecification
         * @param heuristic
         * @param offerMechanism
         * @param numberOfRuns
         * @return Milliseconds needed for calculations.
         */
        static long long examineMyopicPrices (  FileMetadataConstants::FileSpecification fileSpecification, 
                                                GMHAlgorithm::GMHeuristic heuristic,
                                                GMHAlgorithm::PriceSettingMechanism offerMechanism,
                                                const uint numberOfRuns
                                             );
        
        /**
         * Follows same pattern as examineMyopicPrices() / run(): Run all phases
         * of simulation a given number of times; write protocol in specified
         * location.
         * Designed for phase 2 of evaluation.
         * @param fileSpecification
         * @param heuristic
         * @param offerMechanism
         * @param numberOfRuns
         * @return Milliseconds needed for calculations.
         */
        static long long runTestSeries_P2 ( FileMetadataConstants::FileSpecification fileSpecification, 
                                            GMHAlgorithm::GMHeuristic heuristic,
                                            GMHAlgorithm::PriceSettingMechanism offerMechanism,
                                            const uint numberOfRuns
                                        );
        
                /**
         * Follows same pattern as examineMyopicPrices() / run(): Run all phases
         * of simulation a given number of times; write protocol in specified
         * location.
         * Designed for phase 3 of evaluation.
         * @param fileSpecification
         * @param heuristic
         * @param offerMechanism
         * @param numberOfRuns
         * @return Concatenated string of all results in this run; fitted for
         *         usage in spreadsheet. 
         * 
         */
        static long long runTestSeries_P3 ( FileMetadataConstants::FileSpecification fileSpecification, 
                                            GMHAlgorithm::GMHeuristic heuristic,
                                            GMHAlgorithm::PriceSettingMechanism offerMechanism,
                                            const uint numberOfRuns,
                                            ofstream& resultSummaryFile
                                        );
        
        
        /**
         * Auxiliary function; used for code replacement in main().
         * Runs MHESonCore::runTestSeries_P2().
         */
        static void evaluatePhase2();
        
        /**
         * Auxiliary function; used for code replacement in main().
         * Runs MHESonCore::runTestSeries_P3().
         */
        static void evaluatePhase3();
        
        /**
         * Evaluates distribution of MP sum in groups.
         * Auxiliary function; used for code replacement in main().
         */
        static void evaluateMPDistribution();
        
        /**
         * Preprocesses DBLP data in order to increase efficiency of input parsing.
         * Usage example (in main()):
         *    core.prepareDBLP("../resources/DBLP/text/out.dblp_coauthor", "../resources/DBLP/text/out.dblp_coauthor_inverted_reduced_weighted");
         * @param inputPath
         * @param outputPath
         */
        void prepareDBLP(string inputPath, string outputPath);
        
        /**
         * Getter/setter for _graph.
         * @return Reference to graph used.
         */
        Graph& graph();
        
        /**
         * Getter/setter for _sigmaMP.
         * @return Sum of calculated MPs of all vertices.
         */
        double& sigmaMP();
        
        /**
         * Collects metadata about MP distribution in groups (number of members
         * and MP sum in each group).
         * @return Vector of pairs, containing the number members of each group
         * in the first and the MP sum in the second element.
         */
        vector<pair<uint, float>> getGroupMPMetadata();
        
        /**
         * Auxiliary method - determines file path for protocol files. Used in
         * runTestSeries().
         * @param fileSpecification
         * @param heuristic
         * @param offerMechanism
         * @return Complete file path.
         */
        static string generateLogFilePath(  FileMetadataConstants::FileSpecification fileSpecification,
                                            GMHAlgorithm::GMHeuristic heuristic,
                                            GMHAlgorithm::PriceSettingMechanism offerMechanism);
        
    private:
        /**
         * --- Variables ---
         */
        
        /**
         * File metadata (path, name, estimated number of vertices, ...)
         */
        FileMetadata _fileMetadata;
        
        /**
         * Graph containing all read vertices
         */
        Graph _graph;
        
        /**
         * Heuristic used to calculate myopic prices and map vertices to groups
         */
        GroupMappingHeuristic* _gmHeuristic;
        
        /**
         * Contains vertex IDs of vertices chosen as part of influence set.
         * Realised as unordered set due to its ability to reserve memory
         * in advance.
         */
        unordered_set<uint> _influenceSet;
        
        /**
         * Contains vertex IDs of vertices which accepted the offer. Declining vertices
         * are deleted from original container (graph->vertices()), because no
         * network effects appear.
         */
        unordered_set<uint> _buyerSet;
        
        /**
         * Contains vertex groups
         */
        vector<VertexGroup> _vertexGroups;
        
        /**
         * Sum of all myopic prices (used for statistical/evaluation purposes).
         */
        double _sigmaMP;
        
        
        /**
         * --- Methods ---
         */
        
        /**
         * PHASE 1: Parse input file, process vertex data.
         * Note: Domain of GMH (and descendants).
         * Simple wrapper for InputManagement.parseInputFile().
         * @return Status code (-1 for error, 1 for success).
         */
        int parseInputFile();
        
        /**
         * PHASE 2: Picks influence set. 
         * Note: Domain of GMH (and descendants).
         * @return Number of vertices picked for influence set.
         */
        uint pickInfluenceSet();
        
        /**
         * PHASE 3: Calculates the myopic prices of all vertices.
         * Rearranges vertex collection (sorting, erasing vertices not needed anymore)
         * after calculation.
         * @return Mean myopic price (averaged over all vertices - exempt IS).
         */
        float calculateMyopicPrices();
        
        /**
         * PHASE 4: Map vertices to groups (depends on implementation of GM heuristic).
         * Simple wrapper for GMH::mapGroups().
         * @return Number of non-empty groups.
         */
        uint mapGroups();
        
        /**
         * PHASE 5: Offer product to certain parts (chosen by random) of each 
         * of the k groups in l rounds.
         * @return Revenue made by offering.
         * @see ParametrizationSettings
         */
        float runIterativeOffering();
} ;

#endif	/* MHESONCORE_H */