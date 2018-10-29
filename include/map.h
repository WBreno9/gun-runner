#ifndef MAP_H
#define MAP_H

#include <entity.h>

class EMap : public Entity {
public:
        EMap(std::string name);
        ~EMap();

        std::string GetTypeString() { return "map"; }

        virtual void Update();
private:
        std::string m_name;
};

#endif