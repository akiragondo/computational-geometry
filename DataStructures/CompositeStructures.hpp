//
//  CompositeStructures.hpp
//  AppsGC
//
//  Created by Gustavo Akira Gondo on 03/09/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

class ElementsList;
class AdjacencyList;
class DCEdge;
#ifndef CompositeStructures_hpp
#define CompositeStructures_hpp

#include "DataStructures.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
#include "Common.hpp"
#include <math.h>       /* atan */
#include <cmath>       /* atan */
class Vector {
    Vertex p1,p2;
    char str[50];
public:
    void setNewVertices(Vertex new_p1, Vertex new_p2){
        p1 = new_p1;
        p2 = new_p2;
        normalizeVertices();
    }
    char* getVectorDescriptor(){
        strcpy(str, "(");
        strcat(str, std::to_string(p2.x).c_str());
        strcat(str, ", ");
        strcat(str, std::to_string(p2.y).c_str());
        strcat(str, ", ");
        strcat(str, std::to_string(p2.z).c_str());
        strcat(str, ")");
        return(str);
    }
    
    double getScalarProduct(Vector v){
        double scalarProduct = 0;
        scalarProduct += p2.x * v.p2.x;
        scalarProduct += p2.y * v.p2.y;
        scalarProduct += p2.z * v.p2.z;
        return(scalarProduct);
    }
    double getModulus(){
        return(sqrt(pow(p2.x,2) + pow(p2.y,2) + pow(p2.z,2)));
    }
    
    Vector getVectorProduct(Vector v){
        Vertex product;
        product.x = (p2.y*v.p2.z) - (p2.z*v.p2.y);
        product.y = (p2.x*v.p2.z) - (p2.z*v.p2.x);
        product.z = (p2.x*v.p2.y) - (p2.y*v.p2.x);
        Vector result(Vertex(0.0,0.0,0.0), product);
        return(result);
    }
    double getAngle(Vector v){
        double dotResult = getScalarProduct(v);
        double angle = acos (dotResult/(getModulus()*v.getModulus())) * 180 / PI;
        return(angle);
    }
    void normalizeVertices(){
        double xOffset = p1.x;
        double yOffset = p1.y;
        p2.x = p2.x - xOffset;
        p2.y = p2.y - yOffset;
    }
    Vector(Vertex new_p1, Vertex new_p2){
        p1 = new_p1;
        p2 = new_p2;
        normalizeVertices();
    }
    Vector(){
        p1.x = -1;
        p1.y = -1;
        p1.z = -1;
        p2.x = -1;
        p2.y = -1;
        p2.z = -1;
    }
    Vertex getP1(){return(p1);}
    Vertex getP2(){return(p2);}
};

class Polygon{
private:
    Vertex* verticesList[MAX_POLYGON_SIZE];
    Color* colorList[MAX_POLYGON_SIZE];
    int polygonSize = 0;
public:
    bool isInsidePolygon();
    bool isVertexEar(int index);
    int getPolygonSize();
    int addVertex(Vertex* vertex);
    int addVertex(Vertex* vertex, Color* color);
    Vertex* getVertex(int index);
    Color* getColor(int index);
    double getPolygonArea();
    int paintConvexPoints(bool isCounterClockwise);
    int flagEarVertices(bool paint);
    int classifySplitMerge();
    int triangulate(AdjacencyList* adjList);
    int triNewPoint(ElementsList* elements);
    int removeVertex(int index);
};

#endif /* CompositeStructures_hpp */
