/* 
 * File:   fct_givecards.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 27 mai 2018, 1:23
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
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
void humanizePlayer(int idPlayer,LinkedList *target)
{
	if(idPlayer<gen_Number) return;															// gen_Number est passé de linkedlist.c à linkedlist.h pour plus de "visibilité"...
	lc_Datas *searchresult=lc_search(target,idPlayer);
	if(searchresult)
	{
		t_UNOPlayer *tmp=(t_UNOPlayer*)searchresult->value;
		tmp->isHuman=true;
	}
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


