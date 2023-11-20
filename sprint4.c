 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024


void nameFile(char* file,FILE* fichiertxt) {
	// Initialisation des variables
	char newFileName[250];
	char pdfFileName[250];
	
	// Copie de file dans la variable pdfFileName
	strcpy(pdfFileName, file);
	
	// Conversion des ' ' en '_'
    for(int i = 0; i < 250 && pdfFileName[i] != '\0'; i++) {
		if(pdfFileName[i] == ' ') {
			newFileName[i] = '_';
		} else
			newFileName[i] = pdfFileName[i];
	}
	
	// On s'assure que la chaîne se termine correctement
	newFileName[strlen(pdfFileName)] = '\0';

	

	// On écrit dans le fichier
    fprintf(fichiertxt, "Nom du fichier : %s\n", newFileName);

    
}

void insertion_titre(char * fichier,FILE* fichiertxt){

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
    fprintf(fichiertxt,"titre: ");
     while (fgets(line, BUFFER_SIZE, file) != NULL) {        
               if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r') {    
                fclose(file);
            
            }
            else fputs(line,fichiertxt);

            
        }
    

}

void insertion_resume(char * fichier,FILE* fichierTxt){
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
          if (strstr(line, "Abstract") != NULL) {
            foundAbstract = 1;
        } 
        if (foundAbstract==1) {
            
               if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r') {
                    isEmpty += 1;
                   
                }
                
                 if (isEmpty==2) {
                // La ligne est vide, nous avons atteint la fin du paragraphe
                fclose(file);
            
            }
             else fprintf(fichierTxt,"%s",line);

            
        }
 
	}
    
}

void parseurTxt(char * fichierPdf){
	FILE *file2 = fopen("fichier.txt", "w");
	nameFile(fichierPdf,file2);
	insertion_titre(fichierPdf,file2);
	insertion_resume(fichierPdf,file2);
	pclose(file2);
}

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
    // Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
    FILE *file = fopen("temp.txt", "r");

    // On cherche les lignes qui contiennent les auteurs avec leurs adresses
    char line[BUFFER_SIZE];
    int foundTitle = 0;

	fprintf(outputFile, "%s", "    <auteur>");

	while (fgets(line, BUFFER_SIZE, file) != NULL) {
		if (line[0] == '\n' || line[0] == '\t' || line[0] == ' ' || line[0] == '\r') {
			foundTitle = 1;
			continue;
		}
		if (foundTitle==1) {
			if (strstr(line, "Abstract") != NULL || strstr(line, "article") != NULL || strstr(line, "present") != NULL) {
				fprintf(outputFile, "%s", "    </auteur>\n");
				break;
			} else {
				fprintf(outputFile, "    %s", line);
			}
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
	
	if(strcmp(format,"-x")==0)
		convertirXml("compression.pdf");
	else if(strcmp(format,"-t")==0)
		parseurTxt("compression.pdf");
}


int main(char *argv[]) {
	parseur("-x");
    return 0;  
}
