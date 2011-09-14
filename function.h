#ifndef POLYNUM_H
#define POLYNUM_H
#include <QObject>
#include <qstack.h>
#include <qmath.h>
#include <QHash>

class Function;
struct func ;
const double pi = 22.0/7 ;
struct poly
{
    poly()
    {
        this->L = this->R = NULL ;
        this->operat = NULL ;
        this->elem = NULL ;
    }
    func* elem ;
    QChar operat ;
    poly* R ;
    poly* L ;
};

struct func
{
    func()
    {
        this->con = 0;
        this->expen = NULL ;
        this->id = "con";
        this->pow = 0 ;
    }
    QString id ;
    double con ;
    double pow ;
    poly* expen ;
};


class Function
{
public:
    poly* Root ;


    Function();
    Function(QString);
    Function(func );
    Function(poly*);
    Function(const Function & );
    Function(const double);
    ~Function();

    static bool checkpoly (QString str );
    void fixup();

    double F (double value_x);
    poly* Determine(QString str);
    void AddToTree(poly * &T,QString str);

    void Add_ConstToDic(QString str ,double valu) ;

    void operator =(const Function& Exp);
    void operator +=(const Function& Exp);
    void operator -=(const Function& Exp);
    void operator *=(const Function& Exp);
    Function operator *(const Function& Exp);
    Function operator +(const Function& Exp);
    Function operator -(const Function& Exp);


    Function operator *(const QString str);
    Function operator +(const QString str);
    Function operator -(const QString str);
    void operator +=(const QString str);
    void operator *=(const QString str);

    Function operator *(const double other);
    Function operator +(const double other);
    Function operator -(const double other);
    void operator +=(const double other);
    void operator *=(const double other);

    void insertfunc(func );
    void clear_Function ();
    QString ToQString ();
    static void init();
private :

    void copy_func(const func* From,func* &TO);
    void Help_copy(const poly* Exp,poly* &new_Exp);
    void copy(const Function &Exp,Function &To);

    void delete_func(func* &f);
    void Help_Delete(poly* &Exp);
    void Delete(Function&);

    void printpoly(poly* T,QString & );//To String
    double valu_Function (poly * & T,double value_x);

    // jebory commits
    void ProssPluse(poly*);
    void ProssMul(poly*);


};

#endif // POLYNUM_H
