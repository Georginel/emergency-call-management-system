/* NEAGU Georginel-Aurelian - 322CB */
#ifndef INTERVENTION_H
#define INTERVENTION_H

#include "unit.h"
#include "incident.h"

// Interventie
typedef struct intervention {
	TIncident incident; 
	TUnit unit;         
	
	struct intervention *prev;
	struct intervention *next;
} *TIntervention;

// Nod pentru stiva de interventii
typedef struct intervention_node {
	TIntervention intervention;
	struct intervention_node *next;
} *TInterventionNode;

// Functii ajutatoare pentru interventii
void insert_intervention_end(TIntervention sentinel, TIntervention new_intervention); // Insereaza o interventie la sfarsitul listei
void push_undo_stack(TInterventionNode *stack_top, TIntervention new_intervention); // Adauga o interventie la varful stivei de istoric

#endif
