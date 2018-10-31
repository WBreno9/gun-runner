#ifndef GAME_H
#define GAME_H

#include <inc.h>
#include <entity.h>
#include <camera.h>
#include <renderer.h>
#include <physics.h>
#include <box.h>

#define MAP_SIZE 35

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
        void DrawMap(int boxSize, int modelFootPrint, int interval, float frequence);
        void DrawMap(int boxSize, int interval, float frequence);

        void TallMap(int boxSize, EBox* box, int x, int y, int z);
        void FlatMap(int boxSize, EBox* box, int x, int y, int z);

        void updateDelta();
        void mainLoop();

        Renderer *renderer;
        Physics *physics;
};

#endif
