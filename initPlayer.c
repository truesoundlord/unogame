/* 
 * File:   initPlayer.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 21 avril 2018, 22:17
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
void initPlayer(char *name,bool isHuman,t_UNOPlayer *target)
{
	if(target==NULL) return;
	
	target->isHuman=isHuman;
	target->name=malloc(MAXLEN);
	target->name=name;
	target->score=0;
	target->hand=lc_init();
	target->statistiques=(t_stats){0,0,0,0,0,0.0,0,0,0,0,0};
}


// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


