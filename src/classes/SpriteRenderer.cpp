#include "SpriteRenderer.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(Shader& shader)
    : shader(shader) {
    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::initRenderData() {
    float vertices[] = {
        // positions      // texture coordinates
         0.0f, 1.0f,      0.0f, 1.0f,
         1.0f, 0.0f,      1.0f, 0.0f,
         0.0f, 0.0f,      0.0f, 0.0f,

         0.0f, 1.0f,      0.0f, 1.0f,
         1.0f, 1.0f,      1.0f, 1.0f,
         1.0f, 0.0f,      1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



void SpriteRenderer::DrawSprite(
    Texture2D& texture,
    glm::vec2 position,  //from glm use vec2 dtype and create a variable named positon 
    glm::vec2 size,
    float rotation,
    glm::vec3 color
) {
    shader.Use();

    glm::mat4 model(1.0f); //create a 4x4 matrix and initialize its all values to 1.0f
    model = glm::translate(model, glm::vec3(position, 0.0f));
    //glm(translate) parameters 
    //param 1 : The matrix you wanna apply the movement
    //param 2 : glm::vec3 [x y z]

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader.SetMatrix4("model", model);
    shader.SetVector3("spriteColor", color.r, color.g, color.b);

    texture.Bind();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}