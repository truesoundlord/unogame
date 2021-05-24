/* 
 * File:   fct_givehand.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 3 avril 2018, 11:20
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
void InitCards(LinkedList *target,e_Colors cardcolor)
{
	int cptCard;
	int cptCouleur=0;
	int Max;
	t_Card *uneCarte;
	
	switch(cardcolor)
	{
		case RED:			
		case GREEN:
		case BLUE:
		case YELLOW:	uneCarte=malloc(sizeof(t_Card));
									uneCarte->type=0;
									uneCarte->value=unocard_values[0];
									uneCarte->color=cardcolor;
									lc_add((void*)uneCarte,target,cssmuserdef,sizeof(t_Card));
									
									Max=MAXCARDS-2;
									do
									{
										for(cptCard=1;cptCard<Max;cptCard++)
										{	
											uneCarte=malloc(sizeof(t_Card));
											uneCarte->type=cptCard;
											uneCarte->value=unocard_values[cptCard];
											uneCarte->color=cardcolor;
	
											lc_add((void*)uneCarte,target,cssmuserdef,sizeof(t_Card));
										}
									}while(++cptCouleur<2);
;									break;
		case SPECIAL:	Max=MAXCARDS;
									cptCouleur=0;
									do
									{
										for(cptCard=MAXCARDS-2;cptCard<Max;cptCard++)
										{	
											uneCarte=malloc(sizeof(t_Card));
											uneCarte->type=cptCard;
											uneCarte->value=unocard_values[cptCard];
											uneCarte->color=cardcolor;
											
											lc_add((void*)uneCarte,target,cssmuserdef,sizeof(t_Card));
										}
									}while(++cptCouleur<4);
									break;
	}
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


