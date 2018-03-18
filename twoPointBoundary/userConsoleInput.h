//#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "string.h"
#include "stdlib.h"

char * userConsoleInput(char * helpInfo);


/*help info is the string displayed above where input is accepted to help user with formatting*/
char * userConsoleInput(char * helpInfo) {
	int charCount = 0;//count of current characters in returned string
	int limit = 290;//limit of characters in returned string
	char c;//variable for most recently input character
	char *tmp;//pointer for dynamic string manipulation functionality
	char *result = (char *)malloc(300 * sizeof(char));//string to return
	memset(result, 0, 300);//zeroing out memory for return string
	do {

		if ((charCount != 0) && (charCount <= limit))//if this is not the first char, and the last character was not a backspace
			strncat(result, &c, 1);//add the new character to the string
		charCount = strlen(result);//update the length of the string
		/**************Display help info, input string, and character count***************/
		if (charCount == limit)
			printf("%s\n\nPress [ENTER] when done.\nCharacter count : %d of %d\tMaximum length reached.\n\n:> %s", helpInfo, charCount, limit, result);
		else
			printf("%s\n\nPress [ENTER] when done.\nCharacter count : %d of %d\n\n:> %s", helpInfo, charCount, limit, result);
		/************grab the users keypress*************/
		c = getch();
		/****************if user enters backspace*********************/
		if (c == (char)0x8) {
			tmp = result;//set tmp pointer to the first character in the path string
			while (*tmp != NULL) { tmp++; } //advance to the end of the string
			tmp--;//move tmp pointer to last character
			*tmp = (char)0x00;//overwrite that character with NULL
			charCount = 0;//set char count to 0 to prepare for next iteration
		}//if
		 /***************otherwise some other character was entered*****************/
		else if (c == (char)0x1B) {
			//if user presses escape key, do nothing, otherwise string anomolies occur
		}
		else if (c != (char)0xD)//as long as that character is not ENTER
			charCount++;//update the character count
		system("cls");//clear the console
	} while (c != (char)0xD);//do this loop while the character entered is not ENTER(0xD)
	return result;
}//userConsoleInput