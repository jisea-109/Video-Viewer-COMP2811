#ifndef ADD_FILE_H
#define ADD_FILE_H

#include "the_player.h"

#include <QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QUrl>

class AddFile : public QPushButton {
    Q_OBJECT

public:
    AddFile(QWidget *parent, QHBoxLayout &videoButtonsLayout) :  QPushButton(parent){
        setText("Add File");
        QHBoxLayout *vbl=&videoButtonsLayout;
        connect(this, SIGNAL(released()), this, SLOT(showMessage()));

    }
    void setPairsVector(vector<QVBoxLayout*>* p) {
        pairs = p;
    }
    QWidget *videoButtonsWidget;
    QHBoxLayout *videoButtonsLayout;
    ThePlayer *player;
    vector<TheButton*> buttons;
    vector<TheButtonInfo> videos;

private:
    vector<QVBoxLayout*> *pairs;

public slots:
    void showMessage();
    void addTheFile();//QHBoxLayout &vbl
signals:
    void clicked();
};

#endif // ADD_FILE_H
