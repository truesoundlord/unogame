/* 
 * File:   displayPlayers.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 21 avril 2018, 1:51
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdbool.h>

#include <linkedlist.h>
#include "unogame.h"

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

void displayPlayers(LinkedList *target)
{
	if(!target) return;
	lc_Datas *parcours=target->pHead;
	while(parcours)
	{
		t_UNOPlayer *player=(t_UNOPlayer*)parcours->value;
		UpdateStats(player,ST_RANK,player->statistiques.currentrank);					// Modifier les best et worse rank... par rapport au classement courant...
		
		//fprintf(stderr,"[oldrank: %02d]\n",player->statistiques.oldrank);
		
		if(player->statistiques.oldrank>=1 && player->statistiques.oldrank-player->statistiques.currentrank)
				fprintf(stderr,"%02d\t[%s {%2s}]\t\x1b[1m%05d\x1b[0m (%02d/%02d) [%+d]\n",	player->statistiques.currentrank,player->name,player->isHuman?"H":"IA",player->score,
																																										player->statistiques.bestrank,
																																										player->statistiques.worserank,
																																										player->statistiques.oldrank-player->statistiques.currentrank);
		else
				fprintf(stderr,"%02d\t[%s {%2s}]\t\x1b[1m%05d\x1b[0m (%02d/%02d) [-]\n",		player->statistiques.currentrank,player->name,player->isHuman?"H":"IA",player->score,
																																										player->statistiques.bestrank,
																																										player->statistiques.worserank);
		
		parcours=parcours->pNext;
		player->statistiques.oldrank=player->statistiques.currentrank;   // 4 novembre 2018
	}
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


