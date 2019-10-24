//
//  appTriangles.hpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#ifndef appTriangles_h
#define appTriangles_h
#include "DataStructures.hpp"
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#define MAX_NUM_POINTS 2048

// GLFW
#include <GLFW/glfw3.h>
#include "Common.hpp"
#include "shader.h"
int runAppTriangles();


#endif /* appTriangles_h */
