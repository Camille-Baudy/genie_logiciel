#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

void extractionTitre(char * fichier,FILE * fichierXml){

    // Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r");
   

    // Chercher la ligne qui contient le titre du papier
    char line[BUFFER_SIZE];
    int isEmpty = 0;
    fprintf(fichierXml,"%s","    <titre> ");
     while (fgets(line, BUFFER_SIZE, file) != NULL) {             
               if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r') {   
                    fprintf(fichierXml,"	</titre>\n");
                    fclose(file);
				}
				else
					fprintf(fichierXml,"%s",line);
        }
}

void extractionAuteur(char *fichier, FILE *outputFile) {
	// On créer la commande pdf2txt
	char command[BUFFER_SIZE];
	sprintf(command, "pdf2txt -o temp.txt %s", fichier);
	
	// On exécute la commande pdf2txt
	system(command);
	
	// On ouvre le fichier texte converti
	FILE *file = fopen("temp.txt", "r");
	//FILE *file2 = fopen(fichierXml, "a");
	
	// On cherche les lignes qui contiennent les auteurs avec leurs adresses
	char line[BUFFER_SIZE];
	int isEmpty = 0;
	fprintf(outputFile, "%s", "    <auteur> ");
	while (fgets(line, BUFFER_SIZE, file) != NULL) {
		fprintf(outputFile, "%s", line);
		if (line[3] == '\n' || line[3] == '\t' || line[3] == ' ' || line[3] == '\r') {
			fprintf(outputFile, "    </auteur> ");
			fclose(file);
		}
	}
}

void extractionAbstract(char * fichier,FILE * fichierXml){
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r");
    
    char line[BUFFER_SIZE];
    int foundAbstract = 0;
    int isEmpty = 0;
    
    fprintf(fichierXml,"%s","	<abstract>");
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
          if (strstr(line, "Abstract") != NULL) {
            foundAbstract = 1;
            continue;
        }
        if (foundAbstract==1) {
            
               if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r') {
                    isEmpty += 1;
                   
                }
                else {
					 if (isEmpty==2) {
						// La ligne est vide, nous avons atteint la fin du paragraphe
						fprintf(fichierXml,"%s","	</abstract>\n");
						fclose(file);
						}
					else 
						fprintf(fichierXml,"	%s",line);
				}

            
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
				fprintf(outputFile,"	%s",line); 
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
	
	//titre
	extractionTitre("compression.pdf",outputFile);
	
	//auteur
	extractionAuteur("compression.pdf",outputFile);
	
	//abstract
	extractionAbstract("compression.pdf",outputFile);
	
	
	//biblio
	fprintf(outputFile, "	<biblio>");
	extractionBiblio("compression.pdf",outputFile);
	fprintf(outputFile, "</biblio>\n");
	

    fclose(outputFile);
	
}

void parseur(char *format){
	
	if(strcmp(format,"-t")==0)
		convertirXml("compression.pdf");
	//else f(strcmp(format,"-x")==0)
		//convertirTxt("compression.pdf");
}



int main(char *argv[]) {
	parseur("-t");
    return 0;  
}
	
