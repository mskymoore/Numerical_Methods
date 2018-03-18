/*
PROGRAMMER: M. Skylar Moore 0939795
COURSE: CSCI 3321
DATE: 6/21/2016
ASSIGNMENT: Assigment 2: Secant Method
ENVIRONMENT: C++ on Windows 7, compiled in visual studio 2010 professional
FILES INCLUDED: Zero.cpp, Zero_Log.txt, stdio.h, math.h, conio.h, string.h, stdlib.h, windows.h
PURPOSE: This program uses to secant method to demonstrate the capability of this algorithm to converge on a zero of a function
		 given two starting points.
INPUT: the function, the tolerance of difference from zero(epsilon in this case), path to output location
PRECONDITIONS: Up to date C++ compiler
OUTPUT: iteration number, value of x, value of f(x), value of x a zero was found at
POSTCONDITIONS: A .txt file named "Zero_Log.txt" is created with the output in the path specified by the user.
ALGORITHM: ****THE SECANT METHOD IS IN 'secantMethod' FUNCTION****
		   ****READ COMMENTS IF INTERESTED IN MAIN METHOD, OR BUFFER BUILDER, WHICH ARE USED MOSTLY FOR OUTPUT****
		   ****THE ALGORITHM CONTAINED HEREIN REFERS ONLY TO THE MATHEMATICAL OPERATIONS IN 'secantMethod'****
		BEGIN
		   FUNCTION bufferBuilder > for building output string
		   LOCAL DECLARATIONS
		   Assign fa and fb their values from evaluating f(x) #defined at top of file with a and b
		    if |f(a)| > |f(b)|
				exchange values of a and b
				exchange values of f(a) and f(b)
			//if
		   print output format information to file
		   build the strings for the first two lines
		   print the first two lines with the initial guesses to the file
		   For Loop [until the iteration reaches nmax, continue]
				if |f(a)| > |f(b)|
					exchange values of a and b
					exchange values of f(a) and f(b)
				//if
				delta equals [ b minus a ] divided by [ f(b) minus f(a) ]
				b equals a
				f(b) equals f(a)
				delta equals delta * f(a)
				if( f(a) is close enough to zero(smaller than 1*10^-9))
					print that the solution has been found to the console and the file
					return the pointer to the file that has the output log(implicit break from the for loop when returning)
				//if
				a equals a minus delta
				f(a) equals new value evaluated at new a
				build the string for this iteration
				print the string for this iteration to the file
		   //for
		   return the pointer to the file that has the output loog
		END
*/

#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#define f(x) (tanh(x) - 0.5)
#define OpLen 15
#define xLen 16


int main(){
	printf("%c",(char)0x7);
	/******************LOCAL DECLARATIONS*********************/
	FILE *secantMethod(FILE *fp, long double a, long double b, int nmax, long double epsilon);
	int charCount;
	FILE *fp;
	char c;
	char *tmp;
	char *path = (char *)malloc(300 * sizeof(char));

	/********DO THIS LOOP UNTIL USER CHOOSES TO EXIT*********/
	do {
		system("cls");//clear the console loop restarted, file path entered was not correct
		charCount = 0;//reset the character count
		memset(path,0,300);//zero out the path
		/*****************REQUESTING FILE PATH FOR "Zero_Log.txt"*****************/
		
		/***************DO THIS LOOP UNTIL THE USER PRESSES ENTER*******************/
		do{
			if(charCount != 0){//if this is not the first char, and the last character was not a backspace
				strncat(path,&c,1);//add the new character to the path string
			}//if
			charCount = strlen(path);//update the length of the string
			/*************PRINT HELPER STATEMENTS**************/
			printf("Please enter the path to the folder where you would like to store the output.\n\n");
			printf("Path must be less than 260 characters.\n\nFile Separators are '/'\n\n");
			printf("Please leave off a terminating '/'\n\nEXAMPLE: C:/Users/User_Name/Documents\n\nCharacter count: %d of 260\n\nPath: %s",charCount,path);
			/************grab the users keypress*************/
			c = getch();
			/****************if user enters backspace*********************/
			if(c == (char)0x8){
				tmp = path;//set tmp pointer to the first character in the path string
				//advance to the end of the string
				while(*tmp != NULL){
					tmp++;
				}//while
				tmp--;//move tmp pointer to last character
				*tmp = (char)0x00;//overwrite that character with NULL
				charCount = 0;//set char count to 0 to prepare for next iteration
			}//if
			/***************otherwise some other character was entered*****************/
			else{
				if(c != (char)0xD)//as long as that character is not ENTER
					charCount++;//update the character count
			}//else
			system("cls");//clear the console
		}while(c != (char)0xD);//do this loop while the character entered is not ENTER

	
		/*****************OPEN FILE FOR WRITING*******************/
		strcat(path,"/Zero_Log.txt");
		fp = fopen(path , "w");
		/********if file exists and was created or overwritten*************************/
		if(fp != NULL){
			//calling secant method
			fp = secantMethod(fp,-1,2,20,0.000000001);
			//tell the user where the log file is
			printf("\n\nLog File Path: %s\n\n",path);
			//open the log file for the user if they so desire
			printf("Press [ESC] to exit, or any other key to open\nthe log file with the default .txt editor.");
			c = getch();//get the key they entered
			if(c == (char)0x1B){//if they hit escape quit the program
				break;
				fclose(fp);
			}//if
			ShellExecute(0,0,path,0,0,SW_SHOW);
			//close the file 
			fclose(fp);
			//wait for user to enter a key to close the console
			getch();
		}//if
		/***************otherwise bad file path******************/
		else{
			//let the user know there is something wrong with the path they entered
			printf("%c%c%c",(char)0x7,(char)0x7,(char)0x7);
			printf("\n\nFile Failed to Open or be Created!\nCheck that the path entered is valid and you have read/write permission.\nPlease try again.\n\n");
			printf("\nPress [ENTER] to proceed or any other key to exit.\n");
			c = getch();//get the key thay entered
			if(c != (char)0xD)//if they hit escape quit the program
				break;
		}//else
	}while(fp == NULL);//while 
}//main



/****************SECANT METHOD**********************/
FILE *secantMethod(FILE *fp, long double a, long double b, int nmax, long double epsilon){

	/*************FUNCTION USED FOR BUILDING OUTPUT STRING**************/
	void bufferBuilder(char *buffer, int n, long double x, long double fx);

	/**************LOCAL DECLARATIONS***************/
	long double fa, fb, delta, tmp;
	int n = 2;
	char *buffer = (char *)malloc(100 * sizeof(char));
	memset(buffer,0,100);

	/****************ALGORITHM BEGIN****************/
	//getting initital values for f(x) at a and b
	fa = f(a);
	fb = f(b);
	//intitial check for location of root guesses
	if(fabs(fa) > fabs(fb)){
		tmp = a;
		a = b;
		b = tmp;
		tmp = fa;
		fa = fb;
		fb = tmp;
	}//if
	//print output format
	fprintf(fp,"Iteration:     x:              f(x):            \n");
	//printing to file
	bufferBuilder(buffer,0,a,fa);
	fprintf(fp,buffer);
	bufferBuilder(buffer,1,b,fb);
	fprintf(fp,buffer);
	//for loop to continue execution of algorithm until desired condition is met
	for(n; n <= nmax; n++){
		if(fabs(fa) > fabs(fb)){
			tmp = a;
			a = b;
			b = tmp;
			tmp = fa;
			fa = fb;
			fb = tmp;
		}//if
		delta = (b - a)/(fb - fa);
		b = a;
		fb = fa;
		delta = delta * fa;
		//desired condition
		if(fabs(fa) < epsilon){
			printf("\nZero Found:\n\nf( x = %g ) < %g",a,epsilon);
			fprintf(fp,"\nZero Found:\nf( x = %g ) < %g",a,epsilon);
			return fp;
		}//if
		a = a - delta;
		fa = f(a);
		//printing to file
		bufferBuilder(buffer,n,a,fa);
		fprintf(fp,buffer);
	}//for
	return fp;
}//secantMethod



/**************BUFFER BUILDER***************/
void bufferBuilder(char *buffer, int n, long double x, long double fx){
	/**************LOCAL DELCARATIONS***************/
	int strln;
	char *tmp = (char *)malloc(100 * sizeof(char));//declare the string for tmp to write to buffer
	memset(buffer,0,100);//zero that place in memory

	/*************BUILDING OUTPUT STRING IN 'buffer' TO WRITE TO FILE***************/
	sprintf(tmp,"%d",n);//print the index of iteration to the temp string
	strln = strlen(tmp);//get the length of that string
	while(strln < OpLen){//while the string is not the desired length
		strcat(tmp," ");//add a space
		strln++;//update length counter
	}//while
	strcat(buffer,tmp);//concatenate the temp string on the buffer string

	memset(tmp,0,100);//zero out the temp string
	sprintf(tmp,"%g",x);//print the value of x to the temp string
	strln = strlen(tmp);//get the length of that string
	while(strln < xLen){//while the string is not the desired length
		strcat(tmp," ");//add a space
		strln++;//update length counter
	}//while
	strcat(buffer,tmp);//concatenate the temp string on the buffer string

	memset(tmp,0,100);//zero out the temp string
	sprintf(tmp,"%g",fx);//print the value of f(x) to the temp string
	strcat(buffer,tmp);//concatenate the temp string on the buffer string
	strcat(buffer,"\n");//go to the next line
	free(tmp);//free the memory reserved for tmp
}//bufferBuilder