#include "add_file.h"
#include "the_button.h"
#include <QMessageBox>

void AddFile::showMessage() {
    /*QMessageBox *popUp= new QMessageBox;
    popUp->addButton(QMessageBox::Ok);
    popUp->setWindowTitle("File Chooser");
    popUp->setText("This would be a file chooser");
    popUp->show();*/
    if(QMessageBox::information(this, "Tomeo", "This is a file chooser")==QMessageBox::Ok){
        addTheFile();
    }
    //connect(&popUp, SIGNAL(buttonClicked()), this, SLOT(addTheFile()));
}

void AddFile:: addTheFile(){
    TheButton *button = new TheButton(videoButtonsWidget);
    button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
    buttons.push_back(button);
    // create label for the button
    QLabel* buttonTitle = new QLabel(QString(videos[0].url->fileName()));
    buttonTitle->setAlignment(Qt::AlignCenter);

    QVBoxLayout* nextPair = new QVBoxLayout();
    nextPair->addWidget(button);
    nextPair->addWidget(buttonTitle);
    pairs->push_back(nextPair);
    videoButtonsLayout->addLayout(nextPair);
    button->init(&videos.at(0));
    //videoButtonsLayout->addWidget(button);
    //button->init(&videos.at(0));

    QMessageBox *popUp= new QMessageBox;
    popUp->setText("Test!");
    popUp->show();
}


