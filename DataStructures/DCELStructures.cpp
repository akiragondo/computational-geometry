//
//  DCELStructures.cpp
//  AppsGC
//
//  Created by Gustavo Akira Gondo on 12/09/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "DCELStructures.hpp"

using namespace std;



int DCEdge::updateDCEdge(DCEdge *e1, DCEdge *e2){
    //Faz processo do E1
    e2->prev = e1;
    e1->next->prev = e2->twin;
    e2->twin->next = e1->next;
    e1->next = e2;
    return(0);
};

int updateDCEdgeTNext(DCEdge *e1, DCEdge *e2){
    //Faz processo do E1
    e2->twin->prev = e1->prev;
    e1->prev->next = e2->twin;
    e1->prev = e2;
    e2->next = e1;
    return(0);
};

int DCEdge::disconnectEdge(DCEdge *e1, DCEdge *e2) {
    //TODO
    return(0);
}

VertexDS* DCEdge::getOrigin(){
    return(prev->target);
}


list<VertexDS *> DSFace::getFaceVertices() {
    list<VertexDS*> vertexList;
    //Coloca a primeira aresta na lista
    DCEdge* iteratorEdge = faceEdge;
    vertexList.push_front(faceEdge->getOrigin());
    iteratorEdge = faceEdge->next;
    
    //Enquanto não retornar à mesma aresta, pega a próxima aresta
    while(iteratorEdge!= faceEdge){
        vertexList.push_front(iteratorEdge->getOrigin());
        iteratorEdge = iteratorEdge->next;
    }
    
    return(vertexList);
}


list<DCEdge *> DSFace::getFaceEdges() {
    list<DCEdge*> edgeList;
    //Coloca a primeira aresta na lista
    DCEdge* iteratorEdge = faceEdge;
    edgeList.push_front(faceEdge);
    iteratorEdge = faceEdge->next;
    //Enquanto não retornar à mesma aresta, pega a próxima aresta
    while(iteratorEdge!= faceEdge){
        edgeList.push_front(iteratorEdge);
        iteratorEdge = iteratorEdge->next;
    }
    
    return(edgeList);
}


DCEdge *VertexDS::getOPrev() {
    if(vertexEdge != nullptr){
        vertexEdge = vertexEdge->twin->prev;
    }
    return(vertexEdge);
}


DCEdge *VertexDS::getONext() {
    if(vertexEdge != nullptr){
        vertexEdge = vertexEdge->prev->twin;
    }
    return(vertexEdge);
}

VertexDS::VertexDS(double x, double y, double z) {
    vertexEdge = nullptr;
    vertex.assignCoord(x, y, z);
    color.assignColor(STANDARD_R, STANDARD_G, STANDARD_B);
}

VertexDS::VertexDS(double x, double y, double z, double r, double g, double b) {
    vertexEdge = nullptr;
    vertex.assignCoord(x, y, z);
    color.assignColor(r, g, b);
}

VertexDS::VertexDS(Vertex v, Color c) {
    vertexEdge = nullptr;
    vertex.assignCoord(v.x, v.y, v.z);
    color.assignColor(c.r, c.g, c.b);
}


int DCEList::addNewConnection(VertexDS* vertex1,VertexDS*  vertex2, Color* c){
    //Create edge origin and target positions
    DCEL[adjIndex].createConnection(vertex1, vertex2);
    adjIndex++;
    //Updates edges
    return(0);
}

DCEdge *DCEList::getDCEdge(int index) { 
    return(&(DCEL[index]));
}

Color *DCEList::getConnectionColor(int index) { 
    return (&(DCEL[index].connectionColor));
}
int DCEdge::copyContents(DCEdge e1){
    target = e1.target;
    prev = e1.prev;
    next = e1.next;
    twin = e1.twin;
    face = e1.face;
    
    connectionColor = e1.connectionColor;
    return(0);
}

int DCEList::removeConnection(DCEdge* edge) {
    VertexDS* origin = edge->getOrigin();
    VertexDS* target = edge->target;
    DCEdge* twin = edge->twin;
    
    DSFace* newFace = new DSFace();
    newFace->faceEdge = edge->next;
    //DisconnectOrbit
    target->vertexEdge = edge->next;
    origin->vertexEdge = twin->next;
    
    edge->next->prev = twin->prev;
    twin->prev->next = edge->next;
    twin->next->prev = edge->prev;
    edge->prev->next = twin->next;
    
    return(0);
}


int DCELElementsList::addVertex(VertexDS* p){
    if(nVertices >= MAX_NUM_POINTS)
        return(1);
    memoryVertices[nVertices] = p;
    nVertices++;
    upToDate = false;
    return(0);
}
int DCELElementsList::addVertexConnection(VertexDS* p1, VertexDS* p2, Color* c){
    if(adjList.getNumberOfConnections() >= MAX_NUM_POINTS*3)
        return(1);
    adjList.addNewConnection(p1, p2, c);
    upToDate = false;
    return(0);
}

int DCELElementsList::updateLists(GLuint *VBO, GLuint *VAO, GLuint *CBO){
    if(!upToDate){
        //Limpa todos os pontos
        for(int i = 0; i < (nVertices + adjList.getNumberOfConnections()); i++){
            vertices[i].assignCoord(-1, -1, -1);
            colors[i].assignColor(255, 200, 0);
        }
        //Desenha todos os pontos
        for(int i = 0; i < nVertices; i++){
            vertices[i].assignCoord((memoryVertices[i]->vertex).x, (memoryVertices[i]->vertex).y, (memoryVertices[i]->vertex).z);
            colors[i].assignColor((memoryVertices[i]->color).r, (memoryVertices[i]->color).g, (memoryVertices[i]->color).b);
        }
        //Desenha todas as arestas
        for(int i = 0; i < adjList.getNumberOfConnections(); i++){  
            vertices[i*2+nVertices].assignCoord((adjList.getDCEdge(i)->getOrigin()->vertex).x,(adjList.getDCEdge(i)->getOrigin()->vertex).y,(adjList.getDCEdge(i)->getOrigin()->vertex).z);
            colors[i*2+nVertices].assignColor((adjList.getDCEdge(i)->connectionColor).r, (adjList.getDCEdge(i)->connectionColor).g, (adjList.getDCEdge(i)->connectionColor).b);
            vertices[i*2+nVertices+1].assignCoord((adjList.getDCEdge(i)->target->vertex).x,(adjList.getDCEdge(i)->target->vertex).y,(adjList.getDCEdge(i)->target->vertex).z);
            colors[i*2+nVertices + 1].assignColor((adjList.getDCEdge(i)->connectionColor).r, (adjList.getDCEdge(i)->connectionColor).g, (adjList.getDCEdge(i)->connectionColor).b);
        }
        glBindBuffer( GL_ARRAY_BUFFER, *VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, *CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    }
    return(0);
}

Vertex *DCELElementsList::getVerticesList() {
    return(vertices);
}

Color *DCELElementsList::getColorsList() {
    return(colors);
}

int DCELElementsList::getNVertices() {
    return(nVertices);
}

int DCELElementsList::getNConnections() {
    return(adjList.getNumberOfConnections()*2);
}

bool DCELElementsList::isUpToDate() {
    return(upToDate);
}

DCEList* DCELElementsList::getAdjList(){
    return(&adjList);
}
int DCELElementsList::removeVertex(VertexDS* p){
    for(int i = 0; i < nVertices; i++){
        if(memoryVertices[i] == p){
            for(int j = i+1; j < nVertices; j++)
                memoryVertices[j-1] = memoryVertices[j];
        }
    }
    return(0);
}
int DCELElementsList::removeConnection(DCEdge *edge){
    adjList.removeConnection(edge);
    return(0);
}

VertexDS** DCELElementsList::getMemoryVerticesList() {
    return(memoryVertices);
}



DCEdge::DCEdge(){
    //Creates a twin and base conneection
    twin = new DCEdge(this);
    target = nullptr;
    
    connectionColor.assignColor(STANDARD_R, STANDARD_G, STANDARD_B);
    
    prev = twin;
    next = twin;
    
}

DCEdge::DCEdge(DCEdge* newTwin){
    //Creates a twin and base conneection
    this->twin = newTwin;
    target = nullptr;
    
    connectionColor.assignColor(STANDARD_R, STANDARD_G, STANDARD_B);
    
    prev = twin;
    next = twin;
}

DCEdge* findConnectingPreviousEdge(VertexDS* originVertex, DCEdge* newEdge){
    //Se o vértice não possuir uma aresta, retorna nulo
    if(originVertex->vertexEdge == nullptr)
        return(nullptr);
    
    //Senão, ele possui mais do que uma aresta, então testa todas elas para ver qual
    // par comporta a aresta newEdge
    DCEdge* firstEdge, *eCurrent, *ePlus;
    eCurrent = originVertex->vertexEdge;
    firstEdge = eCurrent;
    ePlus = originVertex->getONext();
    double apertureAngle = get360angle(eCurrent->target->vertex, eCurrent->getOrigin()->vertex, ePlus->target->vertex);
    double edgeAngle = get360angle(eCurrent->target->vertex, eCurrent->getOrigin()->vertex, newEdge->target->vertex);
    if(edgeAngle < apertureAngle)
        return(eCurrent->twin);
    eCurrent = ePlus;
    ePlus = originVertex->getONext();
    while(eCurrent!= firstEdge){
        apertureAngle = get360angle(eCurrent->target->vertex, eCurrent->getOrigin()->vertex, ePlus->target->vertex);
        edgeAngle = get360angle(eCurrent->target->vertex, eCurrent->getOrigin()->vertex, newEdge->target->vertex);
        if(edgeAngle < apertureAngle)
            return(eCurrent->twin);
        eCurrent = ePlus;
        ePlus = originVertex->getONext();
    }
    cout << "Error" << endl;
    return(nullptr);
}


void printOT(DCEdge* edge){
    cout << "   Origem: (" << edge->getOrigin()->vertex.x << ", "<< edge->getOrigin()->vertex.y << ")" << endl;
    cout << "   Target: (" << edge->target->vertex.x << ", "<< edge->target->vertex.y << ")" << endl;
}

void printVertex(VertexDS* vertex){
    //printa origin e target de vertexEdge
    cout << "Vértice: (" << vertex->vertex.x << ", "<< vertex->vertex.y << ", "<< vertex->vertex.z << ")" << endl;
    DCEdge* e = vertex->getONext();
    DCEdge* firstEdge = e;
    if(e!=nullptr){
        printOT(e);
        e = vertex->getONext();
        while(e != firstEdge){
            printOT(e);
            e = vertex->getONext();
        }
    }
}

void printEdge(DCEdge* edge){
    //printa origin e target
    cout << "Aresta corrente: "<< endl;
    printOT(edge);
    //printa origin e target de twin
    cout << "Aresta twin: "<< endl;
    printOT(edge->twin);
    //printa origin e target de prev
    cout << "Aresta next: "<< endl;
    printOT(edge->next);
    //printa origin e target de next
    cout << "Aresta prev: "<< endl;
    printOT(edge->prev);
    cout << endl << endl;
}

int DCEdge::createConnection(VertexDS *vOrigin, VertexDS *vTarget) {
    target = vTarget;
    twin->target = vOrigin;
    DSFace* new_face1 = new DSFace();
    DSFace* new_face2 = new DSFace();
    
    // Chooses the edge to be used with e
    DCEdge* chosenEdge = findConnectingPreviousEdge(vOrigin, this);
    DCEdge* chosenEdgeTwin = findConnectingPreviousEdge(vTarget, this->twin);
    if(chosenEdge!= nullptr){
        cout <<"Atualizou aresta corrente" << endl;
        updateDCEdge(chosenEdge,this);
    }
    if(chosenEdgeTwin != nullptr){
        cout <<"Atualizou aresta gemea" << endl;
        updateDCEdge(chosenEdgeTwin, this->twin);
    }
    
    
    vTarget->vertexEdge = this->twin;
    vOrigin->vertexEdge = this;
    
    vTarget->vertexEdge->prev->connectionColor.assignColor(0.9, 0.1, 0.9);
    
    //Deleta as faces que nao sao mais necessarias
    delete(vOrigin->vertexEdge->face);
    
    //Coloca as faces nas arestas que sao da nova face
    DCEdge* iterator = vOrigin->vertexEdge;
    DCEdge* firstEdge = vOrigin->vertexEdge;
    iterator->face = new_face1;
    while(iterator!=firstEdge){
        iterator->face = new_face1;
        iterator = iterator->next;
    }
    new_face1->faceEdge = firstEdge;
    iterator = vTarget->vertexEdge;
    firstEdge = vTarget->vertexEdge;
    iterator->face = new_face2;
    while(iterator!=firstEdge){
        iterator->face = new_face2;
        iterator = iterator->next;
    }
    new_face2->faceEdge = firstEdge;
    
    return(0);
}
