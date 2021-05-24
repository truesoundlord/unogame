/* 
 * File:   FreeMemory.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 7 avril 2018, 22:50
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

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
void FreePlayers(LinkedList *target)
{
	// Eviter les "fuites de mémoire"
	// En effet: lorsque je fais les "lc_empty" dans unogameANSI.c je ne prends pas la peine
	// de m'assurer que les cartes des joueurs sont libérées en mémoire (rendues disponibles).
	// Cela provoque "une fuite de mémoire" (memory leaks) car ces espaces mémoires
	// ne sont pas libérés pendant l'exécution du programme...
	
	lc_Datas *freeme=target->pHead;
	while(freeme)
	{
		t_UNOPlayer *tmp=(t_UNOPlayer*)freeme->value;
		if(tmp)
		{
			lc_empty(tmp->hand);
		}
		freeme=freeme->pNext;
	}
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


