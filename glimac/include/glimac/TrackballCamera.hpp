#pragma once

#include <iostream>

namespace glimac {
class TrackballCamera {
    private :
    
        float m_fDistance; // Distance from the center of the scene
        float m_fAngleX; // Angle made by the camera around the x axis of the scene
        float m_fAngleY; // angle made by the camera around the y axis of the scene
        bool m_camera_fps = false; // camera mode
        bool m_locked = false; // camera locked mode
    public :

        TrackballCamera();
        ~TrackballCamera();

        void moveFront(float delta);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        void changeMode();
        inline bool modefps() const {return m_camera_fps;}
        inline void lock() {m_locked = !m_locked;}
        glm::mat4 getViewMatrix() const;
};
}
