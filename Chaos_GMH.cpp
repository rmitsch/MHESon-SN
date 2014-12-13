/* 
 * File:   Chaos_GMH.cpp
 * Author: RM
 * 
 * Created on 12. September 2013, 23:26
 */

#include "Chaos_GMH.h"

Chaos_GMH::Chaos_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism) : GroupMappingHeuristic(graph, influenceSet, vertexGroups, offerMechanism)
{   
}

Chaos_GMH::Chaos_GMH(const Chaos_GMH& orig) : GroupMappingHeuristic(orig)
{
}

Chaos_GMH::Chaos_GMH() : GroupMappingHeuristic()
{
}

Chaos_GMH::~Chaos_GMH()
{
}

uint Chaos_GMH::mapGroups()
{
    // Documentation
    ofstream&   protocol                = Protocol::protocol();
   
    // Get highest myopic price
    vector<Vertex>& vertices            = _graph->vertices();
   
    // Various auxiliary index variables
    uint        lastIndex               = vertices.size() - 1;
    uint        firstIndex              = lastIndex;
    uint        remainingVertexCount    = vertices.size() - 1;
    
    // Random number generator (used for selection of group offer out of MPs of involved vertices)
    RandomNumberGenerator rng(chrono::system_clock::now().time_since_epoch().count());
    
    // Store current group offer
    float currentGroupOffer   = -1;
    
    
    cout << "  :: Gathering vertex group data." << endl << endl;
    protocol << "  :: Gathering vertex group data." << endl << endl;
    
    // Shuffle vertices to avoid strictly ascending order (as given in input files)
    // and introduce randomness to group mapping process
    random_shuffle(vertices.begin(), vertices.end());
    
    // Create groups, store limits as indices
    for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
        // Set first index (as usual if not first group, 0 if first group)
        firstIndex = lastIndex;

        // Set last index
        uint selectedVertexCount = rng.rand() * remainingVertexCount;
        remainingVertexCount    -= selectedVertexCount;
        lastIndex                = firstIndex - selectedVertexCount;
        
        // Pick random vertex whose MP is used as group offer (exempt: MP = 0)
        currentGroupOffer = selectGroupOffer(vertices, firstIndex, lastIndex, 0);
        
        // Add vertex group to collection (set currentGroupOffer as both lower
        // and higher limit, as it is not relevant with CHAOS anyway).
        VertexGroup vertexGroup(firstIndex, lastIndex, currentGroupOffer, currentGroupOffer, currentGroupOffer);
        _vertexGroups->push_back(vertexGroup);
        
        
        // Logging (stdout)
        cout << "  ::   Group #" << groupIndex << endl;
        cout << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice() << endl;
        cout << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice() << endl;
        cout << "  ::      - Group offer:\t" << currentGroupOffer << endl;
        cout << "  ::      - Group members:\t" << _vertexGroups->at(groupIndex).getMemberCount() << endl << endl;
       
        // Logging (protocol file)
        protocol << "  ::   Group #" << groupIndex << endl;
        protocol << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice() << endl;
        protocol << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice() << endl;
        protocol << "  ::      - Group offer:\t" << currentGroupOffer << endl;
        protocol << "  ::      - Group members:\t" << _vertexGroups->at(groupIndex).getMemberCount() << endl << endl;
    }
   
    
   // Analyse saturation of vertex group pool, return number of nonempty groups.
    return analyseVertexGroupPool();
}

uint Chaos_GMH::mapGroups_semirandom()
{
    // Documentation
    ofstream&   protocol            = Protocol::protocol();
   
    // Get highest myopic price
    vector<Vertex>& vertices         = _graph->vertices();
   
    // Various auxiliary index variables
    uint        lastIndex           = vertices.size() - 1;
    uint        firstIndex          = lastIndex;
    uint        indexOffset         = 0;
    const uint  verticesPerGroup    = vertices.size() / ParametrizationSettings::NUMBER_OF_GROUPS;
    
    // Random number generator (used for selection of group offer out of MPs of involved vertices)
    RandomNumberGenerator rng(chrono::system_clock::now().time_since_epoch().count());
    
    // Store current group offer
    float currentGroupOffer   = -1;
    
    
    cout << "  :: Gathering vertex group data." << endl << endl;
    protocol << "  :: Gathering vertex group data." << endl << endl;
    
    // Shuffle vertices to avoid strictly ascending order (as given in input files)
    // and introduce randomness to group mapping process
    random_shuffle(vertices.begin(), vertices.end());
    
    // Create groups, store limits as indices
    for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
        // Set first index (as usual if not first group, 0 if first group)
        firstIndex  = vertices.size() - 1 - groupIndex * verticesPerGroup;
        
        // Set last index
        lastIndex   = (firstIndex - verticesPerGroup > 0) ? firstIndex - verticesPerGroup + 1 : 0;
        
        // Pick random vertex whose MP is used as group offer (exempt: MP = 0)
        do {
            indexOffset         = rng.rand() * verticesPerGroup;
            currentGroupOffer   = vertices[firstIndex - indexOffset].myopicPrice();
        } while (!currentGroupOffer);
        
        // Add vertex group to collection (set currentGroupOffer as both lower
        // and higher limit, as it is not relevant with CHAOS anyway).
        VertexGroup vertexGroup(firstIndex, lastIndex, currentGroupOffer, currentGroupOffer, currentGroupOffer);
        _vertexGroups->push_back(vertexGroup);
        
        
        // Logging (stdout)
        cout << "  ::   Group #" << groupIndex << endl;
        cout << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice() << endl;
        cout << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice() << endl;
        cout << "  ::      - Group offer:\t" << currentGroupOffer << endl;
        cout << "  ::      - Group members:\t" << _vertexGroups->at(groupIndex).getMemberCount() << endl << endl;
       
        // Logging (protocol file)
        protocol << "  ::   Group #" << groupIndex << endl;
        protocol << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice() << endl;
        protocol << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice() << endl;
        protocol << "  ::      - Group offer:\t" << currentGroupOffer << endl;
        protocol << "  ::      - Group members:\t" << _vertexGroups->at(groupIndex).getMemberCount() << endl << endl;
    }
   
    
   // Analyse saturation of vertex group pool, return number of nonempty groups.
    return analyseVertexGroupPool();
}

void Chaos_GMH::restructureDataset()
{
    uint deletedVertexCount = _graph->deleteInactiveVertices(false);
    
    // Log
    cout << "  :: Median myopic price:\t\t\t" << _graph->calculateMedianMP() << endl;
    Protocol::protocol() << "  :: Median myopic price:\t\t\t\t\t\t" << _graph->calculateMedianMP() << endl;
    cout << "  :: Number of deleted vertices incl. IS:\t" << deletedVertexCount << endl;
    Protocol::protocol() << "  :: Number of deleted vertices incl. IS:\t\t" << deletedVertexCount << endl;
    cout << "  :: Number of deleted vertices without IS:\t" << deletedVertexCount - _influenceSet->size() << endl;
    Protocol::protocol() << "  :: Number of deleted vertices without IS:\t\t" << deletedVertexCount - _influenceSet->size() << endl;    
}

float Chaos_GMH::selectGroupOffer(const vector<Vertex>& vertices, uint firstIndex, uint lastIndex, float currentLowerLimit) const
{
    float groupOffer            = currentLowerLimit >= 0 ? vertices[lastIndex].getMyopicPrice() : 0;
    const uint iterationLimit   = 10;
    uint       iterationCount   = 0;
    
    // Check if selection reasonable; if not, resort to default.
    if ((firstIndex - lastIndex > 1) && currentLowerLimit >= 0) {
        // Get group offer; avoid groupOffer = 0 if ::RANDOM used.
        if (_offerMechanism == GMHAlgorithm::RANDOM) {
            do {
                groupOffer = vertices[calculateGroupOfferIndex(firstIndex, lastIndex)].getMyopicPrice();
            } while (!groupOffer && iterationCount++ <= iterationLimit);
        }
        
        else {
            groupOffer = vertices[calculateGroupOfferIndex(firstIndex, lastIndex)].getMyopicPrice();
        }
    }
    
    return groupOffer;
}