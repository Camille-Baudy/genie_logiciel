#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main()
{	
	
	parseurTxt("compression.pdf");
	
	return 0;
}
