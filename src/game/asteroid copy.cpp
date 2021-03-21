#include "game/asteroid.h"

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Asteroid::applyforce(float inForce){
    deltaPos.x += sinf(angle * (M_PI/180.0)) * inForce;
    deltaPos.y += cosf(angle * (M_PI/180.0)) * inForce;
}

Asteroid::Asteroid(){
    position.x = RandomFloat(-1.0f, 1.0f);
    position.y = RandomFloat(-1.0f, 1.0f);
    scale = RandomFloat(0.05f, 0.1f);
    angle = RandomFloat(0, 360);
    applyforce(RandomFloat(0.005, 0.015));
    maxspeed = 0.05;

    shader.construct("/Users/archiejames/Desktop/Floater/build/shaders/asteroidshader.vs", "/Users/archiejames/Desktop/Floater/build/shaders/asteroidshader.fs");

    int verticesnumber = rand() % 5 + 4;

    float vertices[] = {
        0.0f, 0.0f, 0.0f,    // 0 middle
        0.5f, 0.25f, 0.0f,   // 1 middle right top
        0.5f, -0.25f, 0.0f,  // 2 middle right bottom
        -0.5f, 0.25f, 0.0f,  // 3 middle left top
        -0.5f, -0.25f, 0.0f, // 4 middle left bottom
        0.25f, 0.5f, 0.0f,   // 5 middle top right
        -0.25f, 0.5f, 0.0f,  // 6 middle top left
        0.25f, -0.5f, 0.0f,  // 7 middle bottom right
        -0.25f, -0.5f, 0.0f  // 8 middle bottom left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, // right
        0, 2, 7, // bottom right
        0, 7, 8, // bottom
        0, 8, 4, // bottom left
        0, 4, 3, // left
        0, 3, 6, // top left
        0, 6, 5, // top
        0, 5, 1  // top right
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    
    glBindVertexArray(0); 
}

void Asteroid::draw(){
    shader.use();

    glm::mat4 trans = glm::mat4(1.0f);
        
    trans = glm::translate(trans, glm::vec3(position.x, position.y, 0.0f));
    trans = glm::rotate(trans, glm::radians(-angle), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));

    unsigned int transformLocAs = glGetUniformLocation(shader.ID,"transform");
    glUniformMatrix4fv(transformLocAs, 1, GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}

void Asteroid::update(){   
    // updates the position of the asteroid 
    clamp(deltaPos.x, maxspeed);
    clamp(deltaPos.y, maxspeed);

    position.x += deltaPos.x;
    position.y += deltaPos.y;
    if (position.x > (1 + maxspeed)) position.x = -(1 + maxspeed);
    if (position.x < -(1 + maxspeed)) position.x = (1 + maxspeed);

    if (position.y > (1 + maxspeed)) position.y = -(1 + maxspeed);
    if (position.y < -(1 + maxspeed)) position.y = (1 + maxspeed);
}
