#include "registerpage.h"
#include "ui_registerpage.h"
#include "qmessagebox.h"
//#include "startpage.h"
//#include "ui_startpage.h"

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
  ft.setFamily("Smiley Sans");
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
      QString tempId = idText->toPlainText();
      QString tempPwd = pwdText->text();

      qDebug() << "Attempting to register user:" << tempId;

      // 检查输入是否为空
      if(tempId.isEmpty() || tempPwd.isEmpty()) {
          QMessageBox::warning(this, "Warning",
                               "Username and password cannot be empty!\n\n用户名和密码不能为空！");
          return;
      }

      // 检查用户名长度
      if(tempId.length() < 3 || tempId.length() > 20) {
          QMessageBox::warning(this, "Warning",
                               "Username must be 3-20 characters!\n\n用户名必须是3-20个字符！");
          return;
      }

      // 检查密码长度
      if(tempPwd.length() < 3 || tempPwd.length() > 20) {
          QMessageBox::warning(this, "Warning",
                               "Password must be 3-20 characters!\n\n密码必须是3-20个字符！");
          return;
      }

      // 注册前先显示等待消息
      QMessageBox::information(this, "Info", "Registering, please wait...\n\n注册中，请稍候...");

      // 调用注册函数
      bool result = client->registerNewUser(tempId, tempPwd);
      qDebug() << "Register function returned:" << result;

      // 等待更长时间获取响应
      QTime dieTime = QTime::currentTime().addMSecs(3000);
      while(QTime::currentTime() < dieTime && client->registerFlag == -1) {
          QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
      }

      qDebug() << "Register flag after waiting:" << client->registerFlag;

      // 创建消息框
      QMessageBox msgBox(this);
      msgBox.setWindowTitle("Registration Result");
      msgBox.setTextFormat(Qt::RichText);
      msgBox.setStyleSheet("QLabel{min-width: 500px; min-height: 150px; font-size: 20px;} QPushButton{width: 120px; height: 50px; font-size: 18px;}");

      if(client->registerFlag == 0) {
          msgBox.setText("<font size='5' color='red'>✗ Registration Failed!</font><br><br>"
                         "<font size='4'>User already exists.</font><br><br>"
                         "<font size='4'>用户名已被注册。</font>");
          msgBox.setIcon(QMessageBox::Warning);
      }
      else if(client->registerFlag == 1) {
          msgBox.setText("<font size='5' color='green'>✓ Registration Successful!</font><br><br>"
                         "<font size='4'>User registered successfully.</font><br><br>"
                         "<font size='4'>注册成功！</font>");
          msgBox.setIcon(QMessageBox::Information);
      }
      else if(client->registerFlag == -1) {
          msgBox.setText("<font size='5' color='orange'>⚠ Timeout Error!</font><br><br>"
                         "<font size='4'>No response from server.</font><br><br>"
                         "<font size='4'>服务器无响应，请检查：</font><br>"
                         "<font size='4'>1. 服务端是否运行</font><br>"
                         "<font size='4'>2. 网络连接</font>");
          msgBox.setIcon(QMessageBox::Critical);
      }
      else {
          msgBox.setText("<font size='5' color='red'>❌ Unknown Error!</font><br><br>"
                         "<font size='4'>Error code: " + QString::number(client->registerFlag) + "</font><br><br>"
                                                                   "<font size='4'>未知错误，错误代码：" + QString::number(client->registerFlag) + "</font>");
          msgBox.setIcon(QMessageBox::Critical);
      }

      msgBox.exec();

      // 重置标志
      client->registerFlag = -1;

      idText->setText((""));
      pwdText->setText((""));
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
