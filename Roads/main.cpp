#include "ShaderManager.h"
#include "Controls.h"
#include "WindowManager.h"
#include <ctime>
#include <vector>

#include "Roads.h"
#include "list.h"

std::vector<node*> pathfinder(node * start, node * end);

point screenToClip(point * coords);

const GLuint NODES_MAX = 30;
GLuint NODES_NUM = 0;
const GLuint ROADS_MAX = 100;
GLuint ROADS_NUM = 0;

node nodes[NODES_MAX];
road roads[ROADS_MAX];

node * selectNode(point * p)
{
	for (int i = 0; i < NODES_NUM; i++)
		if (sqrDist(p, &nodes[i].location) < 0.02*0.02)
			return &nodes[i];
	return nullptr;
}

//Road Creator
struct RoadCreator
{
	static node * start;
	static line l;
	static void init()
	{
		point p;
		p.x = 0;
		p.y = 0;
		l.init(&p, &p);
	}
	static void terminate()
	{

	}
	static void pushNode(node * n)
	{
		if (!n)
			return;

		if (start)
		{
			if (ROADS_NUM < ROADS_MAX)
			{
				setRoad(&roads[ROADS_NUM], start, n);
				ROADS_NUM++;
			}
			start = nullptr;
		}
		else
		{
			if (n->roads_num == ROADS_PER_NODE)
				return;
			start = n;
			l.setSubLineStart(&n->location);
		}
	}
	static void draw(point* coords)
	{
		if (start)
		{
			l.setSubLineEnd(coords);
			l.draw();
		}
	}
};

node *RoadCreator::start = nullptr;
line RoadCreator::l;

//pathfinder
struct waypoint
{
	static uint32_t count;
	waypoint * parent;
	waypoint * prev;
	waypoint * next;
	node* n;
	float G, H, F;

	waypoint(waypoint * pa, node * n,float g, float h)
	{
		parent = pa;
		prev = nullptr;
		next = nullptr;
		this->n = n;
		G = g;
		H = h;
		F = g + h;
		count++;
	}
	~waypoint() { count--; }

	void push(waypoint *wp)
	{
		if (next)
		{
			if (wp->F > next->F)
				next->push(wp);
			else
			{
				wp->prev = this;
				wp->next = next;
				next->prev = wp;
				next = wp;
			}
		}
		else
		{
			next = wp;
			wp->prev = this;
		}
	}
};

uint32_t waypoint::count = 0;

struct waypointList
{
	waypoint *head;

	waypointList() { head = nullptr; }
	~waypointList() 
	{
		if (head)
		{
			while (head->next)
			{
				auto target = head;
				head = head->next;
				delete target;
			}
			delete head;
		}
	}
	void push(waypoint *wp)
	{
		if (head)
		{
			if (wp->F > head->F)
				head->push(wp);
			else
			{
				wp->next = head;
				head->prev = wp;
				head = wp;
			}
		}
		else 
			head = wp;
	}

	void push_front(waypoint* wp)
	{
		if (head)
		{
			wp->next = head;
			wp->prev = nullptr;
			head->prev = wp;
			head = wp;
		}
		else
			head = wp;
	}


	waypoint * pop_front()
	{
		if (head)
		{
			auto target = head;
			head = head->next;
			if (head)
				head->prev = nullptr;
			return target;
		}
		else
			return nullptr;
	}

	waypoint * find(node * n)
	{
		auto iterator = head;
		while (iterator)
		{
			if (iterator->n == n)
				return iterator;
			iterator = iterator->next;
		}
		return nullptr;
	}

	bool check(node * n)
	{
		auto iterator = head;
		while (iterator)
		{
			if (iterator->n == n)
				return true;
			iterator = iterator->next;
		}
		return false;
	}
};

std::vector<node*> pathfinder(node* start, node* end)
{
	waypointList opened;
	std::vector<waypoint*> closed;
	
	std::vector<node*> path_l;


	opened.push(new waypoint(nullptr, start, 0, sqrDist(&start->location, &end->location)/2));
	
	do
	{
		waypoint * current = opened.pop_front();
		if (!current)
		{
			path_l.clear();
			return path_l;
		}

		current->next = nullptr;
		current->prev = nullptr;
		closed.push_back(current);
		if (current->n == end)
			break;

		for (int r = 0; r < current->n->roads_num; r++)
		{
			node * n = current->n->neighbour(r);
			for (auto a : closed)
				if (a->n == n)
					continue;

			float G = current->G + sqrt(sqrDist(&current->n->location, &n->location));
			float H = sqrt(sqrDist(&n->location, &end->location));

			auto wp = opened.find(n);
			if (wp)
			{
				if (G + H < wp->F)
				{
					wp->G = G;
					wp->H = H;
					wp->F = G + H;
					wp->parent = current;
					if (wp != opened.head)
					{
						if (wp->next)
							wp->next->prev = wp->prev;
						if (wp->prev)
							wp->prev->next = wp->next;

						wp->next = nullptr;
						wp->prev = nullptr;

						opened.push(wp);
					}
				}
				continue;
			}

			opened.push(new waypoint(current, n, G, H));
		}

	} while (true);

	auto iterator = closed.back();
	while (iterator)
	{
		path_l.push_back(iterator->n);
		iterator = iterator->parent;
	}

	for (auto a : closed)
		delete a;

	return path_l;
}

std::vector<node*> path;

node * pf_start = nullptr;
void pfPushNode(node * n)
{
	if (!n)
	{
		path.clear();
		return;
	}

	if (pf_start)
	{
		path = pathfinder(pf_start, n);
		pf_start = nullptr;
		std::cout << "Waypoints count = " << waypoint::count << "\n";
	}
	else
	{
		pf_start = n;
		path.clear();
	}
}

void main()
{
	WindowManager::init();
	Controls::init();
	ShaderManager::init();
	RoadCreator::init();
	point::init();

	srand(time(NULL));
	{
		point avg;
		avg.x = 0.0f;
		avg.y = 0.0f;

		//Set random points
		for (GLuint i = 0; i < NODES_MAX; i++)
		{
			nodes[i].location.x = (float)rand() / RAND_MAX * 1.8 - 0.9;
			nodes[i].location.y = (float)rand() / RAND_MAX * 1.8 - 0.9;
			bool reject = false;

			for (int j = 0; j < i; j++)	
			{
				if (sqrDist(&nodes[j].location, &nodes[i].location) < 0.04)	
				{
					reject = true;
					break;
				}
			}

			if (reject) 
			{
				i--;
				continue;
			}

			avg.x += nodes[i].location.x;
			avg.y += nodes[i].location.y;

			NODES_NUM++;
		}

		avg.x /= NODES_NUM;
		avg.y /= NODES_NUM;

		GLfloat dist = INFINITY;
		node * avgNode = nullptr;

		//find the closest point to avg position of all points
		for (GLuint i = 0; i < NODES_MAX; i++)
		{
			GLfloat tempDist = sqrDist(&nodes[i].location, &avg);

			if (tempDist < dist)
			{
				avgNode = &nodes[i];
				dist = tempDist;
			}
		}

		//set roads between points
		for (GLuint i = 0; i < NODES_NUM; i++)
		{
			if (&nodes[i] == avgNode)
				continue;
			dist = INFINITY;
			node* closest = avgNode;
			GLfloat distAvg = sqrDist(&avgNode->location, &nodes[i].location);
			for (int j = 0; j < NODES_NUM; j++)
			{
				if (i == j)
					continue;
				if (sqrDist(&nodes[j].location, &avgNode->location) < distAvg)
				{
					GLfloat distTemp = sqrDist(&nodes[j].location, &nodes[i].location);
					if (distTemp < dist)
					{
						dist = distTemp;
						closest = &nodes[j];
					}
				}
			}

			if(setRoad(&roads[ROADS_NUM], &nodes[i], closest)) ROADS_NUM++;
		}

	}
		
	bool roadCreator = false;

	do 
	{
		if (Controls::mouseInputs() & LMB_PRESS)
		{
			if(roadCreator)
				RoadCreator::pushNode(selectNode(&screenToClip((point*)&Controls::mouseCoords())));
			else
				pfPushNode(selectNode(&screenToClip((point*)&Controls::mouseCoords())));
		}

		if (Controls::actorControls() & ACTOR_STAND)
		{
			roadCreator = !roadCreator;
			RoadCreator::start = nullptr;
			pf_start = nullptr;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(ShaderManager::program);
		for (GLuint i = 0; i < ROADS_NUM; i++)
			roads[i].l.draw();
		RoadCreator::draw(&screenToClip((point*)&Controls::mouseCoords()));
		glUseProgram(ShaderManager::programPoint);
		glUniform3f(glGetUniformLocation(ShaderManager::programPoint, "color"), 1.0, 0.0, 0.0);
		point::bindVertexArray();
		for (GLuint i = 0; i < NODES_NUM; i++)
			nodes[i].location.draw();

		glUniform3f(glGetUniformLocation(ShaderManager::programPoint, "color"), 1.0, 1.0, 0.0);
		for (auto n : path)
			n->location.draw();

		glUniform3f(glGetUniformLocation(ShaderManager::programPoint, "color"), 0.0, 1.0, 0.0);
		if (pf_start)
			pf_start->location.draw();

		Controls::update();
	} while (WindowManager::update());

	ShaderManager::terminate();
	WindowManager::close();
}

point screenToClip(point * coords)
{
	point target;

	target.x = 2 * coords->x / WindowManager::width() - 1;
	target.y = 1 - 2 * coords->y / WindowManager::height();
	return target;
}