//
//  appPoints.cpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "appHull.hpp"

//Color Palette e63946-f1faee-a8dadc-457b9d-1d3557

GLuint VBO, VAO, CBO;
// Window dimensions
const GLuint WIDTH = 1200,HEIGHT =  800;
DCEdge* cursor = nullptr;

// Set up vertex data (and buffer(s)) and attribute pointers
DCELElementsList elements;
Color outra(STANDARD_R,STANDARD_G,STANDARD_B);
Color corDaVitoria(0.0,1.0,1.0);

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos){
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        std::cout << "NEXT"<< std::endl;
        
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        std::cout << "NEXT"<< std::endl;
    }
}


int runAppHull()
{
    GLFWwindow *window;
    int screenWidth, screenHeight;
    
    // Init GLFW
    glfwInit( );
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow( WIDTH, HEIGHT, "CSV50", nullptr, nullptr );
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return(1);
    }
    
    glfwMakeContextCurrent( window );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return(1);
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    
    GLuint program = LoadShaders("/Users/akira/Documents/teste-opengl/teste-opengl/Shaders/simple.vert", "/Users/akira/Documents/teste-opengl/teste-opengl/Shaders/simple.frag");
    
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( elements.getVerticesList() ), elements.getVerticesList(), GL_STATIC_DRAW );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    
    /* Create color buffer object (CBO). */
    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof( elements.getColorsList() ), elements.getColorsList(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    int nRandomPoints = 30;
    VertexDS* randomPoints[nRandomPoints];
    createNRandomPoints(nRandomPoints, &elements, randomPoints);
    
    for(int i = 0; i < nRandomPoints; i++)
        elements.addVertex(randomPoints[i]);
    computesOnion(randomPoints, &elements, nRandomPoints);
    
    //cursor = %TODO;
    
    glPointSize(15);
    
    
    //poly.triangulate(elements.getAdjList());
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    
    while ( !glfwWindowShouldClose( window ) )
    {
        // Render
        // Clear the colorbuffer
        glClearColor( 241.0/255.0, 250.0/255.0, 238.0/255.0, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        elements.updateLists(&VBO, &VAO, &CBO);
        
        // Draw our first triangle
        glUseProgram( program );
        glBindVertexArray( VAO );
        //glDrawArrays( GL_TRIANGLES, 0, nPoints);
        glDrawArrays( GL_POINTS, 0, elements.getNVertices());
        glDrawArrays( GL_LINES, elements.getNVertices(), elements.getNConnections());
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
    }
    for (int i = 0; i < elements.getAdjList()->getNumberOfConnections(); i++)
        printEdge(elements.getAdjList()->getDCEdge(i));
//    for (int i = 0; i < elements.getNVertices(); i++)
//        printVertex(elements.getMemoryVerticesList()[i]);
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &CBO );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}
