/* 
 * File:   savescores.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 7 avril 2018, 12:24
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
void SaveScores(LinkedList *players)
{
	FILE *descFichier=fopen(scorename,"w");
	
	if(descFichier)
	{
		lc_Datas *lc_parcours=players->pHead;
		while(lc_parcours)
		{
			t_savedscores tmpScore;
			t_UNOPlayer *tmpPlayer=(t_UNOPlayer*)lc_parcours->value;
			
			fprintf(stdout,"[%s] %s Score: %03d\n",__func__,tmpPlayer->name,tmpPlayer->score);
			tmpScore=(t_savedscores){tmpPlayer->name,tmpPlayer->score};
			
			fwrite((void*)&tmpScore,sizeof(t_savedscores),1,descFichier);
			lc_parcours=lc_parcours->pNext;
		}
		fclose(descFichier);
	}
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


