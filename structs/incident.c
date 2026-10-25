/* NEAGU Georginel-Aurelian - 322CB */
#include <stdlib.h>
#include "incident.h"


// Insereaza un incident la finalul listei dublu inlantuite circulare cu santinela
void insert_incident_end(TIncident sentinel, TIncident new_incident) {
	if (!sentinel || !new_incident) 
		return;

	// Introducem noul nod intre fostul ultim element si santinela
	TIncident last = sentinel->prev;
	
	new_incident->next = sentinel;  
	new_incident->prev = last;            
	last->next = new_incident;            
	sentinel->prev = new_incident;  
}

// Adauga un incident la finalul cozii de asteptare
void enqueue_incident(TIncidentNode *queue_head, TIncident new_incident) {
	TIncidentNode new_incident_node = malloc(sizeof(struct incident_node));
	if (!new_incident_node) 
		return;
	
	new_incident_node->incident = new_incident;
	new_incident_node->next = NULL;

	// Daca coada e goala, nodul devine capul acesteia
	if (*queue_head == NULL) {
		*queue_head = new_incident_node;
	} else {
		// Daca coada nu este goala, parcurgem pana la ultimul element si legam noul nod
		TIncidentNode current = *queue_head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_incident_node;
	}
}

// Adauga un incident la inceputul cozii
void push_incident_front(TIncidentNode *queue_head, TIncident new_incident) {
	TIncidentNode new_incident_node = malloc(sizeof(struct incident_node));
	if (!new_incident_node) return;
	
	// Noul nod devine noul cap al cozii
	new_incident_node->incident = new_incident;
	new_incident_node->next = *queue_head;
	*queue_head = new_incident_node;
}
