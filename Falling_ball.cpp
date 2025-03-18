#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

float ballY=0.5f;
float ballSpeed=0.0f;
float acc=-9.8f;
float dt=0.002f;

void initopenGL(){
    if(!glfwInit()){
        std::cerr<<"Error initializing glfw";
        exit(1);
    }
    GLFWwindow *window=glfwCreateWindow(800,600,"Falling Ball",NULL,NULL);
    if(!window){
        std::cerr<<"Error creating window";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glewInit();
}

void update(){
    ballSpeed+=acc*dt;
    ballY+=ballSpeed*dt+0.5f*acc*dt*dt;
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f,ballY-0.1f);
    glVertex2f(0.1f,ballY-0.1f);
    glVertex2f(0.1f,ballY+0.1f);
    glVertex2f(-0.1f,ballY+0.1f);
    glEnd();
}

int main(){
    initopenGL();
    GLFWwindow *window=glfwGetCurrentContext();

    while(!glfwWindowShouldClose(window)){
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}