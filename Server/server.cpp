#include "server.h"

server::server()
{
    database = new dataBase();
    serverEnd = new QTcpServer();
    QObject::connect(serverEnd, &QTcpServer::newConnection, this, &server::clientHandler);

    serverEnd->listen(QHostAddress::LocalHost,8888);
    qDebug() << "Server started on port 8888";
}

void server::clientHandler() {
    clientSocket = serverEnd->nextPendingConnection();
    QObject::connect(clientSocket, &QTcpSocket::readyRead, this, &server::readData);
    QObject::connect(clientSocket,&QTcpSocket::disconnected, this, &server::disconnect);
    qDebug() << "New client connected";
}

void server::readData() {
    QString buffer = clientSocket->readAll();
    qDebug() << "Received:" << buffer;
    QStringList instruction = buffer.split('&');

    for(int i = 0; i < instruction.size(); i++) {
        qDebug() << "Instruction[" << i << "]:" << instruction[i];
    }

    if (instruction[0] == "REGISTER"){
        int information = database->registerFunc(instruction[1], instruction[2]);//0 registered 1 unregistered
        QByteArray content;
        content.append("REG_RESULT");
        content.append('&');
        content.append(QString::number(information).toUtf8());
        qDebug() << "Sending REG_RESULT:" << content;
        clientSocket->write(content);
    }
    else if (instruction[0] == "VERIFY"){
        int information = database->loginFunc(instruction[1], instruction[2]);//0 unregistered 1 wrong password 2 successfully
        QByteArray content;
        content.append("VER_RESULT");
        content.append('&');
        content.append(QString::number(information).toUtf8());
        qDebug() << "Sending VER_RESULT:" << content;
        clientSocket->write(content);
    }
    else if (instruction[0] == "PROFILE"){
        QString difficulty = instruction[1];
        qDebug() << "Processing profile request, difficulty:" << difficulty;

        QString list = database->showUserRankListWithDifficulty(difficulty);

        QByteArray content;
        content.append("PRO_RESULT");
        content.append('&');
        content.append(list.toUtf8());
        qDebug() << "Sending PRO_RESULT, length:" << list.length();
        clientSocket->write(content);
    }
    else if (instruction[0] == "RANKLIST"){
        QString difficulty = instruction[1];
        qDebug() << "Processing ranklist request, difficulty:" << difficulty;

        QString list = database->showRankListWithDifficulty(difficulty);

        QByteArray content;
        content.append("RAN_RESULT");
        content.append('&');
        content.append(list.toUtf8());
        qDebug() << "Sending RAN_RESULT, length:" << list.length();
        clientSocket->write(content);
    }
    else if (instruction[0] == "UPDATE"){
        QString difficultyText = instruction[2];  // 直接取文字
        database->update(instruction[1], difficultyText, instruction[3].toInt());
        QByteArray content;
        content.append("UPD_RESULT");
        clientSocket->write(content);
        qDebug() << "Updated score for user:" << instruction[1];
    }
    else if (instruction[0] == "SUBMIT_SCORE") {
        QString username = instruction[1];
        QString difficultyText = instruction[2];  // "简单"、"普通"、"困难"
        int score = instruction[3].toInt();

        qDebug() << "Received score - User:" << username
                 << "Difficulty:" << difficultyText
                 << "Score:" << score;

        database->update(username, difficultyText, score);

        QByteArray content;
        content.append("SCORE_SUBMITTED");
        clientSocket->write(content);
    }
    else {
        qDebug() << "Unknown instruction:" << instruction[0];
    }
}

void server::disconnect() {
    qDebug() << "Client disconnected";
    clientSocket->deleteLater();
}
