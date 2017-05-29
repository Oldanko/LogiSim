#ifndef ROADS_H
#define ROADS_H

#include <GL\glew.h>

struct point;

struct line
{
	GLuint vao, vbo;
	void init(point *start, point *end);
	void setSubLineStart(point * start);
	void setSubLineEnd(point * end);
	void draw();
	~line();
};

struct point
{
	static GLuint vao, vbo;
	static void init();
	static void bindVertexArray();
	void draw();
	GLfloat x, y;
} ;

GLfloat sqrDist(point * a, point * b);

struct road;
struct node;

const int ROADS_PER_NODE = 8;

struct road {
	line l;
	node * a;
	node * b;
};

struct node {
	road * roads[ROADS_PER_NODE];
	point location;
	unsigned int roads_num = 0;
	node * neighbour(unsigned int n);
};

bool setRoad(road * r, node * a, node * b);

#endif // ROADS_H
