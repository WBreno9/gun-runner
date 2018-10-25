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
}
