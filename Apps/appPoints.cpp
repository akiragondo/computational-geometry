//
//  appPoints.cpp
//  teste-opengl
//
//  Created by Gustavo Akira Gondo on 21/08/19.
//  Copyright © 2019 Gustavo Akira Gondo. All rights reserved.
//

#include "appPoints.hpp"

#define MAX_NUM_POINTS 4096

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec4 eColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"eColor = vec4(color.x, color.y, color.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec4 eColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = eColor;\n"
"}\n\0";

//Color Palette e63946-f1faee-a8dadc-457b9d-1d3557

GLuint VBO, VAO, CBO;
// Window dimensions
const GLuint WIDTH = 800,HEIGHT =  800;

// Set up vertex data (and buffer(s)) and attribute pointers
Vertex vertices[MAX_NUM_POINTS];
Color colors[MAX_NUM_POINTS];

int nRandomPoints = 2400;
int nPoints =0 ;
int userInsertedPoints =0;


void insertNewVertex(double xPosition, double yPosition, bool isUser){
    double xc = 2.0*(xPosition/(double)WIDTH) - 1.0;
    double yc = 2.0*((HEIGHT - yPosition)/(double)HEIGHT) - 1.0;
    std::cout << "Left Button Pressed in coordinates x: " << xc << " - y: " <<yc << std::endl;
    vertices[nPoints].assignCoord(xc, yc, 0.0);
    if(!isUser)
        colors[nPoints].assignColor(0.7, 0.6, 0.7);
    else
        colors[nPoints].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
    nPoints++;
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void insertRandomPoints(GLFWwindow *window,int nRandomPoints){
    int randomY = rand()%HEIGHT;
    int randomX = rand()%WIDTH;
    for (int i = 0; i < nRandomPoints; i++){
        randomY = rand()%HEIGHT;
        randomX = rand()%WIDTH;
        insertNewVertex(randomX, randomY,false);
        std::cout << "Inserted new Random point" << std::endl;
    }
}
void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos){
    double xc = 2.0*(xPos/(double)WIDTH) - 1.0;
    double yc = 2.0*((HEIGHT - yPos)/(double)HEIGHT) - 1.0;
    if(userInsertedPoints == 1){
        vertices[nPoints-1].assignCoord(xc, yc, 0.0);
        colors[nPoints-1].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
        //Recalcula cores para todos os outros pontos
        double slope = (vertices[nPoints-1].y - vertices[nPoints-2].y)/(vertices[nPoints-1].x - vertices[nPoints-2].x);
        //Calculate y intercept at 0 and at HEIGHT
        Vertex p1 = vertices[nPoints-2];
        Vertex p2 = vertices[nPoints-1];
        double y0 = ((p2.y - p1.y)/(p2.x-p1.x))*(p1.x*(-1)) + p1.y;
        double yw = ((p2.y - p1.y)/(p2.x-p1.x))*(WIDTH - p1.x*(-1)) + p1.y;
        Vertex p0(0, y0,0.0);
        Vertex pw(WIDTH, yw,0.0);
        if(p2.x < p1.x){
            p0.assignCoord(pw.x, pw.y, pw.z);
            pw.assignCoord(0, y0, 0.0);
        }
        for (int i = 0; i<nRandomPoints; i++) {
            if(left(p0, pw, vertices[i]))
                colors[i].assignColor(230/255.0, 57/255.0, 70/255.0);
            else
                colors[i].assignColor(29/255.0, 53/255.0, 87/255.0);
        }
        std::cout<< "Slope of the curve: " << slope << std::endl;
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    }
}



void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        if(userInsertedPoints == 0){
            insertNewVertex(xPos, yPos, true);
            userInsertedPoints++;
            nPoints++;
        }
        else if(userInsertedPoints == 1){
            insertNewVertex(xPos, yPos, true);
            userInsertedPoints++;
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        if(userInsertedPoints == 2){
            vertices[nPoints-1].assignCoord(-1, -1, -1 );
            vertices[nPoints-2].assignCoord(-1, -1, -1 );
            userInsertedPoints = 0;
            nPoints = nRandomPoints;
        }
        else if (userInsertedPoints == 1){
            vertices[nPoints-1].assignCoord(-1, -1, -1 );
            userInsertedPoints = 0;
            nPoints = nRandomPoints;
        }
    }
}

int runAppPoints()
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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    
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
    
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
    
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return(1);
    }
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );
    
    // Check for compile time errors
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
    
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return(1);
    }
    
    // Link shaders
    GLuint shaderProgram = glCreateProgram( );
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );
    
    // Check for linking errors
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    
    if ( !success )
    {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return(1);
    }
    insertRandomPoints(window, nRandomPoints);
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
    
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    
    /* Create color buffer object (CBO). */
    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    glPointSize(10);
    glLineWidth(2);
    
    //insertRandomPoints(window, nRandomPoints);
    
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 241.0/255.0, 250.0/255.0, 238.0/255.0, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Draw our first triangle
        glUseProgram( shaderProgram );
        glBindVertexArray( VAO );
        //glDrawArrays( GL_TRIANGLES, 0, nPoints);
        glDrawArrays( GL_POINTS, 0, nPoints);
        glDrawArrays( GL_LINES, nRandomPoints, 2);
        
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
