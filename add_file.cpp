#include "add_file.h"
#include "the_button.h"
#include <QMessageBox>

void AddFile::showMessage() {
    if(QMessageBox::information(this, "Tomeo", "This is a file chooser")==QMessageBox::Ok){
        addTheFile();
    }
}

void AddFile:: addTheFile(){
    //initialising button
    TheButton *button = new TheButton(videoButtonsWidget);
    button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
    buttons.push_back(button);
    // create label for the button
    QLabel* buttonTitle = new QLabel(QString(videos[0].url->fileName()));
    buttonTitle->setAlignment(Qt::AlignCenter);

    //making compliant with new organisation feature
    QVBoxLayout* nextPair = new QVBoxLayout();
    nextPair->addWidget(button);
    nextPair->addWidget(buttonTitle);
    pairs->push_back(nextPair);
    videoButtonsLayout->addLayout(nextPair);
    button->init(&videos.at(0));

}


