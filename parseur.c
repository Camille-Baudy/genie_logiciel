#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

//------------------------création du sous dossier-----------------------------------

//supprime un dossier et tous les fichiers qu'il contient
void SupprimerDossier(char * Dossier) 
{ 
    struct dirent *dir;
    struct stat st;
    DIR *d = opendir(Dossier); //ouverture du dossier
    
	if(d){
		while ((dir = readdir(d))) {
			char child_path[200];//stock le chemin de l'entrée parcouru
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
				continue; 
			snprintf(child_path, sizeof(child_path), "%s/%s", Dossier, dir->d_name);
			if (S_ISDIR(st.st_mode)) //si c'est un sous dossier
				SupprimerDossier(child_path);
			else if (remove(child_path) != 0) //supprime les fichiers dans le sous dossier
					perror("remove");
		}
		closedir(d);
		if (rmdir(Dossier) != 0) { //supprime le sous dossier
			perror("rmdir");
		}
	}
}

//créer un sous dossier dans un dossier 
void CreerSousDossier(char * Dossier, char * SousDossier) 
{
	struct dirent *dir;
    DIR *d = opendir(Dossier);
	if(d){
		char Path[256];
		snprintf(Path, sizeof(Path), "%s/%s", Dossier, SousDossier);//concaténation du chemin

       // Si le sous dossier existe, on l'efface
		SupprimerDossier(Path);
			
		//Creation du sous dossier
		if (mkdir(Path, 0755) != 0)
			printf("Impossible de creer le sous-dossier");
		closedir(d);
   }
}



//------------------------parseur TXT-----------------------------------

//extraire le nom du fichier pdf
void extractionNomTxt(char* file,FILE* fichiertxt) {
	// Initialisation des variables
	char newFileName[250];
	char pdfFileName[250];
	
	//Enlève le .pdf dans le nom du fichier 
	strcpy(pdfFileName, file);
	char *extension = strrchr(pdfFileName, '.'); 
    if (extension != NULL) {
        *extension = '\0'; // Met fin à la chaîne à l'emplacement du point
    }
	
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

//extraire le titre du fichier pdf 
void extractionTitreTxt(char * fichier,FILE* fichiertxt){

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
    // Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
    

}

//extraire le résumé du fichier pdf
void extractionResumeTxt(char * fichier,FILE* fichierTxt){
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
	// Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
    
}

//convertir un fichier pdf en Txt
void convertirTxt(char * file, char* Dossier, char * SousDossier){
	
	//Enlève le .pdf dans le nom du fichier 
	char pdfFileName[100];
    strcpy(pdfFileName, file);
    char *extension = strrchr(pdfFileName, '.'); 
    if (extension != NULL) {
        *extension = '\0'; // Met fin à la chaîne à l'emplacement du point
    }
	
	//Création du nom du fichier txt  en fonction de celui du pdf
	char outputFileName[100];
    snprintf(outputFileName, sizeof(outputFileName), "%s/%s/%s.txt", Dossier,SousDossier,pdfFileName);
    
    //Chemin du fichier pdf
	char FileName[100];
    snprintf(FileName, sizeof(FileName), "%s/%s", Dossier,file);

	//Ouverture du fichier txt
    FILE *outputFile = fopen(outputFileName, "w");
    
    //Insertion des éléments
	extractionNomTxt(file,outputFile);
	extractionTitreTxt(FileName,outputFile);
	extractionResumeTxt(FileName,outputFile);
	
	fclose(outputFile);
}

//convertir tous les fichiers pdf d'un dossier en txt
void parseurTxt(char* Dossier) 
{ 
	//création du sous dossier
	CreerSousDossier(Dossier,"FichiersTxt");
    DIR *directory;
    struct dirent *entry;

    if ((directory = opendir(Dossier)) != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            // On vérifie que l'élément est un fichier et pas un répertoire
            if (entry->d_type == DT_REG) {
                // Appel de convertirXml pour chaque fichier trouvé
                char filename[256]; // La longueur du nom de fichier peut varier
                strncpy(filename, entry->d_name, sizeof(filename) - 1);
                convertirTxt(filename,Dossier,"FichiersTxt");
            }
        }
        closedir(directory);
    } else
        perror("Erreur lors de l'ouverture du dossier");
}

//------------------------Parseur XML-----------------------------------

void extractionNomXml(char* file,FILE* fichiertxt) {
	// Initialisation des variables
	char newFileName[250];
	char pdfFileName[250];
	
	//Enlève le .pdf dans le nom du fichier 
	strcpy(pdfFileName, file);
	char *extension = strrchr(pdfFileName, '.'); 
    if (extension != NULL) {
        *extension = '\0'; // Met fin à la chaîne à l'emplacement du point
    }
	
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
    fprintf(fichiertxt, "	<preambule> %s </preambule> \n",newFileName);
  
}

void extractionTitreXml(char * fichier,FILE * fichierXml){

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
        
    // Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
}

void extractionAuteurXml(char *fichier, FILE *outputFile) {
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
	// Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
}

void extractionAbstractXml(char * fichier,FILE * fichierXml){
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
    
    // On cherche la ligne qui contient le mot Abstract
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
		if (strstr(line, "Abstract") != NULL) {
            foundAbstract = 1;
            continue; //on passe à la ligne suivante
		}
        if (foundAbstract==1) { //si on a trouvé le mot Abstract
			if (line[0] == '\n' || line[0] == '\t'|| line[0] == ' ' || line[0] == '\r')
                    isEmpty += 1;
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
    
    // Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
}


void extractionBiblioXml(char * fichier, FILE * outputFile){
	char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
   FILE *file = fopen("temp.txt", "r"); 
    
    char line[BUFFER_SIZE];
	int found = 0;
	int isEmpty = 0;
	
	fprintf(outputFile, "	<biblio>");
	// On cherche la ligne qui contient le mot References
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
          if (strstr(line, "References") != NULL) {
            found = 1;
            continue; //on passe à la ligne suivante
        } 
        if (found==1) { //si on a trouvé le mot References
              if (line[0] != '\n')
				fprintf(outputFile,"	%s",line); 
        }
	}
	fprintf(outputFile, "</biblio>\n");
	fclose(file);
	// Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
}

void extractionConclusionXml(char * fichier,FILE * fichierXml){
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
    // On cherche la ligne qui contient le mot Conclusion
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
					else if (line[0] != '\n')
						fprintf(fichierXml,"	%s",line);
				}    
        }
     // Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
 
}

void extractionIntroductionXml(char *fichier, FILE *outputFile) {
	// Créer la commande pdf2txt
    char command[BUFFER_SIZE];
    sprintf(command, "pdf2txt -o temp.txt %s", fichier);

    // Exécuter la commande pdf2txt
    system(command);

    // Ouvrir le fichier texte converti
    FILE *file = fopen("temp.txt", "r");

    // On cherche les lignes qui contiennent l'introduction
	char line[BUFFER_SIZE];
    int foundIntroduction = 0;
//    int isEmpty = 0;
    
    fprintf(outputFile,"%s","	<introduction>");
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        if (strstr(line, "Introduction") != NULL) {
            foundIntroduction = 1;
            continue;
        }
        if (foundIntroduction==1) {
			if ((line[0] == '\n' || line[0] == '\t' || line[0] == ' ' || line[0] == '\r') && strstr(line, "2 ") != NULL || strstr(line, "2.") != NULL) {
				fprintf(outputFile,"%s","	</introduction>\n");
				break;
			} else if (line[0] != '\n')
				fprintf(outputFile,"	%s",line);
			}
        }
    // Suppression du fichier temporaire
    if (remove("temp.txt") != 0)
        perror("Erreur lors de la suppression du fichier temporaire");
 
}


//convertir un fichier pdf en Xml
void convertirXml(char* file, char* Dossier, char * SousDossier)
{  
	//Enlève le .pdf dans le nom du fichier 
	char pdfFileName[100];
    strcpy(pdfFileName, file);
    char *extension = strrchr(pdfFileName, '.'); 
    if (extension != NULL) {
        *extension = '\0'; // Met fin à la chaîne à l'emplacement du point
    }
	
	//Création du nom du fichier xml en fonction de celui du pdf
	char outputFileName[100];
    snprintf(outputFileName, sizeof(outputFileName), "%s/%s/%s.xml", Dossier,SousDossier,pdfFileName);
    
    //Chemin du fichier pdf
	char FileName[100];
    snprintf(FileName, sizeof(FileName), "%s/%s", Dossier,file);

	//Ouverture du fichier xml
    FILE *outputFile = fopen(outputFileName, "w");
    
    
    if (outputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return;
    }

	// On écrit dans le fichier
	fprintf(outputFile,"<?xml encoding='utf-8' ?> \n");
	fprintf(outputFile,"<article>\n");
	
	//extraction du préambule
	extractionNomXml(file,outputFile);
	
	/*// Conversion des ' ' en '_' 
    for(int i = 0; i < 250 && pdfFileName[i] != '\0'; i++) {
		if(pdfFileName[i] == '_')
			pdfFileName[i] = ' ';
	}
	fprintf(outputFile, "	<preambule> %s </preambule> \n", pdfFileName);*/
	
	//titre
	extractionTitreXml(FileName,outputFile);
	
	//auteur
	extractionAuteurXml(FileName,outputFile);
	
	//abstract
	extractionAbstractXml(FileName,outputFile);
	
	//introduction
	extractionIntroductionXml(FileName,outputFile);
	
	//conclusion
	extractionConclusionXml(FileName,outputFile);
	
	//biblio
	extractionBiblioXml(FileName,outputFile);
	
	
	fprintf(outputFile,"</article>\n");
    fclose(outputFile);
	
}

//convertir tous les fichiers pdf d'un dossier en xml
void parseurXml(char* Dossier) 
{ 
	//création du sous dossier
	CreerSousDossier(Dossier,"FichiersXml");
    DIR *directory;
    struct dirent *entry;

    if ((directory = opendir(Dossier)) != NULL) {
        while ((entry = readdir(directory)) != NULL) {
            // On vérifie que l'élément est un fichier et pas un répertoire
            if (entry->d_type == DT_REG) {
                // Appel de convertirXml pour chaque fichier trouvé
                char filename[256]; // La longueur du nom de fichier peut varier
                strncpy(filename, entry->d_name, sizeof(filename) - 1);
                convertirXml(filename,Dossier,"FichiersXml");
            }
        }
        closedir(directory);
    } else
        perror("Erreur lors de l'ouverture du dossier");
}

// ----------------------Parseur----------------------------------------

void parseur(char *format){
	
	if(strcmp(format,"-x")==0)
		parseurXml("CORPUS_TRAIN");
	else if(strcmp(format,"-t")==0)
		parseurTxt("CORPUS_TRAIN");

}

int main(char *argv[]) {
	parseur("-x");
    return 0;  
}
