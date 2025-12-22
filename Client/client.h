#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    // 网络功能
    bool registerNewUser(QString userName, QString password);
    bool verifyUser(QString userName, QString password);
    void getProfile();
    void getRankList();
    void update(int score);

    // 登录状态
    bool isLogined = false;
    QString currentUsername = "";

    // 标志变量
    int registerFlag = -1;
    int verifyFlag = -1;
    QString userlist;
    QString ranklist;

private slots:
    void readDataSlot();

private:
    QTcpSocket *socket;
    QString username;
    QString password;
};

#endif // CLIENT_H
