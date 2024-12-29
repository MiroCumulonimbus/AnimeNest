 #include "animenestwindow.h"
#include "ui_animenestwindow.h"

//  图表初始化
void AnimeNestWindow::IniChartANDTables()
{
    // 创建并设置新的布局
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->tab_6);
    mainLayout->setSpacing(0);  // 设置控件间的间距为0
    mainLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局的四个边缘间距为0

    // 创建 QChartView 并设置渲染选项
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  // 设置为扩展模式

    // 设置 QChart 的边距，减少周围的空白区域
    m_chart->setMargins(QMargins(5, 0, 5, 5));  // 左、上、右、下的边距设置为0

    // 设置 QChartView 的边距，确保它没有多余的间隙
    chartView->setContentsMargins(0, 0, 0, 0);  // 去除视图的边距

    // 将 chartView 添加到布局中
    mainLayout->addWidget(chartView);

    m_chart->addSeries(storyseries);
    m_chart->addSeries(screenseries);
    m_chart->addSeries(roleseries);

    axisX->setLabelFormat("%g");
    axisX->setRange(1,12);
//    axisX->setTitleText("集数");
    axisY->setRange(1, 10);
//    axisY->setTitleText("分数");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    storyseries->attachAxis(axisX);
    storyseries->attachAxis(axisY);
    storyseries->setName("剧情");
    screenseries->attachAxis(axisX);
    screenseries->attachAxis(axisY);
    screenseries->setName("画面");
    roleseries->attachAxis(axisX);
    roleseries->attachAxis(axisY);
    roleseries->setName("人设");
    // 启用图例（Legend）以显示每个系列的名称
    m_chart->legend()->setVisible(true);

    ui->HistoryViewDetail->setColumnCount(1);
    ui->HistoryViewDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->HistoryViewDetail->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->HistoryViewDetail->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("日志"));

    ui->HistoryViewDetailInput->setColumnCount(5);
    ui->HistoryViewDetailInput->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->HistoryViewDetailInput->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->HistoryViewDetailInput->setSortingEnabled(true);
    ui->HistoryViewDetailInput->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->HistoryViewDetailInput->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("集数")
                                                   <<QString::fromUtf8("剧情")<<QString::fromUtf8("画面")<<QString::fromUtf8("人设")<<QString::fromUtf8("观看时间"));
    ui->HistoryViewDetailInput->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->BangumiSelfService->setColumnCount(3);
    ui->BangumiSelfService->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->BangumiSelfService->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->BangumiSelfService->setSortingEnabled(true);
    ui->BangumiSelfService->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("番名")<<QString::fromUtf8("Bangumi链接")<<QString::fromUtf8("更新链接"));
    ui->BangumiSelfService->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->girigiriloveSelfService->setColumnCount(3);
    ui->girigiriloveSelfService->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->girigiriloveSelfService->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->girigiriloveSelfService->setSortingEnabled(true);
    ui->girigiriloveSelfService->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("番名")<<QString::fromUtf8("girigirilove链接")<<QString::fromUtf8("更新链接"));
    ui->girigiriloveSelfService->setContextMenuPolicy(Qt::CustomContextMenu);


    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

//  按钮绑定
    ui->RecentUpdate->conLabel = ui->RecentUpdateInterval;
    ui->WatchingAnime->conLabel = ui->WatchingAnimeInterval;
    ui->WatchedAnime->conLabel = ui->WatchedAnimeCredibilityLevel;
    ui->DiscardAnime->conLabel = ui->DiscardAnimeInterval;
    ui->DiscardAnime2->conLabel = ui->DiscardAnime2Interval;
    ui->LocalSeasonAnime->conLabel = ui->LocalSeasonAnimeInterval;
    ui->LoadSeasonAnime->conLabel = ui->LoadSeasonAnimeInterval;

//  分段标签初始化
    ui->minScore->limitMax = 10;
    ui->minScore->AddValue = 0.1;
    ui->maxScore->limitMax = 10;
    ui->maxScore->AddValue = 0.1;

//    ui->LoadSeason->LabelType = 4;
    ui->FetchSeason->LabelType = 4;

    ui->dataToday->setDate(Today.date());
//    ui->LoadSeason->setText(LocalSeason);
    ui->FetchSeason->setText(LocalSeason);


    // 创建菜单
    QMenu *menu = new QMenu(this);

    // 示例站点数据
    QStringList sites = OnlineStreamData.keys();
    for(auto i : sites){
        if(!OpenOrder.contains(i))
            OpenOrder.append(i);
    }

    // 创建QAction，并为每个站点添加复选框
    for (const QString &site : OpenOrder) {
        QAction *action = new QAction(site, this);
        action->setCheckable(true);  // 设置为复选框
        action->setChecked(true);  // 默认启用
        if(getHash(Setting, "打开方式", site, 0).isEmpty()){
            Setting["打开方式"][site] = {"1"};
        }
        else if(!getHash(Setting, "打开方式", site, 0).isEmpty() && getHash(Setting, "打开方式", site, 0) == "0")
            action->setChecked(false);

        // 连接复选框状态变化的信号
        connect(action, &QAction::triggered, [action, site, this]() {
            if (action->isChecked()) {
                this->Setting["打开方式"][site] = {"1"};
            } else {
                this->Setting["打开方式"][site] = {"0"};
            }
        });

        menu->addAction(action);
    }

    ui->OnlineWay->setMenu(menu);


}

// 表格初始化-设置表头等   tableWidget
void AnimeNestWindow::IniTable(int type){
    if(type == 1){
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("番名")
                                                       <<QString::fromUtf8("剧情")<<QString::fromUtf8("画面")<<QString::fromUtf8("人设")<<QString::fromUtf8("综合")<<QString::fromUtf8("观看集数"));

        ui->tableWidget->setColumnWidth( 1 , 50 );
        ui->tableWidget->setColumnWidth( 2 , 50 );
        ui->tableWidget->setColumnWidth( 3 , 50 );
        ui->tableWidget->setColumnWidth( 4 , 50 );
        ui->tableWidget->setColumnWidth( 5 , 100 );
        TableType = 1;
    }
    else if(type == 2){
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("番名")
                                                       <<QString::fromUtf8("Bangumi")<<QString::fromUtf8("Anilist")<<QString::fromUtf8("推荐度")<<QString::fromUtf8("在线")<<QString::fromUtf8("是否追番"));

        ui->tableWidget->setColumnWidth( 1 , 70 );
        ui->tableWidget->setColumnWidth( 2 , 70 );
        ui->tableWidget->setColumnWidth( 3 , 70 );
        ui->tableWidget->setColumnWidth( 4 , 50 );
        ui->tableWidget->setColumnWidth( 5 , 70 );
        TableType = 2;
    }

}

// 初始化 Calendar & Widget 信息
void AnimeNestWindow::IniCalendarWidget(){
//  计算每天看过的番
    HistoryInDetail.clear();
    for(auto j : AnimeMangerMap)
    {
        if(j.allAnimeNames.size() < 1)
            continue;
        for(int k = 0 ; k < j.interpolatedEpisodeScores.size() ; ++k)
        {
            HistoryInDetail[j.interpolatedEpisodeScores[k].Time.toString("yyyy.MM.dd")].push_back(j.allAnimeNames[0] + " "+ QString::number(k+1).rightJustified(2, '0'));
        }
        if(j.isMovie){
            HistoryInDetail[j.viewLastDate.toString("yyyy.MM.dd")].push_back(j.allAnimeNames[0]);
            HistoryInDetail[j.viewLastDate.toString("yyyy.MM.dd")].push_back(j.allAnimeNames[0]);
            HistoryInDetail[j.viewLastDate.toString("yyyy.MM.dd")].push_back(j.allAnimeNames[0]);
            HistoryInDetail[j.viewLastDate.toString("yyyy.MM.dd")].push_back(j.allAnimeNames[0]);
        }
    }
//  初始化当日信息
    ui->HistoryViewDetail->clearContents();
    ui->HistoryViewDetail->setRowCount(0);
    for(int i = 0 ; i < HistoryInDetail[Today.toString("yyyy.MM.dd")].size() ; ++i){
        int RowCont;
        RowCont=ui->HistoryViewDetail->rowCount();
        ui->HistoryViewDetail->insertRow(RowCont);//增加一行
        QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(HistoryInDetail[Today.toString("yyyy.MM.dd")][i]);
        ui->HistoryViewDetail->setItem(RowCont,0,WidgetAnime0);
    }
    ui->HistoryViewDetail->setSortingEnabled(true);

//  更新历史记录 calendarWidget修改颜色
    QTextCharFormat a;
    for(auto o = HistoryInDetail.begin() ; o != HistoryInDetail.end() ; ++o)
    {
        int colorValue = o.value().size()*10 > 255 ? 255 : o.value().size()*10;
        a.setBackground(QBrush(QColor(qRgb(255 - colorValue, 255 , 255 - colorValue))));
        ui->calendarWidget->setDateTextFormat( QDate::fromString(o.key() , "yyyy.MM.dd") , a);
    }
}


double getDirectorySizeInGB(const QString &dirPath) {
    QDir dir(dirPath);
    double totalSize = 0.0;

    // 获取目录下所有的文件和子目录
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &fileInfo : fileInfoList) {
        if (fileInfo.isDir()) {
            // 递归计算子目录的大小
            totalSize += getDirectorySizeInGB(fileInfo.filePath());
        } else {
            // 累加文件的大小
            totalSize += fileInfo.size();
        }
    }

    // 将大小从字节转换为GB，并返回保留一位小数的结果
    totalSize /= 1024.0 * 1024.0 * 1024.0;
    totalSize = QString::number(totalSize,'f',1).toDouble();
    return totalSize; // GB
}

//  AllMyAnimeLabel 初始化
void AnimeNestWindow::IniData(){
    AnimeMangerMap.clear();
//    mainBangumiLink.clear();
    fastLinkBangumiToAll.clear();
    noFastLinkItem.clear();
//    loadData("AnimeNestData.json", AnimeJsonMap);
//    loadData("BangumiData.json", AnimeFetchedBangumiData);
//    loadData("AnilistData.json", AnimeFetchedAnilistData);
//    loadData("GirigiriloveData.json", AnimeFetchedGirigiriloveData);
//    loadData("IyxdmData.json", AnimeFetchedIyxdmData);

    // 记录多线程方法的执行时间
    QElapsedTimer asyncTimer;
    asyncTimer.start();  // 启动计时器

    auto LocalDatafiles = getFiles("JsonFiles\\LocalData", 1, {"*.json"});
    auto RankDatafiles = getFiles("JsonFiles\\RankData", 1, {"*.json"});
    auto OnlineStreamDatafiles = getFiles("JsonFiles\\OnlineStreamData", 1, {"*.json"});
    for (auto i : LocalDatafiles) {
        QString fileNameWithoutExtension = i.fileName().chopped(5);  // 移除 .json 后缀
        dataFilesSaveUrl[fileNameWithoutExtension] = i.filePath();
        loadData(i.filePath(), LocalData[fileNameWithoutExtension]);
    }
    for (auto i : RankDatafiles) {
        QString fileNameWithoutExtension = i.fileName().chopped(5);  // 移除 .json 后缀
        dataFilesSaveUrl[fileNameWithoutExtension] = i.filePath();
        loadData(i.filePath(), RankData[fileNameWithoutExtension]);
    }
    for (auto i : OnlineStreamDatafiles) {
        QString fileNameWithoutExtension = i.fileName().chopped(5);  // 移除 .json 后缀
        dataFilesSaveUrl[fileNameWithoutExtension] = i.filePath();
        loadData(i.filePath(), OnlineStreamData[fileNameWithoutExtension]);
    }


    loadData("config.json", Setting);
    // 记录多线程方法的时间
        qDebug() << "计时器1：" << asyncTimer.elapsed() << "毫秒";
        asyncTimer.start();  // 启动计时器

//  导入已存储的 AnimeJsonMap 到 AnimeMangerMap
    for(auto i = LocalData.begin() ; i != LocalData.end() ; ++i){
        for(auto it=i.value().begin() ; it!= i.value().end() ; ++it){
            if(it.key() == "" || it.value().empty()){
                i.value().remove(it.key());
                continue;
            }
            else if(getHash(it.value(), "已删除", 0) == "1")
                continue;
            AnimeMangerMap[it.key()] = AnimeDataManger(it.value() ,it.key());
        }
    }
//  载入本地下载的文件到 AnimeMangerMap
    QVector<QString> findUrl = {"F:/Anime", "E:/Anime"};
    QFileInfoList findResult;
    for(auto i : findUrl){
        findResult.append(getFolders(i,true));
    }
    for(auto &i : findResult){
        QString Name = i.fileName();
        if(Setting["系统路径"]["屏蔽文件夹名"].contains(Name))
            continue;
        // 在文件中找出最大的集数（从1开始连续的最大的数）
        QFileInfoList getFilesCount = getFiles(i.filePath(), 1);
        QSet<int> episodeSet;
        int totalFilesCount = 0;
        QRegularExpression episodeRegex("(?<=[\\[\\s第E#])(\\d+?)(?=[\\]\\s集话話v.])");
        for (const QFileInfo &fileInfo : getFilesCount) {
            QString fileName = fileInfo.fileName();
            QRegularExpressionMatch match = episodeRegex.match(fileName);
            if (match.hasMatch()) {
                episodeSet.insert(match.captured(1).toInt());
            }
        }
        for (int i = 1; ; ++i) {
            if (episodeSet.contains(i)) {
                totalFilesCount = i; // 找到当前集数
            } else {
                break; // 找到缺失的集数，结束
            }
        }
        // 将文件路径中的正斜杠替换为双反斜杠
        QString filePathWithBackslashes = i.filePath().replace("/", "\\");
        if (AnimeMangerMap.contains(Name)) {
            AnimeMangerMap[Name].hasLocalFile = true;
            AnimeMangerMap[Name].localFilePath = filePathWithBackslashes;
            AnimeMangerMap[Name].floderSize =  getDirectorySizeInGB(AnimeMangerMap[Name].localFilePath);
            if (totalFilesCount != 0 && totalFilesCount < 200)
                AnimeMangerMap[Name].totalEpisodes = totalFilesCount;
        } else {
//            QHash<QString, QVector<QString>> temp;
//            qDebug() << "新词条: " << Name;
//            temp.insert("0", {"6", "6", "6", getLocalTime()});
//            AddNewScore(Name, temp);
            AnimeMangerMap[Name] = AnimeDataManger(Name);
            AnimeMangerMap[Name].canWatchNow = true;
            AnimeMangerMap[Name].hasLocalFile = true;
            AnimeMangerMap[Name].isCurrentlyWatching = true;
            AnimeMangerMap[Name].localFilePath = filePathWithBackslashes;
            AnimeMangerMap[Name].isCatchingUpAnime = true;
            AnimeMangerMap[Name].floderSize =  getDirectorySizeInGB(AnimeMangerMap[Name].localFilePath);
            if (totalFilesCount != 0 && totalFilesCount < 200)
                AnimeMangerMap[Name].totalEpisodes = totalFilesCount;
        }
    }
    // 记录多线程方法的时间
        qDebug() << "计时器2：" << asyncTimer.elapsed() << "毫秒";
        asyncTimer.start();  // 启动计时器
//    for(auto i : AnimeMangerMap){
//        if(i.totalEpisodes == 0 && (i.sumScore > 8.2 || i.hasLocalFile) &&
//                (AnimeFetchedData[i.allAnimeNames[0]]["总集数"].size() == 0 || AnimeFetchedData[i.allAnimeNames[0]]["总集数"][0].toInt() == 0))
//            qDebug() << i.allAnimeNames[0];
//    }

    // 快速链接，本地 link->Bangumi
    QHash<QString, QVector<QString>> fastLinkLocalToBangumi;
    for(auto i = AnimeMangerMap.begin(); i!= AnimeMangerMap.end(); ++i){
        if(i->thisBangumiLink == ""){
            QHash<QString, QHash<QString, QVector<QString>>> BangumiData = getHash(RankData, "Bangumi");
            for(auto j = BangumiData.begin(); j!= BangumiData.end(); ++j){
                if(vectorMatch(i->seasons, getHash(j.value(), "更新年月"))){
                    QVector<QString> bangumiNames = getHash(j.value(), "番名");
                    bangumiNames.append(getHash(j.value(), "别名"));
                    if(vectorMatch(i->allAnimeNames, bangumiNames)){
                        fastLinkLocalToBangumi[i->allAnimeNames[0]].append(j.key());
                    }
                }
            }
        }
        else{
            fastLinkLocalToBangumi[i->allAnimeNames[0]].append(i->thisBangumiLink);
        }
    }
    for(auto i = fastLinkLocalToBangumi.begin(); i!= fastLinkLocalToBangumi.end(); ++i){
        if(i.value().size() == 1){
            AnimeMangerMap[i.key()].thisBangumiLink = i.value()[0];
            fastLinkBangumiToAll[i.value()[0]]["AnimeMangerMap"] = {i.key()};
        }
    }
    for(auto i : AnimeMangerMap){
        if(i.thisBangumiLink == "")
            noFastLinkItem["AnimeMangerMap"].append(i.allAnimeNames[0]);
    }

    // 记录多线程方法的时间
        qDebug() << "计时器3：" << asyncTimer.elapsed() << "毫秒";
        asyncTimer.start();  // 启动计时器

//    // 快速链接，Bangumi->link all
//    for(auto i = RankData.begin(); i!= RankData.end();++i){
//        if(i.key() != "Bangumi"){
//            for(auto j = RankData["Bangumi"].begin(); j!= RankData["Bangumi"].end();++j){
//                QString LinkId = i.key();
//                if(getHash(j.value(), LinkId, 0) == ""){
//                    QHash<QString, QHash<QString, QVector<QString>>> Linkvalue = getHash(RankData, LinkId);
//                    for(auto k = Linkvalue.begin(); k!= Linkvalue.end(); ++k){
//                        if(vectorMatch(getHash(j.value(), "原名"), getHash(k.value(), "原名"))){
//                            fastLinkBangumiToAll[j.key()][LinkId].append(k.key());
//                        }
//                    }
//                }
//                else{
//                    fastLinkBangumiToAll[j.key()][LinkId] = getHash(j.value(), LinkId);
//                }
//            }
//        }
//    }

//    for(auto i = OnlineStreamData.begin(); i!= OnlineStreamData.end();++i){
//        for(auto j = RankData["Bangumi"].begin(); j!= RankData["Bangumi"].end();++j){
//            QString LinkId = i.key();
//            if(getHash(j.value(), LinkId, 0) == ""){
//                QHash<QString, QHash<QString, QVector<QString>>> Linkvalue = getHash(OnlineStreamData, LinkId);
//                for(auto k = Linkvalue.begin(); k!= Linkvalue.end(); ++k){
//                    if(vectorMatch(getHash(j.value(), "更新年月"), getHash(k.value(), "更新年月"))){
//                        QVector<QString> bangumiNames = getHash(j.value(), "番名");
//                        bangumiNames.append(getHash(j.value(), "别名"));
//                        if(vectorMatch(getHash(k.value(), "番名"), bangumiNames)){
//                            fastLinkBangumiToAll[j.key()][LinkId].append(k.key());
//                        }
//                    }
//                }
//            }
//            else{
//                fastLinkBangumiToAll[j.key()][LinkId] = getHash(j.value(), LinkId);
//            }
//        }
//    }
    //// 多线程测快速链接，Bangumi->link all
    bool doFastMarch = ui->FastMarch->isChecked();

    QMutex resultMutex;  // 用于保护共享资源
    QHash<QString, QHash<QString, QVector<QString>>> localResults;
    for(auto tt = RankData.begin(); tt != RankData.end() ; ++tt){
        QString LinkId = tt.key();
        if(LinkId != "Bangumi"){
            // 定义每个 LinkId 的并行处理任务
            auto processLinkTask = [&](const QString &BangumiId) {
                if (getHash(RankData["Bangumi"], BangumiId, LinkId, 0) != ""){
                    // 如果RankData中有LinkId，直接赋值
                    QMutexLocker locker(&resultMutex);
                    localResults[BangumiId][LinkId] = getHash(RankData["Bangumi"], BangumiId, LinkId);
                }
                else if(doFastMarch) {
                    // 如果当前RankData中没有LinkId的字段，继续查找匹配
                    QHash<QString, QHash<QString, QVector<QString>>> Linkvalue = getHash(RankData, LinkId);
                    for (auto k = Linkvalue.begin(); k != Linkvalue.end(); ++k) {
                        QVector<QString> romajiName = getHash(k.value(), "罗马字番名");
                        romajiName.append(getHash(k.value(), "英文番名"));
                        if(vectorMatch(getHash(RankData["Bangumi"], BangumiId, "原名"), getHash(k.value(), "原名"))){
                            // 将结果保存到局部变量中
                            QMutexLocker locker(&resultMutex);
                            localResults[BangumiId][LinkId].append(k.key());
                        }
                        else if(vectorMatch(getHash(RankData["Bangumi"], BangumiId, "别名"), romajiName)){
                            // 将结果保存到局部变量中
                            QMutexLocker locker(&resultMutex);
                            localResults[BangumiId][LinkId].append(k.key());
                        }
                    }
                }
            };

            // 并行化处理每个 LinkId
            QList<QString> linkIds = RankData["Bangumi"].keys();
            QtConcurrent::blockingMap(linkIds, processLinkTask);
        }
    }
    for (auto i = localResults.begin(); i != localResults.end(); ++i) {
        for (auto j = i.value().begin(); j != i.value().end(); ++j) {
            fastLinkBangumiToAll[i.key()][j.key()] = j.value();
        }
    }
    localResults.clear();

    // 记录多线程方法的时间
        qDebug() << "计时器4：" << asyncTimer.elapsed() << "毫秒";
        asyncTimer.start();  // 启动计时器

    for(auto tt = OnlineStreamData.begin(); tt != OnlineStreamData.end() ; ++tt){
        QString LinkId = tt.key();
        // 定义每个 LinkId 的并行处理任务
        auto processLinkTask = [&](const QString &BangumiId) {
            if (getHash(RankData["Bangumi"], BangumiId, LinkId, 0) != ""){
                // 如果RankData中有LinkId，直接赋值
                QMutexLocker locker(&resultMutex);
                localResults[BangumiId][LinkId] = getHash(RankData["Bangumi"], BangumiId, LinkId);
            }
            else if(doFastMarch) {
                // 如果当前RankData中没有LinkId的字段，继续查找匹配
                QHash<QString, QHash<QString, QVector<QString>>> Linkvalue = getHash(OnlineStreamData, LinkId);
                int maxBlur = 0;
                QString maxBlurItem = "";
                for (auto k = Linkvalue.begin(); k != Linkvalue.end(); ++k) {
                    if((!getHash(k.value(), "更新年月",0).isEmpty() &&
                            vectorMatch(getHash(RankData["Bangumi"], BangumiId, "更新年月"), getHash(k.value(), "更新年月")))
                            ||
                            (!getHash(k.value(), "年份",0).isEmpty() &&
                             vectorMatch(getHash(RankData["Bangumi"], BangumiId, "标签"), getHash(k.value(), "年份"))))
                    {
                        QVector<QString> bangumiNames = getHash(RankData["Bangumi"], BangumiId, "番名");
                        bangumiNames.append(getHash(RankData["Bangumi"], BangumiId, "别名"));
                        if (vectorMatch(getHash(k.value(), "番名"), bangumiNames)) {
                            QMutexLocker locker(&resultMutex);
                            localResults[BangumiId][LinkId].append(k.key());
                        }
                        else{
                            int localBlur = vectorMatchBlur(getHash(k.value(), "番名"), bangumiNames, IniAnimeNameMatchPrecise-20);
                            if(localBlur > IniAnimeNameMatchPrecise && localBlur > maxBlur){
                                maxBlur = localBlur;
                                maxBlurItem = k.key();
                            }
                        }
                    }
                }
                if(!maxBlurItem.isEmpty()){
                    QMutexLocker locker(&resultMutex);
                    localResults[BangumiId][LinkId].append(maxBlurItem);
                }
            }
        };

        // 并行化处理每个 LinkId
        QList<QString> linkIds = RankData["Bangumi"].keys();
        QtConcurrent::blockingMap(linkIds, processLinkTask);
    }

    // 合并局部结果到全局结果
    for (auto i = localResults.begin(); i != localResults.end(); ++i) {
        for (auto j = i.value().begin(); j != i.value().end(); ++j) {
            fastLinkBangumiToAll[i.key()][j.key()] = j.value();
        }
    }
    // 记录多线程方法的时间
        qDebug() << "计时器5：" << asyncTimer.elapsed() << "毫秒";
        asyncTimer.start();  // 启动计时器

//    QHash<QString, QHash<QString, int>> caculate;
    for(auto i = fastLinkBangumiToAll.begin(); i!= fastLinkBangumiToAll.end(); ++i){
        for(auto j = i.value().begin(); j!= i.value().end() ; ++j){
            if(j.value().size() == 1)
                RankData["Bangumi"][i.key()][j.key()] = j.value();
            else
                fastLinkBangumiToAll[i.key()].remove(j.key());
        }
    }

    QHash<QString, QSet<QString>> matchedItem;
    for(const auto &k : fastLinkBangumiToAll){
        for(auto j = k.begin(); j != k.end() ; ++j){
            matchedItem[j.key()].insert(j.value()[0]);
        }
    }
    for(auto i = RankData.begin(); i!= RankData.end(); ++i){
        if(i.key() == "Bangumi")
            continue;
        for(auto j = i.value().begin(); j!= i.value().end() ; ++j){
            if(!matchedItem[i.key()].contains(j.key())){
                noFastLinkItem[i.key()].append(j.key());
            }
        }
    }

    // 记录多线程方法的时间
        qDebug() << "计时器6：" << asyncTimer.elapsed() << "毫秒";

//    qDebug() << noFastLinkItem;

//    for(auto i = caculate.begin(); i!= caculate.end(); ++i){
//        qDebug() << "匹配值: " << i.key();
//        qDebug() << "总体个数: " << fastLinkBangumiToAll.size();
//        qDebug() << "匹配成功: " << i.value()["sucess"];
//        qDebug() << "无匹配: " << i.value()["empty"];
//        qDebug() << "多个匹配: " << i.value()["toomore"];
//    }
//    saveData("fastLinkBangumiToAll.json", fastLinkBangumiToAll);


//    QHash<QString, QHash<QString, QHash<QString, QVector<QString>>>> matchBangumi, matchGirigirilove;
//    QHash<QString, QHash<QString, AnimeDataManger>> matchLocal;
//    for(auto i = AnimeFetchedBangumiData.begin() ; i != AnimeFetchedBangumiData.end() ; ++i){
//        if(i.value().contains("标签")){
//            for(auto j : i.value()["标签"]){
//                if(j.contains("年") && j.contains("月")){
//                    QDate temp = QDate::fromString(j, "yyyy年M月");
//                    if(temp.isValid()){
//                        matchBangumi[j].insert(i.key(), i.value());
//                    }
//                }
//            }
//        }
//    }
//    for(auto i = AnimeFetchedGirigiriloveData.begin() ; i != AnimeFetchedGirigiriloveData.end() ; ++i){
//        if(i.value().contains("更新年月") && !i.value()["更新年月"].empty()){
//            if(i.value()["更新年月"][0].contains("年") && i.value()["更新年月"][0].contains("月")){
//                QDate temp = QDate::fromString(i.value()["更新年月"][0], "yyyy年M月");
//                if(temp.isValid()){
//                    matchGirigirilove[i.value()["更新年月"][0]].insert(i.key(), i.value());
//                }
//            }
//        }
//    }
//    for(auto i = AnimeMangerMap.begin() ; i != AnimeMangerMap.end() ; ++i){
//        for(auto j : i.value().seasons){
//            matchLocal[j].insert(i.key(), i.value());
//        }

//    }
//    int matchingSucess = 0, matchingFailEmpty = 0, matchingFailTooMore = 0, totalMatching = 0;
//    for(auto i = matchBangumi.begin() ; i != matchBangumi.end() ; ++i){
//        if(matchGirigirilove.contains(i.key())){
//            totalMatching += i.value().size();
//            for(auto j = i.value().begin() ; j != i.value().end() ; ++j){
//                QVector<QString> matchingResult;
//                for(auto k = matchGirigirilove[i.key()].begin() ; k != matchGirigirilove[i.key()].end() ; ++k){
//                    if(k.value().contains("番名")){
//                        if(j.value().contains("番名")){
//                            if(vectorMatch(k.value()["番名"], j.value()["番名"]))
//                                matchingResult.push_back(k.key());
//                        }
//                        if(j.value().contains("别名")){
//                            if(vectorMatch(k.value()["番名"], j.value()["别名"]))
//                                matchingResult.push_back(k.key());
//                        }
//                    }
//                }
//                matchingResult = removeDuplicates(matchingResult);
//                if(matchingResult.size() == 1){
////                    qDebug() << "匹配成功: " << j.value()["番名"][0] << " & " << matchGirigirilove[i.key()][matchingResult[0]]["番名"][0];
//                    AnimeFetchedBangumiData[j.key()]["girigirilove链接"] = {matchingResult[0]};
//                    AnimeFetchedGirigiriloveData[matchingResult[0]]["Bangumi链接"] = {j.key()};
//                    matchingSucess++;
//                }
//                else if(matchingResult.size() == 0){
////                    qDebug() << "无匹配: " << j.value()["番名"][0];
//                    matchingFailEmpty++;
//                }
//                else{
////                    qDebug() << matchingResult;
////                    qDebug() << "多个匹配: " << j.value()["番名"][0];
////                    for(int q = 0 ; q < matchingResult.size() ; ++q)
////                        qDebug() << "结果" << QString::number(q+1) << ": " << matchGirigirilove[i.key()][matchingResult[q]]["番名"][0];
//                    matchingFailTooMore++;
//                }
//            }
//        }
//    }
////    qDebug() << "总体个数: " << totalMatching;
////    qDebug() << "匹配成功: " << matchingSucess;
////    qDebug() << "无匹配: " << matchingFailEmpty;
////    qDebug() << "多个匹配: " << matchingFailTooMore;

//    matchingSucess = 0, matchingFailEmpty = 0, matchingFailTooMore = 0, totalMatching = 0;
//    for(auto i = matchLocal.begin() ; i != matchLocal.end() ; ++i){
//        if(matchBangumi.contains(i.key())){
//            totalMatching += i.value().size();
//            for(auto j = i.value().begin() ; j != i.value().end() ; ++j){
//                QVector<QString> matchingResult;
//                for(auto k = matchBangumi[i.key()].begin() ; k != matchBangumi[i.key()].end() ; ++k){
//                    if(k.value().contains("番名")){
//                        if(vectorMatch(j.value().allAnimeNames, k.value()["番名"]))
//                            matchingResult.push_back(k.key());
////                        else if(vectorMatchBlur(j.value().allAnimeNames, k.value()["番名"]) > IniAnimeNameMatchPrecise){
////                            matchingResult.push_back(k.key());
////                        }
//                    }
//                    if(k.value().contains("别名")){
//                        if(vectorMatch(j.value().allAnimeNames, k.value()["别名"]))
//                            matchingResult.push_back(k.key());
////                        else if(vectorMatchBlur(j.value().allAnimeNames, k.value()["别名"]) > IniAnimeNameMatchPrecise){
////                            matchingResult.push_back(k.key());
////                        }
//                    }
//                }
//                matchingResult = removeDuplicates(matchingResult);
//                if(matchingResult.size() == 1){
////                    qDebug() << "匹配成功: " << j.value()["番名"][0] << " & " << matchBangumi[i.key()][matchingResult[0]]["番名"][0];
//                    if(AnimeMangerMap.contains(j.value().allAnimeNames[0])){
//                        AnimeMangerMap[j.value().allAnimeNames[0]].thisBangumiLink = matchingResult[0];
//                        matchingSucess++;
//                    }

//                }
//                else if(matchingResult.size() == 0){
////                    qDebug() << "无匹配: " << j.value()["番名"][0];
//                    matchingFailEmpty++;
//                }
//                else{
////                    qDebug() << matchingResult;
////                    qDebug() << "多个匹配: " << j.value()["番名"][0];
////                    for(int q = 0 ; q < matchingResult.size() ; ++q)
////                        qDebug() << "结果" << QString::number(q+1) << ": " << matchBangumi[i.key()][matchingResult[q]]["番名"][0];
//                    matchingFailTooMore++;
//                }
//            }
//        }
//    }
//    qDebug() << "总体个数: " << totalMatching;
//    qDebug() << "匹配成功: " << matchingSucess;
//    qDebug() << "无匹配: " << matchingFailEmpty;
//    qDebug() << "多个匹配: " << matchingFailTooMore;

//    int haveLink = 0;
//    for(auto i : AnimeMangerMap){
//        if(i.thisBangumiLink != ""){
//            haveLink++;
//        }
//    }
//    qDebug() << haveLink;

////    matchingSucess = 0, matchingFailEmpty = 0, matchingFailTooMore = 0, totalMatching = 0;

////    totalMatching += AnimeJsonMap.size();
////    for(auto j = AnimeJsonMap.begin() ; j != AnimeJsonMap.end() ; ++j){
////        QVector<QString> matchingResult;
////        for(auto k = AnimeFetchedBangumiData.begin() ; k != AnimeFetchedBangumiData.end() ; ++k){
////            if(j.value().contains("番名")){
////                if(k.value().contains("番名")){
////                    if(vectorMatch(j.value()["番名"], k.value()["番名"]))
////                        matchingResult.push_back(k.key());
////                }
////                if(k.value().contains("别名")){
////                    if(vectorMatch(j.value()["番名"], k.value()["别名"]))
////                        matchingResult.push_back(k.key());
////                }
////            }
////        }
////        matchingResult = removeDuplicates(matchingResult);
////        if(matchingResult.size() == 1){
//////                    qDebug() << "匹配成功: " << j.value()["番名"][0] << " & " << matchBangumi[i.key()][matchingResult[0]]["番名"][0];
////                    AnimeJsonMap[j.value()["番名"][0]]["Bangumi链接"] = {matchingResult[0]};
////            matchingSucess++;
////        }
////        else if(matchingResult.size() == 0){
//////                    qDebug() << "无匹配: " << j.value()["番名"][0];
////            matchingFailEmpty++;
////        }
////        else{
//////                    qDebug() << matchingResult;
//////                    qDebug() << "多个匹配: " << j.value()["番名"][0];
//////                    for(int q = 0 ; q < matchingResult.size() ; ++q)
//////                        qDebug() << "结果" << QString::number(q+1) << ": " << matchBangumi[i.key()][matchingResult[q]]["番名"][0];
////            matchingFailTooMore++;
////        }
////    }

////    qDebug() << "总体个数: " << totalMatching;
////    qDebug() << "匹配成功: " << matchingSucess;
////    qDebug() << "无匹配: " << matchingFailEmpty;
////    qDebug() << "多个匹配: " << matchingFailTooMore;

////    matchingSucess = 0, matchingFailEmpty = 0, matchingFailTooMore = 0, totalMatching = 0;

////    totalMatching += AnimeFetchedGirigiriloveData.size();
////    for(auto j = AnimeFetchedGirigiriloveData.begin() ; j != AnimeFetchedGirigiriloveData.end() ; ++j){
////        QVector<QString> matchingResult;
////        for(auto k = AnimeFetchedBangumiData.begin() ; k != AnimeFetchedBangumiData.end() ; ++k){
////            if(j.value().contains("番名")){
////                if(k.value().contains("番名")){
////                    if(vectorMatch(j.value()["番名"], k.value()["番名"]))
////                        matchingResult.push_back(k.key());
////                    else if(vectorMatchBlur(j.value()["番名"], k.value()["番名"],IniAnimeNameMatchPrecise) > IniAnimeNameMatchPrecise){
////                        matchingResult.push_back(k.key());
////                    }
////                }
////                if(k.value().contains("别名")){
////                    if(vectorMatch(j.value()["番名"], k.value()["别名"]))
////                        matchingResult.push_back(k.key());
////                    else if(vectorMatchBlur(j.value()["番名"], k.value()["别名"],IniAnimeNameMatchPrecise) > IniAnimeNameMatchPrecise){
////                        matchingResult.push_back(k.key());
////                    }
////                }
////            }
////        }
////        matchingResult = removeDuplicates(matchingResult);
////        if(matchingResult.size() == 1){
//////                    qDebug() << "匹配成功: " << j.value()["番名"][0] << " & " << matchBangumi[i.key()][matchingResult[0]]["番名"][0];
//////                    AnimeJsonMap[j.value()["番名"][0]]["Bangumi链接"] = {matchingResult[0]};
////            matchingSucess++;
////        }
////        else if(matchingResult.size() == 0){
//////                    qDebug() << "无匹配: " << j.value()["番名"][0];
////            matchingFailEmpty++;
////        }
////        else{
//////                    qDebug() << matchingResult;
//////                    qDebug() << "多个匹配: " << j.value()["番名"][0];
//////                    for(int q = 0 ; q < matchingResult.size() ; ++q)
//////                        qDebug() << "结果" << QString::number(q+1) << ": " << matchBangumi[i.key()][matchingResult[q]]["番名"][0];
////            matchingFailTooMore++;
////        }
////    }

////    qDebug() << "总体个数: " << totalMatching;
////    qDebug() << "匹配成功: " << matchingSucess;
////    qDebug() << "无匹配: " << matchingFailEmpty;
////    qDebug() << "多个匹配: " << matchingFailTooMore;



////    QHash<QString, QVector<QString>> matchingResult;
////    for(auto j = AnimeJsonMap.begin() ; j != AnimeJsonMap.end() ; ++j){
////        if(j.value().contains("Bangumi链接")){
////            matchingResult[getHash(j.value(), "Bangumi链接", 0)].push_back(getHash(j.value(), "番名", 0));
////        }
////    }
////    qDebug() << matchingResult;
////    matchingSucess = 0, matchingFailEmpty = 0, matchingFailTooMore = 0, totalMatching = 0;
////    for(auto i : matchingResult){
////        if(i.size() == 1)
////            matchingSucess++;
////        if(i.size() > 1){
////            matchingFailTooMore += i.size();
////            qDebug() << i;
////            for(auto j : i){
////                if(!getHash(AnimeJsonMap, j, "Bangumi链接", 0).isEmpty()){
////                    AnimeJsonMap[j].remove("Bangumi链接");
////                }
////            }

////        }
////        totalMatching += i.size();
////    }

////    qDebug() << "总体个数: " << totalMatching;
////    qDebug() << "匹配成功: " << matchingSucess;
////    qDebug() << "无匹配: " << matchingFailEmpty;
////    qDebug() << "多个匹配: " << matchingFailTooMore;

}


//  设置剪贴板 用于检索番剧
void AnimeNestWindow::SetCopyName(bool HasEpisode){
    QClipboard *clipboard = QApplication::clipboard();
    QString CopyName = ui->AnimeName->toPlainText();
    if(HasEpisode){
        int startEpisode = AnimeMangerMap[CopyName].currentEpisode + AnimeMangerMap[CopyName].extraDownloadEpisodes + 1;
        QString setStartEpisode = "(" + QString::number(startEpisode).rightJustified(2, '0');

        QString defaultPrefix = "" , defaultsuffix = "";
        if(Setting["下载设置"].contains("下载默认前缀"))
            defaultPrefix = Setting["下载设置"]["下载默认前缀"][0];
        if(Setting["下载设置"].contains("下载默认后缀"))
            defaultsuffix = Setting["下载设置"]["下载默认后缀"][0];
        CopyName = defaultPrefix + " " + AnimeMangerMap[CopyName].downloadName + " " + setStartEpisode
                   + " " + defaultsuffix;
    }
    clipboard->setText(CopyName);
}
