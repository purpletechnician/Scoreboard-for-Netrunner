#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::Player1_Name(QString text)
{
    ui->Player1_Name->setText(text);
}
void Dialog::Player2_Name(QString text)
{
    ui->Player2_Name->setText(text);
}
void Dialog::Player1_Score(int text)
{
    ui->Player1_Score->setText(QString::number(text));
}
void Dialog::Player2_Score(int text)
{
    ui->Player2_Score->setText(QString::number(text));
}

void Dialog::Clock(QString text)
{
    ui->Clock->setText(text);
}
