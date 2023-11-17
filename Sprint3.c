#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

void extractionTitre(char * fichier,char * fichierXml){

    // Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r");
   FILE *file2=fopen(fichierXml,"a");
   

    // Chercher la ligne qui contient le titre du papier
    char line[BUFFER_SIZE];
    int isEmpty = 0;
    fprintf(fichierXml,"%s","    <titre> ");
     while (fgets(line, BUFFER_SIZE, file) != NULL) {
            fprintf(fichierXml,"%s",line);
             
               if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r') {   
                    fprintf(fichierXml,"    </titre> ");
                    fclose(file2);
            
            }
        }
}


void extractionBiblio(char * fichier, FILE * outputFile){
	char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r"); 
    //FILE *file2 = fopen("temp2.txt", "a");
    
    char line[BUFFER_SIZE];
	int foundAbstract = 0;
	int isEmpty = 0;
	
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
          if (strstr(line, "References") != NULL) {
            foundAbstract = 1;
            continue;
        } 
        if (foundAbstract==1) {
              if (line[0] != '\n')
				fprintf(outputFile,"%s",line); 
        }
	}
    
 
  fclose(file);
}


void convertirXml(char* file){
	// On ouvre le fichier output.txt
    FILE *outputFile = fopen("output.xml", "w");
    if (outputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return;
    }

	// On écrit dans le fichier
	fprintf(outputFile,"<?xml encoding='utf-8' ?> \n");
	fprintf(outputFile,"<article>\n");
	
	//extraction du préambule
	fprintf(outputFile, "	<premabule> %s </preambule> \n", file);
	extractionTitre("compression.pdf",outputFile);
	fprintf(outputFile, "	<biblio>");
	extractionBiblio("compression.pdf",outputFile);
	fprintf(outputFile, "</biblio>\n");
	

    fclose(outputFile);
	
}




int main() {
	convertirXml("compression.pdf");
    return 0;  
}
	
