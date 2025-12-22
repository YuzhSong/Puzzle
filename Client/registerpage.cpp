#include "registerpage.h"
#include "ui_registerpage.h"
#include "qmessagebox.h"
#include <QRegularExpression>

extern Client *client;

class StartPage;

registerPage::registerPage(QWidget *parent) :
                                              QWidget(parent),
                                              ui(new Ui::registerPage)
{
    this->hide();
  ui->setupUi(this);
  ui->setupUi(this);
  returnButton = new HoverButton();
  confirmButton = new HoverButton();

  returnButton->setSound(":/music/button/button_mouseover.wav", ":/music/button/button_mouseleave.wav", ":/music/button/button_press.wav", ":/music/button/button_release.wav"); //默认音效
  confirmButton->setSound(":/music/button/button_mouseover.wav", ":/music/button/button_mouseleave.wav", ":/music/button/button_press.wav", ":/music/button/button_release.wav");
  //禁用最大化按钮、设置窗口大小固定
  this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
  this->setFixedSize(this->width(),this->height());

  id = new QLabel(this);
  pwd = new QLabel(this);
  id->setText("id");
  pwd->setText("password");

  idText = new QTextEdit(this);
  pwdText = new QLineEdit(this);

  pwdText->setEchoMode(QLineEdit::Password);

  QFont ft;
  ft.setPointSize(25);
  id->setFont(ft);
  pwd->setFont(ft);
  idText->setFont(ft);
  pwdText->setFont(ft);


  QPalette pl = idText->palette();
  pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
  idText->setPalette(pl);

  QPalette pl1 = pwdText->palette();
  pl1.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
  pwdText->setPalette(pl1);


  id->setGeometry(this->width()/2+400,this->height()/2+250,250,100);
  pwd->setGeometry(this->width()/2+400,this->height()/2+400,250,100);
  idText->setGeometry(this->width()/2+800,this->height()/2+265,400,75);
  pwdText->setGeometry(this->width()/2+800,this->height()/2+415,400,75);

  //  ranklist = new QTextBrowser(this);
  //  ranklist->setText("");
  //  //  ranklist->setText("ID\t\t\t\tSCORE\t\t\t\tRANK\n");
  //  QFont ft;
  //  ft.setPointSize(30);
  //  ranklist->setFont(ft);
  //  ranklist->setGeometry(this->width()/2+350,this->height()/2+190,1000,450);
  //  QPalette pl = ranklist->palette();
  //  pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
  //  ranklist->setPalette(pl);
  //  ranklist->setStyleSheet("QTextBrowser{border-width:0;border-style:outset}");



  //全屏
  QWidget::showFullScreen();
  //设置窗口背景黑色
  QPalette palette(this->palette());
  palette.setColor(QPalette::Window, Qt::black);
  this->setPalette(palette);
  //设置鼠标-普通
  setCursor(QCursor(QPixmap("://picture/mouse1.png")));
  //    QTimer::singleShot(1500, this, [=](){
  QParallelAnimationGroup *group = new QParallelAnimationGroup;
  group->addAnimation(ShowBackground());
  SetButton();
  returnButton->showContent("Return",30);
  confirmButton->showContent("Register",30);

  group->addAnimation(returnButton->textAnim);
  group->addAnimation(confirmButton->textAnim);

  group->addAnimation(ShowTitle());
  Sleep(200);
  group->start(QAbstractAnimation::DeleteWhenStopped);
  //    });

  id->raise();
  pwd->raise();
  idText->raise();
  pwdText->raise();

  this->hide();
}

registerPage::~registerPage()
{
  delete ui;
}

QPropertyAnimation * registerPage::ShowTitle(){
  QPixmap pix;
  QLabel *title = new QLabel(this);
  title->setGeometry(this->width()/2-903/2,-title->height(),903,200);
  setAdaptedImg(":/picture/StartPage/title.png",title);
  title->show();
  QPropertyAnimation *animation = new QPropertyAnimation(title, "geometry",this);
  animation->setDuration(2000);
  animation->setStartValue(QRect(title->x(), title->y(), title->width(), title->height()));
  animation->setEndValue(QRect(title->x(), 100, title->width(), title->height()));
  animation->setEasingCurve(QEasingCurve::OutExpo);
  return animation;

}
QPropertyAnimation *  registerPage::ShowBackground(){
  QPixmap pix;
  QLabel *background = new QLabel(this);
  bkAnim = new QPropertyAnimation(background, "geometry",this);
  setBkImg("://picture/StartPage/background.png",background);
  background->show();
  bkAnim->setDuration(2000);
  bkAnim->setStartValue(QRect(background->x(), background->y(), background->width(), background->height()));
  bkAnim->setEndValue(QRect(background->x(), this->height() - background->height(), background->width(), background->height()));
  bkAnim->setEasingCurve(QEasingCurve::InOutCubic);
  return bkAnim;
}

void registerPage::SetButton(){
  returnButton->setCircle(this->width()/10, this->width()/2+300, this->height()/2+400, this->width(), this->height(),\
                           ":/picture/button/ball.png", "", this);
  confirmButton->setCircle(this->width()/10, this->width()/2-300, this->height()/2+400, this->width(), this->height(),\
                           ":/picture/button/ball.png", "", this);
  connect(returnButton, &HoverButton::clicked, [=](){
    this->hide();
    showStartPage();
    //    vector<user> tempGamers =database->showRankList();
    //    QString s = "ID\t\t\t\tSCORE\t\t\t\tRANK\n";

    //    for (auto iter = tempGamers.begin(); iter != tempGamers.end(); iter++) {
    //      //      qDebug()<<QString("id:%1    password:%2    score:%3    rank:%4").arg((*iter).username).arg((*iter).password).arg((*iter).score).arg((*iter).rank);
    //      s = s + (*iter).username + "\t\t\t\t" + QString::number((*iter).score) + "\t\t\t" + QString::number((*iter).rank)  + "\n";
    //      cout << s.toStdString() << endl;
    //    }

    //    ranklist->setText(s);
    //    this->hide();
    //    gameWidget->setupScene();
    //    gameWidget->show();
  }) ;

  //  connect(gameWidget, &GameWidget::showStartPage, [=](){
  //    this->show();
  //  }) ;

  connect(confirmButton,&HoverButton::clicked, [=]() {
      QString tempId = idText->toPlainText().trimmed();  // 去除空格
      QString tempPwd = pwdText->text().trimmed();

      // 输入验证
      if (tempId.isEmpty()) {
          QMessageBox::warning(this, "注册失败", "用户名不能为空！");
          return;
      }

      if (tempPwd.isEmpty()) {
          QMessageBox::warning(this, "注册失败", "密码不能为空！");
          return;
      }

      // 密码格式限制：6-20位，包含字母和数字
      QRegularExpression pwdRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d@$!%*?&]{6,20}$");
      if (!pwdRegex.match(tempPwd).hasMatch()) {
          QMessageBox::warning(this, "密码格式错误",
                               "密码必须满足以下要求：\n"
                               "• 长度6-20位\n"
                               "• 至少包含一个字母和一个数字\n"
                               "• 只能包含字母、数字和特殊字符 @$!%*?&");
          return;
      }

      // 用户名格式限制：3-15位，字母数字开头
      QRegularExpression userRegex("^[A-Za-z0-9][A-Za-z0-9_.]{2,14}$");
      if (!userRegex.match(tempId).hasMatch()) {
          QMessageBox::warning(this, "用户名格式错误",
                               "用户名必须满足以下要求：\n"
                               "• 长度3-15位\n"
                               "• 以字母或数字开头\n"
                               "• 只能包含字母、数字、下划线和点");
          return;
      }

      // 调用客户端注册
      client->registerNewUser(tempId, tempPwd);
      int flag = client->registerFlag;

      QMessageBox msgBox;
      if(flag == 0) {
          msgBox.setText("用户已存在！");
      } else if(flag == 1) {
          msgBox.setText("注册成功！");
      } else {
          msgBox.setText("注册失败，请稍后重试！");
      }

      msgBox.exec();
      idText->setText("");
      pwdText->setText("");
  });
}


//将path的图片放置到label上，自适应label大小
void registerPage::setAdaptedImg(QString path,QLabel *label)
{
  QImage image(path);
  QSize size=label->size();
  QImage image2=image.scaled(size,Qt::IgnoreAspectRatio);//重新调整图像大小以适应label
  label->setPixmap(QPixmap::fromImage(image2));//显示
}
//将path的图片放置到label上，自适应label大小
void registerPage::setBkImg(QString path,QLabel *label)
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

void registerPage::keyPressEvent(QKeyEvent *ev)
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
void registerPage::Sleep(int msec)
{
  QTime dieTime = QTime::currentTime().addMSecs(msec);
  while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
