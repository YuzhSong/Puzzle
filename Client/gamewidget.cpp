#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <utility> // 添加这个头文件用于 std::pair 和 std::make_pair
#include <algorithm> // 用于排序和去重
#include <set> // 用于去重

// 修改构造函数
GameWidget::GameWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    // 在构造函数体内初始化新成员
    hintBlinkTimer = nullptr;
    hintLabel1 = nullptr;
    hintLabel2 = nullptr;
    hintBlinkState = false;
}

void GameWidget::setupScene(int i){
    gameOver=false;
    is_paused=false;
    exitMagic=true;

    score=0;
    trans=0;
    QSoundEffect* wel = new QSoundEffect(this);
    wel->setSource(QUrl("qrc:/music/effect/Welcome.wav"));
    wel->play();
    scoreTextLbl=new QLabel(this);
    scoreTextLbl->setGeometry(360,130,150,50);
    scoreTextLbl->setAlignment(Qt::AlignCenter);
    scoreTextLbl->setFont(QFont("Euclid",40,QFont::Normal));
    scoreTextLbl->setStyleSheet("QLabel{color:white;}");
    scoreTextLbl->setVisible(true);

    //禁用最大化按钮、设置窗口大小固定
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());
    //全屏
    QWidget::showFullScreen();
    //循环播放背景音乐

    sound=new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/music/background/music-2.wav"));
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();

    //窗口基本设置
    setWindowFlag(Qt::Window);  //设置为独立窗口
    setWindowTitle("Bejeweled");
    setWindowIcon(QIcon("://picture/app_icon.ico"));

    //设置窗口背景黑色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, Qt::black);
    this->setPalette(palette);

    //背景图片 棋盘框 分数框
    setAdaptedImg(":/picture/backdrop00.jpg",ui->oriBkLbl);
    setAdaptedImg(":/picture/frame.png",ui->borderLbl);
    setAdaptedImg(":/picture/scorepod.png",ui->scoreLbl);
    //设置变红的四周提示灯标签
    redBorder=new QLabel(this);
    redBorder->setGeometry(610, 2, 1055, 1073);
    setAdaptedImg(":/picture/frame_red.png",redBorder);
    redBorder->setAttribute(Qt::WA_TransparentForMouseEvents);
    //辅助label，不用管
    ui->menuLbl->setVisible(false);
    ui->hintLbl->setVisible(false);
    ui->pauseLbl->setVisible(false);
    ui->reSetBtn->setVisible(false);

    menuButton = new HoverButton(this);
    menuButton->setGeometry(ui->menuLbl->geometry());
    menuButton->setImage(":/picture/3balls/ball1.png",nullptr,ui->menuLbl->width(),ui->menuLbl->height(),ui->menuLbl);

    hintButton = new HoverButton(this);
    hintButton->setGeometry(ui->hintLbl->geometry());
    hintButton->setImage(":/picture/3balls/ball2.png",nullptr,ui->hintLbl->width(),ui->hintLbl->height(),ui->hintLbl);

    pauseButton = new HoverButton(this);
    pauseButton->setGeometry(ui->pauseLbl->geometry());
    pauseButton->setImage(":/picture/3balls/ball3.png",nullptr,ui->pauseLbl->width(),ui->pauseLbl->height(),ui->pauseLbl);

    reSetButton = new HoverButton(this);
    reSetButton->setGeometry(ui->reSetBtn->geometry());
    reSetButton->setImage(":/picture/Settingpage/dialogbutton.png",nullptr,ui->reSetBtn->width(),ui->reSetBtn->height(),ui->reSetBtn);

    //语言切换
    if(i==1){
        menuButton->showContent("MENU",20);
        menuButton->show();
        hintButton->showContent("HINT",30);
        hintButton->show();
        pauseButton->showContent("PAUSE",20);
        pauseButton->show();
        reSetButton->showContent("RESET",15);
        reSetButton->show();
    }
    if(i==0){
        menuButton->showContent("菜单",20);
        menuButton->show();
        hintButton->showContent("提示",30);
        hintButton->show();
        pauseButton->showContent("暂停",20);
        pauseButton->show();
        reSetButton->showContent("重置",15);
        reSetButton->show();
    }
    //设置鼠标-普通
    setCursor(QCursor(QPixmap("://picture/mouse1.png")));

    //进度条
    int totalTime=8000;
    progressBar = new MyProBar(this);
    progressBar->setRange(0,totalTime);
    progressBar->setValue(totalTime);
    progressBar->setTextVisible(false);
    progressBar->show();

    //动画

    //棋盘
    QPropertyAnimation* anim1 = new QPropertyAnimation(ui->borderLbl, "geometry");
    anim1->setDuration(500);
    anim1->setStartValue(QRect(610+1055, 2, 1055, 1073));
    anim1->setEndValue(QRect(610, 2, 1055, 1073));
    anim1->setEasingCurve(QEasingCurve::OutQuad);
    //分数版
    QPropertyAnimation* anim2 = new QPropertyAnimation(ui->scoreLbl, "geometry");
    anim2->setDuration(500);
    anim2->setStartValue(QRect(270, 0, 327, 178));

    anim2->setEndValue(QRect(270, 80, 327, 178));
    anim2->setEasingCurve(QEasingCurve::InQuad);
    //进度条
    QPropertyAnimation* anim3 = new QPropertyAnimation(progressBar,"geometry");
    anim3->setDuration(500);
    anim3->setStartValue(QRect(653+1055,1010,982,47));
    anim3->setEndValue(QRect(653,1010,982,47));
    anim3->setEasingCurve(QEasingCurve::OutQuad);
    //菜单栏
    QPropertyAnimation* anim4 = new QPropertyAnimation(menuButton,"geometry");
    anim4->setDuration(500);
    anim4->setStartValue(QRect(menuButton->x(),menuButton->y()+1000,menuButton->width(),menuButton->height()));
    anim4->setEndValue(QRect(menuButton->x(),menuButton->y(),menuButton->width(),menuButton->height()));
    anim4->setEasingCurve((QEasingCurve::OutQuad));
    QPropertyAnimation* anim5 = new QPropertyAnimation(hintButton,"geometry");
    anim5->setDuration(500);
    anim5->setStartValue(QRect(hintButton->x(),hintButton->y()+1000,hintButton->width(),hintButton->height()));
    anim5->setEndValue(QRect(hintButton->x(),hintButton->y(),hintButton->width(),hintButton->height()));
    anim5->setEasingCurve((QEasingCurve::OutQuad));
    QPropertyAnimation* anim6 = new QPropertyAnimation(pauseButton,"geometry");
    anim6->setDuration(500);
    anim6->setStartValue(QRect(pauseButton->x(),pauseButton->y()+1000,pauseButton->width(),pauseButton->height()));
    anim6->setEndValue(QRect(pauseButton->x(),pauseButton->y(),pauseButton->width(),pauseButton->height()));
    anim6->setEasingCurve((QEasingCurve::OutQuad));
    QPropertyAnimation* anim7 = new QPropertyAnimation(reSetButton,"geometry");
    anim7->setDuration(500);
    anim7->setStartValue(QRect(reSetButton->x(),reSetButton->y()+1000,reSetButton->width(),reSetButton->height()));
    anim7->setEndValue(QRect(reSetButton->x(),reSetButton->y(),reSetButton->width(),reSetButton->height()));
    anim6->setEasingCurve((QEasingCurve::OutQuad));

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(anim1);
    group->addAnimation(anim2);
    group->addAnimation(anim3);
    group->addAnimation(anim4);
    group->addAnimation(anim5);
    group->addAnimation(anim6);
    group->addAnimation(anim7);
    group->start(QAbstractAnimation::DeleteWhenStopped);

    Sleep(600);
    startGame();

    //开始记时
    progressTimer = new QTimer(this);
    progressTimer->setInterval(15);
    progressTimer->start();
    redBorderTimer=new QTimer(this);
    redBorderTimer->setInterval(500);
    timeoutTimer=new QTimer(this);
    timeoutTimer->setInterval(30);
    //timeoutTimer->start();
    connect(redBorderTimer, &QTimer::timeout, [=](){
        if(redBordershow==0){
            redBorder->show();
            redBordershow=1;
        }else{
            redBorder->hide();
            redBordershow=0;
        }
    });
    //设置超时标签
    outLabel=new QLabel(this);
    connect(timeoutTimer, &QTimer::timeout, [=](){
        if(trans<=1)
            trans=trans+0.01;
        else {
            is_acting=false;
            return;
        }
        outLabel->setGeometry(837,388,600,300);

        outLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        QPixmap input(":/picture/time_out.png");
        QImage image(input.size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter p(&image);
        p.setOpacity(trans);
        p.drawPixmap(0, 0, input);
        p.end();
        QPixmap output = QPixmap::fromImage(image);
        outLabel->setPixmap(output);
        outLabel->show();
        outLabel->setParent(this);

    });

    connect(progressTimer, &QTimer::timeout, [=](){
        if(!is_paused){
            if(progressBar->value() == 0){
                gameOver=true;
                forbidAll(true);
                //allFallOut();

                //计时结束
                if(!timeoutTimer->isActive()){
                    timeoutTimer->start();
                    if(effect)
                        delete effect;
                    effect = new QSoundEffect(this);
                    effect->setSource(QUrl("qrc:/music/effect/Time_Up.wav"));
                    effect->play();
                    //client->update(score);
                }
                if(redBorderTimer->isActive()){
                    redBorderTimer->stop();
                    redBorder->show();
                    redBordershow=1;
                }


            }
            else{
                // 1. 正常递减
                progressBar->setValue(progressBar->value() - 1);

                // 2. 判断比例
                double ratio = progressBar->value() / static_cast<double>(totalTime);

                if(ratio <= 0.25){
                    // 情况 A: 低于25%，开启警报
                    if(!redBorderTimer->isActive()){
                        redBorderTimer->start();
                    }
                }
                else {
                    // 情况 B: 高于25%（新增逻辑），关闭警报
                    if(redBorderTimer->isActive()){
                        redBorderTimer->stop();      // 停止闪烁定时器
                        redBorder->hide();           // 隐藏红色边框
                        redBordershow = 0;           // 重置标记变量位（如果有的话）
                    }
                }
            }
        }

    });

    connect(reSetButton,&HoverButton::clicked,[=]{
        if(is_acting||gameOver)
            return;
        reSetBoard();
    });

    connect(menuButton, &HoverButton::clicked, [=](){

        if(is_acting)
            return;

        // 清除提示高亮
        clearHintHighlight();

        client->update(score);
        sound->stop();
        this->hide();
        showStartPage();

        if(timeoutTimer)
            delete timeoutTimer;
        if(outLabel)
            delete outLabel;
        if(redBorderTimer)
            delete redBorderTimer;
        if(redBorder)
            delete redBorder;

        if(reSetButton)
            delete reSetButton;
        if(menuButton)
            delete menuButton;

        if(hintButton)
            delete hintButton;
        if(pauseBKLbl)
            delete pauseBKLbl;
        if(pauseBKgif){
            pauseBKgif->stop();
            delete pauseBKgif;
        }
        if(pauseTXLbl)
            delete pauseTXLbl;
        if(pauseButton)
            delete pauseButton;
        if(progressTimer)
            delete progressTimer;
        if(progressBar)
            delete progressBar;
        if(selectedLbl)
            delete selectedLbl;
        if(scoreTextLbl)
            delete scoreTextLbl;

        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                delete gems[i][j];
            }
        }
        delete boardWidget;
    }) ;

    // 修改提示按钮的点击事件
    connect(hintButton,&HoverButton::clicked,[=](){
        if(gameOver || is_acting)
            return;

        // 清除旧的提示
        clearHintHighlight();

        // 查找可交换的宝石对
        auto swapPair = findSwapHint();
        if(swapPair.first.x != -1 && swapPair.first.y != -1) {
            // 存储找到的交换对
            hintSwapPair = swapPair;

            // 创建第一个提示高亮框
            hintLabel1 = new QLabel(boardWidget);
            hintLabel1->setGeometry(swapPair.first.x * LEN, swapPair.first.y * LEN, LEN, LEN);
            hintLabel1->setStyleSheet("background-color: rgba(255, 255, 0, 50); border: 4px solid rgba(255, 215, 0, 200);");
            hintLabel1->setAttribute(Qt::WA_TransparentForMouseEvents);
            hintLabel1->show();

            // 创建第二个提示高亮框
            hintLabel2 = new QLabel(boardWidget);
            hintLabel2->setGeometry(swapPair.second.x * LEN, swapPair.second.y * LEN, LEN, LEN);
            hintLabel2->setStyleSheet("background-color: rgba(255, 255, 0, 50); border: 4px solid rgba(255, 215, 0, 200);");
            hintLabel2->setAttribute(Qt::WA_TransparentForMouseEvents);
            hintLabel2->show();

            // 创建并启动闪烁定时器
            hintBlinkTimer = new QTimer(this);
            hintBlinkTimer->setInterval(500); // 500ms闪烁一次
            connect(hintBlinkTimer, &QTimer::timeout, [=](){
                hintBlinkState = !hintBlinkState;
                if(hintLabel1) hintLabel1->setVisible(hintBlinkState);
                if(hintLabel2) hintLabel2->setVisible(hintBlinkState);
            });
            hintBlinkTimer->start();

            // 播放提示音效
            if(effect) delete effect;
            effect = new QSoundEffect(this);
            effect->setSource(QUrl("qrc:/music/effect/hint.wav"));
            effect->play();
        } else {
            // 没有找到可交换的位置
            QSoundEffect *noHint = new QSoundEffect(this);
            noHint->setSource(QUrl("qrc:/music/effect/no_hint.wav"));
            noHint->play();
        }
    });

    connect(pauseButton,&HoverButton::clicked,[=]{
        if(gameOver)
            return;
        if(!is_acting){
            if(!is_paused)
            {
                if(i==1){
                    pauseButton->label.setText("CONTINUE");
                }else{
                    pauseButton->label.setText("继续");
                }
                forbidAll(true);
                pauseBKLbl = new QLabel(boardWidget);
                pauseBKLbl->setGeometry(0,0,952, 952);
                pauseBKgif = new QMovie("://picture/starsBK.gif",QByteArray(),boardWidget);
                pauseBKgif->setScaledSize(QSize(952,952));
                pauseBKLbl->setMovie(pauseBKgif);
                pauseBKLbl->show();
                pauseBKgif->start();

                pauseTXLbl = new QLabel(boardWidget);
                pauseTXLbl->setGeometry(250,boardWidget->height()/2,440,30);
                pauseTXLbl->setText("The Game Has Been Paused.");
                pauseTXLbl->setAlignment(Qt::AlignCenter);
                pauseTXLbl->setFont(QFont("BoDoni MT",25, QFont::Normal));
                pauseTXLbl->setStyleSheet("QLabel{color:white;}");
                pauseTXLbl->setVisible(true);
                is_paused=true;
            }
            else if(is_paused){
                if(i==1){
                    pauseButton->label.setText("PAUSE");
                }else{
                    pauseButton->label.setText("暂停");
                }
                pauseBKLbl->hide();
                if(pauseBKgif){
                    pauseBKgif->stop();
                }

                if(pauseBKLbl){

                }

                if(pauseTXLbl)
                    pauseTXLbl->setText("");

                forbidAll(false);
                is_paused=false;
            }
        }
    });
}

void GameWidget::reSetBoard(){
    allFallOut();
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            unsigned int type;
            while(true) {
                type = QRandomGenerator::global()->bounded(1, DIFFICULITY + 1);
                bool matchH = (i >= 2 && getBaseType(gemType[i-1][j]) == type && getBaseType(gemType[i-2][j]) == type);
                bool matchV = (j >= 2 && getBaseType(gemType[i][j-1]) == type && getBaseType(gemType[i][j-2]) == type);
                if(!matchH && !matchV) break;
            }
            gemType[i][j] = type;
        }
    }

    //掉落动画
    QParallelAnimationGroup *group=new QParallelAnimationGroup;
    for(int j = 7; j >=0; --j){
        for(int i = 0; i <8 ; ++i){
            // gemType[i][j] = gemType[i][j] % static_cast<unsigned int>(DIFFICULITY) + 1;
            gems[i][j] = new Gem(static_cast<int>(gemType[i][j]), 118, i, j , boardWidget);
            gems[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            group->addAnimation(startfallAnimation(gems[i][j],j+1));
            connect(gems[i][j], &Gem::mouseClickedGem, this, &GameWidget::act);
        }
    }
    group->start();
    eliminateTimes=0;
    connect(group, &QParallelAnimationGroup::finished, [=] {

        forbidAll(false);
        is_acting=false;
        int s = updateBombList();
        score+=s;
        if(s!=0){
            Sleep(100);
            forbidAll(true);//禁用
            is_acting=true;
            eliminateBoard();
            exitMagic=false;
            eliminateTimes++;
        }else{
            eliminateTimes=0;
        }
        playSound(eliminateTimes);

        delete group;
    });

    return;
}

//将path的图片放置到label上，自适应label大小
void GameWidget::setAdaptedImg(QString path,QLabel *label)
{
    QImage image(path);
    QSize size=label->size();

    QImage image2=image.scaled(size,Qt::IgnoreAspectRatio);//重新调整图像大小以适应label
    label->setPixmap(QPixmap::fromImage(image2));//显示
}

void GameWidget::keyPressEvent(QKeyEvent *ev)
{
    //Esc退出全屏
    if(ev->key() == Qt::Key_Escape)
    {
        QWidget::showNormal();
        return;
    }
    //F11全屏
    if(ev->key() == Qt::Key_F11)
    {
        QWidget::showFullScreen();
        return;
    }
    QWidget::keyPressEvent(ev);
}

void GameWidget::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void GameWidget::forbidAll(bool forbid){//true forbit ,false release
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(gems[i][j])
                gems[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, forbid);
        }
    }

    // 如果禁用所有，也隐藏提示框
    if(forbid) {
        if(hintLabel1) hintLabel1->setVisible(false);
        if(hintLabel2) hintLabel2->setVisible(false);
        if(hintBlinkTimer && hintBlinkTimer->isActive()) {
            hintBlinkTimer->stop();
        }
    } else {
        // 恢复显示
        if(hintLabel1 && hintLabel2) {
            hintLabel1->setVisible(hintBlinkState);
            hintLabel2->setVisible(hintBlinkState);
            if(hintBlinkTimer) hintBlinkTimer->start();
        }
    }
}

//关于宝石的随机数生成
int GameWidget::randomGem(){
    return QRandomGenerator::global()->bounded(1, DIFFICULITY+1);
}

void GameWidget::playSound(int type){
    QString src=":/music/effect/effect";
    switch(type){
    case 0:
        return;
    case 1:
        src.append("1.wav");
        break;
    case 2:
        src.append("2.wav");
        break;

    case 3:
        src.append("3.wav");
        break;

    case 4:
        src.append("4.wav");
        break;
    default:
        src.append("5.wav");
        break;
    }
    if(effect)
        delete effect;
    effect = new QSoundEffect(this);
    effect->setSource(QUrl("qrc:" + src));
    effect->play();
}
void GameWidget::startGame(){
    boardWidget = new QWidget(this);

    boardWidget->show();
    boardWidget->setGeometry(665, 44, 952, 952);

    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            unsigned int type;
            while(true) {
                // 随机生成 1 到 DIFFICULTY 的颜色
                type = QRandomGenerator::global()->bounded(1, DIFFICULITY + 1);

                // 检查横向是否有三连 (检查左边两个)
                bool matchH = (i >= 2 &&
                               getBaseType(gemType[i-1][j]) == type &&
                               getBaseType(gemType[i-2][j]) == type);

                // 检查纵向是否有三连 (检查上边两个)
                bool matchV = (j >= 2 &&
                               getBaseType(gemType[i][j-1]) == type &&
                               getBaseType(gemType[i][j-2]) == type);

                // 如果没有三连，则该颜色合法
                if(!matchH && !matchV) break;
            }
            gemType[i][j] = type;
        }
    }

    //掉落动画
    QParallelAnimationGroup *group=new QParallelAnimationGroup;
    for(int j = 7; j >=0; --j){
        for(int i = 0; i <8 ; ++i){
            gemType[i][j] = gemType[i][j] % static_cast<unsigned int>(DIFFICULITY) + 1;
            gems[i][j] = new Gem(static_cast<int>(gemType[i][j]), 118, i, j , boardWidget);
            gems[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            group->addAnimation(startfallAnimation(gems[i][j],j+1));
            connect(gems[i][j], &Gem::mouseClickedGem, this, &GameWidget::act);
        }
    }
    group->start();
    eliminateTimes=0; // 初始化连击数

    connect(group, &QParallelAnimationGroup::finished, [=] {
        scoreTextLbl->setText("0");
        connect(this, &GameWidget::eliminateFinished, [=] {
            if(gameOver) return;

            // 1. 死局检测逻辑
            Point p = tipsdetect();
            if(p.x == -1 && p.y == -1){
                Sleep(200);
                reSetBoard();
                return;
            }

            // 2. 尝试合成特殊宝石 (全图扫描)
            bool hasSpecial = false;
            for(int i = 0; i < 8; ++i) {
                for(int j = 0; j < 8; ++j) {
                    if(tryGenerateSpecial(i, j, -1, -1)) {
                        hasSpecial = true;
                        eliminateTimes++;
                        int specialBonus = 50;
                        int finalScore = calculateComboScore(specialBonus, eliminateTimes);
                        score += finalScore;
                        scoreTextLbl->setText(QString::number(score));

                        qDebug() << "Special Gem Created! Times:" << eliminateTimes << " Score:" << finalScore;
                        break;
                    }
                }
                if(hasSpecial) break;
            }

            // 3. 如果没有合成特殊宝石，才走原来的普通消除逻辑
            if(!hasSpecial) {
                int rawScore = updateBombList();
                if(rawScore != 0){
                    Sleep(100);
                    forbidAll(true);
                    is_acting = true;
                    eliminateTimes++;
                    int finalScore = calculateComboScore(rawScore, eliminateTimes);
                    score += finalScore;
                    scoreTextLbl->setText(QString::number(score));
                    eliminateBoard();
                    exitMagic = false;
                } else {
                    eliminateTimes = 0;
                }
            }

            playSound(eliminateTimes);
        });
        forbidAll(false);
        is_acting=false;
        int s = updateBombList();
        score+=s;
        if(s!=0){
            Sleep(100);
            forbidAll(true);//禁用
            is_acting=true;
            eliminateBoard();
            exitMagic=false;
            eliminateTimes++;
        }else{
            eliminateTimes=0;
        }
        playSound(eliminateTimes);
        delete group;
    });
    connect(this,&GameWidget::finishCount,this,&GameWidget::finishAct);
}

void GameWidget::allFallOut(){
    is_acting=true;
    for(int j = 7; j >=0; --j){
        QParallelAnimationGroup* gruop = new QParallelAnimationGroup;
        for(int i = 0; i <8 ; ++i){
            QPropertyAnimation* anim = new QPropertyAnimation(gems[i][j],"geometry",boardWidget);
            anim->setDuration(700);
            anim->setStartValue(gems[i][j]->geometry());
            anim->setEndValue(QRect(gems[i][j]->geometry().x(),gems[i][j]->geometry().y()+1000,LEN,LEN));
            anim->setEasingCurve(QEasingCurve::InQuad);
            gruop->addAnimation(anim);
        }
        gruop->start(QAbstractAnimation::DeleteWhenStopped);
        Sleep(90);
    }
    Sleep(600);
    //    for(int j = 7; j >=0; --j){
    //        for(int i = 0; i <8 ; ++i){
    //           gems[i][j]->bomb();
    //        }
    //    }
    is_acting=false;
}

void GameWidget::finishAct() {
    if (FTime == 2) {
        FTime = 0;
        bombList.clear();
        int rawScore = updateBombList(); // 获取基础分

        if (rawScore != 0) {
            // 还有消除，继续消除，连击数+1
            eliminateTimes++;

            // 计算连击加成后的分数
            int finalScore = calculateComboScore(rawScore, eliminateTimes);
            score += finalScore;
            scoreTextLbl->setText(QString::number(score));

            qDebug() << "Cascade match found! Combo:" << eliminateTimes << " Score added:" << finalScore;

            forbidAll(true);
            is_acting = true;
            eliminateBoard();
            playSound(eliminateTimes);
        } else {
            // 没有更多消除了，连击结束（eliminateTimes 将在下一次消除开始前在 mouseClicked 或 eliminateFinished 中被重置）
            qDebug() << "No more matches";
            is_acting = false;
            forbidAll(false);
            emit eliminateFinished();
        }
    }
}

QPropertyAnimation* GameWidget::startfallAnimation(Gem *gem, int h){
    //每一行的掉落动画
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", boardWidget);
    animation->setDuration(static_cast<int>((sqrt((8-h)*300+1050)*20)));//时间由高度决定
    animation->setStartValue(QRect(gem->oriX, gem->oriY-118*h-(8-h)*80, LEN, LEN));//高度由行数决定
    animation->setEndValue(QRect(gem->oriX, gem->oriY, LEN, LEN));
    animation->setEasingCurve(QEasingCurve::Linear);
    return animation;
}


void GameWidget::act(Gem* gem){
    qDebug() << "act called on gem at (" << gem->x << "," << gem->y
             << ") type=" << gem->type << " specialType=" << gem->specialType;

    if (gameOver || is_acting || is_paused) {
        return;
    }

    // 清除提示高亮
    clearHintHighlight();
    hintArrowTimes = 6;

    int gemX = gem->x;
    int gemY = gem->y;

    // 如果当前没有宝石被选中，则选中点击的宝石
    if(selectedX == -1 && selectedY == -1){
        selectedX = gemX;
        selectedY = gemY;
        // 创建选框
        if (selectedLbl) {
            delete selectedLbl;
            selectedLbl = nullptr;
        }
        selectedLbl = new QLabel(boardWidget);
        QImage image("://picture/GameWidget/selected.png");
        selectedLbl->setPixmap(QPixmap::fromImage(image));
        selectedLbl->setGeometry(LEN*selectedX, LEN*selectedY, LEN, LEN);
        selectedLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        selectedLbl->show();

        // 让选中宝石旋转
        makeSpin(selectedX,selectedY);
    }
    // 如果已经有选中的宝石，并且点击的是相邻宝石，则交换
    else if( (abs(selectedX - gemX) == 1 && selectedY == gemY) ||
             (abs(selectedY - gemY) == 1 && selectedX == gemX) ){
        int SX = selectedX;
        int SY = selectedY;

        // 移除选框
        if (selectedLbl) {
            delete selectedLbl;
            selectedLbl = nullptr;
        }

        // 停止旋转
        makeStopSpin(SX, SY);

        selectedX = -1;
        selectedY = -1;

        // 交换前先禁用这两个宝石的点击
        if (gems[gemX][gemY]) gems[gemX][gemY]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        if (gems[SX][SY]) gems[SX][SY]->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        // 执行交换动画
        swap(SX, SY, gemX, gemY);
        Sleep(300);

        // 在对象矩阵中交换
        std::swap(gems[gemX][gemY], gems[SX][SY]);

        // 更新宝石的坐标
        if (gems[gemX][gemY]) {
            gems[gemX][gemY]->x = gemX;
            gems[gemX][gemY]->y = gemY;
        }
        if (gems[SX][SY]) {
            gems[SX][SY]->x = SX;
            gems[SX][SY]->y = SY;
        }

        // 交换类型数组
        std::swap(gemType[gemX][gemY], gemType[SX][SY]);

        // 尝试生成特殊宝石
        if (!tryGenerateSpecial(SX, SY, gemX, gemY)) {
            // 如果没有合成特殊宝石，走普通消除逻辑
            int currentScore = updateBombList();

            if(currentScore == 0) {
                // 没有形成消除，交换回来
                if(effect) delete effect;
                effect = new QSoundEffect(this);
                effect->setSource(QUrl("qrc:/music/effect/bad.wav"));
                effect->play();

                // 先交换回来
                std::swap(gemX, SX);
                std::swap(gemY, SY);

                // 执行交换回来的动画
                swap(SX, SY, gemX, gemY);
                Sleep(500);

                // 在对象矩阵中交换回来
                std::swap(gems[gemX][gemY], gems[SX][SY]);

                // 更新宝石的坐标
                if (gems[gemX][gemY]) {
                    gems[gemX][gemY]->x = gemX;
                    gems[gemX][gemY]->y = gemY;
                }
                if (gems[SX][SY]) {
                    gems[SX][SY]->x = SX;
                    gems[SX][SY]->y = SY;
                }

                // 交换类型数组
                std::swap(gemType[gemX][gemY], gemType[SX][SY]);

                // 重新启用点击
                is_acting = false;
                if (gems[gemX][gemY]) gems[gemX][gemY]->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                if (gems[SX][SY]) gems[SX][SY]->setAttribute(Qt::WA_TransparentForMouseEvents, false);

                // 清除禁止状态
                forbidAll(false);
            }
            else {
                // 有消除，开始消除流程
                score += currentScore;
                scoreTextLbl->setText(QString::number(score));

                // 禁用所有宝石点击
                forbidAll(true);
                is_acting = true;

                eliminateTimes = 1;
                playSound(eliminateTimes);

                // 开始普通消除
                eliminateBoard();
                exitMagic = false;
            }
        }
    }
    // 如果点击的是自己，取消选择
    else if (selectedX == gemX && selectedY == gemY) {
        // 移除选框
        if (selectedLbl) {
            delete selectedLbl;
            selectedLbl = nullptr;
        }

        // 停止旋转
        makeStopSpin(selectedX, selectedY);

        selectedX = -1;
        selectedY = -1;
    }
    // 点击其他宝石，改变选择
    else {
        // 移除旧选框
        if (selectedLbl) {
            delete selectedLbl;
            selectedLbl = nullptr;
        }

        // 停止旧宝石旋转
        makeStopSpin(selectedX, selectedY);

        // 选择新宝石
        selectedX = gemX;
        selectedY = gemY;

        // 创建新选框
        selectedLbl = new QLabel(boardWidget);
        QImage image("://picture/GameWidget/selected.png");
        selectedLbl->setPixmap(QPixmap::fromImage(image));
        selectedLbl->setGeometry(LEN*selectedX, LEN*selectedY, LEN, LEN);
        selectedLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        selectedLbl->show();

        // 让新选中宝石旋转
        makeSpin(selectedX, selectedY);
    }
}

void GameWidget::swap(int SX,int SY,int gemX,int gemY){
    // 确保宝石存在
    if (!gems[SX][SY] || !gems[gemX][gemY]) {
        return;
    }

    // 交换位置动画
    int xVal1 = gems[SX][SY]->x * LEN;
    int yVal1 = gems[SX][SY]->y * LEN;
    int width1 = gems[SX][SY]->width();
    int height1 = gems[SX][SY]->height();

    int xVal2 = gems[gemX][gemY]->x * LEN;
    int yVal2 = gems[gemX][gemY]->y * LEN;
    int width2 = gems[gemX][gemY]->width();
    int height2 = gems[gemX][gemY]->height();

    QParallelAnimationGroup* group = new QParallelAnimationGroup;

    // 第一个宝石的动画
    QPropertyAnimation *anim1 = new QPropertyAnimation(gems[SX][SY], "geometry", boardWidget);
    anim1->setDuration(300);
    anim1->setStartValue(QRect(xVal1, yVal1, width1, height1));
    anim1->setEndValue(QRect(xVal2, yVal2, width2, height2));
    anim1->setEasingCurve(QEasingCurve::Linear);
    group->addAnimation(anim1);

    // 第二个宝石的动画
    QPropertyAnimation *anim2 = new QPropertyAnimation(gems[gemX][gemY], "geometry", boardWidget);
    anim2->setDuration(300);
    anim2->setStartValue(QRect(xVal2, yVal2, width2, height2));
    anim2->setEndValue(QRect(xVal1, yVal1, width1, height1));
    anim2->setEasingCurve(QEasingCurve::Linear);
    group->addAnimation(anim2);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWidget::fallAnimation(Gem *gem, int h){
    gem->setAttribute(Qt::WA_TransparentForMouseEvents, true); //屏蔽鼠标
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", this);
    animation->setDuration(static_cast<int>(sqrt(h*118*900)));
    animation->setStartValue(gem->geometry());
    animation->setEndValue(QRect(gem->geometry().x(), gem->geometry().y() + LEN*h, gem->width(), gem->height()));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start();
    connect(animation,&QPropertyAnimation::finished,[=]{
        fallCount++;
        gem->setAttribute(Qt::WA_TransparentForMouseEvents, false); //取消屏蔽
        delete animation;
    });
}

void GameWidget::fillfallAnimation(Gem *gem, int h){
    gem->setAttribute(Qt::WA_TransparentForMouseEvents, true); //屏蔽鼠标
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", this);
    animation->setDuration(static_cast<int>(sqrt(h*118*900)));
    animation->setStartValue(gem->geometry());
    animation->setEndValue(QRect(gem->geometry().x(), gem->geometry().y() + LEN*h, gem->width(), gem->height()));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        fallCount++;
        gem->setAttribute(Qt::WA_TransparentForMouseEvents, false); //取消屏蔽

        // 所有动画完成后恢复交互
        if(fallCount==fallNum){
            is_acting=false;
            forbidAll(false); // 恢复交互
            eliminateFinished();//发射信号
        }
        delete animation;
    });
}

void GameWidget::fall(){
    for(int i = 0; i < 8; ++i)
        for(int j = 7; j >= 0; --j){
            if(fallHeight[i][j] != -1 && fallHeight[i][j] != 0 && gemType[i][j] != 100){
                gemType[i][j + fallHeight[i][j]] = gemType[i][j];
                gems[i][j]->setY(gems[i][j]->y + fallHeight[i][j]);
                gems[i][j + fallHeight[i][j]] = gems[i][j];
                gemType[i][j] = 100;
                fallAnimation(gems[i][j], fallHeight[i][j]);
            }
        }
}

void GameWidget::magicFall() {
    qDebug() << "Starting magicFall";

    // 重置计数器
    fallNum = 0;
    fallCount = 0;

    // 先计算哪些宝石需要掉落
    for (int i = 0; i < 8; ++i) {
        for (int j = 7; j >= 0; --j) {
            if (tHeight[i][j] > 0 && gems[i][j] && gemType[i][j] != 100) {
                fallNum++;
                qDebug() << "Gem at (" << i << "," << j << ") will fall by " << tHeight[i][j];
            }
        }
    }

    qDebug() << "Total gems to fall in magicFall: " << fallNum;

    if (fallNum == 0) {
        qDebug() << "No gems need to fall in magicFall, finishing directly";
        // 直接触发填充
        magicFill();
        return;
    }

    // 执行掉落动画
    for (int i = 0; i < 8; ++i) {
        for (int j = 7; j >= 0; --j) {
            if (tHeight[i][j] > 0 && gems[i][j] && gemType[i][j] != 100) {
                int newY = j + tHeight[i][j];

                if (newY < 8) {
                    // 移动宝石
                    gemType[i][newY] = gemType[i][j];
                    gems[i][j]->setY(newY);
                    gems[i][newY] = gems[i][j];

                    // 原位置清空
                    gemType[i][j] = 100;
                    gems[i][j] = nullptr;

                    // 执行掉落动画
                    QPropertyAnimation* animation = new QPropertyAnimation(gems[i][newY], "geometry", boardWidget);
                    animation->setDuration(static_cast<int>(sqrt(tHeight[i][j] * 100)));
                    animation->setStartValue(QRect(i * LEN, j * LEN, LEN, LEN));
                    animation->setEndValue(QRect(i * LEN, newY * LEN, LEN, LEN));
                    animation->setEasingCurve(QEasingCurve::InQuad);
                    animation->start();

                    connect(animation, &QPropertyAnimation::finished, [=] {
                        delete animation;
                        fallCount++;

                        qDebug() << "Fall animation finished: " << fallCount << "/" << fallNum;

                        if (fallCount >= fallNum) {
                            qDebug() << "All magicFall animations finished, starting magicFill";
                            // 所有动画完成后，开始填充
                            magicFill();
                        }
                    });
                }
            }
        }
    }
}

void GameWidget::magicFill() {
    qDebug() << "Starting magicFill";

    // 计算每列缺失的宝石数
    int lack[8] = {0};

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (gemType[i][j] == 100) {
                lack[i]++;
            }
        }
    }

    qDebug() << "Lack array: "
             << lack[0] << " " << lack[1] << " " << lack[2] << " " << lack[3] << " "
             << lack[4] << " " << lack[5] << " " << lack[6] << " " << lack[7];

    // 生成新宝石
    int newGemsCount = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < lack[i]; ++j) {
            // 从上方生成新宝石
            int newY = lack[i] - j - 1;
            gems[i][newY] = new Gem(randomGem(), LEN, i, newY, boardWidget, -lack[i]);
            gems[i][newY]->setGeometry(LEN * i, LEN * (-j - 1), LEN, LEN);
            gemType[i][newY] = static_cast<unsigned int>(gems[i][newY]->type);
            gems[i][newY]->specialType = 0; // 新宝石不是特殊宝石

            connect(gems[i][newY], &Gem::mouseClickedGem, this, &GameWidget::act);
            newGemsCount++;

            qDebug() << "Created new gem at (" << i << "," << newY << ") type=" << gemType[i][newY];
        }
    }

    qDebug() << "Created " << newGemsCount << " new gems";

    if (newGemsCount == 0) {
        qDebug() << "No new gems to fill, finishing act";
        // 没有新宝石，直接完成
        is_acting = false;
        forbidAll(false);
        emit eliminateFinished();
        return;
    }

    // 执行新宝石的掉落动画
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < lack[i]; ++j) {
            int newY = lack[i] - j - 1;
            if (gems[i][newY]) {
                QPropertyAnimation* animation = new QPropertyAnimation(gems[i][newY], "geometry", boardWidget);
                animation->setDuration(static_cast<int>(sqrt(lack[i] * 100)));
                animation->setStartValue(QRect(i * LEN, -LEN * (j + 1), LEN, LEN));
                animation->setEndValue(QRect(i * LEN, newY * LEN, LEN, LEN));
                animation->setEasingCurve(QEasingCurve::InQuad);
                animation->start();

                connect(animation, &QPropertyAnimation::finished, [=] {
                    delete animation;
                    fallCount++;

                    qDebug() << "Fill animation finished: " << fallCount << "/" << (fallNum + newGemsCount);

                    if (fallCount >= (fallNum + newGemsCount)) {
                        qDebug() << "All fill animations finished, restoring game";
                        // 所有动画完成后恢复游戏
                        is_acting = false;
                        forbidAll(false);
                        emit eliminateFinished();
                    }
                });
            }
        }
    }

    // 更新总掉落数
    fallNum += newGemsCount;
}

void GameWidget::fill(){
    QTimer::singleShot(100, this, [=](){
        int lack[8] = {0};

        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j)
                if(fallHeight[i][j] == -1)//这一列全消的情况
                    lack[i]++;
                else if(fallHeight[i][j] != 0){
                    lack[i] += fallHeight[i][j];//这一列不是全消的情况，直接利用fallHeight即可
                    break;
                }
        }

        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < lack[i]; ++j){
                gems[i][lack[i]-j-1] = new Gem(randomGem(), LEN, i, lack[i]-j-1, boardWidget, -lack[i]);
                gems[i][lack[i]-j-1]->setGeometry(LEN*i, LEN*(-j-1), LEN, LEN);
                gemType[i][lack[i]-j-1] = static_cast<unsigned int>(gems[i][lack[i]-j-1]->type);
                connect(gems[i][lack[i]-j-1], &Gem::mouseClickedGem, this, &GameWidget::act);
            }

        for(int i = 0; i < 8; ++i)
            for(int j = 7; j >= 0; --j){
                if(fallHeight[i][j] != -1 && fallHeight[i][j] != 0 && gemType[i][j] != 100){
                    fallNum++;
                }
            }

        for(int i = 0; i < 8; ++i)
            fallNum+=lack[i];

        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < lack[i]; ++j){
                fillfallAnimation(gems[i][lack[i]-j-1], lack[i]);
            }

        // 如果没有任何掉落动画，直接恢复交互
        if(fallNum == 0) {
            is_acting = false;
            forbidAll(false);
            emit eliminateFinished();
        }
    });
}

void GameWidget::generateMagic(int cX, int cY, int type, int time) {
    std::vector<Gem*> tempList;
    eliminateTimes++;
    int materialCount = tempList.size();
    int baseScore = materialCount * 10;
    score += calculateComboScore(baseScore, eliminateTimes);
    if (time == 1) {
        tempList = bombsToMakeMagic1;
        bombsToMakeMagic1.clear();
    } else if (time == 2) {
        tempList = bombsToMakeMagic2;
        bombsToMakeMagic2.clear();
    }

    // 动画组
    QParallelAnimationGroup* gather = new QParallelAnimationGroup;

    // 确保中心点不参与"移动向中心"的动画
    for (Gem* gem : tempList) {
        if (gem && (gem->x != cX || gem->y != cY)) {
            QPropertyAnimation* anim = new QPropertyAnimation(gem, "geometry", boardWidget);
            anim->setDuration(500);
            anim->setStartValue(gem->geometry());
            anim->setEndValue(gems[cX][cY]->geometry());
            anim->setEasingCurve(QEasingCurve::InBack);
            gather->addAnimation(anim);
        }
    }

    gather->start();

    connect(gather, &QParallelAnimationGroup::finished, [=] {
        delete gather;

        // 1. 设置生成的特殊宝石数据
        if (gems[cX][cY]) {
            // 获取原基础色
            unsigned int baseColor = getBaseType(gemType[cX][cY]);
            if (baseColor > 7 || baseColor < 1) baseColor = 1;

            // 时间球
            if (type == TIME_GEM_TYPE) {
                gemType[cX][cY] = SPECIAL_MASK + baseColor;
                gems[cX][cY]->type = static_cast<int>(SPECIAL_MASK + baseColor);
                gems[cX][cY]->specialType = TIME_GEM_TYPE;
                gems[cX][cY]->setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gems[cX][cY]->path_stable[baseColor]));
            }
            // 炸弹
            else if (type == BOMB_GEM_TYPE) {
                gemType[cX][cY] = SPECIAL_MASK + baseColor;
                gems[cX][cY]->type = static_cast<int>(SPECIAL_MASK + baseColor);
                gems[cX][cY]->specialType = BOMB_GEM_TYPE;
                gems[cX][cY]->setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gems[cX][cY]->path_stable[baseColor]));
            }
            // 十字消
            else if (type == CROSS_GEM_TYPE) {
                gemType[cX][cY] = SPECIAL_MASK + baseColor;
                gems[cX][cY]->type = static_cast<int>(SPECIAL_MASK + baseColor);
                gems[cX][cY]->specialType = CROSS_GEM_TYPE;
                gems[cX][cY]->setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gems[cX][cY]->path_stable[baseColor]));
            }

            // 添加边框特效
            if (gems[cX][cY]->specialBorder) delete gems[cX][cY]->specialBorder;
            gems[cX][cY]->specialBorder = new QLabel(gems[cX][cY]);
            gems[cX][cY]->specialBorder->setGeometry(0,0,LEN,LEN);
            gems[cX][cY]->specialBorder->setStyleSheet(getSpecialBorderStyle(type));
            gems[cX][cY]->specialBorder->show();
        }

        // 2. 清理被合并的宝石
        std::set<std::pair<int, int>> toRemove;
        std::vector<std::pair<int, int>> queue;

        // --- 第一阶段：计算受灾范围 ---
        for (Gem* gem : tempList) {
            if (gem && !(gem->x == cX && gem->y == cY)) {
                if (toRemove.find({gem->x, gem->y}) == toRemove.end()) {
                    toRemove.insert({gem->x, gem->y});
                    queue.push_back({gem->x, gem->y});
                }
            }
        }

        // BFS 核心循环
        int head = 0;
        while(head < queue.size()) {
            std::pair<int, int> curr = queue[head++];
            int gx = curr.first;
            int gy = curr.second;

            int sType = 0;
            if (gems[gx][gy]) sType = gems[gx][gy]->specialType;

            // 情况A：如果是炸弹，波及周围 3x3
            if (sType == BOMB_GEM_TYPE) {
                for (int nx = gx - 1; nx <= gx + 1; ++nx) {
                    for (int ny = gy - 1; ny <= gy + 1; ++ny) {
                        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && !(nx == cX && ny == cY)) {
                            if (toRemove.find({nx, ny}) == toRemove.end()) {
                                toRemove.insert({nx, ny});
                                queue.push_back({nx, ny});
                            }
                        }
                    }
                }
            }
            // 情况B：如果是十字消，波及整行整列
            else if (sType == CROSS_GEM_TYPE) {
                for (int i = 0; i < 8; ++i) {
                    // 行扫描
                    if (!(i == cX && gy == cY) && toRemove.find({i, gy}) == toRemove.end()) {
                        toRemove.insert({i, gy});
                        queue.push_back({i, gy});
                    }
                    // 列扫描
                    if (!(gx == cX && i == cY) && toRemove.find({gx, i}) == toRemove.end()) {
                        toRemove.insert({gx, i});
                        queue.push_back({gx, i});
                    }
                }
            }
        }

        // --- 第二阶段：安全销毁 ---
        for (auto const& pos : toRemove) {
            int rx = pos.first;
            int ry = pos.second;

            if (gems[rx][ry] != nullptr && gemType[rx][ry] != 100) {
                if (!(rx == cX && ry == cY)) {
                    int roadBaseScore = 20;
                    score += calculateComboScore(roadBaseScore, eliminateTimes);
                }

                gems[pos.first][pos.second]->hide();
                gems[pos.first][pos.second]->deleteLater();
                gems[rx][ry]->bomb();
                gems[pos.first][pos.second] = nullptr;
                gemType[pos.first][pos.second] = 100;
            }
        }

        QTimer::singleShot(150, [=] {
            // 3. 计算掉落列 (tHeight)
            for(int i=0; i<8; i++)
                for(int j=0; j<8; j++)
                    tHeight[i][j] = 0;

            // 标记空位
            for(int i=0; i<8; i++) {
                for(int j=0; j<8; j++) {
                    if(gemType[i][j] == 100) {
                        tHeight[i][j] = -1;
                    }
                }
            }

            // 计算每一列上方宝石需要下落的格数
            for(int i=0; i<8; i++) {
                for(int j=0; j<8; j++) {
                    if(tHeight[i][j] == -1) {
                        for(int k=j-1; k>=0; k--) {
                            if(tHeight[i][k] != -1) {
                                tHeight[i][k]++;
                            }
                        }
                    }
                }
            }

            // 4. 触发掉落
            magicFall();
        });
    });
}

// 辅助函数：根据特殊类型返回边框样式
QString GameWidget::getSpecialBorderStyle(int type) {
    switch(type) {
    case 1: // 爆炸宝石 - 蓝色边框
        return "border: 5px solid rgba(0, 0, 255, 200); background-color: rgba(0, 0, 255, 30);";
    case 2: // 十字消宝石 - 红色边框
        return "border: 5px solid rgba(255, 0, 0, 200); background-color: rgba(255, 0, 0, 30);";
    case 3: // 时间球宝石 - 绿色边框
        return "border: 5px solid rgba(0, 255, 0, 200); background-color: rgba(0, 255, 0, 30);";
    default:
        return "";
    }
}

int GameWidget::getBombsToMakeMagic(int cX, int cY, std::vector<Gem*>& bombsToMakeMagic, int time)
{
    // 清空传入的向量
    bombsToMakeMagic.clear();

    int leftCount = 0, rightCount = 0, upCount = 0, downCount = 0;

    // 获取基础颜色
    unsigned int baseType = getBaseType(gemType[cX][cY]);

    // 如果基础颜色为0（万能宝石）或100（空位），不生成特殊宝石
    if (baseType == 0 || baseType == 100) {
        return -1;
    }

    // 水平方向检测
    for (int i = cX - 1; i >= 0; i--) {
        if (getBaseType(gemType[i][cY]) == baseType) {
            leftCount++;
            bombsToMakeMagic.push_back(gems[i][cY]);
        } else {
            break;
        }
    }
    for (int i = cX + 1; i < 8; i++) {
        if (getBaseType(gemType[i][cY]) == baseType) {
            rightCount++;
            bombsToMakeMagic.push_back(gems[i][cY]);
        } else {
            break;
        }
    }

    // 重新清空，检测垂直方向
    bombsToMakeMagic.clear();

    // 垂直方向检测
    for (int j = cY - 1; j >= 0; j--) {
        if (getBaseType(gemType[cX][j]) == baseType) {
            upCount++;
            bombsToMakeMagic.push_back(gems[cX][j]);
        } else {
            break;
        }
    }
    for (int j = cY + 1; j < 8; j++) {
        if (getBaseType(gemType[cX][j]) == baseType) {
            downCount++;
            bombsToMakeMagic.push_back(gems[cX][j]);
        } else {
            break;
        }
    }

    // 计算水平和垂直方向的连续宝石数（包括自身）
    int horizontalTotal = leftCount + rightCount + 1;
    int verticalTotal = upCount + downCount + 1;

    // 重新清空，根据条件重新填充
    bombsToMakeMagic.clear();

    int magicType = -1; // -1无，0普通三消，1爆炸，2十字消，3万能

    // 检查是否为直线消除
    bool isHorizontalLine = (horizontalTotal >= 3);
    bool isVerticalLine = (verticalTotal >= 3);

    // 优先检查直线消除
    if (isHorizontalLine) {
        // 水平直线消除
        if (horizontalTotal == 5) {
            // 5连消直线：生成十字消宝石
            magicType = CROSS_GEM_TYPE;
            for (int i = cX - leftCount; i <= cX + rightCount; i++) {
                if (i != cX) bombsToMakeMagic.push_back(gems[i][cY]);
            }
        }
        else if (horizontalTotal == 4 || horizontalTotal == 6) {
            // 4连消或6连消直线：无特殊效果，按普通三消处理
            magicType = 0;
            for (int i = cX - leftCount; i <= cX + rightCount; i++) {
                if (i != cX) bombsToMakeMagic.push_back(gems[i][cY]);
            }
        }
        else if (horizontalTotal >= 3) {
            // 3连消：普通三消
            magicType = 0;
            for (int i = cX - leftCount; i <= cX + rightCount; i++) {
                if (i != cX) bombsToMakeMagic.push_back(gems[i][cY]);
            }
        }
    }
    else if (isVerticalLine) {
        // 垂直直线消除
        if (verticalTotal == 5) {
            // 5连消直线：生成十字消宝石
            magicType = CROSS_GEM_TYPE;
            for (int j = cY - upCount; j <= cY + downCount; j++) {
                if (j != cY) bombsToMakeMagic.push_back(gems[cX][j]);
            }
        }
        else if (verticalTotal == 4 || verticalTotal == 6) {
            // 4连消或6连消直线：无特殊效果，按普通三消处理
            magicType = 0;
            for (int j = cY - upCount; j <= cY + downCount; j++) {
                if (j != cY) bombsToMakeMagic.push_back(gems[cX][j]);
            }
        }
        else if (verticalTotal >= 3) {
            // 3连消：普通三消
            magicType = 0;
            for (int j = cY - upCount; j <= cY + downCount; j++) {
                if (j != cY) bombsToMakeMagic.push_back(gems[cX][j]);
            }
        }
    }

    // 如果不是直线消除或者直线消除少于5个，检查L型和T型
    if (magicType == -1 || magicType == 0) {
        // 收集所有相邻相同宝石（用于L型和T型检测）
        std::vector<Gem*> allAdjacent;

        // 水平方向
        for (int i = cX - leftCount; i <= cX + rightCount; i++) {
            if (i != cX && getBaseType(gemType[i][cY]) == baseType) {
                allAdjacent.push_back(gems[i][cY]);
            }
        }

        // 垂直方向
        for (int j = cY - upCount; j <= cY + downCount; j++) {
            if (j != cY && getBaseType(gemType[cX][j]) == baseType) {
                allAdjacent.push_back(gems[cX][j]);
            }
        }

        // 检查对角线上的相同宝石（用于L型和T型检测）
        // 上左
        if (cX > 0 && cY > 0 && getBaseType(gemType[cX-1][cY-1]) == baseType) {
            allAdjacent.push_back(gems[cX-1][cY-1]);
        }
        // 上右
        if (cX < 7 && cY > 0 && getBaseType(gemType[cX+1][cY-1]) == baseType) {
            allAdjacent.push_back(gems[cX+1][cY-1]);
        }
        // 下左
        if (cX > 0 && cY < 7 && getBaseType(gemType[cX-1][cY+1]) == baseType) {
            allAdjacent.push_back(gems[cX-1][cY+1]);
        }
        // 下右
        if (cX < 7 && cY < 7 && getBaseType(gemType[cX+1][cY+1]) == baseType) {
            allAdjacent.push_back(gems[cX+1][cY+1]);
        }

        // 计算总宝石数（包括自身）
        int totalGems = 1 + allAdjacent.size();

        // 检查是否为L型或T型
        bool isLShape = false;
        bool isTShape = false;

        // L型检测：至少4个宝石，形成直角
        // 简单检测：既有水平方向的匹配，也有垂直方向的匹配
        if (leftCount + rightCount >= 1 && upCount + downCount >= 1) {
            // 至少有水平方向1个和垂直方向1个
            if (totalGems >= 4) {
                isLShape = true;
            }
        }

        // T型检测：至少4个宝石，形成T字形
        // 简单检测：一个方向有3个或更多，另一个方向有1个或更多
        if ((horizontalTotal >= 3 && verticalTotal >= 2) ||
            (verticalTotal >= 3 && horizontalTotal >= 2)) {
            if (totalGems >= 4) {
                isTShape = true;
            }
        }

        // 如果检测到T型，生成爆炸宝石
        if (isTShape) {
            magicType = BOMB_GEM_TYPE;
            bombsToMakeMagic = allAdjacent;
        }
        // 如果检测到L型，生成万能宝石
        else if (isLShape) {
            magicType = TIME_GEM_TYPE;
            bombsToMakeMagic = allAdjacent;
        }
        // 如果没有检测到特殊形状，但直线消除已经处理过（magicType可能是0）
        else if (magicType == -1) {
            // 检查是否满足普通三消条件
            if (horizontalTotal >= 3) {
                magicType = 0;
                for (int i = cX - leftCount; i <= cX + rightCount; i++) {
                    if (i != cX) bombsToMakeMagic.push_back(gems[i][cY]);
                }
            } else if (verticalTotal >= 3) {
                magicType = 0;
                for (int j = cY - upCount; j <= cY + downCount; j++) {
                    if (j != cY) bombsToMakeMagic.push_back(gems[cX][j]);
                }
            }
        }
    }

    // 确保消除列表中没有重复的宝石
    std::sort(bombsToMakeMagic.begin(), bombsToMakeMagic.end());
    bombsToMakeMagic.erase(std::unique(bombsToMakeMagic.begin(), bombsToMakeMagic.end()), bombsToMakeMagic.end());

    // 存储结果
    if (time == 1) {
        bombsToMakeMagic1 = bombsToMakeMagic;
    } else if (time == 2) {
        bombsToMakeMagic2 = bombsToMakeMagic;
    }

    return magicType;
}

// 获取宝石的基础颜色类型
unsigned int GameWidget::getBaseType(unsigned int type) {
    if (type == 100) return 100; // 空位
    if (type == 0) return 0;     // 普通万能宝石

    // 如果是特殊宝石，获取基础颜色
    if (type >= SPECIAL_MASK) {
        return type - SPECIAL_MASK; // 减去掩码得到基础颜色
    }

    return type; // 普通宝石
}
// 获取宝石的特殊类型（如果不是特殊宝石返回0）
unsigned int GameWidget::getSpecialType(unsigned int type) {
    if (type >= SPECIAL_MASK) {
        // 特殊宝石，需要检查对应的Gem对象
        // 这个函数需要配合坐标使用，所以我们添加一个新的函数
        return 0; // 临时返回0，稍后实现
    }

    // 普通万能宝石
    if (type == 0) return TIME_GEM_TYPE;

    return 0; // 不是特殊宝石
}

int GameWidget::getSpecialGemType(int x, int y) {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return 0;
    if (!gems[x][y]) return 0;
    return gems[x][y]->specialType;
}
bool GameWidget::typesMatch(unsigned int type1, unsigned int type2) {
    // 如果任意一个是空位，不匹配
    if (type1 == 100 || type2 == 100) {
        return false;
    }

    // 如果任意一个是万能宝石（类型0），可以匹配任何非空类型
    if (type1 == 0 || type2 == 0) {
        return true;
    }

    // 获取基础颜色
    unsigned int base1 = getBaseType(type1);
    unsigned int base2 = getBaseType(type2);

    // 基础颜色相同则匹配
    return base1 == base2;
}

bool GameWidget::checkGemMatch(int x1, int y1, int x2, int y2) {
    // 确保坐标有效
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 ||
        x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        return false;
    }

    // 如果任意一个位置没有宝石，不匹配
    if (!gems[x1][y1] || !gems[x2][y2]) {
        return false;
    }

    unsigned int type1 = gemType[x1][y1];
    unsigned int type2 = gemType[x2][y2];

    // 如果任意一个是空位，不匹配
    if (type1 == 100 || type2 == 100) {
        return false;
    }

    // 获取基础颜色
    unsigned int base1 = getBaseType(type1);
    unsigned int base2 = getBaseType(type2);

    // 基础颜色相同则匹配
    return (base1 == base2);
}

bool GameWidget::tryGenerateSpecial(int x1, int y1, int x2, int y2) {
    int specialType1 = -1, specialType2 = -1;
    std::vector<Gem*> adjacentGems1, adjacentGems2;

    // 1. 检测两个位置是否满足特殊宝石条件
    bool hasSpecial1 = checkSpecialGemCondition(x1, y1, specialType1, adjacentGems1);
    bool hasSpecial2 = (x2 != -1) ? checkSpecialGemCondition(x2, y2, specialType2, adjacentGems2) : false;

    if (hasSpecial1 || hasSpecial2) {
        // 2. 确定最终生成的位置（优先级逻辑）
        int finalX, finalY, finalType;
        std::vector<Gem*> finalAdjacentGems;

        if (hasSpecial1 && (!hasSpecial2 || specialType1 >= specialType2)) {
            finalX = x1; finalY = y1; finalType = specialType1; finalAdjacentGems = adjacentGems1;
        } else {
            finalX = x2; finalY = y2; finalType = specialType2; finalAdjacentGems = adjacentGems2;
        }

        // 3. 准备数据并触发合成
        is_acting = true;
        forbidAll(true);
        bombsToMakeMagic1 = finalAdjacentGems;
        generateMagic(finalX, finalY, finalType, 1);
        QTimer::singleShot(800, [=]() {
            // 此时 generateMagic 内部的 magicFall 应该已经完成
            // 我们在这里执行一次全图扫描
            int cascadeScore = updateBombList();

            if (cascadeScore > 0) {
                // 如果掉落后形成了新的消除，进入递归消除流程
                score += cascadeScore;
                scoreTextLbl->setText(QString::number(score));
                eliminateTimes++;
                playSound(eliminateTimes);
                eliminateBoard(); // 调用普通消除函数处理后续
            } else {
                // 如果没有新消除，结束回合
                is_acting = false;
                forbidAll(false);
                eliminateTimes = 1;
            }
        });

        playSound(1); // 播放合成音效
        return true;
    }
    return false;
}

int GameWidget::updateBombList() {
    if (gameOver)
        return 0;

    int score = 0;
    bool eliminating[8][8] = {false};

    // 先清空之前的消除列表
    bombList.clear();

    // 逐行检测连续3个或以上的匹配
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) { // 只需检测到第6列
            // 跳过已标记消除的位置
            if (eliminating[i][j]) continue;

            int matchCount = 1;
            // 向后检查匹配的宝石
            for (int k = j + 1; k < 8; k++) {
                if (checkGemMatch(i, j, i, k)) {
                    matchCount++;
                } else {
                    break;
                }
            }

            if (matchCount >= 3) {
                // 标记这些宝石为消除
                for (int k = j; k < j + matchCount; k++) {
                    eliminating[i][k] = true;
                }
                j += (matchCount - 1);
            }
        }
    }

    // 逐列检测连续3个或以上的匹配
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 6; i++) { // 只需检测到第6行
            if (eliminating[i][j]) continue;

            int matchCount = 1;
            for (int k = i + 1; k < 8; k++) {
                if (checkGemMatch(i, j, k, j)) {
                    matchCount++;
                } else {
                    break;
                }
            }

            if (matchCount >= 3) {
                for (int k = i; k < i + matchCount; k++) {
                    eliminating[k][j] = true;
                }
                i += (matchCount - 1);
            }
        }
    }

    // 收集要消除的宝石
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (eliminating[i][j] && gems[i][j]) {
                bombList.push_back(gems[i][j]);
                score += 10;
            }
        }
    }

    return score;
}

Point GameWidget::tipsdetect(){
    int types[12][12];
    for (int i = 0; i < 10; ++i) {
        types[0][i]=-1;
        types[1][i]=-1;
        types[10][i]=-1;
        types[11][i]=-1;
        types[i][0]=-1;
        types[i][1]=-1;
        types[i][10]=-1;
        types[i][11]=-1;
    }
    for(int i=2;i<=9;i++){
        for(int j=2;j<=9;j++){
            types[i][j]=gems[i-2][j-2]->type;
        }
    }
    //分两种情况讨论
    for(int i=2;i<=9;i++){
        for (int j=2;j<=9;j++) {
            int t=types[i][j];
            //第一，检测连着两个相同类型的宝石，并检测其周围
            if(t==types[i][j+1]){
                if(t==types[i][j-2])
                    return Point(i-2,j-4);
                if(t==types[i-1][j-1])
                    return Point(i-3,j-3);
                if(t==types[i+1][j-1])
                    return Point(i-1,j-3);
                if(t==types[i-1][j+2])
                    return Point(i-3,j);
                if(t==types[i+1][j+2])
                    return Point(i-1,j);
                if(t==types[i][j+3])
                    return Point(i-2,j+1);

            }
            //第二，检测两个跳着的宝石，看是否有宝石能填补中间位置
            if(t==types[i][j+2]){
                if(t==types[i-1][j+1])
                    return Point(i-3,j-1);
                if(t==types[i+1][j+1])
                    return Point(i-1,j-1);
            }
        }
    }
    //列检测
    for(int i=2;i<=9;i++){
        for (int j=2;j<=9;j++) {
            int t=types[j][i];
            if(t==types[j+1][i]){
                if(t==types[j-2][i])
                    return Point(j-4,i-2);
                if(t==types[j-1][i-1])
                    return Point(j-3,i-3);
                if(t==types[j-1][i+1])
                    return Point(j-3,i-1);
                if(t==types[j+2][i-1])
                    return Point(j,i-3);
                if(t==types[j+2][i+1])
                    return Point(j,i-1);
                if(t==types[j+3][i])
                    return Point(j+1,i-2);

            }
            if(t==types[j+2][i]){
                if(t==types[j+1][i-1])
                    return Point(j-1,i-3);
                if(t==types[j+1][i+1])
                    return Point(j-1,i-1);

            }
        }
    }

    return Point(-1,-1);
}

void GameWidget::eliminateBoard() {
    if (gameOver) return;
    is_acting = true;

    if (bombList.empty()) {
        is_acting = false;
        forbidAll(false);
        return;
    }

    // 使用索引遍历，允许循环中添加新元素
    for (size_t i = 0; i < bombList.size(); ++i) {
        Gem* currentGem = bombList[i];
        if (!currentGem) continue;

        int x = currentGem->x;
        int y = currentGem->y;
        int sType = currentGem->specialType;

        std::vector<Gem*> affectedGems;

        // --- 时间球处理逻辑 ---
        if (sType == TIME_GEM_TYPE) {
            // 增加10秒时间 (10000毫秒)
            int timeBonus = 667;
            if (progressBar) {
                int currentVal = progressBar->value();
                int maxVal = progressBar->maximum();
                progressBar->setValue(qMin(currentVal + timeBonus, maxVal));
            }
            qDebug() << "Time Gem Activated: +10 seconds";
        }
        // --- 炸弹处理逻辑 ---
        else if (sType == BOMB_GEM_TYPE) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && gems[nx][ny]) {
                        affectedGems.push_back(gems[nx][ny]);
                    }
                }
            }
        }
        // --- 十字消处理逻辑 ---
        else if (sType == CROSS_GEM_TYPE) {
            for (int k = 0; k < 8; k++) {
                if (gems[k][y]) affectedGems.push_back(gems[k][y]); // 整行
                if (gems[x][k]) affectedGems.push_back(gems[x][k]); // 整列
            }
        }

        // --- 将受影响的宝石加入列表 (去重) ---
        for (Gem* target : affectedGems) {
            if (target == currentGem) continue;

            // 检查是否已经在列表中
            bool exists = false;
            for (const auto& existing : bombList) {
                if (existing == target) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                bombList.push_back(target);
                // 连锁消除加分
                int extraScore = 20;
                score += calculateComboScore(extraScore, eliminateTimes);
                scoreTextLbl->setText(QString::number(score));
            }
        }
    }
    // 第一步：重置掉落高度数组
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            fallHeight[i][j] = 0;

    // 第二步：标记要消除的宝石并计算掉落高度
    for (unsigned int i = 0; i < bombList.size(); i++) {
        if (bombList[i]) {
            int x = bombList[i]->x;
            int y = bombList[i]->y;

            // 确保坐标有效
            if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                // 标记为消除
                gemType[x][y] = 100;
                fallHeight[x][y] = -1;

                // 计算上方宝石的掉落高度
                for (int k = y - 1; k >= 0; --k)
                    if (fallHeight[x][k] != -1)
                        fallHeight[x][k]++;
            }
        }
    }

    // 第三步：播放消除动画
    QParallelAnimationGroup* animGroup = new QParallelAnimationGroup;

    for (unsigned int i = 0; i < bombList.size(); i++) {
        if (bombList[i]) {
            QPropertyAnimation* anim = new QPropertyAnimation(bombList[i], "geometry", boardWidget);
            anim->setDuration(300);
            anim->setStartValue(bombList[i]->geometry());
            anim->setEndValue(QRect(bombList[i]->geometry().center(), QSize(1, 1)));
            anim->setEasingCurve(QEasingCurve::InQuad);
            animGroup->addAnimation(anim);
        }
    }

    animGroup->start();

    // 动画结束后进行实际消除
    connect(animGroup, &QParallelAnimationGroup::finished, [=] {
        delete animGroup;

        // 第四步：实际消除宝石
        for (unsigned int i = 0; i < bombList.size(); i++) {
            if (bombList[i]) {
                int x = bombList[i]->x;
                int y = bombList[i]->y;

                // 确保坐标有效
                if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                    // 删除特殊边框
                    if (bombList[i]->specialBorder) {
                        delete bombList[i]->specialBorder;
                        bombList[i]->specialBorder = nullptr;
                    }

                    // 删除宝石
                    bombList[i]->bomb();
                    gems[x][y] = nullptr;
                    gemType[x][y] = 100;
                }
            }
        }

        bombList.clear();

        // 第五步：触发掉落和填充
        fall();
        fill();
    });
}

void GameWidget::makeSpin(int SX,int SY){
    if(!gems[SX][SY]||SX==-1)
        return;
    gems[SX][SY]->setStyleSheet(QString("QPushButton{background-color:transparent;border:0px;}"));
    gems[SX][SY]->spinLabel = new QLabel(gems[SX][SY]);
    gems[SX][SY]->spinLabel->setGeometry(0,0,gems[SX][SY]->width(), gems[SX][SY]->height());
    gems[SX][SY]->spinGif = new QMovie(gems[SX][SY]->path_dynamic[gems[SX][SY]->type], QByteArray(), gems[SX][SY]);
    gems[SX][SY]->spinGif->setScaledSize(QSize(gems[SX][SY]->width(), gems[SX][SY]->height()));
    gems[SX][SY]->spinLabel->setMovie(gems[SX][SY]->spinGif);
    gems[SX][SY]->spinLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    gems[SX][SY]->spinLabel->show();
    gems[SX][SY]->spinGif->start();
}

void GameWidget::makeStopSpin(int SX,int SY){
    if(!gems[SX][SY]||SX==-1)
        return;
    if(gems[SX][SY]->spinGif)
        gems[SX][SY]->spinGif->stop();
    if(gems[SX][SY]->spinLabel)
        gems[SX][SY]->spinLabel->clear();

    gems[SX][SY]->setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gems[SX][SY]->path_stable[gems[SX][SY]->type]));
    gems[SX][SY]->setIconSize(QSize(LEN, LEN));
}

// 新添加的提示功能函数实现

// 清除提示高亮
void GameWidget::clearHintHighlight() {
    if(hintBlinkTimer) {
        hintBlinkTimer->stop();
        delete hintBlinkTimer;
        hintBlinkTimer = nullptr;
    }

    if(hintLabel1) {
        delete hintLabel1;
        hintLabel1 = nullptr;
    }

    if(hintLabel2) {
        delete hintLabel2;
        hintLabel2 = nullptr;
    }

    hintBlinkState = false;
}

// 查找可交换的宝石对
std::pair<Point, Point> GameWidget::findSwapHint() {
    // 遍历所有相邻的宝石对
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            // 检查右侧邻居
            if(i + 1 < 8) {
                if(checkThreeMatch(i, j, i + 1, j)) {
                    return std::make_pair(Point(i, j), Point(i + 1, j));
                }
            }
            // 检查下方邻居
            if(j + 1 < 8) {
                if(checkThreeMatch(i, j, i, j + 1)) {
                    return std::make_pair(Point(i, j), Point(i, j + 1));
                }
            }
        }
    }
    return std::make_pair(Point(-1, -1), Point(-1, -1));
}

// 检查交换两个宝石是否能形成三连消
bool GameWidget::checkThreeMatch(int x1, int y1, int x2, int y2) {
    // 临时交换宝石类型
    std::swap(gemType[x1][y1], gemType[x2][y2]);

    bool foundMatch = false;

    // 检查交换后是否形成三连消
    // 检查行
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 6; j++) { // 只需要检查到第6列，因为需要连续3个
            unsigned int baseType1 = getBaseType(gemType[i][j]);
            unsigned int baseType2 = getBaseType(gemType[i][j+1]);
            unsigned int baseType3 = getBaseType(gemType[i][j+2]);

            // 如果任意一个是万能宝石（0或100），视为匹配
            bool type1Universal = (baseType1 == 0 || baseType1 == 100);
            bool type2Universal = (baseType2 == 0 || baseType2 == 100);
            bool type3Universal = (baseType3 == 0 || baseType3 == 100);

            // 检查是否匹配：三个类型相同，或者有万能宝石参与
            bool match = false;
            if(type1Universal && type2Universal && type3Universal) {
                match = true; // 三个都是万能宝石
            } else if(type1Universal && baseType2 == baseType3) {
                match = true; // 第一个是万能宝石，后两个相同
            } else if(type2Universal && baseType1 == baseType3) {
                match = true; // 第二个是万能宝石，第一个和第三个相同
            } else if(type3Universal && baseType1 == baseType2) {
                match = true; // 第三个是万能宝石，前两个相同
            } else if(baseType1 == baseType2 && baseType2 == baseType3) {
                match = true; // 三个都相同
            }

            if(match && baseType1 != 100 && baseType2 != 100 && baseType3 != 100) {
                foundMatch = true;
                break;
            }
        }
        if(foundMatch) break;
    }

    // 检查列
    if(!foundMatch) {
        for(int i = 0; i < 6; i++) { // 只需要检查到第6行
            for(int j = 0; j < 8; j++) {
                unsigned int baseType1 = getBaseType(gemType[i][j]);
                unsigned int baseType2 = getBaseType(gemType[i+1][j]);
                unsigned int baseType3 = getBaseType(gemType[i+2][j]);

                // 如果任意一个是万能宝石（0或100），视为匹配
                bool type1Universal = (baseType1 == 0 || baseType1 == 100);
                bool type2Universal = (baseType2 == 0 || baseType2 == 100);
                bool type3Universal = (baseType3 == 0 || baseType3 == 100);

                // 检查是否匹配：三个类型相同，或者有万能宝石参与
                bool match = false;
                if(type1Universal && type2Universal && type3Universal) {
                    match = true; // 三个都是万能宝石
                } else if(type1Universal && baseType2 == baseType3) {
                    match = true; // 第一个是万能宝石，后两个相同
                } else if(type2Universal && baseType1 == baseType3) {
                    match = true; // 第二个是万能宝石，第一个和第三个相同
                } else if(type3Universal && baseType1 == baseType2) {
                    match = true; // 第三个是万能宝石，前两个相同
                } else if(baseType1 == baseType2 && baseType2 == baseType3) {
                    match = true; // 三个都相同
                }

                if(match && baseType1 != 100 && baseType2 != 100 && baseType3 != 100) {
                    foundMatch = true;
                    break;
                }
            }
            if(foundMatch) break;
        }
    }

    // 恢复交换
    std::swap(gemType[x1][y1], gemType[x2][y2]);

    return foundMatch;
}

// 添加缺失的函数实现
void GameWidget::showHint() {
    // 提示功能已经在hintButton的clicked事件中实现
}

bool GameWidget::hasMatch(int x1, int y1, int x2, int y2) {
    return checkThreeMatch(x1, y1, x2, y2);
}
bool GameWidget::checkSpecialGemCondition(int cX, int cY, int& specialType, std::vector<Gem*>& adjacentGems) {
    adjacentGems.clear();
    specialType = -1;

    if (cX < 0 || cX >= 8 || cY < 0 || cY >= 8 || !gems[cX][cY]) return false;

    unsigned int baseColor = getBaseType(gemType[cX][cY]);
    if (baseColor == 0 || baseColor == 100 || baseColor > 7) return false;

    // 1. 统计四个方向的连续宝石数量
    int left = 0, right = 0, up = 0, down = 0;

    // 向左
    for (int i = cX - 1; i >= 0; i--) {
        if (getBaseType(gemType[i][cY]) == baseColor) left++;
        else break;
    }
    // 向右
    for (int i = cX + 1; i < 8; i++) {
        if (getBaseType(gemType[i][cY]) == baseColor) right++;
        else break;
    }
    // 向上
    for (int j = cY - 1; j >= 0; j--) {
        if (getBaseType(gemType[cX][j]) == baseColor) up++;
        else break;
    }
    // 向下
    for (int j = cY + 1; j < 8; j++) {
        if (getBaseType(gemType[cX][j]) == baseColor) down++;
        else break;
    }

    int hTotal = left + right + 1; // 水平总数
    int vTotal = up + down + 1;    // 垂直总数

    // 2. 收集涉及的宝石
    if (hTotal >= 3) {
        for (int i = cX - left; i <= cX + right; i++) {
            if (i != cX && gems[i][cY]) adjacentGems.push_back(gems[i][cY]);
        }
    }
    if (vTotal >= 3) {
        for (int j = cY - up; j <= cY + down; j++) {
            if (j != cY && gems[cX][j]) adjacentGems.push_back(gems[cX][j]);
        }
    }

    // 3. 判定逻辑 (优先级：5连 > T型 > L型)

    // --- 情况A: 5连 (生成十字消) ---
    if (hTotal >= 5 || vTotal >= 5) {
        specialType = CROSS_GEM_TYPE; // 十字消
        qDebug() << "Condition Met: 5-Line -> Cross";
        return true;
    }

    // --- 情况B: 双向消除 (T型 或 L型) ---
    if (hTotal >= 3 && vTotal >= 3) {
        // 判断 T型 还是 L型
        bool hMiddle = (left > 0 && right > 0); // 水平在中间
        bool vMiddle = (up > 0 && down > 0);    // 垂直在中间

        if (hMiddle || vMiddle) {
            // 只要有一个方向是在中间，就是 T型 (生成炸弹)
            specialType = BOMB_GEM_TYPE;
            qDebug() << "Condition Met: T-Shape -> Bomb";
        } else {
            // 两个方向都在末端，就是 L型 (生成时间球)
            specialType = TIME_GEM_TYPE;
            qDebug() << "Condition Met: L-Shape -> Time Gem";
        }
        return true;
    }
    return false;
}

// 连击分数计算策略
int GameWidget::calculateComboScore(int rawScore, int combo) {
    double multiplier = 1.0;

    // 自定义非线性倍率
    switch (combo) {
    case 0: // 第一次消除
    case 1: multiplier = 1.0; break;
    case 2: multiplier = 1.5; break; // 2连
    case 3: multiplier = 2.5; break; // 3连，倍率提升
    case 4: multiplier = 4.0; break; // 4连，大幅提升
    case 5: multiplier = 6.0; break; // 5连
    default: multiplier = 6.0; break; // 5连以上按5连算
    }

    qDebug() << "Combo:" << combo << " Multiplier:" << multiplier << " Raw:" << rawScore;
    return static_cast<int>(rawScore * multiplier);
}

void GameWidget::setUserInfo(QString username, int difficulty) {
    this->currentUsername = username;

    // 限制难度范围，防止出错
    if (difficulty < 4) difficulty = 4;
    if (difficulty > 7) difficulty = 7;

    this->DIFFICULITY = difficulty;

    qDebug() << "Game Settings Updated - User:" << username << " Difficulty:" << this->DIFFICULITY;
}
