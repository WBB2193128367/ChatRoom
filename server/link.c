#include "server.h"
#include "package.h"


void is_malloc_ok(Link newnode)
{
	if(newnode == NULL)
	{
		perror("malloc error");
		exit(1);
	}
}


void create_node(Link * newnode)
{
	(*newnode) = (Link)malloc(sizeof(Node));
	is_malloc_ok(*newnode);
}


void insert_head(Link * head, Link newnode)
{
	if((*head) == NULL)
	{
		(*head) = newnode;
		newnode->next = NULL;
	}
	else
	{
		newnode->next = (*head);
		(*head) = newnode;
	}
}

void display_list(Link head)
{
	Link p;

	p = head;

	if(p == NULL)
	{
		printf("there is no client!\n");
	}
	else
	{
		while(p != NULL)
		{
			printf("id : %d  name : %s\n", p->id, p->name);

			p = p->next;
		}
	}
}


int find_node(Link head, int id)
{
	Link p;
	p = head;

	while(p != NULL)
	{
		if(p->id == id)
		{
			return ONLINEIN;
		}
		
		p = p->next;
	}

	return ONLINEOUT;
}


int find_online(Link head, Msg * Pmsg)
{
	Link p;
	p = head;

	while(p != NULL)
	{
		if(strncmp(p->name, Pmsg->toname, NAMESIZE) == 0)
		{
			Pmsg->fd = p->fd;
			Pmsg->revert = ONLINEIN;
			return ONLINEIN;
		}
		
		p = p->next;
	}
	Pmsg->revert = ONLINEOUT;
	return ONLINEOUT;
		
}


void delete_node(Link * head, Msg * Pmsg)
{
	if((*head) == NULL)
	{
		printf("there is no client to delete!\n");
		return;
	}

	Link p, q;

	p = q = (*head);

	if((*head)->id == Pmsg->id)
	{
		(*head) = (*head)->next;
		free(p);
	}
	else
	{
		while(q->id != Pmsg->id && q != NULL)
		{
			p = q;
			q = q->next;
		}

		p->next = q->next;
		free(q);
	}
}