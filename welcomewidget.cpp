#include "welcomewidget.h"
#include "ui_welcomewidget.h"

#include <QPixmap>

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);

    // 1. 设置背景图片
    QPixmap bg(":/background1.png");

    // 2. 让图片平铺满整个 Label
    ui->backgroundLabel->setPixmap(bg);
    ui->backgroundLabel->setScaledContents(true);

    connect(ui->loginBtn, &QPushButton::clicked, this, &WelcomeWidget::loginButtonClicked);
    connect(ui->registerBtn, &QPushButton::clicked, this, &WelcomeWidget::registerButtonClicked);
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    // 每次窗口大小改变，重新设置一次 pixmap 确保填充
    QPixmap bg(":/background1.png");
    ui->backgroundLabel->setPixmap(bg.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}
