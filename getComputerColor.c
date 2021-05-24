/* 
 * File:   getComputerColor.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 4 avril 2018, 9:54
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
e_Colors getComputerColor(LinkedList *comphand,t_Card *reference)
{
	int nbColors[MAXCOLORS]={0,0,0,0};
	int indicePlusGrand=0;
	int cptElem;
	
	
	if(comphand && reference)
	{
		// Compter le nombre de cartes pour chaque couleurs 
		lc_Datas *datasCouleurs=comphand->pHead;
		while(datasCouleurs)
		{
			t_Card *tmpCarte=(t_Card*)datasCouleurs->value;
			nbColors[tmpCarte->color]++;
			datasCouleurs=datasCouleurs->pNext;
		}
		// Rechercher la couleur qui est la plus représentée dans la main du joueur
		
		for(cptElem=1,indicePlusGrand=0;cptElem<MAXCOLORS;cptElem++)
		{
			if(nbColors[cptElem]>nbColors[indicePlusGrand])
			{
				indicePlusGrand=cptElem;
			}
		}
		fprintf(stdout,"Couleur choisie: [%s%s\x1b[0m]\n",ansicolors[indicePlusGrand],unocard_colors[indicePlusGrand]);
		return indicePlusGrand;
	}
	return -1;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


