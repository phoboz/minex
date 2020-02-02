// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "element.h"

// ---------------------------------------------------------------------------

void give_element(
	struct element *elmnt,
	struct element **head
	)
{
	// Add element to list
	if (head)
	{
		elmnt->prev = 0;
		elmnt->next = *head;
		*head = elmnt;

		if (elmnt->next != 0)
		{
			elmnt->next->prev = elmnt;
		}
	}
	else
	{
		elmnt->prev = 0;
		elmnt->next = 0;
	}
}

void take_element(
	struct element *elmnt,
	struct element **head
	)
{
	// Remove element from list
	if (head)
	{
		if (elmnt->prev != 0)
		{
			elmnt->prev->next = elmnt->next;
		}

		if (elmnt->next != 0)
		{
			elmnt->next->prev = elmnt->prev;
		}

		if (*head == elmnt)
		{
			*head = elmnt->next;
		}
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
