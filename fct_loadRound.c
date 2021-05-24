/* 
 * File:   fct_loadRound.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 22 juin 2018, 4:54
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <dirent.h>
#include <unistd.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
#endif

#ifndef __UEPWIDE__
#include <uep_wide.h>
#endif

#ifndef UNOGAME_H
#include "unogame.h"
#endif

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

int loadPlayer(FILE *pDesc,t_UNOPlayer *target)
{
	if(!pDesc || !target) return -1;
		
	int cptcard=0;
	
	fprintf(stdout,"\n[%s] Loading player...\n",__func__);
		
	// Sauvegarder la main du joueur, certes, mais d'abord indiquer le nombre de cartes... (nous sommes en mode "dynamique")
	fread((void*)&cptcard,sizeof(int),1,pDesc);
	
	// ...ensuite sauvegarder les cartes
	
	// fprintf(stdout,"[%s] CALL !!\n",__func__);
	fprintf(stdout,"\n[CARDS LOAD] Loading cards...");
	
	while(cptcard>0)
	{
		t_Card *tmp=malloc(sizeof(t_Card));
		fread((void*)tmp,sizeof(t_Card),1,pDesc);
		lc_add((void*)tmp,target->hand,cssmuserdef,sizeof(t_Card));
		cptcard--;
	}
	
	//fprintf(stdout,"[%s] Vérification: %02d cartes en main...\n",__func__,target->hand->NbElem);
	displayCards(target);
	
	fprintf(stdout,"done !!!\n");
	
	// Charger le score actuel du joueur...
	
	fread((void*)&target->score,sizeof(int),1,pDesc);
	//fprintf(stdout,"[%s] Vérification: score %05d ...\n",__func__,target->score);
	
	// Charger le nom du joueur
	
	target->name=malloc(MAXLEN);
	fread((void*)target->name,MAXLEN,1,pDesc);
	//fprintf(stdout,"[%s] Vérification: [%s]\n",__func__,target->name);
	
	// Charger le type de joueur (humain ou pas)
	
	fread((void*)&target->isHuman,sizeof(bool),1,pDesc);
	//fprintf(stdout,"[%s] Vérification: %s...\n",__func__,target->isHuman?"humain":"IA");
	
	// Charger les statistiques
	
	fread((void*)&target->statistiques,sizeof(t_stats),1,pDesc);					// FLUSH !!
	DisplayStats(target);
	
	fprintf(stdout,"\n[%s] ...end\n",__func__);
	
	return 0;
}

// Retourne l'identifiant du joueur actuel (CurrentPlayer)

int loadGame(t_UNOGame *pPartie)
{
	FILE *descFichier;
	
	int selectedslot=0;
	int currentPlayerID=0;
	char filename[MAXPATHLEN];
	
	LinkedList *ll_filenames=lc_init();
	
	if(!pPartie) return -1;
	
	// Proposer les fichiers à charger en mémoire...
	
	DIR           *desc_REP;
  struct dirent *fichier;
	
	fprintf(stdout,"Sélectionnez un fichier: \n");
	int cpt=0;
	desc_REP=opendir(REPLAY);
	if(desc_REP)
	{
		// Lire le répertoire...
		chdir(REPLAY);
		while((fichier=readdir(desc_REP)) != NULL)
		{
			if(strstr(fichier->d_name,".dat"))
			{
				struct stat infosfichier;
				
				int rc=stat(fichier->d_name,&infosfichier);
				if(rc!=-1)
				{
					fprintf(stdout,"[%02d] %s (%06d bytes)\n",cpt+1,fichier->d_name,infosfichier.st_size);
					lc_add((void*)fichier->d_name,ll_filenames,cssmuserdef,strlen(fichier->d_name));
					cpt++;
				}
			}
		}
	}
	closedir(desc_REP);

	int longueur=(cpt/10)+1;
	fprintf(stdout,"[GAME] Choisissez votre sauvegarde à charger: ");
	do
	{
		char *ALancienne=getnchar(longueur);
		selectedslot=atoi(ALancienne);
	}while(selectedslot<1 || selectedslot>cpt+1);
	
	// Ici ça va être du sport...
	
	lc_Datas *tmp=lc_get(ll_filenames,selectedslot-1);					// récupérer l'élément de la liste chaînée (wrapper)
	char *tmpSource=(char*)tmp->value;
	
	//fprintf(stdout,"[DEBUG] [%s]\n",tmpSource);
	strcpy(filename,tmpSource);
	
	// Permettre d'en sélectionner un...
	
	selectedslot--;
	fprintf(stdout,"\n[LOAD] Loading element %03d from disk...",selectedslot);
	fflush(stdout);
	
	// Charger les informations en mémoire...
	
	descFichier=fopen(filename,"r");
	
	if(descFichier==NULL)
	{
		perror("I/O Error:> ");
		return -1;
	}
	
	// Charger les joueurs
	
	// Comme le nombre de joueurs n'est pas connu à l'avance...
	
	fread((void*)&cpt,sizeof(int),1,descFichier);
	
	//fprintf(stdout,"Nombre de joueurs: %02d\n",cpt);
	
	//FreePlayers(pPartie->Players);
		
	// Aller chercher les joueurs sur le fichier...
	
	while(cpt>0)
	{
		t_UNOPlayer *loadhim=malloc(sizeof(t_UNOPlayer));
		loadhim->hand=lc_init();
		loadPlayer(descFichier,loadhim);
		lc_add((void*)loadhim,pPartie->Players,cssmuserdef,sizeof(t_UNOPlayer));
		cpt--;
	}
	
	//fprintf(stdout,"Vérification: %02d joueurs dans la liste\n",pPartie->Players->NbElem);
	
	fread((void*)&cpt,sizeof(int),1,descFichier);
	
	//fprintf(stdout,"%03d cartes dans le deck\n",cpt);
	
	// Charger les cartes du deck
	
	while(cpt>0)
	{
		t_Card *loadcard=malloc(sizeof(t_Card));
		fread((void*)loadcard,sizeof(t_Card),1,descFichier);
		lc_add((void*)loadcard,pPartie->Cards,cssmuserdef,sizeof(t_Card));
		cpt--;
	}
	
	//fprintf(stdout,"Vérification: %03d cartes dans le deck\n",pPartie->Cards->NbElem);
	
	//fprintf(stdout,"[%s] DEBUG -- STEP\n",__func__);
	
	// Charger le joueur et la carte courante...
	
	pPartie->CurrentPlayer=malloc(sizeof(t_UNOPlayer));
	pPartie->CurrentPlayer->name=malloc(MAXLEN);
	
	fread((void*)pPartie->CurrentPlayer->name,MAXLEN,1,descFichier);
	
	//fprintf(stdout,"[%s] DEBUG -- STEP\n",__func__);
	//fprintf(stdout,"[%s] Vérification: [%s]\n",__func__,pPartie->CurrentPlayer->name);
	
	lc_Datas *parcours=pPartie->Players->pHead;
	while(parcours)
	{
		t_UNOPlayer *tmpPlayer=(t_UNOPlayer*)parcours->value;
		if(!strcmp(pPartie->CurrentPlayer->name,tmpPlayer->name)) currentPlayerID=parcours->item_Number;
		parcours=parcours->pNext;
	}
	if(!currentPlayerID) return -2; // indique que le joueur n'a pas été trouvé (???)
	
	//fprintf(stdout,"[%s] Vérification: [current player ID {%05d}]\n",__func__,currentPlayerID);
	
	
	pPartie->CurrentCard=malloc(sizeof(t_Card));
	fread((void*)pPartie->CurrentCard,sizeof(t_Card),1,descFichier);
	
	
	// juillet 2018
	if(pPartie->CurrentCard->type==-1) // cas du joueur qui commence la partie sans carte jouée
	{
		pPartie->CurrentCard=NULL;
	}
	/*else
		fprintf(stdout,"[%s] Vérification: [%s {%s}]\n",__func__,unocard_names[pPartie->CurrentCard->type],unocard_colors[pPartie->CurrentCard->color]);*/
		
	// Charger le round actuel et le nombre de rounds total...
	
	fread((void*)&pPartie->currentRound,sizeof(int),1,descFichier);
	//fprintf(stdout,"Vérification: round actuel %03d\n",pPartie->currentRound+1);
		
	fread((void*)&pPartie->MaximumRounds,sizeof(int),1,descFichier);
	//fprintf(stdout,"Vérification: manches %03d\n",pPartie->MaximumRounds);
	
	fread((void*)&pPartie->bDirection,sizeof(bool),1,descFichier);
	
	
	fclose(descFichier);
	fprintf(stdout,"\n[LOAD] ... done !!\n",selectedslot);
	
	chdir("..");
	return currentPlayerID;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


