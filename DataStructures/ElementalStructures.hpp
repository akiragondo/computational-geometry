//
//  ElementalStructures.hpp
//  AppsGC
//
//  Created by Gustavo Akira Gondo on 03/09/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#ifndef ElementalStructures_hpp
#define ElementalStructures_hpp

// GLEW
#define GLEW_STATIC
#include "Constants.h"
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <stdio.h>


class Vertex{
public:
    GLfloat x,y,z;
    unsigned char flags;
    Vertex(GLfloat newX, GLfloat newY, GLfloat newZ){
        x = newX;
        y = newY;
        z = newZ;
        flags = 0;
    }
    Vertex(){
        x = -1;
        y = -1;
        z = -1;
        flags = 0;
    }
    void assignCoord(GLfloat newX, GLfloat newY, GLfloat newZ){
        x = newX;
        y = newY;
        z = newZ;
    }
};

class Color{
public:
    GLfloat r,g,b;
    Color(GLfloat newR, GLfloat newG, GLfloat newB){
        r = newR;
        g = newG;
        b = newB;
    }
    Color(){
        r = -1;
        g = -1;
        b = -1;
    }
    void assignColor(GLfloat newR, GLfloat newG, GLfloat newB){
        r = newR;
        g = newG;
        b = newB;
    }
};


#endif /* ElementalStructures_hpp */
