/* 
 * File:   isNumeric.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 26 mai 2018, 16:08
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include <regex.h>


// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************
bool isNumeric(char *target)
{
	int errcode;
	regex_t preg;
	
	if(target==NULL) return false;
	
	const char *str_regex = "[:digit:]";																						// l'expression régulière va vérifier si "target" ne contient que des nombres
	
	errcode = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);									// préparation de l'exécution de l'expression régulière
	
	if(errcode==0)																																	// si tout s'est bien passé...
	{
		return regexec(&preg, target, 0, NULL, 0);																		// ...nous retournons le résultat de l'exécution de l'expression régulière
																																									// SI "target" ne contient que des nombres la fonction retourne "true"
																																									// SINON elle retourne "false"
	} 
	return false;
}
// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


