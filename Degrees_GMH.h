/* 
 * File:   Degrees_GMH.h
 * Author: RM
 *
 * Created on 16. September 2013, 22:19
 */

#ifndef DEGREES_GMH_H
#define	DEGREES_GMH_H

#include "Definitions.h"
#include "GroupMappingHeuristic.h"

class Degrees_GMH : public GroupMappingHeuristic
{
    public:
        Degrees_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism);
        Degrees_GMH(const Degrees_GMH& orig);
        Degrees_GMH();
        virtual ~Degrees_GMH();

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

#endif	/* DEGREES_GMH_H */

