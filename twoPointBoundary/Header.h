#pragma once
#include "stdafx.h"


static const double c0 = 4.0;
static const double c1 = 12.0;
static const double c2 = -15.0;
static const double a1 = 1.0 / 4.0;
static const double a2 = 1.0 / 6.0;
static const double b10 = 1.0 / 4.0;
static const double b20 = 19.0 / 90.0;
static const double b21 = -2.0 / 45.0;


double g(double x, double(*RK)(double(*fxt)(double t, double x), double x, double t, double h, double bound, FILE *outfile, int log), FILE * fp, double(*fxt)(double t, double x));
double fxt(double t, double x);
double thirdRungeKutta(double(*fxt)(double t, double x), double x, double t, double h, double bound, FILE *outFile, int log);
void bufferBuilder(char *buffer, int n, double x, double t);
void buffBuilder(char *buffer, int n, double x0, double x1, double t);
FILE *secantMethod(FILE *fp, double a, double b, int nmax, double epsilon);

static double(*XofT) (double, double) = fxt;

double g(double x, FILE * fp, double(*fxt)(double t, double x)) {
	double result = x + thirdRungeKutta(fxt, x, 0.0, 0.025, 40, fp, 0) - 3.0;
	return result;
}//g

double fxt(double t, double x) {
	double result = 0.09 * x * x + x + cos(10.0 * t);
	return result;
}

double thirdRungeKutta(double(*fxt)(double t, double x), double x, double t, double h, double bound, FILE *outFile, int log) {
	/*********LOCAL DECLARATIONS*********/
	double k1;
	double k2;
	double f0;
	char *buffer = (char *)malloc(100 * sizeof(char));//buffer string for output
	int iterNum = 0;//iterations completed


					/**********CODE**********/
	if (log == 1) {//if the user requested output to console
		//printf("\n**************************************************\n**************************************************\n**************************************************\n\n");
		//printf("\nStep Size -> h:%.8f\n\nIteration:\tX:\t\t\tT:\n%d\t\t%.16f\t%.8f\n", h, iterNum, x, t);
		fprintf(outFile, "\n**************************************************\n**************************************************\n**************************************************\n\n");
		fprintf(outFile, "\nStep Size -> h:%.8f\n\nIteration:\t\tX:\t\t\t\t\t\tT:\n%d\t\t\t\t%.16f\t\t%.8f\n", h, iterNum, x, t);
	}
	
	while (iterNum < bound) {//while we haven't completed all iterations specified
		iterNum++;//add 1 to the number of iterations completed
		f0 = fxt(t, x);//evaluate the derivative at the current point
		k1 = x + h*(b10*f0);
		k2 = x + h*(b20*f0 + b21*fxt(t + a1*h, k1));
		x = x + h*(c0*f0 + c1*fxt(t + a1*h, k1) + c2*fxt(t + a2*h, k2));//update x
		t = t + h;//update t

		bufferBuilder(buffer, iterNum, x, t);//build the output string
		if (log == 1) {//if the user requested output to console
			//printf("%s", buffer);//print the output string for this iteration
			fprintf(outFile, buffer);//print to file
		}
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

FILE *secantMethod(FILE *fp, double a, double b, int nmax, double epsilon) {

	/*************FUNCTION USED FOR BUILDING OUTPUT STRING**************/
	//void bufferBuilder(char *buffer, int n, long double x, long double fx);

	
	/**************LOCAL DECLARATIONS***************/
	double fa, fb, delta, tmp, rKa, rKb;
	int n = 2;
	char *buffer = (char *)malloc(100 * sizeof(char));
	memset(buffer, 0, 100);

	/****************ALGORITHM BEGIN****************/
	//getting initital values for f(x) at a and b
	fa = g(a,fp,XofT);
	fb = g(b,fp,XofT);
	rKa = thirdRungeKutta(XofT, a, 0.0, 0.025, 40, fp, 0);
	rKb = thirdRungeKutta(XofT, b, 0.0, 0.025, 40, fp, 0);

	fprintf(fp, "Secant Method Table:\n\n");

	//intitial check for location of root guesses
	if (fabs(fa) > fabs(fb)) {
		tmp = a;
		a = b;
		b = tmp;
		tmp = fa;
		fa = fb;
		fb = tmp;
	}//if
	 //print output format
	fprintf(fp, "\nIteration:      x(0):                   x(1)                    Error:\n");
	//printing to file
	buffBuilder(buffer, 0, a, rKa, fa);
	fprintf(fp, buffer);
	buffBuilder(buffer, 1, b, rKb, fb);
	fprintf(fp, buffer);
	//for loop to continue execution of algorithm until desired condition is met
	for (n; n <= nmax; n++) {
		if (fabs(fa) > fabs(fb)) {
			tmp = a;
			a = b;
			b = tmp;
			tmp = fa;
			fa = fb;
			fb = tmp;
		}//if
		delta = (b - a) / (fb - fa);
		b = a;
		fb = fa;
		delta = delta * fa;
		//desired condition
		if (fabs(fa) < epsilon) {
			//printf("\nZero Found:\n\nf( x = %g ) < %g", a, epsilon);
			fprintf(fp, "\nZero Found:\nf( x = %g ) < %g", a, epsilon);
			fprintf(fp, "\n\n\nRunge Kutta Integration Table:\n");
			rKa = thirdRungeKutta(XofT, a, 0.0, 0.025, 40, fp, 1);
			fprintf(fp, "\nFinal Result:\nX(0) = %1.16f\t X(1) = %1.16f\t Error: %1.16f", a, rKa, fabs(g(a, fp, XofT)));
			fprintf(fp, "\n\nx(0) + x(1) = %1.16f", (a + rKa));
			return fp;
		}//if
		a = a - delta;
		fa = g(a, fp, XofT);
		//printing to file
		buffBuilder(buffer, n, a, thirdRungeKutta(XofT,a,0.0,0.025,40,fp,0), fa);
		fprintf(fp, buffer);
	}//for
	return fp;
}//secantMethod


void buffBuilder(char *buffer, int n, double x0, double x1, double t) {
	/**************LOCAL DELCARATIONS***************/
	int strln;
	int OpLen = 16;//length for first segment of string
	int xLen = 24;//length for second segment of string
	int yLen = 24;
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
	sprintf(tmp, "%.16f", x0);//print the value of x to the temp string
	strln = strlen(tmp);//get the length of that string
	while (strln < xLen) {//while the string is not the desired length
		strcat(tmp, " ");//add a space
		strln++;//update length counter
	}//while
	strcat(buffer, tmp);//concatenate the temp string on the buffer string

	memset(tmp, 0, 100);//zero out the temp string
	sprintf(tmp, "%.16f", x1);//print the value of x to the temp string
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