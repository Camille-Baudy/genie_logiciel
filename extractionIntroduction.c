void extractionIntroduction(char *fichier, FILE *outputFile) {
	// Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
    FILE *file = fopen("temp.txt", "r");

    // On cherche les lignes qui contiennent l'introduction
	char line[BUFFER_SIZE];
    bool foundIntroduction = false;
//    int isEmpty = 0;
    
    fprintf(outputFile,"%s","	<introduction>");
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        if (strstr(line, "Introduction") != NULL) {
            foundIntroduction = true;
            continue;
        }
        if (foundIntroduction) {
			if (strstr(line, "2 ") != NULL || strstr(line, "2.") != NULL) {
				fprintf(outputFile,"%s","	</introduction>\n");
				fclose(file);
			} else {
				fprintf(outputFile,"	%s",line);
			}
        }
    }
}
