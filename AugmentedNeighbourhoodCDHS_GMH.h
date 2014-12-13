/* 
 * File:   AugmentedNeighbourhoodCDHS_GMH.h
 * Author: RM
 *
 * Created on 14. September 2013, 20:39
 */

#ifndef AUGMENTEDNEIGHBOURHOODCDHS_GMH_H
#define	AUGMENTEDNEIGHBOURHOODCDHS_GMH_H

#include "Definitions.h"
#include "GroupMappingHeuristic.h"

class AugmentedNeighbourhoodCDHS_GMH : public GroupMappingHeuristic
{
    public:
        AugmentedNeighbourhoodCDHS_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism);
        AugmentedNeighbourhoodCDHS_GMH(const AugmentedNeighbourhoodCDHS_GMH& orig);
        AugmentedNeighbourhoodCDHS_GMH();
        virtual ~AugmentedNeighbourhoodCDHS_GMH();

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
         * --- Methods ---
         */
        
        /**
         * Calculates emmetropic prices (EP), which accounts for influence of
         * neighbours that are not part of influence set. 
         * Note: Could be extendend in order to include n-th generation neigh-
         * bours, positive evaluation results provided.
         * EP is calculated as P(r_a > r_b) * MP_b * W_ba + MP_a, where "a" is
         * currently examined and "b" is adjacent vertex.
         * @return Average emmetropic price.
         */
        float calculateEmmetropicPrices();

        
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
        
        /**
         * --- Variables ---
         */
} ;

#endif	/* AUGMENTEDNEIGHBOURHOODCDHS_GMH_H */

