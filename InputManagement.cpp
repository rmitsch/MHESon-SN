/* 
 * File:   InputManagement.cpp
 * Author: RM
 * 
 * Created on 26. Juli 2013, 22:46
 */

#include "InputManagement.h"
#include "RandomNumberGenerator.h"

InputManagement::InputManagement()
{
}

InputManagement::InputManagement(const InputManagement& orig)
{
}

InputManagement::~InputManagement()
{
}

// Clarification: Preprocessed input files have source IDs in right column,
// destination IDs in left column - and are sorted according to the
// destination IDs.
// I. e.: Edges recorded within a vertex are _incoming_ edges.
// Vertices covered in Graph.scanForPassiveVertices() therefore are vertices with an
// arbitrary number of outgoing, but zero incoming edges.
int InputManagement::parseInputFile(string inputPath, FileMetadataConstants::DataFileType fileType, Graph& graph)
{
    ofstream& protocol = Protocol::protocol();
    cout << "  :: Input path is " << inputPath << "." << endl;
    protocol << "  :: Input path is " << inputPath << "." << endl;
    
    // Open file
    ifstream inputFile;
    inputFile.open(inputPath.c_str(), ios::in);
    
    if (!inputFile.is_open()) {
        cout << "  -- Failed opening file; exiting program." << endl << endl;
        protocol << "  -- Failed opening file; exiting program." << endl << endl;
        
        return -1;
    }
    
    // Call parsing method fitted to file type
    switch (fileType) {
        case FileMetadataConstants::DBLP:
            cout << "  :: Type of examined data file: DBLP" << endl;
            protocol << "  :: Type of examined data file: DBLP" << endl;
            parseDBLPFile(inputFile, graph);
        break;
        
        case FileMetadataConstants::ADVOGATO:
            cout << "  :: Type of examined data file: ADVOGATO" << endl;
            protocol << "  :: Type of examined data file: ADVOGATO" << endl;
            InputManagement::parseADVOGATOFile(inputFile, graph);
        break;
        
        case FileMetadataConstants::SNAP:
            cout << "  :: Type of examined data file: SNAP" << endl;
            protocol << "  :: Type of examined data file: SNAP" << endl;
            InputManagement::parseSNAPFile(inputFile, graph);
        break;
        
        default:
            cout << "  :: Unknown file type; exiting program.";
            protocol << "  :: Unknown file type; exiting program.";
    }
    
    cout << "  :: Graph status " << endl;
    cout << "  ::   Size:\t\t\t" << graph.vertices().size() << endl;
    cout << "  ::   Capacity:\t\t" << graph.vertices().capacity() << endl;
    cout << "  ::   Passive/deaf vertices:\t" << graph.passiveVertexCount() << endl;
    
    protocol << "  :: Graph status " << endl;
    protocol << "  ::   Size:\t\t\t\t\t" << graph.vertices().size() << endl;
    protocol << "  ::   Capacity:\t\t\t\t" << graph.vertices().capacity() << endl;
    protocol << "  ::   Passive/deaf vertices:\t" << graph.passiveVertexCount() << endl;
    
    return 1;
}

uint InputManagement::parseSNAPFile(ifstream& inputFile, Graph& graph)
{
    // Strings for storing input temporarily
    string                  currentLine = "#";
    string                  tempID;
    
    // Vertex IDs
    int                     lastFID     = -2;
    int                     firstID     = -1;
    int                     secondID    = -1;
    // Number of processed vertices
    int                     count       = 0;
    
    // Temporary collection for edges of current vertex
    vector<Edge>            edges;
    // Current vertex (storing all read information)
    Vertex                    currVertex;
    // Vertex collection
    vector<Vertex>&           vertices       = graph.vertices();
    // Vertex map
    map<uint, uint>&        vertexIDMap   = graph.vertexIDMap();
    
    // Container for random numbers for edge weights.
    vector<float>           edgeWeights;
    // Container for vertex-round assignations
    vector<float>           roundAssignations;
    // Random number generator
    RandomNumberGenerator   rng(chrono::system_clock::now().time_since_epoch().count());
     
    
    // Allocating memory and generating random numbers
    edgeWeights.reserve(InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS);
    roundAssignations.reserve(InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS);
    rng.generateRandomNumbers(edgeWeights);
    rng.generateRandomNumbers(roundAssignations);
    
    // Skip comment lines
    streampos position;
    do {
        position = inputFile.tellg();    
        getline(inputFile, currentLine);
    }
    while (!inputFile.eof() && currentLine[0] == '#');
    // Jump back to begin of first content line
    inputFile.seekg(position);
    
    
    // Parse actual data
    while (!inputFile.eof()) {
        tempID.clear();
        
        // Note: g++ stubbornly refuses knowledge of std::stoi(),
        // so atoi() is used here instead.
        
        // Read first ID
        inputFile >> tempID;
        // Check for blank line
        if (tempID.size()) {
            // Convert first ID to integer
            firstID = atoi(tempID.c_str());
            // Read second ID
            inputFile >> tempID;
            // Convert second ID to integer
            secondID = atoi(tempID.c_str());

            // New source (vertex) detected - push old vertex to graph, 
            // reset edge collection
            if (lastFID != firstID) {
                // Add old vertex to collection
                if (lastFID >= 0) {
                    // Swap edge data with currVertex edge data
                    currVertex.swapEdges(edges);
                    
                    // Determine round in which vertex will be processed
                    currVertex.roundAssignation() = roundAssignations[count % InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS] 
                                                  * ParametrizationSettings::NUMBER_OF_ROUNDS + 1;
                    
                    // Add currVertex to vertex collection
                    vertices.push_back(currVertex);
                    
                    // Map vertex (ID is key, index is value)
                    vertexIDMap[currVertex.vertexID()] = count;
                }

                // Set new vertex ID
                currVertex.vertexID() = firstID;
                
                // Delete currently stored edges
                edges.clear();
            }

            // Add edge between firstID and secondID to edge collection.
            // Weight is taken from randomly generated edge weights.
            edges.push_back(Edge(currVertex.vertexID(), secondID, edgeWeights[count % InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS]));
            
            // Set new source ID, iterate counter
            lastFID = firstID;
            count++;
        }
    }
    
    // After loop has finished: Wrap up - add one last vertex to graph.
    currVertex.swapEdges(edges);
    vertices.push_back(currVertex);
    
    // Finalize input parsing and processing
    graph.finalizeInputParsing(roundAssignations);  
}

uint InputManagement::parseDBLPFile(ifstream& inputFile, Graph& graph)
{
    // Strings for storing input temporarily
    string                  tempID;
    
    // Auxiliary variables for handling vertex data
    int                     lastFID     = -2;
    int                     firstID     = -1;
    int                     secondID    = -1;
    int                     edgeCount   = -1;
    float                   edgeWeight  = -1;
    // Number of processed vertices
    int                     count       = 0;
    
    // Temporary collection for edges of current vertex
    vector<Edge>            edges;
    // Current vertex (storing all read information)
    Vertex                    currVertex;
    // Vertex collection
    vector<Vertex>&           vertices       = graph.vertices();
    // Vertex map
    map<uint, uint>&        vertexIDMap   = graph.vertexIDMap();
    
    // Container for vertex-round assignations
    vector<float>           roundAssignations;
    // Random number generator
    RandomNumberGenerator   rng(chrono::system_clock::now().time_since_epoch().count());
     
    
    // Allocating memory and generating random numbers
    roundAssignations.reserve(InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS);
    rng.generateRandomNumbers(roundAssignations);
    
    // Parse actual data
    while (!inputFile.eof()) {
        tempID.clear();
        
        // Note: g++ stubbornly refuses knowledge of std::stoi(),
        // so atoi() is used here instead.
        
        // Read first ID
        inputFile >> tempID;
        // Check for blank line
        if (tempID.size()) {
            // Convert first ID to integer
            firstID = atoi(tempID.c_str());
            // Read second ID
            inputFile >> tempID;
            // Convert second ID to integer
            secondID = atoi(tempID.c_str());
            
            // Read number of edges, calculate edge weight
            inputFile >> tempID;
            // Convert edge count to integer
            edgeCount = atoi(tempID.c_str());
            // Calculate edge weight as log(1 + edgeCount)
            edgeWeight = log(1 + edgeCount);

            
            // New source (vertex) detected? Push old vertex to graph, 
            // reset edge collection
            if (lastFID != firstID) {
                // Add old vertex to collection
                if (lastFID >= 0) {
                    // Swap edge data with currVertex edge data
                    currVertex.swapEdges(edges);
                    
                    // Determine round in which vertex will be processed
                    currVertex.roundAssignation() = roundAssignations[count % InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS] 
                                                  * ParametrizationSettings::NUMBER_OF_ROUNDS + 1;
                    
                    // Add currVertex to vertex collection
                    vertices.push_back(currVertex);
                    
                    // Map vertex (ID is key, index is value)
                    vertexIDMap[currVertex.vertexID()] = count;
                }

                // Set new vertex ID
                currVertex.vertexID() = firstID;
                
                // Delete currently stored edges
                edges.clear();
            }

            // Add edge between firstID and secondID to edge collection.
            // Weight is taken from randomly generated edge weights.
            edges.push_back(Edge(currVertex.vertexID(), secondID, edgeWeight));
            
            // Set new source ID, iterate counter
            lastFID = firstID;
            count++;
        }
    }
    
    // After loop has finished: Wrap up - add one last vertex to graph.
    currVertex.swapEdges(edges);
    vertices.push_back(currVertex);
    
    // Finalize input parsing and processing
    graph.finalizeInputParsing(roundAssignations);
}

uint InputManagement::parseADVOGATOFile(ifstream& inputFile, Graph& graph)
{
    // Strings for storing input temporarily
    string                  tempID;
    
    // Vertex IDs
    int                     lastFID     = -2;
    int                     firstID     = -1;
    int                     secondID    = -1;
    // Current edge weight
    float                   weight      = 0;
    // Number of processed vertices
    int                     count       = 0;
    
    // Temporary collection for edges of current vertex
    vector<Edge>            edges;
    // Current vertex (storing all read information)
    Vertex                    currVertex;
    // Vertex collection
    vector<Vertex>&           vertices       = graph.vertices();
    // Vertex map
    map<uint, uint>&        vertexIDMap   = graph.vertexIDMap();
    
    // Container for vertex-round assignations
    vector<float>           roundAssignations;
    // Random number generator
    RandomNumberGenerator   rng(chrono::system_clock::now().time_since_epoch().count());
    
    
    // Allocating memory and generating random numbers
    roundAssignations.reserve(InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS);
    rng.generateRandomNumbers(roundAssignations);
    
    
    // Parse actual data
    while (!inputFile.eof()) {
        tempID.clear();
        
        // Note: g++ stubbornly refuses knowledge of std::stoi(),
        // so atoi() is used here instead.
        
        // Read first ID
        inputFile >> tempID;
        // Check for blank line
        if (tempID.size()) {
            // Convert first ID to integer
            firstID = atoi(tempID.c_str());
            // Read second ID
            inputFile >> tempID;
            // Convert second ID to integer
            secondID = atoi(tempID.c_str());
            // Read edge weight
            inputFile >> tempID;
            // Convert second ID (destination ID) to integer
            weight = atof(tempID.c_str());
            
            // New source (vertex) detected - push old vertex to graph, 
            // reset edge collection
            if (lastFID != firstID) {
                // Add old vertex to collection
                if (lastFID >= 0) {
                    // Swap edge data with currVertex edge data
                    currVertex.swapEdges(edges);
                    
                    // Determine round in which vertex will be processed
                    currVertex.roundAssignation() = roundAssignations[count % InputManagement::SNAP_MAX_NUMBER_OF_EDGE_WEIGHTS] 
                                                  * ParametrizationSettings::NUMBER_OF_ROUNDS + 1;
                    
                    // Add currVertex to vertex collection
                    vertices.push_back(currVertex);
                    
                    // Map vertex (ID is key, index is value)
                    vertexIDMap[currVertex.vertexID()] = count;
                }

                // Set new vertex ID
                currVertex.vertexID() = firstID;
                
                // Delete currently stored edges
                edges.clear();
            }

            // Add edge between firstID and secondID to edge collection.
            // Weight is taken from randomly generated edge weights.
            edges.push_back(Edge(currVertex.vertexID(), secondID, weight));
            
            // Set new source ID, iterate counter
            lastFID = firstID;
            count++;
        }
    }
    
    // After loop has finished: Wrap up - add one last vertex to graph.
    currVertex.swapEdges(edges);
    vertices.push_back(currVertex);
    
    // Finalize input parsing and processing
    graph.finalizeInputParsing(roundAssignations);
}

void InputManagement::prepareDBLP(string inputPath, string outputPath)
{
    ifstream inputFile;
    ofstream outputFile;
    
    string firstID;
    string secondID;
    
    const uint  expectedSize    = 17631144;
    uint        count           = 0;
    uint        fID             = 0;
    uint        sID             = 0;
    
    // Contains edges, used for sorting
    vector<pair<uint, uint>> edges(expectedSize / 2);
    
    cout << endl << "  :: Trying to open " << inputPath << endl;
    inputFile.open(inputPath.c_str(), ios::in);   
    outputFile.open(outputPath.c_str(), ios::out);   
    
    // Skip comment line
    inputFile.ignore(100, '\n');
    while (!inputFile.eof()) {
        inputFile >> firstID;
        inputFile >> secondID;
        // Ignore rest of the line
        inputFile.ignore(100, '\n');
        
        // Cast to numbers
        fID = atoi(firstID.c_str());
        sID = atoi(secondID.c_str());
    
        // Reverse source/destination order
        edges.push_back(pair<uint, uint>(sID, fID));    
    }
    cout << "  :: Finished reading." << endl;
    
    // Sort edges (by destination first, source second)
    sort(edges.begin(), edges.end(), DBLP_Edge_SortPred());
    cout << "  :: Finished sorting." << endl;
    
    // Write sorted edges/vertex combinations to file
    // Modified version with weights
    for (uint index = 0; index < edges.size(); index++) {
        // Count how many identical pairs appear
        for (count = index; count < edges.size() && edges[count] == edges[index]; count++);
        
        outputFile << edges[index].first << " " << edges[index].second << " " << (count - index) << endl;
        
        index = count - 1;
        count = 0;
    }
    
    cout << "  :: Finished writing." << endl << endl;
    
    inputFile.close();
    outputFile.close();
}