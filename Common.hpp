//
//  Apps.hpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//
class Vertex;
class Vector;

#ifndef Common_hpp
#define Common_hpp

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
class DCELElementsList;
class VertexDS;

// GLFW
#include <GLFW/glfw3.h>
#include "ElementalStructures.hpp"
#include "CompositeStructures.hpp"
#include "DCELStructures.hpp"
#include <list>
#include <iterator>
#include <math.h>       /* atan */
using namespace std;
int modulo(int x,int N);
bool left(Vertex p1, Vertex p2, Vertex p3);
double triangleArea(Vertex p1, Vertex p2, Vertex p3);
bool leftOn(Vertex p1, Vertex p2, Vertex p3);
bool isCollinear(Vertex p1, Vertex p2, Vertex p3);
double getAngleBetweenEdges(Vertex p1, Vertex p2, Vertex p3);
Vertex getIntersectingPoint(Vertex p1, Vertex p2, Vertex p3, Vertex p4);
bool isUnder(Vertex pp, Vertex pq);
void createNRandomPoints(int numberOfPoints, DCELElementsList* elements, VertexDS** emptyVector);
double get360angle(Vertex p1, Vertex p2, Vertex p3);
list<int> computesConvexHull(VertexDS** vertexList, DCELElementsList* elements, int nPoints);
void computesOnion(VertexDS** vertexList, DCELElementsList* elements, int nPoints);
void computesOnionLayer(VertexDS** vertexList, DCELElementsList* elements, int* nPoints);
#endif /* Apps_hpp */



