#include <empty.h>

Empty::Empty(std::string modelName)
{
        visible = true;
        modelHandle = m_renderer->findModel(modelName);
        modelActive = true;
}

Empty::~Empty()
{
}

void Empty::Update()
{
        //transform.rotate(glm::vec3(0.f, delta, 0.f), Transform::WORLD_RELATIVE);
}
