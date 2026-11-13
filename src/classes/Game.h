#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.h"
#include "Texture2D.h"

class SpriteRenderer;

// Possible conditions of the game.
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

struct Brick {   //a brick structure 
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
    bool destroyed = false;
};

class Game {
public:
    GameState State;
    bool Keys[1024]{};

    unsigned int Width;
    unsigned int Height;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

private:
    Shader spriteShader;
    std::unique_ptr<SpriteRenderer> renderer;

    Texture2D paddleTexture;  //Texture 2D is user defined class 
    Texture2D ballTexture;
    Texture2D blockTexture;

    glm::vec2 paddlePosition;
    glm::vec2 paddleSize;
    float paddleSpeed = 500.0f;

    glm::vec2 ballPosition;
    glm::vec2 ballSize;
    glm::vec2 ballVelocity;

    std::vector<Brick> bricks;
};