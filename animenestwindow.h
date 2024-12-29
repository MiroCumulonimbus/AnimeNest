#ifndef ANIMENESTWINDOW_H
#define ANIMENESTWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtCharts>

#include "CustomAlgorithms.h"
#include "animedatamanger.h"
#include "myobject.h"
#include "datadefine.h"

#include "networkfetch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AnimeNestWindow; }
QT_END_NAMESPACE

struct tableData{
    QVector<QString> insertData;
    QString Name;
    QString BangumiLink;
};

class AnimeNestWindow : public QMainWindow
{
    Q_OBJECT

public:
    AnimeNestWindow(QWidget *parent = nullptr);
    ~AnimeNestWindow();


//  add 添加类型函数
    void AddNewScore(QString Name , QHash<QString,QVector<QString>> Info);   // 添加分数
    void AddNewLabel(QString Name , QString Label);                          // 添加标签
    void AddUrlOrName(QString Name , QString Info , int index);

//  delete 删除类型函数
    void DeleteSigEpisode(QString Name , QString Episode);  // 按单集删除
    void SigFullDelete(QString Name);
    void DeleteLabel(QString Name , QString Label);         // 删除标签

    // 查询函数
    void LoadAnimeWithSeason(QString targetLabel, int votesLimit = 100, int totalEpisodeLimit = 5);
    void LoadAnimeWithLabel(QVector<QString> targetLabel, int votesLimit = 100, int totalEpisodeLimit = 5);
    void LoadAnimeWithName(QString Name, int votesLimit = 100, int totalEpisodeLimit = 5);
    void LoadAnimeWithProducer(QString Name, int matchType);
//    void LoadAnimeBaseFunction(const QString &key, const QHash<QString, QVector<QString>> &value, int &count);

    void GetFindAnime(QString Name);
    void GetAllAnime();
    void GetDiscardAnime();
    void GetDiscardAnime2();
    void GetWatchingLastAnime();
    void GetAnimeMovie();
    void GetLocalSeasonAnime();
    void GetRecentUpdate();
    void GetRecentUpdate(const QHash<QString, QHash<QString, QVector<QString>>> &updateInfo);
    void GetShelvedAnime();
    void GetWatchedAnime();
    void GetWatchingAnime();
    void GetLabelAnime(QString Label , int LabelCount);             // 按标签查找

    bool OpenLocalFile(QString Name , int episode);         // 打开本地路径文件
    bool OpenWebUrl(QString Name , int episode);            // 打开浏览器观看
//    void DownLoadNew();                                     // 打开网页下载
    void CreateNewLocalUrl(QString Name);                   // 创建本地目录

//    void AdvancedDownloaded();                              // 高级筛选 - 已下载

//  设置右键菜单
    void RightClickDetailWidget(const QPoint &pos);
    void RightClickLabelWidget(const QPoint &pos);

//  更新函数
    void UpdateAnimeMangerMap(const QString &Name = "");   // 更新 AnimeMangerMap 类参数
    void UpdateSplineChart(const QString &Name);// 更新图表
    void UpdateHistoryView();                   // 更新 HistoryViewDetail 统计的 当日/当周/当月 番剧
    void UpdateHistoryViewDetailInput(QString Name);    // 更新 HistoryViewDetailInput 统计的 当日/当周/当月 番剧

    bool UpdateEpisodeListWidget(const QString &Name, const QString &BangumiLink);
    void UpdateBangumiSelfService(const QString &Name, const QString &BangumiLink, bool hasBangumiLink);
    void UpdategirigiriloveSelfService(const QString &Name, const QString &BangumiLink, bool hasgirigiriloveLink);
    void UpdateDetailDisplayANDFollowingAnimeData(const QString &Name, const QString &BangumiLink);
    void UpdateSettingWidget(const QString &Name, const QString &BangumiLink);
    void UpdateDetailInfo(QString Name, QString BangumiLink);

    void UpdateDataFromBangumi(QString Name, QString BangumiLink);

    // 表格操作
    tableData tableDataHandle(AnimeDataManger item);
    tableData tableDataHandle(QHash<QString, QVector<QString>> item, QString BangumiLink);
    void InsertItem(tableData myItem, int &countInsertItem);
    void TableWidgetClick();

    // 初始化
    void IniChartANDTables();
    void IniTable(int type);
    void IniData();
    void IniCalendarWidget();             // 初始化 Calendar & Widget 信息

    void SetCopyName(bool);            // 设置剪贴板

    void closeEvent(QCloseEvent *);                // 关闭事件
    bool eventFilter(QObject *obj, QEvent *event);

//    QHash<QString,QHash<QString,QVector<QString>>> AnimeJsonMap;
//    QHash<QString,QHash<QString,QVector<QString>>> AnimeFetchedBangumiData, AnimeFetchedAnilistData;
//    QHash<QString,QHash<QString,QVector<QString>>> AnimeFetchedGirigiriloveData, AnimeFetchedIyxdmData;
    QHash<QString,QHash<QString,QVector<QString>>> Setting;

    QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> LocalData, RankData, OnlineStreamData;
    QHash<QString, QHash<QString, QVector<QString>>> fastLinkBangumiToAll;
    QHash<QString, QVector<QString>> noFastLinkItem;
    QHash<QString, QString> dataFilesSaveUrl;

    QHash<QString , QVector<QString>> HistoryInDetail;

    QHash<QString, AnimeDataManger> AnimeMangerMap;
//    QVector<AnimeDataManger> tempAnimeTable;
    QVector<tableData> tempAnimeTable;

//    QString mainBangumiLink;

    int TableType = 1;

private:
    Ui::AnimeNestWindow *ui;
    QChart *m_chart;
    QLineSeries *storyseries , *screenseries , *roleseries ;
    QChartView *chartView;
    QValueAxis *axisX , *axisY;
};
#endif // ANIMENESTWINDOW_H
