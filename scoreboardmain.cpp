#include "scoreboardmain.h"
#include "ui_scoreboardmain.h"
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
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QStandardPaths>
#include <QTableWidget>
#include <QTextDocument>
#include <QTextTableCell>
#include <QClipboard>
#include <QPrinter>
#include <QPainter>
#include <QImage>
//#include <QImageReader>
#include <QBitmap>

#ifdef Q_OS_WIN
    #include <windows.h> //For Hotkey/Shortcut key
#endif

using namespace std;

static string version_info = "1.06"; //Please Change this after a update!
static QString appName = "Scoreboard for Android:Netrunner & Star Wars:Destiny" ;

static string Update_URL = "https://github.com/purpletechnician/Scoreboard-for-Netrunner";
static string CardDB_URL_ANR = "http://www.netrunnerdb.com/api/2.0/public/card/" ;
static string CardDB_URL_SWD = "http://swdestinydb.com/api/public/card/" ;
static string CardDB_URL = CardDB_URL_ANR ;
static QString CardDB_DecklistURL_ANR = "http://www.netrunnerdb.com/api/2.0/public/decklist/" ;
static QString CardDB_DecklistURL_SWD = "http://swdestinydb.com/api/public/decklist/" ;
static QString CardDB_DecklistURL = CardDB_DecklistURL_ANR ;
static QString BaseCardURL_ANR = "http://www.netrunnerdb.com/card_image/";
static QString BaseCardURL_SWD = "http://swdestinydb.com/bundles/cards/en/"; // "01/" added for first cycle, "02/" for second and so on
static QString BaseCardURL = BaseCardURL_ANR ;
static QString Game_ANR = "ANR" ;
static QString Game_SWD = "SWD" ;
static QString Game = Game_ANR ;
static QString ANRCardDB = "/ANRCardDB/";
static QString SWDCardDB = "/SWDCardDB/";
static QString xxxCardDB = ANRCardDB;
static int cardWidth = 300;
static int cardHeight = 418;
static QString SWDCardFormat = ".json";

static QString saveLocation = QDir::currentPath() ;
static string sLocation = saveLocation.toStdString();
static QString Window_Name = appName +" "+QString::fromStdString(version_info);
static QString QUpdate_URL = QString::fromStdString(Update_URL);
static QString QCardDB_URL = QString::fromStdString(CardDB_URL);

static QString *cardJsonData(new QString) ;

static int Player1_Score = 0, Player2_Score = 0; //Players Score integer
static int minu = 65, seco = 0; //Minutes and Seconds integer
static int mins = 0, secs = 0; //Used for To_input
static int saved_speed = 0; //Stored speed integer
// static int last_speed = 0; //Stored speed integer
static int speed = 1000; //Speed integer | Default = 1000 (1seconds) | Milliseconds = 1000(0.001seconds)
static int Clock_button = 0; //Start/Stop button
static int Normal_Speed = 1000, Millisecond_Speed = 93;
static int PresetRadio = 3;

static bool bol = false; //Open/Close for the serve
static bool many = false; //For milliseconds register
static bool input_stop = false; //Stop getting the number from the input
static bool getinput = false;
static bool testplayer = false; //Test player button
static bool to_switch; //On/Off for To_input
static bool Milliseconds = false, Minute_Zero = false, Hotkey = false, Stopwatch_input = false; //On/Off for Milliseconds, Add zero to minute, Hotkey, Stopwatch
static bool presetbool = true; //Preset timer
static bool updateyes = false; // For new version of Scoreboard

static string minutes_zero = "", seconds_zero = ""; //For 9 <-> 0 | Example: 09 05
static string Player1_Name = "", Player2_Name = ""; //Name for Player1, Player2
static string Player1_Id = "", Player2_Id = ""; //Id for Player1, Player2
static string Round = "", Round_info = "", Custom = "", NR_text="", NRS_text="";
static string clock_symbol = ":"; //Clock Symbol | Default = : | Milliseconds = .
static QString Clock_text = "65:00"; //Clock Text
static QString choosenCard, choosenName;
static int Max_healthP1C1, Max_healthP1C2, Max_healthP1C3, Max_healthP1C4, Max_healthP1C5;
static int Max_healthP2C1, Max_healthP2C2, Max_healthP2C3, Max_healthP2C4, Max_healthP2C5;

static QList<QString> IdList;
static QList<QString> Id1SWDList, Id2SWDList;

struct FactionColorCode {
    QString Faction;
    QString Faction_rich ;
    QString StartColor;
    QString EndColor;
    QLinearGradient FactionGradient ;
};

static QList<FactionColorCode> FactionColorCodes;

struct Card_info {
    QString Title;
    QString Code;
    QString Image_url ;
    QString Faction ;
};
static QList<Card_info> Card_infoList ;

struct CardSWD_info {
    QString Name;
    QString Code;
    QString Image_src ;
    int Health ;
};
static QList<CardSWD_info> CardSWD_infoList ;

struct Deck_info {
    QString Card;
    QString Number;
};
static QList<Deck_info> Deck_infoList ;

struct Pack_info { // For ANR
    int Pack_code;
    int Total_cards;
};
static QList<Pack_info> Pack_infoList;

struct Set_info { // For SWD
    int Set_code;
    int Total_cards;
};
static QList<Set_info> Set_infoList;

static QList<QString> DeckList;

static QLinearGradient linearGrad;

static int row = 0 ;
static QString strFile ;

//Ofstream for outputting to .txt
static ofstream Player1_Name_Output, Player2_Name_Output, Player1_Id_Output, Player2_Id_Output, Player1_Score_Output, Player2_Score_Output ;
static ofstream Player1_Id2_Output, Player2_Id2_Output, Player1_Id3_Output, Player2_Id3_Output, Player1_Id4_Output, Player2_Id4_Output, Player1_Id5_Output, Player2_Id5_Output ;
static ofstream Period_Output, Clock_Output;
static ofstream Round_Output, Round_info_Output, NR_Output, NRS_Output, UA_Output;
static ofstream Player1_char1Health_Output, Player1_char2Health_Output, Player1_char3Health_Output, Player1_char4Health_Output, Player1_char5Health_Output;
static ofstream Player2_char1Health_Output, Player2_char2Health_Output, Player2_char3Health_Output, Player2_char4Health_Output, Player2_char5Health_Output;
//QFile Player1_Id_HTML, Player2_Id_HTML;

static ofstream CardDB ;

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
    connect(ui->search_Input, SIGNAL(textChanged(const QString &)), this, SLOT(searchChanged(const QString &)));

    qDebug() << saveLocation ;
    QDir oDir(saveLocation+"/Output/");
    if (!oDir.exists())
    {
        oDir.mkpath(saveLocation+"/Output/");
        qDebug() << saveLocation+"/Output/";
    }
    QDir cANRDir(saveLocation+ANRCardDB);
    if (!cANRDir.exists())
    {
        cANRDir.mkpath(saveLocation+ANRCardDB);
        qDebug() << saveLocation+ANRCardDB;
    }
    QDir cSWDDir(saveLocation+SWDCardDB);
    if (!cSWDDir.exists())
    {
        cSWDDir.mkpath(saveLocation+SWDCardDB);
        qDebug() << saveLocation+SWDCardDB;
    }
    QDir uDir(saveLocation+"/Update/");
    if (!uDir.exists())
    {
        uDir.mkpath(saveLocation+"/Update/");
    }

    writexml();
    Player1_Name_Output.open(sLocation+"/Output/Player1_Name.txt");
    Player2_Name_Output.open(sLocation+"/Output/Player2_Name.txt");
    Player1_Id_Output.open(sLocation+"/Output/Player1_Id.txt");
    Player1_Id2_Output.open(sLocation+"/Output/Player1_Id2.txt");
    Player1_Id3_Output.open(sLocation+"/Output/Player1_Id3.txt");
    Player1_Id4_Output.open(sLocation+"/Output/Player1_Id4.txt");
    Player1_Id5_Output.open(sLocation+"/Output/Player1_Id5.txt");
    Player2_Id_Output.open(sLocation+"/Output/Player2_Id.txt");
    Player2_Id2_Output.open(sLocation+"/Output/Player2_Id2.txt");
    Player2_Id3_Output.open(sLocation+"/Output/Player2_Id3.txt");
    Player2_Id4_Output.open(sLocation+"/Output/Player2_Id4.txt");
    Player2_Id5_Output.open(sLocation+"/Output/Player2_Id5.txt");
    Player1_Score_Output.open(sLocation+"/Output/Player1_Score.txt");
    Player2_Score_Output.open(sLocation+"/Output/Player2_Score.txt");
    Player1_char1Health_Output.open(sLocation+"/Output/Player1_char1Health.txt");
    Player1_char2Health_Output.open(sLocation+"/Output/Player1_char2Health.txt");
    Player1_char3Health_Output.open(sLocation+"/Output/Player1_char3Health.txt");
    Player1_char4Health_Output.open(sLocation+"/Output/Player1_char4Health.txt");
    Player1_char5Health_Output.open(sLocation+"/Output/Player1_char5Health.txt");
    Player2_char1Health_Output.open(sLocation+"/Output/Player2_char1Health.txt");
    Player2_char2Health_Output.open(sLocation+"/Output/Player2_char2Health.txt");
    Player2_char3Health_Output.open(sLocation+"/Output/Player2_char3Health.txt");
    Player2_char4Health_Output.open(sLocation+"/Output/Player2_char4Health.txt");
    Player2_char5Health_Output.open(sLocation+"/Output/Player2_char5Health.txt");

    Clock_Output.open(sLocation+"/Output/Clock.txt");
    Round_Output.open(sLocation+"/Output/Round.txt");
    NR_Output.open(sLocation+"/Output/Next_round.txt");
    NRS_Output.open(sLocation+"/Output/Next_round_start.txt");
    UA_Output.open(sLocation+"/Output/Unattended.txt");

    Player1_Id_Output << "";
    Player2_Id_Output << "";
    Player1_Name_Output << "";
    Player2_Name_Output << "";
    UA_Output << "";

    if (ui->NoScoreIdOutput->isChecked())
    {
         Player1_Score_Output << "";
         Player2_Score_Output << "";
         Player1_Id_Output << "";
         Player1_Id2_Output << "";
         Player1_Id3_Output << "";
         Player1_Id4_Output << "";
         Player1_Id5_Output << "";
         Player2_Id_Output << "";
         Player2_Id2_Output << "";
         Player2_Id3_Output << "";
         Player2_Id4_Output << "";
         Player2_Id5_Output << "";
         Player1_char1Health_Output << "";
         Player1_char2Health_Output << "";
         Player1_char3Health_Output << "";
         Player1_char4Health_Output << "";
         Player1_char5Health_Output << "";
         Player2_char1Health_Output << "";
         Player2_char2Health_Output << "";
         Player2_char3Health_Output << "";
         Player2_char4Health_Output << "";
         Player2_char5Health_Output << "";
     }
     else
     {
         Player1_Score_Output << Player1_Score;
         Player2_Score_Output << Player2_Score;
     }

    QFile::remove(saveLocation+"/Output/Player1_Id1.jpg");
    QFile::remove(saveLocation+"/Output/Player1_Id2.jpg");
    QFile::remove(saveLocation+"/Output/Player1_Id3.jpg");
    QFile::remove(saveLocation+"/Output/Player1_Id4.jpg");
    QFile::remove(saveLocation+"/Output/Player1_Id5.jpg");
    QFile::remove(saveLocation+"/Output/Player2_Id1.jpg");
    QFile::remove(saveLocation+"/Output/Player2_Id2.jpg");
    QFile::remove(saveLocation+"/Output/Player2_Id3.jpg");
    QFile::remove(saveLocation+"/Output/Player2_Id4.jpg");
    QFile::remove(saveLocation+"/Output/Player2_Id5.jpg");

    Clock_Output << "00:00";
    Round_Output << "";
    NR_Output << "";
    NRS_Output << "";
    UA_Output << "";

    if (QFile::exists(saveLocation+"/Output/Background.jpg"))
    {
        QFile::remove(saveLocation+"/Output/Background.jpg");
    }
    QFile::copy(saveLocation+"/ID_Pictures/ANRBackground.jpg",saveLocation+"/Output/Background.jpg");

    Player1_Name_Output.close();
    Player2_Name_Output.close();
    Player1_Id_Output.close();
    Player1_Id2_Output.close();
    Player1_Id3_Output.close();
    Player1_Id4_Output.close();
    Player1_Id5_Output.close();
    Player2_Id_Output.close();
    Player2_Id2_Output.close();
    Player2_Id3_Output.close();
    Player2_Id4_Output.close();
    Player2_Id5_Output.close();
    Player1_Score_Output.close();
    Player2_Score_Output.close();

    Player1_char1Health_Output.close();
    Player1_char2Health_Output.close();
    Player1_char3Health_Output.close();
    Player1_char4Health_Output.close();
    Player1_char5Health_Output.close();
    Player2_char1Health_Output.close();
    Player2_char2Health_Output.close();
    Player2_char3Health_Output.close();
    Player2_char4Health_Output.close();
    Player2_char5Health_Output.close();

    Clock_Output.close();
    Round_Output.close();
    NR_Output.close();
    NRS_Output.close();
    UA_Output.close();

    createColors();

    addANR_Ids();
    ui->Player1Id_Input->addItems(IdList);
    ui->Player2Id_Input->addItems(IdList);

    Id1SWDList.append("n1"); Id2SWDList.append("n1");
    Id1SWDList.append("e1"); Id2SWDList.append("e1");
    Id1SWDList.append("n2"); Id2SWDList.append("n2");
    Id1SWDList.append("e2"); Id2SWDList.append("e2");
    Id1SWDList.append("n3"); Id2SWDList.append("n3");
    Id1SWDList.append("e3"); Id2SWDList.append("e3");
    Id1SWDList.append("n4"); Id2SWDList.append("n4");
    Id1SWDList.append("e4"); Id2SWDList.append("e4");
    Id1SWDList.append("n5"); Id2SWDList.append("n5");
    Id1SWDList.append("e5"); Id2SWDList.append("e5");
    ui->comboBoxChooseCharP1->addItems(Id1SWDList);
    ui->comboBoxChooseCharP2->addItems(Id1SWDList);

    loadDecks();

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

    this->setWindowTitle(Window_Name);
    ui->StopMusic_Button->setVisible(false);
    ui->Warning_Label->setVisible(false);

    ui->Player1Id2_Input->setVisible(false);
    ui->Player1Id3_Input->setVisible(false);
    ui->Player1Id4_Input->setVisible(false);
    ui->Player1Id5_Input->setVisible(false);
    ui->Player2Id2_Input->setVisible(false);
    ui->Player2Id3_Input->setVisible(false);
    ui->Player2Id4_Input->setVisible(false);
    ui->Player2Id5_Input->setVisible(false);
    ui->spinBox1P1->setVisible(false);
    ui->spinBox2P1->setVisible(false);
    ui->spinBox3P1->setVisible(false);
    ui->spinBox4P1->setVisible(false);
    ui->spinBox5P1->setVisible(false);
    ui->spinBox1P2->setVisible(false);
    ui->spinBox2P2->setVisible(false);
    ui->spinBox3P2->setVisible(false);
    ui->spinBox4P2->setVisible(false);
    ui->spinBox5P2->setVisible(false);
    ui->comboBoxChooseCharP1->setVisible(false);
    ui->comboBoxChooseCharP2->setVisible(false);
    ui->ChooseCharP1->setVisible(false);
    ui->ChooseCharP2->setVisible(false);

    ui->search_Input->setFocus();

    ui->Start_Button->setStyleSheet("* { background-color: rgba(0,255,0, 1.0) }");

    if (Game == Game_ANR)
    {
        if (QFile::exists(saveLocation+ANRCardDB+"Cards_ANR.bin"))
        {
            ui->downloadCards_Label->setText("ANRCards-file downloaded");
        }
        if (QFile::exists(saveLocation+ANRCardDB+"01001.png"))
        {
            ui->saveCards_Label->setText("1+ cards already downloaded");
        }
    }

    if (Game == Game_SWD)
    {
        if (QFile::exists(saveLocation+SWDCardDB+"Cards_SWD.bin"))
        {
            ui->downloadCards_Label->setText("SWDCards-file already downloaded");
        }
    }

    //Create new QNetworkAccessManager and parents to this
    managerOne = new QNetworkAccessManager(this);
    managerTwo = new QNetworkAccessManager(this);
    managerThree = new QNetworkAccessManager(this) ;
    managerFour = new QNetworkAccessManager(this) ;

    //Connect first to replyFinished QnetworkReply
    connect(managerOne,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    managerOne->get(QNetworkRequest(QUrl(QUpdate_URL)));


    if (QFile::exists(saveLocation+ANRCardDB+"Cards_ANR.bin"))
    {
        QFile file(saveLocation+ANRCardDB+"Cards_ANR.bin");
        if (!file.open(QIODevice::ReadOnly))
            qDebug() << "Cannot open file Cards_ANR.bin for read";
        QDataStream stream(&file);
        stream >> Card_infoList ;
        file.close();
    }

    if (QFile::exists(saveLocation+SWDCardDB+"Cards_SWD.bin"))
    {
        QFile file(saveLocation+SWDCardDB+"Cards_SWD.bin");
        if (!file.open(QIODevice::ReadOnly))
            qDebug() << "Cannot open file Cards_SWD.bin for read";
        QDataStream stream(&file);
        stream >> CardSWD_infoList ;
        file.close();
    }

     //ui->tableDeck->setRowCount(2);
     //ui->tableDeck->setColumnCount(2);
     //ui->tableDeck->horizontalHeader()->setVisible(false);
     ui->tableDeck->verticalHeader()->setVisible(true);
     ui->tableDeck->verticalHeader()->setSectionsMovable(true);
     //ui->tableDeck->horizontalHeader()->resizeSection(0, 188);
     ui->tableDeck->horizontalHeader()->resizeSection(0, 168);
     ui->tableDeck->horizontalHeader()->resizeSection(1, 20);
     ui->tableDeck->verticalHeader()->setDefaultSectionSize(20);

     QFontDatabase::addApplicationFont(":/fonts/netrunner.ttf");
     QFontDatabase::addApplicationFont(":/fonts/Monkirta Pursuit NC.ttf");

     QFont fontNetrunner = QFont("netrunner",7,1);
     QFont fontMonkirta = QFont("Monkirta Pursuit NC",10, 1);
     //ui->tableDeck->setFont(fontMonkirta);
     ui->Player1Id_Input->setFont(fontNetrunner);
     ui->Player2Id_Input->setFont(fontNetrunner);
     //qDebug() << ui->fontComboBoxDecklist->currentFont() ;
     connect(ui->fontComboBoxDecklist, SIGNAL(currentFontChanged(QFont)),SLOT(fontComboBoxDecklist_currentChanged()));

     ui->pushImportDeck->setToolTip("Imports deck from these NetrunnerDb-sources:\n* bbCode (not yet implemented)\n* Markdown (not yet implemented)\n* Plaintext (not yet implemented)\n* Jinteki.net (implemented)\n* NetrunnerDb-URL (implemented)\n");
}

void ScoreboardMain::addANR_Ids()
{
    IdList.clear();
    IdList.append("Anarch: Wyvern"); IdList.append("Anarch: Noise"); IdList.append("Anarch: Whizzard");
    IdList.append("Anarch: Quetzal"); IdList.append("Anarch: Edward Kim"); IdList.append("Anarch: MaxX"); IdList.append("Anarch: Valencia Estevez"); IdList.append("Anarch: Null");
    IdList.append("Anarch: Omar Keung"); IdList.append("Anarch: Alice Merchant"); IdList.append("Anarch: Reina Roja"); IdList.append("Anarch: Freedom Khumalo"); IdList.append("Anarch: Nathaniel Gnat Hall");
    IdList.append("Criminal: Boris Syfr Kovac"); IdList.append("Criminal: Andomeda");
    IdList.append("Criminal: Iain Stirling"); IdList.append("Criminal: Ken Express Tenma"); IdList.append("Criminal: Silouette"); IdList.append("Criminal: Leela Patel");
    IdList.append("Criminal: Armand Geist Walker"); IdList.append("Criminal: Laramy Fisk");IdList.append("Criminal: Nero Severn");IdList.append("Criminal: Khan");
    IdList.append("Criminal: Los"); IdList.append("Criminal: Steve Cambridge"); IdList.append("Criminal: Gabriel Santiago"); IdList.append("Criminal: 419"); IdList.append("Criminal: Liza Talking Thunder");
    IdList.append("Criminal: Az McCaffrey");
    IdList.append("Shaper: Jamie Bzzz Micken"); IdList.append("Shaper: Kate Mac McCaffrey");
    IdList.append("Shaper: Rielle Kit Peddler"); IdList.append("Shaper: The Professor"); IdList.append("Shaper: Exile"); IdList.append("Shaper: Nasir Meidan");
    IdList.append("Shaper: Hayley Kaplan") ;IdList.append("Shaper: Jesminder Sareen"); IdList.append("Shaper: Ele Smoke Scovak"); IdList.append("Shaper: Ayla Bios Rahim");
    IdList.append("Shaper: Chaos Theory"); IdList.append("Shaper: Kabonesa Wu"); IdList.append("Shaper: Akiko Nisei");
    IdList.append("Shaper: Lat");
    IdList.append(" Adam"); IdList.append(" Apex"); IdList.append(" Sunny Lebeau");

    IdList.append("HB: Strategic Innovations"); IdList.append("HB: Engineering the Future");
    IdList.append("HB: Cerebral Imaging"); IdList.append("HB: Custom Biotics"); IdList.append("HB: NEXT Design"); IdList.append("HB: The Foundry"); IdList.append("HB: Cybernetics Division");
    IdList.append("HB: Architects of Tomorrow"); IdList.append("HB: Seidr Laboratiries");  IdList.append("HB: Stronger Together"); IdList.append("HB: Asa Group"); IdList.append("HB: Sportsmetal");
    IdList.append("HB: Mirrormorph");
    IdList.append("NBN: Information Dynamics"); IdList.append("NBN: The World is Yours*");
    IdList.append("NBN: Near-Earth Hub"); IdList.append("NBN: Haarpsichord Studios"); IdList.append("NBN: SYNC"); IdList.append("NBN: New Angeles Sol"); IdList.append("NBN: Spark Agency");
    IdList.append("NBN: Harishchandra Ent."); IdList.append("NBN: Controlling the Message"); IdList.append("NBN: Making News"); IdList.append("NBN: Azmari EdTech"); IdList.append("NBN: Acme Consulting");
    IdList.append("Jinteki: Synthetic Systems"); IdList.append("Jinteki: Harmony Medtech"); IdList.append("Jinteki: Nisei Division"); IdList.append("Jinteki: Tennin Institute"); IdList.append("Jinteki: Industrial Genomics");
    IdList.append("Jinteki: Jinteki Biotech"); IdList.append("Jinteki: Chronos Protocol"); IdList.append("Jinteki: Palana Foods"); IdList.append("Jinteki: Potential Unleashed");
    IdList.append("Jinteki: Aginfusion"); IdList.append("Jinteki: Personal Evolution"); IdList.append("Jinteki: Mti Mwekundu"); IdList.append("Jinteki: Saraswati Mnemonics");
    IdList.append("Jinteki: Hyoubu Institute"); IdList.append("Jinteki: Replicating Perfection");
    IdList.append("Weyland: Fringe Applications"); IdList.append("Weyland: Because We Built It"); IdList.append("Weyland: GRNDL");
    IdList.append("Weyland: Blue Sun"); IdList.append("Weyland: Argus Security"); IdList.append("Weyland: Gagarin Deep Space"); IdList.append("Weyland: Titan Transnational");
    IdList.append("Weyland: Builder of Nations"); IdList.append("Weyland: Jemison Astronautics"); IdList.append("Weyland: Skorpios Defence Systems"); IdList.append("Weyland: Building a Better World");
    IdList.append("Weyland: SSO Industries"); IdList.append("Weyland: The Outfit");
}

void ScoreboardMain::fontComboBoxDecklist_currentChanged()
{
    ui->tableDeck->setFont(ui->fontComboBoxDecklist->currentFont());
}

void ScoreboardMain::createColors()
{
    ui->comboFactionColor->addItem("None");

    linearGrad.setStart(QPoint(0,0)); linearGrad.setFinalStop(QPoint(ui->search_Input->width()/2,0));
    linearGrad.setColorAt(0,"#564B8A"); linearGrad.setColorAt(1,"#EEE0F8");
    FactionColorCodes.append({"haas-bioroid", "HB", "#564B8A", "#EEE0F8", linearGrad});
    ui->comboFactionColor->addItem("HB");

    linearGrad.setColorAt(0,"#F2CA50"); linearGrad.setColorAt(1,"#EFE4D0");
    FactionColorCodes.append({"nbn", "NBN", "#F2CA50", "#EFE4D0", linearGrad});
    ui->comboFactionColor->addItem("NBN");

    linearGrad.setColorAt(0,"#8C0712"); linearGrad.setColorAt(1,"#EFE6DF");
    FactionColorCodes.append({"jinteki", "Jinteki", "#8C0712", "#EFE6DF", linearGrad});
    ui->comboFactionColor->addItem("Jinteki");

    linearGrad.setColorAt(0,"#39735E"); linearGrad.setColorAt(1,"#EBF4F0");
    FactionColorCodes.append({"weyland-consortium", "Weyland", "#39735E", "#EBF4F0", linearGrad});
    ui->comboFactionColor->addItem("Weyland");

    linearGrad.setColorAt(0,"#F26D3D"); linearGrad.setColorAt(1,"#EFE6DF");
    FactionColorCodes.append({"anarch", "Anarch", "#F26D3D", "#EFE6DF", linearGrad});
    ui->comboFactionColor->addItem("Anarch");

    linearGrad.setColorAt(0,"#60B54E"); linearGrad.setColorAt(1,"#EBFFEA");
    FactionColorCodes.append({"shaper", "Shaper", "#60B54E", "#EBFFEA", linearGrad});
    ui->comboFactionColor->addItem("Shaper");

    linearGrad.setColorAt(0,"#6093DE"); linearGrad.setColorAt(1,"#DDE7FD");
    FactionColorCodes.append({"criminal", "Criminal", "#6093DE", "#DDE7FD", linearGrad});
    ui->comboFactionColor->addItem("Criminal");

    linearGrad.setColorAt(0,"#B3B7BF"); linearGrad.setColorAt(1,"#EFEFF4");
    FactionColorCodes.append({"neutral-runner", "Neutral", "#B3B7BF", "#EFEFF4", linearGrad});
    ui->comboFactionColor->addItem("Neutral");

    linearGrad.setColorAt(0,"#B3B7BF"); linearGrad.setColorAt(1,"#EFEFF4");
    FactionColorCodes.append({"neutral-corp", "Neutral", "#B3B7BF", "#EFEFF4", linearGrad});

    linearGrad.setColorAt(0,"#B3B7BF"); linearGrad.setColorAt(1,"#EFEFF4");
    FactionColorCodes.append({"adam", "Adam", "#B3B7BF", "#EFEFF4", linearGrad});
    ui->comboFactionColor->addItem("Adam");

    linearGrad.setColorAt(0,"#B3B7BF"); linearGrad.setColorAt(1,"#EFEFF4");
    FactionColorCodes.append({"apex", "Apex", "#B3B7BF", "#EFEFF4", linearGrad});
    ui->comboFactionColor->addItem("Apex");

    linearGrad.setColorAt(0,"#B3B7BF"); linearGrad.setColorAt(1,"#EFEFF4");
    FactionColorCodes.append({"sunny-lebeau", "Sunny Lebeau", "#B3B7BF", "#EFEFF4", linearGrad});
    ui->comboFactionColor->addItem("Sunny Lebeau");

    connect(ui->comboFactionColor, SIGNAL(currentIndexChanged(int)), SLOT(comboFactionColor_currentIndexChanged()));
}

void ScoreboardMain::comboFactionColor_currentIndexChanged()
{
    QPalette pal = palette();
    QString choosenFaction=ui->comboFactionColor->currentText() ;
    foreach(FactionColorCode faction, FactionColorCodes)
    {
        if (faction.Faction_rich == choosenFaction)
        {
            linearGrad.setStart(QPoint(0,0)); linearGrad.setFinalStop(QPoint(ui->LabelFactionColor->width(),ui->LabelFactionColor->height()));
            linearGrad.setColorAt(0,faction.StartColor); linearGrad.setColorAt(1,faction.EndColor);
            pal.setBrush(ui->LabelFactionColor->backgroundRole(), QBrush(linearGrad));
            ui->LabelFactionColor->setAutoFillBackground(true);
            ui->LabelFactionColor->setPalette(pal);
            ui->LabelFactionColor->show();
        }

    }
}

void ScoreboardMain::searchChanged(const QString &newvalue)
{
    if (Game == Game_ANR)
    {
        ui->List_Output->clear();
        bool found_first = false;
        if (newvalue.count()>=3)
        {
            foreach(Card_info data, Card_infoList)
            {
                if (data.Title.toLower().contains(newvalue))
                {
                    QListWidgetItem *newCard = new QListWidgetItem(data.Title+" #"+data.Code);
                    foreach(FactionColorCode faction, FactionColorCodes)
                    {
                        if (faction.Faction == data.Faction)
                        {
                            if (ui->checkUseGradientsDecklists->isChecked())
                                newCard->setBackground(QBrush(faction.FactionGradient));
                        /*if (ui->checkUseImageDecklists->isChecked())
                        {
                            QString image = saveLocation+"/CardDB/"+data.Code+".png";
                            QImage original(image);
                            QRect rect(0,original.height()/5-10, original.width(), original.height()/5+10);
                            //QPixmap cropped = original.copy(rect).scaled(50,50,Qt::KeepAspectRatio);
                            QImage cropped = original.copy(rect);
                            QPainter p;
                            p.begin(&cropped);
                            p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                            p.fillRect(cropped.rect(),QColor(0,0,0,50));
                            p.end();
                            ui->labelTest->setPixmap(QPixmap::fromImage(cropped));
                            newCard->setBackground(QBrush(QImage(cropped)));
                        }*/
                        }
                    }
                    ui->List_Output->addItem(newCard);
                    if (found_first == false)
                    {
                        ui->List_Output->setCurrentRow(0);
                        QListWidgetItem *card = ui->List_Output->currentItem();
                        QString str = card->text();
                        choosenCard = str.mid(str.indexOf("#")+1,str.length());
                    }
                    found_first=true;
                }
            }
        }
    }
    if (Game == Game_SWD)
    {
        ui->List_Output->clear();
        bool found_first = false;
        if (newvalue.count()>=3)
        {
            foreach(CardSWD_info data, CardSWD_infoList)
            {
                if (data.Name.toLower().contains(newvalue))
                {
                    QListWidgetItem *newCard = new QListWidgetItem(data.Name+" #"+data.Code);
                    foreach(FactionColorCode faction, FactionColorCodes)
                    {
                        /*if (faction.Faction == data.Faction)
                        {
                            if (ui->checkUseGradientsDecklists->isChecked())
                                newCard->setBackground(QBrush(faction.FactionGradient));
                            if (ui->checkUseImageDecklists->isChecked())
                            {
                            QString image = saveLocation+"/CardDB/"+data.Code+".png";
                            QImage original(image);
                            QRect rect(0,original.height()/5-10, original.width(), original.height()/5+10);
                            //QPixmap cropped = original.copy(rect).scaled(50,50,Qt::KeepAspectRatio);
                            QImage cropped = original.copy(rect);
                            QPainter p;
                            p.begin(&cropped);
                            p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                            p.fillRect(cropped.rect(),QColor(0,0,0,50));
                            p.end();
                            ui->labelTest->setPixmap(QPixmap::fromImage(cropped));
                            newCard->setBackground(QBrush(QImage(cropped)));
                            }
                        }*/
                    }
                    ui->List_Output->addItem(newCard);
                    if (found_first == false)
                    {
                        ui->List_Output->setCurrentRow(0);
                        QListWidgetItem *card = ui->List_Output->currentItem();
                        QString str = card->text();
                        choosenCard = str.mid(str.indexOf("#")+1,str.length());
                        choosenName = str ;
                    }
                    found_first=true;
                }
            }
        }

    }
}

void ScoreboardMain::on_List_Output_clicked()
{
    QListWidgetItem *card = ui->List_Output->currentItem();
    QString str = card->text();
    choosenCard = str.mid(str.indexOf("#")+1,str.length());
    choosenName = str ;
}

void ScoreboardMain::on_Show_right_clicked()
{
    if (Game == Game_ANR)
    {
        if (QFile::exists(saveLocation+"/Output/Card_right.png"))
        {
            QFile::remove(saveLocation+"/Output/Card_right.png");
        }
        QFile::copy(saveLocation+xxxCardDB+choosenCard+".png",saveLocation+"/Output/Card_right.png");

        if (ui->Show_right->text()=="Show right")
        {
            ui->Show_right->setText("Hide right");
        }
        else
        {
            ui->Show_right->setText("Show right");
            QFile::remove(saveLocation+"/Output/Card_right.png");
            ui->search_Input->setText("");
            ui->search_Input->setFocus();
        }
    }
    if (Game == Game_SWD)
    {
        if (QFile::exists(saveLocation+"/Output/Card_right.jpg"))
        {
            QFile::remove(saveLocation+"/Output/Card_right.jpg");
        }
        QFile::copy(saveLocation+xxxCardDB+choosenCard+".jpg",saveLocation+"/Output/Card_right.jpg");

        if (ui->Show_right->text()=="Show right")
        {
            ui->Show_right->setText("Hide right");
        }
        else
        {
            ui->Show_right->setText("Show right");
            QFile::remove(saveLocation+"/Output/Card_right.jpg");
            ui->search_Input->setText("");
            ui->search_Input->setFocus();
        }
    }

}

void ScoreboardMain::on_Show_left_clicked()
{
    if (Game == Game_ANR)
    {
        if (QFile::exists(saveLocation+"/Output/Card_left.png"))
        {
            QFile::remove(saveLocation+"/Output/Card_left.png");
        }
        QFile::copy(saveLocation+xxxCardDB+choosenCard+".png",saveLocation+"/Output/Card_left.png");

        if (ui->Show_left->text()=="Show left")
        {
            ui->Show_left->setText("Hide left");
        }
        else
        {
            ui->Show_left->setText("Show left");
            QFile::remove(saveLocation+"/Output/Card_left.png");
            ui->search_Input->setText("");
            ui->search_Input->setFocus();
        }
    }
    if (Game == Game_SWD)
    {
        if (QFile::exists(saveLocation+"/Output/Card_left.jpg"))
        {
            QFile::remove(saveLocation+"/Output/Card_left.jpg");
        }
        QFile::copy(saveLocation+xxxCardDB+choosenCard+".jpg",saveLocation+"/Output/Card_left.jpg");

        if (ui->Show_left->text()=="Show left")
        {
            ui->Show_left->setText("Hide left");
        }
        else
        {
            ui->Show_left->setText("Show left");
            QFile::remove(saveLocation+"/Output/Card_left.jpg");
            ui->search_Input->setText("");
            ui->search_Input->setFocus();
        }
    }
}


void ScoreboardMain::on_ChooseCharP1_clicked()
{
    QString characterNumber = ui->comboBoxChooseCharP1->currentText();
    int health;

    foreach(CardSWD_info data, CardSWD_infoList)
    {
        //if (choosenName.toLower().contains(data.Name))
        if (choosenCard==data.Code)
        {
            qDebug()<<"Found card:"<<data.Code;
            health = data.Health;

            qDebug()<<"ChoosenName"<<choosenName<<" Health:"<<health;
        }
    }

    if (characterNumber.right(1) == "1")
    {
            ui->Player1Id_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player1Id_Input->addItem(choosenName);
            Player1_Id_Output.open(sLocation+"/Output/Player1_Id.txt");
            Player1_Id_Output<<choosenName.toStdString();
            Player1_Id_Output.close();
            QFile::remove(saveLocation+"/Output/Player1_Id1.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player1_Id1.jpg");

            ui->spinBox1P1->setValue(health);
            Max_healthP1C1 = health;
            ScoreboardMain::on_spinBox1P1_valueChanged();
    }
    if (characterNumber.right(1) == "2")
    {
            ui->Player1Id2_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player1Id2_Input->addItem(choosenName);
            Player1_Id2_Output.open(sLocation+"/Output/Player1_Id2.txt");
            Player1_Id2_Output<<choosenName.toStdString();
            Player1_Id2_Output.close();
            QFile::remove(saveLocation+"/Output/Player1_Id2.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player1_Id2.jpg");

            ui->spinBox2P1->setValue(health);
            Max_healthP1C2 = health;
            ScoreboardMain::on_spinBox2P1_valueChanged();
    }
    if (characterNumber.right(1) == "3")
    {
            ui->Player1Id3_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player1Id3_Input->addItem(choosenName);
            Player1_Id3_Output.open(sLocation+"/Output/Player1_Id3.txt");
            Player1_Id3_Output<<choosenName.toStdString();
            Player1_Id3_Output.close();
            QFile::remove(saveLocation+"/Output/Player1_Id3.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player1_Id3.jpg");

            ui->spinBox3P1->setValue(health);
            Max_healthP1C3 = health;
            ScoreboardMain::on_spinBox3P1_valueChanged();
    }
    if (characterNumber.right(1) == "4")
    {
            ui->Player1Id4_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player1Id4_Input->addItem(choosenName);
            Player1_Id4_Output.open(sLocation+"/Output/Player1_Id4.txt");
            Player1_Id4_Output<<choosenName.toStdString();
            Player1_Id4_Output.close();
            QFile::remove(saveLocation+"/Output/Player1_Id4.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player1_Id4.jpg");

            ui->spinBox4P1->setValue(health);
            Max_healthP1C4 = health;
            ScoreboardMain::on_spinBox4P1_valueChanged();
    }
    if (characterNumber.right(1) == "5")
    {
            ui->Player1Id5_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player1Id5_Input->addItem(choosenName);
            Player1_Id5_Output.open(sLocation+"/Output/Player1_Id5.txt");
            Player1_Id5_Output<<choosenName.toStdString();
            Player1_Id5_Output.close();
            QFile::remove(saveLocation+"/Output/Player1_Id5.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player1_Id5.jpg");

            ui->spinBox5P1->setValue(health);
            Max_healthP1C5 = health;
            ScoreboardMain::on_spinBox5P1_valueChanged();
    }
    ui->search_Input->setText("");
    ui->search_Input->setFocus();
}

void ScoreboardMain::on_ChooseCharP2_clicked()
{
    QString characterNumber = ui->comboBoxChooseCharP2->currentText();
    int health = 1;

    foreach(CardSWD_info data, CardSWD_infoList)
    {
        //if (choosenName.toLower().contains(data.Name))
        if (choosenCard==data.Code)
        {
            health = (int)data.Health;
            qDebug()<<"ChoosenName"<<choosenName<<" Health:"<<health;
        }
    }

    if (characterNumber.right(1) == "1")
    {
            ui->Player2Id_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player2Id_Input->addItem(choosenName);
            Player2_Id_Output.open(sLocation+"/Output/Player2_Id.txt");
            Player2_Id_Output<<choosenName.toStdString();
            Player2_Id_Output.close();
            QFile::remove(saveLocation+"/Output/Player2_Id1.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player2_Id1.jpg");

            ui->spinBox1P2->setValue(health);
            Max_healthP2C1 = health;
            ScoreboardMain::on_spinBox1P2_valueChanged();
    }
    if (characterNumber.right(1) == "2")
    {
            ui->Player2Id2_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player2Id2_Input->addItem(choosenName);
            Player2_Id2_Output.open(sLocation+"/Output/Player2_Id2.txt");
            Player2_Id2_Output<<choosenName.toStdString();
            Player2_Id2_Output.close();
            QFile::remove(saveLocation+"/Output/Player2_Id2.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player2_Id2.jpg");

            ui->spinBox2P2->setValue(health);
            Max_healthP2C2 = health;
            ScoreboardMain::on_spinBox2P2_valueChanged();
    }
    if (characterNumber.right(1) == "3")
    {
            ui->Player2Id3_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player2Id3_Input->addItem(choosenName);
            Player2_Id3_Output.open(sLocation+"/Output/Player2_Id3.txt");
            Player2_Id3_Output<<choosenName.toStdString();
            Player2_Id3_Output.close();
            QFile::remove(saveLocation+"/Output/Player2_Id3.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player2_Id3.jpg");

            ui->spinBox3P2->setValue(health);
            Max_healthP2C3 = health;
            ScoreboardMain::on_spinBox3P2_valueChanged();
    }
    if (characterNumber.right(1) == "4")
    {
            ui->Player2Id4_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player2Id4_Input->addItem(choosenName);
            Player2_Id4_Output.open(sLocation+"/Output/Player2_Id4.txt");
            Player2_Id4_Output<<choosenName.toStdString();
            Player2_Id4_Output.close();
            QFile::remove(saveLocation+"/Output/Player2_Id4.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player2_Id4.jpg");

            ui->spinBox4P2->setValue(health);
            Max_healthP2C4 = health;
            ScoreboardMain::on_spinBox4P2_valueChanged();
    }
    if (characterNumber.right(1) == "5")
    {
            ui->Player2Id5_Input->clear();
            if (characterNumber.left(1) == "e")
                choosenName = "e"+choosenName;
            ui->Player2Id5_Input->addItem(choosenName);
            Player2_Id5_Output.open(sLocation+"/Output/Player2_Id5.txt");
            Player2_Id5_Output<<choosenName.toStdString();
            Player2_Id5_Output.close();
            QFile::remove(saveLocation+"/Output/Player2_Id5.jpg");
            QFile::copy(saveLocation+xxxCardDB+choosenCard+"_small.jpg",saveLocation+"/Output/Player2_Id5.jpg");

            ui->spinBox5P2->setValue(health);
            Max_healthP2C5 = health;
            ScoreboardMain::on_spinBox5P2_valueChanged();
    }
    ui->search_Input->setText("");
    ui->search_Input->setFocus();
}

void ScoreboardMain::on_addCardToDeck_clicked()
{
    bool found = false ;
    int inc = 1 ;
    QColor endColor ;
    QListWidgetItem *card = ui->List_Output->currentItem();
    if (Game == Game_ANR)
    {
        foreach(FactionColorCode faction, FactionColorCodes)
        {
            if (card->background() == faction.FactionGradient)
            {
                endColor.setNamedColor(faction.EndColor);
            }
        }
    }
    QString textLong = card->text();
    QString text = textLong.mid(0,textLong.indexOf("#")-1);
    QString cardNumber = textLong.mid(textLong.indexOf("#")+1,textLong.length());
    for (int r = 0 ; r < ui->tableDeck->rowCount() ; ++r)
    {
        if (ui->tableDeck->item(r,0))
        {
            if (text == ui->tableDeck->item(r, 0)->text())
            {
                inc =  ui->tableDeck->item(r,1)->text().toInt() + 1 ;
                QString incString = QString::number(inc);
                QTableWidgetItem *widgetItemPlus = new QTableWidgetItem(incString);
                if (Game == Game_ANR)
                {
                    if (ui->checkUseGradientsDecklists->isChecked())
                        widgetItemPlus->setBackground(QBrush(endColor));
                }
                found = true ;

                if (Game == Game_ANR)
                {
                    if (ui->tableDeck->item(r,1)->text().toInt() < 3)
                        ui->tableDeck->setItem(r,1,widgetItemPlus);
                }
                if (Game == Game_SWD)
                {
                    if (ui->tableDeck->item(r,1)->text().toInt() < 2)
                        ui->tableDeck->setItem(r,1,widgetItemPlus);
                }
             }
        }
    }
    if (found == false)
    {
        ui->tableDeck->setRowCount(row+1);
        QTableWidgetItem *widgetItem = new QTableWidgetItem(text) ;
        QTableWidgetItem *widgetItemOne = new QTableWidgetItem("1");
        if (Game == Game_ANR)
        {
            QBrush gradient = card->background() ;
            if (ui->checkUseGradientsDecklists->isChecked())
            {
                widgetItem->setBackground(gradient);
                widgetItemOne->setBackground(QBrush(endColor));
            }
        }
        ui->tableDeck->setItem(row, 0, widgetItem);
        ui->tableDeck->setItem(row,1,widgetItemOne);
        row++;

        int rowTotalHeight=0;
        int count=ui->tableDeck->verticalHeader()->count();
        for (int i = 0; i < count; ++i)
                rowTotalHeight+=ui->tableDeck->verticalHeader()->sectionSize(i);
        rowTotalHeight+=2; // For lines around tablewidget
        ui->tableDeck->setMinimumHeight(rowTotalHeight);
        ui->tableDeck->setMaximumHeight(rowTotalHeight);
    }
}

void ScoreboardMain::on_removeCardFromDeck_clicked()
{
    QList<QTableWidgetItem *> list = ui->tableDeck->selectedItems();
    foreach(QTableWidgetItem *item, list)
    {
        qDebug() << item->text();
        ui->tableDeck->removeRow(item->row());
        row--;
    }
}

void ScoreboardMain::on_saveToDeck_clicked()
{
    QString name = "Deck-"+ui->lineSaveDeck->text();
    ui->tableDeck->clearSelection();
    ui->tableDeck->horizontalHeader()->resizeSection(0, 188);
    ui->tableDeck->verticalHeader()->setVisible(false);
    ui->tableDeck->grab().save(saveLocation+"/Output/"+name+".png");
    ui->comboPlayer1Deck->addItem(name);
    ui->comboPlayer2Deck->addItem(name);
    DeckList.append(name);

    QFile file(saveLocation+"/Output/Decks.bin");
    if (!file.open(QIODevice::WriteOnly))
        qDebug() << "Cannot open file Decks.bin for write/append";
    QDataStream stream(&file);
    stream << DeckList ;
    file.close();
    ui->tableDeck->horizontalHeader()->resizeSection(0, 168);
    ui->tableDeck->verticalHeader()->setVisible(true);
}

void ScoreboardMain::loadDecks()
{
    QFile file(saveLocation+"/Output/Decks.bin");
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "Cannot open file Decks.bin for read";
    else
    {
        QDataStream stream(&file);
        stream >> DeckList ;
        file.close();
        foreach(QString deck, DeckList)
        {
            ui->comboPlayer1Deck->addItem(deck);
            ui->comboPlayer2Deck->addItem(deck);
        }
    }
}

void ScoreboardMain::on_pushClearDeck_clicked()
{
    ui->tableDeck->clear();
    row=0;
}

void ScoreboardMain::on_pushImportDeck_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString originalText = clipboard->text();
    if (originalText.mid(0,3)== "<b>") // bbCode
    {
        qDebug() << "bbCode";
    }
    if (originalText.mid(0,2)== "##") // Markdown (reddit)
    {
        qDebug() << "Markdown";

    }
    if (originalText.mid(0,4)== "http") // Decklist from Netrunnerdb.com
    {
        qDebug() << "Netrunnerdb.com decklist";
        int decklistStartURL = originalText.indexOf("list/", 0)+5 ;
        int decklistEndURL = originalText.indexOf("/", decklistStartURL) ;
        QString CardNumber = originalText.mid(decklistStartURL, decklistEndURL-decklistStartURL);
        //qDebug() << CardNumber ;
        QString CardURL= CardDB_DecklistURL+CardNumber;
        connect(managerThree,SIGNAL(finished(QNetworkReply*)),this,SLOT(getDeckResult(QNetworkReply*)));
        managerThree->get(QNetworkRequest(QUrl(CardURL)));
    }
    else
    {
        QRegExp letter("[a-zA-Z]");  // a letter
        if (letter.exactMatch(originalText.mid(0,1))) // Plaintext
        {
            qDebug() << "Plaintext";

        }

        QRegExp digit("\\d");  // a digit (\d)
        if (digit.exactMatch(originalText.mid(0,1))) // Jinteki.net
        {
            qDebug() << "Jinteki.net";
            int iterate = 0;
            //qDebug() << originalText << " Length:" << originalText.length();
            QString foundLine, foundNumber, foundCard ;
            int foundSpace, endOfLine ;
            QColor endColor ;
            do
            {
                //qDebug() << "Iterate:" << iterate ;
                endOfLine = originalText.indexOf("\n",iterate);
                QString foundLine = originalText.mid(iterate,(endOfLine-iterate));
                //qDebug() << "FoundLine:" <<foundLine ;
                foundSpace = foundLine.indexOf(" ", 0) ;
                //qDebug() << "Space:" << foundSpace ;
                foundNumber = foundLine.mid(0, foundSpace);
                foundCard = foundLine.mid(foundSpace+1, foundLine.length()-foundSpace);
                //qDebug() << "Number" << foundNumber << ":" << foundCard ;

                iterate=endOfLine+1;
                //qDebug() << "NewIterate:" << iterate;

                ui->tableDeck->setRowCount(row+1);
                QTableWidgetItem *widgetCard = new QTableWidgetItem(foundCard) ;
                QTableWidgetItem *widgetNumber = new QTableWidgetItem(foundNumber);
                QString Faction = getFactionFromCardtext(foundCard);
                if (Faction == "Error")
                    qDebug() << "Problem finding card";
                if (Faction != "" && ui->checkUseGradientsDecklists->isChecked())
                {
                        foreach(FactionColorCode faction, FactionColorCodes)
                        {
                            if (faction.Faction.toLower() == Faction)
                            {
                                widgetCard->setBackground(QBrush(faction.FactionGradient));
                                endColor.setNamedColor(faction.EndColor);
                                widgetNumber->setBackground(QBrush(endColor));
                            }
                        }

                }
                ui->tableDeck->setItem(row,0,widgetCard);
                ui->tableDeck->setItem(row,1,widgetNumber);
                row++;

            }
            while (iterate>0); // Reaching EoL = 0

            int rowTotalHeight=0;
            int count=ui->tableDeck->verticalHeader()->count();
            for (int i = 0; i < count; ++i)
                    rowTotalHeight+=ui->tableDeck->verticalHeader()->sectionSize(i);
            rowTotalHeight+=2; // For lines around tablewidget
            ui->tableDeck->setMinimumHeight(rowTotalHeight);
            ui->tableDeck->setMaximumHeight(rowTotalHeight);

        }
    }
}

QString ScoreboardMain::getCardnumberFromCardtext(QString Card)
{
    foreach(Card_info data, Card_infoList)
    {
        if (data.Title.toLower().contains(Card.toLower()))
        {
            return data.Code ;
        }
    }
    return "Error";
}

QString ScoreboardMain::getFactionFromCardtext(QString Card)
{
    foreach(Card_info data, Card_infoList)
    {
        //qDebug() << data.Title ;
        if (data.Title.toLower().contains(Card.toLower()))
        {
            return data.Faction ;
        }
    }
    return "Error" ;
}

QString ScoreboardMain::getCardtextFromCardnumber(QString Card)
{
    foreach(Card_info data, Card_infoList)
    {
        //qDebug() << data.Title ;
        if (data.Code.toInt() == Card.toInt())
        {
            return data.Title ;
        }
    }
    return "Error" ;

}

void ScoreboardMain::on_pushShowPlayer1Deck_clicked()
{
    if (QFile::exists(saveLocation+"/Output/Deck_left.png"))
    {
        QFile::remove(saveLocation+"/Output/Deck_left.png");
    }
    QFile::copy(saveLocation+"/Output/"+ui->comboPlayer1Deck->currentText()+".png",saveLocation+"/Output/Deck_left.png");

    if (ui->pushShowPlayer1Deck->text()=="Show Deck")
    {
        ui->pushShowPlayer1Deck->setText("Hide Deck");
    }
    else
    {
        ui->pushShowPlayer1Deck->setText("Show Deck");
        QFile::remove(saveLocation+"/Output/Deck_left.png");
        ui->search_Input->setText("");
        ui->search_Input->setFocus();
    }
}

void ScoreboardMain::on_pushShowPlayer2Deck_clicked()
{
    if (QFile::exists(saveLocation+"/Output/Deck_right.png"))
    {
        QFile::remove(saveLocation+"/Output/Deck_right.png");
    }
    QFile::copy(saveLocation+"/Output/"+ui->comboPlayer2Deck->currentText()+".png",saveLocation+"/Output/Deck_right.png");

    if (ui->pushShowPlayer2Deck->text()=="Show Deck")
    {
        ui->pushShowPlayer2Deck->setText("Hide Deck");
    }
    else
    {
        ui->pushShowPlayer2Deck->setText("Show Deck");
        QFile::remove(saveLocation+"/Output/Deck_right.png");
        ui->search_Input->setText("");
        ui->search_Input->setFocus();
    }

}

void ScoreboardMain::getCardsResult()
{
    // Connect networkManager response to the handler
    connect(managerTwo, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCardResult(QNetworkReply*)));
    // We get the data, namely JSON file from a site on a particular url
    if (Game == Game_ANR)
    {
        Pack_info pack_info;
        Pack_infoList.clear();
        Card_infoList.clear();
        pack_info = {1,113}; Pack_infoList.append(pack_info); // Core
        pack_info = {2,120}; Pack_infoList.append(pack_info); // Genesis
        pack_info = {3,55}; Pack_infoList.append(pack_info); // Creation&Control
        pack_info = {4,120}; Pack_infoList.append(pack_info); // Spin
        pack_info = {5,55}; Pack_infoList.append(pack_info); // Honor&Profit
        pack_info = {6,120}; Pack_infoList.append(pack_info); // Lunar
        pack_info = {7,55}; Pack_infoList.append(pack_info); // Order&Chaos
        pack_info = {8,120}; Pack_infoList.append(pack_info); // SanSan
        pack_info = {9,55}; Pack_infoList.append(pack_info); // Data&Destiny
        pack_info = {10,114}; Pack_infoList.append(pack_info); // Mumbad
        pack_info = {11,120}; Pack_infoList.append(pack_info); // Flashpoint
        pack_info = {12,120}; Pack_infoList.append(pack_info); // Red Sand
        pack_info = {13,57}; Pack_infoList.append(pack_info); // Terminal Directive
        pack_info = {20,132}; Pack_infoList.append(pack_info); // Revised Core
        pack_info = {21,120}; Pack_infoList.append(pack_info); // Kitara
        pack_info = {22,57}; Pack_infoList.append(pack_info); // Reign and Reverie
        pack_info = {23,101}; Pack_infoList.append(pack_info); // Magnum Opus, just 8 cards but they are numbered between 23001-23101
        pack_info = {24,1}; Pack_infoList.append(pack_info); // NAPD Multiplayer
        pack_info = {25,147}; Pack_infoList.append(pack_info); // System Core 2019
        pack_info = {26,65}; Pack_infoList.append(pack_info); // Downfall
        QList<Pack_info>::iterator p;
        for (p=Pack_infoList.begin(); p != Pack_infoList.end(); p++)
        {
            QString Cardnumber ;
            for (int j=1; j<=(*p).Total_cards;j++)
            {
                QString Cardnumber = QString("%1").arg((*p).Pack_code,2,10,QChar('0'))+ QString("%1").arg(j,3,10,QChar('0')) ;
                QString CardURL = QCardDB_URL+Cardnumber;
                managerTwo->get(QNetworkRequest(QUrl(CardURL)));
            }
        }
    }
    if (Game == Game_SWD)
    {
        //TBD
        Set_info set_info;
        Set_infoList.clear();
        //Card_infoSWDList.clear();
        set_info = {1,174}; Set_infoList.append(set_info); // Awakenings
        set_info = {2,160}; Set_infoList.append(set_info); // Spirit of the Rebellion
        set_info = {3,160}; Set_infoList.append(set_info); // Empire at War
        set_info = {4,46}; Set_infoList.append(set_info); // Two-player starter
        set_info = {5,180}; Set_infoList.append(set_info); // Legacies
        set_info = {6,20}; Set_infoList.append(set_info); // Rivals
        set_info = {7,160}; Set_infoList.append(set_info); // Way of the Force
        set_info = {8,160}; Set_infoList.append(set_info); // Across the Galaxy
        set_info = {9,160}; Set_infoList.append(set_info); // Convergence
        set_info = {10,160}; Set_infoList.append(set_info); // Allies of Necessity
        set_info = {11,160}; Set_infoList.append(set_info); // Spark of Hope
        //set_info = {12,160}; Set_infoList.append(set_info); // Covert Missions
        QList<Set_info>::iterator p;
        for (p=Set_infoList.begin(); p != Set_infoList.end(); p++)
        {
            QString Cardnumber ;
            for (int j=1; j<=(*p).Total_cards;j++)
            {
                QString Cardnumber = QString("%1").arg((*p).Set_code,2,10,QChar('0'))+ QString("%1").arg(j,3,10,QChar('0')) ;
                QString CardURL = QCardDB_URL+Cardnumber+SWDCardFormat;
                qDebug() << "CardURL" << CardURL;
                managerTwo->get(QNetworkRequest(QUrl(CardURL)));
            }
        }
        //qDebug()<<Set_infoList;

    }
}

void ScoreboardMain::on_saveCards_Button_clicked()
{
    if (Game == Game_ANR)
    {
        if (!QFile::exists(saveLocation+ANRCardDB+"Cards_ANR.bin"))
        {
            ui->saveCards_Label->setText("Saving Cards");
            QFile file(saveLocation+ANRCardDB+"Cards_ANR.bin");
            if (!file.open(QIODevice::WriteOnly))
                qDebug() << "Cannot open file for write";
            QDataStream stream(&file);
            stream << Card_infoList ;
            file.close();
            ui->saveCards_Label->setText("Cards saved");
        }
    }
    if (Game == Game_SWD)
    {
        if (!QFile::exists(saveLocation+SWDCardDB+"Cards_SWD.bin"))
        {
            ui->saveCards_Label->setText("Saving Cards");
            QFile file(saveLocation+SWDCardDB+"Cards_SWD.bin");
            if (!file.open(QIODevice::WriteOnly))
                qDebug() << "Cannot open file for write";
            QDataStream stream(&file);
            stream << CardSWD_infoList ;
            file.close();
            ui->saveCards_Label->setText("Cards saved");
        }
    }

    connect(managerFour, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCardURLResult(QNetworkReply*)));
    if (Game == Game_ANR)
    {
        foreach (Card_info data, Card_infoList)
        {
            QString CardURL = data.Image_url;
            qDebug()<<CardURL;
            QString code = data.Code;
            QString faction = data.Faction ;
            qDebug()<<"foreach   Code: "<<code;
            QNetworkReply* reply = managerFour->get(QNetworkRequest(QUrl(CardURL)));
            reply->setProperty("code",code);
        }
    }
    if (Game == Game_SWD)
    {
        //TBD
        foreach (CardSWD_info data, CardSWD_infoList)
        {
            QString CardURL = data.Image_src;
            qDebug()<<CardURL;
            QString code = data.Code;
            double health = data.Health ;
            qDebug()<<"foreach   Code: "<<code;
            QNetworkReply* reply = managerFour->get(QNetworkRequest(QUrl(CardURL)));
            reply->setProperty("code",code);
        }
    }
}

void ScoreboardMain::getCardURLResult(QNetworkReply *replyCard)
{

    if (Game == Game_ANR)
    {
        QString code = replyCard->property("code").toString();
        QString imageName = saveLocation+ANRCardDB+code+".png";
        qDebug()<<"getCardURLResult "<<code;
        QFile file(imageName);
        if (!file.open(QIODevice::ReadWrite))
            qDebug() << "Cannot open file "<<code;
        QByteArray data=replyCard->readAll();
        file.write(data);
        file.close();

        QImage img;
        img.load(imageName, "PNG");
        QPixmap pixmap = QPixmap::fromImage(img);
        qDebug()<<"Saving card: "<<code<<" width: "<<pixmap.width();
        if (pixmap.width() != 300)
        {
            pixmap = QPixmap::fromImage(img.scaled(cardWidth,cardHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
            file.open(QIODevice::WriteOnly);
            pixmap.save(&file,"png",100);
            file.close();
            qDebug() <<"Scaling: "<<code;
            //qDebug()<<"Saving card: "<<code<<" width: "<<img.width();
        }
        ui->saveCards_Label->setText("Saving ANR-card "+code);
    }
    else if (Game == Game_SWD)
    {
        QString code = replyCard->property("code").toString();
        QString imageName = saveLocation+SWDCardDB+code+".jpg" ;
        QFile file(imageName);
        qDebug()<<"getCardURLResult "<<code;
        if (!file.open(QIODevice::ReadWrite))
            qDebug() << "Cannot open file "<<code;
        QByteArray data=replyCard->readAll();
        file.write(data);
        file.close();

        QImage img;
        img.load(imageName, "JPG"); // Or "JPEG"?
        QPixmap pixmap = QPixmap::fromImage(img);
        qDebug()<<"Saving card: "<<code<<" width: "<<pixmap.width();
        if (pixmap.width() != 300)
        {
            pixmap = QPixmap::fromImage(img.scaled(cardWidth,cardHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
            file.open(QIODevice::WriteOnly);
            pixmap.save(&file,"jpg",100);
            file.close();
            qDebug() <<"Scaling: "<<code;
            //qDebug()<<"Saving card: "<<code<<" width: "<<img.width();
        }
        QImage img_orig;
        img_orig.load(imageName, "JPG");
        QImage img_small = img_orig.copy(75,65,150,150);
        QPixmap pixmap_small = QPixmap::fromImage(img_small);

        QBitmap map(150,150);
        map.fill(Qt::color0);
        QPainter painter(&map);
        painter.setBrush(Qt::color1);
        painter.drawRoundedRect(0,0,150,150,75,75);
        pixmap_small.setMask(map);

        QString imageName_small = saveLocation+SWDCardDB+code+"_small.jpg" ;
        QFile file_small(imageName_small);
        file_small.open(QIODevice::WriteOnly);
        pixmap_small.save(&file_small,"jpg",100);
        file_small.close();

        ui->saveCards_Label->setText("Saving SWD-card "+code);
    }
    replyCard->deleteLater();
}

QDataStream &operator<<(QDataStream &stream, const Card_info &data)
{
   stream << data.Title << data.Code << data.Image_url << data.Faction;
   return stream;
}

QDataStream &operator<<(QDataStream &stream, const CardSWD_info &data)
{
    stream << data.Name << data.Code << data.Image_src << data.Health;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Card_info &data)
{
    stream >> data.Title ;
    stream >> data.Code ;
    stream >> data.Image_url ;
    stream >> data.Faction ;
    return stream ;
}

QDataStream &operator>>(QDataStream &stream, CardSWD_info &data)
{
    stream >> data.Name ;
    stream >> data.Code ;
    stream >> data.Image_src ;
    stream >> data.Health ;
    return stream ;
}

void ScoreboardMain::getCardResult(QNetworkReply *replyCard)
{
    if (Game == Game_ANR)
    {
        qDebug()<<"getCardResult ANR";
        QJsonParseError jsonError;
        QString Carddata = replyCard->readAll();
        QJsonDocument CardJson = QJsonDocument::fromJson(Carddata.toUtf8(),&jsonError);
        if (jsonError.error != QJsonParseError::NoError){
            qDebug() << jsonError.errorString();
        }
        QJsonArray CarddataArray = CardJson.object().value("data").toArray();
        foreach (const QJsonValue & v, CarddataArray)
        {
            Card_info temp_Card_info;
            temp_Card_info.Title = v.toObject().value("title").toString();
            temp_Card_info.Code = v.toObject().value("code").toString();
            temp_Card_info.Image_url = v.toObject().value("image_url").toString();
            temp_Card_info.Faction = v.toObject().value("faction_code").toString();
            if (temp_Card_info.Image_url == "")
                temp_Card_info.Image_url = BaseCardURL+temp_Card_info.Code+".png";
            qDebug() << temp_Card_info.Image_url;
            Card_infoList.append(temp_Card_info);
            ui->downloadCards_Label->setText("Downloading data: "+temp_Card_info.Code);
            //qDebug()<<temp_Card_info.Code;
        }
        replyCard->deleteLater();
    }
    if (Game == Game_SWD)
    {
        qDebug()<<"getCardResult SWD"; // TBD
        QJsonParseError jsonError;
        QString Carddata = replyCard->readAll();
        //qDebug()<<Carddata;
        QJsonDocument CardJson = QJsonDocument::fromJson(Carddata.toUtf8(),&jsonError);
        /*if (jsonError.error != QJsonParseError::NoError){
            qDebug() << jsonError.errorString();
        }*/
        QJsonObject CarddataObject = CardJson.object() ;
        //foreach (const QJsonValue & v, CarddataArray)
        //{
            CardSWD_info temp_Card_info;
            temp_Card_info.Name = CarddataObject.value("name").toString();
            temp_Card_info.Code = CarddataObject.value("code").toString();
            qDebug()<<"Name:"<<temp_Card_info.Name<<" Code:"<<temp_Card_info.Code;
            temp_Card_info.Image_src = CarddataObject.value("imagesrc").toString();
            //qDebug() << temp_Card_info.Image_src;
            temp_Card_info.Health = (int)CarddataObject.value("health").toDouble();
            qDebug()<<"Health:"<<temp_Card_info.Health<<" HealthDou:"<<CarddataObject.value("health").toDouble();
            if (temp_Card_info.Image_src == "")
                temp_Card_info.Image_src = BaseCardURL+temp_Card_info.Code.left(2)+"/"+temp_Card_info.Code+".jpg";
            CardSWD_infoList.append(temp_Card_info);
            ui->downloadCards_Label->setText("Downloading data: "+temp_Card_info.Code);
        //}
        replyCard->deleteLater();

    }
}

void ScoreboardMain::getDeckResult(QNetworkReply *replyCard)
{
    QJsonParseError jsonError;
    QColor endColor;
    QString Deckdata = replyCard->readAll();
    QJsonDocument DeckJson = QJsonDocument::fromJson(Deckdata.toUtf8(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError){
      qDebug() << jsonError.errorString();
    }
    QJsonArray DeckdataArray = DeckJson.object().value("data").toArray();
    foreach (const QJsonValue & v, DeckdataArray)
    {
        QVariantMap DeckcardMap = v.toObject().value("cards").toObject().toVariantMap();
        for(QVariantMap::const_iterator iter = DeckcardMap.begin(); iter != DeckcardMap.end(); ++iter) {

            qDebug() << iter.key() << iter.value();
            QString foundCard = getCardtextFromCardnumber(iter.key());
            QString foundNumber = iter.value().toString();

            ui->tableDeck->setRowCount(row+1);
            QTableWidgetItem *widgetCard = new QTableWidgetItem(foundCard) ;
            QTableWidgetItem *widgetNumber = new QTableWidgetItem(foundNumber);
            QString Faction = getFactionFromCardtext(foundCard);
            if (Faction == "Error")
                qDebug() << "Problem finding card";
            if (Faction != "" && ui->checkUseGradientsDecklists->isChecked())
            {
                foreach(FactionColorCode faction, FactionColorCodes)
                {
                    if (faction.Faction.toLower() == Faction)
                    {
                        widgetCard->setBackground(QBrush(faction.FactionGradient));
                        endColor.setNamedColor(faction.EndColor);
                        widgetNumber->setBackground(QBrush(endColor));
                    }
                }

            }
            ui->tableDeck->setItem(row,0,widgetCard);
            ui->tableDeck->setItem(row,1,widgetNumber);
            row++;

        }

        int rowTotalHeight=0;
        int count=ui->tableDeck->verticalHeader()->count();
        for (int i = 0; i < count; ++i)
                rowTotalHeight+=ui->tableDeck->verticalHeader()->sectionSize(i);
         rowTotalHeight+=2; // For lines around tablewidget
         ui->tableDeck->setMinimumHeight(rowTotalHeight);
         ui->tableDeck->setMaximumHeight(rowTotalHeight);
    }
    replyCard->deleteLater();
}

void ScoreboardMain::replyFinished(QNetworkReply *reply)
{
    //int stopreply = 0;
    string getstring = "";
    size_t str;
    uint a = 0;
    //bool updateyes = false;
    //Check if there is a error
    if(reply->error())
    {
        ui->Testing->setText("Error: 0001, Not able to check for updates!");
        qDebug() << reply->error();
    }
    else
    {
        ui->Testing->setText("Reading URL");

        ofstream textout;
        textout.open(sLocation+"/Update/out.txt");
        //Read all from the url and writes to a file
        textout << reply->readAll().toStdString();
        textout.close();
        ifstream textin;
        textin.open(sLocation+"/Update/out.txt");
        if(textin.is_open())
        {
            while(getline(textin, getstring))
            {
                ui->Testing->setStyleSheet("QLabel{color: rgb(0, 0, 0);}");
                str = getstring.find(version_info);
                if(str!=string::npos)
                {
                    ui->Testing->setText("Status: No Update");
                    updateyes = true;
                    return;
                    //break;
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
//

void ScoreboardMain::WarningBox()
{
    if(updateyes == false)
    {
       QString HTMLcode_Update = "&nbsp;&nbsp;&nbsp;<a href=";
       HTMLcode_Update.append(QUpdate_URL);
       HTMLcode_Update.append("/releases");
       HTMLcode_Update.append(">Download Update</a>&nbsp;&nbsp;&nbsp;") ;
       HTMLcode_Update=HTMLcode_Update.toHtmlEscaped();
       QMessageBox::warning(this,"New Update Found", HTMLcode_Update);
       //stopreply = 1;
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
        Player1_Score_Output.open(sLocation+"/Output/Player1_Score.txt");
        Player2_Score_Output.open(sLocation+"/Output/Player2_Score.txt");

        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_Score_Output << "";
            Player2_Score_Output << "";
        }
        else
        {
            qDebug()<<"Updating scores";
            Player1_Score_Output << Player1_Score;
            Player2_Score_Output << Player2_Score;
         }
        Player1_Score_Output.close();
        Player2_Score_Output.close();
    }
    di->Player1_Score(Player1_Score);
    di->Player2_Score(Player2_Score);

    ui->Player1Score_Label->setText(QString::number(Player1_Score));
    ui->Player2Score_Label->setText(QString::number(Player2_Score));
}


void ScoreboardMain::on_downloadCards_Button_clicked() // downloadCards Button
{
    getCardsResult();
}

void ScoreboardMain::on_UA_Button_clicked() //Unattended Button
{
    UA_Output.open(sLocation+"/Output/Unattended.txt");
    if (ui->UA_Button->text()=="Show Unattended")
    {
        UA_Output << "Currently unattended";
        ui->UA_Button->setText("Hide Unattended");
    }
    else
    {
        UA_Output << "";
        ui->UA_Button->setText("Show Unattended");
    }
    UA_Output.close();
}

void ScoreboardMain::on_UpNextScreen_clicked() //UpNext Button
{
    QString TextNRSI= ui->Next_round_start_Input->text(), TextNRI=ui->Next_round_Input->text();
    NR_text=TextNRI.toUtf8().constData(); NRS_text=TextNRSI.toUtf8().constData();
    NR_Output.open(sLocation+"/Output/Next_round.txt");
    NRS_Output.open(sLocation+"/Output/Next_round_start.txt");
    if (ui->UpNextScreen->text()=="Show UpNextScreen")
    {
         QFile::copy(saveLocation+"/ID_Pictures/UpNext.png",saveLocation+"/Output/UpNext.png");
         NR_Output<<NR_text;
         NRS_Output<<NRS_text;
         ui->UpNextScreen->setText("Hide UpNextScreen");
    }
    else
    {
        if (QFile::exists(saveLocation+"/Output/UpNext.png"))
        {
            QFile::remove(saveLocation+"/Output/UpNext.png");
        }
        NR_Output<<"";
        NRS_Output<<"";
        ui->UpNextScreen->setText("Show UpNextScreen");
    }
    NR_Output.close();
    NRS_Output.close();
}

void ScoreboardMain::on_spinBox1P1_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_char1Health_Output.open(sLocation+"/Output/Player1_char1Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_char1Health_Output << "";
        }
        else
        {
            qDebug()<<"Updating scores";
            Player1_char1Health_Output << ui->spinBox1P1->value()<<"/"<<Max_healthP1C1;
        }
        Player1_char1Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox2P1_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_char2Health_Output.open(sLocation+"/Output/Player1_char2Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_char2Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player1_char2Health_Output << ui->spinBox2P1->value()<<"/"<<Max_healthP1C2;
        }
        Player1_char2Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox3P1_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_char3Health_Output.open(sLocation+"/Output/Player1_char3Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_char3Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player1_char3Health_Output << ui->spinBox3P1->value()<<"/"<<Max_healthP1C3;
        }
        Player1_char3Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox4P1_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_char4Health_Output.open(sLocation+"/Output/Player1_char4Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_char4Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player1_char4Health_Output << ui->spinBox4P1->value()<<"/"<<Max_healthP1C4;
        }
        Player1_char4Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox5P1_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_char5Health_Output.open(sLocation+"/Output/Player1_char5Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_char5Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player1_char5Health_Output << ui->spinBox5P1->value()<<"/"<<Max_healthP1C5;
        }
        Player1_char5Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox1P2_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player2_char1Health_Output.open(sLocation+"/Output/Player2_char1Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player2_char1Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player2_char1Health_Output << ui->spinBox1P2->value()<<"/"<<Max_healthP2C1;
        }
        Player2_char1Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox2P2_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player2_char2Health_Output.open(sLocation+"/Output/Player2_char2Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player2_char2Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player2_char2Health_Output << ui->spinBox2P2->value()<<"/"<<Max_healthP2C2;
        }
        Player2_char2Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox3P2_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player2_char3Health_Output.open(sLocation+"/Output/Player2_char3Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player2_char3Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player2_char3Health_Output << ui->spinBox3P2->value()<<"/"<<Max_healthP2C3;
        }
        Player2_char3Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox4P2_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player2_char4Health_Output.open(sLocation+"/Output/Player2_char4Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player2_char4Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player2_char4Health_Output << ui->spinBox4P2->value()<<"/"<<Max_healthP2C4;
        }
        Player2_char4Health_Output.close();
    }
}

void ScoreboardMain::on_spinBox5P2_valueChanged()
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player2_char5Health_Output.open(sLocation+"/Output/Player2_char5Health.txt");
        if (ui->NoScoreIdOutput->isChecked())
        {
            Player2_char5Health_Output << "";
       }
        else
        {
            qDebug()<<"Updating scores";
            Player2_char5Health_Output << ui->spinBox5P2->value()<<"/"<<Max_healthP2C5;
        }
        Player2_char5Health_Output.close();
    }
}

void ScoreboardMain::on_Player1UP_Button_clicked() //Player1_Up Button
{
    Player1_Score++;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Player1_Score:" + QString::number(Player1_Score) + "\n").toUtf8());
    }
    else
    {
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
    }
    else
    {
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
    }
    else
    {
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
    }
    else
    {
        return;
    }
}

void ScoreboardMain::on_Update_Team_Button_clicked() //Update Team Name Button
{
    QString Player1N = ui->Player1Name_Input->text(), Player2N = ui->Player2Name_Input->text();
    QString Player1I = ui->Player1Id_Input->currentText(), Player2I = ui->Player2Id_Input->currentText();
    QString RoundI = ui->Round_Input->text(); QString CustomI = ui->Custom_Input->text();
    Player1_Name = Player1N.toUtf8().constData();
    Player2_Name = Player2N.toUtf8().constData();
    Player1_Id = Player1I.toUtf8().constData();
    Player2_Id = Player2I.toUtf8().constData();
    Round= RoundI.toUtf8().constData();
    Custom = CustomI.toUtf8().constData();
    if(ui->checkBox->isChecked())
    {
        writexml();
    }
    else
    {
        Player1_Name_Output.open(sLocation+"/Output/Player1_Name.txt");
        Player2_Name_Output.open(sLocation+"/Output/Player2_Name.txt");
        Round_Output.open(sLocation+"/Output/Round.txt");
        Round_info_Output.open(sLocation+"/Output/Round_info.txt");
        Player1_Name_Output << Player1_Name;
        Player2_Name_Output << Player2_Name;
        Player1_Id_Output.open(sLocation+"/Output/Player1_Id.txt");
        Player2_Id_Output.open(sLocation+"/Output/Player2_Id.txt");
        QFile Player1_Id_HTML(saveLocation+"/Output/Player1_Id_HTML.html");
        Player1_Id_HTML.open(QFile::WriteOnly);
        QFile Player2_Id_HTML(saveLocation+"/Output/Player2_Id_HTML.html");
        Player2_Id_HTML.open(QFile::WriteOnly);
        QFile Player1_FactionId(saveLocation+"/Output/Player1_FactionId.txt");
        Player1_FactionId.open(QFile::WriteOnly);
        QFile Player2_FactionId(saveLocation+"/Output/Player2_FactionId.txt");
        Player2_FactionId.open(QFile::WriteOnly);


        if(ui->Swiss_Radio->isChecked())
        {
            Round_info_Output << "Swiss Round " << Round;
            Round_Output << Round ;
            if (Game == Game_ANR)
            {
                PresetRadio=3;
            }
            else if (Game == Game_SWD)
            {
                PresetRadio=5;
            }
            on_Reset_Button_clicked();
        }
        if(ui->Top_cut_Radio->isChecked())
        {
            Round_info_Output << "Top cut Round " << Round;
            Round_Output << Round ;
            PresetRadio=1;
            on_Reset_Button_clicked();
        }
        if(ui->Grand_final_Radio->isChecked())
        {
            Round_info_Output << "Grand Final";
            PresetRadio=2;
            on_Reset_Button_clicked();
        }
        if(ui->Casual_Radio->isChecked())
        {
            Round_info_Output << "Casual";
            PresetRadio=4;
            on_Reset_Button_clicked();
        }
        if(ui->Custom_Radio->isChecked())
        {
            Round_info_Output << Custom;
            PresetRadio=1;
            on_Reset_Button_clicked();
        }

        if (ui->NoScoreIdOutput->isChecked())
        {
            Player1_Score_Output.open(sLocation+"/Output/Player1_Score.txt");
            Player2_Score_Output.open(sLocation+"/Output/Player2_Score.txt");
            Player1_Score_Output << "";
            Player2_Score_Output << "";
            Player1_Score_Output.close();
            Player2_Score_Output.close();

            Player1_Id_Output << "";
            Player2_Id_Output << "";

            if (QFile::exists(saveLocation+"/Output/Player1_Id.png"))
            {
                QFile::remove(saveLocation+"/Output/Player1_Id.png");
            }

            if (QFile::exists(saveLocation+"/Output/Player2_Id.png"))
            {
                QFile::remove(saveLocation+"/Output/Player2_Id.png");
            }

            if (QFile::exists(saveLocation+"/Output/AgendaPoint.png"))
            {
                QFile::remove(saveLocation+"/Output/AgendaPoint.png");
            }

            if (QFile::exists(saveLocation+"/Output/faction_bakgrund.png"))
            {
                QFile::remove(saveLocation+"/Output/faction_bakgrund.png");
            }

        }
        else
        {
            if (Game == Game_ANR)
            {
                QString out1Id = ui->Player1Id_Input->currentText().mid(1,ui->Player1Id_Input->currentText().length()-1) ;
                Player1_Id_Output << out1Id.toStdString() ;
                QString out2Id = ui->Player2Id_Input->currentText().mid(1,ui->Player2Id_Input->currentText().length()-1) ;
                Player2_Id_Output << out2Id.toStdString() ;
            }
            if (Game == Game_SWD)
            {
                Player1_Id_Output << ui->Player1Id_Input->currentText().toStdString() ;
                Player2_Id_Output << ui->Player2Id_Input->currentText().toStdString() ;
            }

            if (QFile::exists(saveLocation+"/Output/Player1_Id.png"))
            {
                QFile::remove(saveLocation+"/Output/Player1_Id.png");
            }
            QString p1Faction = ui->Player1Id_Input->currentText().mid(0,1);
            QString p1FactionId ;
            //QString p1FactionId, p1FactionHTML;
            /*p1FactionHTML.append("<!DOCTYPE html><html lang=\"en\" dir=\"ltr\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><title> </title></head><body><p style=\"font-family:netrunner\">");
            p1FactionHTML.append(p1Faction);
            p1FactionHTML.append("</p></body></html>");*/
            p1FactionId.append(p1Faction);
            //qDebug() <<p1Faction ;

            /*QTextStream out1HTML(&Player1_Id_HTML);
            out1HTML.setCodec("UTF-8");
            out1HTML << p1FactionHTML ;
            QTextStream out1(&Player1_FactionId);
            out1.setCodec("UTF-8");
            out1 << p1FactionId ;*/

            if (Game == Game_ANR)
            {
                QString p1_id= ui->Player1Id_Input->currentText().mid(1,2); // First char is a Netrunner.ttf factionchar
                QString p1_idfile=saveLocation+"/ID_Pictures/" ;
                p1_idfile.append(p1_id);
                //qDebug() << "Id:" << p1_id ;
                p1_idfile.append(".png");
                QFile::copy(p1_idfile,saveLocation+"/Output/Player1_Id.png");
            }

            QString p2Faction = ui->Player2Id_Input->currentText().mid(0,1);
            QString p2FactionId ;
            //QString p2FactionId, p2FactionHTML ;
            /*p2FactionHTML.append("<!DOCTYPE html><html lang=\"en\" dir=\"ltr\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><title> </title></head><body><p style=\"font-family:netrunner\">");
            p2FactionHTML.append(p2Faction);
            p2FactionHTML.append("</p></body></html>");*/
            p2FactionId.append(p2Faction);

            /*QTextStream out2HTML(&Player2_Id_HTML);
            out2HTML.setCodec("UTF-8");
            out2HTML << p2FactionHTML ;
            QTextStream out2(&Player2_FactionId);
            out2.setCodec("UTF-8");
            out2 << p2FactionId ;*/

            if (QFile::exists(saveLocation+"/Output/Player2_Id.png"))
            {
                QFile::remove(saveLocation+"/Output/Player2_Id.png");
            }
            if (Game == Game_ANR)
            {
                QString p2_id= ui->Player2Id_Input->currentText().mid(1,2); // First char is a Netrunner.ttf factionchar
                QString p2_idfile=saveLocation+"/ID_pictures/" ;
                p2_idfile.append(p2_id);
                p2_idfile.append(".png");
                QFile::copy(p2_idfile,saveLocation+"/Output/Player2_Id.png");

                QFile::copy(saveLocation+"/ID_pictures/AgendaPoint.png",saveLocation+"/Output/AgendaPoint.png");
                QFile::copy(saveLocation+"/ID_pictures/faction_bakgrund.png",saveLocation+"/Output/faction_bakgrund.png");
            }

        }

        Player1_Name_Output.close();
        Player2_Name_Output.close();
        Player1_Id_Output.close();
        Player2_Id_Output.close();
        //Player1_Id_HTML.close();
        //Player2_Id_HTML.close();
        Player1_FactionId.close();
        Player2_FactionId.close();
        Round_Output.close();
        Round_info_Output.close();
    }

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
    }
    else if(minu == 0 && Stopwatch_input == false){
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
        }
        else if(saved_speed == Millisecond_Speed)
        {
            speed = Millisecond_Speed;
        }
    }
    if(Clock_button == 1 && Stopwatch_input == false)
    {
        input_stop = true;
        ui->Start_Button->setText("Paus");
        ScoreboardMain::Timer_Control();
    }
    if(Clock_button == 1 && Stopwatch_input == true)
    {
        input_stop = true;
        ui->Start_Button->setText("Paus");
        ScoreboardMain::Stopwatch_Control();
    }
    if (ui->Start_Button->text() == "Paus")
    {
        ui->Start_Button->setStyleSheet("* { background-color: rgba(255,0,0) }");
    }
    if (ui->Start_Button->text() == "Start")
    {
        ui->Start_Button->setStyleSheet("* { background-color: rgba(0,255,0) }");
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
    }
    else if(seco > 8 && Milliseconds == false){
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
    }
    else
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
    }
    else
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
    }
    else
    {
    Clock_button = 0;
    many = 0;
    if(presetbool == true && PresetRadio == 1 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  40:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(40);
        clock_symbol = ":";
        minu = 40;
        seco = 0;
        Clock_Output.open(sLocation+"/Output/Clock.txt");
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
    }
    else if(presetbool == true && PresetRadio == 2 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  00:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(0);
        clock_symbol = ":";
        minu = 00;
        seco = 0;
        Clock_Output.open(sLocation+"/Output/Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        ui->Clock_Label->setText("  00:00");
        if(ui->pushButton->text() == "Disable")
        {
            client->write(QString("Clock:  00:00").toUtf8());
        }
    }
    else if(presetbool == true && PresetRadio == 3 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  65:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(65);
        clock_symbol = ":";
        minu = 65;
        seco = 0;
        Clock_Output.open(sLocation+"/Output/Clock.txt");
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
    }
    else if(presetbool == true && PresetRadio == 4 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  70:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(70);
        minu = 70;
        seco = 0;
        clock_symbol = ":";
        Clock_Output.open(sLocation+"/Output/Clock.txt");
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
    }
    else if(presetbool == true && PresetRadio == 5 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  35:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(35);
        minu = 35;
        seco = 0;
        clock_symbol = ":";
        Clock_Output.open(sLocation+"/Output/Clock.txt");
        Clock_Output << "35:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "35:00";
            writexml();
        }
        ui->Clock_Label->setText("  35:00");
        if(ui->pushButton->text() == "Disable")
        {
            client->write(QString("Clock:  35:00").toUtf8());
        }
    }
    else if(presetbool == true && PresetRadio == 6 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  90:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(90);
        minu = 90;
        seco = 0;
        clock_symbol = ":";
        Clock_Output.open(sLocation+"/Output/Clock.txt");
        Clock_Output << "90:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "90:00";
            writexml();
        }
        ui->Clock_Label->setText("  90:00");
        if(ui->pushButton->text() == "Disable")
        {
            client->write(QString("Clock:  90:00").toUtf8());
        }
    }
    else
    {
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        Clock_Output.open(sLocation+"/Output/Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        ui->Clock_Label->setText("  00:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(0);
        minu = 0;
        seco = 0;
        Clock_Output.open(sLocation+"/Output/Clock.txt");
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
    }
    else
    {
        currenttime->stop();
        ui->Clock_Label->setText("  00:00");
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        Clock_Output.open(sLocation+"/Output/Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        seconds_zero = "";
        minutes_zero = "";
    }
}

void ScoreboardMain::timeclock()
{
    time_t current = time(nullptr); // 0 before
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
    Clock_Output.open(sLocation+"/Output/Clock.txt");
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
    Clock_Output.open(sLocation+"/Output/Clock.txt");
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

/*void ScoreboardMain::hotkeys()
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
}*/

void ScoreboardMain::on_About_ActionBar_triggered()
{
    QMessageBox about;
    about.setText("About");
    about.setInformativeText("                                                                                                                  ");
    about.setInformativeText("This Program is made by: HakanCarlstrom and based upon Scoreboard+ by ha0511kr \n"
                             "\nIf you have any questions or suggetions please contact me at \n"
                             "Email: hakan.aw.carlstrom at gmail.com \n"
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
    Changelog.setInformativeText("Based on Scoreboard+ 1.8.1Dev but revamped for Netrunner");
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

static bool truefalse = false;

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
        if(ui->Forty_Radio->isChecked())
        {
            PresetRadio = 1;
        }
        if(ui->Sixty_Radio->isChecked())
        {
            PresetRadio = 2;
        }
        if(ui->Sixtyfive_Radio->isChecked())
        {
            PresetRadio = 3;
        }
        if(ui->Seventy_Radio->isChecked())
        {
            PresetRadio = 4;
        }
        if(ui->Thirtyfive_Radio->isChecked())
        {
            PresetRadio = 5;
        }
        if(ui->Ninty_Radio->isChecked())
        {
            PresetRadio = 6;
        }
    }else{
        PresetRadio = 0;
        presetbool = false;
    }
}

void ScoreboardMain::on_Forty_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 1;
    }
}

void ScoreboardMain::on_Sixty_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 2;
    }
}

void ScoreboardMain::on_Sixtyfive_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 3;
    }
}

void ScoreboardMain::on_Seventy_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 4;
    }
}

void ScoreboardMain::on_Thirtyfive_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 5;
    }
}

void ScoreboardMain::on_Ninty_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 6;
    }
}

void ScoreboardMain::xml()
{
    //Creates QFile to the directory
    QFile file(saveLocation+"/Output/Xml.xml");

    //Check if file is writeable
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->Testing->setText("xml Could not be created");
    }else{
    ScoreboardMain::writexml();
    }

}
static int a = 0;

void ScoreboardMain::writexml()
{

    a++;
    //Creates a new xml file
    QFile file(saveLocation+"/Output/Xml.xml");
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
        QFile::remove(saveLocation+"/Output/Xml.xml");
        QFile::remove(saveLocation+"/Update/out.txt");
    }
}

void ScoreboardMain::on_actionNetrunner_triggered()
{
    ui->Player1Id2_Input->setVisible(false);
    ui->Player1Id3_Input->setVisible(false);
    ui->Player1Id4_Input->setVisible(false);
    ui->Player1Id5_Input->setVisible(false);
    ui->Player2Id2_Input->setVisible(false);
    ui->Player2Id3_Input->setVisible(false);
    ui->Player2Id4_Input->setVisible(false);
    ui->Player2Id5_Input->setVisible(false);
    ui->spinBox1P1->setVisible(false);
    ui->spinBox2P1->setVisible(false);
    ui->spinBox3P1->setVisible(false);
    ui->spinBox4P1->setVisible(false);
    ui->spinBox5P1->setVisible(false);
    ui->spinBox1P2->setVisible(false);
    ui->spinBox2P2->setVisible(false);
    ui->spinBox3P2->setVisible(false);
    ui->spinBox4P2->setVisible(false);
    ui->spinBox5P2->setVisible(false);
    ui->comboBoxChooseCharP1->setVisible(false);
    ui->comboBoxChooseCharP2->setVisible(false);
    ui->ChooseCharP1->setVisible(false);
    ui->ChooseCharP2->setVisible(false);

    ui->pushShowPlayer1Deck->setVisible(true);
    ui->pushShowPlayer2Deck->setVisible(true);
    ui->Player1DOWN_Button->setVisible(true);
    ui->Player1UP_Button->setVisible(true);
    ui->Player2DOWN_Button->setVisible(true);
    ui->Player2UP_Button->setVisible(true);
    ui->Player1Score_Label->setVisible(true);
    ui->Player2Score_Label->setVisible(true);
    ui->comboPlayer1Deck->setVisible(true);
    ui->comboPlayer2Deck->setVisible(true);

    Game = Game_ANR;
    CardDB_URL = CardDB_URL_ANR ;
    QCardDB_URL = QString::fromStdString(CardDB_URL);
    CardDB_DecklistURL = CardDB_DecklistURL_ANR ;
    BaseCardURL = BaseCardURL_ANR ;
    xxxCardDB=ANRCardDB;

    if (QFile::exists(saveLocation+"/Output/Background.jpg"))
    {
        QFile::remove(saveLocation+"/Output/Background.jpg");
    }
    QFile::copy(saveLocation+"/ID_Pictures/ANRBackground.jpg",saveLocation+"/Output/Background.jpg");

    //addANR_Ids();
    ui->Player1Id_Input->clear();
    ui->Player2Id_Input->clear();
    ui->Player1Id_Input->addItems(IdList);
    ui->Player2Id_Input->addItems(IdList);
}

void ScoreboardMain::on_actionSWDestiny_triggered()
{
    ui->Player1Id2_Input->setVisible(true);
    ui->Player1Id3_Input->setVisible(true);
    ui->Player1Id4_Input->setVisible(true);
    ui->Player1Id5_Input->setVisible(true);
    ui->Player2Id2_Input->setVisible(true);
    ui->Player2Id3_Input->setVisible(true);
    ui->Player2Id4_Input->setVisible(true);
    ui->Player2Id5_Input->setVisible(true);
    ui->spinBox1P1->setVisible(true);
    ui->spinBox2P1->setVisible(true);
    ui->spinBox3P1->setVisible(true);
    ui->spinBox4P1->setVisible(true);
    ui->spinBox5P1->setVisible(true);
    ui->spinBox1P2->setVisible(true);
    ui->spinBox2P2->setVisible(true);
    ui->spinBox3P2->setVisible(true);
    ui->spinBox4P2->setVisible(true);
    ui->spinBox5P2->setVisible(true);
    ui->comboBoxChooseCharP1->setVisible(true);
    ui->comboBoxChooseCharP2->setVisible(true);
    ui->ChooseCharP1->setVisible(true);
    ui->ChooseCharP2->setVisible(true);

    ui->pushShowPlayer1Deck->setVisible(false);
    ui->pushShowPlayer2Deck->setVisible(false);
    ui->Player1DOWN_Button->setVisible(false);
    ui->Player1UP_Button->setVisible(false);
    ui->Player2DOWN_Button->setVisible(false);
    ui->Player2UP_Button->setVisible(false);
    ui->Player1Score_Label->setVisible(false);
    ui->Player2Score_Label->setVisible(false);
    ui->comboPlayer1Deck->setVisible(false);
    ui->comboPlayer2Deck->setVisible(false);

    Game = Game_SWD;
    CardDB_URL = CardDB_URL_SWD ;
    QCardDB_URL = QString::fromStdString(CardDB_URL);
    CardDB_DecklistURL = CardDB_DecklistURL_SWD ;
    BaseCardURL = BaseCardURL_SWD ;
    xxxCardDB=SWDCardDB;

    if (QFile::exists(saveLocation+"/Output/Background.jpg"))
    {
        QFile::remove(saveLocation+"/Output/Background.jpg");
    }
    QFile::copy(saveLocation+"/SWD/SWDBackground.jpg",saveLocation+"/Output/Background.jpg");

    ui->Player1Id_Input->clear();
    ui->Player2Id_Input->clear();
    //IdList.clear();
}

void ScoreboardMain::on_actionDonate_triggered()
{
    QMessageBox DonateBox;
    //DonateBox.setText("<a href=https://goo.gl/fQD3gI>Donate!</a>");
    DonateBox.setInformativeText("Want to support me? I know this is completely optional\nbut it will definitely help me to keep working on this project!\nI don't care how much remember,every little bit counts!");
    DonateBox.exec();
}

void ScoreboardMain::on_SoundSpeed_Slider_sliderMoved(int slidepos)
{
    qreal test = (qreal)slidepos;
    //qreal test = slidepos;
    if(test < 0.0)
    {
        //test = -0.9;
    }
    player->setPlaybackRate(test);
}
