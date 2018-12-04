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
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//	 GAME IS CURRENTLY WORKING: VERSION OF GAME 1.2		|
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

int main() {
	system("chcp 1252");
	Randomize();
	/*enum states gameState;*/
	gameState = INIT;
	int firstEntry = TRUE;
	typedef struct houseMap MapT;
	struct Shop Shop;
	struct Kermit kermit;
	kermit.HP = 150;
	//Giving a start Sword for Kermit
	int mapShown = 0;
	int doorCheck = 0;
	char siteRange = 'S';
	unsigned int W = 0;
	unsigned int H = 0;
	unsigned int moves = 0;
	unsigned int moveF = 0;
	int coins = 300;
	int flashlight = 0;
	int move[4];
	int keyCheck = 0;
	//debug mode!
	unsigned int keys = 4;
	
	//-----------------------------------------------------------
	//shop items

	const char *testchar[100];
	testchar[0] = "Cost: 75 coins --> Healing Flask (50 Healing)";
	testchar[1] = "Cost: 150 coins --> Sword Class B";
	testchar[2] = "Cost 300 coins --> Sword Class S";
	testchar[3] = "Cost 75 coins --> Damage Flask (50 Damage)";
	for (int a = 0; a < 4; a++) {
		strcpy_s(Shop.items[a], sizeof(Shop.items[a]), testchar[a]);
	}

	//-----------------------------------------------------------

	//-----------------------------------------------------------

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

	addFlashlight(theMap, &W, &H, &test, 0, 0);
	addShop(theMap,&test);
	addCoins(theMap, &test);
	inputT inputVal;

	while (TRUE) {
		if (gameState == INIT) {
			int loopvalue = 0;
			kermit.posX = W / 2 - 1;
			kermit.posY = H / 2 - 1;
			//adding default weapon
			char t = 'S';
			kermit.weapons[0] = t;
			kermit.weapons[1] = '\0';

			while (theMap.mArr[kermit.posX][kermit.posY] != ' ') {
				if (loopvalue == 4) {
					kermit.posX++;
				}
				else {
					kermit.posY++;
				}
				loopvalue++;
			}
			theMap.mArr[kermit.posX][kermit.posY] = '@';
			sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
			initFunc(&gameState, theMap, &keys, &flashlight, &W, &H, &kermit.posX, &kermit.posY);
		}
		else if (gameState == RUNNING) {
			for (int a = 0; a < strlen(kermit.weapons); a++) {
				updatedurability(&kermit, a);
			}
			//Action set
			if (flashlight == 0) {
				siteRange = 'S';
			}
			else {
				siteRange = 'M';
			}
			updateMap(theMap, &firstEntry);

			printf("Number of currently moves: | > %d < |\n\n", moves);

			printf("Current coin value: %d\n", coins);

			//user input using standarn input
			/*inputVal = getUserInput();*/

			//user input using arrow keys
			moveInput(&move, &keyCheck, &doorCheck);

			sightRadius(&kermit.posX, &kermit.posY, theMap, 0, siteRange);
			collateralSightCalc(theMap, &H, &W, &kermit.posX, &kermit.posY);
			kermit.prevX = kermit.posX;
			kermit.prevY = kermit.posY;

			
			/*while (checkActionValid(inputVal, &kermit.posX, &kermit.posY, theMap, &keys, &gameState, &flashlight, &coins) < 1) {
				system("cls");
				sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
				drawMap(theMap);
				inputVal = getUserInput();
			}*/


			if (flashlight == 1) {
				moveF++;
				if (moveF >= 14) {
					moveF = 0;
					flashlight = 0;
				}
			}
			moves++;

			validMoveInput(move, theMap, &kermit, &gameState, &flashlight, &coins,&keyCheck, &keys, &doorCheck);
			/*action(inputVal, &kermit.posX, &kermit.posY, theMap);*/
			updateKermitPoss(&kermit.posX, &kermit.posY, &kermit.prevX, &kermit.prevY, theMap);
			sightRadius(&kermit.posX, &kermit.posY, theMap, 1, siteRange);
			checkFight(theMap, siteRange, &gameState, &enemyArr);
			shopNearby(&kermit.posX, &kermit.posY, theMap, Shop, &kermit, &coins, siteRange, &H, &W);
		}
		else if (gameState == MENU) {
			system("cls");
		}
		else if (gameState == EXIT) {
			break;
		}
		else if (gameState == WIN) {
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
			int highlight = 1;
			printf("Watch out, an enemy is attacking you!");
			//debugMessage
			printf("Entering battlestance...");
			Sleep(2000);
			system("cls");
			int attackerIndex = 0;
			for (int a = 0; a < 5; a++) {
				if (enemyArr[a].stance == 1 && enemyArr[a].dead != 1) {
					attackerIndex = a;
					printf("The attacker is: %d\n", a);
					Sleep(1500);
				}
			}
			//Init the screen;
			printf("Fight progressing...\n");
			Sleep(1000);
			int choice = weaponList(&kermit, highlight);

			currentWeaponUpdate(&kermit, &damage);
			if (enemyArr[attackerIndex].weapon == 'S') {
				enemyDmg = 35;
			}
			else if (enemyArr[attackerIndex].weapon == 'A') {
				enemyDmg = 30;
			}
			else if (enemyArr[attackerIndex].weapon == 'B') {
				enemyDmg = 25;
			}
			else if (enemyArr[attackerIndex].weapon == 'C') {
				enemyDmg = 20;
			}
			else if (enemyArr[attackerIndex].weapon == 'D') {
				enemyDmg = 15;
			}
			else if (enemyArr[attackerIndex].weapon == 'E') {
				enemyDmg = 10;
			}
			else if (enemyArr[attackerIndex].weapon == 'F') {
				enemyDmg = 5;
			}
			//FightingLoop
			while (kermit.HP > 0 || enemyArr[attackerIndex].HP > 0) {
				if (kermit.HP < 1 || enemyArr[attackerIndex].HP < 1) {
					break;
				}
				char *lookWord = malloc(24 * sizeof(char));
				int flaskchoice = 0;
				int numberOfItems = 0;
				int switchRun = 0;
				int forceBack = 0;
				int n = 0;
				unsigned char testingChar;
				if (kermit.durability[choice] <= 0) {
					kermit.currentWeapon = ' ';
					for (int a = choice; a < strlen(kermit.weapons); a++) {
						kermit.weapons[a] = kermit.weapons[a + 1];
					}
					for (int a = choice; a < strlen(kermit.durability); a++) {
						kermit.durability[a] = kermit.durability[a + 1];
					}
					choice = weaponList(&kermit);
					currentWeaponUpdate(&kermit, &damage);
				}
				system("cls");
				printf("Your HP: %d \t\t\t\t\t\t\t Enemy HP: %d\n", kermit.HP, enemyArr[attackerIndex].HP);
				printf("Current WeaponClass: %c (Durability: %d) \t\t\t Current enemy weaponclass: %c\n\n", kermit.currentWeapon,kermit.durability[choice], enemyArr[attackerIndex].weapon);

				printf("Please choose move: \n");
				int attackChoice = 0;

				printf("1: Quick Attack\n2: Heavy Attack\n3: Usables\n Choice: ");
				scanf_s("%d", &attackChoice);
				while (attackChoice < 1 || attackChoice > 3) {
					printf("Not a valid attackmove, please try again: ");
					scanf_s("%d", &attackChoice);
				}
				switch (attackChoice) {
				case 1:
					kermit.durability[choice]--;
					enemyArr[attackerIndex].HP -= damage;
					printf("Nice hit! You did %d damage", damage);
					Sleep(2500);
					break;
				case 2:
					//init 50% chance of hit
					randomNum = RandomInteger(1, 2);
					if (randomNum == 1) {
						kermit.durability[choice] -= 2;
						enemyArr[attackerIndex].HP -= damage * 2;
						printf("Great hit! You did %d damage", damage * 2);
						Sleep(2500);
						break;
					}
					else {
						printf("The heavy hit missed...");
						Sleep(2500);
						break;
					}
				case 3:
					//make a screen where it shows all flasks availible
					system("cls");
					switchRun = 1;
					char *assign[100];
					for (n; n < (sizeof(kermit.flasks) / sizeof(kermit.flasks[0])); n++) {
						testingChar = kermit.flasks[n][0];
						if (isalpha(testingChar)) {
							numberOfItems++;
							printf("Item %d: %s\n", n + 1, kermit.flasks[n]);
						}
					}
					if (numberOfItems == 0) {
						printf("You had no items in your inventory\n");
						Sleep(2500);
						break;
					}
					printf("Please choose Item (0: return): ");
					scanf_s("%d", &flaskchoice);
					while (flaskchoice < 0 || flaskchoice > numberOfItems) {
						printf("Not a valid number, try again: ");
						scanf_s("%d", &flaskchoice);
					}
					if (flaskchoice == 0) {
						forceBack = 1;
						break;
					}
					forceBack = 1;
					switchRun = 1;
					for (int a = 0; a < strlen(kermit.flasks[flaskchoice - 1]); a++) {
						if (kermit.flasks[flaskchoice - 1][a] == ' ') {
							lookWord[a] = '\0';
							break;
						}
						lookWord[a] = kermit.flasks[flaskchoice - 1][a];
					}
					if (strcmp("Healing", lookWord) == 0) {
						assign[0] = ' ';
						strcpy_s(kermit.flasks[flaskchoice - 1], sizeof(kermit.flasks[flaskchoice - 1]), assign);
						kermit.HP += 50;
						printf("Great job, you healed for 50 HP!\n");
						Sleep(2500);
					} else if (strcmp("Damage", lookWord) == 0) {
						assign[0] = ' ';
						strcpy_s(kermit.flasks[flaskchoice - 1], sizeof(kermit.flasks[flaskchoice - 1]), assign);
						enemyArr[attackerIndex].HP -= 50;
						printf("Great job, you did 50 Damage on the enemy!\n");
						Sleep(2500);
					}
					/*switch (flaskchoice) {
					case 1: 
						forceBack = 1;
						switchRun = 1;
						for (int a = 0; a < strlen(kermit.flasks[flaskchoice - 1]); a++) {
							if (kermit.flasks[flaskchoice - 1][a] == ' ') {
								lookWord[a] = '\0';
								break;
							}
							lookWord[a] = kermit.flasks[flaskchoice - 1][a];
						}
						if (strcmp("Healing", lookWord) == 0) {
							assign[0] = ' ';
							strcpy_s(kermit.flasks[flaskchoice - 1], sizeof(kermit.flasks[flaskchoice - 1]), assign);
							kermit.HP += 50;
							printf("Great job, you healed for 50 HP!\n");
							Sleep(2500);
						}
						break;
					case 2:
						forceBack = 1;
						switchRun = 1;
						for (int a = 0; a < strlen(kermit.flasks[sizeof(kermit.flasks) / sizeof(kermit.flasks[0]) - 1]); a++) {
							if (kermit.flasks[flaskchoice - 1][a] == ' ') {
								lookWord[a] = '\0';
								break;
							}
							lookWord[a] = kermit.flasks[flaskchoice - 1][a];
						}
						if (strcmp("Damage", lookWord) == 0) {
							assign[0] = ' ';
							strcpy_s(kermit.flasks[flaskchoice - 1], sizeof(kermit.flasks[flaskchoice - 1]), assign);
							enemyArr[attackerIndex].HP -= 50;
							printf("Great job, you damaged the enemy for 50 damage!\n");
							Sleep(2500);
						}
						break;
					}*/
					break;
				}
				if ((numberOfItems == 0 && switchRun == 1) || forceBack == 1) {
					switchRun = 0;
					continue;
				}
				int enemyattackChoice = 0;
				int randnum = 0;
				enemyattackChoice = RandomInteger(2, 3);
				if (enemyattackChoice == 2) {
					printf("\nEnemy is attacking with quick attack...");
					kermit.HP -= enemyDmg;
					printf("\nYou lost %d heath. Current heath: %d", enemyDmg, kermit.HP);
					Sleep(1500);
				}
				else {
					printf("\nEnemy is attacking with heavy attack...");
					randnum = RandomInteger(1, 2);
					if (randnum == 1) {
						kermit.HP -= 2*enemyDmg;
						printf("\nYou lost %d heath. Current heath: %d", enemyDmg, kermit.HP);
						Sleep(1500);
					}
					else {
						printf("\nThe enemy missed you! How lucky!?");
						Sleep(1500);
					}
				}
			}
			if (kermit.HP < 1) {
				gameState = EXIT;
				break;
			}
			else if (enemyArr[attackerIndex].HP < 1) {
				enemyArr[attackerIndex].dead = 1;
				theMap.mArr[enemyArr[attackerIndex].x][enemyArr[attackerIndex].y] = ' ';
				gameState = RUNNING;
				continue;
			}
		}
	}
	printf("Game is now closing: Press enter to continue...");
	getchar();
	return 0;
}
