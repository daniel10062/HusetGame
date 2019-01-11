/*
* File: houseHelper.h
* Version: 1.0
* Last modified on Tue Nov 5 2018 by CaAn
* -----------------------------------------------------
* This interface provides several functions for the house game
* These functions are added by the student.
*/

#ifndef _househelper_h
#define _househelper_h

#include "houseManager.h"
#include "myIOLib.h"

enum states { INIT = 1, RUNNING, MENU, EXIT, WIN, SAVE, FIGHT };

enum states gameState;

enum operation { endgame, help, take, open, show, hide };
typedef enum operation operationT;
typedef const char *enumOperation[];
#define OPERATIONTEXTS { "avsluta", "hjälp", "tag", "öppna", "visa", "göm" }
#define NUMBEROFOPERATIONS 6

enum mapObject { none, key, up, down, left, right, door, map };
typedef enum mapObject mapObjectT;
typedef const char *enumMapObject[];
#define OBJECTTEXTS { "ingen", "nyckel","uppåt","nedåt","vänster","höger", "dörr","karta" }
#define NUMBEROFOBJECTS 8

struct userInput {
	operationT op;
	mapObjectT mObj;
};
typedef struct userInput inputT;

struct userPosition {
	int row, col;
};
typedef struct userPosition positionT;

struct Kermit {
	int prevX;
	int prevY;
	int posX;
	int posY;
	int HP;
	char weapons[100];
	int durability[100];
	char flasks[40][41];
	char currentWeapon;
};

struct Shop {
	char items[30][50 + 1];
};

typedef struct Enemy {
	int HP;
	int x;
	int y;
	int dead;
	//Weapons go from S - A - B - C - D - E
	char weapon;
	int stance; //In attack: 1 if not 0;
} Enemy;

/********************************************************************
Function: placeObject()
Usage: int success = placeObject(theMap, xPos, yPos,chr,posReceived,success);
---------------------------------------------------------------------
Purpose:Places a specific charater, symbolising an object, on the map.
Arguments: The functions takes the map-structure as a parameter, the
indexes of the place and the character to place. If index 0,0 is passed
the function will pick a random index. If the character gets a randomly 
determined position it is returned via positionT-argument.The last argument 
determines if the object-charater should be visible on the map (1=visible/0=not shown).
If placement is successful the function return 1 (true) otherwise 0 (false).
********************************************************************/
int placeObject(MapT , int, int, char, positionT *, int);

/********************************************************************
Function: getUserInput()

Purpose:reads the userinput as a string and tries to determine the operation 
and the object (to operate on).
Arguments: The functions takes no arguments.
The function return the input object as a structure of type inputT. 
This structure has two components op (of type operationT) and 
mObj (of type mapObjectT). If successful they are given the appropriate 
values, otherwhise both are set to -1.
********************************************************************/
inputT getUserInput(void);

void spawnEnemy(Enemy *enemyarr, MapT themap, inputT *obj, int *W, int *H);
void addShop(MapT themap, positionT *test);
void shopNearby(int *kermitX, int *kermitY, MapT themap, struct Shop Shop, struct Kermit *kermit, int *coins);
void shopScreen(MapT themap, int *kermitX, int *kermitY, struct Shop Shop, struct Kermit *kermit, int *coins);
void addCoins(MapT themap, positionT *test);
void loadScreen(MapT themap, int *key, int *width, int *height, int *flashlight, int *kermitX, int *kermitY);
void creatorScreen(int *gamestate, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY);
void initFunc(int *gameState, MapT theMap, int *keys, int *flashlight, int  *W, int *H, int *kermitX, int *kermitY);
void updateMap(MapT theMap, int *firstEntry);
void howToPlay(int *gameState, MapT theMap, int *keys, int *W, int *H, int *flashlight, int *kermitX, int *kermitY);
void action(inputT inputVal, int *kermitX, int *kermitY, MapT themap);
void updateKermitPoss(int *kermitX, int *kermitY, int *prevX, int *prevY, MapT themap);
void sightRadius(int *kermitX, int *kermitY, MapT themap, int changer, char siteRange);
int checkActionValid(inputT inputVal, int *kermitX, int *kermitY, MapT themap, int *keys, int *gamestate, int *flashlight, int *coins);
int checkOpenDoor(int *kermitX, int *kermitY, MapT themap);
int checkWinDoor(int *kermitX, int *kermitY, MapT themap);
void collateralSightCalc(MapT themap, int *height, int *width, int *kermitX, int *kermitY);
void addFlashlight(MapT themap, int width, int height, positionT *test, int choiceX, int choiceY);
int checkIfMoveFlashlight(int *kermitX, int *kermitY, MapT themap, int *flashlight);
void saveScreen(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY);
void createFile(int *key, int *flashlight, MapT themap, int *width, int *height, int *kermitX, int *kermitY);
void checkFight(MapT themap, char site, int *gamestate, Enemy *enemyarr);
//void enemyMove(Enemy *enemyarr, MapT themap, int *kX, int *kY);
void loadFile(MapT themap, int *key, int *width, int *height, int *kermitX, int *kermitY);
int weaponList(struct Kermit *kermit);
void currentWeaponUpdate(struct Kermit *kermit, int *damage);
void updatedurability(struct Kermit *, int a);
void moveInput(int *move, int *keyCheck, int *doorcheck);
void validMoveInput(int *move, MapT themap, struct Kermit *kermit, int *gamestate, int *flashlight, int *coins, int *checkkey, int *keys, int *doorCheck);
void showOptions(int hover);
int isInArr(int val, int *arr, int size);
void remove_element(int *array, int index, int array_length);

#endif
