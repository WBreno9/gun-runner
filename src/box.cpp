#include <box.h>

EBox::EBox(float scale, float mass, glm::vec3 pos) {
        modelHandle = m_renderer->findModel("box");
        modelActive = true;
        visible = true;

        transform.setScale(glm::vec3(scale));
        transform.setPos(pos);

        btCollisionShape* shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f) * scale);

        rigidBody = m_physics->createRigidBody(shape, false, mass, &transform);
}

EBox::~EBox() {

}

void EBox::Update() {
        //std::clog << glm::to_string(transform.m_pos) << std::endl;
}