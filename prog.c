/*#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFFER_SIZE 1024

//gcc -Wall -c tp1.c
//gcc tp1.c -o tp1
// ./tp1

void remove(char *chaine) {
    int longueur = strlen(chaine);
    for (int i = 0; i < longueur; i++) {
        if (chaine[i] == ' ') {
            chaine[i] = '_';
        }
    }
}

void openFile(char* filename, char* nouveauNom) {
    char *extension = strrchr(filename, '.');
    if (extension == NULL) {
        printf("Erreur : Aucune extension de fichier trouvée.\n");
        return;
    }

    *extension = '\0';

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        *extension = '.';
        return;
    }

    remplacerEspacesParTirets(filename);

    fclose(file);
    strcpy(nouveauNom, filename);

    *extension = '.';
    strcat(nouveauNom, extension);
}

int main() {
    char nomFichier[100];
    char nouveauNom[100];
    printf("Entrez le nom du fichier : ");
    fgets(nomFichier, sizeof(nomFichier), stdin);
    nomFichier[strcspn(nomFichier, "\n")] = '\0';

    openFile(nomFichier, nouveauNom);

    printf("Nom de fichier modifié : %s\n", nouveauNom);

    return 0;
}*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFFER_SIZE 1024

void nameFile(char* file) {
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

	// On ouvre le fichier output.txt
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return;
    }

	// On écrit dans le fichier
    fprintf(outputFile, "Nom du fichier : %s\n", newFileName);

    fclose(outputFile);
}


int main() {
	
	nameFile("test fichier.pdf");

	//char commande[BUFFER_SIZE];
	//sprintf(commande, "pdf2txt -o temp.txt %s", pdfFileName);


	//system(commande);

	//FILE *file = fopen("temp.txt", "r");
    //if (file == NULL) {
    //    perror("Erreur lors de l'ouverture du fichier");
    //    return 1;
    //}
    
    //char ligne[BUFFER_SIZE];
    //while (fgets(ligne, sizeof(ligne), file)) {
    //    int longueur = strlen(ligne);
    //    for(int i = 0; i < longueur; i++) {
    //        if(ligne[i] == ' ') {
    //            ligne[i] = '_';
    //        }
    //    }
    //}
	
    
    //fclose(file);
    //remove("temp.txt");
    
    return 0;  
}
