/* NEAGU Georginel-Aurelian - 322CB */
#include <stdlib.h>
#include "unit.h"

// Adauga un echipaj la finalul cozii simplu inlantuite de disponibilitate
void enqueue_unit(TUnitNode *queue_head, TUnit new_unit) {
	TUnitNode new_node = malloc(sizeof(struct unit_node));
	if (!new_node) return;
	
	new_node->unit = new_unit;
	new_node->next = NULL;

	// Daca coada e goala, noul nod devine capul acesteia
	if (*queue_head == NULL) {
		*queue_head = new_node;
	} else {
		// Daca coada nu este goala, parcurgem pana la ultimul element si legam noul nod
		TUnitNode current = *queue_head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
}