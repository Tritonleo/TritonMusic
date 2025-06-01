#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lyricpage.h"
#include <QMainWindow>
#include <QtMultimedia>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTextStream>
#include <QTextBrowser>
#include <QImage>
#include "tag.h"
#include "fileref.h"
#include "mpegfile.h"
#include "flacfile.h"
#include "id3v2tag.h"
#include "id3v2frame.h"
#include "id3v2.h"
#include "attachedpictureframe.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;

    bool loopPlay=true;
    bool m_isUesrAction = false;
    bool defaultState = false;
    QString durationTime;
    QString currentTime;
    int playingRow = 0; // 当前播放的行号

    LyricPage *lyricPage;

    enum LoopMode
    {
        LoopNone,
        LoopSingle,
        LoopList
    };
    LoopMode currentLoopMode = LoopNone;

    QUrl getUrlFromItem(QListWidgetItem *item);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 

private slots:
    void on_addFileBtn_clicked();
    void on_playBtn_clicked();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_clearBtn_clicked();
    void on_removeBtn_clicked();
    void on_soundBtn_clicked();
    void on_loopBtn_clicked();
    void on_nextBtn_clicked();
    void on_sliderVolume_valueChanged(int value);
    void on_sliderPosition_valueChanged(int value);
    void on_previousBtn_clicked();

    void do_sliderPositionChanged(qint64 position);
    void do_durationChanged(qint64 duration);
    void do_stateChanged(QMediaPlayer::PlaybackState state);
    
    void setCover(const QString& filePath);

    void goDefaultState();
};
#endif // MAINWINDOW_H
