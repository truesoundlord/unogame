/* 
 * File:   unogame.h
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 3 avril 2018, 5:04
 */

#ifndef UNOGAME_H
#define UNOGAME_H

#include <stdbool.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <string.h>

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

// **************************************************************************** 
// Définitions des constantes symboliques
// **************************************************************************** 

#define GAMECARDS	108
#define MAXCARDS	15
#define MAXLEN		25
#define MAXCOLORS	4


//#define MAXROUNDS	5

#define MAXPLAYERS		10
#define DFLT_PLAYERS	4
#define DFLT_ROUNDS		5
#define HANDCARDS			7

#define STATDIR	"./stats"
#define REPLAY	"./replay"

#define NORMALSPEED

// ****************************************************************************
// SECTION : variables externes/globales/...
// ****************************************************************************

static char unocard_names[MAXCARDS][MAXLEN]={	"Zéro","Un","Deux","Trois","Quatre","Cinq","Six","Sept","Huit","Neuf",
																							"PASSE TOUR","INVERSER SENS","PIOCHE DEUX (+2)","WILD (choisir couleur)","WILD PIOCHE QUATRE (+4)"};
static int unocard_values[MAXCARDS]={0,1,2,3,4,5,6,7,8,9,10,10,10,25,50};
static char unocard_colors[MAXCOLORS+1][MAXLEN]={"Rouge","Vert","Bleu","Jaune","Spécial"};

static char ansicolors[MAXCOLORS+2][MAXLEN]=	{
																								"\x1b[38;2;255;0;0;1m",								// ROUGE
																								"\x1b[38;2;0;255;0;1m",								// VERT	
																								"\x1b[38;2;45;115;200;1m",						// BLEU
																								"\x1b[38;2;255;215;0;1m",							// JAUNE
																								"\x1b[38;2;168;221;224;5;1m",					// SPECIAL
																								"\x1b[0m"															// RESET
																							};

#ifdef NORMALSPEED
static struct timespec Temporisation={0L,50000000L}; 
static struct timespec Lecture={0L,750000000L};					
#else
static struct timespec Temporisation={0L,25000000L};
static struct timespec Lecture={0L,250000000L};					
#endif


// **************************************************************************** 
// Définition(s) des structures, types et énumérations
// **************************************************************************** 

typedef enum{NOTURN=10,CHANGETURN,TWOCARDS,CHANGECOLOR,BOOM}e_Cards;
typedef enum{INVALID,RED=0,GREEN,BLUE,YELLOW,SPECIAL,RESET}e_Colors;
typedef enum{ST_SCORE,ST_VICTORY,ST_LOST,ST_BMIN,ST_BMSU,ST_CARD,ST_DRAW,ST_RANK,ST_RDWNR}e_stats;
typedef enum{EVT_CARDP,EVT_CARDT,EVT_COLORC}e_EVTType;

typedef struct s_Card
{
	int type;
	int value;
	int color;
	
	// bool	bReversed;
	
	//SDL_Texture	*t_Card;
	//SDL_Rect		position;
	
}t_Card;

typedef struct s_stats
{
	int biggestscore;
	int bestrank;
	int currentrank;
	int oldrank;
	int worserank;
	
	int victories;
	int defeats;
	int draws;
	int rndwnr;
		
	int boominflicted;
	int boomsuffered;
	double boomratio;
	
	int played_red;
	int played_blue;
	int played_green;
	int played_yellow;
	int played_special;
}t_stats;

typedef struct s_UNOPayer
{
	LinkedList	*hand;
	t_stats			statistiques;
	
	int		score;
	bool	isHuman;
	char	*name;
	
}t_UNOPlayer;


typedef struct s_savedscores
{
	char *playername;
	unsigned int score;
}t_savedscores;


typedef struct s_Map
{
	t_UNOPlayer *player;
	t_stats	*stats;
}t_Map;

typedef struct s_UNOGame
{
	LinkedList *Players;
	LinkedList *Cards;
	
	int	currentRound;
	int MaximumRounds;
	
	t_Card *CurrentCard;
	t_UNOPlayer *CurrentPlayer;
	
	bool bDirection;
	
}t_UNOGame;



// **************************************************************************** 
// Définition(s) des fonctions
// **************************************************************************** 
void InitCards(LinkedList*,e_Colors);
void InitDeck(LinkedList*);
void giveHand(LinkedList*,LinkedList*);
void FreePlayers(LinkedList*);

void PlayRound(LinkedList*, LinkedList*);
void giveCards(LinkedList*,int);
void displayCards(t_UNOPlayer*);

bool CheckCardPlayed(t_Card*,t_Card*,LinkedList*);
bool checkForSolution(LinkedList*,t_Card*);
bool gameRunning(LinkedList*,int*);
bool isNumeric(char*);

t_Card* checkForCounterAction(LinkedList*,t_Card*);
t_Card* getComputerCard(LinkedList*,t_Card*,int*);

e_Colors getComputerColor(LinkedList*,t_Card*);

void SortScores(LinkedList*);
void DisplayScores(LinkedList*);

void SaveScores(LinkedList*);
void LoadScores(LinkedList*);

//void FreePlayer(void*);
//void FreeCard(void*);

void InitGame(int);
void UpdateStats(t_UNOPlayer*,e_stats,int);
void DisplayStats(t_UNOPlayer*);
void humanizePlayer(int,LinkedList*);
void initPlayer(char*,_Bool,t_UNOPlayer*);
void displayPlayers(LinkedList*);

void UNOLog(t_UNOPlayer*,t_Card*);

int setPlayerReady(t_UNOPlayer*,LinkedList*,LinkedList*);
int savestats(LinkedList *);
int getScore(LinkedList *);
int loadGame(t_UNOGame *);
int saveGame(t_UNOGame *);

char* generateName(int);

LinkedList* SortList(LinkedList*);

#endif /* UNOGAME_H */

