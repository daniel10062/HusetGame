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
#include <Windows.h>
#include <stdbool.h>

#define TRUE 1
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
	unsigned char * input = getString("\nVad vill du g�ra: "); // getString() from the myIOLib library or use similar function in Roberts library 
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
	for (int i = 0; i < 13; i++) {
		int succes = placeObject(themap, 0, 0, inputval, &*test, 0);
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
				printf("En nyckel hittad! G�r f�r att plocka upp den... (Din totala m�ngd nycklar �r %d)", *keys);
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
	char *temp[100];
	int hover = 0;
	char singlechar;
	unsigned char valuetesting;
	showITEMS:system("cls");
	int index = 0;
	if (hover > 3) {
		hover = 0;
	}
	else if (hover < 0) {
		hover = 0;
	}
	printf("\t\t\t\t\t\t Your coins: %d\n", *coins);
	for (int a = 0; a < 4; a++) {
		if (hover == a) {
			printf("%d: " ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, index + 1, Shop.items[a]);
		}
		else {
			printf("%d: %s\n", index + 1, Shop.items[a]);
		}
		index++;
	}
	int ch;
	do {
		ch = _getch();
		if (ch == 0 || ch == 224)
		{
			switch (_getch())
			{
			case 72:
				hover -= 1;
				goto showITEMS;
				break;

			case 80:
				hover += 1;
				goto showITEMS;
				break;
			}
		}
	} while (ch != 13);

	switch (hover) {
	case 0:
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
		printf("\n\nTransaction Complete!");
		Sleep(2500);
		break;
	case 1:
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
				printf("\n\nTransaction Complete!");
				Sleep(2500);
				updatedurability(&*kermit, a);
				break;
			}
		}
		break;
	case 2:
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
				printf("\n\nTransaction Complete!");
				updatedurability(&kermit, a);
				Sleep(2500);
				break;
			}
		}
		break;
	case 3:
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
		printf("\n\nTransaction Complete!");
		Sleep(2500);
		break;
	case 4:
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

void loadScreen(MapT themap, int *key, int *width, int *height, int *flashlight, int *kermitX, int *kermitY, int *moves, int *numEnemies) {
	//Add the functions about files	
	loadFile(themap, &*key, &*width, &*height, &*flashlight, &*kermitX, &*kermitY, &*moves, &*numEnemies);
}

void saveScreen(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY, int *moves, int *numEnemies) {
	createFile(&*key, &*flashlight, themap, &*width, &*height, &*kermitX, &*kermitY, &*moves, &*numEnemies);
}

void createFile(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY, int *moves, int *numEnemies) {
	//moves
	//number of enemies left
	char* filename = "MapInfoSAVED.txt";
	char* singleInfoFile = "singleInfo.txt";
	FILE *mapInfo;
	FILE *singleInfo;
	fopen_s(&mapInfo, filename, "w+");
	fopen_s(&singleInfo, singleInfoFile, "w+");
	//Storing the current data from the map
	for (int a = 0; a < *width; a++) {
		for (int b = 0; b < *height; b++) {
			fprintf(mapInfo, "%c", themap.mArr[a][b]);
			/*fputc(themap.mArr[a][b], mapInfo);*/
			if (themap.mArr[a][b] == '@') {
				system("cls");
				printf("We found kermit!! pos: %d, %d", a, b);
				Sleep(1500);

			}
			fprintf_s(mapInfo, "\n");
		}
	}
	//printing current number of keys
	fprintf(singleInfo, "%d\n", *key);
	fprintf(singleInfo, "%d\n", *moves);
	fprintf(singleInfo, "%d\n", *numEnemies);

	fclose(mapInfo);
	fclose(singleInfo);
}

void loadFile(MapT themap, int *key, int *width, int *height, int *kermitX, int *kermitY, int *moves, int *numEnemies) {
	FILE* mapFile;
	FILE* singleInfo;
	/*themap = createMap(*width, *height, 20);*/
	char* temp;
	unsigned char t;
	temp = malloc(12 * sizeof(char));
	fopen_s(&mapFile, "MapInfoSAVED.txt", "r");
	fopen_s(&singleInfo, "singleInfo.txt", "r");
	if (mapFile == NULL) {
		printf("Unable to open the file...");
		return;
	}
	if (singleInfo == NULL) {
		printf("Unable to open the file...");
		return;
	}
	fscanf_s(singleInfo, "%d\n", &*key);
	printf("Keys: %d\n", *key);
	fscanf_s(singleInfo, "%d\n", &*moves);
	printf("Moves: %d\n", *moves);
	Sleep(2500);
	fscanf_s(singleInfo, "%d", &*numEnemies);
	printf("Number of enemies: %d", *numEnemies);
	Sleep(2500);
	fclose(singleInfo);
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

void spawnEnemy(Enemy *enemyarr, MapT themap, inputT *obj, int *W, int *H, int pos) {
	// init all values for enemy object
		enemyarr[pos].HP = 150;
		char arr[8];
		int offset = 3;
		arr[0] = 'S';
		arr[1] = 'A';
		arr[2] = 'B';
		arr[3] = 'C';
		arr[4] = 'D';
		arr[5] = 'E';
		arr[6] = 'F';
		int randInt = RandomInteger(0, 6);
		enemyarr[pos].weapon = arr[randInt];
		enemyarr[pos].x = RandomInteger(4, *W - offset);
		enemyarr[pos].y = RandomInteger(4, *H - offset);
		enemyarr[pos].dead = 0;
		while (themap.mArr[enemyarr[pos].x][enemyarr[pos].y] != ' ') {
			enemyarr[pos].x = RandomInteger(4, *W - offset);
			enemyarr[pos].y = RandomInteger(4, *H - offset);
		}
		enemyarr[pos].stance = 0;
		int succes = placeObject(themap, enemyarr[pos].x, enemyarr[pos].y, '�', &*obj, 0);
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

void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY, int *moves, int *numEnemy, Enemy enemyarr[], positionT test) {
	//Shows a screen in which the creator information is being displayed
	system("cls");
	printf("Game: Huset\n");
	printf("Version: 0.8\n");
	printf("-------------\n");
	printf("Creator: Daniel Ekeroth");
	getchar();
	printf("\n\nPress enter to go back to the menu");
	getchar();
	initFunc(&*gamestate, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY, &*moves, &*numEnemy, &enemyarr, test);
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
		for (int r = r1 - 1; r < E1 + 1; r++) {
			for (int k = k1 - 1; k < E2 + 1; k++) {
				if (themap.mArr[*kermitX + r][*kermitY + k] == 'B') {
					themap.vArr[*kermitX + r][*kermitY + k] = 1;
				}
			}
		}
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

void howToPlay(int *gameState, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY, int *moves, int *numEnemy, Enemy enemyarr[], positionT test) {
	//Gives the user some information on how the game works
	system("cls");
	int choice = 0;
	printf(ANSI_COLOR_CYAN "KEY ELEMENTS TO KNOW ABOUT THE GAME" ANSI_COLOR_RESET "\n");
	printf("If you see a 'e' it means --> OutsideWall\n");
	printf("If you see a 'M' it means --> OutsideDoor\n");
	printf("If you see a 'K' it means --> Key\n");
	printf("If you see a 'D' it means --> Insidedoor\n");
	printf("If you see a 'w' it means --> insideWall\n");
	printf("If you see a '�' it means --> Enemy\n");
	printf("If you see a 'F' it means --> Flashlight\n");
	printf("If you see a 'B' it means --> Shop\n");
	printf("If you see a '@' it means --> Player Character\n");
	printf("\n" ANSI_COLOR_GREEN "---------------------------------------------------" ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_MAGENTA "IMPORTANT THINGS TO KNOW" ANSI_COLOR_RESET "\n");
	printf("You move your character by using your arrow keys.\n");
	printf("Try gather as much coins as possible in order to by powerful weapons to defeat your enemies!\n\n");
	printf("=================================================\nEnter 1 to go back to the meny or 2 to exit: --> ");
	scanf_s("%d", &choice);
	if (choice == 1) {
		initFunc(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY, &*moves, &*numEnemy, &enemyarr, test);
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

void drawOptions(int *hover) {
	if (*hover < 1) {
		*hover = 5;
	}
	else if (*hover > 5) {
		*hover = 1;
	}
	system("cls");
	printf("\n\n\t\t\t\t\t\t  Welcome  to  Huset\n\n");
	if (*hover == 1) {
		printf(ANSI_COLOR_MAGENTA "\t\t\t\t\t\t       New  Game" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t\t       New  Game\n\n");
	}
	if (*hover == 2) {
		printf(ANSI_COLOR_MAGENTA	"\t\t\t\t\t              Load   Game" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t              Load   Game\n\n");
	}
	if (*hover == 3) {
		printf(ANSI_COLOR_MAGENTA"\t\t\t\t\t             Creator  Info" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t             Creator  Info\n\n");
	}
	if (*hover == 4) {
		printf(ANSI_COLOR_MAGENTA"\t\t\t\t\t             How  To  Play" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t             How  To  Play\n\n");
	}
	if (*hover == 5) {
		printf(ANSI_COLOR_MAGENTA"\t\t\t\t\t                 Exit" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t                 Exit\n\n");
	}
}

void drawDiff(int *hoverInt) {
	if (*hoverInt < 1) {
		*hoverInt = 3;
	}
	else if (*hoverInt > 3) {
		*hoverInt = 1;
	}
	system("cls");
	printf(ANSI_COLOR_CYAN "\t\t\t\t\t\t  Choose difficulty \n\n" ANSI_COLOR_RESET);
	if (*hoverInt == 1) {
		printf(ANSI_COLOR_MAGENTA "\t\t\t\t\t\t       Easy" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t\t       Easy\n\n");
	}
	if (*hoverInt == 2) {
		printf(ANSI_COLOR_MAGENTA	"\t\t\t\t\t	       Medium" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t	       Medium\n\n");
	}
	if (*hoverInt == 3) {
		printf(ANSI_COLOR_MAGENTA"\t\t\t\t\t	       Hard" ANSI_COLOR_RESET "\n\n");
	}
	else {
		printf("\t\t\t\t\t	       Hard\n\n");
	}
}

void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight, int  *W, int *H, int *kermitX, int *kermitY, int *moves, int *numEnemies, Enemy *enemyarr,positionT test ) {
	int menuChoice = 0;
	int hover = 1;
	//fixing menu
	drawOptions(&hover);
	unsigned int difficulty = 0;
	int DiffHover = 1;
	int ch;
	int hc;
	START:if (*gameState == INIT) {
		do {
			ch = _getch();
			if (ch == 13) {
				switch (hover) {
				case 1:
					printf(ANSI_COLOR_CYAN "Please choose difficulty level" ANSI_COLOR_RESET "\n");
					drawDiff(&DiffHover);
					do {
						hc = _getch();
						if (hc == 13) {
							if (DiffHover == 1) {
								difficulty = 15;
							}
							else if (DiffHover == 2) {
								difficulty = 20;
							}
							else if (DiffHover == 3) {
								difficulty = 25;
							}
							goto ENDWHILE;
						} else if (hc == 0 || hc == 224) {
							switch (_getch())
							{
							case 72:
								DiffHover -= 1;
								drawDiff(&DiffHover);
								break;
							case 80:
								DiffHover += 1;
								drawDiff(&DiffHover);
								break;
							}
						}
					} while (hc != 13);
					ENDWHILE:for (int num = 1; num <= *numEnemies; num++) {
						spawnEnemy(&*enemyarr, theMap, &test, &*W, &*H, num - 1);
					}
					addFlashlight(theMap, &*W, &*H, &test, 0, 0);
					for (int a = 0; a < 6; a++) {
						addShop(theMap, &test, *W, *H);
					}
					addCoins(theMap, &test);
					while (theMap.mArr[*kermitX][*kermitY] != ' ') {
						*kermitX += 1;
					}
					theMap.mArr[*kermitX][*kermitY] = '@';
					sightRadius(&*kermitX, &*kermitY, theMap, 1, 'S');
					*gameState = RUNNING;
					goto START;
				case 2:
					printf("File is being loaded....\n");
					loadScreen(theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY, &*moves, &*numEnemies, &enemyarr, test);
					Sleep(2000);
					*gameState = RUNNING;
					goto START;
				case 3:
					creatorScreen(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY, &*moves, &*numEnemies, &enemyarr, test);
					hover = 1;
					drawOptions(&hover);
					break;
				case 4:
					howToPlay(&*gameState, theMap, &*keys, &*W, &*H, &*flashlight, &*kermitX, &*kermitY, &*moves, &*numEnemies, &enemyarr, test);
					hover = 1;
					drawOptions(&hover);
					break;
				case 5:
					*gameState = EXIT;
					goto START;
				}
			}
			else if (ch == 0 || ch == 224) {
				switch (_getch())
				{
				case 72:
					hover -= 1;
					drawOptions(&hover);
					break;
				case 80:
					hover += 1;
					drawOptions(&hover);
					break;
				}
			}
		} while (ch != 13);
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

void addShop(MapT themap, positionT *test, int width, int height) {
	int wArr[254];
	int hArr[254];
	int index = 0;
	for (int n = 0; n < width; n++) {
		if (n % 4 == 0) {
			wArr[index] = n;
			index++;
		}
	}
	index = 0;
	for (int n = 0; n < height; n++) {
		if (n % 4 == 0) {
			hArr[index] = n;
			index++;
		}
	}
	char inputval = 'B';
	int posX, posY;
	int offset = 2;
	int loopStart = 0;
	int side[4];
	side[0] = offset;
	side[1] = width - offset;
	side[2] = offset;
	side[3] = height - offset;
	INITVALUES:posX = RandomInteger(side[0], side[1]);
	posY = RandomInteger(side[2], side[3]);
	while (!isInArr(posX, wArr, sizeof(wArr) / sizeof(wArr[0]))) {
		posX = RandomInteger(side[0], side[1]);
	}
	for (int x = 0; x < sizeof(wArr) / sizeof(wArr[0]); x++) {
		if (wArr[x] == posX) {
			remove_element(wArr, x, sizeof(wArr) / sizeof(wArr[0]));
		}
	}
	while (!isInArr(posY, hArr, sizeof(hArr) / sizeof(hArr[0]))) {
		posY = RandomInteger(side[2], side[3]);
	}
	for (int y = 0; y < sizeof(hArr) / sizeof(hArr[0]); y++) {
		if (hArr[y] == posY) {
			remove_element(hArr, y, sizeof(hArr) / sizeof(hArr[0]));
		}
	}
	if (themap.mArr[posX][posY] == ' ') {
		int succes = placeObject(themap, posX, posY, inputval, &*test, 0);
	}
	else {
		goto INITVALUES;
	}
}

int isInArr(int val, int *arr, int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (arr[i] == val)
			return true;
	}
	return false;
}

void remove_element(int *arr, int index, int array_length)
{
	int i;
	for (i = index; i < array_length - 1; i++) {
		arr[i] = arr[i + 1];
	}
}

void showShopOpt(int hover){
	system("cls");
	printf("Would you like to enter the Shop?\n");
	if (hover == 1) {
		printf(ANSI_COLOR_CYAN "Yes" ANSI_COLOR_RESET "\n");
	}
	else {
		printf("Yes\n");
	}
	if (hover == 2) {
		printf(ANSI_COLOR_CYAN "No" ANSI_COLOR_RESET "\n");
	}
	else {
		printf("No\n");
	}
}

void shopNearby(int *kermitX, int *kermitY, MapT themap, struct Shop Shop, struct Kermit *kermit, int *coins, char siterange, int *height, int *width) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	int hover = 1;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[*kermitX + r][*kermitY + k] == 'B') {
			int ch;
			SHOPVALUES:showShopOpt(hover);
			do {
				ch = _getch();
				if (ch == 13) {
					system("cls");
					if (hover == 1) {
						shopScreen(themap, &*kermitX, &*kermitY, Shop, &(*kermit), &*coins);
						themap.mArr[*kermitX][*kermitY] = ' ';
						collateralSightCalc(themap, &*height, &*width, &*kermitX, &*kermitY);
						*kermitX = kermit->prevX;
						*kermitY = kermit->prevY;
						updateKermitPoss(&kermit->posX, &kermit->posY, &kermit->prevX, &kermit->prevY, themap);
						sightRadius(&kermit->posX, &kermit->posY, themap, 1, siterange);
					}
					else if (hover == 2) {
						themap.mArr[*kermitX][*kermitY] = ' ';
						collateralSightCalc(themap, &*height, &*width, &*kermitX, &*kermitY);
						*kermitX = kermit->prevX;
						*kermitY = kermit->prevY;
						updateKermitPoss(&kermit->posX, &kermit->posY, &kermit->prevX, &kermit->prevY, themap);
						sightRadius(&kermit->posX, &kermit->posY, themap, 1, siterange);
					}
				}
				else if (ch == 0 || ch == 224) {
					switch (_getch()) {
					case 72:
						hover -= 1;
						if (hover < 1) {
							hover = 2;
						}
						goto SHOPVALUES;
					case 80:
						hover += 1;
						if (hover > 2) {
							hover = 1;
						}
						goto SHOPVALUES;
					}
				}
			} while (ch != 13);
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

void updatedurability(struct Kermit *kermit, int a) {
	if (kermit->weapons[a] == 'S') {
		kermit->durability[a] = 15;
	}
	else if (kermit->weapons[a] == 'A') {
		kermit->durability[a] = 13;
	}
	else if (kermit->weapons[a] == 'B') {
		kermit->durability[a] = 11;
	}
	else if (kermit->weapons[a] == 'C') {
		kermit->durability[a] = 9;
	}
	else if (kermit->weapons[a] == 'D') {
		kermit->durability[a] = 7;
	}
	else if (kermit->weapons[a] == 'E') {
		kermit->durability[a] = 5;
	}
	else if (kermit->weapons[a] == 'F') {
		kermit->durability[a] = 100;
	}
}

void currentWeaponUpdate(struct Kermit *kermit, int *damage) {
	if (kermit->currentWeapon == 'S') {
		*damage = 35;
	}
	else if (kermit->currentWeapon == 'A') {
		*damage = 30;
	}
	else if (kermit->currentWeapon == 'B') {
		*damage = 25;
	}
	else if (kermit->currentWeapon == 'C') {
		*damage = 20;
	}
	else if (kermit->currentWeapon == 'D') {
		*damage = 15;
	}
	else if (kermit->currentWeapon == 'E') {
		*damage = 10;
	}
	else if (kermit->currentWeapon == 'F') {
		*damage = 5;
	}
}

int weaponList(struct Kermit *kermit, int highlight) {
	int invSize = strlen(kermit->weapons);
	/*int choice = 0;*/
	int ch;
	if (invSize == 0 && kermit->currentWeapon != 'F') {
		printf("\nYou hade no weapons in your inventory so your fighting with your fists only!");
		kermit->currentWeapon = 'F';
		Sleep(2500);
		return;
	}
	else {
		printf("Please choose from your inventory ---> \n");
	}
SCREENOPT:system("cls");
	if (highlight < 1) {
		highlight = invSize;
	}
	else if (highlight > invSize) {
		highlight = 1;
	}
	for (int i = 1; i < invSize + 1; i++) {
		if (i == highlight) {
			printf("%d:"ANSI_COLOR_RED " Weapon Class %c" ANSI_COLOR_RESET " (Durability: %d)\n", i, kermit->weapons[i - 1], kermit->durability[i - 1]);
		}
		else {
			printf("%d: Weapon Class %c (Durability: %d)\n", i, kermit->weapons[i - 1], kermit->durability[i - 1]);
		}
	}
	do {
		ch = _getch();
		if (highlight < 1) {
			highlight = invSize;
		}
		else if (highlight > invSize) {
			highlight = 1;
		}
		if (ch == 13) {
			goto CHOICE;
		}
		else if (ch == 0 || ch == 224) {
			switch (_getch()) {
			case 72:
				highlight -= 1;
				goto SCREENOPT;
			case 80:
				highlight += 1;
				goto SCREENOPT;
			}
		}
	} while (ch == 13);
	CHOICE:kermit->currentWeapon = kermit->weapons[highlight - 1];
	printf("Your current weaponclass is %c\n", kermit->currentWeapon);
	return (highlight - 1);
}


void validMoveInput(int *move, MapT themap, struct Kermit *kermit, int *gamestate, int *flashlight, int *coins, int *checkkey, int *keys, int *doorCheck) {
	if (*checkkey == 1) {
		checkKeyMove(themap, &*kermit, &*keys, move);
	}
	else if (*doorCheck == 1) {
		checkDoorMove(themap, &*kermit, &*keys);
	}
	else if (move[0] == 1){
		if (themap.mArr[kermit->posX - 1][kermit->posY] == ' ') {
			kermit->posX -= 1;
		}
		else if (checkOpenDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			kermit->posX -= 1;
		}
		else if (checkWinDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			*gamestate = WIN;
			kermit->posX -= 1;
		}
		else if (checkIfMoveFlashlight(&kermit->posX, &kermit->posY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			kermit->posX -= 1;
		}
		else if (checkIfMoveCoins(&kermit->posX, &kermit->posY, themap, &*coins) == 1) {
			kermit->posX -= 1;
		}
	}
	else if (move[1] == 1) {
		if (themap.mArr[kermit->posX + 1][kermit->posY] == ' ') {
			kermit->posX++;
		}
		else if (checkOpenDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			kermit->posX += 1;
		}
		else if (checkWinDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			*gamestate = WIN;
			kermit->posX++;
		}
		else if (checkIfMoveFlashlight(&kermit->posX, &kermit->posY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			kermit->posX++;
		}
		else if (checkIfMoveCoins(&kermit->posX, &kermit->posY, themap, &*coins) == 1) {
			kermit->posX++;
		}
	}
	else if (move[2] == 1) {
		if (themap.mArr[kermit->posX][kermit->posY + 1] == ' ') {
			kermit->posY++;
		}
		else if (checkOpenDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			kermit->posY += 1;
		}
		else if (checkWinDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			*gamestate = WIN;
			kermit->posY++;
		}
		else if (checkIfMoveFlashlight(&kermit->posX, &kermit->posY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			kermit->posY++;
		}
		else if (checkIfMoveCoins(&kermit->posX, &kermit->posY, themap, &*coins) == 1) {
			kermit->posY++;
		}
		else if (checkIfMoveCoins(&kermit->posX, &kermit->posY, themap, &*coins) == 1) {
			kermit->posY++;
		}
	}
	else if (move[3] == 1) {
		if (themap.mArr[kermit->posX][kermit->posY - 1] == ' ') {
			kermit->posY--;
		}
		else if (checkOpenDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			kermit->posY -= 1;
		}
		else if (checkWinDoor(&kermit->posX, &kermit->posY, themap) == 1) {
			*gamestate = WIN;
			kermit->posY--;
		}
		else if (checkIfMoveFlashlight(&kermit->posX, &kermit->posY, themap, &*flashlight) == 1) {
			*flashlight = 1;
			kermit->posY--;
		}
		else if (checkIfMoveCoins(&kermit->posX, &kermit->posY, themap, &*coins) == 1) {
			kermit->posY--;
		}
	}
	*checkkey = 0;
	*doorCheck = 0;
}

void moveInput(int *move, int *keyCheck, int *doorcheck, int *gamestate) {
	*keyCheck = 0;
	*doorcheck = 0;
	int ch;
	for (int a = 0; a < 4; a++) {
		move[a] = 0;
	}
	ch = _getch();
	if (ch == 107 || ch == 75) {
		*keyCheck = 1;
	}
	else if (ch == 68 || ch == 100) {
		*doorcheck = 1;
	}
	else if (ch == 27) {
		*gamestate = MENU;
	}
	else if (ch == 0 || ch == 224)
	{
		switch (_getch())
		{
		case 72:
			move[0] = 1;
			break;

		case 80:
			move[1] = 1;
			break;
		case 77:
			move[2] = 1;
			break;
		case 75:
			move[3] = 1;
			break;
		}
	}
}


int checkKeyMove(MapT themap, struct Kermit *kermit, int *keys) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[kermit->posX + r][kermit->posY + k] == 'K') {
			kermit->posX += r;
			kermit->posY += k;
			*keys += 1;
			printf("You found a key! Number of keys in inventory: %d", *keys);
			Sleep(2500);
			return;
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
	printf("There is no key around your (x, y) axis to be picked up");
	Sleep(2500);
	return 0;
}

int checkDoorMove(MapT themap, struct Kermit *kermit, int *keys) {
	int r = -1;
	int k = 0;
	int temp = 0;
	int index = 0;
	for (int t = 0; t < 4; t++) {
		if (themap.mArr[kermit->posX + r][kermit->posY + k] == 'D') {
			if (*keys < 1) {
				printf("You were unable to open the door because you had no key");
				Sleep(3000);
				return;
			}
			else if (*keys > 0) {
				themap.mArr[kermit->posX + r][kermit->posY + k] = 'o';
				kermit->posX += 2 * r;
				kermit->posY += 2 * k;
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
	return 0;
}