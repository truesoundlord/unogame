/* 
 * File:   UpdateStats.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 16 avril 2018, 22:58
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
void UpdateStats(t_UNOPlayer *target,e_stats stat,int value)
{
	double boomtotal=target->statistiques.boomsuffered+target->statistiques.boominflicted;
		
	switch(stat)
	{
		case ST_SCORE:	if(target->statistiques.biggestscore<value) target->statistiques.biggestscore=value;
										break;
		case ST_VICTORY:	target->statistiques.victories++;
											break;
		case ST_LOST:	target->statistiques.defeats++; 
									break;
		case ST_BMIN: target->statistiques.boominflicted++;
									break;
		case ST_BMSU:	target->statistiques.boomsuffered++;
									break;
		case ST_CARD:	
									switch(value)
									{
										case RED:	target->statistiques.played_red++;
															break;
										case GREEN:	target->statistiques.played_green++;
																break;
										case BLUE:	target->statistiques.played_blue++;
																break;	
										case YELLOW:	target->statistiques.played_yellow++;
																	break;
										case SPECIAL:	target->statistiques.played_special++;
																	break;
									}
									break;
		case ST_DRAW: target->statistiques.draws++;
									break;
		case ST_RANK: if(target->statistiques.bestrank>=value) target->statistiques.bestrank=value;
									if(target->statistiques.worserank<=value) target->statistiques.worserank=value;
									if(target->statistiques.bestrank==0) target->statistiques.bestrank=value;
			
									//target->statistiques.currentrank=value+1; // ce sera fait dans SortList (plus tard :{ )
									break;
		case ST_RDWNR:	target->statistiques.rndwnr++;
										break;
	}
	if(boomtotal)
		target->statistiques.boomratio=((double)target->statistiques.boominflicted/boomtotal)*100.0;
	
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


