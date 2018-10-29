#include <map.h>

EMap::EMap(std::string name) : m_name(name) {
        modelHandle = m_renderer->findModel(m_name);
        modelActive = true;
        visible = true;
}

EMap::~EMap() {

}

void EMap::Update() {

}