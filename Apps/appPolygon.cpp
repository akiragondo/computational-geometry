//
//  appPoints.cpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "appPolygon.hpp"

//Color Palette e63946-f1faee-a8dadc-457b9d-1d3557

GLuint VBO, VAO, CBO;
// Window dimensions
const GLuint WIDTH = 800,HEIGHT =  800;
Vertex* cursor = nullptr;
Vertex* cursorPoint1 = nullptr;
Vertex* cursorPoint2 = nullptr;
Vertex intersects[6];
Polygon poly1;
Polygon poly2;
// Set up vertex data (and buffer(s)) and attribute pointers
ElementsList elements;

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos){
    double xc = 2.0*(xPos/(double)WIDTH) - 1.0;
    double yc = 2.0*((HEIGHT - yPos)/(double)HEIGHT) - 1.0;
    
    cursor->x = xc;
    cursor->y = yc;
    
    for(int i = 0; i < poly1.getPolygonSize() - 1; i ++){
        if(cursor == cursorPoint1)
            intersects[i] = getIntersectingPoint(*poly1.getVertex(5), *cursor, *poly1.getVertex(i), *poly1.getVertex((i + 1)%poly1.getPolygonSize()));
        else
            intersects[i] = getIntersectingPoint(*poly1.getVertex(1), *cursor, *poly1.getVertex(i), *poly1.getVertex((i + 1)%poly1.getPolygonSize()));
    }
    std::cout<<"Area: " <<poly1.getPolygonArea() << std::endl;
    
    poly1.paintConvexPoints(false);
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( elements.getVerticesList() ), elements.getVerticesList(), GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(elements.getColorsList()), elements.getColorsList(), GL_STATIC_DRAW);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        if(cursor == cursorPoint1)
            cursor = cursorPoint2;
        else
            cursor = cursorPoint1;
    }
    poly2.paintConvexPoints(false);
}

void paintPolygon(Polygon poly){
    if(poly.getPolygonArea() < 0){
        for(int i = 0; i < poly.getPolygonSize(); i++)
            poly.getColor(i)->assignColor(0.8, 0.1, 0.1);
    }
    else{
        for(int i = 0; i < poly.getPolygonSize(); i++)
            poly.getColor(i)->assignColor(0.1, 0.1, 0.8);
    }
}

int runAppPolygon()
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
    //Cria callbacks
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    
    //Cria Polígono horário
    Vertex p0(0,0,0);
    Vertex p1(0.4,0.5,0);
    Vertex p2(0.6,0.5,0);
    Vertex p3(0.2,0,0);
    Vertex p4(0.6,-0.5,0);
    Vertex p5(0.4,-0.5,0);
    Color c1(0.5, 0.1, 0.1);
    Color c2(0.1, 0.5, 0.1);
    poly1.addVertex(&p0,&c1);
    poly1.addVertex(&p1,&c2);
    poly1.addVertex(&p2,&c1);
    poly1.addVertex(&p3,&c1);
    poly1.addVertex(&p4,&c1);
    poly1.addVertex(&p5,&c2);
    elements.addPolygon(poly1);
    cursorPoint1 = &p1;
    cursorPoint2 = &p5;
    cursor = cursorPoint2;
    elements.addVertexConnection(&p1, cursor, &c2);
    
    for(int i = 0; i < poly1.getPolygonSize() - 1; i ++){
        if(cursor == cursorPoint1)
            intersects[i] = getIntersectingPoint(*poly1.getVertex(5), *cursor, *poly1.getVertex(i), *poly1.getVertex((i + 1)%poly1.getPolygonSize()));
        else
            intersects[i] = getIntersectingPoint(*poly1.getVertex(1), *cursor, *poly1.getVertex(i), *poly1.getVertex((i + 1)%poly1.getPolygonSize()));
    }
    for (int i = 0; i < 6; i++)
        elements.addVertex(&(intersects[i]),  &c1);
    
    //Cria Polígono antihorário
    Vertex pp0(-0.1,0,0);
    Vertex pp1(-0.5,-0.5,0);
    Vertex pp2(-0.7,-0.5,0);
    Vertex pp3(-0.3,0,0);
    Vertex pp4(-0.7,0.5,0);
    Vertex pp5(-0.5,0.5,0);
    poly2.addVertex(&pp5);
    poly2.addVertex(&pp4);
    poly2.addVertex(&pp3);
    poly2.addVertex(&pp2);
    poly2.addVertex(&pp1);
    poly2.addVertex(&pp0);
    elements.addPolygon(poly2);
    glPointSize(10);
    paintPolygon(poly1);
    paintPolygon(poly2);
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
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &CBO );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}
