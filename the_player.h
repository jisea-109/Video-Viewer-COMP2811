//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QLabel>
#include <QSlider>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    long updateCount = 0;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
    }

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    // video length
    int length = 0;
    // timer
    QLabel *timer_text;
    // time_scroller
    QSlider *time_scroller;
private slots:

    void playStateChanged (QMediaPlayer::State ms);


public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    //timer setting
    void setTimer(int currentInfo);
    // jump to in video
    void jumpToinVideo (int sec);
    // set the range of scroller; 0 to video end time
    void durationChanged(int duration);
     // set the position of scroller
    void positionChanged(int progress);
};

#endif //CW2_THE_PLAYER_H
