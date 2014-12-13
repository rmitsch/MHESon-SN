/* 
 * File:   Vertex.h
 * Author: RM
 *
 * Created on 26. Juli 2013, 22:55
 */

#ifndef NODE_H
#define	NODE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <map>

#include "Definitions.h"
#include "Edge.h"

class Vertex
{
    public:
        Vertex();
        Vertex(uint vertexID, vector<Edge>& edges);
        Vertex(const Vertex& orig);
        virtual ~Vertex();
        
        uint&           vertexID();
        float&          upperPriceLimit();
        float&          myopicPrice();
        float&          emmetropicPrice();
        vector<Edge>&   incomingEdges();
        short&          roundAssignation();
        
        uint            getVertexID() const;
        float           getMyopicPrice() const;
        float           getEmmetropicPrice() const;
        size_t          getIncomingEdgeCount() const;
        
        /**
         * Swap edges/connections of vertex with given argument.
         * @param edges
         */
        void swapEdges(vector<Edge>& edges);
        
        /**
         * Overload <<-operator for output operation.
         * @param stream
         * @param vertex
         * @return Used instance of ostream.
         */
        friend ostream& operator<< (ostream& stream, const Vertex& vertex);
        
        /**
         * Compares two vertices on base of number of incoming edges.<br>
         * Used in CDHS heuristic.
         * @param str
         * @return 
         */
        bool operator < (const Vertex& str) const;
        
        /**
         * Sort predicate based on emmetropic prices of involved vertices.
         * @param a
         * @param b
         * @return 
         */
        static bool compareEmmetropicPrices(const Vertex& a, const Vertex& b);

        /**
         * Sort predicate based on edge counts (ignoring weighting).
         * @param a
         * @param b
         * @return 
         */        
        static bool compareIncomingEdgeCount(const Vertex& a, const Vertex& b);
        
        /**
         * Calculates myopic price (depends on number of incoming edges).
         * MP equates to 50% of the weight of all incoming edges connected to
         * IS vertices.
         * @return Calculated myopic price of vertex.
         */
        float calculateMyopicPrice();

        /**
         * Calculates emmetropic price (EP), which accounts for influence of
         * neighbours that are not part of influence set.
         * EP is calculated as P(r_a > r_b) * MP_b * W_ba + MP_a, where "a" is
         * currently examined and "b" is adjacent vertex.
         * @param vertices Collection of vertices for which emmetropic prices are to
         * be calculated.
         * @return Calculated emmetropic price of vertex (note: not stored in 
         * member variable).
         */
        float calculateEmmetropicPrice();
        
        /**
         * Calculates upper price limit of agent/vertex, assuming up-to-date
         * buyer set and influence set data.
         * @return Upper price limit for this agent / vertex.
         */
        float calculatePriceLimit();
        
        /**
         * Sets static IS reference.
         * @param _influenceSet
         * @see Vertex._influenceSet
         */
        static void setIS(unordered_set<uint>& influenceSet);
        
        /**
         * Sets static BS reference.
         * @param buyerSet
         * @see Vertex._buyerSet
         */
        static void setBS(unordered_set<uint>& buyerSet);
        
        /**
         * Sets static vertex ID map reference.
         * @param vertexIDMap
         * @see Graph._vertexIDMap
         */
        static void setVertexIDMap(map<uint, uint>& vertexIDMap);
        
    protected:
        // Vertex data
        uint    _vertexID;
        // Upper price limit agent is willing to pay
        float   _upperPriceLimit;
        // Myopic price
        float   _myopicPrice;
        // Emmetropic price (used only in AN-CDHS heuristic)
        float   _emmetropicPrice;
        // Determines index of round in which vertex is to be processed
        short   _roundAssignation;
        
        /**
         * Connections to other vertices.
         * Remark: Represents _incoming_ edges, as it is more sensible
         * to work with them than with outgoing edges. 
         */
        vector<Edge> _incomingEdges;
        
        /**
         * Reference to influence set (needed for calculation of MPs).
         */
        static unordered_set<uint>* _influenceSet;
        
        /**
         * Reference to set of buyers (needed for calculation of price limits).
         */
        static unordered_set<uint>* _buyerSet;
        
        /**
         * Reference to Graph._vertexIDMap containing map of vertex IDs/indices.
         * @see Graph._vertexIDMap
         */
        static map<uint, uint>* _vertexIDMap;
} ;

#endif	/* NODE_H */