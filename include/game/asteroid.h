#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "utility/coord.h"
#include "shader.h"

using namespace std;

void clamp(float &variable, float maxpoint);

class Asteroid{
public:
    coord position, deltaPos;
    float angle, maxspeed, scale, screen_boundary;
    int verticesnumber;

    Shader shader;

    unsigned int VBO, VAO, EBO;   

    Asteroid(float boundary);
    void applyforce(float inForce);
    void update();
    void draw();
};