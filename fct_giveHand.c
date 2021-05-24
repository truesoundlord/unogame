/* 
 * File:   fct_giveHand.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 27 mai 2018, 1:22
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void giveHand(LinkedList *hand,LinkedList *deck)
{
	int cptCartes=0;
	int pickedcard=0;
	
	while(cptCartes<HANDCARDS)
	{
		srand(clock());
		pickedcard=rand()%deck->NbElem;
		
		lc_Datas *tmp=lc_get(deck,pickedcard);
		int idElem=tmp->item_Number;
		
		t_Card *handcard=malloc(sizeof(t_Card));
		handcard=(t_Card*)tmp->value;
		
		//fprintf(stderr,"[%s] DEBUG %08x <-> %08x\n",__func__,tmp->Free,FreeCard);
		lc_delete(deck,idElem);
		
#ifdef DEBUG
		fprintf(stdout,"[%s] carte [%s] {%s} [%d]\n",__func__,unocard_names[handcard->type],unocard_colors[handcard->color],handcard->value);
#endif
		//int idCard=lc_add((void*)handcard,hand,cssmuserdef,sizeof(t_Card));
		
		lc_add((void*)handcard,hand,cssmuserdef,sizeof(t_Card));
		
		// juin 2018
		// Si je veux être cohérent avec la libération de la mémoire il faut indiquer quelle est la fonction a appeler pour libérer
		// la mémoire lorsque cette carte sera jouée ^^
		
		// lc_setFreeItemByID(hand,idCard,FreeCard); // en effet les cartes de ll_deck n'ont rien à voir avec les cartes de hand, en effet ce sont des pointeurs différents :{
		// Du coup il faut pour chaque carte ajoutée à la main, spécifier de nouveau la fonction de libération de la mémoire :{ (pas super)
		
		nanosleep(&Temporisation,NULL);
		cptCartes++;
	}
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


