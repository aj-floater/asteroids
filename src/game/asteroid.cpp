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

bool isNear(vector<int> vector, int number, int threshold){
    bool isNear = false;
    for (int v : vector){
        if (number < v + threshold && number >= v)
            isNear = true;
        if (number > v - threshold && number <= v)
            isNear = true;
    }
    return isNear;
}

vector<int> generateangles(int numberofangles){
    vector<int> temp_angles;
    int randomnumber = 0;
    for (int i = 0; i < numberofangles; i++){
        randomnumber = RandomFloat(0, 360);
        while (isNear(temp_angles, randomnumber, 20)){
            randomnumber = RandomFloat(0, 360);
        }
        temp_angles.push_back(randomnumber);
    }
    sort(temp_angles.begin(), temp_angles.end());

    // vector<int> angles;
    // angles.push_back(temp_angles.at(0));
    // for (int i = 1; i < temp_angles.size(); i++)
    //     angles.push_back(temp_angles.at(i) - temp_angles.at(i - 1));
    // angles.push_back(360 - temp_angles.at(temp_angles.size()-1));
    
    return temp_angles;
}

Asteroid::Asteroid(float boundary){
    position.x = RandomFloat(-1.0f, 1.0f);
    position.y = RandomFloat(-1.0f, 1.0f);
    scale = RandomFloat(0.1f, 0.2f);
    angle = RandomFloat(0, 360);
    applyforce(RandomFloat(0.005, 0.015));
    maxspeed = 0.05;
    screen_boundary = boundary;

    shader.construct("/Users/archiejames/Desktop/Floater/build/shaders/asteroidshader.vs", "/Users/archiejames/Desktop/Floater/build/shaders/asteroidshader.fs");

    verticesnumber = rand() % 5 + 4;
    vector<int> angles = generateangles(verticesnumber);

    float vertices[verticesnumber*3];
    unsigned int indices[(verticesnumber-1)*3];

    for (int i = 0; i < 3; i++){
        vertices[i] = 0.0f;
    }
    float radius = RandomFloat(0.1, 0.5);
    for (int i = 0; i < verticesnumber; i++){
        vertices[i*3+3] = -sinf(angles[i] * (M_PI/180.0)) * radius;
        vertices[i*3+4] = cosf(angles[i] * (M_PI/180.0)) * radius;
        vertices[i*3+5] = 0.0f;
        indices[i*3] = 0;
        indices[i*3+1] = i+1;
        indices[i*3+2] = i+2;
    }

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
    glDrawElements(GL_TRIANGLES, verticesnumber*3, GL_UNSIGNED_INT, 0);
}

void Asteroid::update(){   
    // updates the position of the asteroid 
    clamp(deltaPos.x, maxspeed);
    clamp(deltaPos.y, maxspeed);

    position.x += deltaPos.x;
    position.y += deltaPos.y;
    if (position.x > (1 + screen_boundary)) position.x = -(1 + screen_boundary);
    if (position.x < -(1 + screen_boundary)) position.x = (1 + screen_boundary);

    if (position.y > (1 + screen_boundary)) position.y = -(1 + screen_boundary);
    if (position.y < -(1 + screen_boundary)) position.y = (1 + screen_boundary);
}
