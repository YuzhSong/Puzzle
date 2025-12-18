#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    connect(ui->btnBack, &QPushButton::clicked, this, &LoginWidget::backButtonClicked);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}
