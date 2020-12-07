//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

#include <QTime>
#include <QSlider>
#include <QLabel>
using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}
void ThePlayer::jumpToinVideo(int sec)
{
   setPosition(sec * 1000);
}
void ThePlayer::setTimer(int duration)
{
    QString Str;
    if (duration || length) {
        QTime currentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000); // currentTime(hour, minute, second)
        QTime totalTime((length / 3600) % 60, (length / 60) % 60, length % 60, (length * 1000) % 1000); // totalTime(hour, minute, second)
        QString format = "hh:mm:ss";
        Str = currentTime.toString(format) + " | " + totalTime.toString(format);
    }
    timer_text->setText(Str);
}
void ThePlayer::durationChanged(int duration)
{
    length = duration / 1000;
    time_scroller->setMaximum(length);
}
void ThePlayer::positionChanged(int duration)
{
    if (!time_scroller->isSliderDown())
        time_scroller->setValue(duration / 1000);

    setTimer(duration / 1000); //update
}
