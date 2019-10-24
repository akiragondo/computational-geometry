//
//  Apps.cpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//


#include "Common.hpp"


double triangleArea(Vertex p1, Vertex p2, Vertex p3){
    //Area = 1/2((x2-x1)(y3-y1)-(x3-x1)(y2-y1))
    double area = -0.5*((p2.x - p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y));
    return(area);
}
bool left(Vertex p1, Vertex p2, Vertex p3){
    //Area = 1/2((x2-x1)(y3-y1)-(x3-x1)(y2-y1))
    double area = triangleArea(p1, p2, p3);
    if(area > 0){
        return(true);
    }
    else{
        return(false);
    }
}

bool leftOn(Vertex p1, Vertex p2, Vertex p3){
    //Area = 1/2((x2-x1)(y3-y1)-(x3-x1)(y2-y1))
    double area = triangleArea(p1, p2, p3);
    if(area >= 0){
        return(true);
    }
    else{
        return(false);
    }
}
bool isCollinear(Vertex p1, Vertex p2, Vertex p3){
    double area = triangleArea(p1, p2, p3);
    if(area == 0){
        return(true);
    }
    else
        return(false);
}
    
    
Vertex getIntersectingPoint(Vertex p1, Vertex p2, Vertex p3, Vertex p4){
    //Vetor 1 -p0, p1
    //Vetor 2 -p0, p1
    //Calcula valor de alpha
    double alpha = 0, beta = 0;
    alpha = ((p4.x - p3.x)*(p3.y-p1.y) - (p3.x - p1.x)*(p4.y - p3.y));
    alpha /= ((p4.x - p3.x)*(p2.y-p1.y) - (p2.x - p1.x)*(p4.y - p3.y));
    //Calcula valor de beta
    beta = ((p2.x - p1.x)*(p3.y-p1.y) - (p3.x - p1.x)*(p2.y - p1.y));
    beta /= ((p4.x - p3.x)*(p2.y-p1.y) - (p2.x - p1.x)*(p4.y - p3.y));
    // x = x1 + alpha*(x2-x1)
    // y = y1 + alpha*(y2-y1)
    
    double x = p1.x + alpha*(p2.x-p1.x);
    double y = p1.y + alpha*(p2.y-p1.y);
    
    if(alpha >= 1 || alpha <= 0 || beta >= 1 || beta <= 0){
        x = -2;
        y = -2;
    }
    Vertex P(x,y,0.0);
    
    return(P);
}


double get360angle(Vertex p1, Vertex p2, Vertex p3){
    //P2 as center point
    Vector v1(p2,p1);
    Vector v2(p2,p3);
    double dot = v1.getP2().x*v2.getP2().x + v1.getP2().y*v2.getP2().y;
    double det = v1.getP2().x*v2.getP2().y - v1.getP2().y*v2.getP2().x;
    double angle = atan2(det, dot)*180/PI;
    if(isnan(angle))
        angle = 360;
    if(angle < 0)
        angle = 360 + angle;
    angle = 360 - angle;
    return(angle);
}


double getAngleBetweenEdges(Vertex p1, Vertex p2, Vertex p3){
    //Area = 1/2((x2-x1)(y3-y1)-(x3-x1)(y2-y1))
    double area = triangleArea(p1, p2, p3);
    //P2 as center point
    Vector v1(p2,p1);
    Vector v2(p2,p3);
    double angle = v1.getAngle(v2);
    if(area >=  0)
        angle = 360-angle;
    if(isnan(angle))
        angle = 360;
    return(angle);
}


double RandomFloat(double min, double max)
{
    // this  function assumes max > min, you may want
    // more robust error checking for a non-debug build
    assert(max > min);
    double random = ((double) rand()) / (double) RAND_MAX;
    
    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    double range = max - min;
    return (random*range) + min;
}

void createNRandomPoints(int numberOfPoints, DCELElementsList* elements, VertexDS** emptyVector){
    double newX, newY;
    VertexDS* newVertex;
    for (int i = 0 ; i<numberOfPoints; i++){
        newX = RandomFloat(-0.8, 0.8);
        newY = RandomFloat(-0.8, 0.8);
        newVertex = new VertexDS(newX, newY, 0.0);
        emptyVector[i] = newVertex;
    }
}



bool isUnder(Vertex pp, Vertex pq){
    if(pp.y < pq.y)
        return(true);
    if(pp.y == pq.y && pp.x > pq.x)
        return(true);
    return(false);
}


list<int> computesConvexHull(VertexDS** vertexList, DCELElementsList* elements, int nPoints){
    //Takes the extreme vertex
    VertexDS* minimumPoint = vertexList[0];
    list<int> connectedPoints;
    double minimumIndex = -1;
    for(int i = 1; i < nPoints; i++){
        if(isUnder(vertexList[i]->vertex,minimumPoint->vertex)){
            minimumPoint = vertexList[i];
            minimumIndex = i;
        }
    }
    connectedPoints.push_front(minimumIndex);
    minimumPoint->color.assignColor(1.0, 0, 0);
    
    Vertex firstVertexAuxiliary(1.0, minimumPoint->vertex.y, 0);
    
    VertexDS* currentVertex = nullptr;
    VertexDS* chosenVertex = nullptr;
    VertexDS* lastChosen = nullptr;
    VertexDS* lastChosenMinus = nullptr;
    int pointsConnected = 0;
    
    Color color(STANDARD_R, STANDARD_G, STANDARD_B);
    double minimumAngle = 123123;
    double angleBetweenVertices;
    for(int i = 0; i < nPoints; i++){
        currentVertex = vertexList[i];
        if(currentVertex != minimumPoint){
            angleBetweenVertices = getAngleBetweenEdges(currentVertex->vertex, minimumPoint->vertex, firstVertexAuxiliary);
            if(angleBetweenVertices < minimumAngle){
                chosenVertex = currentVertex;
                minimumIndex = i;
                minimumAngle = angleBetweenVertices;
            }
        }
    }
    connectedPoints.push_front(minimumIndex);
    chosenVertex->color.assignColor(0, 0, 1.0);
    elements->addVertexConnection(minimumPoint, chosenVertex, &(color));
    cout << connectedPoints.size() << endl;
    lastChosenMinus = minimumPoint;
    lastChosen = chosenVertex;
    pointsConnected++;
    while(pointsConnected < nPoints+1 and lastChosen!=minimumPoint){
        currentVertex = nullptr;
        chosenVertex = nullptr;
        minimumIndex = -1;
        minimumAngle = 123123;
        //Computes the angle between Ve + Vx and an edge to every other vertex
        for(int i = 0; i < nPoints; i++){
            currentVertex = vertexList[i];
            if(currentVertex != lastChosen && currentVertex != lastChosenMinus){
                angleBetweenVertices = getAngleBetweenEdges(currentVertex->vertex, lastChosen->vertex, lastChosenMinus->vertex);
                if(angleBetweenVertices < minimumAngle){
                    minimumIndex = i;
                    chosenVertex = currentVertex;
                    minimumAngle = angleBetweenVertices;
                }
            }
        }
        elements->addVertexConnection(lastChosen, chosenVertex, &(color));
        if(chosenVertex!=minimumPoint)
            connectedPoints.push_front(minimumIndex);
        lastChosenMinus = lastChosen;
        lastChosen = chosenVertex;
        pointsConnected ++;
        cout << connectedPoints.size() << endl;
        //takes the vertex with minimum angle
        
        // if the minimum angle is repeated, takes the smallest edge
        //Continues until the first vertex chosen is chosen to be the next
    }
    return(connectedPoints);
}


int modulo(int x,int N){
    return (x % N + N) %N;
}



void removeFromVertexList(VertexDS** vertexList, int index, int* nPoints){
    for (int j = index; j < *nPoints; j++){
        vertexList[j] = vertexList[j+1];
    }
    *nPoints-=1;
}


void computesOnionLayer(VertexDS** vertexList, DCELElementsList* elements, int* nPoints){
    //Takes the extreme vertex
    VertexDS* minimumPoint = vertexList[0];
    double minimumIndex = -1;
    for(int i = 1; i < *nPoints; i++){
        if(isUnder(vertexList[i]->vertex,minimumPoint->vertex)){
            minimumPoint = vertexList[i];
            minimumIndex = i;
        }
    }
    
    minimumPoint->color.assignColor(1.0, 0, 0);
    
    Vertex firstVertexAuxiliary(1.0, minimumPoint->vertex.y, 0);
    
    VertexDS* currentVertex = nullptr;
    VertexDS* chosenVertex = nullptr;
    VertexDS* lastChosen = nullptr;
    VertexDS* lastChosenMinus = nullptr;
    int pointsConnected = 0;
    
    Color color(STANDARD_R, STANDARD_G, STANDARD_B);
    double minimumAngle = 123123;
    double angleBetweenVertices;
    for(int i = 0; i < *nPoints; i++){
        currentVertex = vertexList[i];
        if(currentVertex != minimumPoint){
            angleBetweenVertices = getAngleBetweenEdges(currentVertex->vertex, minimumPoint->vertex, firstVertexAuxiliary);
            if(angleBetweenVertices < minimumAngle){
                chosenVertex = currentVertex;
                minimumIndex = i;
                minimumAngle = angleBetweenVertices;
            }
        }
    }
    
    chosenVertex->color.assignColor(0, 0, 1.0);
    removeFromVertexList(vertexList, minimumIndex, nPoints);
    elements->addVertexConnection(minimumPoint, chosenVertex, &(color));
    lastChosenMinus = minimumPoint;
    lastChosen = chosenVertex;
    pointsConnected++;
    while(lastChosen!=minimumPoint && *nPoints > 0){
        currentVertex = nullptr;
        chosenVertex = nullptr;
        minimumIndex = -1;
        minimumAngle = 123123;
        //Computes the angle between Ve + Vx and an edge to every other vertex
        for(int i = 0; i < *nPoints; i++){
            currentVertex = vertexList[i];
            if(currentVertex != lastChosen && currentVertex != lastChosenMinus){
                angleBetweenVertices = getAngleBetweenEdges(currentVertex->vertex, lastChosen->vertex, lastChosenMinus->vertex);
                if(angleBetweenVertices < minimumAngle){
                    minimumIndex = i;
                    chosenVertex = currentVertex;
                    minimumAngle = angleBetweenVertices;
                }
            }
        }
        elements->addVertexConnection(lastChosen, chosenVertex, &(color));
        removeFromVertexList(vertexList, minimumIndex, nPoints);
        lastChosenMinus = lastChosen;
        lastChosen = chosenVertex;
    }
}


void computesOnion(VertexDS** vertexList, DCELElementsList* elements, int nPoints){
    //Takes the extreme vertex
    while(nPoints > 1)
        computesOnionLayer(vertexList, elements, &nPoints);
}
