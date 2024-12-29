#ifndef CUSTOMALGORITHMS_H
#define CUSTOMALGORITHMS_H

#endif // CUSTOMALGORITHMS_H
#pragma once

#include <QDate>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTime>
#include <QAction>
#include <QDir>
#include <QSet>
#include <QDebug>
#include <QTreeWidget>


// 获取指定路径下的所有文件夹和文件信息。
// 给定一个路径，返回该路径下的所有文件夹
QFileInfoList getFolders(const QString &path, bool createIfNotExist = false);
// 给定一个路径，返回该路径及其给定层数的子文件夹中的文件
QFileInfoList getFiles(const QString &path, int depth, QStringList filters = {"*.mp4", "*.avi", "*.mkv", "*.mov"});

// 根据 reference 查找 target 中匹配的 keys 字段值，并检测冲突
QVector<QVector<QString>> getRefKey(const QVector<QString> &reference,
                                     const QHash<QString, QHash<QString, QVector<QString>>> &target,
                                     const QVector<QString> &keys);

QVector<QVector<QString>> vectorStringCut(const QVector<QString> &input, const QString &cutType);

QString getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                QString key1, QString key2, QString key3, int vecTarget);

QVector<QString> getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                         QString key1, QString key2, QString key3);

QHash<QString, QVector<QString>> getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                                                           QString key1, QString key2);

QHash<QString, QHash<QString, QVector<QString>>> getHash(const QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> &target,
                                          QString key1);
QString getHash(const QHash<QString, QHash<QString, QVector<QString>>> &target, QString key1, QString key2, int vecTarget);
QVector<QString> getHash(const QHash<QString, QHash<QString, QVector<QString>>> &target, QString key1, QString key2);
QHash<QString, QVector<QString>> getHash(const QHash<QString, QHash<QString, QVector<QString>>> &target, QString key1);

QString getHash(const QHash<QString, QVector<QString>> &target, QString key1, int vecTarget);
QVector<QString> getHash(const QHash<QString, QVector<QString>> &target, QString key1);

// 数据展示
void dataDisplay(const QHash<QString,QHash<QString,QVector<QString>>> &data, QTreeWidget *displayWidget);


// days 返回 d1 -> d2有多少天 d2日期大时为正数
int daysBetween(const QString& d1, const QString& d2);
int daysBetween(const QDateTime& d1, const QDateTime& d2);
int daysBetween(const QDate& d1, const QDate& d2);

// 以  /  格式返回日期
// 生成当前时间/"yyyy.MM.dd" 年月日 /"ddd" 星期几 /"yyyy年M月" 当前季度
QString getLocalTime();
QString getLocalWeek();
QString getLocalSeason();

// 给定一个QString日期 yyyy年M月d日 和 一个int总集数 n 返回季度
QVector<QString> getQStringSeasons(QString season, int totalEpisode);

// 信息弹窗
void messageBoxWindow(QString message);

// vector -> string
// {a , b , c} -> a<addType>b<addType>c
QString stringVectorToString(QVector<QString> vString, QString addType);

// 判断两个数组是否有相同的元素
bool vectorMatch(QVector<QString> v1, QVector<QString> v2);
// 判断两个数组是否有相同的元素(模糊匹配)
int vectorMatchBlur(QVector<QString> v1, QVector<QString> v2, int similarityThreshold);
int levenshteinDistance(const QString &s1, const QString &s2, int threshold);
// 数组去重且保留原顺序
QVector<QString> removeDuplicates(const QVector<QString>& vec);
// 标签去重
QVector<QString> removeDuplicateLabels(const QVector<QString>& vec);

// 数据匹配与合并
// 返回值: 合并结果（例：将为“Name”添加“***”）
// result: 两组数据合并的结果
QString dataMatch(QVector<QString> v1, QVector<QString> v2, QVector<QString>& result);
QString dataMatch(QHash<QString,QVector<QString>> v1,
                  QHash<QString,QVector<QString>> v2,
                  QHash<QString,QVector<QString>>& result,
                  QHash<QString,QVector<QString>>& addedData);

QString dataMatch(QHash<QString,QHash<QString,QVector<QString>>> v1,
                  QHash<QString,QHash<QString,QVector<QString>>> v2,
                  QHash<QString,QHash<QString,QVector<QString>>>& result,
                  QHash<QString,QHash<QString,QVector<QString>>>& addedData);

// 文件读写
void loadData(QString fileName, QHash<QString,QHash<QString,QVector<QString>>>& hashOfSS_V);
void saveData(QString fileName, QHash<QString,QHash<QString,QVector<QString>>>& hashOfSS_V);

//  设置日志信息
void setLogsInfo(QString logs);
