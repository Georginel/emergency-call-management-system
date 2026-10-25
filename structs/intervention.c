/* NEAGU Georginel-Aurelian - 322CB */
#include <stdlib.h>
#include "intervention.h"

// Insereaza o interventie la finalul listei dublu inlantuite circulare cu santinela
void insert_intervention_end(TIntervention sentinel, TIntervention new_intervention) {
	if (!sentinel || !new_intervention) return;

	// Introducem noul nod intre fostul ultim element si santinela
	TIntervention last = sentinel->prev;
	
	new_intervention->next = sentinel;
	new_intervention->prev = last;
	last->next = new_intervention;
	sentinel->prev = new_intervention;
}

// Adauga o interventie in varful stivei de istoric
void push_undo_stack(TInterventionNode *stack_top, TIntervention new_intervention) {
	TInterventionNode new_node = malloc(sizeof(struct intervention_node));
	if (!new_node) return;

	// Noul nod devine varful stivei
	new_node->intervention = new_intervention;
	new_node->next = *stack_top; 
	*stack_top = new_node;       
}
