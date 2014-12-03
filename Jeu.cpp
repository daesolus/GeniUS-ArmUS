#include "Jeu.h"
#include <iostream>
#include <time.h>
#include <libarmus.h>

Jeu::Jeu(int niveau)
{
	this->niveau = niveau;
	this->pointage = 0;
	this->caseRendu = 0;
	this->questions = questionsDifficile();
	initJeu();
}


int* Jeu::niveauFacile()
{
	char question[] = "Placer les planetes allant de la plus proche du soleil a la plus eloignee";
	planete.question = question;
	//Ordre des plan�tes
	int* ordrePlanete = new int[NOMBRECARTES];
	for(int i = 1; i<= NOMBRECARTES;++i)
		ordrePlanete[i-1] = i;

	return ordrePlanete;
}

int* Jeu::niveauMoyen()
{
	char question[] = "Placer les plan�tes allant de la plus proche du soleil � la plus �loign�e";
	planete.question = question;
	int* ordrePlanete = new int[NOMBRECARTES];
	int i = 0;
	ordrePlanete[i++] = MERCURE;
	ordrePlanete[i++] = MARS;
	ordrePlanete[i++] = VENUS;
	ordrePlanete[i++] = TERRE;
	ordrePlanete[i++] = NEPTUNE;
	ordrePlanete[i++] = URANUS;
	ordrePlanete[i++] = SATURNE;
	ordrePlanete[i++] = JUPITER;
	return ordrePlanete;
}

int* Jeu::niveauDifficile()
{
	int* ordrePlanete = new int[NOMBRECARTES-1];
	int i = 7;
	vector<int> listeNombre;
	for(int j=1;j<=NOMBRECARTES;++j)
	{
		listeNombre.push_back(i);
	}

	ordrePlanete[random(i,listeNombre)] = MERCURE;
	ordrePlanete[random(--i,listeNombre)] = MARS;
	ordrePlanete[random(--i,listeNombre)] = VENUS;
	ordrePlanete[random(--i,listeNombre)] = TERRE;
	ordrePlanete[random(--i,listeNombre)] = NEPTUNE;
	ordrePlanete[random(--i,listeNombre)] = URANUS;
	ordrePlanete[random(--i,listeNombre)] = SATURNE;
	ordrePlanete[random(--i,listeNombre)] = JUPITER;
	return ordrePlanete;
}

map<int,char*> Jeu::questionsDifficile()
{
	map<int,char* > unMap;
	char qMercure[] = "Mon nom est utilis� dans le tableau p�riodique";
	char qVenus[] = "Je suis la plan�te la plus brillante vue de la terre";
	char qTerre[] = "Il y a beaucoup de micro-organisme qui m'habite";
	char qMars[] = "On me surnomme la plan�te rouge, car ma surface contient du fer oxyd�";
	char qJupiter[] = "Je suis entour� de 67 satellites, dont Io et Europe";
	char qSaturne[] = "J'ai des anneaux tr�s prononc�s";
	char qUranus[] = "Je dois mon nom � la divinit� romaine du ciel";
	char qNeptune[]= "Je dois mon nom � la divinit� romaine des oc�ans";
	unMap[MERCURE] = qMercure;
	unMap[VENUS] = qVenus;
	unMap[TERRE] = qTerre;
	unMap[MARS] = qMars;
	unMap[JUPITER] = qJupiter;
	unMap[SATURNE] = qSaturne;
	unMap[URANUS] = qUranus;
	unMap[NEPTUNE] = qNeptune;
	return unMap;
}

void Jeu::initJeu()
{
	switch (this->niveau)
	{
		case 1:planete.ordre = niveauFacile();break;
		case 2:planete.ordre = niveauMoyen();break;
		case 3:planete.ordre = niveauDifficile();
			   planete.question = questions[1];
			   LCD_Printf(planete.question);
			   break;
	}
}

void Jeu::jouer(int carte)
{
	this->pointage += (carte == planete.ordre[caseRendu]) ? 1 : 0;
	this->caseRendu++;
	if(this->niveau==3)
		planete.question = questionsDifficile()[planete.ordre[caseRendu]];
}

char* Jeu::lireQuestion()
{
	return planete.question;
}

int Jeu::lireCaseRendu()
{
	return this->caseRendu;
}


//A faire plus tard
int Jeu::lirePointage()
{
	return this->pointage;
}

int Jeu::random(int max,vector<int> &listeNombre)
{
	const int min = 0;
	srand(time(NULL));
	int random = (rand() % (max - min + 1)) + min;
	int result = listeNombre[random];
	listeNombre.erase(listeNombre.begin() + random);
	return result;
}







