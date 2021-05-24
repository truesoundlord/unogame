/* 
 * File:   fct_saveround.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 22 juin 2018, 2:00
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <time.h>
#include <locale.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
#endif

#ifndef UNOGAME_H
#include "unogame.h"
#endif

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************

#define SZBUF			255
#define MAXSLOTS	8

// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

struct tm *s_Date;
time_t	rawtime;

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

/* PROTOCOLE 
 * 
 * 1. Sauvegarder le nombre de cartes de la main du joueur
 * 2. Sauvegarder les cartes de la main du joueur
 * 3. Sauvegarder le score actuel
 * 4. Sauvegarder le nom du joueur
 * 5. Sauvegarder son IA
 * 6. Sauvegarder les statistiques
 */

int savePlayer(FILE *pDesc,t_UNOPlayer *target)
{
	if(!pDesc || !target) return -1;
	
	// Sauvegarder la main du joueur, certes, mais d'abord indiquer le nombre de cartes... (nous sommes en mode "dynamique")
	fwrite((void*)&target->hand->NbElem,sizeof(int),1,pDesc);
	
	// ...ensuite sauvegarder les cartes
	
	lc_Datas *parcours=target->hand->pHead;
	while(parcours)
	{
		t_Card *unecarte=(t_Card*)parcours->value;
		fwrite((void*)unecarte,sizeof(t_Card),1,pDesc);				// FLUSH !! (nous pouvons sauvegarder d'un coup une structure)
		fflush(pDesc);
		parcours=parcours->pNext;
	}
	
	// Sauvegarder le score actuel du joueur...
	
	fwrite((void*)&target->score,sizeof(int),1,pDesc);
	
	// Sauvegarder le nom du joueur
	
	fwrite((void*)target->name,MAXLEN,1,pDesc);
	
	// Sauvegarder le type de joueur (humain ou pas)
	
	fwrite((void*)&target->isHuman,sizeof(bool),1,pDesc);
	
	// Sauvegarder les statistiques
	
	//DisplayStats(target);
	fwrite((void*)&target->statistiques,sizeof(t_stats),1,pDesc);					// FLUSH !!
	fflush(pDesc);
	
	return 0;
}

/* Structure concernant la sauvegarde des données 
 * Vu que nous chargeons en mémoire dans le MEME ORDRE les données écrites
 * de manières SEQUENTIELLES dans le fichier sur le disque
 * il convient d'établir un PROTOCOLE !!!
 * 
 * 1. Le nombre de joueurs impliqués dans la partie
 * 2. Les joueurs (en  fonction de leur nombre) 
 * 3. Le nombre de cartes du deck
 * 4. Le deck (en fonction du nombre de cartes restantes)
 * 5. Le nom du joueur courrant
 * 6. La manche en cours
 * 7. Le nombre de manches maximum
 * 8. Le sens giratoire
 */

int saveGame(t_UNOGame *pPartie)
{
	static unsigned int saveslot=0;
	
	FILE *descFichier;
	
	char strDate[SZBUF]="\0";
	char filename[MAXPATHLEN];
	
	if(!pPartie) return -1;
	
	// Créer le fichier contenant la sauvegarde de la partie
	
	time(&rawtime);
	setlocale(LC_ALL,"fr_BE.utf8");
	
	s_Date=localtime(&rawtime);
	strftime(strDate,SZBUF,"[%A %d %B %Y",s_Date);
	
	sprintf(filename,"%s/%s_%03d].dat",REPLAY,strDate,saveslot);
	fprintf(stdout,"[%s] Vérification: %s\n",__func__,filename);
	
	umask(0);
	mkdir(REPLAY,S_IRWXO);
	
	descFichier=fopen(filename,"w");
	
	if(descFichier==NULL)
	{
		perror("[I/O Error");
		printf("]\n");
		return -1;
	}

	fprintf(stdout,"\n[SAVE] Saving slot %03d on disk...",saveslot);
	
	// Sauvegarder les joueurs
	
	// Comme le nombre de joueurs n'est pas connu à l'avance...
	
	fwrite((void*)&pPartie->Players->NbElem,sizeof(int),1,descFichier);				// Le '&' est utilisé ici car la variable NbElem est "un entier" et non "un pointeur"
																																						// comme j'ai besoin de l'adresse en mémoire où se trouve NbElem je dois faire précéder
																																						// le tout par '&'...
	lc_Datas *parcours=pPartie->Players->pHead;
	while(parcours)
	{
		t_UNOPlayer *saveMe=(t_UNOPlayer*)parcours->value;		
		savePlayer(descFichier,saveMe);
		parcours=parcours->pNext;
	}
	
	// Sauvegarder les cartes du deck
	fwrite((void*)&pPartie->Cards->NbElem,sizeof(int),1,descFichier);
	
	parcours=pPartie->Cards->pHead;
	while(parcours)
	{
		t_Card *saveCard=(t_Card*)parcours->value;
		fwrite((void*)saveCard,sizeof(t_Card),1,descFichier);
		parcours=parcours->pNext;
	}
	
	// Sauvegarder le joueur et la carte courante...
	
	fwrite((void*)pPartie->CurrentPlayer->name,MAXLEN,1,descFichier);
	
	// juillet 2018
	if(!pPartie->CurrentCard)  // pas de carte encore jouée (ça peut arriver ;})
	{
		pPartie->CurrentCard=malloc(sizeof(t_Card));
		pPartie->CurrentCard->color=-1;
		pPartie->CurrentCard->type=-1;
		pPartie->CurrentCard->value=-1;
	}
		
	fwrite((void*)pPartie->CurrentCard,sizeof(t_Card),1,descFichier);
	
	// Sauvegarder le round actuel et le nombre de rounds total...
	
	fwrite((void*)&pPartie->currentRound,sizeof(int),1,descFichier);
	fwrite((void*)&pPartie->MaximumRounds,sizeof(int),1,descFichier);
	
	fwrite((void*)&pPartie->bDirection,sizeof(bool),1,descFichier);
	
	saveslot++;
	if(saveslot>MAXSLOTS) saveslot=0;
	fclose(descFichier);
	fprintf(stdout,"\n[SAVE] ... done !!\n");
	return 0;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


