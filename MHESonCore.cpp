/* 
 * File:   MHESonCore.cpp
 * Author: RM
 * 
 * Created on 18. August 2013, 00:33
 */

#include "MHESonCore.h"

MHESonCore::MHESonCore()
{
}

MHESonCore::MHESonCore(const MHESonCore& orig)
{
}

MHESonCore::~MHESonCore()
{
    // Free memory used for GMH instance
    delete _gmHeuristic;
}

void MHESonCore::init(FileMetadataConstants::FileSpecification fileSpecification, GMHAlgorithm::GMHeuristic heuristic, GMHAlgorithm::PriceSettingMechanism offerMechanism)
{
    ofstream& protocol = Protocol::protocol();
    chrono::time_point<chrono::high_resolution_clock> start, end;
    
    // Log parametrization settings
    ParametrizationSettings::log();
    
    // Init MHESonCore
    Protocol::introducePhase("Initialization");
    
    // Take start time
    start = chrono::high_resolution_clock::now();
    
    // Load settings for specified file
    _fileMetadata.loadSettings(fileSpecification);
    
    // Set _graph properties
    _graph.init(_fileMetadata.fileName(), _fileMetadata.hasNativeEdgeWeights(), _fileMetadata.isDirected(), _fileMetadata.sizeEstimation());
    
    // Init heuristic
    switch (heuristic) {
        case GMHAlgorithm::CDHS:
            cout << "  :: Chosen heuristic:\t\tCDHS" << endl;
            protocol << "  :: Chosen heuristic:\t\t\tCDHS" << endl;
            
            // Allocate new instance of GMH - CDHS
            _gmHeuristic = new CDHS_GMH(_graph, _influenceSet, _vertexGroups, offerMechanism);
        break;
        
        case GMHAlgorithm::CHAOS:
            cout << "  :: Chosen heuristic:\t\tCHAOS" << endl;
            protocol << "  :: Chosen heuristic:\t\t\tCHAOS" << endl;
            
            // Allocate new instance of GMH - CHAOS
            _gmHeuristic = new Chaos_GMH(_graph, _influenceSet, _vertexGroups, offerMechanism);
        break;

        case GMHAlgorithm::DEGREES:
            cout << "  :: Chosen heuristic:\t\tDEGREES" << endl;
            protocol << "  :: Chosen heuristic:\t\t\tDEGREES" << endl;
            
            // Allocate new instance of GMH - CHAOS
            _gmHeuristic = new Degrees_GMH(_graph, _influenceSet, _vertexGroups, offerMechanism);
        break;
        
        case GMHAlgorithm::NEIGHBOURHOOD:
            cout << "  :: Chosen heuristic:\t\tAN-CDHS - Augmented Neighbourhood CDHS" << endl;
            protocol << "  :: Chosen heuristic:\t\t\tAN_CDHS - Augmented Neighbourhood CDHS" << endl;
            
            // Allocate new instance of GMH - CHAOS
            _gmHeuristic = new AugmentedNeighbourhoodCDHS_GMH(_graph, _influenceSet, _vertexGroups, offerMechanism);
        break;
        
        default:
            cout << "  :: Incorrect heuristic. Exiting program." << endl << endl;
            protocol << "  :: Incorrect heuristic. Exiting program." << endl << endl;
            return;
    }
    
    // Take end time
    end = chrono::high_resolution_clock::now();
    
    cout << "  :: Chosen file:\t\t" << _fileMetadata.fileName() << endl;
    cout << "  :: Native edge weighting:\t" << _fileMetadata.hasNativeEdgeWeights() << endl;
    cout << "  :: Directed:\t\t\t" << _fileMetadata.isDirected() << endl;
    cout << "  :: Estimated vertex count:\t" << _fileMetadata.sizeEstimation() << endl;
    
    protocol << "  :: Chosen file:\t\t\t\t" << _fileMetadata.fileName() << endl;
    protocol << "  :: Native edge weighting:\t\t" << _fileMetadata.hasNativeEdgeWeights() << endl;
    protocol << "  :: Directed:\t\t\t\t\t" << _fileMetadata.isDirected() << endl;
    protocol << "  :: Estimated vertex count:\t" << _fileMetadata.sizeEstimation() << endl;

    Protocol::concludePhase(chrono::duration_cast<chrono::milliseconds>(end - start));
}

float MHESonCore::run(const uint lastPhaseIndex)
{
    ofstream& protocol = Protocol::protocol();
    chrono::time_point<chrono::high_resolution_clock> start, end;
    
    // -------------------------------------------------------------------------
    // 1. Parse Input file
    if (lastPhaseIndex >= MHESonCoreMetadata::PARSING) {
        Protocol::introducePhase("Input Parsing");
        start   = chrono::high_resolution_clock::now();

        if (parseInputFile() < 0)
            return -1;

        end     = chrono::high_resolution_clock::now();
        Protocol::concludePhase(chrono::duration_cast<chrono::milliseconds>(end - start));
    }
    // -------------------------------------------------------------------------
    
    // -------------------------------------------------------------------------
    // 2. Pick influence set
    if (lastPhaseIndex >= MHESonCoreMetadata::IS_PICKING) {
        Protocol::introducePhase("Picking Influence Set");
        start   = chrono::high_resolution_clock::now();

        pickInfluenceSet();

        end     = chrono::high_resolution_clock::now();
        Protocol::concludePhase(chrono::duration_cast<chrono::milliseconds>(end - start));
    }
    // -------------------------------------------------------------------------
    
    
    // -------------------------------------------------------------------------
    // 3. Calculate myopic price of vertices, sort vertices by their myopic prices
    if (lastPhaseIndex >= MHESonCoreMetadata::MP_CALCULATION) {
        Protocol::introducePhase("Calculating Myopic Prices");
        start   = chrono::high_resolution_clock::now();

        calculateMyopicPrices();

        end     = chrono::high_resolution_clock::now();
        Protocol::concludePhase(chrono::duration_cast<chrono::milliseconds>(end - start));
    }
    // -------------------------------------------------------------------------
    
    
    // -------------------------------------------------------------------------
    // 4. Map vertices to groups according to chosen heuristic.
    if (lastPhaseIndex >= MHESonCoreMetadata::GROUP_MAPPING) {
        Protocol::introducePhase("Mapping Vertices to Groups");
        start   = chrono::high_resolution_clock::now();

        mapGroups();

        end     = chrono::high_resolution_clock::now();
        Protocol::concludePhase(chrono::duration_cast<chrono::milliseconds>(end - start));
    }
    // -------------------------------------------------------------------------
    
    
    // -------------------------------------------------------------------------
    // 5. Start iterative offering process (k steps)
    if (lastPhaseIndex >= MHESonCoreMetadata::ITERATIVE_OFFERING) {
        Protocol::introducePhase("Iterative Offering");
        start   = chrono::high_resolution_clock::now();

        return runIterativeOffering();

        end     = chrono::high_resolution_clock::now();
        Protocol::concludePhase(chrono::duration_cast<chrono::milliseconds>(end - start));
    }
    // -------------------------------------------------------------------------
}

long long MHESonCore::run(FileMetadataConstants::FileSpecification fileSpecification, GMHAlgorithm::GMHeuristic heuristic, GMHAlgorithm::PriceSettingMechanism offerMechanism, const uint numberOfRuns, const uint lastPhaseIndex)
{
    // Init stopwatch, take start time
    chrono::time_point<chrono::high_resolution_clock> start, end, totalStart, totalEnd;
    totalStart = chrono::high_resolution_clock::now();
    
    for (uint iterationIndex = 0; iterationIndex < numberOfRuns; iterationIndex++) {
        // Take start time
        start = chrono::high_resolution_clock::now();

        // Set up Protocol class
        Protocol::init("", "log", true);

        // Instance auxiliary class guiding the workflow
        MHESonCore core;

        // -------------------------------------------------------------------------
        // Preparation: Assign file, group mapping heuristic, price setting mechanism
        core.init(fileSpecification, heuristic, offerMechanism);
        // -------------------------------------------------------------------------
        
        // -------------------------------------------------------------------------
        // Run algorithm
        core.run(lastPhaseIndex);
        // -------------------------------------------------------------------------


        // Take end time, print elapsed time
        end         = chrono::high_resolution_clock::now();
        auto millis = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Print time measurements
        cout << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;
        Protocol::protocol() << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;

        // Close protocol file
        Protocol::finalize();
    }
    
    // Take end time, print elapsed time
    totalEnd        = chrono::high_resolution_clock::now();
    auto durationMS = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    return durationMS;
}

long long MHESonCore::examineMyopicPrices(FileMetadataConstants::FileSpecification fileSpecification, GMHAlgorithm::GMHeuristic heuristic, GMHAlgorithm::PriceSettingMechanism offerMechanism, const uint numberOfRuns)
{
    // Init stopwatch
    chrono::time_point<chrono::high_resolution_clock> start, end, totalStart, totalEnd;
    
    // Lower and upper IS entry probability (ISEP) limits; respective interval used.
    const float interval    = 0.1;
    const uint stepCount    = 1 / interval;
    
    // Take start time
    totalStart = chrono::high_resolution_clock::now();
    
    
    // Start measurements: Vary ISEP value
    for (uint stepNumber = 1; stepNumber < stepCount; stepNumber++) {
        // Set ISEP
        ParametrizationSettings::INFLUENCE_SET_PROBABILITY = stepNumber * interval;
        
        // Repeat simulation numberOfRuns times
        for (uint iterationIndex = 0; iterationIndex < numberOfRuns; iterationIndex++) {
            // Take start time
            start = chrono::high_resolution_clock::now();

            // Set up Protocol class
            Protocol::init("", "log/00_MP_EVALUATION", true);

            // Instance auxiliary class guiding the workflow
            MHESonCore core;

            // -------------------------------------------------------------------------
            // Preparation: Assign file, group mapping heuristic, price setting mechanism
            core.init(fileSpecification, heuristic, offerMechanism);
            // -------------------------------------------------------------------------

            // -------------------------------------------------------------------------
            // Run algorithm (phases 1 to 3)
            core.run(MHESonCoreMetadata::MP_CALCULATION);
            // -------------------------------------------------------------------------

            // -------------------------------------------------------------------------
            // Print myopic price data (sum and average over all nodes, including IS)
            cout << "  :: --- MP Evaluation ---" << endl;
            cout << "  ::       - ISEP:\t" << ParametrizationSettings::INFLUENCE_SET_PROBABILITY << endl;
            cout << "  ::       - Sigma:\t" << core.sigmaMP() << endl;
            cout << "  ::       - Average:\t" << core.sigmaMP() / core.graph().getOriginalSize() << endl << endl;
            Protocol::protocol() << "  :: --- MP Evaluation ---" << endl;
            Protocol::protocol() << "  ::       - ISEP:\t\t" << ParametrizationSettings::INFLUENCE_SET_PROBABILITY << endl;
            Protocol::protocol() << "  ::       - Sigma:\t\t" << core.sigmaMP() << endl;
            Protocol::protocol() << "  ::       - Average:\t" << core.sigmaMP() / core.graph().getOriginalSize() << endl << endl << endl;
            cout << endl << endl;
            // -------------------------------------------------------------------------
            

            // Take end time, print elapsed time
            end         = chrono::high_resolution_clock::now();
            auto millis = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            // Print time measurements
            cout << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;
            Protocol::protocol() << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;

            // Close protocol file
            Protocol::finalize();
            
            // Wait one second to avoid identical filenames (and thus 
            // overwriting protocol files) if processing time < one second.
            Sleep(1000);
        }
    }
    
    // Take end time, print elapsed time
    totalEnd        = chrono::high_resolution_clock::now();
    auto durationMS = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    return durationMS;
}

long long MHESonCore::runTestSeries_P2(FileMetadataConstants::FileSpecification fileSpecification, GMHAlgorithm::GMHeuristic heuristic, GMHAlgorithm::PriceSettingMechanism offerMechanism, const uint numberOfRuns)
{
    // In addition to protocol files: Init seperate result file containing all
    // results of one combination (summing up results of all 10 runs)
    ofstream results;
    
    // Init stopwatch
    chrono::time_point<chrono::high_resolution_clock> start, end, totalStart, totalEnd;
    
    // Take start time
    totalStart = chrono::high_resolution_clock::now();
    
    // Set IS probability (default value is 0.5)
    ParametrizationSettings::INFLUENCE_SET_PROBABILITY = 0.5;
    
    // Set up result file.
    string filepath = MHESonCore::generateLogFilePath(fileSpecification, heuristic, offerMechanism);
    // If filename empty: Wrong arguments, terminate application (error message
    // already printed).
    if (filepath.empty())
        return 0;
    
    // Open result file
    results.open(filepath + "/results.txt", ios::out);
    
    // Reprint settings
    results << "-------------------------------------------------" << endl;
    results << "File specification:\t\t\t" << fileSpecification << endl;
    results << "Group mapping heuristic:\t" << heuristic << endl;
    results << "Price setting mechanism:\t" << offerMechanism << endl;
    results << "Number of groups:\t\t\t" << ParametrizationSettings::NUMBER_OF_GROUPS << endl;
    results << "Number of rounds:\t\t\t" << ParametrizationSettings::NUMBER_OF_ROUNDS << endl;
    results << "-------------------------------------------------" << endl;
    
    
    // Start measurements: Repeat simulation numberOfRuns times
    for (uint iterationIndex = 0; iterationIndex < numberOfRuns; iterationIndex++) {
        // Take start time
        start = chrono::high_resolution_clock::now();

        // Set up Protocol class
        Protocol::init("", filepath, true);

        // Instance auxiliary class guiding the workflow
        MHESonCore core;

        // -------------------------------------------------------------------------
        // Preparation: Assign file, group mapping heuristic, price setting mechanism
        core.init(fileSpecification, heuristic, offerMechanism);
        // -------------------------------------------------------------------------

        // -------------------------------------------------------------------------
        // Run algorithm (all phases)
        float revenue = core.run(MHESonCoreMetadata::ALL);
        // -------------------------------------------------------------------------

        // -------------------------------------------------------------------------
        // Print relevant result data: Revenue sum (further data can be 
        // retrieved by evaluating protocol files).
        results << endl << revenue;
        // -------------------------------------------------------------------------


        // Take end time, print elapsed time
        end         = chrono::high_resolution_clock::now();
        auto millis = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Print time measurements
        cout << endl << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;
        Protocol::protocol() << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;

        // Close protocol file
        Protocol::finalize();

        // Wait one second to avoid identical filenames (and thus 
        // overwriting protocol files) if processing time < one second.
        Sleep(1000);
    }
    
    results.close();
    
    // Take end time, print elapsed time
    totalEnd        = chrono::high_resolution_clock::now();
    auto durationMS = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    return durationMS;
}

long long MHESonCore::runTestSeries_P3(FileMetadataConstants::FileSpecification fileSpecification, GMHAlgorithm::GMHeuristic heuristic, GMHAlgorithm::PriceSettingMechanism offerMechanism, const uint numberOfRuns, ofstream& resultSummaryFile)
{
    // In addition to protocol files: Init seperate result file containing all
    // results of one combination (summing up results of all 10 runs)
    ofstream results;
    
    // Init stopwatch
    chrono::time_point<chrono::high_resolution_clock> start, end, totalStart, totalEnd;
    
    // Take start time
    totalStart = chrono::high_resolution_clock::now();
    
    // Set IS probability (default value is 0.5)
    ParametrizationSettings::INFLUENCE_SET_PROBABILITY = 0.5;
    
    // Set up result file.
    string filepath = MHESonCore::generateLogFilePath(fileSpecification, heuristic, offerMechanism);
    // If filename empty: Wrong arguments, terminate application (error message
    // already printed).
    if (filepath.empty())
        return 0;
    
    // Use streams to convert numbers to strings (as to_string() doesn't seem
    // to work yet).
    ostringstream ostr1;
    ostringstream ostr2;
    string rounds = "";
    string groups = "";
    
    ostr1.flush();
    ostr1 << (ParametrizationSettings::NUMBER_OF_ROUNDS + 1);
    rounds = ostr1.str();
    
    ostr2.flush();
    ostr2 << ParametrizationSettings::NUMBER_OF_GROUPS;
    groups = ostr2.str();
    
    // Open result file
    results.open(filepath + "/K" + rounds + "_L" + groups + "_results.txt", ios::out);
    
    // Reprint settings
    results << "-------------------------------------------------" << endl;
    results << "File specification:\t\t\t" << fileSpecification << endl;
    results << "Group mapping heuristic:\t" << heuristic << endl;
    results << "Price setting mechanism:\t" << offerMechanism << endl;
    results << "Number of groups:\t\t\t" << ParametrizationSettings::NUMBER_OF_GROUPS << endl;
    results << "Number of rounds:\t\t\t" << ParametrizationSettings::NUMBER_OF_ROUNDS << endl;
    results << "-------------------------------------------------" << endl;
    
    
    // Start measurements: Repeat simulation numberOfRuns times
    for (uint iterationIndex = 0; iterationIndex < numberOfRuns; iterationIndex++) {
        // Take start time
        start = chrono::high_resolution_clock::now();

        // Set up Protocol class
        Protocol::init("", filepath, true);

        // Instance auxiliary class guiding the workflow
        MHESonCore core;

        // -------------------------------------------------------------------------
        // Preparation: Assign file, group mapping heuristic, price setting mechanism
        core.init(fileSpecification, heuristic, offerMechanism);
        // -------------------------------------------------------------------------

        // -------------------------------------------------------------------------
        // Run algorithm (all phases)
        float revenue = core.run(MHESonCoreMetadata::ALL);
        // -------------------------------------------------------------------------

        // -------------------------------------------------------------------------
        // Print relevant result data: Revenue sum (further data can be 
        // retrieved by evaluating protocol files).
        results << endl << revenue;
        resultSummaryFile << revenue << "+";
        // -------------------------------------------------------------------------


        // Take end time, print elapsed time
        end         = chrono::high_resolution_clock::now();
        auto millis = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Print time measurements
        cout << endl << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;
        Protocol::protocol() << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;

        // Close protocol file
        Protocol::finalize();

        // Wait one second to avoid identical filenames (and thus 
        // overwriting protocol files) if processing time < one second.
        Sleep(1000);
    }
    
    results.close();
    
    // Take end time, print elapsed time
    totalEnd        = chrono::high_resolution_clock::now();
    auto durationMS = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    return durationMS;
}

string MHESonCore::generateLogFilePath( FileMetadataConstants::FileSpecification fileSpecification, GMHAlgorithm::GMHeuristic heuristic, GMHAlgorithm::PriceSettingMechanism offerMechanism)
{
    // Result and log files are stored in third level of folder hierarchy, 
    // depending on used 
    //      (1) GM heuristic, 
    //      (2) file and 
    //      (3) price setting mechanism.
            
    string filepath[3];
    
    // Determine first level of folder hierarchy.
    switch (heuristic) {
        case GMHAlgorithm::CDHS:
            filepath[0] = "01_CDHS";
        break;
        
        case GMHAlgorithm::CHAOS:
            filepath[0] = "02_CHAOS";
        break;

        case GMHAlgorithm::DEGREES:
            filepath[0] = "03_DEGREES";
        break;

        case GMHAlgorithm::NEIGHBOURHOOD:
            filepath[0] = "04_NEIGHBOURHOOD";
        break;
        
        default:
            cout << "Error: Unknown heuristic used as argument in MHESonCore::runTestSeries(). Aborting program." << endl << endl;
        return "";
    }
    
    // Determine second level of folder hierarchy.
    switch (fileSpecification) {
        case FileMetadataConstants::ADVOGATO_F:
            filepath[1] = "Advogato";
        break;
        
        case FileMetadataConstants::DBLP_F:
            filepath[1] = "DBLP";
        break;

        default:
            cout << "Error: Illegal file specification used as argument in MHESonCore::runTestSeries(). Aborting program." << endl << endl;
        return "";
    }
    
    // Determine second level of folder hierarchy.
    switch (offerMechanism) {
        case GMHAlgorithm::PriceSettingMechanism::MIN:
            filepath[2] = "MIN";
        break;
        
        case GMHAlgorithm::PriceSettingMechanism::FIRST_QUARTILE:
            filepath[2] = "FIRST_QUARTILE";
        break;

        case GMHAlgorithm::PriceSettingMechanism::MEDIAN:
            filepath[2] = "MEDIAN";
        break;
        
        case GMHAlgorithm::PriceSettingMechanism::THIRD_QUARTILE:
            filepath[2] = "THIRD_QUARTILE";
        break;

        case GMHAlgorithm::PriceSettingMechanism::MAX:
            filepath[2] = "MAX";
        break;

        case GMHAlgorithm::PriceSettingMechanism::RANDOM:
            filepath[2] = "RANDOM";
        break;
        
        default:
            cout << "Error: Illegal file specification used as argument in MHESonCore::runTestSeries(). Aborting program." << endl << endl;
        return ""; 
    }
    
    // Return complete filepath.
    return "log/" + filepath[0] + "/" + filepath[1] + "/" + filepath[2];
}

int MHESonCore::parseInputFile()
{
    return InputManagement::parseInputFile(_fileMetadata.inputPath(), _fileMetadata.fileType(), _graph);
}

uint MHESonCore::pickInfluenceSet()
{
    ofstream& protocol = Protocol::protocol();
    
    cout << "  :: Probability x:\t\t" << ParametrizationSettings::INFLUENCE_SET_PROBABILITY << endl;
    protocol << "  :: Probability x:\t\t\t\t" << ParametrizationSettings::INFLUENCE_SET_PROBABILITY << endl;
    
    // Pick influence set
    _gmHeuristic->pickInfluenceSet();
            
    cout << "  :: Number of vertices in IS:\t" << _influenceSet.size()
         << " (" << static_cast<float>(_influenceSet.size()) * 100 / _graph.vertices().size() << "%)" << endl;
    protocol << "  :: Number of vertices in IS:\t" << _influenceSet.size() 
             << " (" << static_cast<float>(_influenceSet.size()) * 100 / _graph.vertices().size() << "%)" << endl;
    
    return _influenceSet.size();
}

float MHESonCore::calculateMyopicPrices()
{
    // Deliver references to IS, BS and vertex ID map to Vertex class
    Vertex::setIS(_influenceSet);
    Vertex::setBS(_buyerSet);
    Vertex::setVertexIDMap(_graph.vertexIDMap());
    
    // Calculate myopic prices
    _sigmaMP        = _gmHeuristic->calculateMyopicPrices();
    float averageMP = _sigmaMP / (_graph.vertices().size() - _influenceSet.size());
    
    // Log sum of myopic prices and their average (median is calculated / logged
    // seperately).
    cout << "  :: Myopic price sum:\t\t\t\t" << _sigmaMP << endl;
    Protocol::protocol() << "  :: Myopic price sum:\t\t\t\t\t\t\t" << _sigmaMP << endl;
    cout << "  :: Average myopic price:\t\t\t" << averageMP << endl;
    Protocol::protocol() << "  :: Average myopic price:\t\t\t\t\t\t" << averageMP << endl;

    // Sort and clean vertex collection
    _gmHeuristic->restructureDataset();
    
    return averageMP;
}

uint MHESonCore::mapGroups()
{
    return _gmHeuristic->mapGroups();
}

float MHESonCore::runIterativeOffering()
{
    // Protocol file
    ofstream& protocol = Protocol::protocol();
    
    // Total sum of revenues.
    float                   revenue                                     = 0;
    
    // Threshold values for probabilities of vertex 1) to get included in 
    // this round and 2) to take up offer.
    float                   entryProbabilityThreshold                   = 0;
    float                   buyingProbabilityThreshold                  = 0;

    // Various counters; used for statistical purposes).
    uint                    countProcessedTotal                         = 0;
    uint                    countProcessedCurrent                       = 0;
    uint                    countPurchasedCurrent                       = 0;
    vector<vector<uint>>    countPurchased(ParametrizationSettings::NUMBER_OF_ROUNDS);
    vector<vector<uint>>    countProcessed(ParametrizationSettings::NUMBER_OF_ROUNDS);
    vector<vector<float>>   revenues(ParametrizationSettings::NUMBER_OF_ROUNDS);
    const uint              randomCount                                 = 10000;
    
    // Further allocate and initialize statistically used arrays
    for (uint outerIndex = 0; outerIndex < ParametrizationSettings::NUMBER_OF_ROUNDS; outerIndex++) {
        countPurchased[outerIndex].resize(ParametrizationSettings::NUMBER_OF_GROUPS);
        countProcessed[outerIndex].resize(ParametrizationSettings::NUMBER_OF_GROUPS);
        revenues[outerIndex].resize(ParametrizationSettings::NUMBER_OF_GROUPS);
        
        for (uint innerIndex = 0; innerIndex < ParametrizationSettings::NUMBER_OF_GROUPS; innerIndex++) {
            countPurchased[outerIndex][innerIndex]  = 0;
            countProcessed[outerIndex][innerIndex]  = 0;
            revenues[outerIndex][innerIndex]        = 0;
        }   
    }
    
    // Collections
    vector<Vertex>&         vertices                                    = _graph.vertices();
    vector<float>           purchaseProbabilities;
    // Used for temporary storage of new buyers.
    // Data is shifted to usual buyer set after round is completed.
    unordered_set<uint>     tempBuyerSet;
    
    // Random number generator
    RandomNumberGenerator   rng(chrono::system_clock::now().time_since_epoch().count());
    
    // Get one random number between 0 and 1 for each vertex
    purchaseProbabilities.resize(randomCount);
    rng.generateRandomNumbers(purchaseProbabilities);


    // Calculate probability for this round (1 - x) / k
    entryProbabilityThreshold = (1 - ParametrizationSettings::INFLUENCE_SET_PROBABILITY) / ParametrizationSettings::NUMBER_OF_ROUNDS;
        
    // Iterate rounds
    for (uint roundIndex = 1; roundIndex <= ParametrizationSettings::NUMBER_OF_ROUNDS; roundIndex++) {
        
        cout << endl << "  :: --- Round #" << roundIndex << " --- " << endl;
        cout << "  ::     p = " << entryProbabilityThreshold << endl << endl;
        protocol << endl << "  :: --- Round #" << roundIndex << " --- " << endl;
        protocol << "  ::     p = " << entryProbabilityThreshold << endl << endl;
        
        // Iterate groups in each round
        for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
            
            cout << "  ::    Group #" << groupIndex << endl;
            protocol << "  ::    Group #" << groupIndex << endl;
            
            if (!_vertexGroups[groupIndex].getMemberCount()) {
                cout << "  ::    Is empty, no offers submitted." << endl;
                protocol << "  ::    Is empty, no offers submitted." << endl;
            }
            
            else {
                // Iterate vertices in each group
                for (uint index = _vertexGroups[groupIndex].firstIndex(); index >= _vertexGroups[groupIndex].lastIndex(); index--) {
                    // If vertex not yet processed and we are lucky enough
                    if (vertices[index].roundAssignation() == roundIndex) {
                        // Increment counter
                        countProcessedTotal++;
                        countProcessed[roundIndex - 1][groupIndex]++;
                        
                        // Check influential neighbour vertices via _buyersMap (in round 1
                        // aquivalent to IS) and calculate buying probability P_b = P_g / P_n.
                        // E.g.: P_g = 8, P_n = 10. P_b = 8 / 10 = 0.8, which represents
                        // a 0.2 chance of agent purchasing the product. All random
                        // numbers >= 0.8 therefore imply/trigger an acquisition.
                        buyingProbabilityThreshold = _vertexGroups[groupIndex].getOffer() / vertices[index].calculatePriceLimit();

                        // Use said ratio as probability threshold for determining if 
                        // vertex / agent is to accept offer. E. g.: If upper price limit 
                        // is 10 and group offer is 8, there is a 2 in 10 chance for 
                        // a successful transaction.
                        // If random number is higher than threshold/ratio: Product bought.
                        // Else: Do nothing, as vertex is already marked as processed
                        // and will be ignored in following rounds.
                        if (purchaseProbabilities[index % randomCount] >= buyingProbabilityThreshold) {
                             // Mark vertex as buyer
                            tempBuyerSet.insert(vertices[index].vertexID());

                            // Collect revenue
                            revenue += _vertexGroups[groupIndex].getOffer();

                            // Increment counter
                            countPurchased[roundIndex - 1][groupIndex]++;
                        }
                    }

                    if (!index)
                        break;
                }
            
                // Store revenues for current group in current road
                revenues[roundIndex - 1][groupIndex] = countPurchased[roundIndex - 1][groupIndex] * _vertexGroups[groupIndex].getOffer();

                // Make log more readable
                countProcessedCurrent = countProcessed[roundIndex - 1][groupIndex];
                countPurchasedCurrent = countPurchased[roundIndex - 1][groupIndex];

                
                // Log
                cout << "  ::       - Group offer:\t\t\t" << _vertexGroups[groupIndex].getOffer() << endl;
                protocol << "  ::       - Group offer:\t\t\t\t\t" << _vertexGroups[groupIndex].getOffer() << endl;

                cout << "  ::       - Number of submitted offers:\t" << countProcessedCurrent << " (" 
                     << 100 * countProcessedCurrent / static_cast<float>(_vertexGroups[groupIndex].getMemberCount()) << "% of all vertices in group)" << endl;
                protocol << "  ::       - Number of submitted offers:\t" << countProcessedCurrent << " (" 
                         << 100 * countProcessedCurrent / static_cast<float>(_vertexGroups[groupIndex].getMemberCount()) << "% of all vertices in group)" << endl;


                if (countProcessedCurrent) {
                    cout << "  ::       - Number of accepted offers:\t\t" << countPurchasedCurrent << " ("
                         << 100 * countPurchasedCurrent / static_cast<float>(countProcessedCurrent) << "% success rate)" << endl;
                    protocol << "  ::       - Number of accepted offers:\t\t" << countPurchasedCurrent << " ("
                             << 100 * countPurchasedCurrent / static_cast<float>(countProcessedCurrent) << "% success rate)" << endl;
                }
                else {
                    cout << "  ::       - Number of accepted offers:\t\t" << countPurchasedCurrent << " (0% success rate)" << endl;
                    protocol << "  ::       - Number of accepted offers:\t\t" << countPurchasedCurrent << " (0% success rate)" << endl;
                }

                cout << "  ::       - Revenue:\t\t\t\t" << revenues[roundIndex - 1][groupIndex] << endl;
                protocol << "  ::       - Revenue:\t\t\t\t\t\t" << revenues[roundIndex - 1][groupIndex] << endl;
            }

            // Copy temporary buyer set in persistent collection, clear temp. buyer
            // set (done so only vertices processed in past rounds may impact/influenc
            // buying decisions in current round).
            _buyerSet.insert(tempBuyerSet.begin(), tempBuyerSet.end());
            tempBuyerSet.clear();

            // Shuffle random numbers (obviate permanent index-probability linkage)
            random_shuffle(purchaseProbabilities.begin(), purchaseProbabilities.end());

            cout << endl;
            protocol << endl;
        }
    }
    
    
    // Log summary
    cout << endl << "  :: --- Summary ---" << endl;
    cout << "  ::       - " << "Purchased by " << 100 * _buyerSet.size() / static_cast<float>(_graph.getOriginalSize()) << "% of all agents ("
         << 100 * _buyerSet.size() / static_cast<float>(_graph.vertices().size()) << "% when considering IS and passive vertices)." << endl;    
    cout << "  ::       - Number of submitted offers:\t\t\t" << countProcessedTotal << " (" 
         << 100 * countProcessedTotal / static_cast<float>(_graph.vertices().size()) << "% of all vertices exempt IS)." << endl;
    cout << "  ::       - Number of accepted offers:\t\t\t\t" << _buyerSet.size() << " ("
         << 100 * _buyerSet.size() / static_cast<float>(countProcessedTotal) << "% success rate)" << endl;
    cout << "  ::       - Earned revenues:\t\t\t\t\t" << revenue << endl;
    cout << "  ::       - Average revenues (per agent):\t\t\t" << revenue / _graph.getOriginalSize() << endl;
    cout << "  ::       - Average revenues (per purchasing agent / client):\t" << revenue / _buyerSet.size() << endl;
    
    protocol << endl << "  :: --- Summary ---" << endl;
    protocol << "  ::       - " << "Purchased by " << 100 * _buyerSet.size() / static_cast<float>(_graph.getOriginalSize()) << "% of all agents ("
         << 100 * _buyerSet.size() / static_cast<float>(_graph.vertices().size()) << "% when ignoring IS)." << endl;    
    protocol << "  ::       - Number of submitted offers:\t\t\t\t\t\t" << countProcessedTotal << " (" 
         << 100 * countProcessedTotal / static_cast<float>(_graph.vertices().size()) << "% of all vertices exempt IS)." << endl;
    protocol << "  ::       - Number of accepted offers:\t\t\t\t\t\t\t" << _buyerSet.size() << " ("
         << 100 * _buyerSet.size() / static_cast<float>(countProcessedTotal) << "% success rate)" << endl;
    protocol << "  ::       - Earned revenues:\t\t\t\t\t\t\t\t\t" << revenue << endl;
    protocol << "  ::       - Average revenues (per agent):\t\t\t\t\t\t" << revenue / _graph.getOriginalSize() << endl;
    protocol << "  ::       - Average revenues (per purchasing agent / client):\t" << revenue / _buyerSet.size() << endl;
    
    return revenue;
}

Graph& MHESonCore::graph()
{
    return _graph;
}

double& MHESonCore::sigmaMP()
{
    return _sigmaMP;
}

void MHESonCore::prepareDBLP(string inputPath, string outputPath)
{
    InputManagement::prepareDBLP(inputPath, outputPath);
}

void MHESonCore::evaluatePhase2()
{
    // Specify arguments
    const uint numberOfRuns                                             = 10;
    const FileMetadataConstants::FileSpecification fileSpecification    = FileMetadataConstants::DBLP_F;
    const GMHAlgorithm::GMHeuristic gmh                                 = GMHAlgorithm::NEIGHBOURHOOD;
    const GMHAlgorithm::PriceSettingMechanism psm                       = GMHAlgorithm::THIRD_QUARTILE;
    
    
    // Evaluation, phase 2: Run all possible combinations of GMH and PSM.
    for (uint currGMH = GMHAlgorithm::CDHS; currGMH <= GMHAlgorithm::NEIGHBOURHOOD; currGMH++) {
        for (uint currPSM = GMHAlgorithm::MIN; currPSM <= GMHAlgorithm::RANDOM; currPSM++) {
            MHESonCore::runTestSeries_P2(   fileSpecification,
                                            static_cast<GMHAlgorithm::GMHeuristic> (currGMH),
                                            static_cast<GMHAlgorithm::PriceSettingMechanism> (currPSM),
                                            numberOfRuns
                                        );
        }
    }
}

void MHESonCore::evaluatePhase3()
{
    // Specify arguments
    const uint numberOfRuns                                             = 10;
    const FileMetadataConstants::FileSpecification fileSpecification    = FileMetadataConstants::DBLP_F;
    const GMHAlgorithm::GMHeuristic gmh                                 = GMHAlgorithm::NEIGHBOURHOOD;
    const GMHAlgorithm::PriceSettingMechanism psm                       = GMHAlgorithm::MIN;
    // Advogato: 6551 nodes -> log_2(n / 2) = 11.677499...  => 12
    // DBLP: 1248427 nodes -> log_2(n / 2) = 19.251680...   => 20
    const vector<uint> roundNumbers                                     = {2 - 1, 3 - 1, 5 - 1, 10 - 1, 20 - 1, 50 - 1};
    const vector<uint> groupNumbers                                     = {1, 2, 5, 10, 20};
    
    
    // Evaluation, phase 3: Run CDHS/MIN and AN/TQ with given round and group numbers.
    ofstream resultSummary;
    string filepath = MHESonCore::generateLogFilePath(fileSpecification, gmh, psm);
    resultSummary.open(filepath + "/resultSummary.txt", ios::out);
    
    for (uint roundNumber : roundNumbers) {
        // Set number of rounds, calculate post round probability.
        ParametrizationSettings::NUMBER_OF_ROUNDS = roundNumber;
        ParametrizationSettings::calculatePostRoundProbability();
        
        for (uint groupNumber : groupNumbers) {
            ParametrizationSettings::NUMBER_OF_GROUPS = groupNumber;
        
            resultSummary << "::: k = " << roundNumber + 1 << " | l = " << groupNumber << endl << "=(";
            
            // Run test series
            MHESonCore::runTestSeries_P3(  fileSpecification,
                                            static_cast<GMHAlgorithm::GMHeuristic>(gmh),
                                            static_cast<GMHAlgorithm::PriceSettingMechanism>(psm),
                                            numberOfRuns,
                                            resultSummary
                                        );
            
            resultSummary << ")/" << numberOfRuns <<endl;
        }
        
        resultSummary << endl << endl;
    }
    
    resultSummary.close();
}

void MHESonCore::evaluateMPDistribution()
{
    // Specify arguments
    const uint numberOfRuns                                             = 10;
    const FileMetadataConstants::FileSpecification fileSpecification    = FileMetadataConstants::DBLP_F;
    const GMHAlgorithm::GMHeuristic gmh                                 = GMHAlgorithm::NEIGHBOURHOOD;
    const GMHAlgorithm::PriceSettingMechanism psm                       = GMHAlgorithm::MIN;
    // Advogato: 6551 nodes -> log_2(n / 2) = 11.677499...  => 12
    // DBLP: 1248427 nodes -> log_2(n / 2) = 19.251680...   => 20
    // Set numbers of groups and rounds, calculate post round probability (not
    // necessary though, as not relevant until phase 5).
    ParametrizationSettings::NUMBER_OF_GROUPS                           = 12;
    ParametrizationSettings::NUMBER_OF_ROUNDS                           = 2;
    ParametrizationSettings::calculatePostRoundProbability();
    
    // Save results (note: due to unexpexpected program behaviour, average is 
    // calculated in-programm - instead of collecting all single results and 
    // averaging them in result spreadsheet as usual).
    vector<uint> groupSizes(ParametrizationSettings::NUMBER_OF_GROUPS);
    vector<float> groupMPSums(ParametrizationSettings::NUMBER_OF_GROUPS);
    
    // Init stopwatch
    chrono::time_point<chrono::high_resolution_clock> start, end, totalStart, totalEnd;
    
    // Take start time
    totalStart = chrono::high_resolution_clock::now();
    
    // Set IS probability (default value is 0.5)
    ParametrizationSettings::INFLUENCE_SET_PROBABILITY = 0.5;
    
    // Evaluate distribution of MP sums in groups.
    ofstream resultSummary;
    resultSummary.open("log/MPEvaluation_resultSummary.txt", ios::out);
    
    for (uint currGMH = GMHAlgorithm::CDHS; currGMH <= GMHAlgorithm::NEIGHBOURHOOD; currGMH++) {
        // Start log
        resultSummary << "::: GMH = " << currGMH << " | l = " << ParametrizationSettings::NUMBER_OF_GROUPS << endl << endl;

        // Run test series
        for (uint iterationIndex = 0; iterationIndex < numberOfRuns; iterationIndex++) {
            // Set up Protocol class
            Protocol::init("", "log", true);

            // Take start time
            start = chrono::high_resolution_clock::now();
            
            // Instance auxiliary class guiding the workflow
            MHESonCore core;

            // -------------------------------------------------------------------------
            // Preparation: Assign file, group mapping heuristic, price setting mechanism
            core.init(fileSpecification, static_cast<GMHAlgorithm::GMHeuristic>(currGMH), psm);
            // -------------------------------------------------------------------------

            // -------------------------------------------------------------------------
            // Run algorithm (all phases)
            core.run(MHESonCoreMetadata::GROUP_MAPPING);
            // -------------------------------------------------------------------------

            // -------------------------------------------------------------------------
            // Get group information (number of members in group, MP sum) from core
            vector<pair<uint, float>> metadataVector = core.getGroupMPMetadata();
            for (uint index = 0; index < ParametrizationSettings::NUMBER_OF_GROUPS; index++) {
                groupSizes[index]   += metadataVector[index].first;
                groupMPSums[index]  += metadataVector[index].second;
            }
            // -------------------------------------------------------------------------
            

            // Take end time, print elapsed time
            end         = chrono::high_resolution_clock::now();
            auto millis = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            // Print time measurements
            cout << endl << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;
            Protocol::protocol() << "***** Elapsed time in total: " << millis << " ms *****" << endl << endl << endl;

            // Close protocol file
            Protocol::finalize();

            // Wait one second to avoid identical filenames (and thus 
            // overwriting protocol files) if processing time < one second.
            Sleep(1000);
        }
        
        
        resultSummary << " :: Group MP sums:" << endl;
        // Print group MP sums for all groups of this GMH
        for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
            // Print resulting MP sums
            resultSummary << "=(";
            resultSummary << groupMPSums[groupIndex];
            resultSummary << "/" << numberOfRuns << ")\t";
            
            // Reset group MP sum vector
            groupMPSums[groupIndex] = 0;
        }
        
        resultSummary << endl;
        
        resultSummary << " :: Group sizes:" << endl;
        // Print group sizes for all groups of this GMH
        for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
            // Print resulting group sizes
            resultSummary << "=(";
            resultSummary << groupSizes[groupIndex];
            resultSummary << "/" << numberOfRuns << ")\t";
            
            // Reset group size vector
            groupSizes[groupIndex] = 0;
        }
        
        resultSummary << endl << endl;
    }

    resultSummary.close();
    
    // Take end time, print elapsed time
    totalEnd        = chrono::high_resolution_clock::now();
    auto durationMS = chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

vector<pair<uint, float>> MHESonCore::getGroupMPMetadata()
{
    vector<pair<uint, float>> results;
    vector<Vertex>& vertices    = _graph.vertices();
    float tempMPSum             = 0;
    
    
    for (VertexGroup& vertexGroup : _vertexGroups) {
        // Sum up myopic prices
        for (size_t index = vertexGroup.lastIndex(); index <= vertexGroup.firstIndex(); index++) {
            tempMPSum += vertices[index].myopicPrice();
        }
        
        // Push information in result vector, reset temporary MP sum.
        results.push_back(pair<uint, float>(vertexGroup.firstIndex() - vertexGroup.lastIndex() + 1, tempMPSum));
        tempMPSum = 0;
    }
    
    return results;
}