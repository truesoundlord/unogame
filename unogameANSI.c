/* 
 * File:   unogameSDL2.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 3 avril 2018, 5:02
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// 3 avril 2018: simulation mode console
// Ecriture des fonctions

// fct_initcards
// fct_displaycards
// fct_checkcards

// 4 avril 2018: écriture des fonctions

// checkForSolution
// gameRunning
// getComputerCard
// getComputerColor
// getScore

// 5 avril 2018: écriture des fonctions

// checkForCounterAction

// 7 avril 2018: écriture des fonctions

// FreeMemory

// 16 avril 2018: écriture des fonctions

// UpdateStats
// DisplayStats

// 21 avril 2018: écriture des fonctions

// displayPlayers
// initPlayer
// setPlayerReady

// 30 avril 2018: écriture des fonctions

// SortList
// savestats

// 26 mai 2018: écriture de la fonction isNumeric
// Correction d'un tout petit effet de bord de rien du tout dans la fonction generateName().
// Ajoût de prise en charge de certains paramètres... 

// 27 mai 2018

// Passage de plusieurs fonctions du code source unogameANSI.c vers leurs codes sources
// respectifs.

// fct_generateName
// fct_humanizePlayer
// fct_giveHand
// fct_PlayRound

// 4 novembre 2018 correction du bug lié à la position qui n'est plus mise à jour correctement

// 5 novembre 2018 
// TODO: créer une fonction Uno_Log indiquant qui a joué quoi et quand dans le fichier unogame_(date).log

// 10 janvier 2021
// Ajout de la possibilité de déterminer le nombre de joueurs humains en ligne de commande.
// Par défaut ce sera 1

// **************************************************************************** 
// Fichiers de définitions
// **************************************************************************** 

#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <termios.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
#endif

#ifndef __UEPWIDE__
#include <uep_wide.h>
#endif

#ifndef RESSOURCES_H
#include "ressources.h"
#endif

#ifndef UNOGAME_H
#include "unogame.h"
#endif

// **************************************************************************** 
// Déclaration des constantes symboliques
// **************************************************************************** 

// **************************************************************************** 
// Déclaration des variables globales, externes, ...
// **************************************************************************** 

char strCardPath[MAXPATHLEN];


LinkedList *ll_greencards;
LinkedList *ll_redcards;
LinkedList *ll_yellowcards;
LinkedList *ll_bluecards;
LinkedList *ll_specials;
LinkedList *ll_deck;

LinkedList *ll_players;

t_UNOPlayer Joueurs[MAXPLAYERS];							// pour "conserver" les joueurs sans ça il va falloir jouer avec des "objets persistants" sur disque...  [5 nov 2018]

unsigned short Cards=GAMECARDS;
unsigned short nbhumans=1;										// 2021
bool bHumanizeDone=false;											// 2021

struct sigaction Detournement;


// Partager cette variable avec tous les autres codes sources...
   
extern t_UNOGame *UNO_Saves;

// ****************************************************************************
// SECTION : prototypes des fonctions en test pour CE code source
// ****************************************************************************

void Interruptions(int);


// ...une fois qu'elles fonctionnent déplacer l'implémentation dans un fichier séparé
// et déplacer le prototype dans le fichier de définition(s) (header) correspondant
//  /!\ ne pas oublier d'ajouter les headers dans le code source de la fonction
// dont vous venez de déplacer l'implémentation /!\

/*
 * FONCTION PRINCIPALE
 */

int main(int argc,char** argv)
{
	// Simulation d'une partie de uno version texte d'abord...
	int nombremanches=0;
	int maxmanches=DFLT_ROUNDS;
	int maxplayers=DFLT_PLAYERS;
	
			// détournement des interruptions système...
	Detournement.sa_handler=Interruptions;
	Detournement.sa_flags=0;
		
	sigaction(SIGHUP,&Detournement,NULL);
	sigaction(SIGTSTP,&Detournement,NULL);
	sigaction(SIGCONT,&Detournement,NULL);
	sigaction(SIGINT,&Detournement,NULL);
	sigaction(SIGPIPE,&Detournement,NULL);
	
	// 25 mai 2018
	
	if(argc>1) // il y a des paramètres
	{
		// il y a moyen de faire mieux mais bon...
		if(argv[1]!=NULL) // éviter les erreurs de segmentation
		{
			if(isNumeric(argv[1]))				// est-ce que toute la chaîne contient des nombres ? 
				maxmanches=atoi(argv[1]);
			if(strcmp(argv[1],"--help")==0)
			{
				fprintf(stdout,"[USAGE] ./unogame <nombre de manches> <nombre de joueurs> <joueurs humains>\n"); // mai 2019
				exit(EXIT_SUCCESS);
			}
		}
		if(argv[2]!=NULL) 
		{
			if(isNumeric(argv[2]))
				maxplayers=atoi(argv[2]);
		}
		if(argv[3]!=NULL)
		{
			if(isNumeric(argv[3]))
				nbhumans=atoi(argv[3]);
		}
		if(maxplayers==1) maxplayers=2; // on ne joue pas à uno contre soi-même ^^ (mai 2019)
		if(maxplayers<0 || maxplayers>MAXPLAYERS) maxplayers=DFLT_PLAYERS;
		if(maxmanches<0) maxmanches=DFLT_ROUNDS;
	}

	char *generatedName;
	int cptJoueurs;
	
	for(cptJoueurs=0;cptJoueurs<maxplayers;cptJoueurs++)
	{
		generatedName=generateName(MAXLEN);
		initPlayer(generatedName,false,&Joueurs[cptJoueurs]);				// appelé qu'une seule fois (logiquement) [5 nov 2018]
	}

	UNO_Saves=malloc(sizeof(t_UNOGame));
	UNO_Saves->MaximumRounds=maxmanches;
	
	do
	{
		
		fprintf(stdout,">> MANCHE %02d <<",nombremanches+1);
		UNO_Saves->currentRound=nombremanches;
		Cards=GAMECARDS;
		InitGame(maxplayers);
		
		// InitGame initialise les variables globales ll_deck et ll_players...
		
		UNO_Saves->Cards=ll_deck;
		UNO_Saves->Players=ll_players;
		
		PlayRound(ll_players,ll_deck);
		
		fprintf(stdout,">> MANCHE %02d TERMINEE << \n",nombremanches+1);

		// FreePlayers(ll_players);
		
		lc_empty(ll_deck);
		lc_empty(ll_players);
		lc_empty(ll_redcards);
		lc_empty(ll_greencards);
		lc_empty(ll_bluecards);
		lc_empty(ll_yellowcards);
		
		// Tentative --> le C est "faiblard" concernant "les fuites de mémoires" (memory leaks) mais le code est tellement optimisé qu'on s'en fout finalement ^^
		// Les "memory leaks" sont plus embêtantes dans les applications graphiques en 3D qui bouffent énormément de mémoire (en C++) ^^
		
		/*free(ll_deck);
		free(ll_players);
		free(ll_redcards);
		free(ll_greencards);
		free(ll_bluecards);
		free(ll_yellowcards);*/
				
		sleep(1);
	}while(++nombremanches<maxmanches); // FIN D'UNE MANCHE
	
	// char statfile[255]="\0";
	
	// fprintf(stderr,"[4 nov 2018] test Players: %03d\n",UNO_Saves->Players->NbElem); // 5 nov 2018 --> normal, CONNARD, tu as fait un lc_empty et Players pointe sur ll_Players en ligne 204 :{
		
	// Vu que je suis un gros connard, cette boucle ne s'exécutera JAMAIS (de toutes c'est une TRES MAUVAISE manière de procéder)
	
	/*while(UNO_Saves->Players->NbElem)
	{
		lc_Datas *tmp=lc_pop(UNO_Saves->Players);
		t_UNOPlayer *tmpP=(t_UNOPlayer*)tmp->value;
				
		fprintf(stderr,"[deleting...] %p.stats",tmpP);
		
		sprintf(statfile,"%p.stats",tmpP);
		if(unlink(statfile)) fprintf(stderr,"\t... OK !!");
	}*/
	
	FreePlayers(ll_players);			// 2021
	lc_empty(UNO_Saves->Cards);
	
	fprintf(stdout,"\nPartie terminée...\n");
	
	return(EXIT_SUCCESS);
}

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************


void InitDeck(LinkedList *target)
{
	unsigned short pickedcolor;
	unsigned short pickedcard;
	unsigned short hasard;
	
	lc_Datas *tmp;
	do
	{
		srand(clock());
		
		hasard=rand()%100;
		if(hasard<=97 && hasard>5)
		{
			pickedcolor=rand()%MAXCOLORS;
			
			if(pickedcolor==RED) 
			{
				if(ll_redcards->NbElem==0) continue;
				pickedcard=rand()%ll_redcards->NbElem;
				tmp=lc_get(ll_redcards,pickedcard);
				lc_delete(ll_redcards,lc_getIDat(ll_redcards,pickedcard));
			}
			if(pickedcolor==GREEN) 
			{
				if(ll_greencards->NbElem==0) continue;
				pickedcard=rand()%ll_greencards->NbElem;
				tmp=lc_get(ll_greencards,pickedcard);
				lc_delete(ll_greencards,lc_getIDat(ll_greencards,pickedcard));
			}
			if(pickedcolor==YELLOW) 
			{
				if(ll_yellowcards->NbElem==0) continue;
				pickedcard=rand()%ll_yellowcards->NbElem;
				tmp=lc_get(ll_yellowcards,pickedcard);
				lc_delete(ll_yellowcards,lc_getIDat(ll_yellowcards,pickedcard));
			}
			if(pickedcolor==BLUE) 
			{
				if(ll_bluecards->NbElem==0) continue;
				pickedcard=rand()%ll_bluecards->NbElem;
				tmp=lc_get(ll_bluecards,pickedcard);
				lc_delete(ll_bluecards,lc_getIDat(ll_bluecards,pickedcard));
			}
		}
		else
		{
			// Carte spéciale
			if(ll_specials->NbElem==0) continue;																			// au cas où il faut une carte spéciale et qu'il n'y en a plus... on retourne au "do"...
			pickedcard=rand()%ll_specials->NbElem;
			tmp=lc_get(ll_specials,pickedcard);
			lc_delete(ll_specials,lc_getIDat(ll_specials,pickedcard));
		}
		
		t_Card *deckelem=malloc(sizeof(t_Card));
		if(tmp!=NULL)
			deckelem=(t_Card*)tmp->value;
		else continue;
		
#ifdef DEBUG
		fprintf(stdout,"[%s] Carte [%04d] %s {%s} [%02d]\n",__func__,Cards,unocard_names[deckelem->type],unocard_colors[deckelem->color],deckelem->value);
#endif
		
		Cards--;
		lc_add((void*)deckelem,target,cssmuserdef,sizeof(t_Card));
		nanosleep(&Temporisation,NULL);
	}while(Cards>0);
	
}


// ****************************************************************************
// giveCards
// ****************************************************************************

void giveCards(LinkedList *target, int numberofcards)
{
	while(numberofcards>0)
	{
		lc_Datas *tmp=lc_pop(ll_deck);
		if(tmp==NULL) return;
		t_Card *AddedCard=(t_Card*)tmp->value;
		int idCard=lc_add((void*)AddedCard,target,cssmuserdef,sizeof(t_Card));
		
		// juin 2018
		// Il faut bien intégrer que chaque pointeur "empaqueté" dans lc_Datas est un "nouveau espace mémoire", du coup
		// il faut spécifier la fonction de libération de la mémoire ici aussi :{
		
		// lc_setFreeItemByID(target,idCard,FreeCard);
		
		numberofcards--;
	}
}


// ****************************************************************************
// InitGame
// ****************************************************************************

void InitGame(int maxplayers)
{
	ll_redcards=lc_init();
	ll_greencards=lc_init();
	ll_bluecards=lc_init();
	ll_yellowcards=lc_init();
	ll_specials=lc_init();
	
	ll_players=lc_init();
	
	ll_deck=lc_init();
	
	InitCards(ll_redcards,RED);
	InitCards(ll_greencards,GREEN);
	InitCards(ll_bluecards,BLUE);
	InitCards(ll_yellowcards,YELLOW);
	InitCards(ll_specials,SPECIAL);
		
	fprintf(stdout,"\n[INIT] greens: %03d\n",ll_greencards->NbElem);
	fprintf(stdout,"[INIT] reds: %03d\n",ll_redcards->NbElem);
	fprintf(stdout,"[INIT] blues: %03d\n",ll_bluecards->NbElem);
	fprintf(stdout,"[INIT] yellows: %03d\n",ll_yellowcards->NbElem);
	fprintf(stdout,"[INIT] specials: %03d\n",ll_specials->NbElem);
	
	
	InitDeck(ll_deck);
	fprintf(stdout,"[end] greens: %03d\n",ll_greencards->NbElem);
	fprintf(stdout,"[end] reds: %03d\n",ll_redcards->NbElem);
	fprintf(stdout,"[end] blues: %03d\n",ll_bluecards->NbElem);
	fprintf(stdout,"[end] yellows: %03d\n",ll_yellowcards->NbElem);
	fprintf(stdout,"[end] specials: %03d\n",ll_specials->NbElem);
	
	int cptJoueurs;
	
	// pour les tests (x joueurs)
	
	for(cptJoueurs=0;cptJoueurs<maxplayers;cptJoueurs++)
	{
		setPlayerReady(&Joueurs[cptJoueurs],ll_deck,ll_players);  // je "passe" par un tableau pour ne pas "perdre" l'ordre des joueurs... et pour conserver toujours la même adresse pour chaque joueur
	}
	
	// Pour les tests (joueurs humains)
	#ifndef DEMO
		//int idJoueurH1=lc_getIDat(ll_players,0);
		//int idJoueurH2=lc_getIDat(ll_players,1);
		//int idJoueurH3=lc_getIDat(ll_players,2);
		//int idJoueurH4=lc_getIDat(ll_players,3);
	
		//humanizePlayer(idJoueurH1,ll_players);
		//humanizePlayer(idJoueurH2,ll_players);
		//humanizePlayer(idJoueurH3,ll_players);
		//humanizePlayer(idJoueurH4,ll_players);
	
		// 2021
	
		if(!bHumanizeDone)
		{
			srand(time(NULL));
			int idJoueurHumains[nbhumans];

			int cpthumans=0;
			while(cpthumans<nbhumans)
			{
				int indicehuman=rand()%maxplayers;

				idJoueurHumains[cpthumans]=lc_getIDat(ll_players,indicehuman);
				humanizePlayer(idJoueurHumains[cpthumans],ll_players);
				cpthumans++;
			}
		}
		bHumanizeDone=true;
	#endif
}

// ****************************************************************************
// Interruptions
// ****************************************************************************

void Interruptions(int signal)
{
	struct termios oldt, newt;
	
	fprintf(stderr,"[SIGNAL HANDLING] %03d\n",signal);
	
	switch(signal)
	{
		case SIGINT:	
		case SIGTSTP:	
		case SIGCONT:	
		case SIGPIPE:
		case SIGSEGV:	
		case SIGHUP:
						Detournement.sa_handler=SIG_DFL;
						Detournement.sa_flags=0;
						
						sigaction(SIGHUP,&Detournement,NULL);
						sigaction(SIGTSTP,&Detournement,NULL);
						sigaction(SIGCONT,&Detournement,NULL);
						sigaction(SIGINT,&Detournement,NULL);
						sigaction(SIGPIPE,&Detournement,NULL);
						
						tcgetattr(STDIN_FILENO, &oldt);								// au cas où nous étions dans un getch()...
						newt = oldt;
						newt.c_lflag |= (ICANON | ECHO );							// ...restituer le comportement du terminal par défaut
						
						tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
						lc_empty(UNO_Saves->Cards);
						exit(0);
	}
}

