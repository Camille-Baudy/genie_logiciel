# genie_logiciel
# genie_logiciel

Fichier parseur.c : 

fonction supprimerDossier : Sert à supprimer l'ancien sous dossier si il exitait déjà
fonction créerSousDossier : Crée un sous dossier (qui contiendra les fichiers convertis) dans un repertoire. Si le sous dossier existe déjà on le supprime.

Les fonctions d'extractions prennent toutes en paramètre le nom du fichier à parser (pdf) et le fichier de sortie où il y aura les parties extraites (txt ou xml). 
À part pour l'extraction du nom, ces fonctions commencent par convertir le fichier pdf en executant la commande pdf2txt avec l'option -o et en créant un fichier temporaire temp.txt. Ce fichier est supprimer à la fin de la fonction dès que l'extraction est terminé.


Pour le parseur txt : 

fonction extractionNomTxt : Récupère le nom du fichier (en paramètre de la fonction), enlève le .pdf et convertis les "_" en " ". Il l'écrit ensuite dans le fichier de sortie.
fonction extractionTitreTxt : Récupère la première ligne de temp.txt et l'ajoute au fichier de sortie txt
fonction extractionResumeTxt : Récupère le résumé et l'ajoute au fichier de sortie txt . Pour cela, on cherche le mot "Abstract" dans temp.txt puis une ligne vide pour connaître la fin du paragraphe.  
fonction convertirTxt : Prend en paramètre le nom du fichier pdf à parser, le dossier dans lequel il se trouve et le sous dossier où on ajoutera le fichier txt correspondant. Il créer un fichier txt puis y ajoute les extractions à l'aide des fonctions précédentes. Ce fichier txt a le même nom que le fichier pdf et se crée dans un sous dossier.
fonction parseurTxt : Crée un sous dossier dans le répertoire et appelle la fonction convertirTxt pour tous les fichiers du repertoire passé en argument. Les fichiers txt créés se trouvent ainsi dans le sous dossier.


Pour le parseur xml : 

fonction extractionNomXml : Récupère le nom du fichier (en paramètre de la fonction), enlève le .pdf et convertis les "_" en " ". Il écrit ensuite dans le fichier de sortie : <préambule> le nom du fichier </préambule>.
fonction extractionTitreXml : Récupère la première ligne de temp.txt et l'ajoute au fichier de sortie dans des balises <titre>.
fonction extractionAuteurXml : Récupère les auteurs et les ajoute entre des balises <auteur> au fichier de sortie xml . Les auteurs se trouvent entre le titre et le résumé, ainsi on cherche le mot titre dans temp.txt puis le mot "Abstract" pour connaître la fin du paragraphe.  
fonction extractionAbstractXml : Récupère le résumé et l'ajoute entre des balise <abstract> au fichier de sortie xml . Pour cela, on cherche le mot "Abstract" dans temp.txt puis une ligne vide pour connaître la fin du paragraphe.  
fonction extractionBiblioXml : Récupère les références et les ajoute entre des balise <biblio> au fichier de sortie xml . Pour cela, on cherche le mot "References" dans temp.txt.  
fonction extractionConclusiontXml : Récupère la conclusion et l'ajoute entre des balise <conclusion> au fichier de sortie xml . Pour cela, on cherche le mot "Conclusion" dans temp.txt puis une ligne vide pour connaître la fin du paragraphe.  
fonction extractionIntroductionXml : Récupère l'introduction et l'ajoute entre des balise <introduction> au fichier de sortie xml . Pour cela, on cherche le mot "Introduction" dans temp.txt puis une ligne vide pour connaître la fin du paragraphe.  
fonction extractionDiscussionXml : Récupère la discussion et l'ajoute entre des balise <discussion> au fichier de sortie xml . Pour cela, on cherche le mot "Discussion" dans temp.txt puis une ligne vide pour connaître la fin du paragraphe.  
fonction extractionCorpsXml : Récupère le corps du texte et l'ajoute entre des balise <corps> au fichier de sortie xml . Le corps se trouve entre l'introduction et la conclusion, on cherche ainsi ces deux éléments.
fonction extractionDiscussionXml : Récupère la Discussion et l'ajoute entre des balise <discussion> au fichier de sortie xml . Pour cela, on cherche le mot "Disucssion" dans temp.txt puis une ligne vide pour connaître la fin du paragraphe. 

fonction convertirXml : Prend en paramètre le nom du fichier pdf à parser, le dossier dans lequel il se trouve et le sous dossier où on ajoutera le fichier xml correspondant. Il créer un fichier xml puis y ajoute les balises <?xml encoding='utf-8' ?>, <article>, les extractions à l'aide des fonctions précédentes et la balise </article>. Ce fichier xml a le même nom que le fichier pdf et se crée dans un sous dossier.
fonction parseurXml : Crée un sous dossier dans le répetoire et appelle la fonction convertirXml pour tous les fichiers du repertoire passé en argument. Les fichiers xml crées se trouvent ainsi dans le sous dossier.


fonction parseur1 : Choisi le format texte (-t) ou xml (-x) et appelle le parseur correspondant 

fonction parseur2 : Prend en paramètre le repertoire où se trouvent les fichiers pdf et le format à choisir. Crée un sous dossier dans le répetoire, demande à l'utilisateur les fichiers pdf à parser et les met dans Fichiers[]. Ensuite, on appelle la fonction convertirXml ou convertirTxt (selon le format choisi) pour tous les fichiers dans Fichier[].

Pour tester : 
gcc parseur.c -o Parseur
./Parseur -t OU ./Parseur -x

