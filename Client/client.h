#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>
class Client:public QObject
{
    Q_OBJECT
public:
    Client();
    bool registerNewUser(QString userName, QString password);
    bool verifyUser(QString userName, QString password);
    void getProfile(const QString &difficulty = "");
    void getRankList(const QString &difficulty = "");
    void submitScore(QString username, const QString &difficultyText, int score);  // 新增：提交分数
    void update(int score);  // 保持向后兼容

    QString ranklist;
    QString userlist;
    int verifyFlag;
    int registerFlag;
    bool logined;
    QString username;  // 公开用户名，方便其他类访问

private:
    QTcpSocket * socket;
    QString password;

private slots:
    void readDataSlot();
};

#endif // CLIENT_H
