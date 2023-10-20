#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024


void SupprimerDossier(const char *path) {
    struct dirent *dir;
    struct stat st;
    DIR *d = opendir(path); //ouverture du dossier
    
	if(d){
		while ((dir = readdir(d))) {
			char child_path[200];//stock le chemin de l'entrée parcouru
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
				continue; 
			snprintf(child_path, sizeof(child_path), "%s/%s", path, dir->d_name);
			if (S_ISDIR(st.st_mode)) //si c'est un sous dossier
				SupprimerDossier(child_path);
			else if (remove(child_path) != 0) //supprime les fichiers dans le sous dossier
					perror("remove");
		}
		closedir(d);
		if (rmdir(path) != 0) { //supprime le sous dossier
			perror("rmdir");
		}
	}
}


void CreerSousDossier(const char * Dossier) 
{
	struct dirent *dir;
    DIR *d = opendir(Dossier);
	if(d){
		char Path[256];
		snprintf(Path, sizeof(Path), "%s/%s", Dossier, "Sous dossier");//concaténation du chemin

       // Si le sous dossier existe, on l'efface
		SupprimerDossier(Path);
			
		//Creation du sous dossier
		if (mkdir(Path, 0755) != 0)
			printf("Impossible de creer le sous-dossier");
		closedir(d);
   }
}

void insertion_titre(char * fichier){

    // Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r"); 
    FILE *file2 = fopen("temp2.txt", "a");
   

    // Chercher la ligne qui contient le titre du papier
    char line[BUFFER_SIZE];
	int isEmpty = 0;
    
     while (fgets(line, BUFFER_SIZE, file) != NULL) {
            fputs(line,file2);
             
               if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r') {    
                fclose(file2);
            
            }

            
        }
    
 
  fclose(file);
}

void insertion_resume(char * fichier){
	char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r"); 
    FILE *file2 = fopen("temp2.txt", "a");
    
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
                    printf("%i\n",isEmpty);
                   
                }
                
                 if (isEmpty==2) {
                // La ligne est vide, nous avons atteint la fin du paragraphe
                fclose(file2);
            
            }
             else fprintf(file2,"%s",line);

            
        }
 
	}
    
 
  fclose(file);
}
	
