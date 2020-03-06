#ifndef CAMERA_H
#define CAMERA_H

#include <gui>

using namespace std;
using namespace glm;

namespace engine {

	class Camera {
	public:
		static float MaxZoom;
		static float MovementSpeed;
		static float GetXPosition() { return position.x; }
		static float GetYPosition() { return position.y; }
		static void Init(vec3 startPosition, vec3 startUp, GLfloat startYaw, GLfloat startPitch);
		static void mouseControl(float threshold);
		static void keyboardControl();
		static void GoToPoint(GLfloat x, GLfloat y);
		static mat4 calculateViewMatrix();
	private:
		Camera();
		~Camera();
		static vec3 position;
		static vec3 front, up, right, worldUp;
		static GLfloat yaw, pitch;
		static float threshold_x, threshold_y, abs_x, abs_y;
		
		static void update();
	};
};

#endif