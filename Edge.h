/* 
 * File:   Edge.h
 * Author: RM
 *
 * Created on 26. Juli 2013, 22:56
 */

#ifndef EDGE_H
#define	EDGE_H

#include "Definitions.h"

class Edge
{
    public:
        Edge();
        Edge(uint sourceID, uint destinationID, float weight);
        Edge(const Edge& orig);
        virtual ~Edge();
    
        uint&   sourceID();
        uint&   destinationID();
        float&  weight();
        float   getWeight() const;
        
    private:
        uint    _sourceID;
        uint    _destinationID;
        float   _weight;
} ;

#endif	/* EDGE_H */

