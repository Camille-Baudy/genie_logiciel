#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024


void extractionDiscussion(char * fichier,FILE * fichierXml){
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r");
    
    char line[BUFFER_SIZE];
    int foundDiscussion = 0;
    int isEmpty = 0;
    
    fprintf(fichierXml,"%s","	<discussion>");
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
          if ((strstr(line,"Acknowledgments")!=NULL)||(strstr(line,"Acknowledgements")!=NULL)) {
            foundDiscussion = 1;
            continue;
        }
        if (foundDiscussion==1) {
            
					 if ((strstr(line, "References") != NULL) || (strstr(line, " References") != NULL)) {
						// on a atteint la prochaine section
						fprintf(fichierXml,"%s","	</discussion>\n");
						fclose(file);
						}
					else 
						fprintf(fichierXml,"	%s",line);
				}    
        }
 
    }


int main(char *argv[]) {
	FILE *outputFile = fopen("output.xml", "w");
	extractionDiscussion("compression_phrases_Prog-Linear-jair.pdf",outputFile);
    return 0;  
}
