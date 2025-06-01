#include "lyricpage.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QScrollBar>
#include <QFont>
#include <QVBoxLayout>
#include <QFileInfo>

LyricPage::LyricPage(QWidget *parent) : QWidget(parent)
{
    // 主布局（左上部，带内边距）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15); // 上下左右内边距
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft); // 左上部对齐

    // 滚动区域（背景透明，适应左上部）
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: transparent; }"
        "QScrollBar { width: 10px; background: #333; }"
        "QScrollBar::handle { background: #666; }"
    );
    mainLayout->addWidget(scrollArea);

    // 歌词容器（垂直布局，顶部对齐）
    lyricContainer = new QWidget(scrollArea);
    scrollArea->setWidget(lyricContainer);
    lyricLayout = new QVBoxLayout(lyricContainer);
    lyricLayout->setSpacing(8); // 行间距
    lyricLayout->setAlignment(Qt::AlignTop); // 歌词从顶部开始显示

    // 初始化占位符（无歌词时显示）
    QLabel *placeholder = new QLabel("暂无歌词");
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setStyleSheet("color: #999; font-size: 14px;");
    lyricLayout->addWidget(placeholder);
}

void LyricPage::loadLyric(const QString &audioPath)
{
    clearLyrics();
    QString lrcPath = getLrcPath(audioPath);
    
    QFile file(lrcPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return; // 无歌词文件时静默处理
    }

    QByteArray data = file.readAll();
    QString content;

    // 优先尝试UTF-8解码
    content = QString::fromUtf8(data);
    if (!content.contains('[')) { // 简单判断是否为有效歌词（包含时间戳标记）
        // 尝试本地编码（Windows默认GBK，macOS默认UTF-8已被优先处理）
        content = QString::fromLocal8Bit(data);
    }
    
    parseLrc(content);
    createLyricLabels();
}

void LyricPage::parseLrc(const QString &content)
{
    lyricList.clear();
    QRegularExpression re(R"(\[(\d{2}):(\d{2})\.(\d{2,3})\](.*))");
    
    for (const QString &line : content.split('\n')) {
        QRegularExpressionMatch match = re.match(line.trimmed());
        if (!match.hasMatch()) continue;

        int min = match.captured(1).toInt();
        int sec = match.captured(2).toInt();
        int msec = match.captured(3).toInt();
        QString text = match.captured(4).trimmed();

        // 转换为总毫秒数（处理2位/3位毫秒）
        qint64 totalTime = min * 60000 + sec * 1000 + (msec < 100 ? msec * 10 : msec);
        
        if (!text.isEmpty()) {
            lyricList.append({totalTime, text});
        }
    }

    // 按时间排序
    std::sort(lyricList.begin(), lyricList.end(),
              [](const LyricLine &a, const LyricLine &b) { return a.time < b.time; });
}

void LyricPage::createLyricLabels()
{
    // 清除旧标签（Qt 6 保留itemAt()和takeAt()）
    while (QLayoutItem *item = lyricLayout->takeAt(0)) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    // 添加新歌词标签
    for (const LyricLine &line : lyricList) {
        QLabel *label = new QLabel(line.text);
        label->setAlignment(Qt::AlignCenter);
        label->setFont(QFont("微软雅黑", 12));
        label->setStyleSheet("color: #999;");  // 默认灰色
        label->setMinimumHeight(30);           // 行高固定
        label->setWordWrap(true);          // 自动换行
        lyricLayout->addWidget(label);
    }

    lyricLayout->addStretch();  // 保持底部空间
}

void LyricPage::updateLyricPosition(qint64 position)
{
    if (lyricList.isEmpty()) return;

    // 查找当前行
    int newIndex = -1;
    for (int i = 0; i < lyricList.size(); ++i) {
        if (position >= lyricList[i].time 
            && (i == lyricList.size()-1 || position < lyricList[i+1].time)) {
            newIndex = i;
            break;
        }
    }

    if (newIndex == currentLineIndex) return;

    // 更新高亮
    if (currentLineIndex != -1) {
        if (QLabel *oldLabel = qobject_cast<QLabel*>(lyricLayout->itemAt(currentLineIndex)->widget())) {
            oldLabel->setStyleSheet("color: #999;");
        }
    }

    if (newIndex != -1) {
        if (QLabel *newLabel = qobject_cast<QLabel*>(lyricLayout->itemAt(newIndex)->widget())) {
            newLabel->setStyleSheet("color: #2196F3; font-weight: bold;");
            
            // 滚动到居中
            int labelY = newLabel->y() + newLabel->height() / 2;
            scrollArea->verticalScrollBar()->setValue(labelY - scrollArea->height() / 2);
        }
    }

    currentLineIndex = newIndex;
}

QString LyricPage::getLrcPath(const QString &audioPath)
{
    QFileInfo fileInfo(audioPath);
    return fileInfo.absolutePath() + "/" + fileInfo.completeBaseName() + ".lrc";
}

void LyricPage::clearLyrics()
{
    lyricList.clear();
    currentLineIndex = -1;
    createLyricLabels();
}