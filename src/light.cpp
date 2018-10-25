#include <light.h>

ELight::ELight() {
        m_light = m_renderer->addLight(RLight(glm::vec3(0.f), glm::vec3(1.0f)));
}

ELight::~ELight() {
        m_light->m_isActive = false;
}

void ELight::Update() {
        m_light->m_pos = transform.pos;
        m_light->m_modified = true;
}

void ELight::setColor(glm::vec3 color) {
        m_light->m_color = color;
        m_light->m_modified = true;
}