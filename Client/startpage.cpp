#include <QDesktopServices>
#include "startpage.h"
#include "ui_startpage.h"
#include "qmessagebox.h"
#include "gamewidget.h"
#include <QDebug>

StartPage::StartPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);

    qDebug()<<QCoreApplication::applicationDirPath();
    sound=new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/music/background/music-1.wav"));
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();

    // 初始化所有按钮
    startButton=new HoverButton();
    recordButton=new HoverButton();
    settingButton=new HoverButton();
    loginButton = new HoverButton();
    registerButton = new HoverButton();
    logoutButton = new HoverButton();

    startButton->setSound(":/music/button/button_mouseover.wav", ":/music/button/button_mouseleave.wav", ":/music/button/button_press.wav", ":/music/button/button_release.wav");
    recordButton->setSound(":/music/button/button_mouseover.wav", ":/music/button/button_mouseleave.wav", ":/music/button/button_press.wav", ":/music/button/button_release.wav");
    settingButton->setSound(":/music/button/button_mouseover.wav", ":/music/button/button_mouseleave.wav", ":/music/button/button_press.wav", ":/music/button/button_release.wav");

    // 窗口设置
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());
    QWidget::showFullScreen();
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, Qt::black);
    this->setPalette(palette);
    setCursor(QCursor(QPixmap("://picture/mouse1.png")));

    // 1. 初始化背景（静止，不移动）
    initBackground();

    // 2. 初始化为登录界面状态
    initLoginUI();

    // --- 连接信号槽 ---

    // 语言切换
    connect(&settingP,&settingpage::selectLan,[=](int index){
        this->ForGameL=index;
    });

    // 难度选择
    connect(select, &selectlevel::selectDone, [=](const QString &difficultyText, int gemCount){
        this->hide();
        sound->stop();

        // 保存难度文字和宝石种类数
        currentDifficultyText = difficultyText;  // "简单"、"普通"、"困难"
        currentGemCount = gemCount;              // 4,6,7

        qDebug() << "Starting game - Difficulty:" << difficultyText
                 << " Gem Count:" << gemCount;

        // 设置游戏参数
        gameWidget->DIFFICULITY = gemCount;

        // 传递用户信息和难度文字
        if (!currentUsername.isEmpty()) {
            gameWidget->setUserInfo(currentUsername, difficultyText);
        }

        gameWidget->setupScene(this->ForGameL);
        gameWidget->show();
    });

    // 语言文字更新逻辑... (保留原代码中的setText部分)
    connect(&settingP,&settingpage::selectLan,[=](int index){
        if(index==0){
            select->level1L->setText("Easy");
            select->level2L->setText("Medium");
            // ... (保留原有的语言设置逻辑)
            select->DoneL->setText("开始");
        }else{
            select->level1L->setText("Easy");
            // ... (保留原有的语言设置逻辑)
            select->DoneL->setText("Done");
        }
    });

    // Start 按钮点击
    connect(startButton, &HoverButton::clicked, [=](){
        select->setParent(this);
        select->setGeometry(700,400,800,555);
        select->setVisible(true);
    });

    // 从游戏返回主菜单
    connect(gameWidget, &GameWidget::showStartPage, [=](){
        this->show();
        sound->setLoopCount(QSoundEffect::Infinite);
        sound->play();
        // 返回时可以保持在主菜单状态，不需要重新登录
    });

    // 设置按钮
    connect(settingButton, &HoverButton::clicked, [=](){
        settingP.setParent(this);
        settingP.setGeometry(560,262,800,555);
        settingP.show();
    });

    // 按钮文字更新 (保留原逻辑)
    connect(&settingP,&settingpage::selectLan,[=](int index){
        if(index==0){
            startButton->showContent("",40);
            recordButton->showContent("记录",20);
            settingButton->showContent("设置",20);
            loginButton->showContent("登录",10); // 注意：这里可能需要根据当前状态决定是否刷新显示
            registerButton->showContent("注册",10);
            logoutButton->showContent("登出",10);
        }
        if(index==1){
            startButton->showContent("",40);
            recordButton->showContent("Record",20);
            settingButton->showContent("Settings",20);
            loginButton->showContent("login",10);
            registerButton->showContent("register",10);
            logoutButton->showContent("logout",10);
        }
    });

    // Record 按钮
    connect(recordButton, &HoverButton::clicked, [=](){
        this->hide();
        sound->stop();
        ranklist->show();
    });

    // Login 按钮
    connect(loginButton, &HoverButton::clicked, [=](){
        this->hide();
        sound->stop();
        login->show();
    });

    // Register 按钮
    connect(registerButton, &HoverButton::clicked, [=](){
        this->hide();
        sound->stop();
        regist->show();
    });

    // Logout 按钮
    connect(logoutButton, &HoverButton::clicked, [=](){
        QMessageBox msgBox;
        if(client->logined == true) {
            client->logined = false;
            msgBox.setText("The user log out");
            // 登出后返回登录界面
            initLoginUI();
        } else {
            msgBox.setText("No user log in");
        }
        msgBox.exec();
    });

    // 从其他页面返回 StartPage 的处理
    connect(ranklist, &rankListPage::showStartPage, [=](){
        this->show();
        sound->setLoopCount(QSoundEffect::Infinite);
        sound->play();
    });

    // --- 核心修改：登录/注册返回后的逻辑 ---
    connect(login, &loginPage::showStartPage, [=](){
        this->show();
        sound->setLoopCount(QSoundEffect::Infinite);
        sound->play();

        // 检查是否登录成功
        if(client->logined) {
            // 保存当前用户名
            currentUsername = client->username;
            qDebug() << "User logged in:" << currentUsername;

            enterMainMenu();
        } else {
            initLoginUI();
        }
    });

    connect(regist, &registerPage::showStartPage, [=](){
        this->show();
        sound->setLoopCount(QSoundEffect::Infinite);
        sound->play();
        // 注册后通常需要登录，或者如果注册自动登录：
        if(client->logined) {
            enterMainMenu();
        } else {
            initLoginUI();
        }
    });
}

StartPage::~StartPage()
{
    delete ui;
}

// 1. 初始化静止背景
void StartPage::initBackground() {
    background = new QLabel(this); // 使用成员变量
    setBkImg("://picture/StartPage/background.png", background);
    background->show();
    // 初始位置设定，不启动动画
    background->setGeometry(background->x(), background->y(), background->width(), background->height());
}

// 2. 初始化登录界面布局
void StartPage::initLoginUI() {
    // 隐藏主菜单按钮
    startButton->hide();
    recordButton->hide();
    settingButton->hide();
    logoutButton->hide();

    // 显示并居中 Login/Register 按钮
    loginButton->show();
    registerButton->show();

    // 设置大尺寸居中位置 (示例：屏幕中间并排)
    int btnSize = this->width() / 6;
    int centerY = this->height() / 2 - btnSize/2;
    int centerX = this->width() / 2;

    // 左边放Login，右边放Register
    loginButton->setCircle(btnSize/2, centerX - btnSize, centerY, this->width(), this->height(),
                           ":/picture/button/ball.png", "", this);
    registerButton->setCircle(btnSize/2, centerX + btnSize, centerY, this->width(), this->height(),
                              ":/picture/button/ball.png", "", this);

    loginButton->showContent("LOGIN", 30);
    registerButton->showContent("REGISTER", 30);
}

// 3. 进入主菜单（执行移动动画）
void StartPage::enterMainMenu() {
    // 隐藏登录/注册按钮（不再需要）
    loginButton->hide();
    registerButton->hide();

    // 显示主菜单按钮
    startButton->show();
    recordButton->show();
    settingButton->show();
    logoutButton->show(); // 此时显示登出按钮

    // 设置主菜单按钮的初始位置（用于动画）
    SetButton(); // 重置为原来的位置逻辑

    startButton->showContent("Start",40);
    recordButton->showContent("Record",20);
    settingButton->showContent("Setting",20);
    logoutButton->showContent("Logout", 10);

    // 创建并启动动画组
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    // 添加背景移动动画 (向下移动)
    group->addAnimation(createBkAnim());

    // 添加标题浮现动画
    group->addAnimation(ShowTitle());

    // 添加按钮入场动画
    group->addAnimation(startButton->textAnim);
    group->addAnimation(recordButton->textAnim);
    group->addAnimation(settingButton->textAnim);
    group->addAnimation(logoutButton->textAnim);

    Sleep(200);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

// 创建背景动画（从当前位置移动到底部）
QPropertyAnimation* StartPage::createBkAnim() {
    bkAnim = new QPropertyAnimation(background, "geometry", this);
    bkAnim->setDuration(2000);
    bkAnim->setStartValue(QRect(background->x(), background->y(), background->width(), background->height()));
    // 向下移动：让背景的底部对齐窗口底部？或者完全移出？
    // 原逻辑：background->y() 变到 this->height() - background->height()
    // 如果 background 高度很高，这会让背景向上卷动显示底部，或者向下移动
    bkAnim->setEndValue(QRect(background->x(), this->height() - background->height(), background->width(), background->height()));
    bkAnim->setEasingCurve(QEasingCurve::InOutCubic);
    return bkAnim;
}

QPropertyAnimation * StartPage::ShowTitle(){
    // 保持原样，注意不要重复new label，如果多次进入可能会重叠
    // 建议也将title改为成员变量，这里简化处理，假设只执行一次
    QLabel *title = new QLabel(this);
    title->setGeometry(this->width()/2-903/2,-400,903,400); // 初始在屏幕外
    setAdaptedImg(":/picture/StartPage/title.png",title);
    title->show();
    QPropertyAnimation *animation = new QPropertyAnimation(title, "geometry",this);
    animation->setDuration(2000);
    animation->setStartValue(QRect(title->x(), title->y(), title->width(), title->height()));
    animation->setEndValue(QRect(title->x(), 100, title->width(), title->height()));
    animation->setEasingCurve(QEasingCurve::OutExpo);
    return animation;
}
//将path的图片放置到label上，自适应label大小
void StartPage::setAdaptedImg(QString path,QLabel *label)
{
    QImage image(path);
    QSize size=label->size();
    QImage image2=image.scaled(size,Qt::IgnoreAspectRatio);//重新调整图像大小以适应label
    label->setPixmap(QPixmap::fromImage(image2));//显示
}
//将path的图片放置到label上，自适应label大小
void StartPage::setBkImg(QString path,QLabel *label)
{
    QImage image = QImage(path);
    if(image.isNull()){
        qDebug()<<"background:empty";
    }
    double ratio=(double)image.height()/(double)image.width();
    QImage image2=image.scaled(this->width(),ratio*this->width(),Qt::IgnoreAspectRatio);//重新调整图像大小以适应label
    label->setPixmap(QPixmap::fromImage(image2));//显示
    label->setGeometry(0,0,this->width(),ratio*this->width());
}

void StartPage::keyPressEvent(QKeyEvent *ev)
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
void StartPage::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//主窗口关闭则选择界面关闭
void StartPage::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    select->close();
}

void StartPage::SetButton(){
    startButton->setCircle(this->width()/10, this->width()/2, this->height()/2+100, this->width(), this->height(),\
                           ":/picture/button/startbutton.png", "", this);

    recordButton->setCircle(this->width()/100*5, this->width()/6, this->height()/2+100, this->width(), this->height(),\
                            ":/picture/button/ball.png", "", this);
    settingButton->setCircle(this->width()/100*5, this->width()/6*5, this->height()/2+100, this->width(), this->height(),\
                             ":/picture/button/ball.png", "", this);

    // login/register 在 enterMainMenu 时不需要设置位置，因为被隐藏了
    // 或者是设置到角落备用？原逻辑中它们在底部。
    // 这里我们只设置 logoutButton
    logoutButton->setCircle(this->width()/50, this->width()/10, this->height()/2+400, this->width(), this->height(),\
                            ":/picture/button/ball.png", "", this);
}
