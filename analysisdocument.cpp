
#include "analysisdocument.h"


AnalysisDocument::AnalysisDocument(QWidget *parent) :
    QWidget(parent)
{
    zoomFactor=1;
    memo=NULL;
    currentTool = create;
    currentFunction=NULL;
    this->cur_point_index=-1;
    //this->setMouseTracking(true);
    this->pointColor=Qt::green;
    this->selectedPointColor=Qt::red;
    drawingStep =0.1;
    painter=NULL;

    // domains
    lineA=0;
    lineB=0;
    lineX=0;
    appMode = divizedDiffs;
    constStep=40;
}


void AnalysisDocument::addPoint(AnalysePoint p)
{
    this->points.append(p);
}

void AnalysisDocument::removePoint(int i)
{
    this->points.removeAt(i);
}

void AnalysisDocument::refresh()
{
    current_n=-1;
    if (appMode == divizedDiffs)
        ProDiffs();
    else if (appMode == frontDiffs)
        setFrontDiffsFunction();
    else if (appMode == lagrange)
        setLagrangeFunction();
    this->update();
}

void AnalysisDocument::mouseMoveEvent(QMouseEvent *e)
{
    double x=e->x(),y=e->y();
    fixCoords(x,y);
    cursorPosition.setX(x);
    cursorPosition.setY(y);
    emit mouseMoved(cursorPosition);

    // checking for points under cursor
    for (int i=0; i<points.length();i++)
    {
        QRegion pRect(points[i].x()-5,points[i].y()-5,10,10);
        if ( pRect.contains(QPoint(x,y)) ) // this could be inhanced
            points[i].setMouseOver();
        else
            points[i].unsetMouseOver();
    }

    switch (currentTool)
    {
    case create:
        {
            if (cur_point_index!=-1)
            {
                points[cur_point_index].setX(x);
                points[cur_point_index].setY(y);
                current_n=-1;
                if (appMode == divizedDiffs)
                    ProDiffs();
                else if (appMode == frontDiffs)
                    setFrontDiffsFunction();
                else if (appMode == lagrange)
                    setLagrangeFunction();
            }
        }
    break;
    case select_a:
    case select_b:
    {
        lineX=x;
    }

    default:
        break;
    }
    this->update();
}

void AnalysisDocument::paintEvent(QPaintEvent *e)
{
//    if (painter!=NULL) delete painter;
    painter= new QPainter(this);
    if (! painter->isActive() ) painter->begin(this);
    // initializing ...
    int w,h;
    h=this->height();
    w=this->width();


    painter->translate(w/2,h/2);
    painter->drawText(w/2-20,20,"x");
    painter->scale(zoomFactor,-1*zoomFactor);
//    painter->setViewTransformEnabled(true);

    painter->drawLine(-w/2,0,w/2,0);
    painter->drawLine(0,-h/2,0,h/2);


    painter->setPen(Qt::blue);
    DrawCurve();

    painter->setPen(Qt::magenta);
    painter->drawLine(lineA,-h/2,lineA,h/2);
    painter->drawLine(lineB,-h/2,lineB,h/2);


    switch (currentTool)
    {
    case create:
    {
        double zf =zoomFactor;
        for (int i=0;i<points.length();i++)
        {
            if (points[i].IsMouseOver())
                painter->setPen(selectedPointColor);
            else
                painter->setPen(pointColor);
            painter->drawRect(points[i].x()-5/zf,points[i].y()-5/zf,10/zf,10/zf);
        }
    }
    break;
    case select_y:
    {
        painter->setPen(Qt::yellow);
        painter->drawLine(-w/2,cursorPosition.y(),w/2,cursorPosition.y());
    }
    break;
    case select_a:
    case select_b:
    {
        painter->drawLine(lineX,-h/2,lineX,w/2);
    }

    default:
        break;
    }
    painter->end();

}

int AnalysisDocument::getPointIndex(QPoint p)
{
    int res=-1;
    for (int i=0; i<points.length();i++)
    {
        QRegion pRect(points[i].x()-5,points[i].y()-5,10,10);
        if ( pRect.contains(p) )
        {
            res=i;
            break;
        }
    }
    return res;
}
void AnalysisDocument::fixCoords(double& x,double& y)
{



    x=x-this->width()/2;
    y=-y+this->height()/2;

    x/=zoomFactor;
    y/=zoomFactor;

    if (appMode==frontDiffs  && currentTool == create)
        checkFront(x,constStep);
}

void AnalysisDocument::mousePressEvent(QMouseEvent *e)
{
    double x=e->x(),y=e->y();
    fixCoords(x,y);

    cur_point_index=getPointIndex(QPoint(x,y));
    switch (currentTool)
    {
    case create:
    {
        if (e->button() == Qt::RightButton)
        {
            if (cur_point_index!=-1)
            {
                removePoint(cur_point_index);
                cur_point_index=-1;
                current_n=-1;
                if (appMode == divizedDiffs)
                    ProDiffs();
                else if (appMode == frontDiffs)
                    setFrontDiffsFunction();
                else if (appMode == lagrange)
                    setLagrangeFunction();
            }
        }
        else
        {
            if (cur_point_index==-1)
            {
                points.append(AnalysePoint(x,y));
                cur_point_index=points.length()-1;
                if (appMode == divizedDiffs)
                    ProDiffs();
                else if (appMode == frontDiffs)
                    setFrontDiffsFunction();
                else if (appMode == lagrange)
                    setLagrangeFunction();
            }
        }
    break;
    }
    case select_y:
    {

    }
    break;
    case select_a:
        lineA=lineX ;
        emit lineSelected();
    break;
    case select_b:
        lineB=lineX ;
        emit lineSelected();
    break;

    default:
        break;

}
    this->update();
}

void AnalysisDocument::mouseReleaseEvent(QMouseEvent *e)
{
    switch (currentTool)
    {
    case create:
        if (cur_point_index != -1)
        {
            double x=e->x(),y=e->y();
            fixCoords(x,y);
            points[cur_point_index].setX(x);
            points[cur_point_index].setY(y);
            current_n=0;
            if (appMode == divizedDiffs)
                ProDiffs();
            else if (appMode == frontDiffs)
                setFrontDiffsFunction();
            else if (appMode == lagrange)
                setLagrangeFunction();
            this->update();
            cur_point_index=-1;
        }
        break;
    default:
        break;
    }
}


double AnalysisDocument::ProDiffsRec(int i,int n,QList<AnalysePoint> pt)
{
    double diff;
    if (memo[i][n]!=0xFFFFFFF)
        diff = memo[i][n];
    else if (n==i)
        diff = pt[n].y();
    else
        diff = (ProDiffsRec(i+1 ,n ,pt) - ProDiffsRec(i ,n-1 ,pt))  /  (pt[n].x()-pt[i].x());
    memo[i][n]=diff;
    return diff;
}

void AnalysisDocument::ProDiffs()
{
    int n = points.length();
    if (current_n == n)
    {

    }
    else
    {
        if (memo!=NULL)
            delete [] memo;
        memo = new double* [n];
        current_n=n;
        for (int i=0 ;i<n ;i++)
        {
            memo[i] = new double[n];
            for (int j=0; j<n ; j++)
                memo[i][j] = 0xFFFFFFF ;
        }
        setApproxFunction(points);
    }

}
void AnalysisDocument::setApproxFunction(QList<AnalysePoint> &points)
{   
    Function diffs("1");
    Function acc("0");
    for (int i=0;i<points.length();i++)
    {
        acc +=  diffs * ProDiffsRec(0,i,points);
        diffs *= ( Function("x")- points[i].x() );
    }
    Function *res = new Function(acc);
    setCurrentFunction(res);
}

void AnalysisDocument::DrawCurve()
{
    double w = this->width();
    QPointF p0(-w/2,0);

    if (currentFunction != NULL)
    {
        double y =currentFunction->F(-w/2);
        if (!(y==NAN || y==INFINITY || y==-INFINITY))
            p0.setY(y);// watch out for  NAN
    }

    QPointF p1;

    for (double x=-w/2;x<=this->width();x+=drawingStep)
    {
        p1.setX(x);
        if (currentFunction != NULL)
        {
            double y =currentFunction->F(x);
            if (!(y==NAN || y==INFINITY || y==-INFINITY))
                p1.setY(y);// watch out for  NAN
        }

        painter->drawLine(p0,p1);

        p0=QPointF(p1);
    }
}


void AnalysisDocument::clear()
{

    points.clear();
    update();
}

void AnalysisDocument::setTool(AnalysisDocument::Tool t)
{
    currentTool=t;
    switch (t)
    {
    case create:
        setApproxFunction(points);
        break;
    case select_y:
        break;
    }
}

void AnalysisDocument::setFrontDiffsFunction()
{
    sortPointsList(points);
    double *X,*Y;
    convertVectorToList(points,X,Y);

    Function* f = new Function(front_deff(points.count(),constStep,X,Y));
    setCurrentFunction(f);

}

void AnalysisDocument::setLagrangeFunction()
{
    double *X,*Y;
    convertVectorToList(points,X,Y);
    Function* f = new Function(lagrangeApprox(X,Y,points.count()));
    setCurrentFunction(f);

}

void AnalysisDocument::setCurrentFunction(Function *f)
{
    delete currentFunction;
    this->currentFunction=f;
    emit functionChanged();
}

