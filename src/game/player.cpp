#include "game/player.h"

Player::Player(float boundary){
    maxspeed = 0.025;
    scale = 0.1f;
    screen_boundary = boundary;

    shader.construct("/Users/archiejames/Desktop/Floater/build/shaders/shader.vs", "/Users/archiejames/Desktop/Floater/build/shaders/shader.fs");

    float vertices[] = {
          0.0f,   0.5f, 0.0f, // 0 top
          0.5f,  -0.5f, 0.0f, // 1 bottom right
         0.25f, -0.25f, 0.0f, // 2 right
         -0.5f,  -0.5f, 0.0f, // 3 bottom left
        -0.25f, -0.25f, 0.0f  // 4 left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, // right
        0, 3, 4, // left
        0, 2, 4  // middle
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

void Player::applyforce(float inForce){
    deltaPos.x += sinf(angle * (M_PI/180.0)) * inForce;
    deltaPos.y += cosf(angle * (M_PI/180.0)) * inForce;
}

void Player::rotate(int degree){
    angle += degree;
    if (angle < 0) angle = 360;
    if (angle > 360) angle = 0;
}

void Player::update(){
    // updates the position of the player
    clamp(deltaPos.x, maxspeed);
    clamp(deltaPos.y, maxspeed);
    position.x += deltaPos.x;
    position.y += deltaPos.y;
    if (position.x > (1 + screen_boundary)) position.x = -(1 + screen_boundary);
    if (position.x < -(1 + screen_boundary)) position.x = (1 + screen_boundary);
    if (position.y > (1 + screen_boundary)) position.y = -(1 + screen_boundary);
    if (position.y < -(1 + screen_boundary)) position.y = (1 + screen_boundary);
}

void Player::draw(){
    shader.use();

    glm::mat4 trans = glm::mat4(1.0f);
        
    trans = glm::translate(trans, glm::vec3(position.x, position.y, 0.0f));
    trans = glm::rotate(trans, glm::radians(-angle), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));

    unsigned int transformLoc = glGetUniformLocation(shader.ID,"transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // render
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}