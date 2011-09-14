#ifndef ANALYSISDOCUMENT_H
#define ANALYSISDOCUMENT_H

#include <QWidget>
#include <qmath.h>
#include <QMouseEvent>
#include <QColor>
#include <QPoint>
#include <QtCore>
#include <QPainter>
#include "function.h"
#include "mathlib.h"
#include "analysepoint.h"


class AnalysisDocument : public QWidget
{
    Q_OBJECT
public:
    enum Tool {
        no_tool=0,
        create=1,
        select_y=2,
        select_a=4,
        select_b=8,
    };
    enum approximationMode{
        none=0,
        frontDiffs=1,
        divizedDiffs=2,
        lagrange=4
    };

    // settings
    double drawingStep;
    double precision;

    //adds a point and recalculate functions.
    void addPoint(AnalysePoint p);
    // removes a point and recalculate function
    void removePoint(int i);
    // recalculates the function .
    void refresh();


    // clears the point list
    void clear();

    void setTool(Tool t);

    AnalysisDocument(QWidget *parent = 0);
    void setAppMode(approximationMode val){
        if (val == frontDiffs)
            clear();
        refresh();
        appMode = val;
    }

    Tool currentTool;   // current tool to manipulate the curve
    Function *currentFunction;
    void setCurrentFunction(Function* f);

    //Visuals
    QColor pointColor;
    QColor selectedPointColor;
    double zoomFactor;

    // Front Differents constant Step
    double constStep;
    // domain parties selectors
    double lineA;
    double lineB;
    double lineX;

private:
    approximationMode appMode;
    void fixCoords(double &x, double &y);

    int getPointIndex(QPoint p);

    //points section :
    QList<AnalysePoint> points; // points to generate the function
    int current_n; // current number of points .
    int cur_point_index; // selected point index .

    double **memo; // double indexed array used in memoization for the recursive methd ProDiffsRec

    double ProDiffsRec(int i,int n,QList<AnalysePoint> pt); // recursive method for calculating the front divided differents
    void ProDiffs(); // mehtod for managing 'ProDiffsRec' method and its memo array
    void setApproxFunction(QList<AnalysePoint> &points); // the function that gives the 'y' value for given 'x' in the expected function

    void setFrontDiffsFunction();
    void setLagrangeFunction();

    // Visuals
    void DrawCurve(); // draws the whole plot.
    QPoint cursorPosition;
    QPainter *painter; // painter object preservd for drawing

signals:
    void mouseMoved(QPoint p);
    void lineSelected();
    void functionChanged();
public slots:
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


};

#endif // ANALYSISDOCUMENT_H
