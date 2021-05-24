/* 
 * File:   UNOLog.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Créé le November 5, 2018, 11:27 PM
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// 5 nov 2018
// Nécessité de journaliser tout ce qui se passe pendant une partie (débugging/vérifications plus faciles)

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

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

void UNOLog(t_UNOPlayer *player,t_Card *cardplayed)
{
	time_t epoch;
	struct tm gmtime;
	//struct timespec rttime;
	
	//char line[160];
	//char strRTime[12];
	char precision[92];
	char strFilename[MAXPATHLEN]=REPLAY;
	
	FILE *output;
	
	if(!player || !cardplayed) return;
	
	umask(0);
	mkdir(REPLAY,S_IRWXO);
	
	time(&epoch);
	gmtime=*localtime(&epoch); // l'étoile '*' sert à déréférencer le pointeur retourné par localtime (voir le prototype de la fonction --> man localtime)
	strftime(precision,sizeof(precision),"%A %d %B %Y -- %02H:%02M:%02S",&gmtime);
	
	//clock_gettime(CLOCK_REALTIME,&rttime);
	//sprintf(strRTime,".%09d",rttime.tv_nsec);
	
	//strcat(precision,strRTime);
	sprintf(strFilename,"%s/replay%02d%02d%04d.dat",REPLAY,gmtime.tm_mday,gmtime.tm_mon+1,gmtime.tm_year+1900);					// 2021 oups :{
	output=fopen(strFilename,"a");
	if(output)
	{
		fprintf(output,"[%s] %s -- [%s][%s%s\x1b[0m][%02d]\n",__func__,precision,player->name,ansicolors[cardplayed->color],unocard_names[cardplayed->type],cardplayed->value);
		fclose(output);
	}
}

// A compléter et tester...

void UNOLogEx(char *formats,...)
{
	va_list listeparametres;
	
	t_UNOPlayer	*s_Player;
	t_Card 			*s_Card;
	e_EVTType 	EventType;
	char 				format[3];					// %P 	joueur
																	// %C 	carte
																	// %m		manche
																	// %T		time stamp
																	// %n		time stamp + nanoseconds
																	
	va_start(listeparametres,formats);
	while(*formats)
	{
		switch(*formats++)
		{
			case 's':
				printf("%s",va_arg(listeparametres,char*));
				break;
			case 'P':
				s_Player=(t_UNOPlayer*)va_arg(listeparametres,t_UNOPlayer*);
				printf("%s",s_Player->name);
				break;
			case 'C':
				s_Card=(t_Card*)va_arg(listeparametres,t_Card*);
				printf("[\x1b[1m%s%s{%s}%s\x1b[0m]",unocard_names[s_Card->type],ansicolors[s_Card->color],unocard_colors[s_Card->color],ansicolors[RESET]);
				break;
			case 'm':
				break;
				
		}
	}
	va_end(listeparametres);
}


// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


