//
//  CompositeStructures.cpp
//  AppsGC
//
//  Created by Gustavo Akira Gondo on 03/09/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "CompositeStructures.hpp"

bool Polygon::isInsidePolygon() {
    return(false);
}

int Polygon::getPolygonSize() {
    return(polygonSize);
}

int Polygon::addVertex(Vertex* vertex) {
    verticesList[polygonSize] = vertex;
    Color* color = new Color(0.5,0.1,0.5);
    colorList[polygonSize] = color;
    polygonSize++;
    return(0);
}

int Polygon::addVertex(Vertex* v, Color* color) {
    verticesList[polygonSize] = v;
    colorList[polygonSize] = color;
    polygonSize++;
    return(0);
}

double Polygon::getPolygonArea(){
    Vertex* anchor = verticesList[0];
    double totalArea = 0;
    for(int i = 1; i < polygonSize - 1; i++){
        totalArea -= triangleArea(*anchor, *verticesList[i], *verticesList[i+1]);
    }
    return(totalArea);
}

Vertex* Polygon::getVertex(int index) {
    return(verticesList[index]);
}
Color* Polygon::getColor(int index) {
    return(colorList[index]);
}


int Polygon::paintConvexPoints(bool isCounterClockwise) {
    Vertex p1,p2,p3;
    double angle;
    for(int i = 0 ; i < polygonSize; i++){
        p1 = *verticesList[modulo(i-1, polygonSize)];
        p2 = *verticesList[(i)];
        p3 = *verticesList[(i+1)%(polygonSize)];
        angle = getAngleBetweenEdges(p1,p2,p3);
        if(isCounterClockwise)
            angle = 360 - angle;
        if(angle > 180)
            colorList[i]->assignColor(0.9, 0.1, 0.9);
        else
            colorList[i]->assignColor(0.1, 0.7, 0.1);
        
    }
    return(0);
}

int Polygon::flagEarVertices(bool paint){
    for (int i =0; i < polygonSize; i++){
        if(isVertexEar(i)){
            verticesList[i]->flags = verticesList[i]->flags | ORELHA_MASK;
            colorList[i]->assignColor(0.7, 0.1, 0.7);
        }
        else{
            verticesList[i]->flags = verticesList[i]->flags & ~ ORELHA_MASK;
            colorList[i]->assignColor(0.1, 0.1, 0.7);
        }
    }
    return(0);
}
int Polygon::removeVertex(int index){
    if(index > polygonSize)
        return(1);
    verticesList[index] = nullptr;
    for(int i = index; i < polygonSize; i++)
        verticesList[i] = verticesList[i+1];
    polygonSize--;
    return(0);
}


bool Polygon::isVertexEar(int index){
    Vertex* pMinus = (verticesList[modulo(index - 1, polygonSize)]);
    Vertex* pPlus = (verticesList[modulo(index + 1, polygonSize)]);
    Vertex* p0 = (verticesList[modulo(index, polygonSize)]);
    Vertex* iteratorVertex0;
    Vertex* iteratorVertex1;
    Vertex intersection;
    if(getAngleBetweenEdges(*(pMinus), *(p0), *(pPlus)) > 180){
        return(false);
    }
    for(int i = 0; i< polygonSize; i++){
        iteratorVertex0 = verticesList[i];
        iteratorVertex1 = verticesList[modulo(i + 1, polygonSize)];
        
        intersection = getIntersectingPoint(*pMinus, *pPlus, *iteratorVertex0, *iteratorVertex1);
        //Se tiver interseccao, retorna falso
        if(isnan(intersection.x) || isnan(intersection.y))
            continue;
        if((intersection.x != -2 || intersection.y != -2 )){
            return(false);
        }
    }
    //Check if edge is inside the polygon through vector pMinus
    //pMinus - *((verticesList[modulo(index - 1, polygonSize)]))
    double insideBoundaryAngle = getAngleBetweenEdges(*((verticesList[modulo(index - 2, polygonSize)])), *((verticesList[modulo(index - 1, polygonSize)])), *((verticesList[modulo(index, polygonSize)])));
    double vectorAngle = getAngleBetweenEdges(*((verticesList[modulo(index + 1, polygonSize)])), *((verticesList[modulo(index - 1, polygonSize)])), *((verticesList[modulo(index, polygonSize)])));
    if(insideBoundaryAngle < vectorAngle)
        return(false);
    return(true);
}

int Polygon::classifySplitMerge(){
    Vertex* pMinus;
    Vertex* pPlus;
    Vertex* p0;
    double angle = -123123;
    for(int i = 0; i < polygonSize; i++){
        pMinus = (verticesList[modulo(i - 1, polygonSize)]);
        pPlus = (verticesList[modulo(i + 1, polygonSize)]);
        p0 = (verticesList[modulo(i, polygonSize)]);
        //calcula angulo
        angle =360 - getAngleBetweenEdges(*(pMinus), *(p0), *(pPlus));
        //Checa se start
        if(angle < 180 && isUnder(*pMinus, *p0) &&isUnder(*pPlus, *p0)){
            colorList[i]->assignColor(0.7, 0.2, 0.2);
            verticesList[i]->flags = verticesList[i]->flags & ~ RESET_SPLIT;
            verticesList[i]->flags = verticesList[i]->flags | START_MASK;
        }
        //Checa se split
        else if(angle > 180 && isUnder(*pMinus, *p0) &&isUnder(*pPlus, *p0)){
            colorList[i]->assignColor(0.2, 0.7, 0.2);
            verticesList[i]->flags = verticesList[i]->flags & ~ RESET_SPLIT;
            verticesList[i]->flags = verticesList[i]->flags | SPLIT_MASK;
        }
        //Checa se end
        else if(angle < 180 && not isUnder(*pMinus, *p0) && not isUnder(*pPlus, *p0)){
            colorList[i]->assignColor(0.2, 0.2, 0.7);
            verticesList[i]->flags = verticesList[i]->flags & ~ RESET_SPLIT;
            verticesList[i]->flags = verticesList[i]->flags | END_MASK;
        }
        //Checa se merge
        else if(angle > 180 && not isUnder(*pMinus, *p0) && not isUnder(*pPlus, *p0)){
            colorList[i]->assignColor(0.7, 0.7, 0.0);
            verticesList[i]->flags = verticesList[i]->flags & ~ RESET_SPLIT;
            verticesList[i]->flags = verticesList[i]->flags | MERGE_MASK;
        }
        //Regular
        else{
            colorList[i]->assignColor(0.0, 1.0,1.0);
            verticesList[i]->flags = verticesList[i]->flags & ~ RESET_SPLIT;
            verticesList[i]->flags = verticesList[i]->flags | REGULAR_MASK;
        }
    }
    return(0);
}

int Polygon::triangulate(AdjacencyList* adjList){
    //Faz uma cópia do polígono
    Polygon copy = *this;
    copy.flagEarVertices(true);
    //Opera sobre o vetor
    while(copy.polygonSize >= 4){
        copy.flagEarVertices(true);
        for(int i = 0; i < copy.polygonSize; i++){
            if(copy.verticesList[i]->flags & ORELHA_MASK){
                std::cout << "Coordenada da orelha "<<(copy.verticesList[i]->x) << "Tamanho "<< copy.polygonSize<< std::endl;
                std::cout << "Coordenada da conexao "<<(copy.verticesList[modulo(i-1, copy.polygonSize)]->x) << copy.verticesList[modulo(i+1, copy.polygonSize)]->x<< std::endl;
                adjList->addNewConnection(copy.verticesList[modulo(i-1, copy.polygonSize)], copy.verticesList[modulo(i+1, copy.polygonSize)], colorList[0]);
                copy.removeVertex(i);
                break;
            }
        }
        copy.flagEarVertices(false);
        std::cout << "Atualiza novas orelhas " << copy.polygonSize << std::endl;
        for(int i = 0; i< copy.polygonSize; i++){
            std::cout <<"Vertice n " << i << ": ("<<copy.verticesList[i]->x << ", " << copy.verticesList[i]->y << ")"<< std::endl;
            std::cout <<"FLAGS n " << i << ": ("<<(int)copy.verticesList[i]->flags<< ")"<< std::endl;
        }
    }
    return(0);
}
int Polygon::triNewPoint(ElementsList* elements){
    //Faz uma cópia do polígono
    flagEarVertices(true);
    if(polygonSize >=4){
        for(int i = 0; i < polygonSize; i++){
            if(verticesList[i]->flags & ORELHA_MASK){
                std::cout << "Coordenada da orelha "<<(verticesList[i]->x) << "Tamanho "<< polygonSize<< std::endl;
                std::cout << "Coordenada da conexao "<<(verticesList[modulo(i-1, polygonSize)]->x) << verticesList[modulo(i+1, polygonSize)]->x<< std::endl;
                elements->getAdjList()->addNewConnection(verticesList[modulo(i-1, polygonSize)], verticesList[modulo(i+1, polygonSize)], colorList[0]);
                elements->removeVertex(verticesList[i]);
                removeVertex(i);
                break;
            }
        }
    }
    flagEarVertices(true);
    for(int i = 0; i< polygonSize; i++){
        std::cout <<"Vertice n " << i << ": ("<<verticesList[i]->x << ", " << verticesList[i]->y << ")"<< std::endl;
        if(verticesList[i]->flags&ORELHA_MASK)
            std::cout << "      Orelha" << std::endl;
        std::cout <<"Angulo n " << i << ": ("<< getAngleBetweenEdges(*(verticesList[modulo(i-1, polygonSize)]), *(verticesList[modulo(i, polygonSize)]), *(verticesList[modulo(i+1, polygonSize)])) << ")"<< std::endl;
    }
    
    return(0);
}




