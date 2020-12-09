#ifndef MANAGEBUTTON_H
#define MANAGEBUTTON_H
#include<the_player.h>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QVideoWidget>
#include<QString>
#include<vector>

using namespace std;

class ManageButton : public QPushButton {
    Q_OBJECT

public:
    ManageButton(QWidget* parent) : QPushButton(parent) {
        connect(this, SIGNAL(released()), this, SLOT (clicked() ));
    }
    void setVideoWidget(QVideoWidget* v) {
        video = v;
    }
    void setButtonWidget(QWidget *b) {
        buttonWidget = b;
    }
    void setThePlayer(ThePlayer *p) {
        player = p;
    }
    void setGlobal(vector<TheButton*>* g) {
        global = g;
    }
    void setPath(QString p) {
        path = p;
    }
    void setLlayout(QHBoxLayout *l) {
        b_layout = l;
    }
    void setsublayout(vector<QVBoxLayout*>* l) {
        sub_layout = l;
    }
    void setVideos(vector<TheButtonInfo>* v) {
        videos = v;
    }
    vector<TheButtonInfo> getInfo (QString loc);
private:
    QHBoxLayout *b_layout;
    vector<QVBoxLayout*> *sub_layout;
    QVideoWidget *video;
    QWidget *buttonWidget;
    QString path;
    ThePlayer *player;
    vector<TheButton*>* global;
    vector<TheButtonInfo>* videos;



private slots:
   void clicked();

};

#endif // MANAGEBUTTON_H
