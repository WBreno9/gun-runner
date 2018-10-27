#include <camera.h>

constexpr float radPi = 0.0174532925199432f;

float camX = 0.0f, camY = 0.0f, camZ = 5.0;
float camYaw = 0.0f;
float camPitch = 0.0f;

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

inline void Camera::LockCamera()
{

    if(camPitch > 90) 
        camPitch = 90;

    if(camPitch < -90) 
        camPitch = -90;

    if(camYaw  < 0.0f) 
        camYaw += 360.0f;

    if(camYaw > 360.0f) 
        camYaw -= 360.0f;
}

void Camera::MoveCamera(float distance, float direction)
{
    float rad = (camYaw+ direction) * radPi;

    camX -=  sin(rad) * distance;
    camZ -=  cos(rad) * distance;
}

void Camera::MoveCameraUp(float distance, float direction)
{
    float rad = (camPitch + direction) * radPi;

    camY += sin(rad) * distance;
}

void Camera::Control(float moveVelocity, float mouseVelocity, 
                    bool mouseIn, 
                    GLFWwindow* window, 
                    int width, int height)
{
    int midX = width/2;
    int midY = height/2;

    double tmpX, tmpY;

    glfwGetCursorPos(window, &tmpX, &tmpY);

    camYaw += mouseVelocity* (midX - tmpX);
    camPitch += mouseVelocity* (midY - tmpY);


    Camera::LockCamera();


    int wState = glfwGetKey(window, GLFW_KEY_W);
    int aState = glfwGetKey(window, GLFW_KEY_A);
    int sState = glfwGetKey(window, GLFW_KEY_S);
    int dState = glfwGetKey(window, GLFW_KEY_D);

    if(wState == GLFW_PRESS)
    {
        if(camPitch != 90 && camPitch != -90)
        {
            MoveCamera(moveVelocity, 0.0f);
            MoveCameraUp(moveVelocity, 0.0f);
        }
    }

    if(sState == GLFW_PRESS){
        if(camPitch != 90 && camPitch != -90)
        {
            MoveCamera(moveVelocity, 180.0f);
            MoveCameraUp(moveVelocity, 180.0f);
        }
    }

    if(aState == GLFW_PRESS){
        if(camPitch != 90 && camPitch != -90)
        {
            MoveCamera(moveVelocity, 90.0f);
        }
    }
    else if(dState == GLFW_PRESS){
        MoveCamera(moveVelocity, 270.0f);
    }

    glRotatef(-camPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
}

void Camera::UpdateCamera(){
    glTranslatef(-camX, -camY, -camZ);
}

const glm::mat4 Camera::GetViewMatrix()
{
        return view;
}

const glm::mat4 Camera::GetProjMatrix()
{
        return proj;
}
