#pragma once
#include "WindowManager.h"
#include <glm\glm.hpp>
typedef unsigned short ushort;
typedef unsigned char ubyte;
typedef char byte;

/*Should Not be in this class*/
glm::vec3 rotateY(glm::vec3 v, GLfloat a);

const ushort CAMERA_MOVE_FORWARD	= 0b0000000000000001;
const ushort CAMERA_MOVE_BACK		= 0b0000000000000010;
const ushort CAMERA_MOVE_LEFT		= 0b0000000000000100;
const ushort CAMERA_MOVE_RIGHT		= 0b0000000000001000;

const ushort CAMERA_ROTATE_LEFT		= 0b0000000000010000;
const ushort CAMERA_ROTATE_RIGHT	= 0b0000000000100000;
const ushort CAMERA_ROTATE_UP		= 0b0000000001000000;
const ushort CAMERA_ROTATE_DOWN		= 0b0000000010000000;

const ushort CAMERA_ZOOM_IN			= 0b0000000100000000;
const ushort CAMERA_ZOOM_OUT		= 0b0000001000000000;

const ushort CAMERA_ZOOM_IN_STEP	= 0b0000010000000000;
const ushort CAMERA_ZOOM_OUT_STEP	= 0b0000100000000000;
const ushort CAMERA_ROTATE_MOUSE	= 0b0001000000000000;
const ushort CAMERA_MOVE_MOUSE		= 0b0010000000000000;

const ubyte ACTOR_STAND				= 0b00000001;
const ubyte ACTOR_CROUCH			= 0b00000010;
const ubyte ACTOR_PRONE				= 0b00000100;
const ubyte ACTOR_SHOOT				= 0b00001000;


const ubyte LMB_PRESS				= 0b00000001;
const ubyte LMB_RELEASE				= 0b00000010;
const ubyte LMB_CLICK				= 0b00000100;
const ubyte LMB_IS_PRESSED			= 0b00001000;
const ubyte RMB_PRESS				= 0b00010000;
const ubyte RMB_RELEASE				= 0b00100000;
const ubyte RMB_CLICK				= 0b01000000;
const ubyte RMB_IS_PRESSED			= 0b10000000;


template<class T>struct KeyifPressed
{
	int m_id;
	T m_action;
	KeyifPressed(int id, T action) : m_id(id), m_action(action) {}
};

template<class T>struct KeyPressed
{
	int m_id;
	T m_action;
	bool isPressed;
	KeyPressed(int id, T action) : m_id(id), m_action(action) {}
};

struct MouseButton
{
	glm::vec2 oldCoords;
	glm::vec2 coords;
	glm::vec2 clickCoords;
	int id;
	bool isPressed;
};

class Controls
{
	static MouseButton LMB;
	static MouseButton RMB;
	static MouseButton MMB;

	static double m_x, m_y; // mouse coordinates
	static KeyifPressed<ushort>* m_cameraControlsKeys;
	static KeyPressed<ubyte>* m_actorControlsKeys;
	static ushort m_camera;
	static ubyte m_actor;
	static ubyte m_mouseInputs;

	static byte m_zoom;

public:
	static void init(); // read from file
	static ushort cameraControls();
	static ubyte actorControls();
	static ubyte mouseInputs();
	static void update();

	static float x();
	static float y();

	static void scrollCallback(GLFWwindow * window, double x, double y);
	static glm::vec2 mouseCoords();
	static glm::vec2 mouseMMBCoords();
	static glm::vec2 mouseMMBCoordsDelta();
	static glm::vec2 mouseMMBClickCoords();
};

