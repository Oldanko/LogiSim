#include "Controls.h"

glm::vec3 rotateY(glm::vec3 v, GLfloat a)
{
	return glm::vec3(
		cos(a)*v.x + sin(a)*v.z,
		v.y,
		-sin(a)*v.x + cos(a)*v.z
		);
}

KeyifPressed<ushort> cameraControls_default[] = {
	KeyifPressed<ushort>(GLFW_KEY_W,			CAMERA_MOVE_FORWARD),
	KeyifPressed<ushort>(GLFW_KEY_S,			CAMERA_MOVE_BACK),
	KeyifPressed<ushort>(GLFW_KEY_A,			CAMERA_MOVE_LEFT),
	KeyifPressed<ushort>(GLFW_KEY_D,			CAMERA_MOVE_RIGHT),
	KeyifPressed<ushort>(GLFW_KEY_UP,			CAMERA_ROTATE_UP),
	KeyifPressed<ushort>(GLFW_KEY_DOWN,			CAMERA_ROTATE_DOWN),
	KeyifPressed<ushort>(GLFW_KEY_LEFT,			CAMERA_ROTATE_LEFT),
	KeyifPressed<ushort>(GLFW_KEY_RIGHT,		CAMERA_ROTATE_RIGHT),
	KeyifPressed<ushort>(GLFW_KEY_KP_ADD,		CAMERA_ZOOM_IN),
	KeyifPressed<ushort>(GLFW_KEY_KP_SUBTRACT,	CAMERA_ZOOM_OUT),
};

KeyPressed<ubyte> actorControls_default[] = {
	KeyPressed<ubyte>(GLFW_KEY_SPACE,	ACTOR_STAND),
	KeyPressed<ubyte>(GLFW_KEY_C,		ACTOR_CROUCH),
	KeyPressed<ubyte>(GLFW_KEY_Z,		ACTOR_PRONE),
	KeyPressed<ubyte>(GLFW_KEY_B,		ACTOR_SHOOT)
};

KeyifPressed<ushort>* Controls::m_cameraControlsKeys = cameraControls_default;
KeyPressed<ubyte>* Controls::m_actorControlsKeys = actorControls_default;
ushort Controls::m_camera = 0;
ubyte Controls::m_actor = 0;
ubyte Controls::m_mouseInputs = 0;
byte Controls::m_zoom = 0;


MouseButton Controls::LMB;
MouseButton Controls::RMB;
MouseButton Controls::MMB;

double Controls::m_x;
double Controls::m_y;


void Controls::init()
{
	LMB.id = GLFW_MOUSE_BUTTON_LEFT;
	RMB.id = GLFW_MOUSE_BUTTON_RIGHT;
	MMB.id = GLFW_MOUSE_BUTTON_MIDDLE;
	
	LMB.isPressed = false; 
	RMB.isPressed = false; 
	MMB.isPressed = false;


	glfwSetScrollCallback(WindowManager::window(), scrollCallback);
}

ushort Controls::cameraControls()
{
	return m_camera;
}

ubyte Controls::actorControls()
{
	return m_actor;
}

ubyte Controls::mouseInputs()
{
	return m_mouseInputs;
}

void Controls::update()
{
	// =================Keyboard Input=================

	//Camera Controls
	m_camera = 0;

	for (int i = 0; i < 10; i++)
		if (glfwGetKey(WindowManager::window(), m_cameraControlsKeys[i].m_id) == GLFW_PRESS)
			m_camera |= m_cameraControlsKeys[i].m_action;

	//Actor Controls
	m_actor = 0;

	for (int i = 0; i < 4; i++)
	{
		if (glfwGetKey(WindowManager::window(), m_actorControlsKeys[i].m_id) == GLFW_PRESS)
		{
			if (!m_actorControlsKeys[i].isPressed)
			{
				m_actor |= m_actorControlsKeys[i].m_action;
			}
			m_actorControlsKeys[i].isPressed = true;
		}
		else
			m_actorControlsKeys[i].isPressed = false;
	}

	// =================Mouse Input=================
	
	m_mouseInputs = 0;

	glfwGetCursorPos(WindowManager::window(), &m_x, &m_y);

	// Left Mouse Button
	if (glfwGetMouseButton(WindowManager::window(), LMB.id))
	{
		if (LMB.isPressed == false)
		{
			LMB.coords = LMB.oldCoords = LMB.clickCoords = glm::vec2(m_x, m_y);
			m_mouseInputs |= LMB_PRESS;
			LMB.isPressed = true;
		}
		m_mouseInputs |= LMB_IS_PRESSED;
	}
	else
	{
		if (LMB.isPressed == true)
		{
			if (LMB.coords == glm::vec2(m_x, m_y))
				m_mouseInputs |= LMB_CLICK;
			m_mouseInputs |= LMB_RELEASE;
		}
		LMB.isPressed = false;
	}

	// Right Mouse Button
	if (glfwGetMouseButton(WindowManager::window(), RMB.id))
	{
		if (RMB.isPressed == false)
		{
			RMB.coords = RMB.oldCoords = RMB.clickCoords = glm::vec2(m_x, m_y);
			m_mouseInputs |= RMB_PRESS;
			RMB.isPressed = true;
		}
		m_mouseInputs |= RMB_IS_PRESSED;
	}
	else
	{
		if (RMB.isPressed == true)
		{
			if (RMB.coords == glm::vec2(m_x, m_y))
				m_mouseInputs |= RMB_CLICK;
			m_mouseInputs |= RMB_RELEASE;
		}
		RMB.isPressed = false;
	}

	if (glfwGetMouseButton(WindowManager::window(), MMB.id))
	{
		if (MMB.isPressed == false)
		{
			MMB.coords = MMB.oldCoords = MMB.clickCoords = glm::vec2(m_x, m_y);
		}
		else
		{
			MMB.oldCoords = MMB.coords;
			MMB.coords = glm::vec2(m_x, m_y);
		}

		if (glfwGetKey(WindowManager::window(), GLFW_KEY_LEFT_ALT))
			m_camera |= CAMERA_MOVE_MOUSE;
		else
			m_camera |= CAMERA_ROTATE_MOUSE;
		MMB.isPressed = true;
	}
	else
		MMB.isPressed = false;

	//Scrolling
	switch (m_zoom)
	{
	case 1:
		m_camera |= CAMERA_ZOOM_IN_STEP;
		break;
	case -1:
		m_camera |= CAMERA_ZOOM_OUT_STEP;
		break;
	}
	m_zoom = 0;
	
}

float Controls::x()
{
	return m_x;
}

float Controls::y()
{
	return m_y;
}

void Controls::scrollCallback(GLFWwindow * window, double x, double y)
{
	m_zoom = y;
}

glm::vec2 Controls::mouseCoords()
{
	return glm::vec2(float(m_x), float(m_y));
}

glm::vec2 Controls::mouseMMBCoords()
{
	return MMB.coords;
}

glm::vec2 Controls::mouseMMBCoordsDelta()
{
	return MMB.coords - MMB.oldCoords;
}

glm::vec2 Controls::mouseMMBClickCoords()
{
	return MMB.clickCoords;
}
