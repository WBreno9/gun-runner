#ifndef BOX_H
#define BOX_H

#include <entity.h>

class EBox : public Entity {
public:
        EBox(float scale, float mass);
        ~EBox();

        std::string GetTypeString() { return "box"; }

        void Update();
private:
};

#endif
