#ifndef DATABASE_H
#define DATABASE_H

#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include "iostream"
#include <vector>

using namespace std;

typedef struct {
    QString username;
    QString password;
} user ;

typedef struct {
    QString username;
    int score;
    int rank;
    QString difficulty;      // 添加难度字段
    QDateTime play_time;     // 添加时间字段
} player;


class dataBase
{
public:
    dataBase();

    // 数据库初始化
    void createTables();
    bool checkTablesExist();

    // 用户相关
    int readFromUsers(QString id, QString pwd);
    int readFromPlayers(QString id);
    int loginFunc(QString id, QString pwd);
    int registerFunc(QString id, QString pwd);

    // 分数和排行榜相关
    void update(QString username, QString difficultyText, int score);
    void inquire();

    // 排行榜显示函数 - 修改接口
    QString showRankList();  // 保留原有接口用于兼容
    QString showRankListWithDifficulty(const QString &difficulty);  // 新的带难度参数版本

    QString showUserRankList();  // 保留原有接口用于兼容
    QString showUserRankListWithDifficulty(const QString &difficulty);  // 新的带难度参数版本

    // 数据获取
    vector<player> getPlayers();
    player getPlayer();
    void setPlayer(player a);
    void setUser(user a);

    // 比较函数
    static bool compare(player a, player b);
    static int getDifficultyWeight(const QString &difficulty);

private:
    vector<user> users;
    vector<player> players;
    user usr;
    player gamer;
};

#endif // DATABASE_H
