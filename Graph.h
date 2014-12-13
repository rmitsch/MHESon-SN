/* 
 * File:   Graph.h
 * Author: RM
 *
 * Created on 26. Juli 2013, 22:55
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "Definitions.h"
#include "Vertex.h"
#include "Edge.h"

class Graph
{
    public:
        Graph(string name, bool hasNativeEdgeWeights, bool directed, uint estimatedSize = 0);
        Graph(const Graph& orig);
        Graph();
        virtual ~Graph();
    
        /**
         * Adds vertex to internal vertex collection.
         * Assumption: Vertices are read in order, so there aren't any duplicates
         * used as parameters for addVertex().
         * @param vertex
         * @deprecated Use vertices() in combination with finalizeInputParsing() instead.
         */
        void addVertex(const Vertex& vertex, uint vertexID);
        
        /**
         * Finalizes parsing of input file.
         * @param roundAssignations Collection containing indices of rounds
         * vertices are supposed to enter.
         */
        void finalizeInputParsing(const vector<float>& roundAssignations);
        
        /**
         * 
         * @param stream
         * @param vertex
         * @return 
         */
        friend ostream& operator<< (ostream& stream, const Graph& graph);
        
        /**
         * Returns estimated number of vertices in graph.
         * @return 
         */
        uint& estimatedSize();
        
        /**
         * Returns internal vertex collection.
         * @return 
         */
        vector<Vertex>& vertices();

        /**
         * Returns map representing a vertex ID/index scheme.
         * @return 
         */
        map<uint, uint>& vertexIDMap();
        
        /**
         * Returns reference to count of passive/deaf vertices.
         * @return Reference to count of passive/deaf vertices.
         */
        uint& passiveVertexCount();
        
        /**
         * Sorts vertices using the overloaded comparison operator < 
         * (i. e. sorting by their myopic prices).
         */
        void sortVertices();
        
        /**
         * Deletes influence set vertices and those without any incoming edges -
         * former have already been processed and are available in
         * GMH::_influenceSet, latter don't account for any network effects
         * and are therefore to dismiss.
         * @param isSorted Defines whether or not vertices are sorted by their
         * myopic prices (so that not all vertices have to be iterated in order 
         * to delete inactive ones).
         * @return Number of vertices deleted.
         */
        uint deleteInactiveVertices(bool sortedByMP);
        
        /**
         * Initializes graph settings (same parameters as constructor).
         * @param name
         * @param hasNativeEdgeWeights
         * @param directed
         * @param estimatedSize
         */
        void init(string name, bool hasNativeEdgeWeights, bool directed, uint estimatedSize = 0);
        
        /**
         * Returns original size of graph (i.e. when initialized / after file parsing).
         * @return 
         */
        uint getOriginalSize() const;
        
        /**
         * Sorts vertices and deletes deaf or already used ones (i. e.: Members
         * of influence set and deaf vertices, as former are stored in a more compact
         * form elsewhere and latter don't account for network effects as they
         * have a zero percent chance to take up on the offer and influence 
         * other vertices).
         * Used in phase 3.
         * @see GroupMappingHeuristics._influenceSet
         * @return Number of deleted vertices.
         */
        uint restructure(bool sortedByMP);
        
        /**
         * Calculates median MP. Necessary if vertices not sorted after MP.
         * @return Median MP of vertices in this graph.
         */
        float calculateMedianMP();
        
        /**
         * Add delivered vector of vertices to vertex collection.
         * @param considerNegativeMPs Determines if vertices with negative MPs
         * (e. g., deaf-0 and IS vertices) are to be 
         * @deprecated Not used anymore; mapping takes place on-the-fly in
         * InputManagement::parseXFile() instead.
         * @return Reference to vertex ID/index map.
         */
        const map<uint, uint>& mapVertexCollection(bool considerNegativeMPs = true);
        
        /**
         * Clears vertex map.
         */
        void clearVertexMap();
        
    private:
        
        /**
         *  --- Methods ---
         */
        
        /**
         * Scans edges of saved vertices for passive vertices (only incoming,
         * no outgoing edges. If passive vertices are found, they are added to
         * the internal vertex collection.
         */
        void scanForPassiveVertices(const vector<float>& roundAssignations);
        
        /**
         * Remove-erase idiom for deleting inactive vertices.
         * @param vertex
         * @return 
         */
        static bool removeInactiveVertexPred(const Vertex& vertex);
        
        /**
         *  --- Variables ---
         */
        
        // Actual graph data
        vector<Vertex>    _vertices;
        map<uint, uint> _vertexIDMap;
        
        // Metadata
        string  _name;
        bool    _hasNativeEdgeWeights;
        bool    _isDirected;
        uint    _estimatedSize;
        
        /**
         *  Auxiliary variables
         */
        uint    _vertexCount;
        // Stores number of passive/deaf vertices (e. g. vertices not contributing
        // to any network effects).
        uint    _passiveVertexCount;
        // Provides information about graph size when initialized.
        uint    _originalSize;
} ;

#endif	/* GRAPH_H */