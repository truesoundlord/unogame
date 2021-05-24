/* 
 * File:   getScore.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 4 avril 2018, 10:45
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
int getScore(LinkedList *hand)
{
	int localscore=0;
	
	if(hand==NULL) return -1;
	lc_Datas *datasCompute=hand->pHead;
	while(datasCompute)
	{
		t_Card *carte=(t_Card*)datasCompute->value;
		localscore+=carte->value;
		datasCompute=datasCompute->pNext;
	}
	return localscore;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


