/* NEAGU Georginel-Aurelian - 322CB */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "unit.h"
#include "incident.h"
#include "intervention.h"

// Sistemul de gestionare a apelurilor
typedef struct system {
	struct unit *units; // Am pastrat struct unit* pentru a putea folosi alocarea dinamica
	int units_number; 

	// Santinelele pentru listele dublu inlantuite
	TIncident incidents;           
	TIntervention interventions;   
	
	// Cozile de așteptare pentru incidente
	TIncidentNode queue_high;
	TIncidentNode queue_medium;
	TIncidentNode queue_low;
	
	// Coada de disponibilitate a echipajelor
	TUnitNode queue_available_units;
	
	// Stiva pentru istoricul interventiilor
	TInterventionNode undo_stack;
} *TSystem;

// Functie auxiliara pentru initializarea sistemului
TSystem init_system(FILE *input_file);

// Operatiile principale
void add_incident(struct system *system, int id, const char *priority, const char *description);
void check_units_availability(struct system *system, FILE *output_file);
void dispatch(struct system *system, FILE *output_file);
void undo_last_dispatch(struct system *system, FILE *output_file);
void solved_incident(struct system *system, int id, FILE *output_file);
void show_unit(struct system *system, int id, FILE *output_file);
void show_incident(struct system *system, int id, FILE *output_file);
void show_interventions(struct system *system, FILE *output_file);

// Functie pentru eliberarea memoriei alocate pentru sistem
void free_system(struct system *system);

#endif
