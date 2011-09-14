
#include "mathlib.h"


void checkFront(double &x ,double step)
{
    if (fmod(x,step) > (step / 2))
        x = x - fmod(x,step) + step ;
    else
        x = x - fmod(x,step) ;
}

void sortPointsList(QList<AnalysePoint> &pList)
{
    AnalysePoint pTemp ;
    for (int i=0 ; i<pList.count()-1 ; i++)
    {
        for (int j=i+1 ; j<pList.count() ; j++ )
        {
            if (pList.at(i).x() > pList.at(j).x())
            {
                pTemp = pList.at(i) ;
                pList[i]= pList.at(j) ;
                pList[j]= pTemp;
            }
        }
    }

}


double BinarySearch(double Fnum,double Snum,double E,Function f)
{
    if (fabs(Snum-Fnum) < E)
    {
        return Snum;
    }
    else
    {
        double c = (Fnum+Snum)/2 ;
        double Fc = f.F(c);
        if (f.F(Fnum)*Fc <0)
        {
            if (c < Fnum)
            {
                return BinarySearch(c,Fnum,E,f);
            }
            else
            {
                return BinarySearch(Fnum,c,E,f);
            }
        }
        else
        {
            if (c < Snum)
            {
                return BinarySearch(c,Snum,E,f);
            }
            else
            {
                return BinarySearch(Snum,c,E,f);
            }
        }
    }
}

double WrongSituation(double Fnum,double Snum,double E,Function f)
{
    double c = Snum - ((f.F(Snum)*(Fnum-Snum))/(f.F(Fnum)-f.F(Snum)));
    double Fc = f.F(c);
    if (fabs(Fc) < E)
    {
        return c;
    }
    else
    {
        if (f.F(Fnum)*Fc <0)
        {
            if (c < Fnum)
            {
                return WrongSituation(c,Fnum,E,f);
            }
            else
            {
                return WrongSituation(Fnum,c,E,f);
            }
        }
        else if (f.F(Snum)*Fc <0)
        {
            if (c < Snum)
            {
                return WrongSituation(c,Snum,E,f);
            }
            else
            {
                return WrongSituation(Snum,c,E,f);
            }
        }
    }
}

                /*	Maximization value function definition	*/

void max_val(Function F,double &xl,double &xu,double prec)
        {

        // xl , xu : are parties of The domain , prec : precision .

        double x1 = xl + 0.618 * ( xu - xl );
        double x2 = xu - 0.618 * ( xu - xl );

        if ( fabs(xu-xl) > prec )	// (fabs) is absolute function for double
        {	if (F.F(x1) > F.F(x2))
                        max_val(F,x2,xu,prec);
                else
                        max_val(F,xl,x1,prec);
        }

        }

                /*	sqrt_n function definition	*/

double root_n(double x,double n,double x1,double prec)
{
        double tem_x1,root ;
        //	tem_x1 : The prev beginning value .
        //	root : The finishing value .

        tem_x1 = root_n_help(x,n,x1,n) ;
        root = ( ((n-1)/n) * ( tem_x1 + x /((n-1)*pow(tem_x1,int(n-1))) ) ) ;

        while (fabs(root - tem_x1) > prec)	// prec is a constant precision .
                {
                        tem_x1 = root ;
                        root = ( ((n-1)/n) * ( tem_x1 + x /((n-1)*pow(tem_x1,int(n-1))) ) ) ;
                }

        return root ;
}
                /*	Lagrange methode function definition	*/

Function lagrangeApprox(double* X,double* Y,int n)
{
        Function f_lag ;
        func temp;
        f_lag.insertfunc(temp);	// insert zero value to The finction as a begining value .
        Function temp_func ;
        for (int i=0 ; i<n ; i++)
        {
            temp.con = Y[i] ;
            temp_func.insertfunc(temp);
            f_lag += L_n_x(i,X,n) * temp_func ;	// lagrange low .
            temp_func.clear_Function();
        }

        return f_lag ;
}

                /*	Front defferences methode function definition	*/

Function front_deff(int n,double h,double* X , double* Y)
{
        double* d_y0=new double[n];		// vector of delta_i y0 , exemple : d_y0[2] = delta2 y0 .
        delta_y0(n,Y,d_y0);

        func func_temp;		// temporary func .
       // func_temp.con=0;
        //func_temp.pow=0;
        //func_temp.id="con";


        Function poly(func_temp) ;	//	insert zero value to The function as a begining value .

        func_temp.con=1;	//	|
        func_temp.id="x";	//	|	func = x .
        func_temp.pow=1;	//	|


        Function p1(func_temp);	//	temporary Function .
        Function p2;			//	temproray Function .

        Function poly_temp;		// temporary Function to use it in the loop as accumulator .
        func_temp.pow = 0 ;
        func_temp.id="con";

        for (int i=0 ; i<n ; i++)
        {
                func_temp.con = d_y0[i]/(factorial(i)*pow(h,i)) ;
                poly_temp.insertfunc(func_temp);

                for (int j=0 ; j<i ; j++)
                {
                        func_temp.con=X[j];
                        p2.insertfunc(func_temp);
                        poly_temp *= (p1 - p2) ;
                        p2.clear_Function();
                }

                poly += poly_temp ;
                poly_temp.clear_Function(); // set 0 in new Root
        }

        return poly ;
}


Function L_n_x(int i,double* X,int n)
{
        Function Li_x ;	// Li(x) ;
        func temp ;		// temporary func to fill the Classes (the Functions) .
        temp.con = 1 ;
        Li_x.insertfunc(temp) ;	// insert 1 as a begining value .
        temp.id ="x" ;		// determine temp as a polynomial .
        temp.pow = 1 ;
        Function x(temp);

        Function con ;	// temporary function
        Function denom ;	// temporary function

        temp.pow = 0 ;
        temp.id = "con" ;
        for (int j=0 ; j<n ; j++)
        {
                if (j!=i)
                {
                        temp.con = X[j] ;
                        con.insertfunc(temp) ;
                        temp.con =1 /( X[i] - X[j] ) ;
                        denom.insertfunc(temp) ;
                        Li_x *= (x - con)*denom ;
                }
                con.clear_Function();
                denom.clear_Function();
        }

        return Li_x ;
}

void delta_y0(int points,double* Y,double* delta_y0)
        {
                for (int i=0 ; i<points ;i++ )
                        delta_y0[i] = delta(i,0,Y);
        }

long int factorial(int n )
{
        long int f=1;
        for (int i=1; i <= n ;i++ )
                {
                        f *= i;
                }
        return f ;
}

double delta(int n , int y ,const double* Y)
        {
                if (n >= 1)
                        return delta(n-1,y+1,Y)-delta(n-1,y,Y);
                else
                        return Y[y];
        }
void convertVectorToList(QList<AnalysePoint> pList,double* &X,double* &Y)
{
        X = new double [pList.count()] ;
        Y = new double [pList.count()] ;
        for (int i=0 ; i<pList.count() ; i++)
        {
            X[i] = pList.at(i).x() ;
            Y[i] = pList.at(i).y() ;
        }
}

double root_n_help(double x,double n,double x1,int n1)
        {
                if ( n1 > 1 )
                        {
                        double xi = root_n_help(x,n,x1,n1-1) ;	//	The prev beginning value .
                        return (((n-1)/n) * ( xi + x/((n-1)*pow(xi,n-1)) )) ;
                        }
                else
                        return x1 ;
        }
