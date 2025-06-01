#ifndef LYRICPAGE_H
#define LYRICPAGE_H

#include <QWidget>
#include <QList>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>


struct LyricLine {
    qint64 time;       // 时间戳（毫秒）
    QString text;      // 歌词文本
};

class LyricPage : public QWidget
{
    Q_OBJECT
public:
    explicit LyricPage(QWidget *parent = nullptr);
    void loadLyric(const QString &audioPath);  // 加载对应歌词文件
    void clearLyrics();                      
    void createLyricLabels(); 

public slots:
    void updateLyricPosition(qint64 position); // 更新当前播放位置

private:
    QScrollArea *scrollArea;
    QWidget *lyricContainer;
    QVBoxLayout *lyricLayout;  // 显式保留布局指针
    QList<LyricLine> lyricList;
    int currentLineIndex;

    void parseLrc(const QString &lrcPath);   // 解析lrc文件               

    QString getLrcPath(const QString &audioPath);
};

#endif // LYRICPAGE_H

