/*
 * File:   CDHS_GMH.cpp
 * Author: RM
 *
 * Created on 16. August 2013, 00:18
 */

#include "CDHS_GMH.h"

CDHS_GMH::CDHS_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism) : GroupMappingHeuristic(graph, influenceSet, vertexGroups, offerMechanism)
{  
}

CDHS_GMH::CDHS_GMH(const CDHS_GMH& orig) : GroupMappingHeuristic(orig)
{
}

CDHS_GMH::CDHS_GMH() : GroupMappingHeuristic()
{
}

CDHS_GMH::~CDHS_GMH()
{
}

uint CDHS_GMH::mapGroups()
{
    // Documentation
    ofstream& protocol          = Protocol::protocol();
   
    // Get highest myopic price
    vector<Vertex>& vertices    = _graph->vertices();
   
    float   highestMP           = vertices[vertices.size() - 1].myopicPrice();
    float   currentLowerLimit   = highestMP / ParametrizationSettings::GROUP_LIMIT_DIVISOR;
    uint    lastIndex           = vertices.size() - 1;
    uint    firstIndex          = lastIndex;
   
    cout << "  :: Gathering vertex group data." << endl << endl;
    protocol << "  :: Gathering vertex group data." << endl << endl;
   
    // Create groups, store limits as indices
    for (uint groupIndex = 0; groupIndex < ParametrizationSettings::NUMBER_OF_GROUPS; groupIndex++) {
        // Iterate until criterion (range for myopic prices) for affiliation no longer met
        for (; lastIndex > 0 && (vertices[lastIndex].myopicPrice() > currentLowerLimit); lastIndex--);
       
        // Reset last index to last valid position (if lastIndex > 0)
        if (lastIndex) {
            lastIndex++;
        }
       
        // Determine group offer.
        float groupOffer         = selectGroupOffer(vertices, firstIndex, lastIndex, currentLowerLimit);
        float adjustedLowerLimit = currentLowerLimit >= 0 ? currentLowerLimit : 0;
        
        // Create instance of vertex group, add to collection.
        VertexGroup vertexGroup(firstIndex, lastIndex, adjustedLowerLimit, adjustedLowerLimit * ParametrizationSettings::GROUP_LIMIT_DIVISOR, groupOffer);
        _vertexGroups->push_back(vertexGroup);
       
       
        // Logging (stdout)
        cout << "  ::   Group #" << groupIndex << endl;
        cout << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice() << endl;
        cout << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice() << endl;
        cout << "  ::      - Upper Limit:\t" << adjustedLowerLimit * ParametrizationSettings::GROUP_LIMIT_DIVISOR << endl;
        cout << "  ::      - Lower Limit:\t" << adjustedLowerLimit << endl;
        cout << "  ::      - Group offer:\t" << groupOffer << endl;
        cout << "  ::      - Group members:\t" << vertexGroup.getMemberCount() << endl << endl;
       
        // Logging (protocol file)
        protocol << "  ::   Group #" << groupIndex << endl;
        protocol << "  ::      - First index:\t" << firstIndex << ";\tMP = " << vertices[firstIndex].myopicPrice() << endl;
        protocol << "  ::      - Last index:\t\t" << lastIndex << ";\tMP = " << vertices[lastIndex].myopicPrice() << endl;
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
        if (!vertices[firstIndex].myopicPrice() && firstIndex) {
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

void CDHS_GMH::restructureDataset()
{
    // Restructure dataset
    //   1. Delete vertices with MP < 0 (deaf and IS vertices)
    uint deletedVertexCount = _graph->deleteInactiveVertices(false);
    
    //   2. Sort vertices
    sort(_graph->vertices().begin(), _graph->vertices().end());
    
    // Log
    cout << "  :: Median myopic price:\t\t\t" << _graph->vertices()[_graph->vertices().size() / 2].myopicPrice() << endl;
    Protocol::protocol() << "  :: Median myopic price:\t\t\t\t\t\t" << _graph->vertices()[_graph->vertices().size() / 2].myopicPrice() << endl;
    cout << "  :: Number of deleted vertices incl. IS:\t" << deletedVertexCount << endl;
    Protocol::protocol() << "  :: Number of deleted vertices incl. IS:\t\t" << deletedVertexCount << endl;
    cout << "  :: Number of deleted vertices without IS:\t" << deletedVertexCount - _influenceSet->size() << endl;
    Protocol::protocol() << "  :: Number of deleted vertices without IS:\t\t" << deletedVertexCount - _influenceSet->size() << endl;    
}

float CDHS_GMH::selectGroupOffer(const vector<Vertex>& vertices, uint firstIndex, uint lastIndex, float currentLowerLimit) const
{
    float groupOffer = currentLowerLimit >= 0 ? vertices[lastIndex].getMyopicPrice() : 0;
    
    // Check if selection reasonable; if not, resort to default.
    if ((firstIndex - lastIndex > 1) && currentLowerLimit >= 0) {
        // Get group offer
        groupOffer = vertices[calculateGroupOfferIndex(firstIndex, lastIndex)].getMyopicPrice();
    }
    
    return groupOffer;
}