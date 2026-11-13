//A sprite is a 2D textured image used in a game to represent an object:
//sprite = image + position + size + rotation


//Sprite renderer is responsible for drawing sprites
#pragma once //prevents header file from being included multiple times 

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture2D.h"

class SpriteRenderer {
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

    void DrawSprite(
        Texture2D& texture,
        glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f),
        float rotation = 0.0f,
        glm::vec3 color = glm::vec3(1.0f)
    );

private:
    Shader& shader; //declaras a reference variable named shader that points to Shader object
    unsigned int quadVAO = 0; //Just the variables that stores id
    unsigned int quadVBO = 0;

    void initRenderData();
};