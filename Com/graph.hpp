//
//  graph.hpp
//  Com
//
//  Created by Jonah Glick on 8/17/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include "globals.hpp"
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>

class Graph {
    
public:
    Graph();
    Graph(int vertexCount);
    void addEdge(int vertex1, int vertex2, double weight);
    void removeEdge(int vertex1, int vertex2);
    ~Graph();
    void addToVertexTable(int vertex1, int x, int y); // map from id to position
    double getWeight(int vertex1, int vertex2);
    void printAdjacencyMatrix();
    std::map<int, Vector2> getVertexTable();
    int getVertexCount();
    void removeEdgesForSourceAndDestinationVertices();
    std::vector<int> getAStarVertexPath(std::map<int, double> & h);
    void findNextNode(std::vector<int> & vertexPath,std::map<int, double> & h, int currentVertex);
    std::vector<Vector2> convertToMovementOrders(std::vector<int> vertices);

    void addCoverNode(int vertex, std::vector<CoverNode> coverNodes);
    //std::map<int, Vector2> getCoverNodes(int vertex);
    
    std::map<int, Vector2> getCoverNodeTable(); // coverNode to Coordinates
    
    void addToCoverTable(int coverNode, int x, int y);
    void initializeCoverTable(std::map<int, Vector2> temp);
    void connectCoverNodeToVertex(int vertex, std::vector<int> coverNodes);
    std::vector<int> getCoverNodes(int vertex);
    
    void setCoverToOccupied(int coverNode);
    
    bool isCoverOccupied(int coverNode);
    
private:
    //double** _adjacencyMatrix;
    std::vector< std::vector<double> > _adjacencyMatrix;
    
    int _vertexCount;
    std::map<int, Vector2> _vertices; //id to location (to get vertex location)
    
    
    
    
    
    std::map<int, Vector2> _coverNodes; //cover nodes coordinates

    std::map<int, std::vector<int> > _vertexToCoverNode;
    
    std::map<int, bool> _coverNodeOccupiedTable; //covernode
    
    
    
    
    std::map<int, std::vector<Vector2> > _buildingToIndoorCoverNodes;
    
};


#endif /* graph_hpp */
