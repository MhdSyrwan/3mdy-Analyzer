#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QMessageBox>

#include "mathlib.h"
#include "function.h"
#include "analysisdocument.h"
#include <QLabel>

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel *lbl_pos;
    QLabel *lbl_info;

public slots:
    void updateInfo();
protected:
    void changeEvent(QEvent *e);
private:

    int mouseTimerId;
    Ui::MainWindow *ui;

private slots:
    void on_txt_lineB_editingFinished();
    void on_txt_lineA_editingFinished();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void UpdateLabelText(QPoint);
    void updateCurve();
    void on_actionSelect_toggled(bool );
    void on_verticalSlider_sliderMoved(int position);
    void on_actionCreate_triggered(bool checked);
    void on_actionCreate_toggled(bool );
    void resizeEvent(QResizeEvent *e);
    void on_actionSettings_triggered();
    void on_btn_selectA_clicked();
    void on_btn_selectB_clicked();
    void updateLinesPos();
    void on_btn_FindX_clicked();
    void on_txt_optlineA_editingFinished();
    void on_txt_optlineB_editingFinished();
    void on_btn_selectA_2_clicked();
    void on_btn_selectB_2_clicked();
    void on_radio_divisedDiffs_toggled(bool checked);
    void on_radio_frontDiffs_toggled(bool checked);
    void on_radio_lagrange_toggled(bool checked);
    void on_btn_Op_clicked();
    void updateFunctionText();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionConsole_triggered();
};

#endif // MAINWINDOW_H
