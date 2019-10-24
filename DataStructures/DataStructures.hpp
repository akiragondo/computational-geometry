//
//  DataStructures.hpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 20/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#ifndef DataStructures_hpp
#define DataStructures_hpp


class DCEdge;
class Vector;
class Polygon;
#include "CompositeStructures.hpp"
#include "Common.hpp"


class AdjacencyList{
private:
    int adjIndex = 0;
    Vertex* adjMatrix[MAX_NUM_POINTS*3][2];
    Color* connectionColors[MAX_NUM_POINTS*3];
public:
    AdjacencyList(){
        for(int i = 0; i < MAX_NUM_POINTS*3; i++){
            adjMatrix[i][0] = nullptr;
            adjMatrix[i][0] = nullptr;
            connectionColors[i] = nullptr;
        }
    }
    int getNumberOfConnections(){
        return(adjIndex);
    }
    int addNewConnection(Vertex* vertex1, Vertex* vertex2, Color* c);
    int addsPolygon(Polygon polygon);
    Color* getConnectionColor(int index);
    Vertex* getFirstVertex(int index);
    Vertex* getSecondVertex(int index);
};

class ElementsList {
private:
    int nVertices = 0;
    Vertex* memoryVertices[MAX_NUM_POINTS];
    Color* memoryColors[MAX_NUM_POINTS];
    Vertex vertices[MAX_NUM_POINTS];
    Color colors[MAX_NUM_POINTS];
    AdjacencyList adjList;
    bool upToDate = true;
    Color defaultVertexColor;
    Color defaultConnectionColor;
public:
    int addVertex(Vertex* p, Color* c);
    int addVertexConnection(Vertex* p1, Vertex* p2, Color* c);
    int addPolygon(Polygon poly);
    int removeVertex(Vertex* p);
    int updateLists(GLuint *VBO, GLuint *VAO, GLuint *CBO);
    Vertex* getVerticesList();
    Color* getColorsList();
    AdjacencyList* getAdjList();
    int getNVertices();
    int getNConnections();
    bool isUpToDate();
};

#endif /* DataStructures_hpp */
