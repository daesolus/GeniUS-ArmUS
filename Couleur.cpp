//parce que jsuis bs
#define ln     LCD_Printf("\n")

#include "Couleur.h"

float hues[4] = {BLUE_HUE,RED_HUE,GREEN_HUE,YELLOW_HUE};
CapteurCouleur couleur;
int readAndGetColor(){

	RgbColor readColor;
	readColor = couleur.getColorI2C();
	HsbColor colorsHSB = RGBtoHSB(readColor);
	return currentFloorColor(colorsHSB,true);
}

int currentFloorColor(HsbColor readColor, bool isRobot43)
{

	string liaisonCouleur[6] = {"BLANC","BLEU","ROUGE","VERT","JAUNE","ERREUR"};

	//algorithme pour detecter le blanc (�a inclut le gris fonc� et mm le noir, pour �tre encore plus fiable)
	if((readColor.saturation < 0.3 && readColor.brightness > 0.38) || (readColor.saturation < 0.2 && readColor.brightness > 0.2))
		return WHITE_COLOR;

	if((readColor.saturation < 0.2 && readColor.brightness < 0.2) || (readColor.saturation < 0.2 && readColor.brightness < 0.2))
	{
		//cas sp�cial pour le vert

		float modif = 0.f;
				if(!isRobot43){
					modif = 0.17;

				if(checkSameHue(readColor.hue,hues[2]+modif))
				{
					return GREEN_COLOR;
				}else{
					return BLACK_COLOR;
				}}else{
					return BLACK_COLOR;
				}
		//return BLACK_COLOR;
	}

	for(int i = 0; i<4; i++)
	{
		float modif = 0.f;
		//cas sp�cial pour le vert
		if((!isRobot43) && i == 2)
			modif = 0.17;

		if(checkSameHue(readColor.hue,hues[i]+modif))
		{
			return i+1;
		}
	}

	// 5 etant la valeur 'WTF'
	return 5;

}

bool checkSameHue(float hue1, float hue2)
{
	float difference = abs(hue1-hue2);
	if(difference > 0.5)
		difference = 1-difference;

	if(difference < 0.07f)
		return true;
	else
		return false;

}
HsbColor RGBtoHSB(RgbColor colorsIntRGB)
{
	//chaque channel du rgb d'input est un int de 0 � 255 (en theorie)
	//ici on le convertit en float de 0 � 1
	//TODO: ici j'inverse VOLONTAIREMENT le bleu et le vert. � arranger. ou pas. whatever. notre cercle des couleurs est invers� mais ca change rien
    float colorsRGB[3] = {colorsIntRGB.r/255.f, colorsIntRGB.b/255.f, colorsIntRGB.g/255.f};
    HsbColor colorsHSB;
    float r = colorsRGB[0], g = colorsRGB[1], b = colorsRGB[2];
    float K = 0.f;

    if (g < b)
    {
        //std::swap(g, b);
        float tmp = g;
                g = b;
                b = tmp;

        K = -1.f;
    }

    if (r < g)
    {
        //std::swap(r, g);
    	float tmp = r;
    	        r = g;
    	        g = tmp;

        K = -2.f / 6.f - K;
    }
    float chroma = r - min(g, b);
    colorsHSB.hue = fabs(K + (g - b) / (6.f * chroma + 1e-20f));
    colorsHSB.saturation = chroma / (r + 1e-20f);
    colorsHSB.brightness = r;
    return colorsHSB;
}
