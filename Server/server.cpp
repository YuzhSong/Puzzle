#include "server.h"

server::server()
{
    database = new dataBase();
    serverEnd = new QTcpServer(this);

    // 连接新客户端进入的信号
    connect(serverEnd, &QTcpServer::newConnection, this, &server::clientHandler);

    // 监听任意地址 (Any)，不仅仅是 LocalHost，这样局域网也能连
    if (!serverEnd->listen(QHostAddress::Any, 8888)) {
        qDebug() << "Server Error: Could not start listening!";
    } else {
        qDebug() << "Server started, listening on port 8888...";
    }
}

void server::clientHandler() {
    if (serverEnd->hasPendingConnections()) {
        QTcpSocket *socket = serverEnd->nextPendingConnection();

        // 关键：连接信号时，不要保存 socket 到成员变量 clientSocket
        // 而是让 readData 动态获取发送者
        connect(socket, &QTcpSocket::readyRead, this, &server::readData);
        connect(socket, &QTcpSocket::disconnected, this, &server::disconnect);

        qDebug() << "Client connected from:" << socket->peerAddress().toString();
    }
}

void server::readData() {
    // 获取发送信号的那个 socket 对象
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray rawData = socket->readAll();
    QString buffer = QString::fromUtf8(rawData); // Qt6 推荐写法

    qDebug() << "Received Request:" << buffer;

    // 处理可能存在的空包
    if (buffer.isEmpty()) return;

    QStringList instruction = buffer.split('&');
    if (instruction.isEmpty()) return;

    QString type = instruction[0];
    QByteArray response; // 准备回传的数据

    // --- 1. 注册请求 (REGISTER&id&pwd) ---
    if (type == "REGISTER") {
        if (instruction.size() >= 3) {
            int result = database->registerFunc(instruction[1], instruction[2]);
            response.append("REG_RESULT&");
            response.append(QString::number(result).toUtf8());
            socket->write(response);
            qDebug() << "Register result sent:" << result;
        }
    }
    // --- 2. 登录请求 (VERIFY&id&pwd) ---
    else if (type == "VERIFY") {
        if (instruction.size() >= 3) {
            int result = database->loginFunc(instruction[1], instruction[2]);
            response.append("VER_RESULT&");
            response.append(QString::number(result).toUtf8());
            socket->write(response);
            qDebug() << "Login result sent:" << result;
        }
    }
    // --- 3. 个人历史战绩 (PROFILE) ---
    else if (type == "PROFILE") {
        // 获取该用户的历史战绩字符串
        QString list = database->showUserRankList();
        response.append("PRO_RESULT&");
        response.append(list.toUtf8());
        socket->write(response);
        qDebug() << "Profile data sent.";
    }
    // --- 4. 全球排行榜 (RANKLIST) ---
    else if (type == "RANKLIST") {
        QString list = database->showRankList();
        response.append("RAN_RESULT&");
        response.append(list.toUtf8());
        socket->write(response);
        qDebug() << "Ranklist sent.";
    }
    // --- 5. 更新分数 (UPDATE&id&score) ---
    else if (type == "UPDATE") {
        if (instruction.size() >= 3) {
            bool ok;
            int score = instruction[2].toInt(&ok);
            if (ok) {
                database->update(instruction[1], score);
                qDebug() << "Score updated for" << instruction[1];
            }
        }
    }
}

void server::disconnect() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        qDebug() << "Client Disconnected:" << socket->peerAddress().toString();
        socket->deleteLater(); // 安全删除资源
    }
}
