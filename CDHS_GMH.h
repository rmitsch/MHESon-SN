/*
 * File:   CDHS_GMH.h
 * Author: RM
 *
 * Created on 16. August 2013, 00:18
 */

#ifndef CDHS_GMH_H
#define CDHS_GMH_H

#include "Definitions.h"
#include "GroupMappingHeuristic.h"

class CDHS_GMH : public GroupMappingHeuristic
{
    public:
        CDHS_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism);
        CDHS_GMH(const CDHS_GMH& orig);
        CDHS_GMH();
        virtual ~CDHS_GMH();
       
        /**
         * Map vertices to groups, returns number of non-empty groups.
         * @return Number of non-empty groups.
         */
        uint mapGroups();
       
        /**
         * Prepares dataset for mapping by cleaning inactive and IS vertices and
         * sorting vertices according to decisive criterion.
         */
        void restructureDataset();
        
    private:
        /**
         * Selects group offer (dependent on chosen group offer mechanism).
         * @param vertices
         * @param firstIndex
         * @param lastIndex
         * @param currentLowerLimit
         * @return 
         */
        float selectGroupOffer( const vector<Vertex>& vertices, 
                                        uint firstIndex, 
                                        uint lastIndex,
                                        float currentLowerLimit) const;

} ;

#endif  /* CDHS_GMH_H */
