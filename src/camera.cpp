#include <camera.h>

Camera::Camera(float f, float ar, float np, float fp) : 
        fov(f), aspectRatio(ar), near(np), far(fp) 
{
        if ((far || near || aspectRatio || fov) <= 0.f) {
                std::clog << "ERROR: "
                          << "invalid camera constructor arguments"
                          << std::endl;

                return;
        }

        transform.m_pos = glm::vec3(0.f);
        transform.m_forward = glm::vec3(0.f, 0.f, 1.0f);
}

Camera::~Camera()
{
}

void Camera::Update()
{
        updateMatrices();
}

void Camera::updateMatrices() {
        view = glm::lookAt(transform.m_pos, 
                        transform.m_forward + transform.m_pos, 
                        glm::vec3(0.f, 1.f, 0.f));
        proj = glm::perspective(fov, aspectRatio, near, far);
}

void Camera::LookAt(const glm::vec3& f)
{
        transform.m_forward = glm::normalize(f - transform.m_pos);
}

const glm::mat4 Camera::GetViewMatrix()
{
        return view;
}

const glm::mat4 Camera::GetProjMatrix()
{
        return proj;
}
