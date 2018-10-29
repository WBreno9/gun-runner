#ifndef PLAYER_H
#define PLAYER_H

#include <camera.h>

class EPlayer : public Entity {
public:
        EPlayer(Camera* cam);
        ~EPlayer();

        std::string GetTypeString() { return "Player"; }

        virtual void Update();

private:
        Camera* m_cam;

        float m_angleX;
        float m_angleY;
        
        float m_mouseSpeed;
        float m_moveSpeed;

        void inputMouse();
        void inputKeyboard();
};

#endif