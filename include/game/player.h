#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <iostream>

#include "utility/coord.h"
#include "shader.h"

using namespace std;

void clamp(float &variable, float maxpoint);

class Player{
public:
    coord position, deltaPos;
    float angle, maxspeed, scale, screen_boundary;
    unsigned int VBO, VAO, EBO;

    Shader shader;

    Player(float boundary);

    void rotate(int degree);
    void applyforce(float inForce);
    void update();
    void draw();
};