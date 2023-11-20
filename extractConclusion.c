#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024


void extractionConclusion(char * fichier,FILE * fichierXml){
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r");
    
    char line[BUFFER_SIZE];
    int foundConclusion = 0;
    int isEmpty = 0;
    
    fprintf(fichierXml,"%s","	<conclusion>");
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
          if (strstr(line, "Conclusion") != NULL) {
            foundConclusion = 1;
            continue;
        }
        if (foundConclusion==1) {
            
					 if ((strstr(line, "Acknowledgments") != NULL) || (strstr(line, " Acknowledgements") != NULL)) {
						// on a atteint la prochaine section
						fprintf(fichierXml,"%s","	</conclusion>\n");
						fclose(file);
						}
					else 
						fprintf(fichierXml,"	%s",line);
				}    
        }
 
    }


int main(char *argv[]) {
	extractionConclusion("compression_phrases_Prog-Linear-jair.pdf","output.xml");
    return 0;  
}
