#include "registerwidget.h"
#include "ui_registerwidget.h"

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);

    connect(ui->btnBackReg, &QPushButton::clicked,
            this, &RegisterWidget::backButtonClicked);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}
