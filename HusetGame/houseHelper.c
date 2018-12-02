/*
* File: houseHelper.c
* Version: 1.0
* Last modified on Tue Nov 16 2018 by CaAn
* -----------------------------------------------------
* This interface provides several functions for the house game
* These functions are added by the student.
*/

#include "houseManager.h"
#include "houseHelper.h"
#include <string.h>
#include <ctype.h>

#define TRUE 1

int  placeObject(MapT tMap, int row, int col, char chObj, positionT *pos, int visibility)
{
	if (row < 0 || row > tMap.heigth - 1 || col < 0 || col > tMap.width - 1) //Outside housemap
		return 0;
	if (row == 0 && col == 0) { //Select random values for row and col
		for (int i = 0; i < 100 && tMap.mArr[row][col] != ' ' ; i++) {
			row = rand() % (tMap.heigth - 2) + 1;
			col = rand() % (tMap.width - 2) + 1;
		} 
	}
	if (tMap.mArr[row][col] != ' ')
		return 0;
	tMap.mArr[row][col] = chObj;
	tMap.vArr[row][col] = visibility;
	pos->row = row;
	pos->col = col;
	return 1;
}

inputT getUserInput(void)
{
	unsigned char * input = getString("\nVad vill du göra: "); // getString() from the myIOLib library or use similar function in Roberts library 
	int countA = 0, countB = 0;
	unsigned char * operationStr = NULL;
	unsigned char * objectStr = NULL;
	while (input[countA] != ' ' && input[countA] != '\0')
		countA++;
	if (countA > 0) { //Read first part of string as the operation
		operationStr = (unsigned char *)malloc((countA + 1) * sizeof(unsigned char));
		for (int i = 0; i < countA; i++)
			operationStr[i] = input[i];
		operationStr[countA] = '\0';
		if (input[countA] != '\0') { //Read second part of string as the object to operate on
			countB = countA + 1;
			while (input[countB] != '\0')
				countB++;
			objectStr = (unsigned char*)malloc((countB - countA) * sizeof(unsigned char));
			int j = 0;
			for (int i = (countA + 1); i < countB; i++, j++)
				objectStr[j] = input[i];
			objectStr[j] = '\0';
		}
	}
	//Start appropriate action
	//Find index for the operation and for the object
	enumOperation operationArr = OPERATIONTEXTS;
	enumMapObject objectArr = OBJECTTEXTS;
	inputT inpObj;
	inpObj.op = -1;
	if (operationStr != NULL)
		for (inpObj.op = endgame; inpObj.op < NUMBEROFOPERATIONS; inpObj.op++) {
			if (!strcoll(operationStr, operationArr[inpObj.op]))
				break;
		}
	inpObj.mObj = -1;
	if (objectStr != NULL)
		for (inpObj.mObj = key; inpObj.mObj < NUMBEROFOBJECTS; inpObj.mObj++) {
			if (!strcmp(objectStr, objectArr[inpObj.mObj]))
				break;
		}
	//Release memory allocated on the heap that is only used locally when the function is run
	free(input); //Memory allocated by the getString() function
	free(operationStr);
	free(objectStr);
	return inpObj;
}

void addCoins(MapT themap, positionT *test) {
	char inputval = 'C';
	for (int i = 0; i < 10; i++) {
		int succes = placeObject(themap, 0, 0, inputval, &*test, 1);
	}
}

int checkActionValid(inputT inputVal, int *kermitX, int *kermitY, MapT themap, int *keys, int *gamestate, int *flashlight, int *coins) {
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
		else if (checkIfMoveCoins(&*kermitX, &*kermitY, themap, &*coins) == 1) {
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
		else if (checkIfMoveCoins(&*kermitX, &*kermitY, themap, &*coins) == 1) {
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
		else if (checkIfMoveCoins(&*kermitX, &*kermitY, themap, &*coins) == 1) {
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
		else if (checkIfMoveCoins(&*kermitX, &*kermitY, themap, &*coins) == 1) {
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

void shopScreen(MapT themap, int *kermitX, int *kermitY, struct Shop Shop, struct Kermit *kermit, int *coins) {
	//make a structure to store all the item for the shop to sell then loop throught i in here
	printf("Entering the shop screen....");
	printf("\t\t\t\t\t\t Your coins: %d\n", *coins);
	int index = 0;
	char *temp[100];
	char singlechar;
	unsigned char valuetesting;
	for (int a = 0; a < 4; a++) {
		printf("%d: %s\n", index + 1, Shop.items[a]);
		index++;
	}
	int choice = 0;
	printf("Please specify the type you want to buy with the right index!\nAnswer: ");
	scanf_s("%d", &choice);
	while (choice < 0 || choice > 4) {
		printf("Not a valid index... Try again. --> ");
		scanf_s("%d", &choice);
	}
	switch (choice) {
	case 1:
		if (*coins < 75) {
			printf("Not enough coins, leaving...");
			Sleep(2500);
			break;
		}
		temp[0] = "Healing Potion";
		for (int a = 0; a < 100; a++) {
			valuetesting = kermit->flasks[a][0];
			if (!isalnum(valuetesting)) {
				strcpy_s(kermit->flasks[a], sizeof(kermit->flasks[a]), temp[0]);
				break;
			}
		}
		*coins -= 75;
		// check if money matches the curretn weapon cost
		// minus the cost 
		printf("There you go, a brand new Healing potion");
		Sleep(2500);
		break;
	case 2:
		if (*coins < 150) {
			printf("Not enough coins, leaving...");
			Sleep(2500);
			break;
		}
		*coins -= 150;
		singlechar = 'B';
		for (int a = 0; a < 100; a++) {
			if (!isalnum(kermit->weapons[a])) {
				kermit->weapons[a] = singlechar;
				kermit->weapons[a + 1] = '\0';
				printf("Added the sword...");
				Sleep(2500);
				break;
			}
		}
		break;
	case 3:
		if (*coins < 300) {
			printf("Not enough coins, leaving...");
			Sleep(2500);
			break;
		}
		*coins -= 300;
		singlechar = 'S';
		for (int a = 0; a < 100; a++) {
			if (!isalpha(kermit->weapons[a])) {
				kermit->weapons[a] = singlechar;
				kermit->weapons[a + 1] = '\0';
				printf("You got a new sword with class S!");
				Sleep(2500);
				break;
			}
		}
		break;
	case 4:
		if (*coins < 90) {
			printf("Not enough coins, leaving...");
			Sleep(2500);
			break;
		}
		temp[0] = "Damage Flask";
		for (int a = 0; a < (sizeof(kermit->flasks) / sizeof(kermit->flasks[0])); a++) {
			valuetesting = kermit->flasks[a][0];
			if (!isalpha(valuetesting)) {
				strcpy_s(kermit->flasks[a], sizeof(kermit->flasks[a]), temp[0]);
				break;
			}
		}
		*coins -= 75;
		// check if money matches the curretn weapon cost
		// minus the cost 
		printf("There you go, a brand new Damage potion");
		Sleep(2500);
		break;
	case 5:
		break;
	}
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
	createFile(&*key, &*flashlight, themap, &*width, &*height, &*kermitX, &*kermitY);
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
	// init all values for enemy object
	for (int a = 0; a < 5; a++) {
		enemyarr[a].HP = 150;
		enemyarr[a].weapon = 'E';
		enemyarr[a].x = RandomInteger(4, *W - 5);
		enemyarr[a].y = RandomInteger(4, *H - 5);
		enemyarr[a].dead = 0;
		int loopvalue = 0;
		while (themap.mArr[enemyarr[a].x][enemyarr[a].y] != ' ') {
			if (loopvalue == 4) {
				enemyarr[a].x++;
			}
			else {
				enemyarr[a].y++;
			}
			loopvalue++;
		}
		enemyarr[a].stance = 0;
		int succes = placeObject(themap, enemyarr[a].x, enemyarr[a].y, '§', &*obj, 1);
	}
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

int checkIfMoveCoins(int *kermitX, int *kermitY, MapT themap, int *coins) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[*kermitX + r][*kermitY + k] == 'C') {
			*coins += 50;
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

void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY) {
	//Shows a screen in which the creator information is being displayed
	system("cls");
	printf("Game: Huset\n");
	printf("Version: 0.8\n");
	printf("-------------\n");
	printf("Creator: Daniel Ekeroth");
	getchar();
	printf("\n\nPress enter to go back to the menu");
	getchar();
	initFunc(&*gamestate, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY);
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

void checkFight(MapT themap, char site, int *gamestate, Enemy *enemyarr) {
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
		if (enemyarr[a].dead == 1) {
			continue;
		}
		for (int r = -1; r < 2; r++) {
			for (int k = -1; k < E2; k++) {
				int x = enemyarr[a].x;
				int y = enemyarr[a].y;
				if (themap.mArr[x + r][y + k] == '@') {
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
			else if (themap.mArr[h][w] == 'B') {
				themap.vArr[h][w] = 1;
			}
			else if (themap.mArr[h][w] == 'C') {
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

void updateKermitPoss(int *kermitX, int *kermitY, int *prevX, int *prevY, MapT themap) {
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

void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight, int  *W, int *H, int *kermitX, int *kermitY) {
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

void addShop(MapT themap, positionT *test) {
	char inputval = 'B';
	for (int a = 0; a < 3; a++) {
		int succes = placeObject(themap, 0, 0, inputval, &*test, 1);
	}
}

void shopNearby(int *kermitX, int *kermitY, MapT themap, struct Shop Shop, struct Kermit *kermit, int *coins) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[*kermitX + r][*kermitY + k] == 'B') {
			int answer = 0;
			printf("Would you like to enter the Shop?\n 1/0 (yes/no) Answer: ");
			scanf_s("%d", &answer);
			while (answer < 0 || answer > 1) {
				printf("Not a valid answer, try again. ");
				scanf_s("%d", &answer);
			}
			if (answer == 1) {
				shopScreen(themap, &*kermitX, &*kermitY, Shop, &(*kermit), &*coins);
				if (r > 0 || r < 0) {
					*kermitX -= r;
				}
				else {
					*kermitY -= k;
				}
				themap.mArr[*kermitX][*kermitY] = '@';
			}
			else {
				if (r > 0 || r < 0) {
					if (themap.mArr[*kermitX][*kermitY + r] == ' ') {
						*kermitY += r;
					}
					else if (themap.mArr[*kermitX][*kermitY - r] == ' ') {
						*kermitY -= r;
					}
				}
				else {
					if (themap.mArr[*kermitX + k][*kermitY + r] == ' ') {
						*kermitX += k;
					} else if (themap.mArr[*kermitX - k][*kermitY + r] == ' ') {
						*kermitX -= k;
					}
				}
				themap.mArr[*kermitX][*kermitY] = '@';
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
}
