/* 
 * File:   savestats.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 30 avril 2018, 22:05
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <time.h>
#include <locale.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
#endif

#ifndef UNOGAME_H
#include "unogame.h"
#endif


// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************

#define SZBUF	255

// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

struct tm *s_Date;
time_t	rawtime;

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************
int savestats(LinkedList *target)
{
	LinkedList *SortedList;
	FILE *descFichier;
	
	static unsigned short manche=1;						// 2021 janvier
	
	char strDate[SZBUF]="\0";
	char filename[MAXPATHLEN];
	//char strOutputLine[MAXOUTPUT];
	
	time(&rawtime);
	
	setlocale(LC_ALL,"fr_BE.utf8");
	
	s_Date=localtime(&rawtime);
	strftime(strDate,SZBUF,"%d%B_(%H%M)",s_Date);
	
	if(target==NULL) return -1;
	SortedList=SortList(target);
	if(SortedList==NULL) return -2;
	
	int verif;
	
	sprintf(filename,"%s/%s_manche%02d",STATDIR,strDate,manche);
	
	// S'assurer que le répertoire existe bel et bien
	
	umask(0);
	mkdir(STATDIR,S_IRWXO);
	
	fprintf(stderr,"[DEBUG] %s\n",filename);
	descFichier=fopen(filename,"w");
	
	if(descFichier==NULL)
	{
		perror("[I/O Error: ");
		printf("]\n");
		return -4;
	}
	
	fprintf(descFichier,"[CLASSEMENT]\n");
	for(verif=0;verif<SortedList->NbElem;verif++)
	{
		lc_Datas *parcours=lc_get(SortedList,verif);
		if(parcours==NULL) return -3;
		t_UNOPlayer *joueur=(t_UNOPlayer*)parcours->value;
		
		float ratio=	joueur->statistiques.played_red+joueur->statistiques.played_green+joueur->statistiques.played_blue+
									joueur->statistiques.played_yellow+joueur->statistiques.played_special;
		
		fprintf(descFichier,"[%02d][%s] (%05d) [%02d/%02d]\n",verif+1,joueur->name,joueur->score,joueur->statistiques.currentrank,target->NbElem);
		
		fprintf(descFichier,"-[%-2s]--------------------------------------------------\n",joueur->isHuman?"H":"IA");
		fprintf(descFichier,"[STATISTIQUES]\n");
		
		fprintf(descFichier,"[Score] MAX:\t\t%05d\n\n",joueur->statistiques.biggestscore);
	
		if(joueur->statistiques.bestrank!=0)
			fprintf(descFichier,"[Position] Best rank:\t\t%02d\n",joueur->statistiques.bestrank);
		if(joueur->statistiques.worserank!=0)
			fprintf(descFichier,"[Position] Worse rank:\t%02d\n\n",joueur->statistiques.worserank);
	
		fprintf(descFichier,"[Victories]\t\t\t%05d\n",joueur->statistiques.victories);
		fprintf(descFichier,"[Defeats]\t\t\t\t%05d\n",joueur->statistiques.defeats);
		fprintf(descFichier,"[Draws]\t\t\t\t\t%05d\n",joueur->statistiques.draws);
		fprintf(descFichier,"[Round Winner]\t%05d\n\n",joueur->statistiques.rndwnr);
	
		fprintf(descFichier,"[Boom inflicted]\t%05d\n",joueur->statistiques.boominflicted);
		fprintf(descFichier,"[Boom suffered]\t\t%05d\n",joueur->statistiques.boomsuffered);
		fprintf(descFichier,"[Boom ratio]\t\t\t%.1f %%\n\n",joueur->statistiques.boomratio);
	
		fprintf(descFichier,"[Red]\t\t\t\t%05d (%2.0f%%)\n",joueur->statistiques.played_red,(joueur->statistiques.played_red/ratio)*100.0);
		fprintf(descFichier,"[Green]\t\t\t%05d (%2.0f%%)\n",joueur->statistiques.played_green,(joueur->statistiques.played_green/ratio)*100.0);
		fprintf(descFichier,"[Blue]\t\t\t%05d (%2.0f%%)\n",joueur->statistiques.played_blue,(joueur->statistiques.played_blue/ratio)*100.0);
		fprintf(descFichier,"[Yellow]\t\t%05d (%2.0f%%)\n",joueur->statistiques.played_yellow,(joueur->statistiques.played_yellow/ratio)*100.0);
		fprintf(descFichier,"[Special]\t\t%05d (%2.0f%%)\n",joueur->statistiques.played_special,(joueur->statistiques.played_special/ratio)*100.0);
	
		fprintf(descFichier,"-------------------------------------------------------\n");
		joueur->statistiques.oldrank=verif+1;
		fflush(descFichier);
	}
	manche++;									// 2021 janvier
	fclose(descFichier);
	return 0;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


