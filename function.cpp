#include "function.h"
QHash<QString,double> myDict ;
bool twooperat  = false ; //for print




void Function::init()
{
    myDict.insert("pi",3.142857143);
}

Function::Function()
{
    this->Root = NULL ;
}

Function::Function(const double val)
{
    func *myelem = new func;
    myelem->con=val;

    this->Root = new poly ;
    this->Root->operat = NULL ;
    this->Root->L = this->Root->R = NULL ;
    this->Root->elem = myelem ;
}

Function::Function(func myfunc)
{
    func *myelem =NULL ;
    copy_func(&myfunc,myelem); //for copy the func becaue we need it

    this->Root = new poly ;
    this->Root->operat = NULL ;
    this->Root->L = this->Root->R = NULL ;
    this->Root->elem = myelem ;
}

Function::Function(QString str)
{
    if (str!= "")
       AddToTree(this->Root,str);
    else
        this->Root = NULL ;
}

Function::Function(poly* otherpoly)
 {
    this->Root = otherpoly ;

 }

Function::Function(const Function& T)
{
    copy(T,*this);
}

void Function::delete_func(func* &f)
{
    if (f != NULL)
    {
        Help_Delete(f->expen);
        delete f;
        f=NULL;
    }

}

void Function::Help_Delete(poly* &Exp)
{
    if (Exp != NULL)
    {
        delete_func(Exp->elem);
        Help_Delete(Exp->L);
        Help_Delete(Exp->R);
        delete Exp;
        Exp = NULL;
    }
}

void Function::Delete(Function& Exp)
{
    Function *H_Exp = &Exp;
    Help_Delete(H_Exp->Root);
}


Function::~Function()
{
    Delete(*this);
    this->Root = NULL ;
}

void Function::copy_func(const func* From,func* &To)
{   //copy sin & cos & exp
    if (From != NULL)
    {
        if (To == NULL)
        {
            To = new func;
            To->con = From->con;
            To->id = From->id;
            To->pow = From->pow;
            To->expen = NULL;
        }
        Help_copy(From->expen,To->expen); //copy inside sin or cos or exp
    }
}

void Function::Help_copy(const poly* Exp,poly* &new_Exp)
{   //copy operator & set Null
    if (Exp != NULL)
    {
        if (new_Exp == NULL)
        {
            new_Exp = new poly;
            new_Exp->operat = Exp->operat;
            new_Exp->L = NULL;
            new_Exp->R = NULL;
            new_Exp->elem = NULL;
            copy_func(Exp->elem,new_Exp->elem);
        }
        // all tree
        Help_copy(Exp->L,new_Exp->L);
        Help_copy(Exp->R,new_Exp->R);
    }
}

void Function::copy(const Function& Exp,Function &To)
{
    To.Root =NULL;
    Help_copy(Exp.Root,To.Root);
}

QString ToString(double value)
{
    QString res_str("%1");
    res_str = res_str.arg(value);
    return res_str;
}

poly* Function::Determine(QString str)
{
    bool pow =false;
    poly* res =new poly ;
    res->R=NULL;  res->L=NULL;    res->operat=NULL;
    res->elem = new func ;
    res->elem->id= "con";
    res->elem->con=1;
    res->elem->pow=0;
    res->elem->expen=NULL;
    QString tempnum ="";
    QString temp = "" ;
    QString tempchar ="" ;
    bool editcon =false ;
    for (int j =0;j<str.length();j++)
    {
        while ( str[j].isDigit()||(str[j]=='.')||(str[j]=='-')||(str[j]=='+') )
        {
            editcon = true ;
            tempnum+=str[j];
            j++;
        }
        if (editcon)
        {
            editcon = false ;
            if (tempnum=="-")
                    res->elem->con*=-1 ;
            else if (tempnum!="+") //for +x
                if (pow)
                    res->elem->pow=tempnum.toDouble() ;
                else
                    res->elem->con=tempnum.toDouble() ;
            pow = false ;
            tempnum="";
        }
        if (str[j]=='^')
            pow=true;
        else if (j<str.length())
        {
            tempchar = "" ;
            if (str[j]=='(') //for sin (....)  or  x^2 ;
            {
                j++; //for ( ;
                while (j!=str.length()-1)
                {
                    temp+=str[j];
                    j++;
                }
                AddToTree(res->elem->expen,temp);
            } // for processing the poly internal function ex.sin(...)

            while ( ((str[j]!='(')&&(!str[j].isDigit())&&(str[j]!='*')&&(str[j]!='/')&&(str[j]!='-')&&(str[j]!='+')&&(str[j]!='.')&&(str[j]!='^'))&&((j<str.length()) ) ) //for const varible
            {
                tempchar +=str[j];
                j++ ;
            }
            if ( (tempchar=="sin")||(tempchar=="cos")||(tempchar=="exp")||(tempchar=="x")||(tempchar=="log") )
            {
                res->elem->id = tempchar ;
                res->elem->pow = 1 ;
                j-- ;
            }//for if tempchar is function know
            else  if (tempchar!="")                //function const varible ;
            {
                double const_num =1; //function const varible ;
                typedef QHash<QString,double>::const_iterator iter;
                iter myiter ;

                myiter = myDict.find(tempchar);
                if (myiter!=myDict.end())
                    const_num=myiter.value();
//                if (tempchar=="pi")
  //                  const_num = 3.14;
                if (pow)
                    if (res->elem->pow!=0)
                        res->elem->pow*=const_num ;
                    else
                        res->elem->pow=const_num ;
                else
                    res->elem->con*=const_num ;
                pow = false ;
                j-- ;
            }
        }
     } // finish main for

    return res ;
 } //finish all function


void Function::AddToTree( poly *&T ,QString str) // return poly* >>> root tree >> expen .
   {
       QStack <QChar> s_op ;
       QStack <poly*> s_func;
       bool internal=false ,internal2  = false ;
       QString temp ="";
       poly* T2 = NULL ;
       for (int i=0 ; i<str.length();i++)
       {
           if ((str[i]=='+')||(str[i]=='-'))
           {
               if (internal) //for operat internal funtion
                   temp+=str[i];
               else
               {
                   if( ((temp=="")&&(i==0))||(temp=="")&&(str[i-1]=='(') )
                       temp+=str[i];
                   else if (str[i-1]=='^') // for operat after ^
                       temp+=str[i];
                   else
                   {
                          if (temp!="") //for operat after )
                           {
                               s_func.push_front(Determine(temp));
                               temp="";
                           }
                          while ( (!s_op.empty())&&(s_op.front()!='(') )
                          {
                               T2 = new poly ;
                               T2->operat = s_op.front();   s_op.pop_front();
                               T2->R = s_func.front();      s_func.pop_front();
                               T2->L = s_func.front();      s_func.pop_front();
                               T2->elem=NULL ;
                               s_func.push_front(T2);
                               T=T2 ; T2 = NULL ;
                          }
                          if (str[i]=='-')
                              temp+= str[i] ;
                          s_op.push_front('+');
                          int myt = 0 ;
                      }
                   }
           } // finsh processing the + and -
           else if ( (str[i]=='*')||(str[i]=='/'))  // processing the * and /
           {
               if (internal)
                   temp+=str[i];
               else
               {
                   if (temp!="")
                   {
                       s_func.push_front(Determine(temp));
                       temp="";
                   }
                   if ( (!s_op.empty())&&( (s_op.front()=='*')||(s_op.front()=='/') ) )
                   {
                       T2 = new poly ;
                       T2->operat = s_op.front();   s_op.pop_front();
                       T2->R = s_func.front();      s_func.pop_front();
                       T2->L = s_func.front();      s_func.pop_front();
                       T2->elem=NULL ;
                       s_func.push_front(T2);
                       T=T2 ; T2 = NULL ;
                   }
                   s_op.push_front(str[i]);
               }
           }
           else if (str[i]=='(') //processing the ( and internal boolean ;
           {
               if (temp!="")
               {
                   if (!internal)
                       internal = true;
                   else
                       internal2 = true ;
                   temp+=str[i];
               }
               else
               {
                   s_op.push_front(str[i]);
               }
           }
           else if (str[i]==')')
           {
               if (internal)
               {
                   temp+=str[i];
                   if (internal2)
                       internal2 = false ;
                   else
                       internal = false ;
               }
               else
               {
                   s_func.push_front(Determine(temp));
                   temp = "";
                   while (s_op.front()!='(') // note:
                   {
                       T2 = new poly ;
                       T2->operat = s_op.front();   s_op.pop_front();
                       T2->R = s_func.front();      s_func.pop_front();
                       T2->L = s_func.front();      s_func.pop_front();
                       T2->elem=NULL ;
                       s_func.push_front(T2);
                       T=T2 ; T2 = NULL ;
                   }
                   s_op.pop_front(); //pop the ( in stack ;
               }
           }
           else if (str[i]!=' ') // const and char >>> ex: x^2 , 30 , sin ,....
               temp+=str[i];
       }
   if (temp!="")
       {
        s_func.push_front(Determine(temp)); //
        temp="";
       }
   while (!s_op.empty())
   {
       T2 = new poly ;
       T2->operat = s_op.front();   s_op.pop_front();
       T2->R = s_func.front();      s_func.pop_front();
       T2->L = s_func.front();      s_func.pop_front();
       T2->elem=NULL ;
       s_func.push_front(T2);
       T=T2 ; T2 = NULL ;
   }
   T = s_func.front();
} // finish the all function

QString Function::ToQString ()
{
    QString reswrite ="";
    if (this->Root == NULL)
        return reswrite ;
    else
    {
        poly* T = this ->Root ;
        printpoly(T , reswrite);
    }
    return reswrite ;
}  //finish the all Pring function



void Function::printpoly(poly* T , QString & reswrite)
{
    if ( (T->R!=NULL)&&(T->L!=NULL) ) // for print the poly in case infix ;
    {
        reswrite+='(';
        printpoly(T->L , reswrite);
        if (T->R->elem!=NULL)
        {
            if (T->R->elem->con>=0)
                reswrite+=T->operat;
            else
                if ( (T->operat=='*')||(T->operat=='/') )
                {
                    reswrite+=T->operat;
                    reswrite+='(';
                    twooperat = true ;
                }
        }
        else
            reswrite+=T->operat;
        printpoly(T->R , reswrite);
        if (twooperat)
        {
            twooperat=false ;
            reswrite+=')';
        }
        reswrite+=')';
    }
    else if (T->elem!=NULL)
    {
        if (T->elem->con!=0)//the if for dont print poly in case con =0 ;
        {
            if ( (T->elem->con!=1)||(T->elem->id=="con") )
                if( (T->elem->con==-1)&&(T->elem->id!="con") )
                    reswrite+="-";
            else
                reswrite+=ToString(T->elem->con);
            if (T->elem->pow!=0)  // á?ÚÇ?áÉ ??Ç ßÇä Çá??É 0 áÇ ?ÇÚí á??ÇÚ??
            {
                reswrite+=T->elem->id;
                if (T->elem->pow !=1)
                {
                    reswrite+='^';
                    reswrite+=ToString(T->elem->pow);
                }
                if (T->elem->id!="x") // for  print the internal of sin(....) for example
                {
                    reswrite+='(';
                    printpoly(T->elem->expen,reswrite);
                    reswrite+=')';
                }
            }
        } //finish the if for dont print poly in case con =0 ;
        else
            reswrite+='0';
    } // finish the else of leaf ;

}//finish the all procedure

double Function::F (double value_x)
{
    poly* T =this->Root ;
    return valu_Function (T,value_x);
}

double Function::valu_Function (poly * & T,double value_x)
{
    if ( (T->R!=NULL)&&(T->L!=NULL) ) // for print the poly in case infix ;
    {
        switch (T->operat.cell())
        {
        case '+': return valu_Function(T->L , value_x)+ valu_Function(T->R , value_x);
            break;
        case '-': return valu_Function(T->L , value_x)- valu_Function(T->R , value_x);
            break;
        case '*': return valu_Function(T->L , value_x)* valu_Function(T->R , value_x);
            break;
        case '/': return valu_Function(T->L , value_x)/ valu_Function(T->R , value_x);
            break;
        }//finish the switch
    }
    else if (T->elem!=NULL)
    {
//        switch (T->elem->id)
        if (T->elem->id=="con")
            return T->elem->con ;
        else if (T->elem->id=="x")
            return  (T->elem->con)*(pow(value_x,T->elem->pow));
        else if (T->elem->id=="sin")
            return (T->elem->con)*(pow(sin(valu_Function(T->elem->expen,value_x)),T->elem->pow));
        else if (T->elem->id=="cos")
            return (T->elem->con)*(pow(cos(valu_Function(T->elem->expen,value_x)),T->elem->pow));
        else if (T->elem->id=="exp")
            return (T->elem->con)*(pow(exp(valu_Function(T->elem->expen,value_x)),T->elem->pow));
        else if (T->elem->id=="log")
            return (T->elem->con)*(pow(log(valu_Function(T->elem->expen,value_x)),T->elem->pow));
    }

} //finish the all function ;



Function Function::operator *(const Function &SPoly)
{
    Function* new_Exp = new Function;
    new_Exp->Root = new poly;
    new_Exp->Root->operat = '*';
    new_Exp->Root->L = new_Exp->Root->R = NULL;
    new_Exp->Root->elem = NULL;
    Help_copy(this->Root,new_Exp->Root->L);
    Help_copy(SPoly.Root,new_Exp->Root->R);

    return *new_Exp;
}

Function Function::operator +(const Function& Exp)
{
    Function* new_Exp = new Function;
    new_Exp->Root = new poly;
    new_Exp->Root->operat = '+';
    new_Exp->Root->L = new_Exp->Root->R = NULL;
    new_Exp->Root->elem = NULL;
    Help_copy(this->Root,new_Exp->Root->L);
    Help_copy(Exp.Root,new_Exp->Root->R);

    return *new_Exp;
}

Function Function::operator -(const Function& Exp)
{
    Function* new_Exp = new Function;
    new_Exp->Root = new poly;
    new_Exp->Root->operat = '-';
    new_Exp->Root->L = new_Exp->Root->R = NULL;
    new_Exp->Root->elem = NULL;
    Help_copy(this->Root,new_Exp->Root->L);
    Help_copy(Exp.Root,new_Exp->Root->R);

    return *new_Exp;
}

void Function::operator =(const Function& Exp)
{
    this->Root = NULL;
    Help_copy(Exp.Root,this->Root);

    //*this = Function(Exp);
}

void Function::operator +=(const Function& Exp)
{
    *this = *this + Exp;
}

void Function::operator -=(const Function& Exp)
{
    *this = *this - Exp;
}

void Function::operator *=(const Function& Exp)
{
    *this = *this * Exp;
}

void Function::insertfunc(func myfunc)
{
    func *myelem = NULL  ; //for copy constructer of new func because we need it ;
    copy_func(&myfunc,myelem);

    poly* mypoly = new poly ;
    mypoly->L = mypoly->R = NULL;
    mypoly->elem = myelem;
    mypoly->operat=NULL;

    if (this->Root == NULL)
        this->Root = mypoly ;
    else
    {
        poly* temp = new poly ;
        temp->operat = '+';
        temp->elem = NULL ;
        temp->L = this->Root ;
        temp->R = mypoly ;
        this->Root = temp ;
    }
    fixup();
}

void Function::clear_Function ()
{
    Help_Delete(this->Root) ;
    Root = new poly;
    Root->elem = new func;
    Root->elem->con =0;
    Root->elem->id = "con";
    Root->elem->pow = 0;
    Root->elem->expen = NULL;
}

void Function::Add_ConstToDic(QString str ,double valu)
{
    myDict.insert(str,valu);
}

Function Function::operator +(const QString str)
{
    Function f1(str);
    f1 = *this + f1;
    return f1;
}

Function Function::operator -(const QString str)
{
    Function f1(str);
    f1 = *this - f1;
    return f1;
}

void Function::operator +=(const QString str)
{
    Function f1(str);
    *this+=f1;
}

void Function::operator *=(const QString str)
{
    Function f1(str);
    *this*=f1;
}

Function Function::operator +(const double other)
{
    Function f1(other);
    return f1+*this;
}

Function Function::operator -(const double other)
{
    Function f1(other);
    return *this - f1;
}

void Function::operator +=(const double other)
{
    *this= *this + Function(other) ;
}

void Function::operator *=(const double other)
{
    *this= *this * Function(other) ;
}

Function Function::operator *(const QString str)
{
    return *this * Function(str);
}

Function Function::operator *(const double other)
{
    return *this * Function(other);
}


void Function::fixup()
{
    if (Root != NULL)
    {
        if (Root->operat == '+')
        {
            ProssPluse(Root);
        }
        else if (Root->operat == '*')
        {
            ProssMul(Root);
        }
    }
}

bool Function::checkpoly (QString str )
{
    int count = 0 ;
    bool operat_bracket = false ;
    for ( int i=0 ; i<str.length() ; i++ )
    {
        if (str[i]==' ')
            i++;
        if ((str[i]=='+')||(str[i]=='-')||(str[i]=='*')||(str[i]=='/'))
        {
            if (str[i+1]==')')
                return false ;
            if (i==str.length()-1)
                return false ;
            if ((str[i+1]=='+')||(str[i+1]=='-')||(str[i+1]=='*')||(str[i+1]=='/'))
                return false ;
            if (operat_bracket)
                operat_bracket = false ;
        }
        else
        {
            if ((str[i]=='(')&&(str[i+1]==')'))
                return false ;
            if ( (str[i]==')')&&(str[i+1]=='(') )
                return false ;
            if (str[i]=='(')
                count ++;
            else if (str[i]==')')
            {
                count -- ;
                operat_bracket = true ;
            }
            else if (operat_bracket)
                return false ;
        }
    } //for main for
    if (count !=0)
        return false ;
    return true ;
}





void Function::ProssPluse(poly *T)
{
    if (T != NULL)
    {
        if ((T->R != NULL)&&(T->L != NULL))
        {
            if (T->R->operat == '+')
            {
                ProssPluse(T->R);
            }
            else if (T->L->operat == '+')
            {
                ProssPluse(T->L);
            }
        }
        if ((T->R != NULL)&&(T->R->operat == '*'))
        {
            ProssMul(T->R);
        }
        else if ((T->L != NULL)&&(T->L->operat == '*'))
        {
            ProssMul(T->L);
        }
        if ((T->R->elem != NULL)&&(T->L->elem != NULL))
        {
            if ((T->L->elem->id == "con")&&(T->R->elem->id == "con"))
            {
                if (T->L->elem->pow == T->R->elem->pow)
                {
                    T->operat = ' ';
                    T->elem = new func;
                    T->elem->con = T->L->elem->con + T->R->elem->con;
                    T->elem->expen = NULL;
                    T->elem->id = "con";
                    T->elem->pow = T->L->elem->pow;
                    delete T->R;
                    delete T->L;
                    T->R = NULL;
                    T->L = NULL;
                }

            }
            else if ((T->L->elem->id == "x")&&(T->R->elem->id == "x"))
            {
                if (T->L->elem->pow == T->R->elem->pow)
                {
                    T->operat = ' ';
                    T->elem = new func;
                    T->elem->con = T->R->elem->con + T->L->elem->con;
                    T->elem->id = T->R->elem->id;
                    T->elem->pow = T->R->elem->pow;
                    delete T->R;
                    delete T->L;
                    T->R = NULL;
                    T->L = NULL;
                }
            }
        }
    }
}

void Function::ProssMul(poly *T)
{
    if (T != NULL)
    {
        if ((T->R != NULL)&&(T->L != NULL))
        {
            if ((T->R != NULL)&&(T->R->operat == '*'))
            {
                ProssMul(T->R);
            }
            else if ((T->L != NULL)&&(T->L->operat == '*'))
            {
                ProssMul(T->L);
            }
            if ((T->R != NULL)&&(T->R->operat == '+'))
            {
                ProssPluse(T->R);
            }
            else if ((T->L != NULL)&&(T->L->operat == '+'))
            {
                ProssPluse(T->L);
            }
            if ((T->R != NULL)&&(T->L != NULL))
            {
                if ((T->R->elem != NULL)&& (T->L->elem !=NULL))
                {
                    if ((T->L->elem->id == "con")&&(T->R->elem->id == "con"))
                    {
                        T->operat = ' ';
                        T->elem = new func;
                        T->elem->con = T->R->elem->con * T->L->elem->con;
                        T->elem->id = "con";
                        T->elem->pow = T->L->elem->pow + T->R->elem->pow;
                        T->elem->expen = NULL;
                        delete T->R;
                        delete T->L;
                        T->R = NULL;
                        T->L = NULL;
                    }
                    else if ((T->L->elem->id == "con")||(T->R->elem->id == "con"))
                    {
                        if ((T->L->elem->id == "con")&&((T->R->operat != '+')&&(T->R->operat != '*')))
                        {
                            T->operat = ' ';
                            T->elem = new func;
                            T->elem->con = T->R->elem->con * T->L->elem->con;
                            T->elem->pow = T->R->elem->pow;
                            T->elem->id = T->R->elem->id;
                            T->elem->expen = T->R->elem->expen;
                            delete T->R;
                            delete T->L;
                            T->R = NULL;
                            T->L = NULL;
                        }
                        else if ((T->R->elem->id == "con")&&((T->L->operat != '+')&&(T->L->operat != '*')))
                        {
                            T->operat = ' ';
                            T->elem = new func;
                            T->elem->con = T->R->elem->con * T->L->elem->con;
                            T->elem->pow = T->L->elem->pow;
                            T->elem->id = T->L->elem->id;
                            T->elem->expen = T->L->elem->expen;
                            delete T->R;
                            delete T->L;
                            T->R = NULL;
                            T->L = NULL;
                        }
                    }
                    else if ((T->R->elem->id == "x")&&(T->L->elem->id == "x"))
                    {
                        T->operat = ' ';
                        T->elem = new func;
                        T->elem->con = T->R->elem->con * T->L->elem->con;
                        T->elem->id = T->R->elem->id;
                        T->elem->pow = T->L->elem->pow + T->R->elem->pow;
                        delete T->R;
                        delete T->L;
                        T->L = NULL;
                        T->R = NULL;
                    }
                }
            }
        }
    }
}
