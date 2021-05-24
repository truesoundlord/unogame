/* 
 * File:   checkForCounterAction.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 5 avril 2018, 6:20
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************
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
t_Card* checkForCounterAction(LinkedList *hand,t_Card *reference)
{
	if(hand==NULL) return NULL;
	lc_Datas *datasSeek=hand->pHead;
	while(datasSeek)
	{
		t_Card *candidate=malloc(sizeof(t_Card));
		candidate=(t_Card*)datasSeek->value;
		if(candidate->type==reference->type && reference->color==SPECIAL)
		{
			lc_delete(hand,datasSeek->item_Number);
			return candidate;
		}
		datasSeek=datasSeek->pNext;
	}
	return NULL;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


