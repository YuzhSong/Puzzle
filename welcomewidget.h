#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>

namespace Ui {
class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

signals:
    void loginButtonClicked();
    void registerButtonClicked();


public: 
    explicit WelcomeWidget(QWidget *parent = nullptr);
    ~WelcomeWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::WelcomeWidget *ui;
};

#endif // WELCOMEWIDGET_H
