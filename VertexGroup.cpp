/* 
 * File:   VertexGroup.cpp
 * Author: RM
 * 
 * Created on 26. Juli 2013, 23:26
 */

#include "VertexGroup.h"

VertexGroup::VertexGroup(uint firstIndex, uint lastIndex, float lowerLimit, float upperLimit, float offer) : _firstIndex(firstIndex), _lastIndex(lastIndex), _lowerLimit(lowerLimit), _upperLimit(upperLimit), _offer(offer)
{
}

VertexGroup::VertexGroup()
{
}

VertexGroup::VertexGroup(const VertexGroup& orig)
{
    _firstIndex = orig._firstIndex;
    _lastIndex  = orig._lastIndex;
    _upperLimit = orig._upperLimit;
    _lowerLimit = orig._lowerLimit;
    _offer      = orig._offer;
}

VertexGroup::~VertexGroup()
{
}

uint& VertexGroup::firstIndex()
{
    return _firstIndex;
}

uint& VertexGroup::lastIndex()
{
    return _lastIndex;
}

float& VertexGroup::lowerLimit()
{
    return _lowerLimit;
}

float& VertexGroup::upperLimit()
{
    return _upperLimit;
}

float VertexGroup::getOffer() const
{
    return _offer;
}

uint VertexGroup::getMemberCount() const
{
    return (_firstIndex) ? _firstIndex - _lastIndex + 1 : 0;
}