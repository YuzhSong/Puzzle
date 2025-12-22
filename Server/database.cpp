#include "database.h"
#include <algorithm> // 用于 sort

dataBase::dataBase()
{
    // 使用默认连接名称，避免重复 addDatabase 导致的警告
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("bwjgame.db");
    }

    if (!database.open()) {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    } else {
        qDebug() << "Succeed to connect database.";
    }

    QSqlQuery sql_query;
    // 建表 users
    if(!sql_query.exec("create table if not exists users(id VARCHAR(20) PRIMARY KEY, password VARCHAR(20))")) {
        qDebug() << "Error: Fail to create table users." << sql_query.lastError();
    }

    // 建表 players (建议加上 id 索引或主键，这里暂且保持原样)
    if(!sql_query.exec("create table if not exists players(id VARCHAR(20), score INTEGER, rank INTEGER)")) {
        qDebug() << "Error: Fail to create table players." << sql_query.lastError();
    }

    // 初始化缓存（如果不需要缓存全部数据，这步可以去掉以节省内存）
    this->inquire();
}

// 优化后的用户查询：直接用 SQL 查找，而不是遍历
int dataBase::readFromUsers(QString tempId, QString tempPwd) {
    QSqlQuery query;
    // 只查找特定 ID 的用户
    query.prepare("SELECT password FROM users WHERE id = :id");
    query.bindValue(":id", tempId);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError();
        return 0; // 查询出错当作不存在
    }

    if (query.next()) {
        // 查到了用户，比对密码
        QString storedPwd = query.value(0).toString();

        // 如果 tempPwd 为空（用于注册查重），则只看存在与否
        if (tempPwd.isEmpty()) {
            return 2; // 用户存在
        }

        if (storedPwd == tempPwd) {
            return 2; // 密码正确
        } else {
            return 1; // 密码错误
        }
    } else {
        return 0; // 用户不存在
    }
}

// 登录逻辑
int dataBase::loginFunc(QString tempId, QString tempPwd) {
    int flag = readFromUsers(tempId, tempPwd);
    if(flag == 2) {
        user a;
        a.username = tempId;
        a.password = tempPwd;
        setUser(a);

        // 注意：这里仅设置当前内存里的 player 状态，不一定代表数据库里有分
        player b;
        b.username = tempId;
        b.score = 0;
        b.rank = 0;
        setPlayer(b);
    }
    return flag;
}

// 注册逻辑：修复了之前查询 players 表的错误
int dataBase::registerFunc(QString tempId, QString tempPwd) {
    // 检查 users 表中是否已存在该 ID
    // 传入空密码，只为了检查 ID 是否存在
    int exists = readFromUsers(tempId, "");

    if (exists != 0) {
        return 0; // 用户名已存在，注册失败
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (id, password) VALUES (?, ?)");
    query.addBindValue(tempId);
    query.addBindValue(tempPwd);

    if(!query.exec()) {
        qDebug() << "Register Error:" << query.lastError();
        return 0;
    } else {
        qDebug() << "User registered:" << tempId;
        return 1;
    }
}

// 更新分数
void dataBase::update(QString username, int score) {
    QSqlQuery query;
    // 插入一条新的游戏记录
    query.prepare("INSERT INTO players (id, score, rank) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(score);
    query.addBindValue(0); // 初始 rank 存 0，显示时再计算

    if(!query.exec()) {
        qDebug() << "Update Error:" << query.lastError();
    } else {
        qDebug() << "Score inserted for" << username;
    }
}

// 辅助函数：将数据库全量载入内存（原有逻辑保留）
void dataBase::inquire() {
    users.clear();
    players.clear();

    QSqlQuery query;

    // 载入 Users
    if(query.exec("SELECT * FROM users")) {
        while(query.next()) {
            user u;
            u.username = query.value(0).toString();
            u.password = query.value(1).toString();
            users.push_back(u);
        }
    }

    // 载入 Players
    if(query.exec("SELECT * FROM players")) {
        while(query.next()) {
            player p;
            p.username = query.value(0).toString();
            p.score = query.value(1).toInt();
            p.rank = query.value(2).toInt();
            players.push_back(p);
        }
    }
}

bool compare(player a, player b) {
    return a.score > b.score;
}

// 显示全球排行榜
QString dataBase::showRankList() {
    // 每次查询前刷新数据，确保看到最新排名
    this->inquire();

    if (players.empty()) return "No Ranking Data";

    sort(players.begin(), players.end(), compare);

    QString s = "ID\t\tSCORE\t\tRANK\n";
    int rank = 1;
    for (auto &p : players) {
        p.rank = rank;
        s += p.username + "\t\t" + QString::number(p.score) + "\t\t" + QString::number(p.rank) + "\n";
        rank++;
    }
    return s;
}

// 显示个人历史战绩
QString dataBase::showUserRankList() {
    this->inquire(); // 刷新数据

    QString name = usr.username;
    if (name.isEmpty()) return "Please Login First";

    vector<player> tempPlayer;
    for (const auto &p : players) {
        if (p.username == name) {
            tempPlayer.push_back(p);
        }
    }

    if (tempPlayer.empty()) return "No Game History";

    sort(tempPlayer.begin(), tempPlayer.end(), compare);

    QString s = "ID\t\tSCORE\n";
    for (const auto &p : tempPlayer) {
        s += p.username + "\t\t" + QString::number(p.score) + "\n";
    }
    return s;
}

// 占位函数保留
vector<player> dataBase::getPlayers() { return players; }
player dataBase::getPlayer() { return gamer; }
void dataBase::setPlayer(player a) { gamer = a; }
void dataBase::setUser(user a) { usr = a; }
int dataBase::readFromPlayers(QString id) { return 0; } // 不再需要使用，但保留接口防报错
