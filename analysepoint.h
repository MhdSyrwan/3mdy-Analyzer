#ifndef ANALYSEPOINT_H
#define ANALYSEPOINT_H

#include <QPointF>
#include <QColor>

class AnalysePoint :public QPointF
{
private:
    bool mouseOver;
public:
    AnalysePoint():QPointF() {mouseOver=false;}
    AnalysePoint(qreal x,qreal y):QPointF(x,y) {mouseOver=false;}
    QColor color;
    void setMouseOver() {mouseOver=true;}
    void unsetMouseOver() {mouseOver=false;}
    bool IsMouseOver() {return mouseOver;}

};

#endif // ANALYSEPOINT_H
