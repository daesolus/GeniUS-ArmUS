#include "Robot.h"
#include <libarmus.h>
#include <math.h>

Robot::Robot(int transitions)
{
	AUDIO_SetVolume(90);

	LCD_ClearAndPrint("Debut");
	this->shouldMoveForward = false;
	this->shouldMoveBackwards = false;
	this->bouton = 0;
	this->lastScan = 0;
	currentColor = -1;
	LCD_SetMonitorMode(MONITOR_OFF);
	LCD_ClearAndPrint("Bonjour, et Bienvenue! Je m'appelle R2D3\n\nVeuillez selectionner un niveau de difficulte:\n");
	LCD_Printf("Facile : Vert\nMoyen : Jaune\nDifficile : Rouge\n");

}


void *Robot::scan(void)
{

	nfc.scanTag();
	return 0;
}
void *Robot::scanPointer(void *context)
{
	return ((Robot *)context)->scan();
}

void *Robot::scanCouleur(void)
{
	capteurCouleur.initCapteurI2C();
	return 0;
}

void *Robot::couleurPointer(void *context)
{
	return ((Robot *)context)->scanCouleur();
}

int Robot::lireScan()
{
	return nfc.lireTag();
}

void *Robot::avancerPointer(void *context)
{
	return ((Robot *)context)->avancerAvecLaLigne();
}

void *Robot::avancerAvecLaLigne(void)
{
	Robot::shouldMoveForward = true;

	const int vitesseHigh = 55, vitesseLow = 35;
	const int LG=1, LC=2, LD=4, LGC=3, LDC=6, LGD=5; // code binaire pour identifier les capteurs de lignes Gauche / Centre / Droite
	const float correctionRoues = 30; // transitions / TEMPS
	int lignePositionActuelle = 0, ligneDernierePosition = 0;
	while(Robot::shouldMoveForward)
	{

		// Lecture du suiveur de ligne
		lignePositionActuelle = 0;
		if(ANALOG_Read(1)< 750) lignePositionActuelle += LG; // Gauche
		if(ANALOG_Read(2)< 750)lignePositionActuelle += LC; // Centre
		if(ANALOG_Read(3)< 750)lignePositionActuelle += LD; // Droite

		// Analyse de la lecture du suiveur de ligne

		if(lignePositionActuelle==LC)
		{
			if(ligneDernierePosition==LG || ligneDernierePosition ==LGC)
			{//�tait trop � droite, correction..
				MOTOR_SetSpeed(7,vitesseHigh);
				MOTOR_SetSpeed(8,vitesseLow);
			}
			else if(lignePositionActuelle==LD || lignePositionActuelle ==LDC)
			{//�tait trop � gauche, correction..
				MOTOR_SetSpeed(7,vitesseLow);
				MOTOR_SetSpeed(8,vitesseHigh);
			}
			else
			{//�tait centre, reste centre
				MOTOR_SetSpeed(7,vitesseHigh);
				MOTOR_SetSpeed(8,vitesseHigh);
			}
		}
		else if(lignePositionActuelle==LG || lignePositionActuelle ==LGC) // si LG c'est que robot tend vers la droite
		{//Est trop � droite
			MOTOR_SetSpeed(7,vitesseLow);
			MOTOR_SetSpeed(8,vitesseHigh);
		}
		else if (lignePositionActuelle==LD || lignePositionActuelle ==LDC) // si LD c'est que robot tend vers la gauche
		{//Est trop � gauche
			MOTOR_SetSpeed(7,vitesseHigh);
			MOTOR_SetSpeed(8,vitesseLow);
		}
		else
		{//Si autre chose, juste avancer
			MOTOR_SetSpeed(7,vitesseHigh);
			MOTOR_SetSpeed(8,vitesseHigh);
		}
		ligneDernierePosition = lignePositionActuelle;
	}

		Robot::stopMotors();

		return 0;
}


void *Robot::reculerPointer(void *context)
{
	return ((Robot *)context)->reculerAvecLaLigne();
}

void *Robot::reculerAvecLaLigne(void)
{
	Robot::shouldMoveBackwards = true;

	const int vitesseHigh = -60, vitesseLow = -80;
	const int LG=1, LC=2, LD=4, LGC=3, LDC=6, LGD=5; // code binaire pour identifier les capteurs de lignes Gauche / Centre / Droite
	const float correctionRoues = 30; // transitions / TEMPS
	int lignePositionActuelle = 0, ligneDernierePosition = 0;
		while(Robot::shouldMoveBackwards)
			{

		// Lecture du suiveur de ligne
		lignePositionActuelle = 0;
		if(ANALOG_Read(1)< 750) lignePositionActuelle += LG; // Gauche
		if(ANALOG_Read(2)< 750)lignePositionActuelle += LC; // Centre
		if(ANALOG_Read(3)< 750)lignePositionActuelle += LD; // Droite

		// Analyse de la lecture du suiveur de ligne

		if(lignePositionActuelle==LC)
		{
			if(ligneDernierePosition==LG || ligneDernierePosition ==LGC)
			{//�tait trop � droite, correction..
				MOTOR_SetSpeed(8,vitesseHigh);
				MOTOR_SetSpeed(7,vitesseLow);
			}
			else if(lignePositionActuelle==LD || lignePositionActuelle ==LDC)
			{//�tait trop � gauche, correction..
				MOTOR_SetSpeed(8,vitesseLow);
				MOTOR_SetSpeed(7,vitesseHigh);
			}
			else
			{//�tait centre, reste centre
				MOTOR_SetSpeed(8,vitesseHigh);
				MOTOR_SetSpeed(7,vitesseHigh);
			}
		}
		else if(lignePositionActuelle==LG || lignePositionActuelle ==LGC) // si LG c'est que robot tend vers la droite
		{//Est trop � droite
			MOTOR_SetSpeed(8,vitesseLow);
			MOTOR_SetSpeed(7,vitesseHigh);
		}
		else if (lignePositionActuelle==LD || lignePositionActuelle ==LDC) // si LD c'est que robot tend vers la gauche
		{//Est trop � gauche
			MOTOR_SetSpeed(8,vitesseHigh);
			MOTOR_SetSpeed(7,vitesseLow);
		}
		else
		{//Si autre chose, juste avancer
			MOTOR_SetSpeed(8,vitesseHigh);
			MOTOR_SetSpeed(7,vitesseHigh);
		}
		ligneDernierePosition = lignePositionActuelle;
	}

		Robot::stopMotors();

		return 0;
}


void Robot::stopMotors(){
	MOTOR_SetSpeed(7,0);
	MOTOR_SetSpeed(8,0);
}
void Robot::correctionLigne(float &ligneCorrectionDroite,float &ligneCorrectionGauche,int &ligneDernierePosition,int &lignePositionActuelle)
{
	const int LG=1, LC=2, LD=4, LGC=3, LDC=6, LGD=5; // code binaire pour identifier les capteurs de lignes Gauche / Centre / Droite
	const float correctionRoues = 5; // transitions / TEMPS

	// Lecture du suiveur de ligne
	lignePositionActuelle = 0;
	if(ANALOG_Read(1)<750) lignePositionActuelle += LG; // Gauche
	if(ANALOG_Read(2)< 750)lignePositionActuelle += LC; // Centre
	if(ANALOG_Read(3)< 750)lignePositionActuelle += LD; // Droite

	// Analyse de la lecture du suiveur de ligne

	if(lignePositionActuelle==LC)
	{
		ligneCorrectionDroite = 0;
		ligneCorrectionGauche= 0;
	}
	else if((lignePositionActuelle==LG || lignePositionActuelle ==LGC) && ligneDernierePosition == LC) // si LG c'est que robot tend vers la droite
	{
		ligneCorrectionDroite = correctionRoues;
		ligneCorrectionGauche=0;
	}
	else if ((lignePositionActuelle==LD || lignePositionActuelle ==LDC) && ligneDernierePosition == LC) // si LD c'est que robot tend vers la gauche
	{
		ligneCorrectionDroite = 0;
		ligneCorrectionGauche= correctionRoues+2;
	}
	else
	{
		 if (ligneDernierePosition == LG)
		 {
			 ligneCorrectionDroite = correctionRoues;
		     ligneCorrectionGauche = 0;
		     lignePositionActuelle=LG;
		 }
		 else if(ligneDernierePosition == LD)
		 {
			 ligneCorrectionDroite = 0;
			 ligneCorrectionGauche = correctionRoues;
			 lignePositionActuelle=LD;
		 }
		 else
		 {
			 ligneCorrectionDroite=0;
		 }
		 ligneCorrectionGauche=0;
	}
	ligneDernierePosition = lignePositionActuelle;
}

int Robot::PID(int lecture,int distanceReel,int distanceIdeal,int transitions,float facteurCorrection)
{
	const float FC_VITESSE = 1;//facteur de correction de la vitesse
	const float FC_DISTANCE = 0.5;//facteur de correction de la distance

	return (int)(-FC_VITESSE*(float)(lecture-transitions)+facteurCorrection+30-FC_DISTANCE*(float)(distanceReel-distanceIdeal));
}



//angle 90 degrees
void Robot::tourner(int angle)
{
	float tourCompletCm = 1.604;

	//Pour initialiser les compteurs avant le d�but, �vite quelques bogues intermittants.
	int init;
	init=ENCODER_Read(ENCODER_LEFT);

	int distance = 0;
	while(distance <= (int)((angle - 15)/tourCompletCm))
	{
		distance += ENCODER_Read(ENCODER_LEFT);
		MOTOR_SetSpeed(MOTOR_LEFT,35);
		//LCD_Printf("%i\n",distance);
	}
}

int Robot::getCurrentButton()
{
	return this->bouton;
}


//Attend que lutilisateur appuie sur un bouton
void Robot::setButtonPress()
{
	this->bouton=0;
	while(this->bouton == 0 || this->bouton == 7)
	{
		if(DIGITALIO_Read(14)) //Facile
		{
			bouton = 1;
			//LCD_Printf("\n Facile");
			AUDIO_PlayFile("R2D2-woki.wav");
		}
		else if(DIGITALIO_Read(15)) //Moyen
		{
			bouton = 2;
			//LCD_Printf("\n Moyen");
			AUDIO_PlayFile("R2D2-yeah.wav");
		}
		else if(DIGITALIO_Read(16)) //Difficile
		{
			bouton = 3;
			//LCD_Printf("\n Difficile");
			AUDIO_PlayFile("R2D2-do.wav");
		}
		else if(DIGITALIO_Read(13)) //Instructions
		{
			bouton = 4;
			AUDIO_PlayFile("R2D2-Wii.wav");
		}
	}
}
void Robot::mAJdesLumieres(bool bleu, bool vert, bool orange, bool rouge)
{
	if (bleu == true)
		DIGITALIO_Write(11,1);
	else
		DIGITALIO_Write(11,0);

	if (vert == true)
		DIGITALIO_Write(12,1);
	else
		DIGITALIO_Write(12,0);

	if (orange == true)
		DIGITALIO_Write(13,1);
	else
		DIGITALIO_Write(13,0);

	if (rouge == true)
		DIGITALIO_Write(14,1);
	else
		DIGITALIO_Write(14,0);

}

void Robot::tourner180untilLine()
{
	MOTOR_SetSpeed(7,65);
	MOTOR_SetSpeed(8,-65);
	const int LG=1, LC=2, LD=4, LGC=3, LDC=6, LGD=5; // code binaire pour identifier les capteurs de lignes Gauche / Centre / Droite
	int lignePositionActuelle = 0;
		// Lecture du suiveur de ligne
		lignePositionActuelle = 0;
		if(ANALOG_Read(1)< 750) lignePositionActuelle += LG; // Gauche
		if(ANALOG_Read(2)< 750)lignePositionActuelle += LC; // Centre
		if(ANALOG_Read(3)< 750)lignePositionActuelle += LD; // Droite
		// Analyse de la lecture du suiveur de ligne
		while(lignePositionActuelle!=LC)
		{//Wait till ligne du centre happen
		}
}

void *Robot::colorLoop(void){
	while(1){
	RgbColor readColor;
			readColor = capteurCouleur.getColorI2C();
			LCD_ClearAndPrint("\nR=%i, G=%i, B=%i", readColor.r, readColor.g, readColor.b);

			//fichier << "background-color: rgb(" <<readColor.r<<","<<readColor.g<<","<<readColor.b<<")"<< endl;
			//step 2
	        HsbColor colorsHSB = RGBtoHSB(readColor);
			LCD_Printf("\nH=%.4f, S=%.4f, B=%.4f ", colorsHSB.hue, colorsHSB.saturation, colorsHSB.brightness);

			currentColor = currentFloorColor(colorsHSB,true);

	}
	return 0;
}

