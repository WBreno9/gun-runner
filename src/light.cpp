#include <light.h>

ELight::ELight() {
        m_light = m_renderer->addLight(RLight(glm::vec3(0.f), glm::vec3(1.0f)));
}

ELight::~ELight() {
        m_light->m_isActive = false;
}

void ELight::Update() {
        m_light->setPos(transform.m_pos);
}

void ELight::setColor(glm::vec3 color) {
        m_light->setColor(color);
}