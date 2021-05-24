/* 
 * File:   fct_displaycards.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 3 avril 2018, 11:13
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
void displayCards(t_UNOPlayer *target)
{
	if(target==NULL) return;
	
	lc_Datas *tmpSearch=target->hand->pHead;
	while(tmpSearch!=NULL)
	{
		t_Card *tmpCard=(t_Card*)tmpSearch->value;
		fprintf(stdout,"\t[%s] carte [\x1b[1m%s\x1b[0m] {%s%s\x1b[0m} [%d]\n",__func__,unocard_names[tmpCard->type],ansicolors[tmpCard->color],unocard_colors[tmpCard->color],tmpCard->value);
		tmpSearch=tmpSearch->pNext;
	}
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


