#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <bits/stdc++.h>

float acc=-9.8f;
float dt=0.002f;
float friction=0.99f;

typedef struct Ball{
    float x,y;
    float vx,vy;
}Ball;

std::vector<Ball> balls;

void take_input(){
    int numBalls;
    std::cout<<"Enter the number of balls: ";
    std::cin>>numBalls;

    for(int i=1;i<=numBalls;i++){
        Ball ball;
        std::cout<<"Enter initial X position (-0.8 to 0.8) for ball "<<i<<": ";
        std::cin>>ball.x;

        std::cout<<"Enter initial Y position (-0.8 to 0.8) for ball "<<i<<": ";
        std::cin>>ball.y;

        std::cout<<"Enter initial velocity in X direction for ball "<<i<<": ";
        std::cin>>ball.vx;

        std::cout<<"Enter initial velocity in Y direction for ball "<<i<<": ";
        std::cin>>ball.vy;

        balls.push_back(ball);
    }
}

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
    for(auto &ball:balls){
        ball.vy+=acc*dt;

        ball.y+=ball.vy*dt+0.5f*acc*dt*dt;
        ball.x+=ball.vx*dt;

        if(ball.x>=0.9f){ // Ball hits the right wall
            ball.vx*=-0.8f;
            ball.x=0.9f;
        }

        if(ball.x<=-0.9f){ // Ball hits the left wall
            ball.vx*=-0.8f;
            ball.x=-0.9f;
        }

        if(ball.y>=0.9f){ // Ball hits the ceiling
            ball.vy*=-0.8f;
            ball.y=0.9f;
        }

        if(ball.y<=-0.9f){ // Ball hits the ground
            ball.vy*=-0.8f;
            ball.y=-0.9f;

            ball.vx *= friction;

            if(abs(ball.vx)<0.001f){
                ball.vx=0.0f;
            }
        }
    }
}

void circle_render(Ball *ball){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ball->x, ball->y);
    float radius=0.1f;
    for(int i=0;i<=100;i++){
        float angle=2*M_PI*i/100;
        float x=radius*cos(angle);
        float y=radius*sin(angle);
        glVertex2f(ball->x+x,ball->y+y);
    }

    glEnd();
}

void ball_render(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f,1.0f,0.0f);
    for(auto ball:balls){
        circle_render(&ball);
    }
}

int main(){
    take_input();
    initopenGL();
    GLFWwindow *window=glfwGetCurrentContext();

    while(!glfwWindowShouldClose(window)){
        update();
        ball_render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}