/* 
 * File:   gameRunning.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 4 avril 2018, 0:52
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdbool.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
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
bool gameRunning(LinkedList *players,int *winner)
{
	*winner=0;
	if(players==NULL) return false;
	
	lc_Datas *tmpPlayers=players->pHead;
	while(tmpPlayers!=NULL)
	{
		t_UNOPlayer *Player=(t_UNOPlayer*)tmpPlayers->value;
		if(Player->hand->NbElem==0) 
		{
			*winner=tmpPlayers->item_Number;
			// Petit "bonus" si il y a dix joueurs, il est TRES rare de remporter une manche, donc chaque victoire permet de "gagner" 50 points.
			if(players->NbElem==MAXPLAYERS)
				Player->score-=50;
		
			return false;
		}
		tmpPlayers=tmpPlayers->pNext;
	}
	return true;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


