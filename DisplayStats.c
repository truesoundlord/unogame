/* 
 * File:   DisplayStats.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 16 avril 2018, 23:25
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

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
void DisplayStats(t_UNOPlayer *target)
{
	float ratio=	target->statistiques.played_red+target->statistiques.played_green+target->statistiques.played_blue+
								target->statistiques.played_yellow+target->statistiques.played_special;
	
	fprintf(stderr,"---------------------------------------------------\n");
	fprintf(stderr,"[STATS %s] >>\n",target->name);
	
	fprintf(stderr,"[Score] MAX:\t\t%05d\n\n",target->statistiques.biggestscore);
	
	if(target->statistiques.bestrank!=0)
		fprintf(stderr,"[Position] Best rank:\t%02d\n",target->statistiques.bestrank);
	if(target->statistiques.worserank!=0)
		fprintf(stderr,"[Position] Worse rank:\t%02d\n\n",target->statistiques.worserank);
	
	fprintf(stderr,"[Victories]\t\t%05d\n",target->statistiques.victories);
	fprintf(stderr,"[Defeats]\t\t%05d\n",target->statistiques.defeats);
	fprintf(stderr,"[Draws]\t\t\t%05d\n\n",target->statistiques.draws);
	
	fprintf(stderr,"[Boom inflicted]\t%05d\n",target->statistiques.boominflicted);
	fprintf(stderr,"[Boom suffered]\t\t%05d\n",target->statistiques.boomsuffered);
	fprintf(stderr,"[Boom ratio]\t\t%.1f %%\n\n",target->statistiques.boomratio);
	
	
	if(ratio) // éviter la division par 0
	{
		fprintf(stderr,"[Red]\t\t\t%05d (%03.0f%%)\n",target->statistiques.played_red,(target->statistiques.played_red/ratio)*100.0);
		fprintf(stderr,"[Green]\t\t\t%05d (%03.0f%%)\n",target->statistiques.played_green,(target->statistiques.played_green/ratio)*100.0);
		fprintf(stderr,"[Blue]\t\t\t%05d (%03.0f%%)\n",target->statistiques.played_blue,(target->statistiques.played_blue/ratio)*100.0);
		fprintf(stderr,"[Yellow]\t\t%05d (%03.0f%%)\n",target->statistiques.played_yellow,(target->statistiques.played_yellow/ratio)*100.0);
		fprintf(stderr,"[Special]\t\t%05d (%03.0f%%)\n",target->statistiques.played_special,(target->statistiques.played_special/ratio)*100.0);
	}
		
	fprintf(stderr,"---------------------------------------------------\n");
	//sleep(2);
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


