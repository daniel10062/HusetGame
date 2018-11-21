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

