#ifndef LIGHT_ENTITY_H
#define LIGHT_ENTITY_H

#include <entity.h>

class ELight : public Entity {
public:
        ELight();
        ~ELight();

        std::string GetTypeString() { return "light"; }

        void Update();

        void setColor(glm::vec3 color);
private:
        RLight* m_light;
};

#endif