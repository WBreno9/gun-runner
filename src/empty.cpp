#include <empty.h>

Empty::Empty()
{
        visible = true;
        modelHandle = m_renderer->findModel("error");
        modelActive = true;
}

Empty::~Empty()
{
}

void Empty::Update()
{
        transform.rotate(glm::vec3(0.f, delta, 0.f), Transform::WORLD_RELATIVE);
}
