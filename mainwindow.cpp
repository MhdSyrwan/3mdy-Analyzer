#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <stdlib.h>
#include "console.h"


QString toString(double x)
{
    QString str("%1");
    str =str.arg(x);
    return str;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lbl_pos = new QLabel;
    ui->statusBar->addWidget(lbl_pos);
    lbl_pos->setMinimumWidth(100);
    lbl_info = new QLabel;
    ui->statusBar->addWidget(lbl_info);
    QCoreApplication::setOrganizationName("3mdy");
    QCoreApplication::setOrganizationDomain("3mdy.net");
    QCoreApplication::setApplicationName("3mdy Analyzer");

    updateInfo();
    Function::init();
    ui->widget->currentFunction = new Function("0");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        //ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->ex->text() != ""  &&  ui->ey->text() != "")
    {
        ui->widget->currentTool =  AnalysisDocument::create;
        ui->widget->addPoint(AnalysePoint(ui->ex->text().toDouble(),ui->ey->text().toDouble()));
        ui->widget->refresh();
    }
    else{
        QMessageBox::information(this,"You Missed Something","X or Y Field is Blank !!",0);
    }
}

void MainWindow::on_pushButton_2_clicked()
{

    double num  = ui->txt_rootNum->text().toDouble();
    int n    = ui->txt_rootDeg->text().toInt();
    double x1   = ui->txt_rootUpper->text().toDouble();
    double res  = root_n(num,n,x1,ui->widget->precision);
    ui->lbl_rootRes->setText(toString(res));
    func f;
    f.id = "x";
    f.pow = n;
    f.con = 1;
    Function *a = new Function(f);
    *a += (-1) * num ;
    ui->widget->setCurrentFunction(a);
    ui->widget->update();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->clear();
    ui->widget->refresh();
}

void MainWindow::UpdateLabelText(QPoint pt)
{
    QString str=QString("(%1 , %2)").arg(pt.x()).arg(pt.y());
    lbl_pos->setText(str);

}


void MainWindow::on_actionSelect_toggled(bool val)
{
    if (val)
    {
        ui->widget->currentTool = AnalysisDocument::select_y;
        ui->widget->update();
        ui->actionCreate->setChecked(false);
    }
}


void MainWindow::updateCurve()
{
    QString str(ui->lineEdit->text());
    if (Function::checkpoly(str))
    {
        try {
            Function *a= new Function(str);
            ui->widget->setCurrentFunction(a);
            ui->widget->clear();
        } catch (std::exception e) {

        }

    }
    else
    {
        QMessageBox::critical(this,"Error","Bad Expression",0,0);
    }
    ui->widget->update();

}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    ui->widget->zoomFactor=1+position*0.07;
    ui->widget->update();
}

void MainWindow::on_actionCreate_triggered(bool checked)
{

}

void MainWindow::on_actionCreate_toggled(bool checked)
{
    if (checked)
    {
        ui->widget->currentTool = AnalysisDocument::create;
        ui->widget->update();
        ui->actionSelect->setChecked(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{

}

void MainWindow::on_actionSettings_triggered()
{
    Dialog a(this);
    if (a.exec()==QDialog::Accepted)
    {
        ui->widget->precision=a.precision;
        ui->widget->drawingStep=a.drawingStep;
        ui->widget->constStep=a.constStep;
        updateInfo();
    }
}

void MainWindow::on_btn_selectA_clicked()
{
    ui->widget->currentTool=AnalysisDocument::select_a;
}

void MainWindow::on_btn_selectB_clicked()
{
    ui->widget->currentTool=AnalysisDocument::select_b;
}



void MainWindow::updateLinesPos()
{
    if (ui->widget->currentTool==AnalysisDocument::select_a)
    {
        ui->txt_lineA->setText(toString(ui->widget->lineA));
        ui->txt_optlineA->setText(toString(ui->widget->lineA));
    }
    else if (ui->widget->currentTool==AnalysisDocument::select_b)
    {
        ui->txt_lineB->setText(toString(ui->widget->lineB));
        ui->txt_optlineB->setText(toString(ui->widget->lineB));
    }
    ui->widget->currentTool=AnalysisDocument::no_tool;
}


bool checkRoots(Function *f,double x1,double x2)
{
    return (f->F(x1) * f->F(x2) < 0);
}

void MainWindow::on_btn_FindX_clicked()
{
    if (checkRoots(ui->widget->currentFunction,ui->widget->lineA,ui->widget->lineB))
    {
        if (ui->widget->lineB > ui->widget->lineA )
        {   double res=NAN;
            if (ui->radio_bs->isChecked())
                res = BinarySearch(ui->widget->lineA,ui->widget->lineB,ui->widget->precision,*(ui->widget->currentFunction));
            else if (ui->radio_ws->isChecked())
                res = WrongSituation(ui->widget->lineA,ui->widget->lineB,ui->widget->precision,*(ui->widget->currentFunction));

            ui->lbl_bsResut->setText(toString(res));
        }
        else{
            ui->lbl_bsResut->setText("Error : B<A !!");
        }
    }
    else
    {
        QMessageBox::information(this,"Math Error","No Roots or there is more than one !!",0);
    }

}

void MainWindow::on_txt_lineA_editingFinished()
{
    ui->widget->lineA=ui->txt_lineA->text().toDouble();
    ui->widget->update();
}

void MainWindow::on_txt_lineB_editingFinished()
{
    ui->widget->lineB=ui->txt_lineB->text().toDouble();
    ui->widget->update();
}

void MainWindow::on_txt_optlineA_editingFinished()
{
    ui->widget->lineA=ui->txt_optlineA->text().toDouble();
    ui->widget->update();
}

void MainWindow::on_txt_optlineB_editingFinished()
{
    ui->widget->lineB=ui->txt_optlineB->text().toDouble();
    ui->widget->update();
}

void MainWindow::on_btn_selectA_2_clicked()
{
    ui->widget->currentTool=AnalysisDocument::select_a;
}

void MainWindow::on_btn_selectB_2_clicked()
{
    ui->widget->currentTool=AnalysisDocument::select_b;
}

void MainWindow::on_radio_divisedDiffs_toggled(bool checked)
{
    if (checked)
        ui->widget->setAppMode(AnalysisDocument::divizedDiffs);
}

void MainWindow::on_radio_frontDiffs_toggled(bool checked)
{
    if (checked)
        ui->widget->setAppMode(AnalysisDocument::frontDiffs);
}

void MainWindow::on_radio_lagrange_toggled(bool checked)
{
    if (checked)
        ui->widget->setAppMode(AnalysisDocument::lagrange);
}

void MainWindow::on_btn_Op_clicked()
{
    if (ui->widget->lineA < ui->widget->lineB)
    {
        double xl,xu;
        xl=ui->widget->lineA;
        xu=ui->widget->lineB;
        max_val(*ui->widget->currentFunction,xl,xu,ui->widget->precision);
        double res =(xl+xu)/2;
        ui->lbl_optRes->setText(toString(res));
    }
    else {
        ui->lbl_optRes->setText("Error : B must be greater than A");
    }
}

void MainWindow::updateInfo()
{
    QSettings settings("3mdy", "Analyzer");
    settings.beginGroup("math");
    double precision = settings.value("eps", double(0.01)).toDouble();
    double drawingStep = settings.value("step", double(0.1)).toDouble();
    double constStep = settings.value("constStep", double(0.1)).toDouble();
    ui->widget->precision=precision;
    ui->widget->drawingStep=drawingStep;
    ui->widget->constStep=constStep;
    settings.endGroup();
    ui->widget->update();
    QString str("drawing step=%1 ; constStep=%2 ; epsilon=%3 ; you can edit these from above menus");
    str=str.arg(drawingStep).arg(constStep).arg(precision);
    lbl_info->setText(str);
}

void MainWindow::updateFunctionText()
{
    ui->widget->currentFunction->fixup();
    ui->txt_function->setText(ui->widget->currentFunction->ToQString());
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionAbout_triggered()
{
  QString abtstr= "3mdy Analyzer 2011";
    abtstr.append("\n     second year project\n");
    abtstr.append("\nby 3mdy Group : ");
    abtstr.append("\n  Muhammad Al-Syrwan");
    abtstr.append("\n  Muhammad Yman Al-Turh");
    abtstr.append("\n  Muhammad Al-Jobory");
    abtstr.append("\n  Yaser Al-Srouji Kannany");
    abtstr.append("\n\nITE College University of Damascus");

    QMessageBox::information(this,"About Us",abtstr,0);
}

void MainWindow::on_actionConsole_triggered()
{
    Console a(this);
    a.show();
}
