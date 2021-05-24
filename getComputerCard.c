/* 
 * File:   getComputerCard.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 4 avril 2018, 5:27
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
#include "uep_wide.h"
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
t_Card* getComputerCard(LinkedList *comphand,t_Card *reference,int *identifiant)
{
	if(comphand==NULL) return NULL;
	int nbChoiceRange=comphand->NbElem;
	int tabChoices[nbChoiceRange];
	int indicePlusGrand=0;
	int cursorChoice;
	int cptElem;
	
	// Trier les cartes par leur valeur antichronologiquement
	
	for(cursorChoice=0;cursorChoice<nbChoiceRange;cursorChoice++)
	{
		tabChoices[cursorChoice]=cursorChoice;
	}

	cursorChoice=0;
	while(cursorChoice<nbChoiceRange-1)
	{
		indicePlusGrand=cursorChoice;
		for(cptElem=cursorChoice+1;cptElem<nbChoiceRange;cptElem++)
		{	
			lc_Datas *datasLeft=lc_get(comphand,tabChoices[cptElem]);
			lc_Datas *datasRight=lc_get(comphand,tabChoices[indicePlusGrand]);
		
			t_Card *tmpLeft=(t_Card*)datasLeft->value;
			t_Card *tmpRight=(t_Card*)datasRight->value;
		
			//fprintf(stdout,"[%02d] {%s} (%d) <--> [%02d] {%s} (%d)\n",tmpLeft->value,unocard_names[tmpLeft->type],tabChoices[cptElem],tmpRight->value,unocard_names[tmpRight->type],tabChoices[indicePlusGrand]);
			
			if(tmpLeft->value>tmpRight->value)
			{
				indicePlusGrand=cptElem;
			}
		}
		// Nous avons trouvé un plus grand...
		if(indicePlusGrand!=cursorChoice)
		{
			int tmp=tabChoices[cursorChoice];
			tabChoices[cursorChoice]=tabChoices[indicePlusGrand];
			tabChoices[indicePlusGrand]=tmp;
		}
		
		/*int debug;
		for(debug=0;debug<nbChoiceRange;debug++)
			fprintf(stdout,"[%02d]",tabChoices[debug]);
		fprintf(stdout,"\n");*/
		cursorChoice++;
	}
	
	// Ici les cartes sont triées de celles qui valent le plus de points à celles qui valent le moins
	
	//fprintf(stdout,"[DEBUG] %03d éléments\n",comphand->NbElem);
	
	if(reference==NULL)
	{
		lc_Datas *datasRandom=lc_get(comphand,tabChoices[rand()%comphand->NbElem]);  // 0 serait mieux :}
		t_Card *randomCard=(t_Card*)datasRandom->value;
		*identifiant=datasRandom->item_Number; // BUG 
		return randomCard;
	}
	
	cursorChoice=0;
	while(cursorChoice<nbChoiceRange)
	{
		lc_Datas *datasSorted=lc_get(comphand,tabChoices[cursorChoice]);
		if(datasSorted!=NULL)
		{
			t_Card *SortedCard=(t_Card*)datasSorted->value;
		
			//fprintf(stdout,"[%02d] %s {%s} (%02d)\n",tabChoices[cursorChoice],unocard_names[SortedCard->type],unocard_colors[SortedCard->color],SortedCard->value);
		
			*identifiant=datasSorted->item_Number;
		
			if(SortedCard->color==SPECIAL) return SortedCard;								// on peut jouer à tous moments une carte spéciale
			if(SortedCard->color==reference->color) return SortedCard;			// on peut jouer une carte de la même couleur
			if(SortedCard->type==reference->type)															
			{
				// amélioration du 24 avril 2018: si le type est le même (donc la couleur différente) 
				// MAIS que dans la main une carte de plus faible valeur de la même couleur existe, il faut jouer cette carte
				// on ne change la couleur grâce au type que quand on ne peut pas jouer dans la couleur de référence !!!33333
				lc_Datas *parcours=comphand->pHead;
				while(parcours)
				{
					t_Card *verif=(t_Card*)parcours->value;
					if(verif->color==reference->color) 
					{
						SortedCard=verif;
						*identifiant=parcours->item_Number;
					}
					parcours=parcours->pNext;
				}
				return SortedCard;				// on peut jouer une carte du même type (de la même valeur)
			}
		}
		cursorChoice++;				
	}
	return NULL;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


