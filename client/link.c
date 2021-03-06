#include "client.h"
#include "package.h"


static void is_malloc_ok(Link newnode)
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
		printf("\t----在线列表----\n");
		while(p != NULL)
		{
			printf("\t--%s (%d)--\n", p->name, p->id);

			p = p->next;
		}
		printf("\t---------------\n");
	}
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