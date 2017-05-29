#pragma once

struct list_node
{
	static unsigned int count;
	list_node* next;
	list_node* previous;

	list_node* parent;

	void * item;
	float F;
	float G;
	float H;
	
	list_node(list_node* parent, void *item, float G, float H);
	void cut_out();
	void set_G(float G) { this->G = G; }
	void push(list_node*n);
	~list_node() { count--; }
};

struct listopen
{
	list_node* head;

	listopen()
	{
		head = nullptr;
	}
	~listopen()
	{
		if (head)
		{
			while (head->next)
			{
				head = head->next;
				delete head->previous;
			}
			delete head;
		}
	}


	list_node* check(void * item);
	void push(list_node * n);
	list_node* pop_front();
};

struct listclosed
{
	list_node* head;

	listclosed()
	{
		head = nullptr;
	}
	~listclosed()
	{
		if (head)
		{

			while (head->next)
			{
				head = head->next;
				delete head->previous; // access violation
			}
			delete head;
		}
	}

	void push(list_node * n);
	bool check(void * item);
};