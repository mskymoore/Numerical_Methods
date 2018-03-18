/*
PROGRAMMER: M. Skylar Moore 0939795
COURSE: CSCI 3321
DATE: 7/7/2016
ASSIGNMENT: Assigment 3: 3rd order Runge Kutta
ENVIRONMENT: C++ on Windows 10, compiled in Visual Studio 2015 Enterprise
FILES INCLUDED: thirdRungaKutta.cpp
PURPOSE: to complete numerical integration on the specified function with a 3rd order Runge Kutta Method
INPUT: User choices on output location, and output to console.
PRECONDITIONS: none
OUTPUT: Log file of all steps at path specified by user, and same to console if the user requests it.
POSTCONDITIONS: Log file created
ALGORITHM:

Algorithm below is for the Runge Kutta method named "thirdRungeKutta" all functionality in main and buffer builder function is well commented
if the reader is curious but is not relevant to this assignment and is solely for ease of use of the program.  

thirdRungeKutta()
	declare local variables
	if user requested console output
		print step size to console
		print initial values of x and t to console
	print step size to log file
	print initial values of x and t to log file
	while not all iterations specified completed for this step size
		add 1 to the number of iterations completed
		calculate derivative at this point
		calculate k1 first formula for RK method
		calculate k2 second formula for RK method
		calculate k3 third formula for RK method, assign result to x
		add step size to t, assign result to t
		call buffer builder to build output string for this iteration
		if user requested console output
			print output string to console
		print output string to log file
	print new line
	return the integration result
//thirdRungeKutta

*/
/***********FILES INCLUDED************/
#include "stdafx.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "conio.h"
#include "Windows.h"

/************GLOBAL DECLARATIONS************/
static const double c0 = 4.0;
static const double c1 = 12.0;
static const double c2 = -15.0;
static const double a1 = 1.0/4.0;
static const double a2 = 1.0/6.0;
static const double b10 = 1.0/4.0;
static const double b20 = 19.0/90.0;
static const double b21 = -2.0/45.0;


static const double stepSize[] = { 0.1, 0.05, 0.025, 0.0125, 0.00625, 0.003125, 0.0015625 };
static const int steps[] =		 {  20,   40,    80,    160,     320,      640,      1280 };

/**************FUNCTION PROTOTYPES***************/
double XofT(double t, double x);
double thirdRungeKutta(double(*fxt)(double, double), double x, double t, double h, double bound, FILE *outFile, int log);
void bufferBuilder(char *buffer, int n, double x, double t);

/*******~~~*******~~~MAIN~~~*******~~~********/
int main(){
	/***********LOCAL DECLARATIONS***********/
	double(*fxt) (double, double) = XofT;//pointer to the function XofT
	printf("%c", (char)0x7);//beep
	FILE *fp;
	int log = 2;
	char c;
	char *tmp;
	char *path = (char *)malloc(300 * sizeof(char));

	/*************CODE***************/

	/***************do while user hasn't entered a VALID file path*****************/
	do {
		system("cls");
		int charCount = 0;
		memset(path, 0, 300);

		/***************do while user hasn't completed entering file path for log file*****************/
		do {
			if (charCount != 0) {//if this is not the first char, and the last character was not a backspace
				strncat(path, &c, 1);//add the new character to the path string
			}//if
			charCount = strlen(path);//update the length of the string

			/*************PRINT HELPER STATEMENTS**************/
			printf("Please enter the path to the folder where you would like to store the output.\n\n");
			printf("Path must be less than 260 characters.\n\nFile Separators are '/'\n\n");
			printf("Please leave off a terminating '/'\n\nEXAMPLE: C:/Users/User_Name/Documents\n\nCharacter count: %d of 260\n\nPath: %s", charCount, path);
			/************grab the users keypress*************/
			c = getch();
			/****************if user enters backspace*********************/
			if (c == (char)0x8) {
				tmp = path;//set tmp pointer to the first character in the path string
						   //advance to the end of the string
				while (*tmp != NULL) {
					tmp++;
				}//while
				tmp--;//move tmp pointer to last character
				*tmp = (char)0x00;//overwrite that character with NULL
				charCount = 0;//set char count to 0 to prepare for next iteration
			}//if
			 /***************otherwise some other character was entered*****************/
			else {
				if (c != (char)0xD)//as long as that character is not ENTER
					charCount++;//update the character count
			}//else
			system("cls");//clear the console
		} while (c != (char)0xD);//do this loop while the character entered is not ENTER(0xD)
		strcat(path, "/Runge_Kutta_Log.txt");//concatenate the name of the log file onto the path specified by the user
		fp = fopen(path, "w");//open or create the file at the path the user entered

		if (fp != NULL) {//if the file path entered is valid

			/***************while the user hasn't entered a valid option*****************/
			while (log != 0 && log != 1) {// 
				system("cls");//clear the console
				printf("\nShow output in console?\ny - yes / n - no\n::");//ask user if they would like console output
				c = getch();//accept answer
				if (c == (char)0x59 || c == (char)0x79)//if lowercase or uppercase Y, then yes
					log = 1;//if
				else if (c == (char)0x4E || c == (char)0x6E)//if lowercase or uppercase N, then no
					log = 0;//else if
			}//while

			 /***************for all the values of step size specified*****************/
			for (int i = 0; i < sizeof(stepSize) / sizeof(double); i++) {//
				thirdRungeKutta(fxt, 1.0, 0.0, stepSize[i], steps[i], fp, log);//call the runge kutta method
				if (log == 1) {//if the user requested console output
					if (i < ((sizeof(stepSize) / sizeof(double)) - 1)) {//if this is not the last and largest step size
						printf("Press [ESC] to exit, or any other key to continue");//let the user know they have the option to stop execution after each step size is completed
						c = getch();//accept answer
						if (c == (char)0x1B) {//if user pressed escape key
							break;//break from the loop
						}//if
					}//if
				}//if
			}//for

			if (log == 0)//if user requested no console output
				system("cls");//clear the console
			printf("\nLog file: Runge_Kutta_Log.txt\nPath: %s", path);//print the log file name and path to the log file
			printf("\n\nPress [ESC] to exit, or any other key to open log file with default text editor.");//give user the option to go ahead and open the log file
			c = getch();//accept answer
			fclose(fp);//close the file, writing is complete
			if (c != (char)0x1B) {//if the user pressed anything other than escape
				ShellExecuteA(0, "open", path, 0, 0, SW_SHOW);//open the log file with the default text editor
			}//if
		}//if


		else {//otherwise the file path entered is invalid, notify user, give option to stop execution
			printf("\n\nFile Failed to Open or be Created!\nCheck that the path entered is valid and you have read/write permission.\nPlease try again.\n\n");
			printf("\nPress [ENTER] to proceed or any other key to exit.\n");
			c = getch();//accept answer
			if (c != (char)0xD)//if the user pressed anything other than enter
				break;//break from the loop
		}//else
	} while (fp == NULL);//do while
    return 0;
}//Main


/************Derivative Function of x and t************/
double XofT(double t, double x) {
	double derivative = 1 / (cos(10 * t) + x);
	return derivative;
}//XofT

double thirdRungeKutta(double (*fxt)(double t,double x), double x, double t, double h, double bound, FILE *outFile, int log) {
	/*********LOCAL DECLARATIONS*********/
	double k1;
	double k2;
	double f0;
	char *buffer = (char *)malloc(100 * sizeof(char));//buffer string for output
	int iterNum = 0;//iterations completed


	/**********CODE**********/
	if (log == 1) {//if the user requested output to console
		printf("\n**************************************************\n**************************************************\n**************************************************\n\n");
		printf("\nStep Size -> h:%.8f\n\nIteration:\tX:\t\t\tT:\n%d\t\t%.16f\t%.8f\n", h, iterNum, x, t);
	}
	fprintf(outFile,"\n**************************************************\n**************************************************\n**************************************************\n\n");
	fprintf(outFile,"\nStep Size -> h:%.8f\n\nIteration:\t\tX:\t\t\t\t\t\tT:\n%d\t\t\t\t%.16f\t\t%.8f\n", h, iterNum, x, t);
	while (iterNum < bound) {//while we haven't completed all iterations specified
		iterNum++;//add 1 to the number of iterations completed
		f0 = fxt(t, x);//evaluate the derivative at the current point
		k1 = x + h*(b10*f0);
		k2 = x + h*(b20*f0 + b21*fxt(t + a1*h, k1));
		x = x + h*(c0*f0 + c1*fxt(t + a1*h, k1) + c2*fxt(t + a2*h, k2));//update x
		t = t + h;//update t
		
		bufferBuilder(buffer, iterNum, x, t);//build the output string
		if (log == 1)//if the user requested output to console
			printf("%s", buffer);//print the output string for this iteration
		fprintf(outFile, buffer);//print to file
	}
	printf("\n");//new line
	return x;//return the integration result
}//thirdRungeKutta

void bufferBuilder(char *buffer, int n, double x, double t) {
	/**************LOCAL DELCARATIONS***************/
	int strln;
	int OpLen = 16;//length for first segment of string
	int xLen = 24;//length for second segment of string
	char *tmp = (char *)malloc(100 * sizeof(char));//declare the string for tmp to write to buffer
	memset(buffer, 0, 100);//zero that place in memory

	/*************BUILDING OUTPUT STRING IN 'buffer' TO WRITE TO FILE***************/
	sprintf(tmp, "%d", n);//print the index of iteration to the temp string
	strln = strlen(tmp);//get the length of that string
	while (strln < OpLen) {//while the string is not the desired length
		strcat(tmp, " ");//add a space
		strln++;//update length counter
	}//while
	strcat(buffer, tmp);//concatenate the temp string on the buffer string

	memset(tmp, 0, 100);//zero out the temp string
	sprintf(tmp, "%.16f", x);//print the value of x to the temp string
	strln = strlen(tmp);//get the length of that string
	while (strln < xLen) {//while the string is not the desired length
		strcat(tmp, " ");//add a space
		strln++;//update length counter
	}//while
	strcat(buffer, tmp);//concatenate the temp string on the buffer string

	memset(tmp, 0, 100);//zero out the temp string
	sprintf(tmp, "%.8f", t);//print the value of f(x) to the temp string
	strcat(buffer, tmp);//concatenate the temp string on the buffer string
	strcat(buffer, "\n");//go to the next line
	free(tmp);//free the memory reserved for tmp
}//bufferBuilder