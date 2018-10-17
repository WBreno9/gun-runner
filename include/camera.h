#ifndef CAMERA_H
#define CAMERA_H

#include <entity.h>

class Camera : public Entity
{
public:
        Camera(float f, float ar, float np, float fp);
        ~Camera();

        std::string GetTypeString() { return "Camera"; }

        void Update();

        void LookAt(const glm::vec3& f);

        const glm::mat4 GetMatrix(); 
private:
        float fov;
        float aspectRatio;
        float near;
        float far;

        glm::mat4 view;
        glm::mat4 proj;
};

#endif 
