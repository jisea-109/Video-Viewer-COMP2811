#include "manage_button.h"
#include"QDir"
#include"QDirIterator"
#include"QImageReader"
using namespace std;

// Empty the layout from videos
// source : https://stackoverflow.com/questions/4857188/clearing-a-layout-in-qt
// author: Wes Hardaker
void emptyLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            emptyLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}
//Get videos from directory
vector<TheButtonInfo> ManageButton::getInfo (QString loc) {
    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(loc);
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
//Display videos from folder
void ManageButton::clicked() {
    for(int i=0;i<sub_layout->size();i++) {
        emptyLayout(sub_layout->at(i));
    }
    sub_layout->clear();
    videos->clear();
    global->clear();
    *videos = getInfo(path);
    for ( unsigned int i = 0; i < videos->size(); i++ ) {
        // create the button & add it to the button collection
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        global->push_back(button);

        // create label for the button
        QLabel* buttonTitle = new QLabel(QString(videos->at(i).url->fileName()));
        buttonTitle->setAlignment(Qt::AlignCenter);

        // pair up the label & button in a VBoxLayout object i.e. make the label appear beneath the button
        QVBoxLayout* nextPair = new QVBoxLayout();
        nextPair->addWidget(button);
        nextPair->addWidget(buttonTitle);
        sub_layout->push_back(nextPair);
        b_layout->addLayout(nextPair);

        button->init(&videos->at(i));


    }



}
