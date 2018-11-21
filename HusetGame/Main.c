#include <stdio.h>
#include <ctype.h>
#include "houseHelper.h"
#include "houseManager.h"
#include <Windows.h>
#include "random.h"
#include <time.h>


//defines
#define TRUE 1


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
// GAME IS CURRENTLY WORKING: VERSION OF GAME 1.0  |
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|

/* -----> Current Working Problem <-----
* Problem when wanting to step back through a door already opened. #fixed
* 
*
*/

enum states {INIT = 1, RUNNING, MENU, EXIT, WIN, SAVE};

void loadScreen(MapT themap, int *key, int *width, int *height, int *flashlight);
void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight);
void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight, int  *W, int *H);
void updateMap(MapT theMap, int *firstEntry);
void howToPlay(int *gameState, MapT theMap, int *keys, int *W, int *H, int *flashlight);
void action(inputT inputVal, int *kermitX, int *kermitY, MapT themap);
void updateKermitPoss(int *kermitX, int *kermitY, int *prevX, int *prevY, MapT themap);
void sightRadius(int *kermitX, int *kermitY, MapT themap, int changer, char siteRange);
int checkActionValid(inputT inputVal, int *kermitX, int *kermitY, MapT themap, int *keys, int *gamestate, int *flashlight);
int checkOpenDoor(int *kermitX, int *kermitY, MapT themap);
int checkWinDoor(int *kermitX, int *kermitY, MapT themap);
void collateralSightCalc(MapT themap, int *height, int *width);
void addFlashlight(MapT themap, int width, int height,positionT *test, int choiceX, int choiceY);
int checkIfMoveFlashlight(int *kermitX, int *kermitY, MapT themap, int *flashlight);
void saveScreen(int *key, int *flashlight, MapT themap, int *width, int *height);
void createFile(int *key, int *flashlight, MapT themap, int *width, int *height);
void loadFile(MapT themap, int *key, int *width, int *height);

struct Kermit {
	int prevX;
	int prevY;
	int posX;
	int posY;
};

int main() {

	system("chcp 1252");
	Randomize();
	enum states gameState;
	gameState = INIT;
	int firstEntry = TRUE;
	typedef struct houseMap MapT;
	struct Kermit kermit;
	int mapShown = 0;
	char siteRange = 'S';
	int W = 0;
	int H = 0;
	int flashlight = 0;
	unsigned int keys = 0;
	printf("Please specify the width and height of game room... ");
	scanf_s("%d, %d", &W, &H);
	if (W < 25) {
		W = 25;
	}
	if (H < 25) {
		H = 25;
	}
	positionT test;
	MapT theMap = createMap(W, H, 20);
	//Adding some flashlights to the map, if picked up you will recieve some increased sight
	addFlashlight(theMap, &W, &H, &test, 0, 0);
	inputT inputVal;

	while (TRUE) {
		if (gameState == INIT) {
			int testVar = 0;
			kermit.posX = W / 2 - 1;
			kermit.posY = H / 2 - 1;
			while (theMap.mArr[kermit.posX][kermit.posY] != ' ') {
				if (testVar == 4) {
					kermit.posX += 1;
				}
				else {
					kermit.posY += 1;
				}
				testVar += 1;
			}
			theMap.mArr[kermit.posX][kermit.posY] = '@';
			//Fixin so that Kermit can see 1 radius from him
			sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
			initFunc(&gameState, theMap, &keys, &flashlight, &W, &H);
		}
		else if (gameState == RUNNING) {
			//Action set
			if (flashlight == 1) {
				siteRange = 'M';
			}
			updateMap(theMap, &firstEntry);
			inputVal = getUserInput();
			sightRadius(&kermit.posX, &kermit.posY, theMap, 0, siteRange);
			collateralSightCalc(theMap, &H, &W);
			kermit.prevX = kermit.posX;
			kermit.prevY = kermit.posY;

			while (checkActionValid(inputVal, &kermit.posX, &kermit.posY, theMap, &keys, &gameState, &flashlight) < 1) {
				system("cls");
				sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
				drawMap(theMap);
				inputVal = getUserInput();
			}
			action(inputVal, &kermit.posX, &kermit.posY, theMap);
			updateKermitPoss(&kermit.posX, &kermit.posY, &kermit.prevX, &kermit.prevY, theMap);
			sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
		}
		else if (gameState == MENU) {
			system("cls");
			//show menu options
		}
		else if (gameState == EXIT) {
			break;
		}
		else if (gameState == WIN) {
			//Add scoreScreen here and how many steps until finish
			printf("You made it to the door!!!! Congratulations!");
			Sleep(4000);
			break;
		}
		else if (gameState == SAVE) {
			printf("Saving current game to file....");
			Sleep(2000);
			saveScreen(&keys, &flashlight, theMap, &W, &H);
			printf("Game is saved...\n");
			gameState = EXIT;
		}
	}
	printf("Game is now closing: Press enter to continue...");
	getchar();
	return 0;
}

int checkActionValid(inputT inputVal, int *kermitX, int *kermitY, MapT themap, int *keys, int *gamestate, int *flashlight) {
	if (inputVal.mObj == 2) {
		if (themap.mArr[*kermitX - 1][*kermitY] == ' ') {
			return 1;
		}
		else if (checkOpenDoor(&*kermitX, &*kermitY, themap) == 1) {
			return 1;
		}
		else if (checkWinDoor(&*kermitX, &*kermitY, themap) == 1) {
			*gamestate = WIN;
			return 1;
		}
		else if (checkIfMoveFlashlight(&*kermitX, &*kermitY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			return 1;
		}
	}
	else if (inputVal.mObj == 3) {
		if (themap.mArr[*kermitX + 1][*kermitY] == ' ') {
			return 1;
		}
		else if (checkOpenDoor(&*kermitX, &*kermitY, themap) == 1) {
			return 1;
		}
		else if (checkWinDoor(&*kermitX, &*kermitY, themap) == 1) {
			*gamestate = WIN;
			return 1;
		}
		else if (checkIfMoveFlashlight(&*kermitX, &*kermitY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			return 1;
		}
	}
	else if (inputVal.mObj == 4) {
		if (themap.mArr[*kermitX][*kermitY - 1] == ' ') {
			return 1;
		}
		else if (checkOpenDoor(&*kermitX, &*kermitY, themap) == 1) {
			return 1;
		}
		else if (checkWinDoor(&*kermitX, &*kermitY, themap) == 1) {
			*gamestate = WIN;
			return 1;
		}
		else if (checkIfMoveFlashlight(&*kermitX, &*kermitY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			return 1;
		}
	}
	else if (inputVal.mObj == 5) {
		if (themap.mArr[*kermitX][*kermitY + 1] == ' ') {
			return 1;
		}
		else if (checkOpenDoor(&*kermitX, &*kermitY, themap) == 1) {
			return 1;
		}
		else if (checkWinDoor(&*kermitX, &*kermitY, themap) == 1) {
			*gamestate = WIN;
			return 1;
		}
		else if (checkIfMoveFlashlight(&*kermitX, &*kermitY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			return 1;
		}
	}
	else if (inputVal.op == 3 && inputVal.mObj == 6) {
		int r = -1;
		int k = 0;
		int temp = 0;
		int index = 0;
		for (int t = 0; t < 4; t++) {
			if (themap.mArr[*kermitX + r][*kermitY + k] == 'D') {
				if (*keys < 1) {
					printf("You were unable to open the door because you had no key");
					Sleep(3000);
					return 0;
				}
				else if (*keys > 0) {
					themap.mArr[*kermitX + r][*kermitY + k] = 'o';
					*kermitX += 2 * r;
					*kermitY += 2 * k;
					*keys -= 1;
					return 1;
				}
			}
			else {
				if (index == 1) {
					k -= (2 * k);
				}
				else {
					temp = r;
					r = k;
					k = temp;
				}
			}
			index++;
		}
		printf("There is no door around your (x, y) axis to be opened");
		Sleep(3000);
	}
	else if (inputVal.op == 2 && inputVal.mObj == 1) {
		int r = -1;
		int k = 0;
		int temp = 0;
		int index = 0;
		for (int t = 0; t < 4; t++) {
			if (themap.mArr[*kermitX + r][*kermitY + k] == 'K') {
				*keys += 1;
				*kermitX += r;
				*kermitY += k;
				printf("En nyckel hittad! Går för att plocka upp den... (Din totala mängd nycklar är %d)", *keys);
				Sleep(3000);
				return 1;
			}
			else {
				if (index == 1) {
					k -= (2 * k);
				}
				else {
					temp = r;
					r = k;
					k = temp;
				}
			}
			index++;
		}
		printf("There is no key around your x,y axis");
		Sleep(3000);
	}
	else if (inputVal.op == 0) {
		*gamestate = SAVE;
		return 1;
	}
	return 0;
}

void addFlashlight(MapT themap, int width, int height, positionT *test, int choiceX, int choiceY) {
	//posX and posY is for specifical cases
	int posX = choiceX;
	int posY = choiceY;
	char inputval = 'F';
	if (posX == 0 || posX == NULL || posY == 0 || posY == NULL) {
		for (int i = 0; i < 5; i++) {
			int succes = placeObject(themap, 0, 0, inputval, &*test, 0);
		}
	}
	else {
		int succes = placeObject(themap, 0, 0, inputval, &*test, 0);
	}
}

void loadScreen(MapT themap, int *key, int *width, int *height, int *flashlight) {
	//Add the functions about files	
	loadFile(themap, &*key, &*width, &*height, &*flashlight);
}

void saveScreen(int *key, int *flashlight, MapT themap, int *width, int *height) {
	createFile(&*key, &*flashlight, themap,&*width, &*height);
}

void createFile(int *key, int *flashlight, MapT themap, int *width, int *height) {
	char* filename = "MapInfoSAVED.txt";
	char* keyinfoName = "KeyInfoSAVED.txt";
	FILE *mapInfo;
	FILE *keyInfo;
	fopen_s(&mapInfo, filename, "w+");
	fopen_s(&keyInfo, keyinfoName, "w+");
	for (int a = 0; a < *width; a++) {
		for (int b = 0; b < *height; b++) {
			fprintf(mapInfo, "%c\n", themap.mArr[a][b]);
		}
	}
	fprintf(keyInfo, "%d\n", *key);
	fclose(mapInfo);
	fclose(keyInfo);
}
void loadFile(MapT themap, int *key, int *width, int *height) {
	FILE* mapFile;
	FILE* keyFile;
	char temp;
	fopen_s(&mapFile, "MapInfoSAVED.txt", "r");
	fopen_s(&keyFile, "KeyInfoSAVED.txt", "r");
	if (mapFile == NULL) {
		printf("Unable to open the file...");
		return;
	}
	if (keyFile == NULL) {
		printf("Unable to open the file...");
		return;
	}
	fscanf_s(keyFile, "%d", &*key);
	for (int a = 0; a < *width; a++) {
		for (int b = 0; b < *height; b++) {
			/*themap.mArr[a][b] = ReadLine(mapFile);*/
			fscanf_s(mapFile, "%c", &temp);
			themap.mArr[a][b] = temp;
		}
	}
	fclose(mapFile);
	fclose(keyFile);
}

int checkOpenDoor(int *kermitX, int *kermitY, MapT themap) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[*kermitX + r][*kermitY + k] == 'o') {
			*kermitX += r;
			*kermitY += k;
			return 1;
		}
		else {
			if (index == 1) {
				k -= (2 * k);
			}
			else {
				temp = r;
				r = k;
				k = temp;
			}
		}
		index++;
	}
}

int checkIfMoveFlashlight(int *kermitX, int *kermitY, MapT themap, int *flashlight) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[*kermitX + r][*kermitY + k] == 'F') {
			*flashlight = 1;
			return 1;
		}
		else {
			if (index == 1) {
				k -= (2 * k);
			}
			else {
				temp = r;
				r = k;
				k = temp;
			}
		}
		index++;
	}
	return 0;
}

int checkWinDoor(int *kermitX, int *kermitY, MapT themap) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[*kermitX + r][*kermitY + k] == 'M') {
			*kermitX += r;
			*kermitY += k;
			return 1;
		}
		else {
			if (index == 1) {
				k -= (2 * k);
			}
			else {
				temp = r;
				r = k;
				k = temp;
			}
		}
		index++;
	}
}

void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight){
	//Shows a screen in which the creator information is being displayed
	system("cls");
	printf("Game: Huset\n");
	printf("Version: 0.8\n");
	printf("-------------\n");
	printf("Creator: Daniel Ekeroth");
	getchar();
	printf("\n\nPress enter to go back to the menu");
	getchar();
	initFunc(&*gamestate,  theMap, &*keys, &*W, &*H, &*flashlight);
}

void sightRadius(int *kermitX, int *kermitY, MapT themap, int changer, char site) {
	int val = changer;
	int r1 = 0;
	int k1 = 0;
	int E1 = 0;
	int E2 = 0;
	if (site == 'S') {
		r1 = -1;
		k1 = -1;
		E1 = 2;
		E2 = 2;
	}
	else if (site == 'M') {
		r1 = -2;
		k1 = -2;
		E1 = 3;
		E2 = 3;
	}
	for (int r = r1; r < E1; r++) {
		for (int k = k1; k < E2; k++) {
			if (themap.mArr[*kermitX + r][*kermitY + k] == 'e') {
				themap.vArr[*kermitX + r][*kermitY + k] = 1;
			}
			else {
				themap.vArr[*kermitX + r][*kermitY + k] = val;
			}
		}
	}
}

void collateralSightCalc(MapT themap, int *height, int *width) {
	//Goes through the mapArray to see if there is any bugs where items are visable when they shouldn not be

	for (int h = 0; h < *height; h++) {
		for (int w = 0; w < *width; w++) {
			if (themap.mArr[h][w] == 'e') {
				themap.vArr[h][w] = 1;
			}
			else if (themap.mArr[h][w] == 'M') {
				themap.vArr[h][w] = 1;
			}
			else if (themap.vArr[h][w] == 1) {
				themap.vArr[h][w] = 0;
			}
		}
	}
}

void action(inputT inputVal, int *kermitX, int *kermitY, MapT themap) {
	if (inputVal.mObj == 2) {
		*kermitX -= 1;
	}
	else if (inputVal.mObj == 3) {
		*kermitX += 1;
	}
	else if (inputVal.mObj == 4) {
		*kermitY -= 1;
	}
	else if (inputVal.mObj == 5) {
		*kermitY += 1;
	}
}

void updateKermitPoss(int *kermitX, int *kermitY,int *prevX, int *prevY, MapT themap) {
	themap.mArr[*prevX][*prevY] = ' ';
	themap.mArr[*kermitX][*kermitY] = '@';
	themap.vArr[*kermitX][*kermitY] = 1;
}

void howToPlay(int *gameState, MapT theMap, int *keys, int *W, int *H, int *flashlight) {
	//Gives the user some information on how the game works
	system("cls");
	int choice = 0;
	printf("If you see a 'e' it means --> OutsideWall\n");
	printf("If you see a 'M' it means --> OutsideDoor\n");
	printf("If you see a 'K' it means --> Key\n");
	printf("If you see a 'D' it means --> Insidedoor\n");
	printf("If you see a 'w' it means --> insideWall\n");
	printf("If you see a '@' it means --> Player Character\n");
	printf("=================================================\nEnter 1 to go back to the meny or 2 to exit: --> ");
	scanf_s("%d", &choice);
	if (choice == 1) {
		initFunc(&gameState, theMap, &*keys, &*W, &*H, &*flashlight);
	}
	else if (choice == 2) {
		//endfunction
		*gameState = EXIT;
	}
	while (choice < 1 || choice > 2) {
		printf("Your entered character was not a 1 or 2, try again\n");
		scanf_s("%d", &choice);
	}
}

void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight,int  *W, int *H) {
	int menuChoice = 0;
	//fixing menu
	system("cls");
	printf("--------------------------------------------------------------\n");
	printf("|\t\t\tWelcome to Huset!                    |\n");
	printf("|                                                            |\n");
	printf("|1:	New Game                                             |\n");
	printf("|2:	Load game                                            |\n");
	printf("|3:	Creator Info                                         |\n");
	printf("|4:	How To Play                                          |\n");
	printf("|5:	Exit                                                 |\n");
	printf("--------------------------------------------------------------\n--> ");
	scanf_s("%d", &menuChoice);
	switch (menuChoice) {
	case 1:
		*gameState = RUNNING;
		printf("Launching new game....");
		break;
	case 2:
		printf("File is being loaded....\n");
		loadScreen(theMap, &*keys, &*W, &*H, &*flashlight);
		Sleep(2000);
		*gameState = RUNNING;
		break;
	case 3:
		creatorScreen(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight);
		break;
	case 4:
		howToPlay(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight);
		break;
	case 5:
		*gameState = EXIT;
		break;
	}
}

void updateMap(MapT theMap, int *firstEntry) {
	system("cls");
	drawMap(theMap);
	if (*firstEntry == TRUE) {
		getchar();
		*firstEntry = 2;
	}
}