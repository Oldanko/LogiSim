#pragma once
#include <GL\glew.h>

class ShaderManager
{
	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
public:

	static GLuint program;
	static GLuint programPoint;

	static void init();
	static void terminate();
	
};

