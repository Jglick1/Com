//
//  graph.cpp
//  Com
//
//  Created by Jonah Glick on 8/17/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "graph.hpp"

Graph::Graph() {
    //printf("graph default constructor\n");
}

Graph::Graph(int vertexCount) :
_adjacencyMatrix(vertexCount,std::vector<double>(vertexCount,0.0)),
_vertexCount(vertexCount-2) //not including source and destination nodes
{
    
    //printf("graph constructor\n");
    //this->_vertexCount = vertexCount;
    
    
    //vector< vector<int> > v(10,vector<int>(10,0))
    /*
    for(int i = 0; i<vertexCount; i++) {
        for(int j = 0; j < vertexCount; j++) {
            printf("%f\t",this->_adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
     */
    
    /*
    this->_vertexCount = vertexCount;
    this->_adjacencyMatrix = new double*[vertexCount];
    
    for(int i = 0; i < vertexCount; i++) {
        this->_adjacencyMatrix[i] = new double[vertexCount];
        
        for(int j = 0; j < vertexCount; j++) {
            this->_adjacencyMatrix[i][j] = 0.0;
        }
        
    }
    */

    
    
}

void Graph::printAdjacencyMatrix() {
    for(int i = 0; i < this->_vertexCount+2; i++) {
        
        for(int j = 0; j < this->_vertexCount+2; j++) {
            
            printf("%f\t", this->_adjacencyMatrix[i][j]);

        }
        printf("\n");
    }
}

double Graph::getWeight(int vertex1, int vertex2) {
    
    //return 0.0;
    /*
    if(!(this->_vertices.find(vertex1) == this->_vertices.end()) && !(this->_vertices.find(vertex2) == this->_vertices.end())){
        printf("both in the table!\n");
    }
    */
    
    if(!(this->_vertices.find(vertex1) == this->_vertices.end()) && !(this->_vertices.find(vertex2) == this->_vertices.end())) {                    //if vertex1 and vertex2 are in the _vertices table
        return std::sqrt(std::pow(this->_vertices[vertex1].x - this->_vertices[vertex2].x,2) + std::pow(this->_vertices[vertex1].y - this->_vertices[vertex2].y,2));
    }
    else {
        return 0.0;
    }
}

void Graph::addToVertexTable(int vertex1, int x, int y) { // int to position(x, y)
    this->_vertices.insert(std::make_pair(vertex1, Vector2(x, y)));
}
/*
int Graph::VertexNameToId(int vertex){
    return this->_vertices
}

int Graph::VertexIdToNmae(int id) {
    return this->_vertices[id];
}
*/


void Graph::addEdge(int vertex1, int vertex2, double weight) {
    
    //if(vertex1 >= 0 && vertex1 < this->_vertexCount+2 && vertex2 >= 0 && vertex2 < this->_vertexCount+2) {
        this->_adjacencyMatrix[vertex1][vertex2] = weight;
        this->_adjacencyMatrix[vertex2][vertex1] = weight;
    //}
}

void Graph::removeEdge(int vertex1, int vertex2) {
    if(vertex1 >= 0 && vertex1 < this->_vertexCount && vertex2 >= 0 && vertex2 < this->_vertexCount) {
        this->_adjacencyMatrix[vertex1][vertex2] = 0.0;
        this->_adjacencyMatrix[vertex2][vertex1] = 0.0;
    }
}

Graph::~Graph() {
    /*
    printf("%f\n",this->_adjacencyMatrix[0][0]);

    if(0) {
        for(int i = 0; i<this->_vertexCount; i++) {
            delete[] this->_adjacencyMatrix[i];
        }
        delete[] this->_adjacencyMatrix;
    }
    */
    //printf("graph destructor\n");
    
}

std::map<int, Vector2> Graph::getVertexTable() {
    return this->_vertices;
}

int Graph::getVertexCount() {
    return this->_vertexCount;
}

void Graph::removeEdgesForSourceAndDestinationVertices() {
    
    for(int i = 0; i < this->_vertexCount+1; i++) {
        this->_adjacencyMatrix[this->_vertexCount][i] = 0.0;
        this->_adjacencyMatrix[i][this->_vertexCount] = 0.0;
        
        this->_adjacencyMatrix[this->_vertexCount+1][i] = 0.0;
        this->_adjacencyMatrix[i][this->_vertexCount+1] = 0.0;
    }
}

std::vector<int> Graph::getAStarVertexPath(std::map<int, double> & h) {
    std::vector<int> vertexPath;
    //using https://www.youtube.com/watch?v=eSOJ3ARN5FM&t=598s&frags=pl%2Cwn
    
    printf("start a*\n");
    
    std::vector<int> openVertices;
    std::vector<int> closedVertices;
    
    std::map<int, double> distanceFromA;
    std::map<int, int> previousVertex;
    std::map<int, double> fValue;
    int currentVertex = this->_vertexCount+1;   //the start vertex
    
    //add current vertex to open vertices
    openVertices.push_back(currentVertex);
    //distanceFromA[currentVertex] = 0.0;
    //fValue[currentVertex] = distanceFromA[currentVertex] + h[currentVertex];
    
    
    
    while(currentVertex != this->_vertexCount) {    //while it's not the end vertex
    
        //find vertices adjacent to the current vetex and add them to the list of open vertices
        for(int i = 0; i < (this->_vertexCount+1); i++) {
            if(this->_adjacencyMatrix[currentVertex][i] > 0.0) {
                //if not already in open vertices and not in closed vertices, add to open vertices
                bool alreadyIn = 0;
                for(int j : openVertices) {
                    if(j == i) {
                        alreadyIn = 1;
                    }
                }
                bool inClosed = 0;
                for(int j : closedVertices) {
                    if(j == i) {
                        inClosed = 1;
                    }
                }
                if(!alreadyIn && !inClosed) {
                    openVertices.push_back(i);
                }

                
                if(!inClosed) {
                    //find or update the vertex's DtoA
                    distanceFromA[i] = this->_adjacencyMatrix[currentVertex][i];
                    
                    //find or update the vetex's fValue
                    fValue[i] = distanceFromA[i] + h[i];
                    
                    //find or update the vertex's previous vertex
                    previousVertex[i] = currentVertex;
                }
            }
        }
        
        
        //put current vetex in list of closed Vertices
        //https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
        openVertices.erase(std::remove(openVertices.begin(), openVertices.end(), currentVertex), openVertices.end());
        closedVertices.push_back(currentVertex);
        
        /*
        for (int i : openVertices) {
            //find or update the vertex's DtoA
            distanceFromA[i] = this->_adjacencyMatrix[currentVertex][i];
            
            //find or update the vetex's fValue
            fValue[i] = distanceFromA[i] + h[i];
            
            //find or update the vertex's previous vertex
            previousVertex[i] = currentVertex;
        }
        
        */
        
        
        printf("current: %d, open: ", currentVertex);
        for(int i : openVertices) {
            printf("%d ", i);
        }
        printf("\n");
        
        for(std::pair<int,int> k : previousVertex) {
            printf("\t%d prev is %d\n", k.first, k.second);
        }
        
        
        //find new current vertex
        
        double min = 1000000;
        double minVertex = -1;
        
        for(int i : openVertices) {
            if (fValue[i] < min) {
                min = fValue[i];
                minVertex = i;
            }
        }
        
        //set new current Vertex
        currentVertex = minVertex;
        
        //printf("end: %d\n",currentVertex);
        
    }
    

    
    /*
    //find vertices adjacent to the current vetex and add them to the list of open vertices
    for(int i = 0; i < this->_vertexCount; i++) {
        if(this->_adjacencyMatrix[currentVertex][i] > 0.0) {
            //if not already in open vertices, add to open vertices
            bool alreadyIn = 0;
            for(int j : openVertices) {
                if(j == i) {
                    alreadyIn = 1;
                }
            }
            if(!alreadyIn) {
                openVertices.push_back(i);
            }
            
            //find or update the vertex's DtoA
            distanceFromA[i] = this->_adjacencyMatrix[currentVertex][i] + distanceFromA[currentVertex];
            
            //find or update the vetex's fValue
            fValue[i] = distanceFromA[i] + h[i];
            
            //find or update the vertex's previous vertex
            previousVertex[i] = currentVertex;
            
        }
    }
    
    //close the vertex
    openVertices.erase(std::remove(openVertices.begin(), openVertices.end(), currentVertex), openVertices.end());
    
    
    //choose its sucessor
    
    //the same code again
    
    
    */
    //this loop shoudl be
    /*
    while(currentVertex != this->_vertexCount) { //this is the destination vertex
        
    }
    */
    
    printf("first while loop calculated\n");
    
    
    
    //check to see if the next chosen vertex is this->_vertexCount. If so

    
    
    
    while(currentVertex != (this->_vertexCount+1)) { // while current vertex is not the destination
        
        printf("current vertex %d\n", currentVertex);
        
        vertexPath.insert(vertexPath.begin(), currentVertex);// starts with the destination
        
        currentVertex = previousVertex[currentVertex];
        
    }
    
    
    
    vertexPath.pop_back(); //remove the destination /Graph doens't know these coordinates, and when it tries to convert, it thinks they are 0,0
    
    
    
    /*
    double min = 1000000;               //MAKE THIS INF?
    double minVertex = -1;
    double f = 0.0;
    
    //find open vertices
    std::vector<int> openVertices;
    for(int i = 0; i < this->_vertexCount; i++) {
        if(this->_adjacencyMatrix[this->_vertexCount][i] > 0.0) {
            openVertices.push_back(i);
        }
    }
    
    //find next node
    for(int i : openVertices) {
        f = this->_adjacencyMatrix[this->_vertexCount][i] + h[i];
        if(f < min) {
            min = f;
            minVertex = i;
        }
    }
    
    vertexPath.push_back(minVertex);
    
    //find next node
    findNextNode(vertexPath, h, minVertex);
    */
    
    printf("vertex path return\n");
    
    
    return vertexPath;
    
}

void Graph::findNextNode(std::vector<int> & vertexPath,std::map<int, double> & h, int currentVertex) {
    
    double min = 1000000;               //MAKE THIS INF?
    double minVertex = -1;
    double f = 0.0;
    
    //find open vertices
    std::vector<int> openVertices;
    for(int i = 0; i < this->_vertexCount; i++) {             //vertexCount+2
        if(this->_adjacencyMatrix[currentVertex][i] > 0.0) {
            openVertices.push_back(i);
        }
    }
    //if the end vertex is in sight
    if(this->_adjacencyMatrix[currentVertex][this->_vertexCount+1] > 0.0) {
        openVertices.push_back(this->_vertexCount+1);
    }
    
    
    //find next node
    for(int i : openVertices) {
        f = this->_adjacencyMatrix[currentVertex][i] + h[i];
        if(f < min) {
            min = f;
            minVertex = i;
        }
    }
    
    if(minVertex == this->_vertexCount+1) { //if it is the destination
        //vertexPath.push_back(minVertex);
    }
    else {
        vertexPath.push_back(minVertex);
        findNextNode(vertexPath, h, minVertex);
    }
    
}

std::vector<Vector2> Graph::convertToMovementOrders(std::vector<int> vertices) {
    std::vector<Vector2> orders;
    for(int i : vertices) {
        orders.push_back(this->_vertices[i]);
    }
    
    return orders;
    
}

void Graph::addCoverNode(int vertex, std::vector<Cover> coverNodes) {
    this->_vertexToCover.insert(std::make_pair(vertex, coverNodes));
}


/*
std::map<int, Vector2> Graph::getCoverNodes(int vertex) {

    
    
    
    
    
}
*/
