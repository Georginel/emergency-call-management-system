/* NEAGU Georginel-Aurelian - 322CB */
#ifndef INCIDENT_H
#define INCIDENT_H

// Incident
typedef struct incident {
	int id;               
	char priority[7];     
	char *description;    
	char status[11];      
	
	struct incident *prev;
	struct incident *next;
} *TIncident;

// Nod pentru cozile de incidente
typedef struct incident_node {
	TIncident incident;
	struct incident_node *next;
} *TIncidentNode;

// Functii ajutatoare pentru incidente
void insert_incident_end(TIncident sentinel, TIncident new_incident); // Insereaza un incident la sfarsitul listei
void enqueue_incident(TIncidentNode *queue_head, TIncident new_incident); // Adauga un incident la sfarsitul cozii
void push_incident_front(TIncidentNode *queue_head, TIncident new_incident); // Adauga un incident la inceputul cozii

#endif
