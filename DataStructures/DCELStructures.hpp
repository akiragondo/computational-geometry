//
//  DCELStructures.hpp
//  AppsGC
//
//  Created by Gustavo Akira Gondo on 12/09/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#ifndef DCELStructures_hpp
#define DCELStructures_hpp

#include <stdio.h>
#include <iostream>
#include <list>
#include <iterator>
#include "ElementalStructures.hpp"
#include "Common.hpp"
class DCEdge;

class VertexDS{
public:
    //Atributes
    Vertex vertex;
    Color color;
    DCEdge* vertexEdge;
    
    VertexDS(double x, double y, double z);
    VertexDS(double x, double y, double z, double r, double g, double b);
    VertexDS(Vertex v, Color c);
    
    //Functions
    DCEdge* getONext();
    DCEdge* getOPrev();
};

class DSFace;
    
class DCEdge{
public:
    VertexDS* target;
    DCEdge* prev;
    DCEdge* next;
    DCEdge* twin;
    DSFace* face;
    
    Color connectionColor;
    
    DCEdge();
    DCEdge(DCEdge* twin);
    VertexDS* getOrigin();
    int updateDCEdge(DCEdge* e1, DCEdge* e2);
    int copyContents(DCEdge e1);
    int createConnection(VertexDS* vOrigin, VertexDS* vTarget);
    int disconnectEdge(DCEdge *e1, DCEdge *e2);
};

class DSFace{
public:
    DCEdge* faceEdge;
    
    //TODO - return of list of vertices
    std::list<VertexDS*> getFaceVertices();
    std::list<DCEdge *> getFaceEdges();
};


class DCEList{
private:
    int adjIndex = 0;
    DCEdge DCEL[MAX_NUM_POINTS*3];
    
public:
    int getNumberOfConnections(){return(adjIndex);}
    int addNewConnection(VertexDS* vertex1, VertexDS* vertex2, Color* c);
    int removeConnection(DCEdge* edge);
    Color* getConnectionColor(int index);
    DCEdge* getDCEdge(int index);
};

class DCELElementsList {
private:
    int nVertices = 0;
    VertexDS* memoryVertices[MAX_NUM_POINTS];
    DCEList adjList;
    
    Vertex vertices[MAX_NUM_POINTS];
    Color colors[MAX_NUM_POINTS];
    bool upToDate = true;
    Color defaultVertexColor;
    Color defaultConnectionColor;
    
public:
    int addVertex(VertexDS* p);
    int addVertexConnection(VertexDS* p1, VertexDS* p2, Color* c);
    int updateLists(GLuint *VBO, GLuint *VAO, GLuint *CBO);
    Vertex* getVerticesList();
    VertexDS** getMemoryVerticesList();
    Color* getColorsList();
    int getNVertices();
    int getNConnections();
    bool isUpToDate();
    DCEList* getAdjList();
    int removeVertex(VertexDS* p);
    int removeConnection(DCEdge* edge);
};

void printOT(DCEdge* edge);
void printVertex(VertexDS* vertex);

void printEdge(DCEdge* edge);
#endif /* DCELStructures_hpp */
