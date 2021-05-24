/* 
 * File:   SortList.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 30 avril 2018, 23:13
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
LinkedList* SortList(LinkedList *target)
{
	//static unsigned short stcCount=0; 
	
	if(target==NULL) return NULL;
	
	int		indexes[target->NbElem];
	int		scores[target->NbElem];
	
	LinkedList *tmpSorted=lc_init();

	int cptScores;
	int curseur=0;
	int indicepluspetit=0;

	bool changed=false;
	
	//fprintf(stderr,"[DEBUG] Call count -> %03d\n",++stcCount);

	lc_Datas *parcours=target->pHead;
	while(parcours)
	{
		t_UNOPlayer *tmpPlayer=(t_UNOPlayer*)parcours->value;
		scores[curseur]=tmpPlayer->score;

		indexes[curseur]=curseur;
		parcours=parcours->pNext;
		curseur++;
	}

	// TRIER par ordre chronologique MAIS nous allons uniquement déplacer les indexes (positions/indices)

	curseur=0;
	while(curseur<target->NbElem)
	{
		for(cptScores=curseur;cptScores<target->NbElem;cptScores++)
		{
			if(scores[indexes[cptScores]]<scores[indexes[indicepluspetit]])
			{
				indicepluspetit=cptScores;
				changed=true;
			}
			
			// 22 mai 2018
			// En cas d'égalité, il faut trier selon le nombre de victoires...
			if(scores[indexes[cptScores]]==scores[indexes[indicepluspetit]])
			{
				lc_Datas *lPlayer=lc_get(target,indexes[cptScores]);
				lc_Datas *rPlayer=lc_get(target,indexes[indicepluspetit]);
				
				int lPlayerVictories=((t_UNOPlayer*)lPlayer->value)->statistiques.victories; // victoires du joueur candidat
				int rPlayerVictories=((t_UNOPlayer*)rPlayer->value)->statistiques.victories; // victoires du joueur renseigné comme étant le mieux placé
				
				int lPlayerMax=((t_UNOPlayer*)lPlayer->value)->statistiques.biggestscore;
				int rPlayerMax=((t_UNOPlayer*)rPlayer->value)->statistiques.biggestscore;
				
				if(lPlayerVictories>rPlayerVictories || lPlayerMax<rPlayerMax)		// Si le joueur candidat à un meilleur score que le joueur actuel...
				{	
					//fprintf(stderr,"[DEBUG][%s] %s (max:%05d/won:%02d) <- %s (max:%05d/won:%02d)\n",__func__,((t_UNOPlayer*)lPlayer->value)->name,lPlayerMax,lPlayerVictories,((t_UNOPlayer*)rPlayer->value)->name,rPlayerMax,rPlayerVictories);
					indicepluspetit=cptScores;						// ...il "passe devant".
					changed=true;
				}
			}
			// FIN 22 mai 2018
		}
		// Ici nous avons l'indice de l'élément le plus petit
		if(changed)
		{
			int tmp;

			tmp=indexes[curseur];
			indexes[curseur]=indexes[indicepluspetit];
			indexes[indicepluspetit]=tmp;
			changed=false;
		}
		curseur++;
		indicepluspetit=curseur;
	}
	
	// Reconstruire une liste temporaire en fonction des résultats
	
	for(curseur=0;curseur<target->NbElem;curseur++)
	{
		lc_Datas *parcours=lc_get(target,indexes[curseur]);
		t_UNOPlayer *copyme=(t_UNOPlayer*)parcours->value;
		
		copyme->statistiques.currentrank=curseur+1;
				
		lc_add((void*)copyme,tmpSorted,cssmuserdef,sizeof(t_UNOPlayer));
	}
	//fprintf(stderr,"[%s] END\n",__func__);
	return tmpSorted;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


