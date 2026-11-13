#include "Game.h"

#include "SpriteRenderer.h"
#include "../../include/stb_image.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>

Texture2D LoadGameTexture(const char* path) {
    Texture2D texture;
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (channels == 4) {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    } else {
        texture.internalFormat = GL_RGB;
        texture.imageFormat = GL_RGB;
    }

    texture.Generate(data, width, height);
    stbi_image_free(data);
    return texture;
}

Texture2D LoadCircleTexture() {
    const unsigned int size = 32;
    std::vector<unsigned char> pixels(size * size * 4);

    for (unsigned int y = 0; y < size; ++y) {
        for (unsigned int x = 0; x < size; ++x) {
            float dx = static_cast<float>(x) + 0.5f - size / 2.0f;
            float dy = static_cast<float>(y) + 0.5f - size / 2.0f;
            unsigned int index = (y * size + x) * 4;
            unsigned char alpha = std::hypot(dx, dy) <= size / 2.0f ? 255 : 0;

            pixels[index] = 255;
            pixels[index + 1] = 255;
            pixels[index + 2] = 255;
            pixels[index + 3] = alpha;
        }
    }

    Texture2D texture;
    texture.internalFormat = GL_RGBA;
    texture.imageFormat = GL_RGBA;
    texture.Generate(pixels.data(), size, size);
    return texture;
}

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height) {
}

Game::~Game() = default;

void Game::Init() {
    bricks.clear();
    spriteShader.Load(
        "break_out/shaders/sprite.vert",
        "break_out/shaders/sprite.frag"
    );
    spriteShader.Use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width),
                                      static_cast<float>(Height), 0.0f);
    spriteShader.SetInteger("image", 0);
    spriteShader.SetMatrix4("projection", projection);

    renderer = std::make_unique<SpriteRenderer>(spriteShader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    paddleTexture = LoadGameTexture("break_out/resources/textures/paddle.png");
    ballTexture = LoadCircleTexture();
    blockTexture = LoadGameTexture("break_out/resources/textures/block.png");

    paddleSize = glm::vec2(100.0f, 20.0f);
    paddlePosition = glm::vec2(Width / 2.0f - paddleSize.x / 2.0f,
                               Height - 40.0f);

    ballSize = glm::vec2(20.0f);
    ballPosition = paddlePosition + glm::vec2(paddleSize.x / 2.0f - ballSize.x / 2.0f,
                                               -ballSize.y);
    ballVelocity = glm::vec2(250.0f, -350.0f);

    const float brickWidth = 65.0f;
    const float brickHeight = 20.0f;
    const float leftPadding = 25.0f;
    glm::vec3 colors[] = {
        glm::vec3(0.8f, 0.2f, 0.2f), glm::vec3(0.2f, 0.8f, 0.2f),
        glm::vec3(0.2f, 0.2f, 0.8f), glm::vec3(0.8f, 0.8f, 0.2f)
    };

    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 11; ++column) {
            bricks.push_back({
                glm::vec2(leftPadding + column * (brickWidth + 5.0f),
                          50.0f + row * (brickHeight + 5.0f)),
                glm::vec2(brickWidth, brickHeight), colors[row], false
            });
        }
    }
    
}

void Game::ProcessInput(float dt) {
    if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) {
        paddlePosition.x -= paddleSpeed * dt;
    }
    if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) {
        paddlePosition.x += paddleSpeed * dt;
    }

    if (paddlePosition.x < 0.0f) paddlePosition.x = 0.0f;
    if (paddlePosition.x + paddleSize.x > Width) {
        paddlePosition.x = Width - paddleSize.x;
    }
}

void Game::Update(float dt) {
    if (dt < 0.0f) dt = 0.0f;
    if (dt > 0.02f) dt = 0.02f;

    float movementDistance = glm::length(ballVelocity * dt);
    int steps = static_cast<int>(movementDistance / 4.0f) + 1;
    glm::vec2 step = ballVelocity * dt / static_cast<float>(steps);

    for (int i = 0; i < steps; ++i) {
        ballPosition += step;

        if (ballPosition.x <= 0.0f) {
            ballPosition.x = 0.0f;
            ballVelocity.x = std::abs(ballVelocity.x);
        }
        if (ballPosition.x + ballSize.x >= Width) {
            ballPosition.x = Width - ballSize.x;
            ballVelocity.x = -std::abs(ballVelocity.x);
        }
        if (ballPosition.y <= 0.0f) {
            ballPosition.y = 0.0f;
            ballVelocity.y = std::abs(ballVelocity.y);
        }

        if (ballPosition.y + ballSize.y >= paddlePosition.y &&
            ballPosition.y <= paddlePosition.y + paddleSize.y &&
            ballPosition.x + ballSize.x >= paddlePosition.x &&
            ballPosition.x <= paddlePosition.x + paddleSize.x) {
            ballPosition.y = paddlePosition.y - ballSize.y;
            ballVelocity.y = -std::abs(ballVelocity.y);
        }

        for (Brick& brick : bricks) {
            if (brick.destroyed) continue;

            bool hit = ballPosition.x + ballSize.x >= brick.position.x &&
                       ballPosition.x <= brick.position.x + brick.size.x &&
                       ballPosition.y + ballSize.y >= brick.position.y &&
                       ballPosition.y <= brick.position.y + brick.size.y;

            if (hit) {
                brick.destroyed = true;

                float overlapLeft = ballPosition.x + ballSize.x - brick.position.x;
                float overlapRight = brick.position.x + brick.size.x - ballPosition.x;
                float overlapTop = ballPosition.y + ballSize.y - brick.position.y;
                float overlapBottom = brick.position.y + brick.size.y - ballPosition.y;

                if (std::min(overlapLeft, overlapRight) <
                    std::min(overlapTop, overlapBottom)) {
                    ballVelocity.x = overlapLeft < overlapRight
                        ? -std::abs(ballVelocity.x)
                        : std::abs(ballVelocity.x);
                } else {
                    ballVelocity.y = overlapTop < overlapBottom
                        ? -std::abs(ballVelocity.y)
                        : std::abs(ballVelocity.y);
                }
                break;
            }
        }
    }

    if (ballPosition.y > Height) {
        paddlePosition.x = Width / 2.0f - paddleSize.x / 2.0f;
        ballPosition = paddlePosition + glm::vec2(paddleSize.x / 2.0f - ballSize.x / 2.0f,
                                                   -ballSize.y);
        ballVelocity = glm::vec2(250.0f, -350.0f);
    }
}

void Game::Render() {
    for (const Brick& brick : bricks) {
        if (!brick.destroyed) {
            renderer->DrawSprite(blockTexture, brick.position, brick.size,
                                 0.0f, brick.color);
        }
    }

    renderer->DrawSprite(paddleTexture, paddlePosition, paddleSize);
    renderer->DrawSprite(ballTexture, ballPosition, ballSize, 0.0f,
                         glm::vec3(1.0f));
}