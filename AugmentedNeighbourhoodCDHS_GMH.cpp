/* 
 * File:   AugmentedNeighbourhoodCDHS_GMH.cpp
 * Author: RM
 * 
 * Created on 14. September 2013, 20:39
 */

#include "AugmentedNeighbourhoodCDHS_GMH.h"

AugmentedNeighbourhoodCDHS_GMH::AugmentedNeighbourhoodCDHS_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism) : GroupMappingHeuristic(graph, influenceSet, vertexGroups, offerMechanism)
{  
}

AugmentedNeighbourhoodCDHS_GMH::AugmentedNeighbourhoodCDHS_GMH(const AugmentedNeighbourhoodCDHS_GMH& orig) : GroupMappingHeuristic(orig)
{
}

AugmentedNeighbourhoodCDHS_GMH::AugmentedNeighbourhoodCDHS_GMH() : GroupMappingHeuristic()
{
}

AugmentedNeighbourhoodCDHS_GMH::~AugmentedNeighbourhoodCDHS_GMH()
{
}

uint AugmentedNeighbourhoodCDHS_GMH::mapGroups()
{
    // Documentation
    ofstream& protocol          = Protocol::protocol();
    
    // Get highest myopic price
    vector<Vertex>& vertices    = _graph->vertices();
   
    float   highestEP           = vertices[vertices.size() - 1].emmetropicPrice();
    float   currentLowerLimit   = highestEP / ParametrizationSettings::GROUP_LIMIT_DIVISOR;
    uint    lastIndex           = vertices.size() - 1;
    uint    firstIndex          = lastIndex;
   
    cout << "  :: Gathering vertex group data." << endl << endl;
    protocol << "  :: Gathering vertex group data." << endl << endl;
   
    // Create groups, store limits as indices
    for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
        // Iterate until criterion (range for myopic prices) for affiliation no longer met
        for (; lastIndex > 0 && (vertices[lastIndex].emmetropicPrice() > currentLowerLimit); lastIndex--);
       
        // Reset last index to last valid position (if lastIndex > 0)
        if (lastIndex) {
            lastIndex++;
        }
       
        // Determine group offer / lower limit of group (may be the same, but
        // isn't necessarily).
       float groupOffer         = selectGroupOffer(vertices, firstIndex, lastIndex, currentLowerLimit);
       float adjustedLowerLimit = currentLowerLimit >= 0 ? vertices[lastIndex].emmetropicPrice() : 0;
       
        // Create vertex group, add to collection.
        VertexGroup vertexGroup(firstIndex, lastIndex, adjustedLowerLimit, adjustedLowerLimit * ParametrizationSettings::GROUP_LIMIT_DIVISOR, groupOffer);
        _vertexGroups->push_back(vertexGroup);
       
       
        // Logging (stdout)
        cout << "  ::   Group #" << groupIndex << endl;
        cout << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice()
                                                           << ";\tEP = " << vertices[firstIndex].emmetropicPrice() << endl;
        cout << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice()
                                                           << ";\tEP = " << vertices[lastIndex].emmetropicPrice() << endl;
        cout << "  ::      - Upper Limit:\t" << adjustedLowerLimit * ParametrizationSettings::GROUP_LIMIT_DIVISOR << endl;
        cout << "  ::      - Lower Limit:\t" << adjustedLowerLimit << endl;
        cout << "  ::      - Group offer:\t" << groupOffer << endl;
        cout << "  ::      - Group members:\t" << vertexGroup.getMemberCount() << endl << endl;
       
        // Logging (protocol file)
        protocol << "  ::   Group #" << groupIndex << endl;
        protocol << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice()
                                                               << ";\tEP = " << vertices[firstIndex].emmetropicPrice() << endl;
        protocol << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice()
                                                               << ";\tEP = " << vertices[lastIndex].emmetropicPrice() << endl;
        protocol << "  ::      - Upper Limit:\t" << adjustedLowerLimit * ParametrizationSettings::GROUP_LIMIT_DIVISOR << endl;
        protocol << "  ::      - Lower Limit:\t" << adjustedLowerLimit << endl;
        protocol << "  ::      - Group offer:\t" << groupOffer << endl;
        protocol << "  ::      - Group members:\t" << vertexGroup.getMemberCount() << endl << endl;
       
        // Set (start) index for next group. Set to 0, if lastIndex of this group is zero too.
        firstIndex = lastIndex ? lastIndex - 1 : lastIndex;
       
        // Vertices are sorted, so when _vertices[firstIndex].MP is 0, MPs of all vertices
        // with lower indices are also zero. Consequence: When first vertex of next
        // group has MP = 0, set lower limit to negative value - that way we can
        // continue group mapping (remark: next group is going to encompass ALL
        // remaining - zero-MP - vertices).
        if (!vertices[firstIndex].emmetropicPrice() && firstIndex) {
            currentLowerLimit = -1;
        }
        // If we have a CLL greater zero and first vertex has MP > 0: Same procedure as every round
        else if (currentLowerLimit > 0) {
            currentLowerLimit /= ParametrizationSettings::GROUP_LIMIT_DIVISOR;
        }
        // Else means we are stuck with a negative value for CLL, which implies
        // the previous group was pure MP-zero. -> Set CLL to zero
        else {
            currentLowerLimit = 0;
        }
    }
   
    
    // Analyse saturation of vertex group pool, return number of nonempty groups.
    return analyseVertexGroupPool();
}

void AugmentedNeighbourhoodCDHS_GMH::restructureDataset()
{
    // Calculate emmetropic prices (crucial to sorting process)
    float averageEmmetropicPrice = calculateEmmetropicPrices();
    // Log average emmetropic price
    cout << "  :: Average emmetropic price:\t\t\t" << averageEmmetropicPrice << endl;
    Protocol::protocol() << "  :: Average emmetropic price:\t\t\t\t\t" << averageEmmetropicPrice << endl;
    
    // --- Restructure dataset ---
    
    //  1. Delete vertices with MP < 0 (deaf and IS vertices)
    uint deletedVertexCount = _graph->deleteInactiveVertices(false);
    
    //  2. Calculate median MP (needs to be done before sorting, as it modifies
    // collection structure).
    float medianMP = _graph->calculateMedianMP();
    cout << "  :: Median myopic price:\t\t\t" << medianMP << endl;
    Protocol::protocol() << "  :: Median myopic price:\t\t\t\t\t\t" << medianMP << endl;
    
    //  3. Sort vertices
    sort(_graph->vertices().begin(), _graph->vertices().end(), Vertex::compareEmmetropicPrices);
    
    // Log median EP and number of deleted vertices with and without consideration
    // of IS vertices.
    cout << "  :: Median emmetropic price:\t\t\t" << _graph->vertices()[_graph->vertices().size() / 2].emmetropicPrice() << endl;
    Protocol::protocol() << "  :: Median emmetropic price:\t\t\t\t\t" << _graph->vertices()[_graph->vertices().size() / 2].emmetropicPrice() << endl;
    cout << "  :: Number of deleted vertices incl. IS:\t" << deletedVertexCount << endl;
    Protocol::protocol() << "  :: Number of deleted vertices incl. IS:\t\t" << deletedVertexCount << endl;
    cout << "  :: Number of deleted vertices without IS:\t" << deletedVertexCount - _influenceSet->size() << endl;
    Protocol::protocol() << "  :: Number of deleted vertices without IS:\t\t" << deletedVertexCount - _influenceSet->size() << endl;
}

float AugmentedNeighbourhoodCDHS_GMH::calculateEmmetropicPrices()
{
    // Average emmetropic price
    float averageEP             = 0;
    vector<Vertex>& vertices    = _graph->vertices();
    
    // Calculate emmetropic price for each vertex
    for (Vertex& vertex : vertices) {
        if (!_influenceSet->count(vertex.getVertexID())) {
            // Calculate emmetropic price
            averageEP += vertex.calculateEmmetropicPrice();
        }
    }
    
    // Return average emmetropic price
    return averageEP / (vertices.size() - _influenceSet->size());
}
                                        
float AugmentedNeighbourhoodCDHS_GMH::selectGroupOffer(const vector<Vertex>& vertices, uint firstIndex, uint lastIndex, float currentLowerLimit) const
{
    float groupOffer = currentLowerLimit >= 0 ? vertices[lastIndex].getEmmetropicPrice() : 0;
    
    // Check if selection reasonable; if not, resort to default.
    if ((firstIndex - lastIndex > 1) && currentLowerLimit >= 0) {
        // Get group offer
        groupOffer = vertices[calculateGroupOfferIndex(firstIndex, lastIndex)].getEmmetropicPrice();
    }
    
    return groupOffer;
}