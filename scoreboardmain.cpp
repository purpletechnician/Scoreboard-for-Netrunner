#include "scoreboardmain.h"
#include "ui_scoreboardmain.h"
#include <windows.h> //For Hotkey/Shortcut key
#include <fstream> //For Outputing to .txt
#include <ctime>
#include <QTimer>
#include <QString>
#include <string>
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>
#include <QMediaMetaData>
#include <QComboBox>

using namespace std;

int Player1_Score = 0, Player2_Score = 0; //Team Score integer
int minu = 65, seco = 0; //Minutes and Seconds integer
int mins = 0, secs = 0; //Used for To_input
int saved_speed = 0, last_speed = 0; //Stored speed integer
int speed = 1000; //Speed integer | Default = 1000 (1seconds) | Milliseconds = 1000(0.1seconds)
int Clock_button = 0; //Start/Stop button
int Normal_Speed = 1000, Millisecond_Speed = 93;
int PresetRadio = 2;

bool bol = false; //Open/Close for the server
bool many = false; //For milliseconds register
bool input_stop = false; //Stop getting the number from the input
bool getinput = false;
bool testplayer = false; //Test player button
bool to_switch; //On/Off for To_input
bool Milliseconds = false, Minute_Zero = false, Hotkey = false, Stopwatch_input = false; //On/Off for Milliseconds, Add zero to minute, Hotkey, Stopwatch
bool presetbool = true; //Preset timer
bool SecretIanButton = false; //Secret Ian Button

string minutes_zero = "", seconds_zero = ""; //For 9 <-> 0 | Example: 09 05
string Player1_Name = "", Player2_Name = ""; //Name for Player1, Player2
string Player1_Id = "", Player2_Id = ""; //Id for Player1, Player2
string clock_symbol = ":"; //Clock Symbol | Default = : | Milliseconds = .
string Window_Name = "Scoreboard for Netrunner 0.9"; //Please Change this after a update!
QString Clock_text = "65:00"; //Clock Text
QList<QString> IdList;

ofstream Player1_Name_Output, Player2_Name_Output, Player1_Id_Output, Player2_Id_Output, Player1_Score_Output, Player2_Score_Output, Period_Output, Clock_Output; //Ofstream for outputting to .txt

ScoreboardMain::ScoreboardMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScoreboardMain)
{
    ui->setupUi(this);
    di = new Dialog(this);
    //di->show();
    ScoreboardMain::Opened(); //Declares/Resets Opened() DO NOT DELETE
}



ScoreboardMain::~ScoreboardMain()
{
    delete ui;
}

void ScoreboardMain::Opened() //Resets all
{
    writexml();
    Player1_Name_Output.open(".\\Output\\Player1_Name.txt");
    Player2_Name_Output.open(".\\Output\\Player2_Name.txt");
    Player1_Id_Output.open(".\\Output\\Player1_Id.txt");
    Player2_Id_Output.open(".\\Output\\Player2_Id.txt");
    Player1_Score_Output.open(".\\Output\\Player1_Score.txt");
    Player2_Score_Output.open(".\\Output\\Player2_Score.txt");
    Clock_Output.open(".\\Output\\Clock.txt");

    Player1_Name_Output << "";
    Player2_Name_Output << "";
    Player1_Id_Output << "";
    Player2_Id_Output << "";
    Player1_Score_Output << Player1_Score;
    Player2_Score_Output << Player2_Score;
    Clock_Output << "00:00";

    Player1_Name_Output.close();
    Player2_Name_Output.close();
    Player1_Id_Output.close();
    Player2_Id_Output.close();
    Player1_Score_Output.close();
    Player2_Score_Output.close();
    Clock_Output.close();

    IdList.append("Anarch: Quetzal"); IdList.append("Anarch: Edward Kim"); IdList.append("Anarch: MaxX"); IdList.append("Anarch: Valencia Estevez"); IdList.append("Anarch: Null");
    IdList.append("Anarch: Omar Keung"); IdList.append("Anarch: Alice Merchant"); IdList.append("Anarch: Reina Roja"); IdList.append("Anarch: Freedom Khumalo");
    IdList.append("Criminal: Iain Stirling"); IdList.append("Criminal: Ken Express Tenma"); IdList.append("Criminal: Silouette"); IdList.append("Criminal: Leela Patel");
    IdList.append("Criminal: Armand Geist Walker"); IdList.append("Criminal: Laramy Fisk");IdList.append("Criminal: Nero Severn");IdList.append("Criminal: Khan");
    IdList.append("Criminal: Los"); IdList.append("Criminal: Steve Cambridge"); IdList.append("Criminal: Gabriel Santiago"); IdList.append("Criminal: 419");
    IdList.append("Shaper: Rielle Kit Peddler"); IdList.append("Shaper: The Professor"); IdList.append("Shaper: Exile"); IdList.append("Shaper: Nasir Meidan");
    IdList.append("Shaper: Hayley Kaplan") ;IdList.append("Shaper: Jesminder Sareen"); IdList.append("Shaper: Ele Smoke Scovak"); IdList.append("Shaper: Ayla Bios Rahim");
    IdList.append("Shaper: Chaos Theory"); IdList.append("Shaper: Kabonesa Wu");
    IdList.append("Adam"); IdList.append("Apex"); IdList.append("Sunny Lebeau");

    IdList.append("HB: Cerebral Imaging"); IdList.append("HB: Custom Biotics"); IdList.append("HB: NEXT Design"); IdList.append("HB: The Foundry"); IdList.append("HB: Cybernetics Division");
    IdList.append("HB: Architects of Tomorrow"); IdList.append("HB: Seidr Laboratiries");  IdList.append("HB: Stronger Together"); IdList.append("HB: Asa Group");
    IdList.append("NBN: Near-Earth Hub"); IdList.append("NBN: Haarpsichord Studios"); IdList.append("NBN: SYNC"); IdList.append("NBN: New Angeles Sol"); IdList.append("NBN: Spark Agency");
    IdList.append("NBN: Harishchandra Ent."); IdList.append("NBN: Controlling the Message"); IdList.append("NBN: Making News"); IdList.append("NBN: Azmari EdTech");
    IdList.append("Jinteki: Harmony Medtech"); IdList.append("Jinteki: Nisei Division"); IdList.append("Jinteki: Tennin Institute"); IdList.append("Jinteki: Industrial Genomics");
    IdList.append("Jinteki: Jinteki Biotech"); IdList.append("Jinteki: Chronos Protocol"); IdList.append("Jinteki: Palana Foods"); IdList.append("Jinteki: Potential Unleashed");
    IdList.append("Jinteki: Aginfusion"); IdList.append("Jinteki: Personal Evolution"); IdList.append("Jinteki: Mti Mwekundu");
    IdList.append("Weyland: Blue Sun"); IdList.append("Weyland: Argus Security"); IdList.append("Weyland: Gagarin Deep Space"); IdList.append("Weyland: Titan Transnational");
    IdList.append("Weyland: Builder of Nations"); IdList.append("Weyland: Jemison Astronautics"); IdList.append("Weyland: Skorpios Defence Systems"); IdList.append("Weyland: Building a Better World");
    IdList.append("Weyland: SSO Industries");

    ui->Player1Id_Input->addItems(IdList);
    ui->Player2Id_Input->addItems(IdList);

    ui->Player1UP_Button->setText((QString::fromWCharArray(L"\u25B3")));
    QFont font2 = ui->Player1UP_Button->font();
    font2.setPointSize(20);
    ui->Player1UP_Button->setFont(font2);

    ui->Player1DOWN_Button->setText((QString::fromWCharArray(L"\u25BD")));
    QFont font3 = ui->Player1DOWN_Button->font();
    font3.setPointSize(20);
    ui->Player1DOWN_Button->setFont(font3);

    ui->Player2UP_Button->setText((QString::fromWCharArray(L"\u25B3")));
    QFont font4 = ui->Player2UP_Button->font();
    font4.setPointSize(20);
    ui->Player2UP_Button->setFont(font4);

    ui->Player2DOWN_Button->setText((QString::fromWCharArray(L"\u25BD")));
    QFont font5 = ui->Player2DOWN_Button->font();
    font5.setPointSize(20);
    ui->Player2DOWN_Button->setFont(font5);

    this->setFixedSize(this->size());
    this->setWindowTitle(Window_Name.c_str());
    ui->StopMusic_Button->setVisible(false);
    ui->Warning_Label->setVisible(false);

    //Makes new QNetworkAccessMangager and parents to this
    manager = new QNetworkAccessManager(this);
    //Connect to replyFinished QnetworkReply
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    //Get From the URL
    manager->get(QNetworkRequest(QUrl("https://obsproject.com/forum/resources/scoreboard-windows-mac.150/")));
}

void ScoreboardMain::replyFinished(QNetworkReply *reply)
{
    string getstring = "";
    size_t str;
    int a = 0;
    bool updateyes = false;
    //Check if there is a error
    if(reply->error())
    {
        ui->Testing->setText("Error: 0001, Not able to check for updates!");
    }
    else
    {
        ofstream textout;
        textout.open(".\\Update\\out.txt");
        //Read all from the url and writes to a file
        textout << reply->readAll().toStdString();
        textout.close();
        ifstream textin;
        textin.open(".\\Update\\out.txt");
        if(textin.is_open())
        {
            while(getline(textin, getstring))
            {
                ui->Testing->setStyleSheet("QLabel{color: rgb(0, 0, 0);}");
                str = getstring.find("Version_Info:0.9");
                if(str!=string::npos)
                {
                    ui->Testing->setText("Status: No Update");
                    return;
                    break;
                    updateyes = true;
                }
                a++;
            }
            if(a > getstring.size() && updateyes == false)
            {
                ui->Testing->setStyleSheet("QLabel{color: rgb(255, 0, 0);}");
                ui->Testing->setText("Status: New Update Found! Please Update This Program!");
                WarningBox();
            }
        }
        textin.close();
    }
    reply->deleteLater();
}
int stopreply = 0;

void ScoreboardMain::WarningBox()
{
    if(stopreply == 0)
    {
       QMessageBox::warning(this,"New Update Found", "<a href=https://obsproject.com/forum/resources/scoreboard-windows-mac.150>Update Download</a>");
       stopreply = 1;
    }
    else
    {
    }
}

void ScoreboardMain::Changed() //Changed Score,etc
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_Score_Output.open(".\\Output\\Player1_Score.txt");
        Player2_Score_Output.open(".\\Output\\Player2_Score.txt");

        Player1_Score_Output << Player1_Score;
        Player2_Score_Output << Player2_Score;

        Player1_Score_Output.close();
        Player2_Score_Output.close();
    }
    di->Player1_Score(Player1_Score);
    di->Player2_Score(Player2_Score);

    ui->Player1Score_Label->setText(QString::number(Player1_Score));
    ui->Player2Score_Label->setText(QString::number(Player2_Score));
}

void ScoreboardMain::on_Player1UP_Button_clicked() //Player1_Up Button
{
    Player1_Score++;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Player1_Score:" + QString::number(Player1_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_Player1DOWN_Button_clicked() //Player1_Down Button
{
    Player1_Score--;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Player1_Score:" + QString::number(Player1_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_Player2UP_Button_clicked() //Player2_Up Button
{
    Player2_Score++;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Player2_Score:" + QString::number(Player2_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_Player2DOWN_Button_clicked() //Player2_Down Button
{
    Player2_Score--;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Player2_Score:" + QString::number(Player2_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_Update_Team_Button_clicked() //Update Team Name Button
{
    QString Player1N = ui->Player1Name_Input->text(), Player2N = ui->Player2Name_Input->text();
    QString Player1I = ui->Player1Id_Input->currentText(), Player2I = ui->Player2Id_Input->currentText();
    Player1_Name = Player1N.toUtf8().constData(), Player2_Name = Player2N.toUtf8().constData();
    Player1_Id = Player1I.toUtf8().constData(), Player2_Id = Player2I.toUtf8().constData();
    if(ui->checkBox->isChecked())
    {
        writexml();
    }else{
    Player1_Name_Output.open(".\\Output\\Player1_Name.txt");
    Player2_Name_Output.open(".\\Output\\Player2_Name.txt");
    Player1_Id_Output.open(".\\Output\\Player1_Id.txt");
    Player2_Id_Output.open(".\\Output\\Player2_Id.txt");
    Player1_Name_Output << Player1_Name;
    Player2_Name_Output << Player2_Name;
    Player1_Id_Output << Player1_Id;
    Player2_Id_Output << Player2_Id;
    Player1_Name_Output.close();
    Player2_Name_Output.close();
    Player1_Id_Output.close();
    Player2_Id_Output.close();
    }

    if (QFile::exists(".\\Output\\Player1_Id.png"))
    {
        QFile::remove(".\\Output\\Player1_Id.png");
    }
    QString p1_id=QString::fromStdString(Player1_Id.substr(0,2));
    QString p1_idfile=".\\ID_Pictures\\" ;
    p1_idfile.append(p1_id);
    p1_idfile.append(".png");
    QFile::copy(p1_idfile,".\\Output\\Player1_Id.png");

    if (QFile::exists(".\\Output\\Player2_Id.png"))
    {
        QFile::remove(".\\Output\\Player2_Id.png");
    }
    QString p2_id=QString::fromStdString(Player2_Id.substr(0,2));
    QString p2_idfile=".\\ID_pictures\\" ;
    p2_idfile.append(p2_id);
    p2_idfile.append(".png");
    QFile::copy(p2_idfile,".\\Output\\Player2_Id.png");

    di->Player1_Name(QString::fromStdString(Player1_Name));
    di->Player2_Name(QString::fromStdString(Player2_Name));
    //di->Player1_Id(QString::fromStdString(Player1_Id));
    //di->Player2_Id(QString::fromStdString(Player2_Id));
}

void ScoreboardMain::on_Reset_Score_Button_clicked() //Reset Score Button
{
    Player1_Score = 0;
    Player2_Score = 0;
    ScoreboardMain::Changed();
}

void ScoreboardMain::on_Alway_on_top_Checkbox_clicked(bool alwayson) //Always on top Checkbox
{
    Qt::WindowFlags flags = windowFlags();
    if (alwayson)
    {
        flags ^= Qt::WindowStaysOnBottomHint;
        flags |= Qt::WindowStaysOnTopHint;
    }
    else
    {
        flags ^= Qt::WindowStaysOnTopHint;
        flags |= Qt::WindowStaysOnBottomHint;
    }
    setWindowFlags(flags);
    show();
}

void ScoreboardMain::on_Start_Button_clicked() //Start button
{
    Clock_button++;
    if(minu > 0 && many == false && Stopwatch_input == false){
    speed = Normal_Speed;
    clock_symbol = ":";
    }
    /*if(countu == true && milli == false)
    {
        if(seco == 0)
        {
            seco = -1;
        }else{
    seco--;
    }
        }*/
    if(minu == 0 && Milliseconds == true && Stopwatch_input == false)
    {
        many = true;
        speed = Millisecond_Speed;
        minu = seco;
        minu--;
        seco = 10;
        seconds_zero = "";
        clock_symbol = ".";
    }else if(minu == 0 && Stopwatch_input == false){
    speed = Normal_Speed;
    clock_symbol = ":";
    many = false;
    }
    if(many == false && Stopwatch_input == true && Milliseconds == true)
    {
    many = false;
    speed = Millisecond_Speed;
    minu = seco;
    clock_symbol = ".";
    }
    if(minu == 0 && many == false && Stopwatch_input == true && Milliseconds == false)
    {
    many = true;
    speed = Normal_Speed;
    clock_symbol = ":";
    if(seco < 9 && many == true)
    {
        seconds_zero = "0";
    }
    if(seco > 9)
    {
    seconds_zero = "";
    }
    }
    if(Clock_button == 2 && Stopwatch_input == false)
    {
        input_stop = false;
        clock->stop();
        ui->Start_Button->setText("Start");
    }
    if(Clock_button == 2 && Stopwatch_input == true)
    {
       input_stop = false;
       clock->stop();
       ui->Start_Button->setText("Start");
    }
    if(Clock_button == 3 && Stopwatch_input == false)
    {
        if(speed == Normal_Speed)
        {
            speed = Normal_Speed;
        }else if(speed == Millisecond_Speed)
        {
            speed = Millisecond_Speed;
        }
        input_stop = true;
        Clock_button = 1;
    }
    if(Clock_button == 3 && Stopwatch_input == true)
    {
     Clock_button = 1;
     if(saved_speed == Normal_Speed)
     {
         speed = Normal_Speed;
     }else if(saved_speed == Millisecond_Speed)
     {
         speed = Millisecond_Speed;
     }
    }
    if(Clock_button == 1 && Stopwatch_input == false)
    {
    input_stop = true;
    ui->Start_Button->setText("Stop");
    ScoreboardMain::Timer_Control();
    }
    if(Clock_button == 1 && Stopwatch_input == true)
    {
    input_stop = true;
    ui->Start_Button->setText("Stop");
    ScoreboardMain::Stopwatch_Control();
    }
}

void ScoreboardMain::Stopwatch_Control() //Stopwatch Control
{
    clock = new QTimer(this);
    connect(clock,SIGNAL(timeout()),this,SLOT(Stopwatch()));
    if(seco < 9 && Milliseconds == false)
    {
        seconds_zero = "0";
    }
    if(seco > 9)
    {
    seconds_zero = "";
    }
       // seco++;
    if(minu > 1 && Milliseconds == true)
    {
        speed = Normal_Speed;
        clock_symbol = ":";
        many = true;
        if(seco < 9)
        {
            seconds_zero = "0";
        }
        if(seco > 9)
        {
        seconds_zero = "";
        }
    }
    clock->start(speed);
}

void ScoreboardMain::Timer_Control() //Timer Control
{
    clock = new QTimer(this);
    connect(clock,SIGNAL(timeout()),this,SLOT(Timer()));
    if(seco < 9 && many == false)
    {
        seconds_zero = "0";
    }
    if(seco > 9)
    {
    seconds_zero = "";
    }
    if(minu < 9 && many == false && Minute_Zero == true)
    {
        minutes_zero = "0";
    }
    if(minu > 9 && Minute_Zero == true)
    {
    minutes_zero = "";
    }
    if(minu == -1)
    {
        minu++;
    }
    //seco++; //the problem need to be fixed!
    clock->start(speed);
}

void ScoreboardMain::Stopwatch() //Stopwatch
{
    if(minu == 59 && seco == 9 && Milliseconds == true)
    {
        speed = Normal_Speed;
        minu = 1;
        many = true;
        seco = 0;
        Milliseconds = false;
        seconds_zero = "0";
        clock_symbol = ":";
        clock->start(speed);
    }
    if(ui->Minutes_Input->value() == 0 && many == true)
    {
        if(seco == 0)
        {
            seconds_zero = "0";

        many = true;
        clock_symbol = ":";
        speed = Normal_Speed;
    }
    }
    if(minu < 10 && Minute_Zero == true)
    {
        minutes_zero = "0";
    }else
    {
        minutes_zero = "";
    }
    ui->Clock_Label->setText("  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
    if(ui->checkBox->isChecked())
    {
        Clock_text = QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco);
        writexml();
    }
    Clock_Output.open(".\\Output\\Clock.txt");
    Clock_Output << minutes_zero << minu << clock_symbol << seconds_zero << seco;
    Clock_Output.close();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Clock:  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco)).toUtf8());
    }
    if(getinput == true)
    {
        ui->Minutes_Input->setValue(minu);
        ui->Seconds_Input->setValue(seco);
    }
    if(seco < 9 && Milliseconds == false)
    {
        seconds_zero = "0";
    }else if(seco > 8 && Milliseconds == false){
        seconds_zero = "";
    }
    if(minu == mins && seco == secs && to_switch == true)
    {
        ScoreboardMain::on_Start_Button_clicked();
        if(!ui->BrowseFile_Input->text().isEmpty())
        {
        player->play();
        ui->StopMusic_Button->setVisible(true);
        ui->TestSound_Button->setText("Stop Sound");
        }
    }
    if(Milliseconds == true && many == false && seco == 9)
    {
    seco = 0;
    minu++;
    seconds_zero = "";
    }
    if(seco == 59)
    {
        seco = -1;
        seconds_zero = "0";
        clock_symbol = ":";
        minu++;
    }
    seco++;
}

void ScoreboardMain::Timer() //Timer
{
    //seco--;
    if(seco > 11)
    {
    seconds_zero = "";
    }
 ui->Clock_Label->setText("  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
 di->Clock(QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
 if(ui->checkBox->isChecked())
 {
     Clock_text = QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco);
     writexml();
 }
 Clock_Output.open(".\\Output\\Clock.txt");
 Clock_Output << minutes_zero << minu << clock_symbol << seconds_zero << seco;
 Clock_Output.close();
 if(ui->pushButton->text() == "Disable")
 {
     client->write(QString("Clock:  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco)).toUtf8());
 }
 if(getinput == true)
 {
     ui->Minutes_Input->setValue(minu);
     ui->Seconds_Input->setValue(seco);

 }
 if(minu == 1 && Milliseconds == true && seco == 0 && many == false)
 {
     many = true;
     seco = 10;
     minu = 59;
     speed = Millisecond_Speed;
     seconds_zero = "";
     clock_symbol = ".";
     clock->start(speed);
 }
 if(seco == 0 && many == false)
 {
   seco = 60;
   minu--;
 }
 if(minu < 10 && many == 0 && Minute_Zero == true)
 {
     minutes_zero = "0";
 }else
 {
     minutes_zero = "";
 }
 if(minu == mins && seco == secs && to_switch == true)
 {
     ScoreboardMain::on_Start_Button_clicked();
     if(!ui->BrowseFile_Input->text().isEmpty())
     {
     player->play();
     ui->StopMusic_Button->setVisible(true);
     ui->TestSound_Button->setText("Stop Sound");
     }
     }
 if(seco < 11 && many == false)
 {
     seconds_zero = "0";
 }
 if(minu < 10 && many == 0 && Minute_Zero == true)
 {
     minutes_zero = "0";
 }else
 {
     minutes_zero = "";
 }
 if(seco == 0 && many == true)
 {
     minu--;
     seco = 10;
 }
 seco--;
 if(minu == -1 && seco == 59 && Milliseconds == false)
 {
     clock->stop();
     ui->Testing->setText("Error Output:");
     ui->Clock_Label->setText("  00:00");
     ui->Start_Button->setText("Start");
     Clock_button = 0;
     minu = 0;
     seco = 0;
     input_stop = false;
     many = 0;
     ui->Seconds_Input->setValue(0);
     ui->Minutes_Input->setValue(0);
     speed = Normal_Speed;
     if(!ui->BrowseFile_Input->text().isEmpty())
     {
     player->play();
     ui->StopMusic_Button->setVisible(true);
     ui->TestSound_Button->setText("Stop Sound");
     }
     clock->stop();
 }
 if(minu == 0 && seco == 0 && Milliseconds == true)
 {
     clock->stop();
     ui->Testing->setText("Error Output:");
     ui->Clock_Label->setText("  00:00");
     ui->Start_Button->setText("Start");
     Clock_button = 0;
     minu = 0;
     seco = 0;
     input_stop = false;
     many = 0;
     ui->Seconds_Input->setValue(0);
     ui->Minutes_Input->setValue(0);
     speed = Normal_Speed;
     if(!ui->BrowseFile_Input->text().isEmpty())
     {
     player->play();
     ui->StopMusic_Button->setVisible(true);
     ui->TestSound_Button->setText("Stop Sound");
     }
     clock->stop();
 }
}

void ScoreboardMain::on_Reset_Button_clicked() //Reset Clock button
{
    if(Clock_button == 1)
    {
        ui->Testing->setText("Error Output: Please stop your timer");
    }else{
    Clock_button = 0,many = 0;
    if(presetbool == true && PresetRadio == 1 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  40:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(40);
        clock_symbol = ":";
        minu = 5;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "40:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "40:00";
            writexml();
        }
        ui->Clock_Label->setText("  40:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  40:00").toUtf8());
        }
    }else if(presetbool == true && PresetRadio == 2 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  65:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(65);
        clock_symbol = ":";
        minu = 10;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "65:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "65:00";
            writexml();
        }
        ui->Clock_Label->setText("  65:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  65:00").toUtf8());
        }
    }else if(presetbool == true && PresetRadio == 3 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  70:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(70);
        clock_symbol = ":";
        minu = 15;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "70:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "70:00";
            writexml();
        }
        ui->Clock_Label->setText("  70:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  70:00").toUtf8());
        }
    }else if(presetbool == true && PresetRadio == 4 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  20:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(20);
        minu = 20;
        seco = 0;
        clock_symbol = ":";
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "20:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "20:00";
            writexml();
        }
        ui->Clock_Label->setText("  20:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  20:00").toUtf8());
        }
    }else{
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        ui->Clock_Label->setText("  00:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(0);
        minu = 0;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        ui->Clock_Label->setText("  00:00");
        clock_symbol = ":";
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  00:00").toUtf8());
        }
    }
    many = false;
    ui->Testing->setText("Error Output:");
}
}

void ScoreboardMain::on_To_Checkbox_clicked(bool to)
{
    to_switch = to;
}

void ScoreboardMain::on_Minutes_Input_valueChanged(int arg0)
{
    if(input_stop == false)
    {
    minu = arg0;
    ifclockinputchanged();
    }
    }

void ScoreboardMain::on_Seconds_Input_valueChanged(int arg1)
{
    if(input_stop == false)
    {
    seco = arg1;
     ifclockinputchanged();
    }
}

void ScoreboardMain::on_Minutes_To_In_valueChanged(int arg2)
{
    mins = arg2;
}

void ScoreboardMain::on_Seconds_To_Input_valueChanged(int arg3)
{
    secs = arg3;
}

void ScoreboardMain::on_Addzero_Checkbox_clicked(bool add)
{
    Minute_Zero = add;
}

void ScoreboardMain::on_CurrentTime_Checkbox_clicked(bool current)
{
    if(current == true)
    {
        currenttime = new QTimer(this);
        connect(currenttime, SIGNAL(timeout()),this, SLOT(timeclock()));
        currenttime->start(1000);
    }else{
        currenttime->stop();
        ui->Clock_Label->setText("  00:00");
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        seconds_zero = "";
        minutes_zero = "";
}
}

void ScoreboardMain::timeclock()
{
    time_t current = time(0);
    tm *local = localtime(&current);
    if(local->tm_sec < 10)
    {
        seconds_zero = "0";
    }else{
        seconds_zero = "";
    }
    if(local->tm_min < 10)
    {
        minutes_zero = "0";
    }else{
        minutes_zero = "";
    }
    ui->Clock_Label->clear();
    ui->Clock_Label->setText(QString::number(local->tm_hour) + ":" + QString::fromStdString(minutes_zero) + QString::number(local->tm_min) + ":" + QString::fromStdString(seconds_zero) + QString::number(local->tm_sec));
    if(ui->checkBox->isChecked())
    {
        Clock_text = QString::number(local->tm_hour) + ":" + QString::fromStdString(minutes_zero) + QString::number(local->tm_min) + ":" + QString::fromStdString(seconds_zero) + QString::number(local->tm_sec);
    }
    Clock_Output.open(".\\Output\\Clock.txt");
    Clock_Output << local->tm_hour << ":" << minutes_zero << local->tm_min << ":" << seconds_zero << local->tm_sec;
    Clock_Output.close();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Clock:" + QString::number(local->tm_hour) + ":" + QString::fromStdString(minutes_zero) + QString::number(local->tm_min) + ":" + QString::fromStdString(seconds_zero) + QString::number(local->tm_sec)).toUtf8());
    }
}

void ScoreboardMain::ifclockinputchanged()
{
    if(seco < 10 && many == false)
    {
        seconds_zero = "0";
    }else{
        seconds_zero = "";
    }
    ui->Clock_Label->setText("  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
    if(ui->checkBox->isChecked())
    {
        Clock_text = QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco);
        writexml();
    }
    Clock_Output.open(".\\Output\\Clock.txt");
    Clock_Output << minutes_zero << minu << clock_symbol << seconds_zero << seco;
    Clock_Output.close();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Clock:  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco)).toUtf8());
    }
}

void ScoreboardMain::on_Stopwatch_Checkbox_clicked(bool stop)
{
    Stopwatch_input = stop;
}

void ScoreboardMain::on_Milliseconds_Checkbox_clicked(bool milli)
{
    Milliseconds = milli;
}

void ScoreboardMain::on_Countdown_Checkbox_clicked(bool countdown)
{
    getinput = countdown;
}

void ScoreboardMain::on_Hotkey_Checkbox_clicked(bool Hotkey1)
{
    const int speed2 = 300;
        Hotkey = Hotkey1;
        if(Hotkey == false)
        {
            hotkey->stop();
        }
    hotkey = new QTimer(this);
    connect(hotkey,SIGNAL(timeout()),this,SLOT(hotkeys()));
    hotkey->start(speed2);
}

void ScoreboardMain::hotkeys()
{
    if(GetAsyncKeyState(VK_LCONTROL) && Hotkey == true)
    {
        Player1_Score++;
        ScoreboardMain::Changed();
    }
    if(GetAsyncKeyState(VK_RCONTROL) && Hotkey == true)
    {
        Player2_Score++;
        ScoreboardMain::Changed();
    }
    if(GetAsyncKeyState(VK_LMENU) && Hotkey == true)
    {
        ScoreboardMain::on_Start_Button_clicked();
    }
}

void ScoreboardMain::on_About_ActionBar_triggered()
{
    QMessageBox about;
    about.setText("About");
    about.setInformativeText("                                                                                                                  ");
    about.setInformativeText("This Program is made by: HakanCarlstrom and based upon Scoreboard+ by ha0511kr \n Idea by: XxRocketShotxX \n Adding Soon: \n "
                             "\nIf you have any questions or suggetions please contact me in \n"
                             "Email: hakan.aw.carlstrom at gmail.com \n"
                             "OBS fourm page disscusion \n"
                             "\nThank you for downloading and enjoy Scoreboard for Netrunner");
    about.exec();
}

void ScoreboardMain::on_Hotkey_ActionBar_triggered()
{
    QMessageBox hot;
    hot.setText("Hotkey Help");
    hot.setInformativeText("Player1 score = Left Control \nPlayer2 score = Right Control \nTimer = Left Alt \nConfiguring Hotkey will be added in later versions");
    hot.exec();
}

void ScoreboardMain::on_ChangeLog_ActionBar_triggered()
{
    QMessageBox Changelog;
    Changelog.setText("Changelog:");
    Changelog.setInformativeText("0.9: \n"
                           "- Based on Scoreboard+ 1.8.1Dev but revamped for Netrunner"
                           "Coming Soon:\n"
                           "Pictures for Choosen Id:s"
                           "And More... Please leave a suggestions in the comments");
    Changelog.exec();
}

void ScoreboardMain::on_Usehotkey_Help_clicked()
{
    ScoreboardMain::on_Hotkey_ActionBar_triggered();
}

void ScoreboardMain::on_pushButton_clicked()
{
    if(bol == false)
    {
    //Connects to Acceptconnection if they are connected
    connect(&server, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    //Server is listening to the IP and port
    server.listen(QHostAddress::Any, 1234);
    ui->Status->setStyleSheet("QLabel {color: rgb(225, 227, 0);}");
    ui->Status->setText("Status: Listening");
    ui->pushButton->setText("Disable Listen");
    QMessageBox::warning(this, tr("Warning!"), tr("Please know that remote feature is in beta.\nIt could crash the program."), QMessageBox::Ok);
    bol = true;
    }else if(bol == true){
        server.close();
        server.disconnect();
        if(!client->isOpen())
        {
        client->close();
        }
        ui->pushButton->setText("Enable");
        ui->Status->setStyleSheet("QLabel {color: rgb(255, 0, 0);}");
        ui->Status->setText("Status: Not Connected");
        bol = false;
    }
}

void ScoreboardMain::startRead()
{
    //Loops if server can read string from the cliehnt
    while(client->canReadLine())
    {
    //Reads from the client and put it to QString line
    QString line = QString::fromUtf8(client->readLine()).trimmed();

    if(line == "Player1_Up")
    {
        Player1_Score++;
        Changed();
        //Writes/Sends string/Score to the client
        client->write("Player1_Score:" + QByteArray::number(Player1_Score));
    }
    if(line == "Player2_Up")
    {
        Player2_Score++;
        Changed();
        client->write("Player2_Score:" + QByteArray::number(Player2_Score));
    }
    if(line == "Player1_Down")
    {
        Player1_Score--;
        Changed();
        client->write("Player1_Score:" + QByteArray::number(Player1_Score));
    }
    if(line == "Player2_Down")
    {
        Player2_Score--;
        Changed();
        client->write("Player2_Score:" + QByteArray::number(Player2_Score));
    }

    if(line == "Clock_Button")
    {
        on_Start_Button_clicked();
        client->write("Clock_Button:" + QByteArray::fromStdString(ui->Start_Button->text().toUtf8().constData()));
    }
    if(line.contains("Min:"))
    {
        line.remove("Min:");
        int minui = line.toInt();
        ui->Minutes_Input->setValue(minui);
        minu = minui;
    }
    if(line.contains("Sec:"))
    {
        line.remove("Sec:");
        int secoi = line.toInt();
        ui->Seconds_Input->setValue(secoi);
        seco = secoi;
    }
    if(line.contains("Player1_Name:"))
    {
        line.remove("Player1_Name:");
        ui->Player1Name_Input->setText(line);
        on_Update_Team_Button_clicked();
    }
    if(line.contains("Player2_Name:"))
    {
        line.remove("Player2_Name:");
        ui->Player2Name_Input->setText(line);
        on_Update_Team_Button_clicked();
    }
    if(line == "Reset")
    {
        on_Reset_Button_clicked();
    }
    if(line == "Stop_Watch")
    {
        Stopwatch_input = true;
    }
    if(line == "Stop_Watch_Stop")
    {
        Stopwatch_input = false;
    }
    if(line == "Current_Time")
    {
        on_CurrentTime_Checkbox_clicked(true);
    }
    if(line == "Current_Time_Stop")
    {
        on_CurrentTime_Checkbox_clicked(false);
    }
    if(line == "Reset_Score")
    {
        Player2_Score = 0;
        Player1_Score = 0;
        Changed();
    }
    ui->Status->setText("Client says: " + line);
    }
}


void ScoreboardMain::acceptConnection()
{
    //Connecting to the next pending connection
    client = server.nextPendingConnection();
    //Set status to Connected
    ui->Status->setStyleSheet("QLabel {color: rgb(0, 220, 0);}");
    ui->Status->setText("Status: Connected! From: " + client->peerAddress().toString() + ":" + QString::number(client->peerPort()));
    client->write("Connected to Scoreboard+ \n");
    ui->pushButton->setText("Disable");
    //Connect to the startread to read from the client
    connect(client, SIGNAL(readyRead()),this,SLOT(startRead()));
}

void ScoreboardMain::on_actionSickmind33_triggered()
{
    QMessageBox Sick;
    Sick.setText("About: Sickmind33");
    Sick.setInformativeText("Thank you to Sickmind33 for creating an awesome icon!\nPress Ok to continue");
    Sick.exec();
}

void ScoreboardMain::on_PlaySound_Checkbox_clicked(bool checked10)
{
    if(checked10 == true)
    {
        ui->Browsefile_Button->setEnabled(true);
        ui->BrowseFile_Input->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->TestSound_Button->setEnabled(true);
        ui->SoundSpeed_Slider->setEnabled(true);
    }
    if(ui->TestSound_Button->text() == "Stop Sound")
    {
        ui->PlaySound_Checkbox->setChecked(true);
        ui->Warning_Label->setVisible(true);
    }else{
    if(checked10 == false)
    {
        ui->Warning_Label->setVisible(false);
         ui->TestSound_Button->setDisabled(true);
        ui->horizontalSlider->setDisabled(true);
        ui->Browsefile_Button->setDisabled(true);
        ui->BrowseFile_Input->setDisabled(true);
        ui->BrowseFile_Input->setText("");
        ui->SoundSpeed_Slider->setDisabled(true);
    }
    }
}

void ScoreboardMain::on_Browsefile_Button_clicked()
{
    QString Filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "All files (*.*);; mp3 File (*.mp3)");
    ui->BrowseFile_Input->setText(Filename);
    //player->setMedia(QUrl::fromLocalFile(Filename));
}

void ScoreboardMain::on_BrowseFile_Input_textChanged(const QString &arg1)
{
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(arg1));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(putduration()));
}

void ScoreboardMain::putduration()
{
    ui->Testing->setText(player->metaData(QMediaMetaData::Title).toString());
}

void ScoreboardMain::on_StopMusic_Button_clicked()
{
    player->stop();
    ui->Testing->setText("Error:");
    player->setPosition(0);
    ui->StopMusic_Button->setVisible(false);
    ui->TestSound_Button->setText("Test Sound");
}

void ScoreboardMain::on_horizontalSlider_sliderMoved(int position)
{
    player->setVolume(position);
}



void ScoreboardMain::on_TestSound_Button_clicked()
{
    if(testplayer == false && !(ui->BrowseFile_Input->text().isEmpty()))
    {
        player->play();
        testplayer = true;
        ui->TestSound_Button->setText("Stop Sound");
    }else if(testplayer == true && !(ui->BrowseFile_Input->text().isEmpty())){
        player->stop();
        ui->Testing->setText("Error:");
        testplayer = false;
        ui->TestSound_Button->setText("Test Sound");
        ui->StopMusic_Button->setVisible(false);
    }
}

bool truefalse = false;

void ScoreboardMain::on_Speed_CheckBox_clicked(bool checked11)
{
    if(checked11 == false)
    {
        Normal_Speed = 1000;
    }else if(checked11 == true){
        truefalse = true;
    }
}

void ScoreboardMain::on_SpeedHelp_Button_clicked()
{
    QMessageBox::warning(this,"Speed Help","Use this when your game timer/stopwatch is faster then this program\nDefault: 1000 = 1 sec");
}

void ScoreboardMain::on_Speed_Input_valueChanged(int arg13)
{
    if(truefalse == true)
    {
        Normal_Speed = arg13;
    }else if(truefalse == false){
        Normal_Speed = 1000;
    }
}
void ScoreboardMain::on_TimerPreset_Checkbox_clicked(bool checked15)
{
    if(checked15 == true)
    {
        presetbool = true;
        if(ui->FiveP_Radio->isChecked())
        {
            PresetRadio = 1;
        }
        if(ui->TenP_Radio->isChecked())
        {
            PresetRadio = 2;
        }
        if(ui->FifteenP_Radio->isChecked())
        {
            PresetRadio = 3;
        }
    }else{
        PresetRadio = 0;
        presetbool = false;
    }
}

void ScoreboardMain::on_FiveP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 1;
    }
}

void ScoreboardMain::on_TenP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 2;
    }
}

void ScoreboardMain::on_FifteenP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 3;
    }
}

void ScoreboardMain::on_actionXxRocketshotxX_triggered()
{
    QMessageBox Xx;
    Xx.setText("About: XxRocketshotxX");
    Xx.setInformativeText("Thank you an idea of Scoreboard+!");
    Xx.exec();
}

void ScoreboardMain::xml()
{
    //Creates QFile to the directory
    QFile file(".//Output//Xml.xml");

    //Check if file is writeable
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->Testing->setText("xml Could not be created");
    }else{
    ScoreboardMain::writexml();
    }

}
int a = 0;

void ScoreboardMain::writexml()
{

    a++;
    //Creates a new xml file
    QFile file(".//Output//Xml.xml");
    //Make sure file is only writeonly
    file.open(QIODevice::WriteOnly);
    //Set QXmlStreamWriter to file path to &file
    QXmlStreamWriter xmlWriter(&file);
    //Auto Formatting for xml
    xmlWriter.setAutoFormatting(true);
    //Start document root
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("items");
    xmlWriter.writeTextElement("timestamp",QString::number(a));
    xmlWriter.writeTextElement("Player1Score", QString::number(Player1_Score));
    xmlWriter.writeTextElement("Player2Score", QString::number(Player2_Score));
    xmlWriter.writeTextElement("Player1Name", QString::fromStdString(Player1_Name));
    xmlWriter.writeTextElement("Player2Name", QString::fromStdString(Player2_Name));
    xmlWriter.writeTextElement("Player1Id", QString::fromStdString(Player1_Id));
    xmlWriter.writeTextElement("Player2Id", QString::fromStdString(Player2_Id));
    xmlWriter.writeTextElement("Clock", Clock_text);
    xmlWriter.writeEndElement();
    //End Document
    xmlWriter.writeEndDocument();
    //File Close
    file.close();
}

void ScoreboardMain::on_checkBox_clicked(bool checked20)
{
    if(checked20 == true)
    {
        xml();
    }else{
        QFile::remove(".\\Output\\Xml.xml");
        QFile::remove(".\\Update\\out.txt");
    }
}

void ScoreboardMain::on_actionDonate_triggered()
{
    QMessageBox DonateBox;
    DonateBox.setText("<a href=https://goo.gl/fQD3gI>Donate!</a>");
    DonateBox.setInformativeText("Want to support me? I know this is completely optional\nbut it will definitely help me to keep working on this project!\nI don't care how much remember,every little bit counts!");
    DonateBox.exec();
}

void ScoreboardMain::on_Player1Name_Input_returnPressed()
{
//
}

void ScoreboardMain::on_SoundSpeed_Slider_sliderMoved(int slidepos)
{
    qreal test = (qreal)slidepos;
    if(test < 0.0)
    {
        test = -0.9;
    }
    player->setPlaybackRate(test);
}
