/* 
 * File:   fct_checkcard.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 3 avril 2018, 11:19
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

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
bool CheckCardPlayed(t_Card *reference,t_Card *candidate,LinkedList *hand)
{
	if(!reference || !candidate) return false;
		
	if(candidate->color==SPECIAL) return true;																														// si c'est une carte spéciale c'est bon...
	if(reference->color==candidate->color) return true;																										// si c'est de la même couleur c'est bon...
	if(reference->type==candidate->type)																																	// si c'est du même "type" (9 rouge sur 9 bleu à condition que nous n'ayons pas de carte en bleu)
	{
		// Vérifier qu'il n'y ait pas de carte de la même couleur dans la main du joueur
		// Je pense que dans les règles on ne peux pas faire "l'impasse"
		// Par exemple: changer de couleur alors que nous pouvons jouer une carte
		// de la couleur en référence... on ne changera de couleur que si nous n'avons 
		// pas de carte de cette couleur... 
		lc_Datas *parcours=hand->pHead;
		while(parcours)
		{
			t_Card *verif=(t_Card*)parcours->value;
			if(verif->color==reference->color) return false;
			parcours=parcours->pNext;
		}
		return true;		
	}
	return false;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************
// GAME: je pense que dans les règles de l'uno, mis à part les cartes spéciales qui
// peuvent être jouées à tous moments, nous ne pouvons pas changer de couleur
// avec une carte de la même valeur si nous avons en main une autre carte de la 
// même couleur



