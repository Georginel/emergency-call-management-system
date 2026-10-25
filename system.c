/* NEAGU Georginel-Aurelian - 322CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "unit.h"
#include "incident.h"
#include "intervention.h"

// Alocam memorie pentru tot sistemul si il initializam
TSystem init_system(FILE *input_file) {
	TSystem system = malloc(sizeof(struct system));
	if (!system) return NULL;

	// Citim si alocam vectorul pentru echipaje
	fscanf(input_file, "%d", &system->units_number);
	system->units = malloc(system->units_number * sizeof(struct unit));

	system->queue_high = NULL;
	system->queue_medium = NULL;
	system->queue_low = NULL;
	system->queue_available_units = NULL;
	system->undo_stack = NULL;

	// Adaugam echipajele in coada de disponibilitate
	for (int i = 0; i < system->units_number; i++) {
		fscanf(input_file, "%d %c", &system->units[i].id, &system->units[i].type);
		system->units[i].availability = 1; 

		enqueue_unit(&(system->queue_available_units), &system->units[i]);
	}

	// Setam santinela pentru lista incidentelor
	system->incidents = malloc(sizeof(struct incident));
	system->incidents->id = 0; 
	strcpy(system->incidents->priority, "low");
	system->incidents->description = malloc(strlen("test incident") + 1);
	strcpy(system->incidents->description, "test incident");
	strcpy(system->incidents->status, "solved");
	system->incidents->prev = system->incidents;
	system->incidents->next = system->incidents;

	// Setam santinela pentru lista interventiilor
	system->interventions = malloc(sizeof(struct intervention));
	system->interventions->incident = NULL;
	system->interventions->unit = NULL;
	system->interventions->prev = system->interventions;
	system->interventions->next = system->interventions;

	return system;
}

// Adaugam un nou incident in sistem
void add_incident(TSystem system, int id, const char *priority, const char *description) {
	// Alocam memorie
	TIncident new_incident = malloc(sizeof(struct incident));
	if (!new_incident) return;

	new_incident->id = id;
	strcpy(new_incident->priority, priority);
	new_incident->description = malloc(strlen(description) + 1);
	strcpy(new_incident->description, description);
	strcpy(new_incident->status, "queued");

	// Salvam in istoric, incidentul
	insert_incident_end(system->incidents, new_incident);

	// Plasam incidentul in coada de asteptare corespunzatoare prioritatii sale
	if (strcmp(priority, "high") == 0) {
		enqueue_incident(&(system->queue_high), new_incident);
	} else if (strcmp(priority, "medium") == 0) {
		enqueue_incident(&(system->queue_medium), new_incident);
	} else if (strcmp(priority, "low") == 0) {
		enqueue_incident(&(system->queue_low), new_incident);
	}
}

// Afiseaza numarul de echipaje care stau la rand sa preia o misiune
void check_units_availability(TSystem system, FILE *output_file) {
	int count = 0;
	TUnitNode current = system->queue_available_units;
	while (current != NULL) {
		count++;
		current = current->next;
	}
	fprintf(output_file, "Number of available units: %d\n", count);
}

// Asociaza cel mai urgent incident cu primul echipaj disponibil
void dispatch(TSystem system, FILE *output_file) {
	TIncidentNode *selected_queue = NULL;

	// Cautam prima coada care are cel putin un incident in asteptare
	if (system->queue_high != NULL) {
		selected_queue = &(system->queue_high);
	} else if (system->queue_medium != NULL) {
		selected_queue = &(system->queue_medium);
	} else if (system->queue_low != NULL) {
		selected_queue = &(system->queue_low);
	}

	// Refuzam operatia daca nu sunt incidente sau daca nu avem echipaje
	if (selected_queue == NULL || system->queue_available_units == NULL) {
		fprintf(output_file, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	// Extragem primul incident din coada aleasa
	TIncidentNode incident_node_to_remove = *selected_queue;
	TIncident incident = incident_node_to_remove->incident;
	*selected_queue = incident_node_to_remove->next;
	free(incident_node_to_remove); 

	// Extragem primul echipaj din coada de disponibilitate
	TUnitNode unit_node_to_remove = system->queue_available_units;
	TUnit unit = unit_node_to_remove->unit;
	system->queue_available_units = unit_node_to_remove->next;
	free(unit_node_to_remove);

	// Actualizam statusul
	strcpy(incident->status, "intervened");
	unit->availability = 0;

	// Inregistram asocierea intr-o noua interventie
	TIntervention new_intervention = malloc(sizeof(struct intervention));
	if (!new_intervention) return;
	new_intervention->incident = incident;
	new_intervention->unit = unit;

	// O salvam in istoricul general
	insert_intervention_end(system->interventions, new_intervention);
	push_undo_stack(&(system->undo_stack), new_intervention);
}

// Anuleaza ultimul dispatch aflat in desfasurare
void undo_last_dispatch(TSystem system, FILE *output_file) {
	TIntervention target_intervention = NULL;

	// Cautam in stiva ultima interventie care inca nu a fost rezolvata
	while (system->undo_stack != NULL) {
		TInterventionNode popped_node = system->undo_stack;
		system->undo_stack = system->undo_stack->next;

		TIntervention intervention = popped_node->intervention;
		free(popped_node); 

		if (strcmp(intervention->incident->status, "intervened") == 0) {
			target_intervention = intervention;
			break;
		}
	}

	// Daca nu am gasit nimic valid, afisam eroare
	if (target_intervention == NULL) {
		fprintf(output_file, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	TIncident incident = target_intervention->incident;
	TUnit unit = target_intervention->unit;

	// Restauram starea si eliberam echipajul pentru a prelua un alt incident
	strcpy(incident->status, "queued");
	unit->availability = 1;

	enqueue_unit(&(system->queue_available_units), unit);

	// Repunem incidentul exact la INCEPUTUL cozii sale specifice
	if (strcmp(incident->priority, "high") == 0) {
		push_incident_front(&(system->queue_high), incident);
	} else if (strcmp(incident->priority, "medium") == 0) {
		push_incident_front(&(system->queue_medium), incident);
	} else if (strcmp(incident->priority, "low") == 0) {
		push_incident_front(&(system->queue_low), incident);
	}

	// Scoatem interventia din lista si o stergem din memorie
	target_intervention->prev->next = target_intervention->next;
	target_intervention->next->prev = target_intervention->prev;
	free(target_intervention);
}

// Marcheaza o actiune ca fiind incheiata cu succes
void solved_incident(TSystem system, int id, FILE *output_file) {
	TIntervention current = system->interventions->next;
	TIntervention target_intervention = NULL;

	// Cautam interventia activa in lista circulara dupa ID-ul incidentului
	while (current != system->interventions) {
		if (current->incident->id == id) {
			target_intervention = current;
			break;
		}
		current = current->next;
	}

	// Daca nu exista sau a fost deja rezolvata, afisam eroare
	if (target_intervention == NULL || strcmp(target_intervention->incident->status, "intervened") != 0) {
		fprintf(output_file, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	// Trecem incidentul ca "solved" Repunem echipajul la coada celor disponibili
	strcpy(target_intervention->incident->status, "solved");
	// Repunem echipajul la coada celor disponibili
	target_intervention->unit->availability = 1;
	enqueue_unit(&(system->queue_available_units), target_intervention->unit);
}

// Cauta si afiseaza detaliile unui echipaj
void show_unit(TSystem system, int id, FILE *output_file) {
	TUnit target = NULL;
	
	// Il cautam direct in vectorul de echipaje alocat initial
	for (int i = 0; i < system->units_number; i++) {
		if (system->units[i].id == id) {
			target = &system->units[i];
			break;
		}
	}

	if (target == NULL) {
		fprintf(output_file, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	fprintf(output_file, "Unit %d is type %c and is %s\n", 
			target->id, target->type, target->availability ? "available" : "unavailable");
}

// Cauta si afiseaza detaliile unui incident
void show_incident(TSystem system, int id, FILE *output_file) {
	TIncident current = system->incidents->next;
	TIncident target = NULL;

	// Iteram prin lista circulara, sarind peste santinela
	while (current != system->incidents) {
		if (current->id == id) {
			target = current;
			break;
		}
		current = current->next;
	}

	if (target == NULL) {
		fprintf(output_file, "INVALID OPERATION! ERROR 404\n");
		return;
	}

	fprintf(output_file, "Incident %d has %s priority, the following description: \"%s\" and is %s\n",
			target->id, target->priority, target->description, target->status);
}

// Listeaza istoricul tuturor interventiilor inregistrate
void show_interventions(TSystem system, FILE *output_file) {
	if (system->interventions->next == system->interventions) {
		fprintf(output_file, "No intervention has been initiated\n");
		return;
	}

	// Afisam elementele trecand prin lista circulara
	TIntervention current = system->interventions->next;
	while (current != system->interventions) {
		fprintf(output_file, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n",
				current->incident->id, current->unit->id, current->incident->status);
		current = current->next;
	}
}

// Elibaram memoria sistemului
void free_system(TSystem system) {
	if (!system) return;

	// Eliberam vectorul de echipaje
	if (system->units) {
		free(system->units);
	}

	// Curatam nodurile din cozile de prioritati
	TIncidentNode current_incident_node;
	while (system->queue_high != NULL) {
		current_incident_node = system->queue_high;
		system->queue_high = system->queue_high->next;
		free(current_incident_node);
	}
	while (system->queue_medium != NULL) {
		current_incident_node = system->queue_medium;
		system->queue_medium = system->queue_medium->next;
		free(current_incident_node);
	}
	while (system->queue_low != NULL) {
		current_incident_node = system->queue_low;
		system->queue_low = system->queue_low->next;
		free(current_incident_node);
	}

	// Curatam nodurile cozii de echipaje disponibile
	TUnitNode current_unit_node;
	while (system->queue_available_units != NULL) {
		current_unit_node = system->queue_available_units;
		system->queue_available_units = system->queue_available_units->next;
		free(current_unit_node);
	}

	// Golim stiva de undo
	TInterventionNode current_intervention_node;
	while (system->undo_stack != NULL) {
		current_intervention_node = system->undo_stack;
		system->undo_stack = system->undo_stack->next;
		free(current_intervention_node);
	}

	// Eliberam lista de interventii
	TIntervention current_intervention = system->interventions->next;
	while (current_intervention != system->interventions) {
		TIntervention temp = current_intervention;
		current_intervention = current_intervention->next;
		free(temp); 
	}
	free(system->interventions); 

	// Eliberam lista de incidente
	TIncident current_incident = system->incidents->next;
	while (current_incident != system->incidents) {
		TIncident temp = current_incident;
		current_incident = current_incident->next;
		free(temp->description); 
		free(temp);              
	}
	
	// Eliberam descrierea alocata pentru santinela
	if (system->incidents->description) {
		free(system->incidents->description);
	}
	free(system->incidents);

	// Eliberam structura mare
	free(system);
}