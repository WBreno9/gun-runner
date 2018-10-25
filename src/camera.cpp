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

        transform.pos = glm::vec3(0.f);
        transform.forward = glm::vec3(0.f, 0.f, 1.0f);
}

Camera::~Camera()
{
}

void Camera::Update()
{
        view = glm::lookAt(transform.pos, 
                        transform.forward + transform.pos, 
                        glm::vec3(0.f, 1.f, 0.f));
        proj = glm::perspective(fov, aspectRatio, near, far);
}

void Camera::LookAt(const glm::vec3& f)
{
        transform.forward = glm::normalize(f - transform.pos);
}

const glm::mat4 Camera::GetViewMatrix()
{
        return view;
}

const glm::mat4 Camera::GetProjMatrix()
{
        return proj;
}
