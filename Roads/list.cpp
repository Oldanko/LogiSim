#include "list.h"

unsigned int list_node::count = 0;

list_node::list_node(list_node * parent, void * item, float G, float H)
{
	this->parent = parent;
	this->G = G;
	this->H = H;
	this->item = item;

	this->F = G + H;

	next = nullptr;
	previous = nullptr;

	count++;
}

void list_node::cut_out()
{
	if (previous)
		previous->next = next;
	if(next)
		next->previous = previous;

	previous = nullptr;
	next = nullptr;
	
}

void list_node::push(list_node * n) // stack overflow
{
	if (next)
	{
		if (next->F < n->F)
			next->push(n);
	}
	else
	{
		n->next = next;
		n->previous = this;
		if(next)
			next->previous = n;
			
		next = n;
	}
}

list_node * listopen::check(void * item)
{
	list_node * iterator = head;
	while(iterator)
	{
		if (iterator->item = item)
			return iterator;
		iterator = iterator->next;
	};
	return nullptr;
}

void listopen::push(list_node * n)
{
	if (head)
	{
		if (head->F > n->F)
		{
			n->next = head;
			head->previous = n;
			head = n;
		}
		else
			head->push(n);

	}
	else
		head = n;
}

list_node * listopen::pop_front()
{
	if (head)
	{
		list_node * front = head;
		head = head->next;
		if(head)
			head->previous = nullptr;
		front->next = nullptr;
		return front;
	}
	else
		return nullptr;
}

void listclosed::push(list_node * n)
{
	if (head)
	{
		head->previous = n;
		n->next = head;
		head = n;
	}
	else
		head = n;
}

bool listclosed::check(void * item)
{
	list_node * iterator = head;
	while (iterator)
	{
		if (iterator->item == item)
		{
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}
