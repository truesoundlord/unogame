/* 
 * File:   playRound.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 27 mai 2018, 1:03
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// 27 mai 2018: export de la fonction playRround de unogameANSI.c (voir étapes de développement)

// Utilisation de variables locales permettant FACILEMENT (copier/coller) de 
// placer la fonction dans un code source séparé pour permettre
// la lecture plus facile du code source, autant pour unogameANSI.c que
// pour PlayRound.c

// 22 juin 2018: tentative de gestion de la sauvegarde de parties sous forme de "slots"...
// SOUCIS: le getch() (caché dans ngetchar()) se trouve à plusieurs endroits, il n'y a pas 
// qu'un seul endroit où il y a une interraction avec le clavier, c'est là que je regrette
// de ne pas avoir mis en place un gestionnaire de clavier :{

// SOLUCE: on s'en fout, nous pourrons sauvegarder que si il y a un joueur humain (ce qui est rassurant ^^)
// au moment où on choisi une carte... point barre, merci, au revoir !!

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <uep_wide.h>
#include <linkedlist.h>
#include <termios.h>
#include <unistd.h>

#include "unogame.h"

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************


// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

t_UNOGame *UNO_Saves;								// déclarée externe dans unogameANSI.c

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

// ****************************************************************************
// PlayRound
// ****************************************************************************

void PlayRound(LinkedList *players, LinkedList *deck)
{
	bool	bTurnRight=true;	
	bool	bCanPlay=false;
	bool	isThereASolution=false;
	bool	bEffectOn=true;
	
	bool	bLoadedGame=false;
	
	char *ALAncienne;
	int iChoixJoueur;
	int whowon=0;																				// qui a gagné
	
	t_Card *TopDesk=NULL;
	t_Card *Played;
	t_Card *CounterAction;
	
	LinkedList *Classement;
	
	unsigned short firstPlayer=rand()%players->NbElem;
	
	lc_Datas *datasPlayer=lc_get(players,firstPlayer);  
	lc_Datas *datasTargetPlayer;
	
	t_UNOPlayer *CurrentPlayer=(t_UNOPlayer*)datasPlayer->value;
	
	// boucler ici
	
	fprintf(stdout,"[START] Le joueur %s commence la partie !!\n",CurrentPlayer->name);
	
	while(gameRunning(players,&whowon))
	{
		fprintf(stdout,"[Current Player:\t%20s]\n",CurrentPlayer->name);
		fprintf(stdout,"Cartes en main: %02d\n",CurrentPlayer->hand->NbElem);
		
		if(CurrentPlayer->statistiques.currentrank!=0) // nov 2018
			fprintf(stdout,"Score: %04d (position: %02d/%02d)\n",CurrentPlayer->score,CurrentPlayer->statistiques.currentrank,UNO_Saves->Players->NbElem);
		
		fprintf(stdout,"IA: %s\n",CurrentPlayer->isHuman?"humain":"ordinateur");
				
		UNO_Saves->CurrentCard=TopDesk;								// variable externe partagée avec le code source unogameANSI.c
		UNO_Saves->CurrentPlayer=CurrentPlayer;
		
		// juin 2018: gestion du un contre un
		if(players->NbElem!=2)
			fprintf(stdout,"Sens de rotation: \x1b[1m%s\x1b[0m",bTurnRight?"Aiguilles d'une montre\n":"Sens contraire\n");
		
		nanosleep(&Lecture,NULL);
		
		if(CurrentPlayer->isHuman)
		{
			// *********************************
			// Afficher la main du joueur humain
			// *********************************
			
			lc_Datas *datasSearch;
			lc_Datas *datasPlayed;
			do
			{
				int cptCarte=0;
				datasSearch=CurrentPlayer->hand->pHead;
				
				// Existe-il une solution ? 
				
				isThereASolution=checkForSolution(CurrentPlayer->hand,TopDesk);
				if(!isThereASolution)
				{
					// NON !! le joueur doit prendre une carte...
					fprintf(stdout,"[GAME] Le joueur %s prends une carte...\n",CurrentPlayer->name);
					giveCards(CurrentPlayer->hand,1);
					bCanPlay=false;
					break; // C'est pourquoi il faut vérifier plus bas si bCanPlay est à true ^^
				}
				
				if(TopDesk!=NULL)
					fprintf(stdout,"---> [\x1b[1m%s\x1b[0m] {%s%s\x1b[0m} <---\n",	unocard_names[TopDesk->type],
																																					ansicolors[TopDesk->color],
																																					unocard_colors[TopDesk->color]);
				
				// Afficher la main du joueur
				
				fprintf(stdout,"Main du joueur: \n");
				while(datasSearch!=NULL)
				{
					t_Card *tmpCarte=(t_Card*)datasSearch->value;
					fprintf(stdout,"[%04d] [\x1b[1m%s\x1b[0m] {%s%s\x1b[0m}\n",	++cptCarte,
																																			unocard_names[tmpCarte->type],
																																			ansicolors[tmpCarte->color],
																																			unocard_colors[tmpCarte->color]);
					datasSearch=datasSearch->pNext;
				}
				
				// ******************************
				// Saisir la carte via son indice
				// ******************************
				if(CurrentPlayer->hand->NbElem>=10) fprintf(stdout,"Choisir une carte entre 1 et %d (5 --> 05 OU 5 puis RETURN)\n",CurrentPlayer->hand->NbElem);
				else fprintf(stdout,"Choisir une carte entre 1 et %d\n",CurrentPlayer->hand->NbElem);
				do
				{
					fflush(stdout);
					fflush(stderr);
					ALAncienne=getnchar((CurrentPlayer->hand->NbElem/10)+1);				// en fonction du nombre maximum de choix (un ou deux caractères)
					if(ALAncienne[0]==19) // CTRL-S <-> ASCII 19
					{
						saveGame(UNO_Saves);
					}
					// 22 juin 2018
					// Avec un gestionnaire de clavier ce genre de saloperie ne devrait pas exister :{ 
					if(ALAncienne[0]==12)
					{
						lc_empty(deck);
						lc_empty(players);
						int currentID=loadGame(UNO_Saves);
						if(currentID>=1000) // gen_number commence à 1000, si on a un nombre supérieur il y a des chances que nous ayons un ID valide
						{
							datasPlayer=lc_search(players,currentID);
							CurrentPlayer=(t_UNOPlayer*)datasPlayer->value;
							bTurnRight=UNO_Saves->bDirection;
							bLoadedGame=true;
						}
						else bLoadedGame=false;
						break;
					}
					iChoixJoueur=atoi(ALAncienne);
				}while(iChoixJoueur<1 || iChoixJoueur>CurrentPlayer->hand->NbElem);
				
				// 2021
				
				int caca=nbgetchEx();
				if(caca!=-1)
					fprintf(stderr,"[DEBUG 0] --> \x1b[5;1m%03d\x1b[0m <-- \n",caca);
				
				// end 2021
				
				// 22 juin 2018: c'est pas très beau mais bon...
				
				if(bLoadedGame) break;   // ce n'est pas beau du tout :{ mais il faut bien sortir de toutes ces boucles imbriquées :{
				
				// Récupérer la carte de la main du joueur
				
				iChoixJoueur--;
				
				datasPlayed=lc_get(CurrentPlayer->hand,iChoixJoueur);							// iChoixJoueur est un indice...   
				Played=(t_Card*)datasPlayed->value;
				
				fprintf(stdout,"-----------------------------------------------------------------\n");
				fprintf(stdout,"Carte choisie: %s {%s%s\x1b[0m}\n",	unocard_names[Played->type],
																														ansicolors[Played->color],
																														unocard_colors[Played->color]);
				fprintf(stdout,"-----------------------------------------------------------------\n");
				
				// ***************************************
				// Vérifier si cette carte peut être jouée
				// ***************************************
				
				if(TopDesk!=NULL) 
				{
					bCanPlay=CheckCardPlayed(TopDesk,Played,CurrentPlayer->hand);			// Amélioration le 24 avril 2018
				}
				else // si aucune carte n'a encore été jouée elle est automatiquement validée (c'est logique)
				{
					TopDesk=Played;
					bCanPlay=true;
				} 
				nanosleep(&Lecture,NULL);
			}while(!bCanPlay);
			
			// 22 juin 2018
			if(bLoadedGame)					// je me dégoûte... j'avais pas "réfléchi" à la partie "load" de la partie...
			{
				bLoadedGame=false;
				continue;							// inutile de faire la suite des opérations, on retourne directement en début de la boucle principale...
			}
			
			// ***************************************
			// Jouer la carte
			// ***************************************
			
			if(bCanPlay) // Le joueur a joué une carte... (bCanPlay est à faux quand le joueur n'a pas su jouer une carte)
			{
					// La carte est jouée et donc retirée de la main du joueur...
				TopDesk=Played;
				fprintf(stdout,"-----------------------------------------------------------------\n");
				fprintf(stdout,"Carte jouée: %s {%s%s\x1b[0m}\n",	unocard_names[TopDesk->type],
																													ansicolors[TopDesk->color],
																													unocard_colors[TopDesk->color]);
				fprintf(stdout,"-----------------------------------------------------------------\n");
				// juillet 2018
				if(TopDesk->type>=NOTURN) // si le type est plus grand ou égal à 10 il y a un effet
					bEffectOn=true; // une carte a été jouée donc il faut (peut-être) prendre en compte ses effets...

				lc_delete(CurrentPlayer->hand,datasPlayed->item_Number);
				
				// Journaliser (replay ??)
		
				UNOLog(CurrentPlayer,TopDesk);
			}
		}	
		else
		{
			// I.A. à faire (faite ^^) --> Ici nous faisons "jouer" l'ordinateur...
			int itemNumber;
			
			// *********************************************
			// Vérifier si l'ordinateur peut jouer une carte
			// *********************************************
						
			t_Card *computerChoice=getComputerCard(CurrentPlayer->hand,TopDesk,&itemNumber);
			
			
			if(TopDesk!=NULL)
					fprintf(stdout,"---> [\x1b[1m%s\x1b[0m] {%s%s\x1b[0m} <---\n",	unocard_names[TopDesk->type],
																																					ansicolors[TopDesk->color],
																																					unocard_colors[TopDesk->color]);
			
			// *********************************************
			// Vérifier si l'ordinateur a pu jouer une carte
			// *********************************************
			
			if(computerChoice!=NULL) // OUI
			{
				fprintf(stdout,"-----------------------------------------------------------------\n");
				fprintf(stdout,"[COMP] Choix de l'ordinateur: \x1b[1m%s\x1b[0m {%s%s\x1b[0m} [item: %05d]\n",	unocard_names[computerChoice->type],
																																																			ansicolors[computerChoice->color],
																																																			unocard_colors[computerChoice->color],itemNumber);
				fprintf(stdout,"-----------------------------------------------------------------\n");
				TopDesk=computerChoice;
				// juillet 2018
				if(TopDesk->type>=NOTURN) // si le type est plus grand ou égal à 10 il y a un effet
					bEffectOn=true;
				lc_delete(CurrentPlayer->hand,itemNumber);
				
				// Journaliser (replay ??)
		
				UNOLog(CurrentPlayer,TopDesk);
			}
			else // NON
			{
				fprintf(stdout,"[GAME] Le joueur %s prends une carte...\n",CurrentPlayer->name);
				giveCards(CurrentPlayer->hand,1);
			}
		}
		nanosleep(&Lecture,NULL);
		fflush(stdout);
		fflush(stderr);
		
		// *********************************************
		// Mettre à jour les stats
		// *********************************************
		
		UNO_Saves->bDirection=bTurnRight; // 22 juin 2018		
		UpdateStats(CurrentPlayer,ST_CARD,TopDesk->color);
				
		//************************************************************************
		// Analyse des effets...
		//************************************************************************
		
		if(bEffectOn)  // aurait fonctionné avec TopDesk!=NULL
		{
			//************************************************************************
			// CHANGETURN
			//************************************************************************
						
			if(TopDesk->type==CHANGETURN)
			{
				if(bTurnRight) bTurnRight=false;
				else bTurnRight=true;
			}

			// Déterminer le joueur cible (à droite ou à gauche en fonction du sens)
			//fprintf(stdout,"[DEBUG] START %05d (%s) (%08x)\n",__LINE__,bTurnRight?"right":"left",datasPlayer);
		
			if(bTurnRight)
			{
					// le joueur qui se trouve directement à droite
				datasTargetPlayer=datasPlayer->pNext;
				if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
			}
			else
			{
				// le joueur qui se trouve directement à gauche
				datasTargetPlayer=datasPlayer->pPrevious;
				if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
			}
			
			//fprintf(stdout,"[DEBUG %05d] %08x\n",__LINE__,datasPlayer);
			//fprintf(stdout,"[DEBUG %05d] %08x\n",__LINE__,datasPlayer->pNext);
			
			t_UNOPlayer *TargetPlayer=(t_UNOPlayer*)datasTargetPlayer->value;
			
			//************************************************************************
			// CHANGETURN passer son tour
			//************************************************************************
			
			if(players->NbElem==2 && TopDesk->type==CHANGETURN) 
			{
				if(bTurnRight)
				{
					datasTargetPlayer=datasTargetPlayer->pNext;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
				}
				else
				{
					datasTargetPlayer=datasTargetPlayer->pPrevious;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
				}
				fprintf(stdout,"[Changeturn 2 players][Target player] %05d (%s)\n",datasTargetPlayer->item_Number,TargetPlayer->name);
				sleep(1);
			}
			//fprintf(stdout,"[GAME] TARGET PLAYER: %05d \x1b[1m%s\x1b[0m\n",datasTargetPlayer->item_Number,TargetPlayer->name);
			
			//************************************************************************
			// TWO CARDS prendre deux cartes dans sa gueulle et passer son tour
			//************************************************************************
			if(TopDesk->type==TWOCARDS)
			{
				giveCards(TargetPlayer->hand,2);
				if(bTurnRight)
				{
					datasTargetPlayer=datasTargetPlayer->pNext;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
				}
				else
				{
					datasTargetPlayer=datasTargetPlayer->pPrevious;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
				}
				fprintf(stdout,"[+2][Target player] %05d (%s)\n",datasTargetPlayer->item_Number,TargetPlayer->name);
				sleep(1);
			}

			//************************************************************************
			// NO TURN passer son tour
			//************************************************************************

			if(TopDesk->type==NOTURN)
			{
				if(bTurnRight)
				{
					datasTargetPlayer=datasTargetPlayer->pNext;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
				}
				else
				{
					datasTargetPlayer=datasTargetPlayer->pPrevious;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
				}
				fprintf(stdout,"[No Turn][Target player] %05d (%s)\n",datasTargetPlayer->item_Number,TargetPlayer->name);
				sleep(1);
			}

			//************************************************************************
			// CHANGE COLOR choisir couleur
			//************************************************************************

			// Si il faut jouer une carte en plus de changer la couleur (jouer deux cartes au lieu d'une)
			// il suffit de conserver le joueur actuel (CurrentPlayer) et de reboucler (continue) à la fin du traitement
			// de la carte CHANGE COLOR ^^
			if(TopDesk->type==CHANGECOLOR)
			{
				if(CurrentPlayer->isHuman)
				{
					// Demander la couleur si humain
					int		Color;
					char	tmpColor;
					fprintf(stdout,"[Change color] Choisissez une couleur:\n");
					for(Color=0;Color<MAXCOLORS;Color++)
					{
						fprintf(stdout,"[%02d] %s%s\x1b[0m\n",Color+1,ansicolors[Color],unocard_colors[Color]);
					}
					
					// BUG: de temps en temps le programme semble attendre de vider le buffer (???)
					
					fflush(stdin);
					tcflush(fileno(stdin),TCIFLUSH);
					do
					{
						fflush(stdout);
						fflush(stderr);
						fprintf(stdout,"\n");
						tmpColor=getch();
						tmpColor-=48;
					}while(tmpColor<1 || tmpColor>MAXCOLORS);
					
					TopDesk->color=tmpColor-1;
					fprintf(stdout,"[CC] Nous allons en %s%s\x1b[0m\n",ansicolors[TopDesk->color],unocard_colors[TopDesk->color]);
				}
				else
				{
					TopDesk->color=getComputerColor(CurrentPlayer->hand,TopDesk);
				}
				sleep(1);
			}

			//************************************************************************
			// BOOM (+4) changer de couleur et passer son tour
			//************************************************************************
			
			if(TopDesk->type==BOOM)
			{
				int TotalCards=4;
				
				// Si il y a moins que 10 joueurs, chaque "contre" diminue de 25 points le score actuel du joueur... comme il n'y a pas de -50 points en cas de victoire.
				
				// IDEE: chercher dans la main des joueurs qui suivent (dans le sens de rotation)
				// si une carte "BOUM" ne serait pas en leur possession, si c'est le cas, le nombre
				// de cartes va s'ajouter et se reportera au joueur qui n'aura pas la possibilité de "suivre"
				
				//************************************************************************
				// COUNTERACTION
				//************************************************************************
				// Vérifier si le joueur cible (TargetPlayer) a une carte BOUM dans sa main...
				
				fprintf(stdout,"[COUNTERACTION] TARGET --> %s (%05d)\n",TargetPlayer->name,datasTargetPlayer->item_Number);
				nanosleep(&Lecture,NULL);
				
				do
				{
					fprintf(stdout,"[COUNTERACTION] Est-ce que [%s] a de quoi contrer ???\n",TargetPlayer->name);
					nanosleep(&Lecture,NULL);
#ifdef SPY
					displayCards(TargetPlayer);
#endif
					CounterAction=checkForCounterAction(TargetPlayer->hand,TopDesk);					
					// changer de cible tant qu'il y a une carte BOUM en main
					if(CounterAction)
					{
						UpdateStats(TargetPlayer,ST_CARD,SPECIAL); // oubli (pour les stats) juin 2018
						if(players->NbElem<MAXPLAYERS) TargetPlayer->score-=25; // juin 2018
						fprintf(stdout,"[COUNTERACTION] \x1b[5m -- %s a contré la carte BOUM --\x1b[0m\n",TargetPlayer->name);
						
						UNOLog(TargetPlayer,TopDesk);
						
						nanosleep(&Lecture,NULL);
						if(bTurnRight)
						{
							datasTargetPlayer=datasTargetPlayer->pNext;
							if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
						}
						else
						{
							datasTargetPlayer=datasTargetPlayer->pPrevious;
							if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
						}
						TotalCards+=4;
						TargetPlayer=(t_UNOPlayer*)datasTargetPlayer->value;
						fprintf(stdout,"[COUNTERACTION] Nouveau joueur cible (COMBO): %s (%d cards)\n",TargetPlayer->name,TotalCards);
						nanosleep(&Lecture,NULL);
						
					}
				}while(CounterAction);

				//fprintf(stdout,"[DEBUG] BOUM -- \x1b[1m%s\x1b[0m <- %02d\n",TargetPlayer->name,TotalCards);								
				giveCards(TargetPlayer->hand,TotalCards);
				
				if(CurrentPlayer->isHuman)			// c'est le joueur qui pose la carte "+4" qui choisi la couleur (même en cas de "counteraction")...
				{
					// Demander la couleur si humain
					int		Color;
					char	tmpColor;
					fprintf(stdout,"[BOOM] Choisissez une couleur:\n");
					for(Color=0;Color<MAXCOLORS;Color++)
					{
						fprintf(stdout,"[%02d] %s%s\x1b[0m\n",Color+1,ansicolors[Color],unocard_colors[Color]);
					}
					
					// BUG: de temps en temps le programme semble attendre de vider le buffer (???)
					
					fflush(stdin);
					tcflush(fileno(stdin),TCIFLUSH);
					do
					{
						fflush(stdout);
						fflush(stderr);
						fprintf(stdout,"\n");
						tmpColor=getch();
						tmpColor-=48;
					}while(tmpColor<1 || tmpColor>MAXCOLORS);
					TopDesk->color=tmpColor-1;
					fprintf(stdout,"[BOOM] Nous allons en %s%s\x1b[0m\n",ansicolors[TopDesk->color],unocard_colors[TopDesk->color]);
				}
				else
				{
					TopDesk->color=getComputerColor(CurrentPlayer->hand,TopDesk);
				}
				
				if(bTurnRight)
				{
					datasTargetPlayer=datasTargetPlayer->pNext;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
				}
				else
				{
					datasTargetPlayer=datasTargetPlayer->pPrevious;
					if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
				}
				fprintf(stdout,"\x1b[5m[BOOM]\x1b[0m[Target player] %05d (%s) [%+d]\n",datasTargetPlayer->item_Number,TargetPlayer->name,TotalCards);
				UpdateStats(TargetPlayer,ST_BMSU,0);				// modification des stats (suffered)
				UpdateStats(CurrentPlayer,ST_BMIN,0);				// modification des stats (inflicted)
				nanosleep(&Lecture,NULL);
			}
			bEffectOn=false; // fin du traitement des effets...
		}
		else // si la carte ne provoque pas d'effet... (juillet 2018: pas certain que ce soit nécessaire de placer cela ici -- à vérifier)
		{
			//fprintf(stdout,"[CHECK EFFECTS] %s\n",bEffectOn?"yes":"no");
			if(bTurnRight)
			{
					// le joueur qui se trouve directement à droite
				datasTargetPlayer=datasPlayer->pNext;
				if(datasTargetPlayer==NULL) datasTargetPlayer=players->pHead;
			}
			else
			{
				// le joueur qui se trouve directement à gauche
				datasTargetPlayer=datasPlayer->pPrevious;
				if(datasTargetPlayer==NULL) datasTargetPlayer=players->pTail;
			}
		}
	
		nanosleep(&Lecture,NULL);
		
		// Situation du jeu (lister les joueurs)
								
		lc_Datas *datasStatus=players->pHead;
		while(datasStatus!=NULL)
		{
			//fprintf(stdout,"[Joueur %05d]\n",datasStatus->item_Number);

			t_UNOPlayer *tmpPlayer=(t_UNOPlayer*)datasStatus->value;
			
			int handscore=getScore(tmpPlayer->hand);
			
			if(tmpPlayer->isHuman)
			{
				fprintf(stdout,"[%s] value %03d\n",tmpPlayer->name,handscore);
				displayCards(tmpPlayer);
			}
			else
			{
				char WARNING[10];
				if(tmpPlayer->hand->NbElem==1) strcpy(WARNING,"\x1b[5m\x1b[1m");
				else strcpy(WARNING,"\x1b[1m");
				fprintf(stdout,"[%s] Nombre de cartes: %s%03d\x1b[0m (value: %03d)\n",tmpPlayer->name,WARNING,tmpPlayer->hand->NbElem,handscore);
			}
			
			datasStatus=datasStatus->pNext;
		}
		fprintf(stdout,"-----------------------------------------------------------------------------\n");
		nanosleep(&Lecture,NULL);
		Classement=NULL;

		//fprintf(stdout,"[End of turn][Next player] %05d\n",datasTargetPlayer->item_Number);
		
		// Changer de joueur...
		
		CurrentPlayer=(t_UNOPlayer*)datasTargetPlayer->value;
		datasPlayer=datasTargetPlayer;
		
		//************************************************************************************************
		// Si il n'y a plus de cartes la partie est arrêtée et nous comptons les points
		// dans chaque main des joueurs... 
		//************************************************************************************************
		
		UNO_Saves->CurrentCard=TopDesk;								// variable externe partagée avec le code source unogameANSI.c
		UNO_Saves->CurrentPlayer=CurrentPlayer;
		
		if(deck->NbElem==0)
		{
			// ATTENTION: C'est un peu confus car j'ai pensé aux stats qu'après avoir réfléchi à la manière de gérer la fin de partie, il faudra remettre cela "au propre" :{
			// 1. Modifier les scores de tous les joueurs
			
			fprintf(stdout,"[GAME] No more cards in deck !!\n");
			
			// 2021
			int roundwinnerscore=1000;
			int idRoundWinner=-1;
			
			lc_Datas *parcours=players->pHead;
			while(parcours)
			{
				t_UNOPlayer *tmpPlayer=(t_UNOPlayer*)parcours->value;
				
				int handscore=getScore(tmpPlayer->hand);
				
				// 2021
				if(handscore<roundwinnerscore)
				{
					roundwinnerscore=handscore;
					idRoundWinner=parcours->item_Number;
				}
				
				UpdateStats(tmpPlayer,ST_SCORE,handscore);
				fprintf(stdout,">> %s SCORE %04d {%+04d} [%s]\n",tmpPlayer->name,tmpPlayer->score,handscore,tmpPlayer->isHuman?"human":"computer");
				tmpPlayer->score+=handscore;
				UpdateStats(tmpPlayer,ST_DRAW,0);
				parcours=parcours->pNext;
			}
			
			// 2021
			if(idRoundWinner!=-1) 
			{
				t_UNOPlayer *tmp=(t_UNOPlayer*)(lc_search(players,idRoundWinner)->value);
				fprintf(stdout,"[Gagnant de la manche] \x1b[1m%s\x1b[0m avec \x1b[1m%03d\x1b[0m points\n",tmp->name,roundwinnerscore);
				tmp->score-=15;										// Bonus supplémentaire
				UpdateStats(tmp,ST_RDWNR,0);
			}
			
			// 2. Trier les joueurs en fonction de leurs scores respectifs
			
			Classement=SortList(players);
			break;
		}
		else
			fprintf(stdout,"[GAME] Card in deck %03d\n",deck->NbElem);
		
		fflush(stdout);
		fflush(stderr);
		
		// BUG: de temps en temps je suis bloqué sur une sorte de getch... je sais pas bien d'où ça vient (????) // 2021
		int debug=nbgetch(); // devrait vider le buffer
		if(debug!=-1)
			fprintf(stderr,"[DEBUG 1] --> \x1b[5;1m%03d\x1b[0m <-- \n",debug);
	} // endwhile
	
	// ATTENTION: C'est un peu confus car j'ai pensé aux stats qu'après avoir réfléchi à la manière de gérer la fin de partie, il faudra remettre cela "au propre" :{
		
	if(whowon)
	{
		// 1. Récupérer un pointeur sur le joueur qui a remporté la manche
		lc_Datas *winneris=lc_search(players,whowon);
		t_UNOPlayer *winner=(t_UNOPlayer*)winneris->value;
	
		fprintf(stdout,"[WINNER]\n%s SCORE: %04d\n",winner->name,winner->score);
		
		// 2. Mettre à jour le compteur de victoires pour ce joueur
		
		UpdateStats(winner,ST_VICTORY,0);
				
		// 3. Faire de même avec tous les autres joueurs...
	
		lc_Datas *Loosers=players->pHead;
		while(Loosers)
		{
			t_UNOPlayer *looser=(t_UNOPlayer*)Loosers->value;
			if(Loosers->item_Number!=winneris->item_Number)
			{
				int handscore=getScore(looser->hand);
				
				fprintf(stdout,"[LOOSER]\n%s HAND: %05d\n",looser->name,handscore);
				
				// 3a. afficher les cartes restantes dans la main de chaque joueur "perdant"
				
				displayCards(looser);
				
				// 3b. Mettre à jour les statistiques 
				
				UpdateStats(looser,ST_SCORE,handscore);				// le score relatif aux cartes en main
				UpdateStats(looser,ST_LOST,0);								// modifier le compteur de manches perdues
				
				fprintf(stdout,">> SCORE: %05d <<\n",looser->score+=handscore);
			}
			Loosers=Loosers->pNext;
		}
		
		// 4. Trier les joueurs selon leurs scores et déterminer le classement
		
		Classement=SortList(players);
	} 
	
	if(Classement!=NULL)
		displayPlayers(Classement);
	sleep(1);

	// 5. Afficher les statistiques pour chaque joueur (Pas super mais bon la fonction DisplayStat() se réfère aux statistiques d'UN joueur pas de tous les joueurs :{ )
	
	if(UNO_Saves->MaximumRounds>=5)
		savestats(players);   // ATTENTION: je refais un SortList() dans la fonction savestats() :{ (TODO: faire un savestats sur Classement et enlever le SortList ?)
	
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


