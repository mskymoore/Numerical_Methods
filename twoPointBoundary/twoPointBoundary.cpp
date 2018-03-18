// twoPointBoundary.cpp : Defines the entry point for the console application.
//
/*
PROGRAMMER: M. Skylar Moore 0939795
COURSE: CSCI 3321
DATE: 7/18/2016
ASSIGNMENT: Assigment 4:Two point boundary value problem
ENVIRONMENT: C++ on Windows 10, compiled in Visual Studio 2015 Enterprise
FILES INCLUDED: Header.h, userConsoleInput.h, stdafx.h, twoPointBoundary.cpp
PURPOSE: to solve a nonlinear differential equation via the shooting method
INPUT: User choices on output location, and output to console.
PRECONDITIONS: none
OUTPUT: Log file of all steps at path specified by user, and same to console if the user requests it.
POSTCONDITIONS: Log file created
ALGORITHM:

Algorithm below is for the secant method in conjunction with the third runga kutta as the secant method is all that has really changeed.  All functionality in main and buffer builder function is well commented
if the reader is curious but is not relevant to this assignment and is solely for ease of use of the program.

secantMethod()
	declarations/ memory allocations
	fa gets boundary evaluation of first guess
	fb gets boundary evaluation of second guess
	integration of first guess to get x(1)
	integration of second guess to get x(1)
	print secant method table
	if |fa| > |fb|
		switch values of a and b, fa and fb
	print formatting
	build output strings for evaluations of first two guesses
	for (number of iterations desired not met)
		if |fa| > |fb|
			switch values of a and b, fa and fb
	delta gets b minus a divided by fb minus fa
	b gets a
	delta gets delta times fa
	if within desired accuracy
		print out the final result
	a gets a minus delta
	fa gets boudary evaluation of a
	build output string for this step
	print out put string
//secantMethod

*/
#include "stdafx.h"
#include "Header.h"
#include "userConsoleInput.h"

int main()
{
	int iterCheck = 0;
	FILE *fp;
	char *file;
	char *help;
	char c;
	//file = (char*)malloc(300 * sizeof(char));
	help = (char*)malloc(500 * sizeof(char));
	//file = "C:/Users/mskymoore/Desktop/twoPointBoundary_Log.txt";
	help = "Please enter the path to the folder where you would like to store the output.\n\nPath must be less than 290 characters.\n\nFile Separators are '/'\n\nPlease leave off a terminating '/'\n\nEXAMPLE: C:/Users/User_Name/Documents";
	do {
		if (iterCheck != 0) {
			do {
				printf("File failed to open!\nCheck that you have read/write privileges.\nPress [ENTER] to continue, [ESC] to quit.");
				c = getch();
				if (c == (char)0x1B)
					break;
				else if (c == (char)0xD)
					;
				else {
					system("cls");
					printf("\nINVALID ENTRY\n");
					Sleep(1000);
					system("cls");
				}
			} while (c != (char)0x1B && c != (char)0xD);
			if (c == (char)0x1B)
				break;
		}
		file = userConsoleInput(help);
		strcat(file, "/twoPointBoundary_Log.txt");
		fp = fopen(file, "w");
		if (iterCheck == 0)
			iterCheck++;
	} while (fp == NULL);
	if (fp != NULL) {
		secantMethod(fp, 1.0, 0.7, 20, 1.0 * pow(10, -14));
		ShellExecuteA(0, "open", file, 0, 0, SW_SHOW);
	}//ifB
	//getch();
    return 0;
}

