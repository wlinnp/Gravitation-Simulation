#ifndef GAME_H
#define	GAME_H

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "Constants.h"
#include "Particle.h"

class Game
{
private:
    // screen info
    int width;
    int height;
    
    // for timing frames
    unsigned int start;
    unsigned int last;
    unsigned int current;
    
    // for game status
    bool good;
    bool running;
    
    // SDL managed
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* particleTexture;
    
    // game data
    std::vector<Particle> particles;
    
    // Handle game logic updates here
    void update(double dt);
    
    // Render the game contents to the screen here
    void render();
    
    // Handle any user input events here
    void handleEvent(const SDL_Event& event);

    // helper function for drawing particles to screen    
    void drawParticle(const Particle& p);
    
    // helper function for generating random particles
    Particle randomParticle() const;
public:
    // This will act as our initialize function
    Game();
    
    // We don't want to be able to copy a game
    Game(const Game& other) = delete;
    Game& operator=(const Game& right) = delete;
    
    // This will act as our cleanup function
    ~Game();
    
    // Will handle actually running the game (the gameplay loop)
    int operator()();
};

#endif
