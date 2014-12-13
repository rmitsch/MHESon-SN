/* 
 * File:   GroupMappingHeuristic.cpp
 * Author: RM
 * 
 * Created on 15. August 2013, 00:00
 */

#include "GroupMappingHeuristic.h"

GroupMappingHeuristic::GroupMappingHeuristic(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism)
{
    _graph          = &graph;
    _influenceSet   = &influenceSet;
    _vertexGroups   = &vertexGroups;
    _offerMechanism = offerMechanism;
    
    // Reserve memory for influence set (can be cleared after calculating myopic prices).
    // Size of memory reserved: Number of existend vertices times probability for each vertex 
    // to be included in influence set.
    _influenceSet->reserve(_graph->vertices().size() * ParametrizationSettings::INFLUENCE_SET_PROBABILITY);
}

GroupMappingHeuristic::GroupMappingHeuristic(const GroupMappingHeuristic& orig)
{
    _graph          = orig._graph;
    *_influenceSet  = *orig._influenceSet;
    _vertexGroups   = orig._vertexGroups;
    _offerMechanism = orig._offerMechanism;
}

GroupMappingHeuristic::GroupMappingHeuristic()
{
}

GroupMappingHeuristic::~GroupMappingHeuristic()
{
}

void GroupMappingHeuristic::init(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup> vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism)
{
    _graph          = &graph;
    _influenceSet   = &influenceSet;
    _vertexGroups   = &vertexGroups;
    _offerMechanism = offerMechanism;
    
    // Reserve memory for influence set (can be cleared after calculating myopic prices).
    // Size of memory reserved: Number of existend vertices times probability for each vertex 
    // to be included in influence set.
    _influenceSet->reserve(_graph->vertices().size() * ParametrizationSettings::INFLUENCE_SET_PROBABILITY);
}

uint GroupMappingHeuristic::pickInfluenceSet()
{
    // Get reference to vertex collection stored in graph
    vector<Vertex>& vertices = _graph->vertices();
    
    // Generate vertices.size() random numbers between 0 and 1.
    // If > 0.5: Part of influence set.
    vector<float>           influenceSetMarker(vertices.size());
    RandomNumberGenerator   rng(chrono::system_clock::now().time_since_epoch().count());
    // Fill container with random numbers
    rng.generateRandomNumbers(influenceSetMarker);
    
    // Iterate through IS marker, store vertex ID in influence set when indicated.
    for (uint index = 0; index < influenceSetMarker.size(); index++) {
        
        // If under threshold: Store in influence set
        if (influenceSetMarker[index] <= ParametrizationSettings::INFLUENCE_SET_PROBABILITY) {
            // Add vertex ID to influence set
            _influenceSet->insert(vertices[index].vertexID());
            
            // Set myopic price of influence set member vertex to defined constant value
            // (thus marking them for deletion after vertices are sorted)
            vertices[index].myopicPrice()      = ParametrizationSettings::MYOPIC_PRICE_INFLUENCE_SET;
            
            // Set _processed value to 0 (indicates index of round in which
            // vertex was processed)
            vertices[index].roundAssignation() = 0;
        }
    }
    
    return 0;
}

double GroupMappingHeuristic::calculateMyopicPrices()
{
    double sum                  = 0;
    // Get reference to vertex collection stored in graph
    vector<Vertex>& vertices    = _graph->vertices();
    
    // Iterate through vertices
    for (Vertex& vertex : vertices) {
        // If not in in influence set: Calculate myopic price
        if (!_influenceSet->count(vertex.getVertexID())) {
            sum += vertex.calculateMyopicPrice();
        }
    }
    
    return sum;
}

uint GroupMappingHeuristic::analyseVertexGroupPool()
{
    // Protocol stream entity
    ofstream& protocol  = Protocol::protocol();
    // Number of nonempty groups
    uint nonemptyCount  = 0;
    // Vertex collection
    vector<Vertex>& vertices = _graph->vertices();
    
    
    // --- Analysis and logging ---    
    
    // Perfect fit: Saturated vertex group pool
    if (!_vertexGroups->at(_vertexGroups->size() - 1).lastIndex() &&
        _vertexGroups->at(_vertexGroups->size() - 2).lastIndex()) {
        nonemptyCount = _vertexGroups->size();
       
        cout << "  :: Note: Vertex group pool qualifies as saturated." << endl;
        protocol << "  :: Note: Vertex group pool qualifies as saturated." << endl;
    }
   
    // If last group doesn't end with index zero: Oversaturated vertex group pool
    // Remark: Case doesn't emerge with CHAOS (as vertices are uniformly distributed).
    else if (_vertexGroups->at(_vertexGroups->size() - 1).lastIndex()) {
        nonemptyCount = _vertexGroups->size();
       
        cout << "  :: Note: Vertex group pool qualifies as oversaturated." << endl;
        protocol << "  :: Note: Vertex group pool qualifies as oversaturated." << endl;
       
        // Last group: Absorb surplus vertices
        if (ParametrizationSettings::GM_ABSORB_SURPLUS_NODES) {
            cout << "  :: Last vertex group is modified in order to incorporate surplus vertices. " << endl;
            protocol << "  :: Last vertex group is modified in order to incorporate surplus vertices. " << endl;
       
            VertexGroup& lastGroup  = _vertexGroups->at(_vertexGroups->size() - 1);
            lastGroup.lastIndex()   = 0;
            // If enabled: Consider new vertices for picking group offer.
            if (ParametrizationSettings::GM_DYNAMIC_SURPLUS_LIMITS)
                lastGroup.lowerLimit() = vertices[0].myopicPrice();
        }
    }
   
    // One or more vertex groups don't contain any vertices: Undersaturated vertex group pool
    else {
        for (uint index = 0; index < _vertexGroups->size(); index++) {
            if (_vertexGroups->at(index).firstIndex())
                nonemptyCount++;

            else {
                cout << "  :: Note: Vertex group pool qualifies as undersaturated (" << (_vertexGroups->size() - index) << " groups out of " << _vertexGroups->size() << ")." << endl;
                protocol << "  :: Note: Vertex group pool qualifies as undersaturated (" << (_vertexGroups->size() - index) << " groups out of " << _vertexGroups->size() << ")." << endl;
               
                break;
            }
        }
    }
   
    cout << "  :: Non-empty groups: " << nonemptyCount << endl;
    protocol << "  :: Non-empty groups: " << nonemptyCount << endl;
    
    return nonemptyCount;
}

uint GroupMappingHeuristic::calculateGroupOfferIndex(uint firstIndex, uint lastIndex) const
{
    uint selectedIndex = 0;
    
    switch (_offerMechanism) {
        case GMHAlgorithm::MIN:
            selectedIndex = lastIndex;
         break;

        case GMHAlgorithm::FIRST_QUARTILE:
            selectedIndex = firstIndex - 3 * (firstIndex - lastIndex) / 4;
        break;

        case GMHAlgorithm::MEDIAN:
            selectedIndex = firstIndex - 1 * (firstIndex - lastIndex) / 2;
        break;

        case GMHAlgorithm::THIRD_QUARTILE:
            selectedIndex = firstIndex - 1 * (firstIndex - lastIndex) / 4;
        break;

        case GMHAlgorithm::MAX:
            selectedIndex = firstIndex;
        break;
        
        case GMHAlgorithm::RANDOM:
            RandomNumberGenerator rng(chrono::system_clock::now().time_since_epoch().count());
            selectedIndex = lastIndex + (firstIndex - lastIndex) * rng.rand();
        break;
    }
    
    return selectedIndex;
}