# genie_logiciel

Fichier parseur.c : 

fonction supprimerDossier: sert à supprimer l'ancien sous dossier si il exitait déjà
fonction créerSousDossier : créer un sous dossier (qui contiendra les fichiers convertis) dans un repertoire
fonction extractionNomTxt : récupère le nom du fichier passé en argument en enlevant le .pdf et en convertissant les "_" en " "

Pour le parseur txt : 
fonction extractionTitreTxt : convertis le fichier pdf en un fichier temporaire txt et récupère la première ligne de celui ci
fonction extractionResumeTxt : convertis le fichier pdf en un fichier temporaire txt et récupère le résumé en cherchant le mot "Abstract"
fonction convertirTxt : convertis un fichier pdf (situé dans un dossier) en un fichier txt à l'aide des fonctions précédentes. Ce fichier txt a le même nom que le fichier pdf et se crée dans un sous dossier.
fonction parseurTxt : Créer un sous dossier dans le répetoire et appelle la fonction convertirTxt pour tous les fichiers du repertoire passé en argument. Les fichiers txt crées se trouvent ainsi dans le sous dossier.

Pour le parseur xml : 
les fonctions: extractionNomXml, extractionTitreXml, extractionAuteurXml, extractionAbstractXml,extractionBiblioXml, extractionConclusionXmln, extractionIntrdocutionXml.. : suivent le même principe que les fonctions d'extraction en  txt. Un fichier temporaire txt est créée pour récuperer la partie qui nous interesse. Des balises xml sont ajoutées avant et après cette partie.
fonction convertirXml : convertis un fichier pdf (situé dans un dossier) en un fichier xml à l'aide des fonctions précédentes. Ce fichier xml a le même nom que le fichier pdf et se crée dans un sous dossier.
fonction parseurXml : Créer un sous dossier dans le répetoire et appelle la fonction convertirXml pour tous les fichiers du repertoire passé en argument. Les fichiers xml crées se trouvent ainsi dans le sous dossier.
(La discussion correspont au remerciements)

fonction parseur : choisi le format texte ou xml et appelle le parseur correspondant 

Pour tester : 
gcc parseur.c -o Parseur
./Parseur



