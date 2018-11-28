#include <stdio.h>
#include <ctype.h>
#include "houseHelper.h"
#include "houseManager.h"
#include <Windows.h>
#include "random.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

//defines
#define TRUE 1


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//	 GAME IS CURRENTLY WORKING: VERSION OF GAME 1.0		|
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|

/* -----> Current Working Problem <-----
* Problem when wanting to step back through a door already opened. #fixed
* Problem while loading file, cant get right characters (#fixed)// Sideproblem, Kermit seems to get weird positions after action.
* Problem when a key spawn behind a door and then passing the door will make the key go away
*/

/* -----> Current Working Ideas <-----
* Implimenting an enemy.
* Impliment a type of store where you can buy different objects; Need coins and things that give coins. Different objects to buy, example: Hint where flashlights are; Hint on where to go;
* If enemy is implemented weapons might be good.
* Implement difficulty levels.
*/

/* -----> Enemy | Battlestage <-----
* A gamestate in which the person fights a enemy on the screen;
* Screen---
* Should show available weapons, flasks etc... 
* Show HP, Attacks and how to do.
* Enemy---
* If the enemy is in sight for Kermit, enter battlestage;
* 
*/

enum states {INIT = 1, RUNNING, MENU, EXIT, WIN, SAVE, FIGHT};

typedef struct Enemy {
	int HP;
	int x;
	int y;
	//Weapons go from S - A - B - C - D - E
	char weapon;
	int stance; //In attack: 1 if not 0;
} Enemy;

void spawnEnemy(Enemy *enemyarr, MapT themap, inputT *obj, int *W, int *H);
void loadScreen(MapT themap, int *key, int *width, int *height, int *flashlight, int *kermitX, int *kermitY);
void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY);
void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight, int  *W, int *H, int *kermitX, int *kermitY);
void updateMap(MapT theMap, int *firstEntry);
void howToPlay(int *gameState, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY);
void action(inputT inputVal, int *kermitX, int *kermitY, MapT themap);
void updateKermitPoss(int *kermitX, int *kermitY, int *prevX, int *prevY, MapT themap);
void sightRadius(int *kermitX, int *kermitY, MapT themap, int changer, char siteRange);
int checkActionValid(inputT inputVal, int *kermitX, int *kermitY, MapT themap, int *keys, int *gamestate, int *flashlight);
int checkOpenDoor(int *kermitX, int *kermitY, MapT themap);
int checkWinDoor(int *kermitX, int *kermitY, MapT themap);
void collateralSightCalc(MapT themap, int *height, int *width, int *kermitX, int *kermitY);
void addFlashlight(MapT themap, int width, int height,positionT *test, int choiceX, int choiceY);
int checkIfMoveFlashlight(int *kermitX, int *kermitY, MapT themap, int *flashlight);
void saveScreen(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY);
void createFile(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY);
void checkFight(MapT themap,char site, int *gamestate, Enemy *enemyarr);
//void enemyMove(Enemy *enemyarr, MapT themap, int *kX, int *kY);
void loadFile(MapT themap, int *key, int *width, int *height, int *kermitX, int *kermitY);
//void append(char *s, char c);

struct Kermit {
	int prevX;
	int prevY;
	int posX;
	int posY;
	int HP;
	char weapons[100];
	char currentWeapon;
};

int main() {

	system("chcp 1252");
	Randomize();
	enum states gameState;
	gameState = INIT;
	int firstEntry = TRUE;
	typedef struct houseMap MapT;
	struct Kermit kermit;
	kermit.HP = 150;
	//Giving a start Sword for Kermit
	int mapShown = 0;
	char siteRange = 'S';
	unsigned int W = 0;
	unsigned int H = 0;
	unsigned int moves = 0;
	unsigned int moveF = 0;
	int flashlight = 0;
	unsigned int keys = 0;
		
	//Enemy things...:>
	typedef struct Enemy Enemy;
	Enemy enemyArr[5];
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
	spawnEnemy(&enemyArr, theMap, &test, &W, &H);

	//Adding some flashlights to the map, if picked up you will recieve some increased sight
	addFlashlight(theMap, &W, &H, &test, 0, 0);
	inputT inputVal;

	while (TRUE) {
		if (gameState == INIT) {
			int testVar = 0;
			kermit.posX = W / 2 - 1;
			kermit.posY = H / 2 - 1;
			//adding default weapon
			char t = 'A';
			kermit.weapons[0] = t;
			kermit.weapons[1] = '\0';
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
			initFunc(&gameState, theMap, &keys, &flashlight, &W, &H, &kermit.posX, &kermit.posY);
		}
		else if (gameState == RUNNING) {
			//Action set
			if (flashlight == 0) {
				siteRange = 'S';
			}
			else {
				siteRange = 'M';
			}
			updateMap(theMap, &firstEntry);
			inputVal = getUserInput();
			sightRadius(&kermit.posX, &kermit.posY, theMap, 0, siteRange);
			collateralSightCalc(theMap, &H, &W, &kermit.posX, &kermit.posY);
			kermit.prevX = kermit.posX;
			kermit.prevY = kermit.posY;

			while (checkActionValid(inputVal, &kermit.posX, &kermit.posY, theMap, &keys, &gameState, &flashlight) < 1) {
				system("cls");
				sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
				drawMap(theMap);
				inputVal = getUserInput();
			}
			if (flashlight == 1) {
				moveF++;
				if (moveF >= 14) {
					moveF = 0;
					flashlight = 0;
				}
			}
			if (moves % 10 == 0 && moves != 0) {
				printf("Number of currently moves: | > %d < |", moves);
				Sleep(1500);
			}
			moves++;
			action(inputVal, &kermit.posX, &kermit.posY, theMap);
			//enemyMove(&enemyArr, theMap, &kermit.posX, &kermit.posY);
			updateKermitPoss(&kermit.posX, &kermit.posY, &kermit.prevX, &kermit.prevY, theMap);
			sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
			checkFight(theMap, siteRange, &gameState, &enemyArr);
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
			saveScreen(&keys, &flashlight, theMap, &W, &H, &kermit.posX, &kermit.posX);
			printf("Game is saved...\n");
			gameState = EXIT;
		}
		else if (gameState == FIGHT) {
			int damage = 0;
			int enemyDmg = 0;
			int randomNum = 0;
			printf("Watch out, an enemy is attacking you!");
			//debugMessage
			printf("Entering battlestance...");
			Sleep(2000);
			system("cls");
			int attackerIndex = 0;
			for (int a = 0; a < 5; a++) {
				if (enemyArr[a].stance == 1) {
					attackerIndex = a;
					printf("The attacker is: %d", a);
					Sleep(1500);
				}
			}
			//Init the screen;
			//battleScreen();
			printf("Fight progressing....\n");
			Sleep(1000);
			//int invSize = sizeof(kermit.weapons) / sizeof(kermit.weapons[0]);
			int invSize = strlen(kermit.weapons);
			for (int i = 0; i < invSize; i++) {
				printf("Please choose from your inventory ---> \n");
				printf("%d: Weapon Class %c\n", i, kermit.weapons[i]);
			}
			int choice = 0;
			printf("Please enter the proper index for your weapon choice: ");
			scanf_s("%d", &choice);
			while (choice > invSize) {
				printf("\nNot a valid weaponIndex, Try again.\n Value: ");
				scanf_s("%d", &choice);
			}
			kermit.currentWeapon = kermit.weapons[choice];
			printf("Your current weaponclass is %c\n", kermit.currentWeapon);
			//gameloop
			while (!kermit.HP < 1 || !enemyArr[attackerIndex].HP < 1) {
				system("cls");
				printf("Your HP: %d \t\t\t\t Enemy HP: %d\n", kermit.HP, enemyArr[attackerIndex].HP);
				printf("Current WeaponClass: %c \t\t\t Current enemy weaponclass: %c\n\n", kermit.currentWeapon, enemyArr[attackerIndex].weapon);

				if (kermit.currentWeapon = 'S') {
					damage = 35;
				}
				else if (kermit.currentWeapon = 'A') {
					damage = 30;
				}
				else if (kermit.currentWeapon = 'B') {
					damage = 25;
				}
				else if (kermit.currentWeapon = 'C') {
					damage = 20;
				}
				else if (kermit.currentWeapon = 'D') {
					damage = 15;
				}
				else if (kermit.currentWeapon = 'E') {
					damage = 10;
				}

				if (enemyArr[attackerIndex].weapon = 'S') {
					enemyDmg = 35;
				}
				else if (enemyArr[attackerIndex].weapon = 'A') {
					enemyDmg = 30;
				}
				else if (enemyArr[attackerIndex].weapon = 'B') {
					enemyDmg = 25;
				}
				else if (enemyArr[attackerIndex].weapon = 'C') {
					enemyDmg = 20;
				}
				else if (enemyArr[attackerIndex].weapon = 'D') {
					enemyDmg = 15;
				}
				else if (enemyArr[attackerIndex].weapon = 'E') {
					enemyDmg = 10;
				}
				/*playerAttack(&damage);
				enemyAttack();*/
				printf("Please choose attackmove: \n");
				int attackChoice = 0;

				printf("1: Quick Attack\n2: Heavy Attack");
				scanf_s("%d", &attackChoice);
				while (attackChoice < 1 || attackChoice > 2) {
					printf("Not a valid attackmove, please try again: ");
					scanf_s("%d", &attackChoice);
				}
				switch (attackChoice) {
				case 1:
					enemyArr[attackerIndex].HP -= damage;
					printf("Nice hit! You did %d damage", damage);
					Sleep(1500);
					break;
				case 2:
					//init 50% chance of hit
					randomNum = RandomInteger(1, 3);
					if (randomNum == 1) {
						enemyArr[attackerIndex].HP -= damage * 2;
						printf("Great hit! You did %d damage", damage * 2);
						Sleep(1500);
						break;
					}
					else {
						printf("The heavy hit missed...");
						Sleep(1500);
						break;
					}
				}
				printf("Enemy is attacking with quick attack...");
				kermit.HP -= enemyDmg;
				printf("You lost %d heath. Current heath: %d", enemyDmg, kermit.HP);
			}
			if (kermit.HP < 1) {
				gameState = EXIT;
			}
			else if (enemyArr[attackerIndex].HP < 1) {
				//delete the enemy
				gameState = RUNNING;
			}
		}
	}
	printf("Game is now closing: Press enter to continue...");
	getchar();
	return 0;
}

//void playerAttack(int damage, ) {
//	printf("Please choose attackmove:");
//	int attackChoice = 0;
//	
//	printf("1: Quick Attack\n2: Heavy Attack");
//	scanf_s("%d", &attackChoice);
//	switch (attackChoice) {
//	case 1:
//
//	}
//}
//append function

//void append(char *s, char c) {
//	int len = strlen(s);
//	s[len] = c;
//	s[len + 1] = '\0';
//}	

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

void loadScreen(MapT themap, int *key, int *width, int *height, int *flashlight, int *kermitX, int *kermitY) {
	//Add the functions about files	
	loadFile(themap, &*key, &*width, &*height, &*flashlight, &*kermitX, &*kermitY);
}

void saveScreen(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY) {
	createFile(&*key, &*flashlight, themap,&*width, &*height, &*kermitX, &*kermitY);
}

void createFile(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY) {
	char* filename = "MapInfoSAVED.txt";
	char* keyinfoName = "KeyInfoSAVED.txt";
	FILE *mapInfo;
	FILE *keyInfo;
	fopen_s(&mapInfo, filename, "w+");
	fopen_s(&keyInfo, keyinfoName, "w+");
	for (int a = 0; a < *width; a++) {
		for (int b = 0; b < *height; b++) {
			fprintf(mapInfo, "%c", themap.mArr[a][b]);
			/*fputc(themap.mArr[a][b], mapInfo);*/
			if (themap.mArr[a][b] == '@') {
				printf("We found kermit!! pos: %d, %d", a, b);
				Sleep(1500);

			}
			fprintf_s(mapInfo, "\n");
		}
	}
	fprintf(keyInfo, "%d\n", *key);
	fclose(mapInfo);
	fclose(keyInfo);
}

void loadFile(MapT themap, int *key, int *width, int *height, int *kermitX, int *kermitY) {
	FILE* mapFile;
	FILE* keyFile;
	/*themap = createMap(*width, *height, 20);*/
	char* temp;
	unsigned char t;
	temp = malloc(12 * sizeof(char));
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
	fclose(keyFile);
	for (int a = 0; a < *width; a++) {
		for (int b = 0; b < *height; b++) {
			if (feof(mapFile)) {
				break;
			}
			fgets(temp, 12, mapFile);
			t = temp[0];
			themap.mArr[a][b] = t;	
			if (t == '@') {
				*kermitX = a;
				*kermitY = b;
			}
			/*printf("The current char doing is %c", themap.mArr[a][b]);
			if (themap.mArr[a][b] == "@") {
				*kermitX = a;
				*kermitY = b;
				Sleep(3000);
			}*/
			/*themap.mArr[a][b] = ReadLine(mapFile);*/
			/*fscanf_s(mapFile, "%c\n", &temp);
			themap.mArr[a][b] = temp;*/
		}
	}
	themap.mArr[*kermitX][*kermitY] = '@';
	sightRadius(&*kermitX, &*kermitY, themap, 1, 'S');
	fclose(mapFile);
}

void spawnEnemy(Enemy *enemyarr, MapT themap, inputT *obj, int *W, int *H) {
	for (int a = 0; a < 5; a++) {
		enemyarr[a].HP = 150;
		enemyarr[a].weapon = 'A';
		enemyarr[a].x = RandomInteger(1, *W);
		enemyarr[a].y = RandomInteger(1, *H);
		enemyarr[a].stance = 0;
		int succes = placeObject(themap, enemyarr[a].x, enemyarr[a].y, '§', &*obj, 1);
		//printf("Enemy%d is at (%d, %d)", a, enemyarr[a].x, enemyarr[a].y);
	}
}

//void enemyMove(Enemy *enemyarr, MapT themap, int *kX, int *kY) {
//	//Makes all enemies move towards Kermit
//	for (int a = 0; a < 5; a++) {
//		int x = enemyarr[a].x;
//		int y = enemyarr[a].y;
//		//ACCES VAOLATIOM?
//		themap.mArr[x][y] = ' ';
//		if (x > *kX+1) {
//			if (themap.mArr[x-1][y] == ' ') {
//				x--;
//			}
//		}
//		else if (x < *kX-1) {
//			if (themap.mArr[x + 1][y] == ' ') {
//				x++;
//			}
//		}
//		else {
//			if (y > *kY) {
//				if (themap.mArr[x][y - 1] == ' ') {
//					y--;
//				}
//			}
//			else if (y < *kY) {
//				if (themap.mArr[x][y + 1] == ' ') {
//					y++;
//				}
//			}
//		}
//		themap.mArr[x][y] = '§';
//		themap.vArr[x][y] = 1;
//		enemyarr[a].y = y;
//		enemyarr[a].x = x;
//	}
//}

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

void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY){
	//Shows a screen in which the creator information is being displayed
	system("cls");
	printf("Game: Huset\n");
	printf("Version: 0.8\n");
	printf("-------------\n");
	printf("Creator: Daniel Ekeroth");
	getchar();
	printf("\n\nPress enter to go back to the menu");
	getchar();
	initFunc(&*gamestate,  theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY);
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

void checkFight(MapT themap,char site, int *gamestate, Enemy *enemyarr) {
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
	for (int a = 0; a < 5; a++) {
		for (int r = -1; r < 2; r++) {
			for (int k = -1; k < E2; k++) {
				int x = enemyarr[a].x;
				int y = enemyarr[a].y;
				if (themap.mArr[ x + r][y + k] == '@') {
					*gamestate = FIGHT;
					enemyarr[a].stance = 1;
				}
			}
		}
	}
}

void collateralSightCalc(MapT themap, int *height, int *width, int *kermitX, int *kermitY) {
	//Goes through the mapArray to see if there is any bugs where items are visable when they shouldn not be
	for (int h = 0; h < *height; h++) {
		for (int w = 0; w < *width; w++) {
			if (themap.mArr[h][w] == 'e') {
				themap.vArr[h][w] = 1;
			}
			else if (themap.mArr[h][w] == 'M') {
				themap.vArr[h][w] = 1;
			}
			else if (themap.mArr[h][w] == '§') {
				themap.vArr[h][w] = 1;
			}
			else if (themap.vArr[h][w] == 1) {
				themap.vArr[h][w] = 0;
			}
			else if (themap.mArr[h][w] == '@') {
				if (h != *kermitX || w != *kermitY) {
					themap.mArr[h][w] = ' ';
				}
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

void howToPlay(int *gameState, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY) {
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
		initFunc(&gameState, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY);
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

void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight,int  *W, int *H, int *kermitX, int *kermitY) {
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
		loadScreen(theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY);
		Sleep(2000);
		*gameState = RUNNING;
		break;
	case 3:
		creatorScreen(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY);
		break;
	case 4:
		howToPlay(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY);
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