
#ifndef MATH_LIB
#define MATH_LIB

#include <cmath>
#include "function.h"
#include "analysepoint.h"
#include <QList>

// root searching

double BinarySearch(double Fnum,double Snum,double E,Function f);
double WrongSituation(double Fnum,double Snum,double E,Function f);

// optimaization

/* max_val */
//	F : the function , xl , xu : are parties of The domain , prec : precision .
//	function job : Find the domain containing the max value , and return it in xl and lu .
void max_val(Function F,double &xl,double &xu,double prec);

/* sqrt_n */
//	x : , n : the grade of root , x1 : the begining value .
//	function job : Find the n_root of x value in 0.000001 precision .
double root_n(double x,double n,double x1,double prec);

/* lagrange */
//	X : vector of x's of Points , Y : vector of y's of Points , n : number of Points .
//	function job : Approximation the function and return it using lagrange methode .
Function lagrangeApprox(double* X,double* Y,int n);

/* front_deff */
//	n : number of Points , h : The constant step , X : vector of x's of Points , Y : vector of y's of Points .
//	function job : Approximation the function and return it using Front defferences methode .
Function front_deff(int n,double h,double* X , double* Y);


void checkFront(double &x ,double step);

void convertVectorToList(QList<AnalysePoint> pList,double* &X,double* &Y);

void sortPointsList(QList<AnalysePoint> &pList);

double root_n_help(double x,double n,double x1,int n1);
Function L_n_x(int i,double* X,int n);
double delta(int n , int y ,const double* Y);
long int factorial(int n );
void delta_y0(int points,double* Y,double* delta_y0);

#endif
