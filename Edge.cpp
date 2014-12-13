/* 
 * File:   Edge.cpp
 * Author: RM
 * 
 * Created on 26. Juli 2013, 22:56
 */

#include "Edge.h"

Edge::Edge()
{
}

Edge::Edge(uint sourceID, uint destinationID, float weight) : _sourceID(sourceID), _destinationID(destinationID), _weight(weight)
{
}

Edge::Edge(const Edge& orig)
{
    _sourceID        = orig._sourceID;
    _destinationID   = orig._destinationID;
    _weight          = orig._weight;
}

Edge::~Edge()
{
}

uint& Edge::sourceID()
{
    return _sourceID;
}

uint& Edge::destinationID()
{
    return _destinationID;
}

float& Edge::weight()
{
    return _weight;
}

float Edge::getWeight() const
{
    return _weight;
}