/* 
 * File:   Chaos_GMH.h
 * Author: RM
 *
 * Created on 12. September 2013, 23:26
 */

#ifndef CHAOS_GMH_H
#define	CHAOS_GMH_H

#include "Definitions.h"
#include "GroupMappingHeuristic.h"

/**
 * Note: Chaos_GMH doesn't require vertices to be sorted, therefore
 * no custom sorting predicates are needed.
 */
class Chaos_GMH : public GroupMappingHeuristic
{
    public:
        Chaos_GMH(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism);
        Chaos_GMH(const Chaos_GMH& orig);
        Chaos_GMH();
        virtual ~Chaos_GMH();

        /**
         * Map vertices to groups, returns number of non-empty groups.
         * Relies on random group mapping distribution, with the potential
         * size of current group getting smaller each round with the total
         * number of unassigned vertices diminishing.
         * @return Number of non-empty groups.
         */
        uint mapGroups();
        
        /**
         * Map vertices to groups, returns number of non-empty groups.
         * Relies on constant / uniform group mapping distribution: Each group
         * gets assigned same number of vertices.
         * @return Number of non-empty groups.
         * @deprecated Only mapGroups() in use currently.
         */
        uint mapGroups_semirandom();
        
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

#endif	/* CHAOS_GMH_H */

