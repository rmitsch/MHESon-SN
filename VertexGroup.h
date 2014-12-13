/* 
 * File:   VertexGroup.h
 * Author: RM
 *
 * Created on 26. Juli 2013, 23:26
 */

#ifndef VERTEXGROUP_H
#define	VERTEXGROUP_H

#include "Definitions.h"
#include "Graph.h"

class VertexGroup
{
    public:
        VertexGroup(uint firstIndex, uint lastIndex, float lowerLimit, float upperLimit, float groupOffer);
        VertexGroup(const VertexGroup& orig);
        VertexGroup();
        virtual ~VertexGroup();
        
        uint&   firstIndex();
        uint&   lastIndex();
        float&  upperLimit();
        float&  lowerLimit();
        float&  offer();
        float   getOffer() const;
        uint    getMemberCount() const;
        
    private:
        // After calculating their myopic prices, vertices are arranged accordingly -
        // therefore the vertices of each group are confined inside a limited region.
        uint    _firstIndex;
        uint    _lastIndex;
        float   _upperLimit;
        float   _lowerLimit;
        float   _offer;
} ;

#endif	/* VERTEXGROUP_H */

