#include <player.h>

EPlayer::EPlayer(Camera* cam) : 
        m_cam(cam),
        m_mouseSpeed(0.2f),
        m_moveSpeed(10.f),
        m_angleX(0.f),
        m_angleY(0.f)
{
        m_cam->transform = transform;
}

EPlayer::~EPlayer() {

}

void EPlayer::inputMouse() {
        double mouseX, mouseY;
        int32_t winW, winH;

        glfwGetWindowSize(Entity::window, &winW, &winH);
        glfwGetCursorPos(Entity::window, &mouseX, &mouseY);

        float angleDeltaX = m_mouseSpeed * delta * float(winW/2 - mouseX);
        float angleDeltaY = m_mouseSpeed * delta * float(winH/2 - mouseY);

        if (std::abs(m_angleY + angleDeltaY) < glm::pi<float>()/2.f - 0.01f) {
                transform.rotate(glm::vec3(angleDeltaY, 0.f, 0.f), Transform::SELF_RELATIVE);
                m_angleY += angleDeltaY;
        }

        m_angleX += angleDeltaX;

        transform.rotate(glm::vec3(0.f, angleDeltaX, 0.f), Transform::WORLD_RELATIVE);
        
        glfwSetCursorPos(Entity::window, winW/2, winH/2);


        if(glfwGetMouseButton(Entity::window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                glm::vec3 forward = transform.getForward();
        }
        if(glfwGetMouseButton(Entity::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        }
}

void EPlayer::inputKeyboard() {
        float right = 0.f;
        float forward = 0.f;

        if (glfwGetKey(Entity::window, GLFW_KEY_W) == GLFW_PRESS) forward += delta * m_moveSpeed;
        if (glfwGetKey(Entity::window, GLFW_KEY_S) == GLFW_PRESS) forward -= delta * m_moveSpeed;
        if (glfwGetKey(Entity::window, GLFW_KEY_D) == GLFW_PRESS) right   += delta * m_moveSpeed;
        if (glfwGetKey(Entity::window, GLFW_KEY_A) == GLFW_PRESS) right   -= delta * m_moveSpeed;

        transform.translate(glm::vec3(right, 0, forward), Transform::SELF_RELATIVE);
}

void EPlayer::Update() {
        inputMouse();
        inputKeyboard();

        m_cam->transform = transform;
}
