#include "dialog.h"
#include "ui_dialog.h"
#include <QSettings>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QSettings settings("3mdy", "Analyzer");
    settings.beginGroup("math");
    ui->txt_errorValue->setText(settings.value("eps", double(0.01)).toString());
    ui->txt_drawingStep->setText(settings.value("step", double(0.1)).toString());
    ui->txt_constStep->setText(settings.value("constStep", double(40)).toString());
    precision = settings.value("eps", double(0.01)).toDouble();
    drawingStep = settings.value("step", double(0.1)).toDouble();
    constStep = settings.value("constStep", double(0.1)).toDouble();

    settings.endGroup();


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_Dialog_accepted()
{

}

void Dialog::on_buttonBox_accepted()
{
    precision = ui->txt_errorValue->text().toDouble();
    drawingStep = ui->txt_drawingStep->text().toDouble();
    constStep = ui->txt_constStep->text().toDouble();
    QSettings settings("3mdy", "Analyzer");
    settings.beginGroup("math");
    settings.setValue("eps",precision);
    settings.setValue("step",drawingStep);
    settings.setValue("constStep",constStep);
    settings.endGroup();

}

