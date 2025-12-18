#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomewidget.h"
#include "loginwidget.h"
#include "registerwidget.h"
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 1. 创建 StackedWidget
    QStackedWidget *stack = new QStackedWidget(this);
    setCentralWidget(stack); // 让它占据窗口中心

    // 2. 实例化子页面
    WelcomeWidget *welcome = new WelcomeWidget();
    LoginWidget *login = new LoginWidget();
    RegisterWidget *reg = new RegisterWidget();

    // 3. 添加到堆栈布局
    stack->addWidget(welcome); // Index 0
    stack->addWidget(login);   // Index 1
    stack->addWidget(reg);     // Index 2

    // 4. 信号槽逻辑：切换页面
    // 欢迎页 -> 登录页
    connect(welcome, &WelcomeWidget::loginButtonClicked, [=](){
        stack->setCurrentIndex(1);
    });
    // 欢迎页 -> 注册页
    connect(welcome, &WelcomeWidget::registerButtonClicked, [=](){
        stack->setCurrentIndex(2);
    });
    // 登录页 -> 返回欢迎页
    connect(login, &LoginWidget::backButtonClicked, this, [=]() {
        stack->setCurrentIndex(0);
    });
    // 注册页 -> 返回欢迎页
    connect(reg, &RegisterWidget::backButtonClicked, this, [=]() {
        stack->setCurrentIndex(0);
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}
