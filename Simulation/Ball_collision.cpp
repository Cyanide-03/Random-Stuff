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
    float mass;
    float radius;
}Ball;

std::vector<Ball> balls;

void take_input(){
    srand(time(0));
    int numBalls;
    std::cout<<"Enter the number of balls: ";
    std::cin>>numBalls;

    for(int i=1;i<=numBalls;i++){
        Ball ball;

        ball.x=(rand()%1800-900)/1000.0f; // Random position between -0.9 and 0.9
        ball.y=(rand()%1800-900)/1000.0f; // Random position between -0.9 and 0.9
        ball.vx=(rand()%2000-1000)/250.0f; // Random velocity between -10 and 10
        ball.vy=(rand()%2000-1000)/250.0f; // Random velocity between -10 and 10

        ball.mass=rand()%10+1;
        ball.radius=0.05f+0.01f*ball.mass;

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

void resolve_balls_collision(Ball *ball1,Ball *ball2){
    float dx=ball2->x-ball1->x;
    float dy=ball2->y-ball1->y;
    float distance=sqrt(pow(dx,2)+pow(dy,2));

    if(distance<=ball1->radius+ball2->radius){ // Collision Detected
        float sin_theta=dy/distance;
        float cos_theta=dx/distance;

        float v1n=ball1->vx*cos_theta+ball1->vy*sin_theta; // Normal velocity of ball1
        float v2n=ball2->vx*cos_theta+ball2->vy*sin_theta; // Normal velocity of ball2
        float v1t=ball1->vy*cos_theta-ball1->vx*sin_theta; // Tangential velocity of ball1
        float v2t=ball2->vy*cos_theta-ball2->vx*sin_theta; // Tangential velocity of ball2

        float v1n_new=((ball1->mass-ball2->mass)*v1n+2*ball2->mass*v2n)/(ball1->mass+ball2->mass);
        float v2n_new=((ball2->mass-ball1->mass)*v2n+2*ball1->mass*v1n)/(ball1->mass+ball2->mass);

        ball1->vx=v1n_new*cos_theta+v1t*sin_theta;
        ball2->vx=v2n_new*cos_theta+v2t*sin_theta;
        ball1->vy=v1n_new*sin_theta+v1t*cos_theta;
        ball2->vy=v2n_new*sin_theta+v2t*cos_theta;

        float overlap=(ball1->radius+ball2->radius-distance)/2.0f;
        float movex=overlap*cos_theta;
        float movey=overlap*sin_theta;

        ball1->x-=movex;
        ball2->x+=movex;
        ball1->y-=movey;
        ball2->y+=movey;
    }
}

void update(){
    for(auto &ball:balls){
        ball.y+=ball.vy*dt+0.5f*acc*dt*dt;
        ball.vy+=acc*dt;
        ball.x+=ball.vx*dt;

        if(ball.x+ball.radius>=1.0f){ // Ball hits the right wall
            ball.vx*=-0.8f;
            ball.x=1.0f-ball.radius;
        }

        if(ball.x-ball.radius<=-1.0f){ // Ball hits the left wall
            ball.vx*=-0.8f;
            ball.x=-1.0f+ball.radius;
        }

        if(ball.y+ball.radius>=1.0f){ // Ball hits the ceiling
            ball.vy*=-0.8f;
            ball.y=1.0f-ball.radius;
        }

        if(ball.y-ball.radius<=-1.0f){ // Ball hits the ground
            ball.vy*=-0.8f;
            ball.y=-1.0f+ball.radius;

            ball.vx *= friction;

            if(abs(ball.vx)<0.001f){
                ball.vx=0.0f;
            }
        }
    }

    for(int i=0;i<balls.size();i++){
        for(int j=i+1;j<balls.size();j++){
            resolve_balls_collision(&balls[i], &balls[j]);
        }
    }
}

void circle_render(Ball *ball){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ball->x, ball->y);
    for(int i=0;i<=100;i++){
        float angle=2*M_PI*i/100;
        float x=ball->radius*cos(angle);
        float y=ball->radius*sin(angle);
        glVertex2f(ball->x+x,ball->y+y);
    }

    glEnd();
}

void ball_render(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.1f,0.6f,0.3f);
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