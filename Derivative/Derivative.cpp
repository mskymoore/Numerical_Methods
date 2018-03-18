/*
PROGRAMMER: M. Skylar Moore 0939795
COURSE: CSCI 3321
DATE: 6/11/2016
ASSIGNMENT: Assigment 1: Rounding Errors versus Truncation Errors
ENVIRONMENT: C++ on Windows 7
FILES INCLUDED: Derivative.cpp, Derivative_Log.txt, math.h, stdio.h, string.h, stdlib.h, conio.h
PURPOSE: This program uses the difference approximation formula to approximate the derivative of cosh(x) at x = 0.881373587019543
		 to illustrate the effects of rounding errors and truncation errors by displaying the error.
INPUT: x = 0.881373587019543, path to output location
PRECONDITIONS: Up to date C++ compiler
OUTPUT: h of the approximation formula, f'(x) approximation and error of f'(x) for each h to a file named Derivative_Log.txt
POSTCONDITIONS: A .txt file named "Derivative_Log.txt" is created with the output in the path specified by the user.
ALGORITHM:

include files

define constants
define functions
define output text field lengths

main
	local declarations
	allocate memory for path of output file and buffer for output text
	zero out those memory locations

	request path from user with info on how to input it
	get 280 char path from user
	append the file name for output to the path specified
	attempt to open/create the file
	check to see if the file is open or created
	if it's open
		while h is not small enough
			if this isnt the first iteration of the while loop
				h equals h divided by 2
			if this iteration is divisible evenly by 20
				if this is not the first iteration
					print a new line
				reprint the output formatting for ease of reading
			//if
			print the index of this iteration to the buffer string
			get the length of the string
			print the string to the file
			print spaces until at position 10 of the current line
			zero out buffer string

			print current h value to buffer string
			get the length of the string
			print the string to the file
			print spaces until at position 10 + 17 of the current line
			zero out buffer string

			print current approximate derivative to buffer string
			get the length of the string
			print the string to the file
			print spaces until at position 10 + 17 + 16 of the current line
			zero out buffer string
			print error string to file
			print a new line to the file
			update iteration index
		//while
		print the output destination to user in console
		wait for a keypress to exit
	//if
	else
		let the user know there is something wrong with the path they entered
		give a couple suggestions
		wait for keypress to exit
	//else
//main
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>


#define Exit_Condition 2 * pow(10.0,-12)
#define x  0.881373587019543
#define aprxForm(h) (cosh(x + h) - cosh(x))/h
#define error(h) abs(1-aprxForm(h))
#define OpLen 10
#define hLen 17
#define fLen 16
//#define ELen 
/***************************[MAIN]*****************************/
void main(){
	//local declarations
	int strln;
	int index = 0;
	long double h = 1;
	char *path = (char *)malloc(300 * sizeof(char));
	char *buffer = (char *)malloc(100 * sizeof(char));
	memset(path,0,300);
	memset(buffer,0,100);

	//requesting path from user
	printf("Please enter the path to the folder where you would like to store the output.\n");
	printf("Path must be less than 280 characters.\nFile Separators are '/'\nPlease leave off a terminating '/'\n\n");
	printf("EXAMPLE: C:/Users/User_Name/Documents\n");
	printf("\nPath:");
	//getting path from user
	scanf("%280s", path);
	//adding log file name to path
	strcat(path,"/Derivative_Log.txt");
	//attempting to create or open file
	FILE *fp = fopen(path , "w");
	//checking if file is open
	if(fp != NULL){
		//while h is not small enough
		while (h >= Exit_Condition){
			//if this is not the first iteration
			if(index != 0)
				//h equals h divided by 2
				h = h/2;
			//if this iteration index is divisible evenly by 20
			if(index % 20 == 0){
				//if this is not the first iteration
				if(index != 0)
					//add in a blank line
					fprintf(fp,"\n");
				//print or reprint the output formatting for easy reading
				fprintf(fp,"Op #:     h:               f'(x) aprx:     Error:\n");
			}//if

			//print the iteration index to buffer string
			sprintf(buffer,"%d",(index+1));
			//get the length of buffer string
			strln = strlen(buffer);
			//write buffer string to file
			fprintf(fp,buffer);
			//while the string in the file is not as long as desired
			while(strln < OpLen){
				//add a space
				fprintf(fp," ");
				//add one to the length of the string so we know when we have desired length
				strln++;
			}//while
			//zero out buffer string
			memset(buffer,0,100);

			//the following two blocks follow the same logic as the one directly preceding this comment
			sprintf(buffer,"%g",h);
			strln = strlen(buffer);
			fprintf(fp,buffer);
			while(strln < hLen){
				fprintf(fp," ");
				strln++;
			}//while
			memset(buffer,0,100);

			sprintf(buffer,"%g",aprxForm(h));
			strln = strlen(buffer);
			fprintf(fp,buffer);
			while(strln < fLen){
				fprintf(fp," ");
				strln++;
			}//while
			memset(buffer,0,100);

			//print the error
			fprintf(fp,"%g",error(h));
			//go to the next line
			fprintf(fp,"\n");
			//update the iteration index
			index++;
		}//while
		//Remind the user where the output log is
		printf("Output file: %s\nPress [ENTER] to exit.\n",path);
		//wait for keypress to exit
		getch();
	}//if
	else{
		//let the user know there is something wrong with the path they entered
		printf("File Failed to Open or be Created!\nCheck that the path entered is valid and you have read/write permission.\nPress [ENTER] to exit.\n");
		//wait for keypress to exit
		getch();
	}//else
}//main