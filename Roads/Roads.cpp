#include "Roads.h"
#include "ShaderManager.h"

GLfloat sqrDist(point * a, point * b)
{
	return (a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y);
}

void line::init(point * start, point * end)
{
	GLfloat vertices[] =
	{
		start->x, start->y, end->x, end->y
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}

void line::setSubLineStart(point * start)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(GLfloat), &start->x);
}

void line::setSubLineEnd(point * end)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat), 2 * sizeof(GLfloat), &end->x);
}

void line::draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 2);
}

line::~line()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

GLuint point::vao;
GLuint point::vbo;

void point::init()
{
	GLfloat vertices[] =
	{
		-0.02, -0.02,
		-0.02,  0.02,
		0.02,  0.02,

		-0.02, -0.02,
		0.02,  0.02,
		0.02, -0.02,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}

void point::bindVertexArray()
{
	glBindVertexArray(vao);
}

void point::draw()
{
	glUniform2f(glGetUniformLocation(ShaderManager::programPoint, "offset"), x, y);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool setRoad(road * r, node * a, node * b)
{
	if (a->roads_num < ROADS_PER_NODE && b->roads_num < ROADS_PER_NODE)
	{
		r->a = a;
		r->b = b;
		r->l.init(&a->location, &b->location);

		a->roads[a->roads_num] = r;
		a->roads_num++;
		b->roads[b->roads_num] = r;
		b->roads_num++;
		return true;
	}
	return false;
}

node * node::neighbour(unsigned int n)
{
	if (n < roads_num)
		return (roads[n]->a == this) ? roads[n]->b : roads[n]->a;
	else
		return nullptr;
}
