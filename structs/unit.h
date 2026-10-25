/* NEAGU Georginel-Aurelian - 322CB */
#ifndef UNIT_H
#define UNIT_H

// Echipaj de interventie
typedef struct unit {
	int id;               
	char type;            
	int availability;     
} *TUnit;

// Nod pentru lista de echipaje
typedef struct unit_node {
	TUnit unit;
	struct unit_node *next;
} *TUnitNode;

// Functii ajutatoare pentru echipaje
void enqueue_unit(TUnitNode *queue_head, TUnit new_unit); // Adauga un echipaj la finalul cozii

#endif