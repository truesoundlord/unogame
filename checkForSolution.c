/* 
 * File:   checkForSolution.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 4 avril 2018, 3:03
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
bool checkForSolution(LinkedList *targethand,t_Card *reference)
{
	if(reference==NULL) return true;
	
	lc_Datas *datasSearch=targethand->pHead;
	while(datasSearch)
	{
		t_Card *tmpCarte=(t_Card*)datasSearch->value;
		//fprintf(stdout,"[HAND CARD CHECK] --> [%s] [%s]\n",unocard_colors[tmpCarte->color],unocard_names[tmpCarte->type]);
		if(tmpCarte->color==reference->color) 
		{
			return true;
		}
		if(tmpCarte->type==reference->type) 
		{
			return true;
		}
		if(tmpCarte->color==SPECIAL) // || reference->color==SPECIAL)
		{
			return true;
		}
		datasSearch=datasSearch->pNext;
	}
	
	return false;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


