#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "classes/Game.h"


void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height); // //choosing the size and location of the canvas 
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (action == GLFW_PRESS && key >= 0 && key < 1024) {
        game->Keys[key] = true;
    } else if (action == GLFW_RELEASE && key >= 0 && key < 1024) {
        game->Keys[key] = false;
    }
}


int main()
{
    glfwInit(); //Initialize GLFW

    
    //if we dont give hints opengl will use its default version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Set the major version of the OpenGL context to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set the minor version of the
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Set the OpenGL profile to core
    //configure context before creating window and context , hints are just requests to opengl , it may or may not give us the version we requested
    
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Breakout", NULL, NULL); //Create a window with the specified width, height, and title
    //creates window and context // context is statemachine and memoryspace for opengl to render to the window
    
    glfwMakeContextCurrent(window); //activates the context for the current thread // we can have multiple contexts but only one can be active at a time
    

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    gladLoadGL((GLADloadfunc)glfwGetProcAddress); // Initializes the OpenGL function pointers using the active context.

    Game Breakout(800,600);
    glfwSetWindowUserPointer(window, &Breakout);

    Breakout.Init(); ////Loads shaders, textures, and initial game objects.

    float lastFrameTime = 0.0f;



while (!glfwWindowShouldClose(window)) {  //render loop
    float currentFrameTime =
        static_cast<float>(glfwGetTime()); //Returns elapsed application time in seconds, 
        //used to calculate frame-independent movement.

    float deltaTime =  //seconds elapsed since previous frame
        currentFrameTime - lastFrameTime;

    lastFrameTime = currentFrameTime;

    glfwPollEvents(); //Processes input, resize, and window events.


   

    Breakout.ProcessInput(deltaTime);  //ProcessInput and other are member functions of Game 
    //so cant be called alone 
    Breakout.Update(deltaTime);  //Moves objects and checks collisions.

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //Sets the color used when clearing the framebuffer:


    glClear(GL_COLOR_BUFFER_BIT);

    Breakout.Render();

glfwSwapBuffers(window);

}
 glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
