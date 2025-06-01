#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Triton Music Player");
    setWindowIcon(QIcon("../images/icon.jpg"));
    ui->listWidget->setIconSize(QSize(90, 90)); // 设置列表图标大小
    
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    

    player = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    // 初始化歌词页（通过提升的Widget获取）
    lyricPage = qobject_cast<LyricPage*>(ui->lyricDisplayWidget);


    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::do_sliderPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::do_stateChanged);
    connect(player, &QMediaPlayer::positionChanged,  lyricPage, &LyricPage::updateLyricPosition);

    LoopMode currentLoopMode = LoopList;
    ui->loopBtn->setText("顺序播放");
    
}

void MainWindow::on_nextBtn_clicked()
{
    int count = ui->listWidget->count();
    if (count == 0) return; // 列表为空时直接返回
    m_isUesrAction = true; // 用户操作标志
    int currentRow = playingRow; // 获取当前行号

    switch (currentLoopMode) {
        case LoopNone:
            if (currentRow == count - 1) {
            m_isUesrAction = false; // 如果是顺序播放且最后一首，设置为非用户操作
                return;}// 最后一首不自动切换
            currentRow += 1;
            break;
        case LoopSingle:
            currentRow = currentRow; // 单曲循环不切换
            break;
        case LoopList:
            currentRow = (currentRow + 1) % count; // 循环列表，统一用模运算
            break;
    }
    //切换音频
    ui->listWidget->setCurrentRow(currentRow);
    QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem());
    player->setSource(sourceUrl);
    player->play();
    // 更新歌词
    lyricPage->loadLyric(sourceUrl.toLocalFile()); // 加载对应歌词
    // 更新封面图
    setCover(sourceUrl.toLocalFile());
}

void MainWindow::on_previousBtn_clicked()
{

    int count = ui->listWidget->count();
    if (count == 0) return; // 列表为空时直接返回
    m_isUesrAction = true; // 用户操作标志
    int currentRow = playingRow;

    switch (currentLoopMode) {
        case LoopNone:
            if (currentRow == 0) {
            m_isUesrAction = false; // 如果是顺序播放且第一首，设置为非用户操作
            return; }// 第一首不往前切换
            currentRow -= 1;
            break;
        case LoopSingle:
            currentRow = currentRow; // 单曲循环不切换
            break;
        case LoopList:
            currentRow = (currentRow - 1 + count) % count; // 循环列表，处理负数模运算
            break;
    }
    // 切换音频
    ui->listWidget->setCurrentRow(currentRow);
    QUrl sourceUrl = getUrlFromItem(ui->listWidget->currentItem());
    player->setSource(sourceUrl);
    player->play();
    // 更新歌词
    lyricPage->loadLyric(sourceUrl.toLocalFile()); // 加载对应歌词
    // 更新封面图
    setCover(sourceUrl.toLocalFile());
}
//——————————————————————————————————————————

//播放按钮函数
void MainWindow::on_playBtn_clicked()
{   
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->playBtn->setText("播放");
    } else {
        player->play();
        if(defaultState) {
            lyricPage->loadLyric(player->source().toLocalFile()); // 恢复歌词
            // 更新封面图
            setCover(player->source().toLocalFile());
            defaultState = false; // 重置默认状态
        }
        ui->playBtn->setText("暂停");
    }
}//

//静音函数
void MainWindow::on_soundBtn_clicked()
{
    if(player->audioOutput()->isMuted())
    {
        player->audioOutput()->setMuted(false);
        ui->soundBtn->setIcon(QIcon(":/images/sound.png"));
    }
    else
    {
        player->audioOutput()->setMuted(true);
        ui->soundBtn->setIcon(QIcon(":/images/mute.png"));
    }
}

//音量滑块函数
void MainWindow::on_sliderVolume_valueChanged(int value)
{
    player->audioOutput()->setVolume(value/100.0);
}

void MainWindow::on_sliderPosition_valueChanged(int value)
{   
    if(player->duration()<=0)
        return;//如果音频文件没有加载完成，则不更新进度条   
    if(player->playbackState()==QMediaPlayer::PlayingState)
        player->setPosition(value);     
    currentTime=QString::asprintf("%02d:%02d",value/1000/60,value/1000%60);
    ui->timeLabel->setText(currentTime+"/"+durationTime);
}

//移除按钮函数
void MainWindow::on_removeBtn_clicked()
{
    if(ui->listWidget->count() == 0)
        return; // 列表为空时直接返回
    if(ui->listWidget->currentItem() == nullptr)
        return; // 没有选中项时直接返回
    int selectedRow = ui->listWidget->currentRow();
    if(playingRow!=selectedRow)
        {
            delete ui->listWidget->currentItem(); // 删除当前选中项
        }
    else
        {
            if(ui->listWidget->count() == 0) {
                goDefaultState(); // 如果列表为空，回到默认状态
                QListWidgetItem* item = ui->listWidget->takeItem(selectedRow);
                delete item;
                return;
            } 
            else if(currentLoopMode == LoopNone && selectedRow == ui->listWidget->count() - 1) {
                // 如果是顺序播放且删除的是最后一首，回到默认状态
                goDefaultState();
                QListWidgetItem* item = ui->listWidget->takeItem(selectedRow);
                delete item;
                return;
            } else {
               //先切歌，再删除原item，防止先删除item导致播放错误
                on_nextBtn_clicked(); // 切换到下一首
                QListWidgetItem* item = ui->listWidget->takeItem(selectedRow);
                delete item;
                playingRow = ui->listWidget->currentRow();
            }
        }
}

//清除按钮函数
void MainWindow::on_clearBtn_clicked()
{
    if(ui->listWidget->count() == 0)
        return;
    m_isUesrAction = true; // 用户操作标志
    loopPlay=false;
    lyricPage->clearLyrics(); // 清除所有歌词显示
    ui->playBtn->setText("播放");
    ui->listWidget->clear();
    player->stop();
}

//双击播放函数
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index){
    m_isUesrAction = true; // 用户操作标志
    if (index.isValid()) {
        QListWidgetItem *item = ui->listWidget->item(index.row());
        if (item) {
            QUrl sourceUrl = getUrlFromItem(item);
            player->setSource(sourceUrl);
            player->play();
            ui->playBtn->setText("暂停");
            // 更新歌词
            lyricPage->loadLyric(sourceUrl.toLocalFile()); // 加载对应歌词
            // 更新封面图
            setCover(sourceUrl.toLocalFile());
        }
    }
}

//更改单曲循环、列表循环、顺序播放按键标签
void MainWindow::on_loopBtn_clicked()
{
     switch (currentLoopMode) {
        case LoopNone:    // 当前是顺序播放 → 切换为单曲循环
            currentLoopMode = LoopSingle;
            ui->loopBtn->setText("单曲循环");
            break;
        case LoopSingle:  // 当前是单曲循环 → 切换为列表循环
            currentLoopMode = LoopList;
            ui->loopBtn->setText("列表循环");
            break;
        case LoopList:    // 当前是列表循环 → 切换为顺序播放
            currentLoopMode = LoopNone;
            ui->loopBtn->setText("顺序播放");
            break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
