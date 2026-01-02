#include "database.h"
#include <QFile>        // 添加QFile头文件
#include <QIODevice>    // 添加QIODevice头文件
#include <QDir>         // 添加QDir头文件
#include <QCoreApplication>  // 如果需要使用 applicationDirPath()

dataBase::dataBase()
{
    qDebug() << "Initializing database...";

    // 获取当前目录
    QString currentDir = QDir::currentPath();
    qDebug() << "Current directory:" << currentDir;

    // 创建SQLite数据库连接
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");

    // 尝试多个可能的数据库文件位置
    QString dbPath1 = currentDir + "/bwjgame.db";
    QString dbPath2 = QCoreApplication::applicationDirPath() + "/bwjgame.db";
    QString dbPath3 = "bwjgame.db";

    qDebug() << "Trying database path 1:" << dbPath1;
    qDebug() << "Trying database path 2:" << dbPath2;
    qDebug() << "Trying database path 3:" << dbPath3;

    // 优先使用应用程序目录
    database.setDatabaseName(dbPath2);

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();

        // 尝试其他路径
        database.setDatabaseName(dbPath1);
        if (!database.open()) {
            database.setDatabaseName(dbPath3);
            if (!database.open()) {
                qDebug() << "Error: Cannot open database at any location";
                return;
            }
        }
    }

    qDebug() << "Succeed to connect database at:" << database.databaseName();

    // 检查表是否存在
    if(!checkTablesExist()) {
        createTables();
    }

    this->inquire();
}

void dataBase::createTables() {
    qDebug() << "Creating database tables...";

    QSqlQuery sql_query;

    // 创建users表
    QString createUsersTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "username VARCHAR(50) PRIMARY KEY, "
        "password VARCHAR(50) NOT NULL, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    if(!sql_query.exec(createUsersTable)) {
        qDebug() << "Error: Fail to create users table." << sql_query.lastError();
    } else {
        qDebug() << "Users table created or already exists!";
    }

    // 创建players表（存储游戏记录）
    QString createPlayersTable =
        "CREATE TABLE IF NOT EXISTS players ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username VARCHAR(50) NOT NULL, "
        "difficulty INT NOT NULL DEFAULT 1, "
        "score INT NOT NULL, "
        "play_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY (username) REFERENCES users(username) ON DELETE CASCADE"
        ")";

    if(!sql_query.exec(createPlayersTable)) {
        qDebug() << "Error: Fail to create players table." << sql_query.lastError();
    } else {
        qDebug() << "Players table created or already exists!";
    }

    // 为players表创建索引，提高查询速度
    sql_query.exec("CREATE INDEX IF NOT EXISTS idx_score_difficulty ON players(score DESC, difficulty)");
    sql_query.exec("CREATE INDEX IF NOT EXISTS idx_username ON players(username)");
    sql_query.exec("CREATE INDEX IF NOT EXISTS idx_play_time ON players(play_time DESC)");

    qDebug() << "Database tables created successfully.";
}

bool dataBase::checkTablesExist() {
    QSqlQuery sql_query;

    // 检查users表是否存在
    if(!sql_query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='users'")) {
        qDebug() << "Error checking users table:" << sql_query.lastError();
        return false;
    }
    bool usersTableExists = sql_query.next();

    // 检查players表是否存在
    if(!sql_query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='players'")) {
        qDebug() << "Error checking players table:" << sql_query.lastError();
        return false;
    }
    bool playersTableExists = sql_query.next();

    qDebug() << "Users table exists:" << usersTableExists << "Players table exists:" << playersTableExists;
    return (usersTableExists && playersTableExists);
}

int dataBase::readFromUsers(QString tempId, QString tempPwd) {
    bool userFlag = false;
    bool pwdFlag = false;

    QSqlQuery sql_query;
    QString select_all_sql = "SELECT * FROM users";

    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug() << "Query error:" << sql_query.lastError();
        return 0;
    }
    else
    {
        while(sql_query.next())
        {
            QString id = sql_query.value(0).toString();
            QString password = sql_query.value(1).toString();

            if(tempId == id) {
                userFlag = true;
                if(tempPwd == password) {
                    pwdFlag = true;

                    user u;
                    u.username = id;
                    u.password = password;

                    this->setUser(u);
                    break;
                }
                break;
            }
        }

        if (!userFlag) {
            return 0; // 用户不存在
        }

        if (userFlag && !pwdFlag) {
            return 1; // 密码错误
        }

        return 2; // 登录成功
    }
}

int dataBase::readFromPlayers(QString tempId) {
    bool userFlag = false;
    QSqlQuery sql_query;
    QString select_all_sql = "SELECT * FROM players";

    QString id;

    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return 1;
    }
    else
    {
        while(sql_query.next())
        {
            id = sql_query.value(1).toString(); // username在第二列

            if(tempId == id) {
                userFlag = true;
                break;
            }
        }

        if(userFlag) {
            return 0;
        } else {
            return 1;
        }
    }
}

int dataBase::loginFunc(QString tempId, QString tempPwd) {
    int flag = readFromUsers(tempId,tempPwd);
    if(flag == 2) {
        user a;
        a.username = tempId;
        a.password = tempPwd;
        setUser(a);

        player b;
        b.username = tempId;
        b.score = 0;
        b.rank = 0;
        b.difficulty = 1;
        b.play_time = QDateTime::currentDateTime();
        setPlayer(b);
    }

    return flag;
}

int dataBase::registerFunc(QString tempId, QString tempPwd) {
    // 1. 检查用户是否存在
    QSqlQuery check_query; // 使用专门的 check 变量
    QString check_sql = "SELECT username FROM users WHERE username = ?";
    check_query.prepare(check_sql);
    check_query.addBindValue(tempId);

    if(!check_query.exec()) {
        qDebug() << "Error checking user:" << check_query.lastError().text();
        return 0;
    }

    if(check_query.next()) {
        qDebug() << "User already exists:" << tempId;
        return 0;
    }

    // 2. 插入新用户
    QSqlQuery insert_query; // 使用全新的对象，避免之前的参数残留
    QString insert_sql = "INSERT INTO users (username, password) VALUES (?, ?)";
    insert_query.prepare(insert_sql);
    insert_query.addBindValue(tempId);
    insert_query.addBindValue(tempPwd);

    if(!insert_query.exec()) {
        qDebug() << "Insert error:" << insert_query.lastError().text();
        return 0;
    } else {
        qDebug() << "User registered successfully:" << tempId;
        return 1;
    }
}

void dataBase::inquire() {
    users.clear();
    players.clear();

    QSqlQuery sql_query;

    // 查询users表
    QString select_all_sql = "SELECT * FROM users";
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug() << "Query users error:" << sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString id = sql_query.value(0).toString();
            QString password = sql_query.value(1).toString();
            user u;
            u.username = id;
            u.password = password;
            this->users.push_back(u);
            qDebug() << "User:" << id;
        }
    }

    // 查询players表
    QString select_all_sql1 = "SELECT * FROM players ORDER BY score DESC, difficulty DESC, play_time DESC";
    sql_query.prepare(select_all_sql1);
    if(!sql_query.exec())
    {
        qDebug() << "Query players error:" << sql_query.lastError();
    }
    else
    {
        int rank = 1;
        while(sql_query.next())
        {
            QString username = sql_query.value(1).toString();
            int difficulty = sql_query.value(2).toInt();
            int score = sql_query.value(3).toInt();
            QDateTime play_time = sql_query.value(4).toDateTime();

            player u;
            u.username = username;
            u.score = score;
            u.rank = rank++;
            u.difficulty = difficulty;
            u.play_time = play_time;

            this->players.push_back(u);
            qDebug() << QString("Player: %1, Score: %2, Difficulty: %3, Time: %4")
                            .arg(username).arg(score).arg(difficulty).arg(play_time.toString("yyyy-MM-dd hh:mm"));
        }
    }
}

bool dataBase::compare(player a, player b) {
    // 先按分数降序，再按难度降序
    if(a.score != b.score) {
        return a.score > b.score;
    }
    return a.difficulty > b.difficulty;
}

void dataBase::update(QString username, int difficulty, int score) {
    QSqlQuery sql_query;

    QString insert_sql = "INSERT INTO players (username, difficulty, score) VALUES (?, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(username);
    sql_query.addBindValue(difficulty);
    sql_query.addBindValue(score);

    if(!sql_query.exec())
    {
        qDebug() << "Insert score error:" << sql_query.lastError();
    }
    else
    {
        qDebug() << "Score inserted - User:" << username
                 << "Difficulty:" << difficulty
                 << "Score:" << score;
    }

    // 重新查询更新数据
    this->inquire();
}

void dataBase::update(QString username, int score) {
    // 保持向后兼容，使用默认难度1
    update(username, 1, score);
}

QString dataBase::showRankList() {
    users.clear();
    players.clear();
    this->inquire();

    sort(players.begin(), players.end(), compare);

    QString s = "Rank\tUsername\tScore\tDifficulty\tTime\n";
    s += "---------------------------------------------------\n";

    int i = 1;
    for (auto& player : players) {
        player.rank = i;
        s += QString("%1\t%2\t%3\t%4\t%5\n")
                 .arg(i)
                 .arg(player.username)
                 .arg(player.score)
                 .arg(player.difficulty)
                 .arg(player.play_time.toString("MM-dd hh:mm"));
        i++;
        if(i > 20) break; // 只显示前20名
    }

    return s;
}

QString dataBase::showRankListWithDifficulty(int difficulty) {
    users.clear();
    players.clear();
    this->inquire();

    // 根据难度筛选
    vector<player> filteredPlayers;
    if(difficulty == 0) {
        filteredPlayers = players;
    } else {
        for(auto& p : players) {
            if(p.difficulty == difficulty) {
                filteredPlayers.push_back(p);
            }
        }
    }

    // 按分数降序排序
    sort(filteredPlayers.begin(), filteredPlayers.end(), compare);

    QString s;
    if(difficulty == 0) {
        s = "=== All Difficulties Ranking ===\n";
    } else {
        s = QString("=== Difficulty %1 Ranking ===\n").arg(difficulty);
    }

    s += "Rank\tUsername\tScore\tTime\n";
    s += "------------------------------------\n";

    int rank = 1;
    for(auto& p : filteredPlayers) {
        s += QString("%1\t%2\t%3\t%4\n")
        .arg(rank)
            .arg(p.username)
            .arg(p.score)
            .arg(p.play_time.toString("MM-dd hh:mm"));
        rank++;
        if(rank > 20) break;
    }

    return s;
}

QString dataBase::showUserRankList() {
    users.clear();
    players.clear();
    this->inquire();

    QString s = "";
    QString name = usr.username;

    vector<player> tempPlayer;

    for(auto& p : players) {
        if(p.username == name) {
            tempPlayer.push_back(p);
        }
    }

    if(tempPlayer.empty()) {
        s = "No game records found for user: " + name + "\n";
        return s;
    }

    s = "=== Your Game Records ===\n";
    s += "Score\tDifficulty\tTime\n";
    s += "---------------------------\n";

    // 按时间倒序排序（最近玩的在前面）
    sort(tempPlayer.begin(), tempPlayer.end(), [](player a, player b) {
        return a.play_time > b.play_time;
    });

    for(auto& p : tempPlayer) {
        s += QString("%1\t%2\t%3\n")
        .arg(p.score)
            .arg(p.difficulty)
            .arg(p.play_time.toString("MM-dd hh:mm"));
    }

    return s;
}

vector<player> dataBase::getPlayers() {
    return players;
}

player dataBase::getPlayer() {
    return gamer;
}

void dataBase::setPlayer(player a) {
    gamer = a;
}

void dataBase::setUser(user a) {
    usr = a;
}
