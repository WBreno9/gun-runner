#include <box.h>

EBox::EBox(float scale, float mass) {
        modelHandle = m_renderer->findModel("box");
        modelActive = true;
        visible = true;

        transform.scale(glm::vec3(scale), Transform::WORLD_RELATIVE);

        btCollisionShape* shape = new btBoxShape(btVector3(1.f, 1.f, 1.f) * scale);
        rigidBody = m_physics->createRigidBody(shape, &transform, false, mass);
}

EBox::~EBox() {

}

void EBox::Update() {
        std::clog << glm::to_string(transform.m_pos) << std::endl;
}