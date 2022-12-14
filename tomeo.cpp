/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include "the_player.h"
#include "the_button.h"
#include "add_file.h"
#include "manage_button.h"


using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }
    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
    videoWidget->setMaximumHeight(500);
    videoWidget->setMinimumHeight(500);

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a scrolling row of buttons
    QScrollArea* videoBay = new QScrollArea();
    videoBay->resize(1000, 180);

    QWidget *buttonWidget = new QWidget();
    buttonWidget->resize(1200, 165);

    // a list of the buttons
    vector<TheButton*> buttons;

    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    // horizontal time slider
    QSlider *slider = new QSlider(Qt::Horizontal);
    player->time_scroller = slider;

    // timer
    QLabel *label = new QLabel(); //timer
    player->timer_text = label;

    // connect slider to jumpToinVideo function
    player->connect(slider, &QSlider::sliderMoved, player, &ThePlayer::jumpToinVideo);
    // connect time tracker
    player->connect(player, &QMediaPlayer::durationChanged, player, &ThePlayer::durationChanged);
    // connect current duration
    player->connect(player, &QMediaPlayer::positionChanged, player, &ThePlayer::positionChanged);

    // create the buttons

    //vector of pairs
    vector<QVBoxLayout*> pairs;
    for ( unsigned int i = 0; i < videos.size(); i++ ) {
        // create the button & add it to the button collection
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);

        // create label for the button
        QLabel* buttonTitle = new QLabel(QString(videos[i].url->fileName()));
        buttonTitle->setAlignment(Qt::AlignCenter);

        // pair up the label & button in a VBoxLayout object i.e. make the label appear beneath the button
        QVBoxLayout* nextPair = new QVBoxLayout();
        nextPair->addWidget(button);
        nextPair->addWidget(buttonTitle);
        pairs.push_back(nextPair);
        layout->addLayout(nextPair);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, &videos);

    // set the widget of the scrolling bay (containing all the buttons & labels)
    videoBay->setWidget(buttonWidget);

    // add the video and the buttons to the top level widget
    QWidget *topWidget = new QWidget;
    QVBoxLayout *top = new QVBoxLayout();
    topWidget->setLayout(top);
    top->addWidget(videoWidget);
    top->addWidget(label); // add timer
    top->addWidget(slider); // add slider
    top->addWidget(videoBay);

    //buttons in column to left
    QWidget *leftButtonsWidget = new QWidget;
    AddFile *addFileButton = new AddFile(leftButtonsWidget, *layout);
    addFileButton->videoButtonsWidget=buttonWidget;
    addFileButton->videoButtonsLayout=layout;
    addFileButton->player=player;
    addFileButton->buttons=buttons;
    addFileButton->videos=videos;
    addFileButton->setPairsVector(&pairs);
    //Left column layout
    QVBoxLayout *leftColumn = new QVBoxLayout();
    leftButtonsWidget->setLayout(leftColumn);
    leftColumn->addWidget(addFileButton);

    //(Load folders)

    //Extract dir names and paths
    vector<string>dirnames;
    vector<QString>paths;
    QDir direct(QString::fromStdString(argv[1]));
    direct.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList list = direct.entryInfoList();
    for(int i = 0; i < list.size(); i++) {
        QFileInfo fileinfo = list.at(i);
        //dirs.push_back(fileinfo.fileName());
        paths.push_back(fileinfo.filePath());
        qDebug()<<fileinfo.filePath();
        string str = fileinfo.fileName().toStdString();
        qDebug()<<QString::fromStdString(str);
        dirnames.push_back(str);
    }
    //Create Home Button
    string str = "Home";
    QString name = QString::fromStdString(str);
    ManageButton *btn = new ManageButton(leftButtonsWidget);
    //btn->setLayout(leftColumn);
    btn->setThePlayer(player);
    btn->setVideoWidget(videoWidget);
    btn->setButtonWidget(buttonWidget);
    btn->setVideos(&videos);
    btn->setLlayout(layout);
    btn->setGlobal(&buttons);
    btn->setPath(argv[1]);
    btn->setText(name);
    btn->setsublayout(&pairs);
    leftColumn->addWidget(btn);

    //Load the directories
    for(int i = 0; i < dirnames.size(); i++) {
        QString name = QString::fromStdString(dirnames.at(i));
        ManageButton *btn = new ManageButton(leftButtonsWidget);
        //btn->setLayout(leftColumn);
        btn->setThePlayer(player);
        btn->setVideoWidget(videoWidget);
        btn->setButtonWidget(buttonWidget);
        btn->setVideos(&videos);
        btn->setLlayout(layout);
        btn->setGlobal(&buttons);
        btn->setPath(paths.at(i));
        btn->setText(name);
        btn->setsublayout(&pairs);
        leftColumn->addWidget(btn);
    }


    //(Load folders end)

    // create the main window and layout
    QWidget window;
    QHBoxLayout *main = new QHBoxLayout();
    window.setLayout(main);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(100, 730);

    main->addWidget(leftButtonsWidget);
    main->addWidget(topWidget);


    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
