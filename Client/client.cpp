#include "client.h"

Client::Client()
{
    socket = new QTcpSocket();
    QObject::connect(socket,&QTcpSocket::readyRead,this,&Client::readDataSlot);
    logined = false;
    verifyFlag = -1;
    registerFlag = -1;
}

bool Client::registerNewUser(QString userName, QString password){
    socket->connectToHost("127.0.0.1",8888);
    this->username = userName;
    this->password = password;
    QByteArray content;
    content.append("REGISTER");
    content.append('&');
    content.append(userName.toLocal8Bit());
    content.append('&');
    content.append(password.toLocal8Bit());
    socket->write(content);

    if(socket->waitForReadyRead(1000)==false){
        qDebug() << "Timeout waiting for register response";
    }
    return false;
}

bool Client::verifyUser(QString userName, QString password){
    socket->connectToHost("127.0.0.1",8888);
    this->username = userName;
    this->password = password;
    QByteArray content;
    content.append("VERIFY");
    content.append('&');
    content.append(userName.toLocal8Bit());
    content.append('&');
    content.append(password.toLocal8Bit());
    socket->write(content);
    if(socket->waitForReadyRead(1000)==false){
        qDebug() << "Timeout waiting for verify response";
    }
    return false;
}

void Client::getProfile(const QString &difficulty) {
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;

    // 格式：PROFILE&Easy 或 PROFILE&All
    content.append("PROFILE&");
    content.append(difficulty.toUtf8());

    qDebug() << "Requesting profile with difficulty:" << difficulty;

    // 清除之前的数据
    userlist.clear();

    socket->write(content);
    socket->flush();  // 确保数据发送出去

    // 等待更长时间，让readDataSlot有时间处理
    if(socket->waitForReadyRead(3000) == false) {  // 增加到3秒
        qDebug() << "Timeout waiting for profile response";
    } else {
        qDebug() << "Profile response should be processed by readDataSlot";
    }
}

void Client::getRankList(const QString &difficulty) {
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;

    // 格式：RANKLIST&Medium 或 RANKLIST&All
    content.append("RANKLIST&");
    content.append(difficulty.toUtf8());

    qDebug() << "Requesting ranklist with difficulty:" << difficulty;

    // 清除之前的数据
    ranklist.clear();

    socket->write(content);
    socket->flush();  // 确保数据发送出去

    // 等待更长时间
    if(socket->waitForReadyRead(3000) == false) {
        qDebug() << "Timeout waiting for ranklist response";
    } else {
        qDebug() << "Ranklist response should be processed by readDataSlot";
    }
}

// 新增：提交分数函数
void Client::submitScore(QString username, const QString &difficultyText, int score) {
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;
    content.append("SUBMIT_SCORE");
    content.append('&');
    content.append(username.toLocal8Bit());
    content.append('&');
    content.append(difficultyText.toLocal8Bit());  // 直接是文字
    content.append('&');
    content.append(QString::number(score).toLocal8Bit());

    qDebug() << "Client: Submitting score - User:" << username
             << "Difficulty:" << difficultyText  // 文字
             << "Score:" << score;

    socket->write(content);

    // 等待响应
    if(socket->waitForReadyRead(3000) == false) {
        qDebug() << "Timeout waiting for score submission response";
    }
}

void Client::readDataSlot() {
    auto buffer = socket->readAll();
    QString response = QString::fromUtf8(buffer);
    qDebug() << "Raw response:" << response;

    auto instruction = buffer.split('&');
    for(int i = 0; i < instruction.size(); i++) {
        qDebug() << "Instruction[" << i << "]:" << instruction[i];
    }

    if (instruction[0] == "REG_RESULT"){
        registerFlag = instruction[1].toInt();
        qDebug() << "Register result:" << registerFlag;
    }
    else if (instruction[0] == "VER_RESULT"){
        verifyFlag = instruction[1].toInt();
        qDebug() << "Verify result:" << verifyFlag;
    }
    else if (instruction[0] == "PRO_RESULT"){
        // 需要重新构建字符串，因为可能有多个&
        userlist = response.mid(response.indexOf('&') + 1);
        qDebug() << "Profile received, length:" << userlist.length();
        qDebug() << "Profile content preview:" << userlist.left(100);
    }
    else if (instruction[0] == "RAN_RESULT"){
        // 需要重新构建字符串，因为可能有多个&
        ranklist = response.mid(response.indexOf('&') + 1);
        qDebug() << "Ranklist received, length:" << ranklist.length();
        qDebug() << "Ranklist content preview:" << ranklist.left(100);
    }
    else if (instruction[0] == "SCORE_SUBMITTED"){
        qDebug() << "Score submission confirmed by server";
    }
    else if (instruction[0] == "UPD_RESULT"){
        qDebug() << "Update result received";
    }
}

void Client::update(int score) {
    socket->connectToHost("127.0.0.1",8888);
    QByteArray content;
    content.append("UPDATE");
    content.append('&');
    content.append(username.toLocal8Bit());
    content.append('&');
    content.append(QString::number(score).toLocal8Bit());
    qDebug() << "Client update:" << content;
    socket->write(content);
}
