/* 
 * File:   GroupMappingHeuristics.h
 * Author: RM
 *
 * Created on 15. August 2013, 00:00
 */

#ifndef GROUPMAPPINGHEURISTIC_H
#define	GROUPMAPPINGHEURISTIC_H

#include <unordered_set>

#include "Definitions.h"
#include "ParametrizationSettings.h"
#include "RandomNumberGenerator.h"
#include "VertexGroup.h"
#include "Graph.h"


/**
 * Enumerating all available group mapping heuristics and mechanisms to
 * determine how group offer is set.
 */
namespace GMHAlgorithm
{
    enum GMHeuristic            { CDHS, CHAOS, DEGREES, NEIGHBOURHOOD };
    enum PriceSettingMechanism  { MIN, FIRST_QUARTILE, MEDIAN, THIRD_QUARTILE, MAX, RANDOM};
}

class GroupMappingHeuristic
{
    public:
        GroupMappingHeuristic(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup>& vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism);
        GroupMappingHeuristic(const GroupMappingHeuristic& orig);
        GroupMappingHeuristic();
        virtual ~GroupMappingHeuristic();
    
        /**
         * Initializes graph settings (same parameters as constructor).
         * @param graph
         */
        void init(Graph& graph, unordered_set<uint>& influenceSet, vector<VertexGroup> vertexGroups, GMHAlgorithm::PriceSettingMechanism offerMechanism);
        
        /**
         * Pick influence set, return number of chosen vertices.
         * @return Number of vertices picked as members of influence set.
         */
        virtual uint pickInfluenceSet();
        
        /**
         * Analyses saturation of vertex groups.
         * Note: May alter groups (if oversaturation is detected).
         * @return Number of nonempty groups.
         */
        virtual uint analyseVertexGroupPool();
        
        /**
         * Calculates myopic price of all vertices not included in the influence
         * set.
         * @return Sum of myopic prices of all vertices considered.
         */
        virtual double calculateMyopicPrices();
        
        /**
         * Prepares dataset for mapping by cleaning inactive and IS vertices and
         * sorting vertices according to decisive criterion.
         */
        virtual void restructureDataset()                               = 0;
        
        /**
         * Map vertices to groups, return number of defined groups.
         * @return Number of defined groups.
         */
        virtual uint mapGroups()                                        = 0;
        
    protected:
        /**
         * --- Methods ---
         */
        
        /**
         * Selects group offer (dependent on chosen group offer mechanism).
         * Relies on sorted vertices collection.
         * @param vertices
         * @param firstIndex
         * @param lastIndex
         * @param currentLowerLimit
         * @return 
         */
        virtual float   selectGroupOffer(   const vector<Vertex>& vertices, 
                                            uint firstIndex, 
                                            uint lastIndex, 
                                            float currentLowerLimit) const  = 0;
        
        uint            calculateGroupOfferIndex(uint firstIndex, uint lastIndex) const;
        
        /**
         * --- Variables ---
         */
        
        /**
         * Contains vertex IDs of vertices chosen as part of influence set.
         * Realised as unordered set due to its ability to reserve memory
         * in advance.
         */
        unordered_set<uint>* _influenceSet;
        
        // Contains vertex groups
        vector<VertexGroup>* _vertexGroups;
        
        // Reference to graph containing vertex data
        Graph* _graph;
        
        // Stores group offer setting mechanism to use.
        GMHAlgorithm::PriceSettingMechanism _offerMechanism;
} ;

#endif	/* GROUPMAPPINGHEURISTIC_H */