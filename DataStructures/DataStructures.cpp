//
//  DataStructures.cpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 20/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "DataStructures.hpp"

int AdjacencyList::addNewConnection(Vertex* vertex1,Vertex*  vertex2, Color* c){
    //Checks if the connection was already made
    //returns 0 if connection wasnt made
    //Adds the connection
    adjMatrix[adjIndex][0] = vertex1;
    adjMatrix[adjIndex][1] = vertex2;
    connectionColors[adjIndex] = c;
    adjIndex+=1;
    return(1);
}

Vertex* AdjacencyList::getFirstVertex(int index) {
    return(adjMatrix[index][0]);
}

Vertex* AdjacencyList::getSecondVertex(int index) {
    return(adjMatrix[index][1]);
}


int ElementsList::addVertex(Vertex* p, Color* c){
    if(nVertices >= MAX_NUM_POINTS)
        return(1);
    memoryVertices[nVertices] = p;
    memoryColors[nVertices] = c;
    nVertices++;
    upToDate = false;
    return(0);
}
int ElementsList::addVertexConnection(Vertex* p1, Vertex* p2, Color* c){
    if(adjList.getNumberOfConnections() >= MAX_NUM_POINTS*3)
        return(1);
    adjList.addNewConnection(p1, p2, c);
    upToDate = false;
    return(0);
}
int ElementsList::addPolygon(Polygon poly){
    if(poly.getPolygonSize() <= 0)
        return(1);
    int result = 0;
    for(int i = 0; i < poly.getPolygonSize(); i++){
        result = addVertex(poly.getVertex(i), poly.getColor(i));
        adjList.addNewConnection(poly.getVertex(i), poly.getVertex((i+1)%poly.getPolygonSize()), poly.getColor(i));
        if(result != 0)
            return(1);
    }
    upToDate = false;
    return(0);
}
int ElementsList::updateLists(GLuint *VBO, GLuint *VAO, GLuint *CBO){
    if(!upToDate){
        //Limpa todos os pontos
        for(int i = 0; i < (nVertices + adjList.getNumberOfConnections()); i++){
            vertices[i].assignCoord(-1, -1, -1);
            colors[i].assignColor(255, 200, 0);
        }
        //Desenha todos os pontos
        for(int i = 0; i < nVertices; i++){
            vertices[i].assignCoord((*memoryVertices[i]).x, (*memoryVertices[i]).y, (*memoryVertices[i]).z);
            colors[i].assignColor((*memoryColors[i]).r, (*memoryColors[i]).g, (*memoryColors[i]).b);
        }
        //Desenha todas as arestas
        for(int i = 0; i < adjList.getNumberOfConnections(); i++){
            vertices[i*2+nVertices].assignCoord((*adjList.getFirstVertex(i)).x,(*adjList.getFirstVertex(i)).y,(*adjList.getFirstVertex(i)).z);
            colors[i*2+nVertices].assignColor((*adjList.getConnectionColor(i)).r, (*adjList.getConnectionColor(i)).g, (*adjList.getConnectionColor(i)).b);
            vertices[i*2+nVertices +1].assignCoord((*adjList.getSecondVertex(i)).x,(*adjList.getSecondVertex(i)).y,(*adjList.getSecondVertex(i)).z);
            colors[i*2+nVertices + 1].assignColor((*adjList.getConnectionColor(i)).r, (*adjList.getConnectionColor(i)).g, (*adjList.getConnectionColor(i)).b);
        }
        glBindBuffer( GL_ARRAY_BUFFER, *VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, *CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    }
    return(0);
}

Vertex *ElementsList::getVerticesList() { 
    return(vertices);
}

Color *ElementsList::getColorsList() { 
    return(colors);
}

int ElementsList::getNVertices() {
    return(nVertices);
}

int ElementsList::getNConnections() {
    return(adjList.getNumberOfConnections()*2);
}

bool ElementsList::isUpToDate() { 
    return(upToDate);
}



Color* AdjacencyList::getConnectionColor(int index){
    return(connectionColors[index]);
}


int addNewConnection(Vertex* vertex1, Vertex* vertex2, Color* c);
int addsPolygon(Polygon polygon);
Color* getConnectionColor(int index);
DCEdge* getDCEdge(int index);

AdjacencyList* ElementsList::getAdjList(){
    return(&adjList);
}
int ElementsList::removeVertex(Vertex* p){
    for(int i = 0; i < nVertices; i++){
        if(memoryVertices[i] == p){
            for(int j = i+1; j < nVertices; j++)
                memoryVertices[j-1] = memoryVertices[j];
        }
    }
    return(0);
}
