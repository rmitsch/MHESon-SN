/* 
 * File:   Vertex.cpp
 * Author: RM
 * 
 * Created on 26. Juli 2013, 22:55
 */

#include "Vertex.h"

unordered_set<uint>*    Vertex::_influenceSet;
unordered_set<uint>*    Vertex::_buyerSet;
map<uint, uint>*        Vertex::_vertexIDMap;

Vertex::Vertex()
{
    // Set default values
    _upperPriceLimit    = -1;
    _roundAssignation   = -1;
    _myopicPrice        = ParametrizationSettings::MYOPIC_PRICE_DEFAULT;
    _emmetropicPrice    = ParametrizationSettings::MYOPIC_PRICE_DEFAULT;
}

Vertex::Vertex(uint vertexID, vector<Edge>& edges) : _vertexID(vertexID)
{
    // Swap data with delivered vector / argument
    _incomingEdges.swap(edges);
    
    // Set default values
    _upperPriceLimit    = -1;
    _roundAssignation   = -1;
    _myopicPrice        = ParametrizationSettings::MYOPIC_PRICE_DEFAULT;
    _emmetropicPrice    = ParametrizationSettings::MYOPIC_PRICE_DEFAULT;
}

Vertex::Vertex(const Vertex& orig)
{
    _vertexID             = orig._vertexID;
    _upperPriceLimit    = orig._upperPriceLimit;
    _roundAssignation   = orig._roundAssignation;
    _incomingEdges      = orig._incomingEdges;
    _myopicPrice        = orig._myopicPrice;
    _emmetropicPrice    = orig._emmetropicPrice;
    
    _influenceSet       = orig._influenceSet;
    _buyerSet           = orig._buyerSet;
}

Vertex::~Vertex()
{
}

uint& Vertex::vertexID()
{
    return _vertexID;
}

float& Vertex::upperPriceLimit()
{
    return _upperPriceLimit;
}

float& Vertex::myopicPrice()
{
    return _myopicPrice;
}

float& Vertex::emmetropicPrice()
{
    return _emmetropicPrice;
}

vector<Edge>& Vertex::incomingEdges()
{
    return _incomingEdges;
}

short& Vertex::roundAssignation()
{
    return _roundAssignation;
}

uint Vertex::getVertexID() const
{
    return _vertexID;
}

float Vertex::getMyopicPrice() const
{
    return _myopicPrice;
}

float Vertex::getEmmetropicPrice() const
{
    return _emmetropicPrice;
}

size_t Vertex::getIncomingEdgeCount() const
{
    return _incomingEdges.size();
}

ostream& operator<<(ostream& stream, const Vertex& vertex)
{
    stream << "<> Vertex with ID = " << vertex._vertexID << endl;
    stream << " >   Number of edges = " << vertex._incomingEdges.size() << endl;
    stream << " >   Upper price limit = " << vertex._upperPriceLimit << endl;
    stream << " >   Myopic price = " << vertex._myopicPrice << endl;
    stream << " >   Assigned round = " << vertex._roundAssignation << endl;
    
    return stream;
}

void Vertex::swapEdges(vector<Edge>& edges)
{
    _incomingEdges.swap(edges);
}

bool Vertex::operator < (const Vertex& vertex) const
{
    return _myopicPrice < vertex._myopicPrice;
}

bool Vertex::compareEmmetropicPrices(const Vertex& a, const Vertex& b)
{
    return a._emmetropicPrice < b._emmetropicPrice;
}

bool Vertex::compareIncomingEdgeCount(const Vertex& a, const Vertex& b)
{
    return a._incomingEdges.size() < b._incomingEdges.size();
}

float Vertex::calculateMyopicPrice()
{
    float edgeWeightSum = 0;
    
    // Check if vertex not deaf
    if (_incomingEdges.size()) {
        // Add weight of edge to sum, if corresponding vertex is IS member.
        for (Edge& edge : _incomingEdges) {
            if (_influenceSet->count(edge.destinationID())) {
                edgeWeightSum += edge.weight();
            }
        }
        
        // Calculate final myopic price
        _myopicPrice = edgeWeightSum / 2;
        
        return _myopicPrice;
    }
    
    // If deaf: Assign special (negative) MP value, so vertex can be deleted afterwards
    else {
        _myopicPrice = ParametrizationSettings::MYOPIC_PRICE_DEAF_NODES;
        
        // Return 0 so average MP won't be distorted
        return 0;
    }
}

float Vertex::calculateEmmetropicPrice()
{
    float additionalEPSum = 0;
    
    // If positive MP (otherwise vertex will be deleted anyway)
    if (_incomingEdges.size()) {
        // Base emmetropic on myopic price
        _emmetropicPrice = _myopicPrice;

        // Calculate emmetropic price for each vertex
        for (Edge& edge : _incomingEdges) {
            // Calculate EP here: P(r_a > r_b) * W_ba * EP_AF + MP_a
            additionalEPSum +=  ParametrizationSettings::ANCDHS_POST_ROUND_PROBABILITY *
                                edge.getWeight();
        }

        // Calculate final emmetropic price (and yes, used constant is a magic 
        // number. Who cares.)
        _emmetropicPrice += additionalEPSum * ParametrizationSettings::ANCHDS_EP_ADJUSTMENT_FACTOR;
        
        return _emmetropicPrice;
    }
    
    // Else: Return 0 so average EP won't be distorted
    else {
        _emmetropicPrice = ParametrizationSettings::MYOPIC_PRICE_DEAF_NODES;
        
        return 0;
    }
}

float Vertex::calculatePriceLimit()
{
    _upperPriceLimit = 0;
    
    for (Edge& edge : _incomingEdges) {
        // If neighbour (incoming edge relative to current vertex!)
        // is in posession of product: Take positive influence 
        // in account.
        if (_buyerSet->count(edge.destinationID()) || _influenceSet->count(edge.destinationID())) {
            _upperPriceLimit += edge.weight();
        }
    }
    
    return _upperPriceLimit;
}

void Vertex::setIS(unordered_set<uint>& influenceSet)
{
    _influenceSet = &influenceSet;
}

void Vertex::setBS(unordered_set<uint>& buyerSet)
{
    _buyerSet = &buyerSet;
}

void Vertex::setVertexIDMap(map<uint, uint>& vertexIDMap)
{
    _vertexIDMap = &vertexIDMap;
}