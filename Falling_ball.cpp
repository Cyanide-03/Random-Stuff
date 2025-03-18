#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

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

    if(ballY<=-0.9f){ // Ball hits the ground
        ballSpeed*=-0.8f;
        ballY=-0.9f;
    }
}

void rect_render(){
    glBegin(GL_QUADS);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2f(-0.3f,ballY-0.1f);
    glVertex2f(-0.5f,ballY-0.1f);
    glVertex2f(-0.5f,ballY+0.1f);
    glVertex2f(-0.3f,ballY+0.1f);
    glEnd();
}

void circle_render(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0.4f, ballY);
    float radius=0.1f;
    for(int i=0;i<=100;i++){
        float angle=2*M_PI*i/100;
        float x=radius*cos(angle);
        float y=radius*sin(angle);
        glVertex2f(0.4f+x,ballY+y);
    }

    glEnd();
}

int main(){
    initopenGL();
    GLFWwindow *window=glfwGetCurrentContext();

    while(!glfwWindowShouldClose(window)){
        update();
        circle_render();
        rect_render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}