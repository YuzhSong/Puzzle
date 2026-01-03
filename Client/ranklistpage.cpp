#include "ranklistpage.h"
#include "ui_ranklistpage.h"
#include "client.h"
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QShowEvent>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDateTime>

extern Client *client;

rankListPage::rankListPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rankListPage),
    client(nullptr)  // 初始化为nullptr
{
    ui->setupUi(this);

    // 禁用最大化按钮、设置窗口大小固定
    this->setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(1600, 900);

    qDebug() << "排行榜页面构造函数开始";

    // 设置客户端指针
    this->client = ::client;  // 使用全局的client

    // 设置鼠标
    setCursor(QCursor(QPixmap("://picture/mouse1.png")));

    // 设置按钮文字（如果有HoverButton的特殊方法）
    if (ui->returnButton) {
        ui->returnButton->showContent("Return", 18);
    }
    if (ui->queryButton) {
        ui->queryButton->showContent("Query Rankings", 18);
    }

    // 初始文本
    ui->ranklist->setText("Click 'Query Rankings' to view rankings.\n\n点击'查询排行'查看排行榜。");
    ui->userRanklist->setText("Your personal game records will be shown here.\n\n您的个人游戏记录将显示在这里。");

    // 确保所有控件都可见
    ui->ranklist->setVisible(true);
    ui->userRanklist->setVisible(true);
    ui->totalRankGroup->setVisible(true);
    ui->userRankGroup->setVisible(true);
    ui->difficultyGroupBox->setVisible(true);
    ui->titleLabel->setVisible(true);
    ui->returnButton->setVisible(true);
    ui->queryButton->setVisible(true);

    // 连接信号槽
    connect(ui->returnButton, &HoverButton::clicked,
            this, &rankListPage::onReturnButtonClicked);
    connect(ui->queryButton, &HoverButton::clicked,
            this, &rankListPage::onQueryButtonClicked);

    qDebug() << "排行榜页面构造函数完成";
}

rankListPage::~rankListPage()
{
    delete ui;
}

void rankListPage::setUserRankList(const QString &s)
{
    qDebug() << "设置用户排行榜内容";
    ui->userRanklist->setText(s);
    ui->userRanklist->repaint();  // 强制重绘
}

void rankListPage::setRankList(const QString &s)
{
    qDebug() << "设置总排行榜内容";
    ui->ranklist->setText(s);
    ui->ranklist->repaint();  // 强制重绘
}

void rankListPage::onReturnButtonClicked()
{
    qDebug() << "返回按钮点击";
    this->hide();
    emit showStartPage();
}

void rankListPage::onQueryButtonClicked()
{
    qDebug() << "查询按钮点击";

    // 获取选择的难度
    QString difficulty;
    if (ui->easyRadio->isChecked())
        difficulty = "Easy";
    else if (ui->mediumRadio->isChecked())
        difficulty = "Medium";
    else if (ui->hardRadio->isChecked())
        difficulty = "Hard";
    else
        difficulty = "All";

    qDebug() << "选择的难度:" << difficulty;

    // 显示加载中
    ui->ranklist->setText(QString("Loading %1 rankings...\n\n加载%2排行榜中...")
                              .arg(difficulty)
                              .arg(difficulty == "All" ? "所有难度" : difficulty));
    ui->userRanklist->setText("Loading personal records...\n\n加载个人记录中...");

    // 强制更新显示
    QCoreApplication::processEvents();

    if (!client) {
        qDebug() << "客户端未初始化";
        return;
    }

    qDebug() << "发送查询请求";

    // 先获取排行榜
    client->getRankList(difficulty);

    // 等待并处理
    QTime dieTime = QTime::currentTime().addMSecs(1000);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    qDebug() << "检查排行榜数据，长度:" << client->ranklist.length();

    if (!client->ranklist.isEmpty()) {
        qDebug() << "设置排行榜数据";
        ui->ranklist->setText(client->ranklist);
    } else {
        qDebug() << "排行榜数据为空";
        ui->ranklist->setText("No data received for ranklist\n未接收到排行榜数据");
    }

    // 再获取个人记录
    client->getProfile(difficulty);

    // 再次等待
    dieTime = QTime::currentTime().addMSecs(1000);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    qDebug() << "检查个人记录数据，长度:" << client->userlist.length();

    if (!client->userlist.isEmpty()) {
        qDebug() << "设置个人记录数据";
        ui->userRanklist->setText(client->userlist);
    } else {
        qDebug() << "个人记录数据为空";
        ui->userRanklist->setText("No data received for profile\n未接收到个人记录数据");
    }

    qDebug() << "查询完成";
}

void rankListPage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    qDebug() << "排行榜页面显示事件触发";

    // 确保焦点正确
    ui->ranklist->setFocus();

    // 立即更新显示
    QCoreApplication::processEvents();
}

void rankListPage::keyPressEvent(QKeyEvent *ev)
{
    // Esc退出全屏
    if (ev->key() == Qt::Key_Escape) {
        QWidget::showNormal();
        return;
    }
    // F11全屏
    if (ev->key() == Qt::Key_F11) {
        QWidget::showFullScreen();
        return;
    }
    QWidget::keyPressEvent(ev);
}

void rankListPage::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
