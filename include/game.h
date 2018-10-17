#ifndef GAME_H
#define GAME_H

#include <inc.h>
#include <entity.h>
#include <camera.h>
#include <gutils.h>
#include <renderer.h>

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
        unsigned width, height;
        float delta, currentTime, pastTime;

        Camera* mainCamera;
        std::vector<Model*> models;

        void DrawAll();

        void updateDelta();
        void mainLoop();

        Renderer *renderer;
};

#endif
