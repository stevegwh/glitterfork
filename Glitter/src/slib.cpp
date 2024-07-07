//
// Created by Steve Wheeler on 10/04/2024.
//

#include "slib.hpp"
#include <SDL.h>
#include <glm/gtc/matrix_transform.hpp>

namespace slib
{

    glm::mat4 fpsviewGl( const glm::vec3& eye, float pitch, float yaw )
    {
        pitch = glm::radians(pitch);
        yaw = glm::radians(yaw);
        float cosPitch = cos(pitch);
        float sinPitch = sin(pitch);
        float cosYaw = cos(yaw);
        float sinYaw = sin(yaw);
        
        glm::vec3 xaxis = { cosYaw, 0, -sinYaw };
        glm::vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
        glm::vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
        
        glm::mat4 viewMatrix = {
            {       xaxis.x,            yaxis.x,            zaxis.x,      0 },
            {       xaxis.y,            yaxis.y,            zaxis.y,      0 },
            {       xaxis.z,            yaxis.z,            zaxis.z,      0 },
            { -glm::dot( xaxis, eye ), -glm::dot( yaxis, eye ), -glm::dot( zaxis, eye ), 1 }
        };
        
        return viewMatrix;
    }
    
    collisionInfo CheckRayBoxCollision(ray ray, slib::boundingBox bb)
    {
        return {};
    }

    glm::vec3 ScreenToWorldRay(double xpos, double ypos, glm::mat4 view, glm::mat4 projection, unsigned SCR_WIDTH, unsigned SCR_HEIGHT) 
    {
        return glm::unProject(glm::vec3(xpos, ypos, 1.0),
                       view, projection,
                       glm::vec4(0,0,SCR_WIDTH, SCR_HEIGHT));
    }
    
    ray GetMouseRay(double xpos, double ypos, glm::mat4 view, glm::mat4 projection, unsigned SCR_WIDTH, unsigned SCR_HEIGHT)
    {
        ray out;
        // out.position = camera position
        // out.direciton = ScreenToWorldRay
    }

    bool CheckPointBoxCollision(glm::vec3 point, boundingBox box)
    {
        return  point.x >= box.min.x &&
                point.x <= box.max.x &&
                point.y >= box.min.y &&
                point.y <= box.max.y &&
                point.z >= box.min.z &&
                point.z <= box.max.z;
    }

}