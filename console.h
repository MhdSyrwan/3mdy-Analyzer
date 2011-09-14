#ifndef CONSOLE_H
#define CONSOLE_H

#include <QScrollArea>

namespace Ui {
    class Console;
}

class Console : public QScrollArea
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = 0);
    ~Console();

private slots:
    void on_lineEdit_returnPressed();
private:
    Ui::Console *ui;
};

#endif // CONSOLE_H
