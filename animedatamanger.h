#ifndef ANIMEDATA_H
#define ANIMEDATA_H

#include <QString>
#include <QHash>
#include <QVector>
#include <QDate>
#include <QtMath>
#include <QDebug>
#include "CustomAlgorithms.h"
#include "datadefine.h"

class AnimeDataManger
{
public:
    AnimeDataManger();

    AnimeDataManger(QString AnimeName)
    {
        this->allAnimeNames.push_back(AnimeName);
    }

    AnimeDataManger(QHash<QString,QVector<QString>> &AnimeData , QString AnimeName)
    {
        this->allAnimeNames.push_back(AnimeName);
        update(AnimeData);
    }

    void IniData(QHash<QString,QVector<QString>> &AnimeData);
    void update(QHash<QString,QVector<QString>> &AnimeData);

    sAnimeData GetFinalScore(int index);

    double sumScoreF = 5;         // 保留固定小数的总评分
    double sumScore = 5;           // 总评分
    double storyScore = 0;         // 剧情评分
    double screenScore = 0;        // 画面评分
    double roleScore = 0;          // 人设评分

    QDate viewStartDate;           // 开始观看日期
    QDate viewLastDate;            // 最后观看日期
    QDate updateStartDate;         // 开始更新日期
    QDate completionDate;          // 完成日期

    QVector<QString> seasons;      // 季节列表
    QString displayContent = "";     // 用于表格显示的内容（时间或集数）

    QString downloadName = "";      // 下载使用的名称
    QString onlineUrl = "";         // 在线观看网址
    QString localFilePath = "";     // 本地文件路径
    double floderSize = 0.0;        // 占用内存

    QString thisBangumiLink = "";   // BangumiLink

    bool isAbandoned = false;       // 是否放弃观看
    bool isMovie = false;           // 是否为动画电影
    bool hasWatched = false;        // 是否已观看
    bool isCurrentSeason = false;   // 是否为当季新番
    bool isCurrentlyWatching = false;// 是否正在观看
    bool canWatchNow = true;        // 当前是否可以观看
    bool isSeasonAnime = false;     // 是否为季番
    bool isCatchingUpAnime = false; // 是否为在补番剧
    bool hasLocalFile = false;      // 是否有本地文件

    int episodesGivenUp = 0;        // 放弃观看的集数

    int watchInterval = 0;          // 追番间隔时间
    int totalWatchTime = 0;         // 追番总时间
    int currentEpisode = 0;         // 当前追番集数
    int totalEpisodes = 0;          // 动漫总集数
    int startEpisode = 100;         // 开始观看的集数
    int updateDay = -1;             // 更新的星期几（1-7，分别代表星期一到星期日）

    int extraOnlineEpisodes = 0;    // 在线搜索时额外添加的集数
    int extraDownloadEpisodes = 0;  // 下载时额外添加的集数

    QVector<sAnimeData> interpolatedEpisodeScores;   // 保存经过插值后的每个单集得分
    QHash<QString, sAnimeData> addedEpisodeScores;   // 保存被添加进的所有单集得分
    QVector<QString> allCompleteScores;              // 保存被添加的所有完结得分
    QVector<QString> allAnimeNames;                  // 保存所有该动漫的所有名称

    QVector<QPair<QString, QString>> labels;             // 保存处理过的标签

};

#endif // ANIMEDATA_H
