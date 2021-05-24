/* 
 * File:   fct_generateName.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 27 mai 2018, 1:28
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <linkedlist.h>
#include <time.h>

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
char* generateName(int maxlen)
{
	if(maxlen<=0) return NULL;
	char *tmp=malloc(maxlen);
	char suffixe[6];
	//char suffixe[5]="\0";
	int cpt;
	
	static int order=0;
	
	memset(suffixe,0,6);
	memset(tmp,0,maxlen);
	for(cpt=0;cpt<5;cpt++)
	{
		srand(clock());
		suffixe[cpt]=rand()%26+65;       // nous sommes certains de n'obtenir que des lettres majuscules
		
		nanosleep(&Temporisation,NULL);
	}
	// Bon !! Sans doute que quelque chose a changé dans glibc mais cette méthode
	// ne peut plus fonctionner (septembre 2018)
	//suffixe[cpt]='\0';  // 26 mai 2018 (petit effet de bord de rien du tout ^^)
	sprintf(tmp,"Joueur_%03d_%s",order++,suffixe);
	
	
	return tmp;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


