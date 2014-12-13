/* 
 * File:   Graph.cpp
 * Author: RM
 * 
 * Created on 26. Juli 2013, 22:55
 */

#include "Graph.h"
#include "InputManagement.h"
#include <fstream>

Graph::Graph(string name, bool hasNativeEdgeWeights, bool directed, uint estimatedSize) 
            : _name(name), _hasNativeEdgeWeights(hasNativeEdgeWeights), _isDirected(directed), _estimatedSize(estimatedSize)
{
    _vertexCount = 0;
    
    //  Reserve estimated number of vertices
    if (estimatedSize) {
        cout << "ESTIMATED SIZE = " << _estimatedSize << endl;
        _vertices.reserve(_estimatedSize);
    }
}

Graph::Graph(const Graph& orig)
{
    // Scalar data
    _vertexCount              = orig._vertexCount;
    _hasNativeEdgeWeights   = orig._hasNativeEdgeWeights;
    _isDirected             = orig._isDirected;
    _estimatedSize          = orig._estimatedSize;
    _originalSize           = orig._originalSize;
    
    // Vertex collection
    _vertices               = orig._vertices;
    
    // Maps
    _vertexIDMap              = orig._vertexIDMap;
}

Graph::Graph()
{
    _vertexCount = 0;
}

Graph::~Graph()
{
}

void Graph::init(string name, bool hasNativeEdgeWeights, bool directed, uint estimatedSize)
{
    _name                   = name;
    _hasNativeEdgeWeights   = hasNativeEdgeWeights;
    _isDirected             = directed;
    _estimatedSize          = estimatedSize;
    
    //  Reserve estimated number of vertices
    if (estimatedSize) {
        _vertices.reserve(_estimatedSize);
    }
}

void Graph::addVertex(const Vertex& vertex, uint vertexID)
{
    _vertices[_vertexCount] = vertex;
    // Store vertex ID and index in map
    _vertexIDMap[vertexID] = _vertexCount;
    _vertexCount++;
}

ostream& operator<<(ostream& stream, const Graph& graph)
{
    stream << endl;
    stream << "** Graph name:\t\t\t" << graph._name << endl;
    stream << " *   Native edge weighting?\t" << graph._hasNativeEdgeWeights << endl;
    stream << " *   Is directed?\t\t" << graph._isDirected << endl;
    stream << " *   Vertices" << endl;
    stream << "     * Size:\t\t\t" << graph._vertices.size() << endl;
    stream << "     * Capacity:\t\t" << graph._vertices.capacity() << endl;
    
    uint edgeCount = 0;
    for (Vertex vertex : graph._vertices) {
        edgeCount += vertex.incomingEdges().size();
    }
    
    stream << " *   Edge count:\t\t" << edgeCount << endl;
    
    return stream;
}

void Graph::scanForPassiveVertices(const vector<float>& roundAssignations)
{
    uint        count               = 0;
    uint        destinationID       = -1;
    const uint  lastActiveIndex     = _vertices.size() - 1;
    const uint  assignationsSize    = roundAssignations.size();
    Vertex        currVertex;
    
    // Source IDs are already processed. Here: Identify passive destination
    // vertices (which don't appear in the left column)
    
    for (uint index = 0; index < lastActiveIndex; index++) {
        // Examine destinations of all edges in this vertex and check if
        // they are already in vertex collection. If not: Identified as passive
        // vertex, add to collection.
        for (Edge& edge : _vertices[index].incomingEdges()) {
            destinationID = edge.destinationID();
            if (!_vertexIDMap.count(destinationID)) {
                // Add passive vertex
                currVertex.vertexID()           = destinationID;
                currVertex.roundAssignation() = roundAssignations[index % (assignationsSize - 1)] *
                                              ParametrizationSettings::NUMBER_OF_ROUNDS + 1;
                _vertexIDMap[destinationID]   = _vertexCount;
                
                // Add to vertex collection
                _vertices.push_back(currVertex);
                
                // Increment counters
                count++;                
                _vertexCount++;
            }
        }
    }
    
    // Update passive vertex count
    _passiveVertexCount = count;
}

void Graph::finalizeInputParsing(const vector<float>& roundAssignations)
{
    // Loop through vertex collection, register them in map
    // NOTE: Not needed anymore; is done while parsing input data.
    //mapVertexCollection();

    // Update vertex count
    _vertexCount = _vertices.size();
    
    // Register and store passive vertices, i. e. vertices without any incoming edges.
    scanForPassiveVertices(roundAssignations);
    
    // Store number of vertices
    _originalSize = _vertices.size();
    
    // Free map memory (map was needed for comparison of active with 
    // passive vertices only - e.g.: scanForPassiveVertices())
    _vertexIDMap.clear();
}

const map<uint, uint>& Graph::mapVertexCollection(bool considerNegativeMPs)
{
    // Reset vertex map and count
    _vertexIDMap.clear();
    _vertexCount = 0;
    
    // Store references to all vertices
    if (considerNegativeMPs) {
        // Loop through vertices and store indices in map
        for (uint index = 0; index < _vertices.size(); index++) {
             // Store vertex ID and index in map
            _vertexIDMap[_vertices[index].getVertexID()] = index;
            _vertexCount++;
        }
    }
    
    // Don't consider/store vertices with negative MPs
    else {
        // Loop through vertices and store indices in map
        for (uint index = 0; index < _vertices.size(); index++) {
             // Store vertex ID and index in map
            if (_vertices[index].getMyopicPrice() >= 0) {
                _vertexIDMap[_vertices[index].getVertexID()] = index;
                _vertexCount++;
            }
        }
    }
    
    return _vertexIDMap;
}

void Graph::clearVertexMap()
{
    _vertexIDMap.clear();
}

vector<Vertex>& Graph::vertices()
{
    return _vertices;
}

map<uint, uint>& Graph::vertexIDMap()
{
    return _vertexIDMap;
}

uint& Graph::passiveVertexCount()
{
    return _passiveVertexCount;
}

void Graph::sortVertices()
{
    // Sort vertices
    sort(_vertices.begin(), _vertices.end());
}

 uint Graph::deleteInactiveVertices(bool sortedByMP)
{
    uint count = 0;
    
    // Iterate through vertices, delete all vertices with MP = 0 (deaf vertices) or smaller (influence set).
    if (sortedByMP) {
        // Delete IS vertices from vertex collection
        while (count < _vertices.size() && _vertices[count++].myopicPrice() < 0);

        // Erase inactive vertices
        _vertices.erase(_vertices.begin(), _vertices.begin() + count - 1);
        _vertices.shrink_to_fit();
    }
    // If not sorted by MP: Use remove-erase idiom to iterate through and check
    // all vertices.
    else {
        count = _vertices.size();
        _vertices.erase(remove_if(_vertices.begin(), _vertices.end(), Graph::removeInactiveVertexPred), _vertices.end());
        count -= _vertices.size() - 1;
    }
    
    return count - 1;
}
 
 uint Graph::getOriginalSize() const
 {
     return _originalSize;
 }
 
 uint Graph::restructure(bool sortedByMP)
{
    uint deletedVertexCount;
    
    // Sort vertices
    sortVertices();
    
    // Delete inactive vertices to reduce memory load
    deletedVertexCount = deleteInactiveVertices(sortedByMP);
    
    return deletedVertexCount;
}
 
 bool Graph::removeInactiveVertexPred(const Vertex& vertex)
 {
     return vertex.getMyopicPrice() < 0;
 }
 
 float Graph::calculateMedianMP()
 {
     size_t n = _vertices.size() / 2;
     // nth_element() sorts only one element in correct place (and uses default
     // sort predicate - the < operator - for it).
     nth_element(_vertices.begin(), _vertices.begin() + n, _vertices.end());
     
     return _vertices[n].myopicPrice();
 }