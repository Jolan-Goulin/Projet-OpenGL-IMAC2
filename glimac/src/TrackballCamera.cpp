

#include <glimac/SDLWindowManager.hpp>
#include "glimac/TrackballCamera.hpp"

TrackballCamera::TrackballCamera()
	:m_fDistance(-7.0f), m_fAngleX(25.0f), m_fAngleY(0.0f)
{}

TrackballCamera::~TrackballCamera() 
{}

void TrackballCamera::moveFront(const float delta) {
	// check if camera is locked
	if (m_camera_fps == false && m_locked == false){
		// prevent user to zoom into the earth (when m_fDistance < -1.0)
		m_fDistance = std::min(m_fDistance + delta, -2.0f);
	}
}

void TrackballCamera::rotateLeft(const float degrees) {
	// check if camera is locked
	if (m_camera_fps == false && m_locked == false){
		m_fAngleX += degrees;
	}
}

void TrackballCamera::rotateUp(const float degrees) {
	// check if camera is locked
	if( m_locked == false){
		if (m_camera_fps == false){
			m_fAngleY += degrees;
		}
		else{
			if(degrees <0){
				// camera is in fps mode, the angle can't be over 45° to be realistic
				m_fAngleY = std::max( m_fAngleY+degrees, -45.0f);
			}
			else{
				m_fAngleY = std::min( m_fAngleY+degrees, 45.0f);
			}
			
		}
	}
}

void TrackballCamera::changeMode(){
	// put the camera in or out of the fps mode and change settings to matche the new mode
	// check if camera is locked
	if(m_locked == false){
		if (m_camera_fps == true){
			// if camera was in mode fps, use tps settings
			m_fDistance = -7.0f;
			m_fAngleX = 25.0f;
			m_fAngleY = 0.0f;
			m_camera_fps = false;
		}
		else {
			// if camera was in mode tps, use fps settings
			m_fDistance = -1.0f;
			m_fAngleX = 10.0f;
			m_fAngleY = 0.0f;
			m_camera_fps = true;
		}
	}
}

glm::mat4 TrackballCamera::getViewMatrix() const {
    // Declare a ViewMatrix
	glm::mat4 ViewMatrix = glm::mat4();
	
	// Move the scene back along the z axis (distance: m_fDistance)
	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 0, m_fDistance));
	
	// Rotate the scene around the x axis (angle: m_fAngleX)
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_fAngleX), glm::vec3(1, 0, 0));
	
	// Rotate the scene around the y axis (angle: m_fAngleY)
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_fAngleY), glm::vec3(0, 1, 0));
	
	return ViewMatrix;
}