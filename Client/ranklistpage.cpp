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
        difficulty = "All Difficulties";

    qDebug() << "选择的难度:" << difficulty;

    // 显示加载中
    ui->ranklist->setText(QString("Loading %1 rankings...\n\n加载%2排行榜中...")
                              .arg(difficulty)
                              .arg(difficulty == "All Difficulties" ? "所有难度" : difficulty));
    ui->userRanklist->setText("Loading personal records...\n\n加载个人记录中...");

    // 强制更新显示
    QCoreApplication::processEvents();

    // 检查客户端是否存在
    if (!client) {
        qDebug() << "客户端未初始化，显示示例数据";

        // 显示示例数据 - 修复字符串连接
        QString header = QString("=== %1 Ranking ===\n\n").arg(difficulty.toUpper());
        QString exampleData = header +
                              "Rank  Player            Score  Time\n" +
                              "------------------------------------\n" +
                              "1.    Player_Alpha      9500  02:15\n" +
                              "2.    Player_Beta       8900  02:30\n" +
                              "3.    Player_Gamma      8500  02:45\n" +
                              "4.    Player_Delta      8200  03:00\n" +
                              "5.    Player_Epsilon    7900  03:10\n\n" +
                              "Last updated: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        ui->ranklist->setText(exampleData);

        // 个人记录示例 - 修复字符串连接
        QString personalData = QString("=== Your Records ===\n\n") +
                               "Rank  Score    Difficulty  Date\n" +
                               "--------------------------------\n" +
                               "3rd   8500     Medium      " + QDateTime::currentDateTime().toString("MM-dd") + "\n" +
                               "7th   7200     Hard        " + QDateTime::currentDateTime().addDays(-1).toString("MM-dd") + "\n" +
                               "12th  6800     Easy        " + QDateTime::currentDateTime().addDays(-2).toString("MM-dd") + "\n\n" +
                               "Best Score: 8500 (Medium)";

        ui->userRanklist->setText(personalData);
        return;
    }

    qDebug() << "客户端存在，发送查询请求";

    // 发送查询请求
    client->getProfile();
    client->getRankList();

    // 等待响应
    QTime dieTime = QTime::currentTime().addMSecs(2000);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    qDebug() << "查询完成，处理结果";

    if (!client->ranklist.isEmpty()) {
        qDebug() << "排行榜数据不为空，长度:" << client->ranklist.length();

        QString header = QString("=== %1 Ranking ===\n\n").arg(difficulty.toUpper());
        ui->ranklist->setText(header + client->ranklist);
        ui->userRanklist->setText("=== Your Game Records ===\n\n" + client->userlist);
    } else {
        qDebug() << "排行榜数据为空";
        ui->ranklist->setText("Failed to load rankings. Please try again.\n\n加载失败，请重试。");
        ui->userRanklist->setText("Failed to load personal records.\n\n加载个人记录失败。");
    }
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
