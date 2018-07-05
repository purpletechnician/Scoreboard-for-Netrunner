
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void Player1_Name(QString);
    void Player2_Name(QString);
    void Player1_Score(int);
    void Player2_Score(int);
    void Clock(QString);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
