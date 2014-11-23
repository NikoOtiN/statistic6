#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

struct dialog_data{
    double expected;
    double deviation;
    double inc;
    double count_level;
    double n;
    double a;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    dialog_data getParam();
public slots:
    void checkForm();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
