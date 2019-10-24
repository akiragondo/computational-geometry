
#include "AppTriangles.hpp"

//Color Palette e63946-f1faee-a8dadc-457b9d-1d3557


GLuint VBO, VAO, CBO;
// Window dimensions
const GLuint WIDTH = 600,HEIGHT = 600;
int npoints = 0;

// Set up vertex data (and buffer(s)) and attribute pointers
Vertex vertices[MAX_NUM_POINTS];
Color colors[MAX_NUM_POINTS];

int nRandomPoints = 100;
int nPoints =0 ;

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos);
void cursorEnterCallback(GLFWwindow *window, int entered);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos){
    double xc = 2.0*(xPos/(double)WIDTH) - 1.0;
    double yc = 2.0*((HEIGHT - yPos)/(double)HEIGHT) - 1.0;
    if(nPoints > 0){
        vertices[nPoints-1].assignCoord(xc, yc, 0.0);
        colors[nPoints-1].assignColor(0.7, 0.7, 0.8);
        if((nPoints)%3 == 0){
            if(left(vertices[nPoints-1], vertices[nPoints-2], vertices[nPoints-3])){
                colors[nPoints-1].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
                colors[nPoints-2].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
                colors[nPoints-3].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
            }
            else{
                colors[nPoints-1].assignColor(230.0/255.0, 57.0/255.0, 70.0/255.0);
                colors[nPoints-2].assignColor(230.0/255.0, 57.0/255.0, 70.0/255.0);
                colors[nPoints-3].assignColor(230.0/255.0, 57.0/255.0, 70.0/255.0);
            }
            double area = triangleArea(vertices[nPoints-1],vertices[nPoints-2],vertices[nPoints-3]);
            std::cout << "Area do triangulo atual: " << area << std::endl;
        }
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    }
}

void cursorEnterCallback(GLFWwindow *window, int entered){
    if(entered){
        std::cout<<"Entered Window" << std::endl;
    }
    else{
        std::cout<<"Left Window" << std::endl;
    }
}

void insertNewVertex(double mouseX, double mouseY){
    double xc = 2.0*(mouseX/(double)WIDTH) - 1.0;
    double yc = 2.0*((HEIGHT - mouseY)/(double)HEIGHT) - 1.0;
    std::cout << "Left Button Pressed in coordinates x: " << xc << " - y: " <<yc << std::endl;
    vertices[nPoints].assignCoord(xc, yc, 0.0);
    colors[nPoints].assignColor(0.7, 0.7, 0.8);
    nPoints++;
    if((nPoints)%3 == 0){
        if(left(vertices[nPoints-1], vertices[nPoints-2], vertices[nPoints-3])){
            colors[nPoints-1].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
            colors[nPoints-2].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
            colors[nPoints-3].assignColor(168.0/255.0, 218.0/255.0, 220.0/255.0);
        }
        else{
            colors[nPoints-1].assignColor(230.0/255.0, 57.0/255.0, 70.0/255.0);
            colors[nPoints-2].assignColor(230.0/255.0, 57.0/255.0, 70.0/255.0);
            colors[nPoints-3].assignColor(230.0/255.0, 57.0/255.0, 70.0/255.0);
        }
    }
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        insertNewVertex(xPos, yPos);
        
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        std::cout << "Right Button Pressed" << std::endl;
    }
}


int runAppTriangles()
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
    
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    GLuint program = LoadShaders("/Users/akira/Documents/teste-opengl/teste-opengl/include/simple.vert", "/Users/akira/Documents/teste-opengl/teste-opengl/include/simple.frag");
    glUseProgram(program);
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
    
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 241.0/255.0, 250.0/255.0, 238.0/255.0, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Draw our first triangle
        glUseProgram( program );
        glBindVertexArray( VAO );
        //glDrawArrays( GL_TRIANGLES, 0, nPoints);
        glDrawArrays( GL_POINTS, 0, nPoints);
        glDrawArrays( GL_TRIANGLES, 0, nPoints);
        
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


