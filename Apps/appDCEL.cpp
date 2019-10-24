//
//  appPoints.cpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "appDCEL.hpp"

#define HEXAGONO

//Color Palette e63946-f1faee-a8dadc-457b9d-1d3557

GLuint VBO, VAO, CBO;
// Window dimensions
const GLuint WIDTH = 800,HEIGHT =  800;
DCEdge* cursor = nullptr;
bool toggleFlag = true;

// Set up vertex data (and buffer(s)) and attribute pointers
DCELElementsList elements;
Color outra(STANDARD_R,STANDARD_G,STANDARD_B);
Color corDaVitoria(0.0,1.0,1.0);

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos){
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    list<VertexDS *> faceVertices;
    list<DCEdge *> faceEdges;
    DCEdge* iterator = cursor;
    if (key == GLFW_KEY_Z && action == GLFW_PRESS){
        faceVertices = cursor->face->getFaceVertices();
        faceEdges = cursor->face->getFaceEdges();
        for (auto const& i : faceVertices) {
            if(toggleFlag){
                i->color.assignColor(0.0, 1.0, 0 );
            }
            else{
                i->color.assignColor(0.1 ,  0.5   , 0.1 );
            }
            cout << "Mudou a cor"<< endl;
        }
        for (auto const& i : faceEdges) {
            if(toggleFlag){
                i->connectionColor.assignColor(0.0,1.0,0);
                i->twin->connectionColor.assignColor(0.0,1.0,0);
            }
            else{
                i->connectionColor.assignColor(0.1,0.5,0.1);
                i->twin->connectionColor.assignColor(0.1,0.5,0.1);
            }
            cout << "Mudou a cor"<< endl;
        }
        cursor->connectionColor = corDaVitoria;
        toggleFlag = ! toggleFlag;
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( elements.getVerticesList() ), elements.getVerticesList(), GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(elements.getColorsList()), elements.getColorsList(), GL_STATIC_DRAW);
    }
    
    if (key == GLFW_KEY_X && action == GLFW_PRESS){
        //Insere novo vértice no meio da aresta
        DCEdge* cursorNext = cursor->next;
        
        double middleX = (cursor->target->vertex.x + cursor->getOrigin()->vertex.x)/2;
        double middleY = (cursor->target->vertex.y + cursor->getOrigin()->vertex.y)/2;
        VertexDS* target = cursor->target, *origin = cursor->getOrigin();
        VertexDS* newVertex = new VertexDS(middleX, middleY, 0);
        
        elements.removeConnection(cursor);
        elements.addVertex(newVertex);
        elements.addVertexConnection(origin, newVertex, &corDaVitoria);
        elements.addVertexConnection(newVertex, target, &corDaVitoria);
        
        cursor = cursorNext;
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( elements.getVerticesList() ), elements.getVerticesList(), GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(elements.getColorsList()), elements.getColorsList(), GL_STATIC_DRAW);
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        std::cout << "NEXT"<< std::endl;
        
        cursor->connectionColor = outra;
        cursor->twin->connectionColor = outra;
        cursor = cursor->next;
        cursor->connectionColor = corDaVitoria;
        cursor->twin->connectionColor = corDaVitoria;
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( elements.getVerticesList() ), elements.getVerticesList(), GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(elements.getColorsList()), elements.getColorsList(), GL_STATIC_DRAW);
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        std::cout << "NEXT"<< std::endl;
        
        cursor->connectionColor = outra;
        cursor->twin->connectionColor = outra;
        cursor = cursor->getOrigin()->getONext();
        cursor->connectionColor = corDaVitoria;
        cursor->twin->connectionColor = corDaVitoria;
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( elements.getVerticesList() ), elements.getVerticesList(), GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(elements.getColorsList()), elements.getColorsList(), GL_STATIC_DRAW);
    }
}

int runAppDCEL()
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
    
    //Cria Polígono antihorário para teste de triangulação
    Vertex p0(-0.4,0.4,0);
    Vertex p1(-0.6,0.2,0);
    Vertex p2(-0.4,0.0,0);
    Vertex p3(-0.2,0.0,0);
    Vertex p4(0,-0.2,0);
    Vertex p5(0.5,0.1,0);
    Vertex p6(0.2,0.0,0);
    Vertex p7(0.6,0.2,0);
    Vertex p8(0.4,0.4,0);
    Vertex p9(0.4,0.6,0);
    Vertex p10(0.2,0.6,0);
    Vertex p11(0.0,0.5,0);
    Vertex p12(0.2,0.4,0);
    Vertex p13(0.4,0.2,0);
    Vertex p14(0.0,0.0,0);
    Vertex p15(0.1,0.3,0);
    Vertex p16(0,0.4,0);
    Vertex p17(-0.2,0.6,0);
    
    
    Vertex h0(-0.6,0.0,0);
    Vertex h1(-0.3,-0.3,0);
    Vertex h2(0.0,-0.6,0);
    Vertex h3(0.3,-0.3,0);
    Vertex h4(0.6,0.0,0);
    Vertex h5(0.3,0.3,0);
    Vertex h6(0.0,0.6,0);
    Vertex h7(-0.3,0.3,0);
    Color c1(0.5, 0.1, 0.1);
    Color c2(0.1, 0.5, 0.1);
    //Coloca Vértices nos formatos de VertexDS
    VertexDS v0(p0, c1);
    VertexDS v1(p1, c1);
    VertexDS v2(p2, c1);
    VertexDS v3(p3, c1);
    VertexDS v4(p4, c1);
    VertexDS v5(p5, c1);
    VertexDS v6(p6, c1);
    VertexDS v7(p7, c1);
    VertexDS v8(p8, c1);
    VertexDS v9(p9, c1);
    VertexDS v10(p10, c1);
    VertexDS v11(p11, c1);
    VertexDS v12(p12, c1);
    VertexDS v13(p13, c1);
    VertexDS v14(p14, c1);
    VertexDS v15(p15, c1);
    VertexDS v16(p16, c1);
    VertexDS v17(p17, c1);
    
    VertexDS vh0(h0, c1);
    VertexDS vh1(h1, c1);
    VertexDS vh2(h2, c1);
    VertexDS vh3(h3, c1);
    VertexDS vh4(h4, c1);
    VertexDS vh5(h5, c1);
    VertexDS vh6(h6, c1);
    VertexDS vh7(h7, c1);
    
#ifdef HEXAGONO
    elements.addVertex(&vh0);
    elements.addVertex(&vh1);
    elements.addVertex(&vh2);
    elements.addVertex(&vh3);
    elements.addVertex(&vh4);
    elements.addVertex(&vh5);
    elements.addVertex(&vh6);
    elements.addVertex(&vh7);


    elements.addVertexConnection(&vh0, &vh1, &c2);
    elements.addVertexConnection(&vh1, &vh2, &c2);
    elements.addVertexConnection(&vh2, &vh3, &c2);
    elements.addVertexConnection(&vh3, &vh4, &c2);
    elements.addVertexConnection(&vh4, &vh5, &c2);
    elements.addVertexConnection(&vh5, &vh6, &c2);
    elements.addVertexConnection(&vh6, &vh7, &c2);
    elements.addVertexConnection(&vh7, &vh0, &c2);

    elements.addVertexConnection(&vh1, &vh7, &c2);
    elements.addVertexConnection(&vh3, &vh5, &c2);
    elements.addVertexConnection(&vh1, &vh3, &c2);
    elements.addVertexConnection(&vh5, &vh7, &c2);
    cursor = vh0.vertexEdge;
#endif
#ifndef HEXAGONO
    elements.addVertex(&v0);
    elements.addVertex(&v1);
    elements.addVertex(&v2);
    elements.addVertex(&v3);
    elements.addVertex(&v4);
    elements.addVertex(&v5);
    elements.addVertex(&v6);
    elements.addVertex(&v7);
    elements.addVertex(&v8);
    elements.addVertex(&v9);
    elements.addVertex(&v10);
    elements.addVertex(&v11);
    elements.addVertex(&v12);
    elements.addVertex(&v13);
    elements.addVertex(&v14);
    elements.addVertex(&v15);
    elements.addVertex(&v16);
    elements.addVertex(&v17);

    //Conecta poligono
    elements.addVertexConnection(&v0, &v1, &c2);
    elements.addVertexConnection(&v1, &v2, &c2);
    elements.addVertexConnection(&v2, &v3, &c2);
    elements.addVertexConnection(&v3, &v4, &c2);
    elements.addVertexConnection(&v4, &v5, &c2);
    elements.addVertexConnection(&v5, &v6, &c2);
    elements.addVertexConnection(&v6, &v7, &c2);
    elements.addVertexConnection(&v7, &v8, &c2);
    elements.addVertexConnection(&v8, &v9, &c2);
    elements.addVertexConnection(&v9, &v10, &c2);
    elements.addVertexConnection(&v10, &v11, &c2);
    elements.addVertexConnection(&v11, &v12, &c2);
    elements.addVertexConnection(&v12, &v13, &c2);
    elements.addVertexConnection(&v13, &v14, &c2);
    elements.addVertexConnection(&v14, &v15, &c2);
    elements.addVertexConnection(&v15, &v16, &c2);
    elements.addVertexConnection(&v16, &v17, &c2);
    elements.addVertexConnection(&v17, &v0, &c2);

    //Cria arestas de divisao planar
    elements.addVertexConnection(&v0, &v16, &c2);
    elements.addVertexConnection(&v3, &v14, &c2);
    elements.addVertexConnection(&v8, &v10, &c2);
    cursor = v0.vertexEdge;
#endif
    glPointSize(15);
    
    //poly.triangulate(elements.getAdjList());
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, key_callback);
    
    cursor->connectionColor = corDaVitoria;
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
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
