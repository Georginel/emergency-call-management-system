/* NEAGU Georginel-Aurelian - 322CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"

int main() {
	// Deschidem fisierele de intrare si iesire
	FILE *input_file = fopen("tema1.in", "r");
	FILE *output_file = fopen("tema1.out", "w");
	
	if (!input_file || !output_file) {
		printf("Eroare la deschiderea fisierelor!\n");
		return 1;
	}

	// Initializam sistemul si citim datele echipajelor din fisier
	struct system *system = init_system(input_file);

	// Citim numarul total de comenzi
	int N;
	if (fscanf(input_file, "%d", &N) != 1) {
		fclose(input_file);
		fclose(output_file);
		return 1;
	}

	char command[50];
	
	// Procesam fiecare operatie pe rand
	for (int i = 0; i < N; i++) {
		fscanf(input_file, "%s", command);

		if (strcmp(command, "ADD_INCIDENT") == 0) {
			int id;
			char priority[10];
			char description[256]; // Buffer temporar pentru citirea textului
			
			fscanf(input_file, "%d %s", &id, priority);
			
			// Extragem descrierea plasata intre ghilimele
			fgetc(input_file);
			fgetc(input_file);
			fgets(description, 256, input_file);

			// Eliminam ghilimeaua de la final si "\n"
			description[strlen(description) - 2] = '\0';

			add_incident(system, id, priority, description);
			
		} else if (strcmp(command, "CHECK_UNITS_AVAILABILITY") == 0) {
			check_units_availability(system, output_file);
			
		} else if (strcmp(command, "DISPATCH") == 0) {
			dispatch(system, output_file);
			
		} else if (strcmp(command, "UNDO_LAST_DISPATCH") == 0) {
			undo_last_dispatch(system, output_file);
			
		} else if (strcmp(command, "SOLVED_INCIDENT") == 0) {
			int id;
			fscanf(input_file, "%d", &id);
			solved_incident(system, id, output_file);
			
		} else if (strcmp(command, "SHOW_UNIT") == 0) {
			int id;
			fscanf(input_file, "%d", &id);
			show_unit(system, id, output_file);
			
		} else if (strcmp(command, "SHOW_INCIDENT") == 0) {
			int id;
			fscanf(input_file, "%d", &id);
			show_incident(system, id, output_file);
			
		} else if (strcmp(command, "SHOW_INTERVENTIONS") == 0) {
			show_interventions(system, output_file);
		}
	}

	// Eliberam memoria
	free_system(system);

	// Inchidem fisierele
	fclose(input_file);
	fclose(output_file);

	return 0;
}
