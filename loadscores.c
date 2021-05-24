/* 
 * File:   loadscores.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 7 avril 2018, 12:31
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
void LoadScores(LinkedList *players)
{
	FILE *descFichier=fopen(scorename,"r");
	lc_Datas *lc_parcours=players->pHead;
	while(lc_parcours)
	{
		t_savedscores scoreLoaded;
		fread((void*)&scoreLoaded,sizeof(t_savedscores),1,descFichier);
		
		t_UNOPlayer *Player=(t_UNOPlayer*)lc_parcours->value;
		Player->score=scoreLoaded.score;
		fprintf(stdout,"[%s] %s Score: %03d\n",__func__,Player->name,Player->score);
		
		lc_parcours=lc_parcours->pNext;
	}
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


