#ifndef _ELEMENT_H
#define _ELEMENT_H

struct element
{
	struct element *prev, *next;
};

void give_element(
	struct element *elmnt,
	struct element **head
	);

void take_element(
	struct element *elmnt,
	struct element **head
	);

#endif
