#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

float ballY=0.5f;
float ballX=0.0f;
float ballVy=20.0f;
float ballVx=3.0f;
float acc=-9.8f;
float dt=0.002f;
float friction=0.99f;

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
    ballVy+=acc*dt;

    ballY+=ballVy*dt+0.5f*acc*dt*dt;
    ballX+=ballVx*dt;

    if(ballX>=0.9f){ // Ball hits the right wall
        ballVx*=-0.8f;
        ballX=0.9f;
    }

    if(ballX<=-0.9f){ // Ball hits the left wall
        ballVx*=-0.8f;
        ballX=-0.9f;
    }

    if(ballY>=0.9f){ // Ball hits the ceiling
        ballVy*=-0.8f;
        ballY=0.9f;
    }

    if(ballY<=-0.9f){ // Ball hits the ground
        ballVy*=-0.8f;
        ballY=-0.9f;
    }
}

void circle_render(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(ballX, ballY);
    float radius=0.1f;
    for(int i=0;i<=100;i++){
        float angle=2*M_PI*i/100;
        float x=radius*cos(angle);
        float y=radius*sin(angle);
        glVertex2f(ballX+x,ballY+y);
    }

    glEnd();
}

int main(){
    initopenGL();
    GLFWwindow *window=glfwGetCurrentContext();

    while(!glfwWindowShouldClose(window)){
        update();
        circle_render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}