#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    connect(ui->expected_sb,SIGNAL(valueChanged(double)),this,SLOT(checkForm()));
    connect(ui->inc,SIGNAL(valueChanged(double)),this,SLOT(checkForm()));
    connect(ui->deviation_sb,SIGNAL(valueChanged(double)),this,SLOT(checkForm()));
    connect(ui->size_n,SIGNAL(valueChanged(int)),this,SLOT(checkForm()));
    connect(ui->count_level,SIGNAL(valueChanged(int)),this,SLOT(checkForm()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(accept()));
}

void Dialog::checkForm()
{
    bool result = ui->expected_sb->value() && ui->deviation_sb->value()
            && ui->size_n->value() && ui->count_level->value();

    if(result)
    {
        ui->pushButton->setEnabled(true);
    }
}

dialog_data Dialog::getParam()
{
    dialog_data tmp;
    tmp.expected=ui->expected_sb->value();
    tmp.deviation=ui->deviation_sb->value();
    tmp.a = ui->level_sb->value();
    tmp.count_level=ui->count_level->value();
    tmp.inc = ui->inc->value();
    tmp.n = ui->size_n->value();
    return tmp;
}

Dialog::~Dialog()
{
    delete ui;
}
