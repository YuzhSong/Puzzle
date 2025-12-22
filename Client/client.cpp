#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &Client::readDataSlot);
    isLogined = false;
}

bool Client::registerNewUser(QString userName, QString password){
    socket->connectToHost("127.0.0.1", 8888);
    this->username = userName;
    this->password = password;
    registerFlag = -1; // 重置标志

    QByteArray content;
    content.append("REGISTER");
    content.append('&');
    content.append(userName.toLocal8Bit());
    content.append('&');
    content.append(password.toLocal8Bit());
    socket->write(content);

    // 等待响应
    if(socket->waitForReadyRead(1000)){
        // 数据会在readDataSlot中处理
    }

    // 返回注册结果
    return (registerFlag == 1);
}

bool Client::verifyUser(QString userName, QString password){
    socket->connectToHost("127.0.0.1", 8888);
    this->username = userName;
    this->password = password;
    verifyFlag = -1; // 重置标志

    QByteArray content;
    content.append("VERIFY");
    content.append('&');
    content.append(userName.toLocal8Bit());
    content.append('&');
    content.append(password.toLocal8Bit());
    socket->write(content);

    // 等待响应
    if(socket->waitForReadyRead(1000)){
        // 数据会在readDataSlot中处理
    }

    // 如果验证成功，设置登录状态
    if (verifyFlag == 2) {
        isLogined = true;
        currentUsername = userName;
    }

    return (verifyFlag == 2);
}

void Client::getProfile(){
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;
    content.append("PROFILE");
    socket->write(content);
    if(socket->waitForReadyRead(1000)==false){
        // 超时处理
    }
}

void Client::getRankList() {
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;
    content.append("RANKLIST");
    socket->write(content);
    if(socket->waitForReadyRead(1000)==false){
        // 超时处理
    }
}

void Client::readDataSlot() {
    auto buffer = socket->readAll();
    auto instruction = buffer.split('&');

    // 调试输出
    for(int i = 0; i < instruction.size(); i++) {
        qDebug() << instruction[i] << "\n";
    }

    if (instruction[0] == "REG_RESULT" && instruction.size() >= 2){
        registerFlag = instruction[1].toInt();
    }
    else if (instruction[0] == "VER_RESULT" && instruction.size() >= 2){
        verifyFlag = instruction[1].toInt();
    }
    else if (instruction[0] == "PRO_RESULT" && instruction.size() >= 2){
        userlist = QString::fromUtf8(instruction[1]);
    }
    else if (instruction[0] == "RAN_RESULT" && instruction.size() >= 2){
        ranklist = QString::fromUtf8(instruction[1]);
    }
    else if (instruction[0] == "UPD_RESULT"){
        // 更新结果处理
    }

    socket->abort();
}

void Client::update(int score) {
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;
    content.append("UPDATE");
    content.append('&');
    content.append(username.toLocal8Bit());
    content.append('&');
    content.append(QString::number(score).toLocal8Bit());
    qDebug() << content << "\n";
    socket->write(content);
}
