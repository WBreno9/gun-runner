#ifndef GAME_H
#define GAME_H

#include <inc.h>
#include <entity.h>
#include <gutils.h>

#include <ship.h>
#include <target.h>

class Game 
{
public:
    Game();
    ~Game();

    void createWindow(unsigned w, unsigned h);

    void start();
    void restart();
private:
    GLFWwindow* window;
    float delta, currentTime, pastTime;

    void updateDelta();
    void mainLoop();
};

#endif
