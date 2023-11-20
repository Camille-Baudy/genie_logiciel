void extractionCorps(char *fichier, FILE *outputFile) {
	// Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
    FILE *file = fopen("temp.txt", "r");

    // On cherche les lignes qui contiennent l'introduction
	char line[BUFFER_SIZE];
    bool foundCorps = false;
    
    fprintf(outputFile,"%s","	<corps>");
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        if (strstr(line, "2 ") != NULL || strstr(line, "2. ") != NULL) {
            foundCorps = true;
            continue;
        }
        if (foundCorps) {
            if (strstr(line, "Conclusion") != NULL || strstr(line, " Conclusion") != NULL) {
                fprintf(outputFile, "%s", "    </corps>\n");
                fclose(file);
            } else {
                fprintf(outputFile, "    %s", line);
            }
        }
    }
}
