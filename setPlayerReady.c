/* 
 * File:   resetPlayer.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 21 avril 2018, 22:58
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
int setPlayerReady(t_UNOPlayer *target,LinkedList *targetdeck,LinkedList *playerlist)
{
	if(!target) return -1;
	if(!targetdeck) return -1;
	if(!playerlist) return -1;
		
	lc_empty(target->hand);
	if(targetdeck->NbElem>0)
		giveHand(target->hand,targetdeck);
	
	// 5 nov 2018
	// euh... oui... pour éviter d'avoir des "crasses" en mémoire lors d'un malloc()...
	// je vais virer cette m... et voir ce que cela donne (normalement ça devrait aller vu que je crée un tableau en début de programme "Joueurs")
	
	// conclusions après tests: concluant ^^
	
	lc_add((void*)target,playerlist,cssmuserdef,sizeof(t_UNOPlayer));
	return 0;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


