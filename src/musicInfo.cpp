#include <mainwindow.h>
#include "ui_mainwindow.h"

//——————————————————————————————————————————
//添加音频文件 **核心代码**
//使用QFileDialog::getOpenFileNames()函数选择音频文件
void MainWindow::on_addFileBtn_clicked()
{
    QString curPath = QDir::homePath();
    QString dlgTitle = "选择音频文件";
    QString filter = "音频文件(*.mp3 *.wav *.flac *.m4a *.ogg);;所有文件(*.*)";
    QStringList fileNames = QFileDialog::getOpenFileNames(this, dlgTitle, curPath, filter);

    if (fileNames.isEmpty())
        return; // 无文件选择，返回

    for (int i = 0; i < fileNames.size(); i++)
    {
        QString fullPath = fileNames.at(i); // 完整文件路径（如 D:/music/song.mp3）
        QFileInfo fileInfo(fullPath); 
        QString displayName = fileInfo.fileName(); // 提取纯文件名（如 song.mp3）


        // 创建列表项，显示纯文件名
        QListWidgetItem *item = new QListWidgetItem(displayName);
        item->setIcon(QIcon(":/images/music.png"));
        // 将完整路径存储到列表项的 UserRole 中（后续播放时需要用）
        item->setData(Qt::UserRole, fullPath); 
        
        ui->listWidget->addItem(item);
    }

    if (player->playbackState() != QMediaPlayer::PlayingState)
    {
        ui->listWidget->setCurrentRow(0);
        // 修改 getUrlFromItem 函数，从 UserRole 获取完整路径
        QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem()); 
        player->setSource(sourceUrl);
        player->audioOutput()->setVolume(0.5);
        ui->playBtn->setText("暂停");
        // 添加歌词
        lyricPage->loadLyric(sourceUrl.toLocalFile()); // 加载对应歌词
        // 播放音频
        player->play();
    }
}
//用于获取当前选中音频文件的路径
//返回QUrl类型的音频文件路径
QUrl MainWindow::getUrlFromItem(QListWidgetItem *item)
{
    // 从 UserRole 获取存储的完整路径，转为 QUrl
    playingRow = ui->listWidget->currentRow();
    return QUrl::fromLocalFile(item->data(Qt::UserRole).toString());
}

void MainWindow::do_durationChanged(qint64 duration)
{
    if(duration>=0)
    {
        ui->sliderPosition->setMaximum(duration);
        //设置进度条的最大值
        durationTime=QString::asprintf("%02d:%02d",duration/1000/60,duration/1000%60);
        ui->timeLabel->setText(currentTime+"/"+durationTime);
        //显示当前时间和总时间 
    }
}

void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{
   // 仅处理“播放结束”的情况（StoppedState）
    if (state != QMediaPlayer::StoppedState) return;

    if(m_isUesrAction)
    {
        m_isUesrAction = false; // 重置用户操作标志
        return; // 用户手动操作，直接返回
    }

    // 获取当前播放的行号（列表中的索引）
    int currentRow = playingRow;
    if (currentRow < 0) return; // 无选中项，不处理

    switch (currentLoopMode) {
        case LoopSingle: { 
            // 单曲循环：重新播放当前曲目
            //歌词不用动
            QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem());
            player->setSource(sourceUrl);
            player->play();
            break;
        }
        case LoopList: { 
            // 列表循环：播放下一首（若为最后一首则回到第一首）
            int nextRow = (currentRow + 1) % ui->listWidget->count();
            ui->listWidget->setCurrentRow(nextRow);
            QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem());
            player->setSource(sourceUrl);
            player->play();
            // 更新歌词
            lyricPage->loadLyric(sourceUrl.toLocalFile()); // 加载对应歌词
            break;
        }
        case LoopNone: { 
            // 顺序播放：若当前是最后一首则停止，否则播放下一首（自动连播）
            if (currentRow < ui->listWidget->count() - 1) {
                int nextRow = currentRow + 1;
                ui->listWidget->setCurrentRow(nextRow);
                QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem());
                player->setSource(sourceUrl);
                player->play();
                // 更新歌词
                lyricPage->loadLyric(sourceUrl.toLocalFile()); // 加载对应歌词
            } else {
                // 如果是最后一首，回到默认状态
                goDefaultState();
            }
            break;
        }
    }
}

void MainWindow::do_sliderPositionChanged(qint64 position)
{   

   if(player->duration()>0){
    currentTime=QString::asprintf("%02d:%02d",position/1000/60,position/1000%60);

    ui->sliderPosition->blockSignals(true);
    //阻止信号的发射
    ui->sliderPosition->setSliderPosition(position);
    //设置进度条的值
    ui->sliderPosition->blockSignals(false);
    //允许信号的发射

    ui->timeLabel->setText(currentTime+"/"+durationTime);
   }

}

void MainWindow::goDefaultState()
{
    m_isUesrAction=true;
    if(ui->listWidget->count() == 0) {
        player->stop(); // 停止播放器
        ui->playBtn->setText("播放");
        ui->timeLabel->setText("00:00/00:00");
        ui->sliderPosition->setSliderPosition(0);
        durationTime = "00:00"; // 重置总时长
        currentTime = "00:00"; // 重置当前时间
        defaultState = true; // 设置默认状态标志
        lyricPage->clearLyrics(); // 清除当前歌词显示
        m_isUesrAction = false; // 重置用户操作标志
        ui->listWidget->clear(); // 清空列表 
        ui->listWidget->setCurrentRow(-1);
        playingRow=ui->listWidget->currentRow();
        return;
    }; 
    // 重置播放器状态和界面
    ui->listWidget->setCurrentRow(0);
    playingRow=ui->listWidget->currentRow();
    QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem());
    player->setSource(sourceUrl);
    player->stop();
    ui->sliderPosition->setSliderPosition(0);
    currentTime = QString::asprintf("%02d:%02d", 0, 0);
    ui->timeLabel->setText(currentTime + "/" + durationTime);
    ui->playBtn->setText("播放");
    lyricPage->clearLyrics(); // 清除当前歌词显示]
    defaultState = true; // 设置默认状态标志
    m_isUesrAction = false; // 重置用户操作标志
}
