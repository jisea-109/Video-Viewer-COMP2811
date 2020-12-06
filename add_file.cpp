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
    videoButtonsLayout->addWidget(button);
    button->init(&videos.at(0));

    QMessageBox *popUp= new QMessageBox;
    popUp->setText("Test!");
    popUp->show();
}


