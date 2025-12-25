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

void Client::getProfile(){
    socket->connectToHost("127.0.0.1",8888);
    QByteArray content;
    content.append("PROFILE");
    socket->write(content);
    if(socket->waitForReadyRead(1000)==false){
        qDebug() << "Timeout waiting for profile response";
    }
}

void Client::getRankList() {
    socket->connectToHost("127.0.0.1",8888);
    QByteArray content;
    content.append("RANKLIST");
    socket->write(content);
    if(socket->waitForReadyRead(1000)==false){
        qDebug() << "Timeout waiting for ranklist response";
    }
}

// 新增：提交分数函数
void Client::submitScore(QString username, int difficulty, int score) {
    socket->connectToHost("127.0.0.1", 8888);
    QByteArray content;
    content.append("SUBMIT_SCORE");
    content.append('&');
    content.append(username.toLocal8Bit());
    content.append('&');
    content.append(QString::number(difficulty).toLocal8Bit());
    content.append('&');
    content.append(QString::number(score).toLocal8Bit());
    qDebug() << "Client: Submitting score - User:" << username
             << "Difficulty:" << difficulty
             << "Score:" << score;
    socket->write(content);

    // 等待响应
    if(socket->waitForReadyRead(3000) == false) {
        qDebug() << "Timeout waiting for score submission response";
    } else {
        qDebug() << "Score submitted successfully";
    }
}

void Client::readDataSlot() {
    auto buffer = socket->readAll();
    auto instruction = buffer.split('&');
    for(int i = 0; i < instruction.size(); i++) {
        qDebug() << instruction[i] << "\n";
    }
    if (instruction[0] == "REG_RESULT"){
        registerFlag = instruction[1].toInt();
        qDebug() << "Register result:" << registerFlag;
    }
    if (instruction[0] == "VER_RESULT"){
        verifyFlag = instruction[1].toInt();
        qDebug() << "Verify result:" << verifyFlag;
    }
    if (instruction[0] == "PRO_RESULT"){
        userlist = instruction[1];
        qDebug() << "Profile received, length:" << userlist.length();
    }
    if (instruction[0] == "RAN_RESULT"){
        ranklist = instruction[1];
        qDebug() << "Ranklist received, length:" << ranklist.length();
    }
    if (instruction[0] == "SCORE_SUBMITTED"){  // 新增：分数提交响应
        qDebug() << "Score submission confirmed by server";
    }
    if (instruction[0] == "UPD_RESULT"){
        qDebug() << "Update result received";
    }
    socket->abort();
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
