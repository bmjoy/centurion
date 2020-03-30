#ifndef CAMERA_H
#define CAMERA_H

#ifndef CAMERA_THRESHOLD
#define CAMERA_THRESHOLD 20
#endif

#include <ui.h>

namespace Engine {

	class Camera {

	public:
		static float MaxZoom;
		static float MovementSpeed;
		static int GetCurrentZoom() { return currentZoom; }
		static void SetCurrentZoom(int z) { currentZoom = z; }
		static float GetZoomFactor() { return zoomCameraFactor; }
		static void SetZoomFactor(float f) { zoomCameraFactor = f; }
		static float GetXPosition() { return position.x; }
		static float GetYPosition() { return position.y; }
		static glm::vec2 GetZoomedCoords(float xCoord, float yCoord);
		static float GetYMinimapCoordinate(float y);
		static void Init(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch);
		static void mouseControl();
		static void keyboardControl();
		static void GoToPoint(GLfloat x, GLfloat y);
		static glm::mat4 calculateViewMatrix();
	private:
		Camera();
		~Camera();
		static glm::vec3 position;
		static glm::vec3 front, up, right, worldUp;
		static GLfloat yaw, pitch;
		static float threshold_x, threshold_y, abs_x, abs_y;
		static float zoomCameraFactor;
		static int currentZoom;
		static void update();
	};

};

#endif // !CAMERA_H