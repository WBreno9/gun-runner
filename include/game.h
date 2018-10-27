#ifndef GAME_H
#define GAME_H

#include <inc.h>
#include <entity.h>
#include <camera.h>
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

        //Camera* mainCamera;
        Camera* playerCam;
        std::vector<Model*> models;

        void DrawAll();
        void DrawAll(glm::mat4 v, glm::mat4 p);

        void updateDelta();
        void mainLoop();

        Renderer *renderer;
};

#endif
