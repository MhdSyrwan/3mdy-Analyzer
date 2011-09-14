#include "console.h"
#include "ui_console.h"
#include <QLabel>
Console::Console(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
}

Console::~Console()
{
    delete ui;
}

void Console::on_lineEdit_returnPressed()
{
    QLabel * label = new QLabel(ui->frame);
    label->setText(ui->lineEdit->text());
    ui->frame->layout()->addWidget(label);
    label->show();
    ui->lineEdit->clear();
}
