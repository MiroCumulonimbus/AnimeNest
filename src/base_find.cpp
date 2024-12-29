#include "animenestwindow.h"
#include "ui_animenestwindow.h"

#define TableWidget_Insert_Start(tableID,clearTempAnimeTable) \
    IniTable(tableID); \
    ui->tableWidget->setSortingEnabled(false); \
    ui->tableWidget->clearContents(); \
    ui->tableWidget->setRowCount(0);\
    if(clearTempAnimeTable)\
        tempAnimeTable.clear();

#define TableWidget_Insert_End(sortRow) \
    int countItem = 0;\
    for(auto i : tempAnimeTable)\
        InsertItem(i , countItem);\
    ui->tableWidget->setSortingEnabled(true);\
    ui->tableWidget->sortItems(sortRow , Qt::DescendingOrder);\
    ui->AnimeCount->setText(QString::number(countItem));


// 按名称查找
void AnimeNestWindow::GetFindAnime(QString Name)
{
    if(Name.contains(" ")){
        Name = Name.split(" ")[0];
    }

    if(ui->FindInLocal->isChecked()){
        TableWidget_Insert_Start(1,true)

        for(auto i : AnimeMangerMap){
            for(auto j : i.allAnimeNames){
                if(j.contains(Name, Qt::CaseInsensitive)){
                    tempAnimeTable.push_back(tableDataHandle(i));
                    break;
                }
            }
        }
        if(tempAnimeTable.empty())
            messageBoxWindow("未找到: " + Name);

        TableWidget_Insert_End(4);
    }
    else if(ui->FindInBangumi->isChecked()){
        TableWidget_Insert_Start(2,true)

        for(auto i = RankData["Bangumi"].begin(); i!= RankData["Bangumi"].end() ; ++i){
            QVector<QString> Names = getHash(i.value(),"番名");
            Names.append(getHash(i.value(),"原名"));
            Names.append(getHash(i.value(),"别名"));

            for(auto j : Names){
                if(j.contains(Name, Qt::CaseInsensitive)){
                    tempAnimeTable.push_back(tableDataHandle(i.value(), i.key()));
                    break;
                }
            }
        }
        int sup = 0;
        for(auto i : tempAnimeTable){
            if(i.insertData[4] != "/")
                sup++;
        }
        qDebug() << "支持观看的个数: " << sup;
        if(tempAnimeTable.empty())
            messageBoxWindow("未找到: " + Name);

        TableWidget_Insert_End(3);
    }
}

// 按标签查找
void AnimeNestWindow::GetLabelAnime(QString Label, int LabelCount) {
    // 解析用户输入的标签
    QStringList LabelList = Label.split("、");
    for (QString &s : LabelList) {
        int splitIndex = s.indexOf("/");
        s = s.left(splitIndex); // 保留原始大小写
    }

    if(ui->FindInLocal->isChecked()){
        TableWidget_Insert_Start(1,true);
        bool needGiveup = ui->DiscardAnime->isChecked();

        // 遍历所有动画的标签，筛选符合条件的动画
        for (auto i : AnimeMangerMap) {
            QVector<QString> LocalLabelList;
            for(auto j : i.labels)
                LocalLabelList.push_back(j.first);

            int LocalHitLabel = 0;
            // 检查每个标签是否匹配
            for (const QString &userLabel : LabelList) {
                if (std::any_of(LocalLabelList.begin(), LocalLabelList.end(), [&](const QString &localLabel) {
                        return localLabel.contains(userLabel, Qt::CaseInsensitive); // 使用大小写无关比较
                    })) {
                    // 如果匹配且符合放弃条件，计数
                    if (needGiveup || !i.isAbandoned) {
                        ++LocalHitLabel;
                    }
                }
            }

            // 如果命中标签数达到要求，加入结果表
            if (LocalHitLabel >= LabelCount) {
                tempAnimeTable.push_back(tableDataHandle(i));
            }
        }

        TableWidget_Insert_End(4);
    }
    else if(ui->FindInBangumi->isChecked()){
        TableWidget_Insert_Start(2,true);
        // 遍历所有动画的标签，筛选符合条件的动画
        for(auto i = RankData["Bangumi"].begin(); i!= RankData["Bangumi"].end() ; ++i){
            QVector<QString> labelsBangumi = getHash(i.value(),"标签");
            int LocalHitLabel = 0;
            // 检查每个标签是否匹配
            for (const QString &userLabel : LabelList) {
                if (std::any_of(labelsBangumi.begin(), labelsBangumi.end(), [&](const QString &localLabel) {
                        return localLabel.contains(userLabel, Qt::CaseInsensitive); // 使用大小写无关比较
                    })) {
                        ++LocalHitLabel;
                }
            }
            // 如果命中标签数达到要求，加入结果表
            if (LocalHitLabel >= LabelCount) {
                tempAnimeTable.push_back(tableDataHandle(i.value(), i.key()));
            }
        }
        if(tempAnimeTable.empty())
            messageBoxWindow("未找到满足 " + QString::number(LabelCount) + " 个标签的匹配");

        TableWidget_Insert_End(3);
    }
}

//  全部番剧
void AnimeNestWindow::GetAllAnime(){
    TableWidget_Insert_Start(1,true);
    bool needGiveup = ui->DiscardAnime->isChecked();
    int DiscardInterval = ui->DiscardAnimeInterval->text().toInt();
    for(auto i : AnimeMangerMap){
        if(!i.isMovie && ((needGiveup && i.episodesGivenUp >= DiscardInterval) || !i.isAbandoned))
            tempAnimeTable.push_back(tableDataHandle(i));
    }

    TableWidget_Insert_End(4);
    return ;
}

//  动漫电影
void AnimeNestWindow::GetAnimeMovie(){
    TableWidget_Insert_Start(1,true);
    for(auto i : AnimeMangerMap){
        if(i.isMovie)
            tempAnimeTable.push_back(tableDataHandle(i));
    }

    TableWidget_Insert_End(4);
    return ;
}


//  已放弃
void AnimeNestWindow::GetDiscardAnime(){
    if(ui->AllAnime->isChecked()){
        GetAllAnime();
    }
    if(ui->LocalSeasonAnime->isChecked()){
        ui->LocalSeason->setText(QDate::fromString(LocalSeason , "yyyy年M月").addMonths(ui->LocalSeasonAnimeInterval->text().toInt()*3).toString("yyyy年M月"));
        GetLocalSeasonAnime();
    }
    if(ui->WatchingLastAnime->isChecked()){
        GetWatchingLastAnime();
    }
    return ;
}

//  已放弃
void AnimeNestWindow::GetDiscardAnime2(){
    TableWidget_Insert_Start(1,true);

    for(auto i : AnimeMangerMap){
        int DiscardInterval = ui->DiscardAnime2Interval->text().toInt();
        if(DiscardInterval >= 0)
        {
            if(i.isAbandoned && i.episodesGivenUp >= DiscardInterval)
                tempAnimeTable.push_back(tableDataHandle(i));
        }
        else
        {
            if(i.isAbandoned && i.episodesGivenUp < -DiscardInterval)
                tempAnimeTable.push_back(tableDataHandle(i));
        }
    }

    TableWidget_Insert_End(4);
}


//  当季追番
void AnimeNestWindow::GetLocalSeasonAnime(){
    TableWidget_Insert_Start(1,true);

    ui->LocalSeason->setText(QDate::fromString(LocalSeason, "yyyy年M月")
        .addMonths(ui->LocalSeasonAnimeInterval->text().toInt() * 3)
        .toString("yyyy年M月"));

    bool needGiveup = ui->DiscardAnime->isChecked();
    int DiscardInterval = ui->DiscardAnimeInterval->text().toInt();
    for(auto i : AnimeMangerMap){
        for(auto j : i.seasons){
            if(j == ui->LocalSeason->text() && ((needGiveup && i.episodesGivenUp >= DiscardInterval) || !i.isAbandoned)){
                tempAnimeTable.push_back(tableDataHandle(i));
                break;
            }
        }
    }

    TableWidget_Insert_End(4);
    return ;
}

//  最近更新
void AnimeNestWindow::GetRecentUpdate(){
    TableWidget_Insert_Start(1,true);

    int day = ui->RecentUpdateInterval->text().toInt();
    for(auto i : AnimeMangerMap){
        int days = -10;
        if(i.updateStartDate.isValid())
            days= daysBetween(Today.date().addDays(day), i.updateStartDate);
        bool NotStartType = false;
        if (i.updateStartDate.isValid() && days <= 0 && days >= -1 && i.currentEpisode == 0)
            NotStartType = true;
        if(!i.hasWatched &&  ((i.watchInterval< 9 - (day==0?0:day+1) && i.watchInterval >= 7 - day) || NotStartType)
                && !i.isAbandoned && i.isSeasonAnime && i.canWatchNow)
            tempAnimeTable.push_back(tableDataHandle(i));
    }

    TableWidget_Insert_End(4);
    return ;
}

void AnimeNestWindow::GetRecentUpdate(const QHash<QString, QHash<QString, QVector<QString>>> &updateInfo) {
    TableWidget_Insert_Start(1,true);

    // 过滤有效更新数据
    QHash<QString, int> validUpdates;
    for (auto it = updateInfo.begin(); it != updateInfo.end(); ++it) {
        const QVector<QString> &progress = it.value()["更新进度"];
        if (!progress.isEmpty()) {
            validUpdates[it.key()] = progress[0].toInt();
        }
    }

    // 检查每个动画是否有更新
    for (const auto &anime : AnimeMangerMap) {
        for (const QString &name : anime.allAnimeNames) {
            if (validUpdates.contains(name)) {
                if (anime.currentEpisode < validUpdates[name]) {
                    tempAnimeTable.push_back(tableDataHandle(anime));
                    break; // 当前动画找到更新，跳过后续名字检查
                }
            }
        }
    }

    TableWidget_Insert_End(4);
}

//  搁置番剧
void AnimeNestWindow::GetShelvedAnime(){
    TableWidget_Insert_Start(1,true);
    bool needGiveup = ui->DiscardAnime->isChecked();
    for(auto i : AnimeMangerMap){
        if(!i.hasWatched && (i.watchInterval >= 9 || (i.displayContent == " ---- " && i.watchInterval >= 2 ))
                && i.isCurrentSeason && (needGiveup || !i.isAbandoned) && i.canWatchNow)
            tempAnimeTable.push_back(tableDataHandle(i));
    }

    TableWidget_Insert_End(4);
    return ;
}

//  已追完
void AnimeNestWindow::GetWatchedAnime(){
    TableWidget_Insert_Start(1,true);

    int watchedCredibilityLevel = ui->WatchedAnimeCredibilityLevel->text().toInt();
    for(auto i : AnimeMangerMap){
        int watchedRecord = i.addedEpisodeScores.size() - 1;
        if(i.hasWatched && !i.isMovie && watchedRecord >= watchedCredibilityLevel)
            tempAnimeTable.push_back(tableDataHandle(i));
    }

    TableWidget_Insert_End(4);
    return ;
}


//  正在观看
void AnimeNestWindow::GetWatchingAnime(){
    TableWidget_Insert_Start(1,true);
    bool needGiveup = ui->DiscardAnime->isChecked();

    for(auto i : AnimeMangerMap){
        int intervalCheck = ui->WatchingAnimeInterval->text().toInt();
        if(intervalCheck < 0)
            intervalCheck = (-intervalCheck)*50;
        bool seasonsCheck = true;
        for(auto j : i.seasons){
            if(j >= LocalSeason)
                seasonsCheck = false;
        }
        if(!i.hasWatched && seasonsCheck && (needGiveup || !i.isAbandoned) && !i.isCatchingUpAnime
                && i.canWatchNow && (intervalCheck == 0 ? true : i.watchInterval <= intervalCheck))
        {
            tempAnimeTable.push_back(tableDataHandle(i));
        }
    }

    TableWidget_Insert_End(4);
    return ;
}

//  补番目录
void AnimeNestWindow::GetWatchingLastAnime(){
    TableWidget_Insert_Start(1,true);

    for(auto i : AnimeMangerMap){

        if((i.currentEpisode == 0 && !i.isMovie && !i.isAbandoned &&
                !i.isCurrentSeason && !i.hasWatched &&
                i.watchInterval >= 0) || i.isCatchingUpAnime)
        {
            tempAnimeTable.push_back(tableDataHandle(i));
        }
    }
    TableWidget_Insert_End(4);
    return ;
}


//void AnimeNestWindow::LoadAnimeBaseFunction(const QString &key, const QHash<QString, QVector<QString>> &value, int &count){
//    int RowCont = ui->tableWidget->rowCount();
//    ui->tableWidget->insertRow(RowCont);//增加一行
//    QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(value["番名"][0]);
//    ui->tableWidget->setItem(RowCont,0,WidgetAnime0);
//    QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem(key);
//    ui->tableWidget->setItem(RowCont,1,WidgetAnime1);
//    double BangumiScore = 0.0;
//    int BangumiVotes = 0, BangumiRank = 0;
//    if(value.contains("Bangumi评分") && value["Bangumi评分"].size() > 0){
//        QTableWidgetItem  *WidgetAnime2 = new QTableWidgetItem();
//        BangumiScore = QString::number(value["Bangumi评分"].back().toDouble(),'f',1).toDouble();
//        WidgetAnime2->setData(Qt::DisplayRole,BangumiScore);
//        ui->tableWidget->setItem(RowCont,2,WidgetAnime2);
//    }
//    if(value.contains("投票人数") && value["投票人数"].size() > 0){
//        BangumiVotes = value["投票人数"].back().toInt();
//    }
//    if(value.contains("排名") && value["排名"].size() > 0){
//        BangumiRank = value["排名"].back().toInt();
//    }

//    // 归一化排名 (1 - 10000) 到 (0 - 10)
//    double normalizedRank = (10000 - BangumiRank) / 1000.0;
//    if (normalizedRank > 10.0) normalizedRank = 10.0;

//    // 对投票数进行对数变换 (1 - 10000) -> (0.4 - 1.2)
//    double logVote = log10(BangumiVotes);
//    if (logVote > log10(20000)) logVote = log10(20000);  // 限制最大值

//    // 投票数归一化 (0.4 - 1.2)
//    double normalizedVote = 0.75 * (logVote / log10(20000.0)) + 0.3;

//    // 综合推荐度计算
//    double recommendation = normalizedVote * (0.2 * BangumiScore + 0.45 * normalizedRank) + 0.35 * BangumiScore;

//    // 确保最终推荐度在 0.0 到 10.0 之间
//    if (recommendation > 10.0) recommendation = 10.0;
//    if (recommendation < 0.0) recommendation = 0.0;
//    recommendation = QString::number(recommendation,'f',2).toDouble();
//    QTableWidgetItem  *WidgetAnime3 = new QTableWidgetItem();
//    WidgetAnime3->setData(Qt::DisplayRole,recommendation);
//    ui->tableWidget->setItem(RowCont,3,WidgetAnime3);

//    QString supportOnline = "/";
//    if(value.contains("girigirilove链接") && value["girigirilove链接"].size() > 0 && AnimeFetchedGirigiriloveData.contains(value["girigirilove链接"][0])){
//        supportOnline = "支持";
//    }
//    QTableWidgetItem  *WidgetAnime4 = new QTableWidgetItem(supportOnline);
//    ui->tableWidget->setItem(RowCont,4,WidgetAnime4);

//    QString isFollowing = "/";

//    for(auto k : AnimeMangerMap){
//        if(k.thisBangumiLink != "" && k.thisBangumiLink == key){
//            isFollowing = "已追番";
//            if(k.isAbandoned)
//                isFollowing += " / 弃";
//        }
//    }
//    QTableWidgetItem  *WidgetAnime5 = new QTableWidgetItem(isFollowing);
//    ui->tableWidget->setItem(RowCont,5,WidgetAnime5);

//    ++count;
//}

void AnimeNestWindow::LoadAnimeWithProducer(QString Name, int matchType){

    TableWidget_Insert_Start(2,true)

    for(auto i = RankData["Bangumi"].begin() ; i != RankData["Bangumi"].end() ; ++i){
        if(!getHash(i.value(),"番名",0).isEmpty() || !getHash(i.value(),"原名",0).isEmpty()){
            bool isMatch = false;
            if(matchType == 1){
                QString AnimeProducer = getHash(i.value(), "动画制作", 0);
                if(AnimeProducer == Name)
                    isMatch = true;
            }
            if(matchType == 2){
                QVector<QString> Supervisor;
                Supervisor.append(getHash(i.value(), "导演"));
                Supervisor.append(getHash(i.value(), "总导演"));
                if(Supervisor.contains(Name))
                    isMatch = true;
            }
            if(isMatch){
                tempAnimeTable.push_back(tableDataHandle(i.value(),i.key()));
            }
        }
    }
    TableWidget_Insert_End(3);
}

//  载入番剧
void AnimeNestWindow::LoadAnimeWithLabel(QVector<QString> targetLabel, int votesLimit, int totalEpisodeLimit){

    TableWidget_Insert_Start(2,true)

    bool needDateCheck = false;
    if(targetLabel.size() == 1 && targetLabel[0].contains("年") && targetLabel[0].contains("月") && QDateTime::fromString(targetLabel[0], "yyyy年M月") > Today){
        votesLimit = 0;
        totalEpisodeLimit = 0;
        needDateCheck = true;
    }

    for(auto i = RankData["Bangumi"].begin() ; i != RankData["Bangumi"].end() ; ++i){
        if(!getHash(i.value(),"番名",0).isEmpty() || !getHash(i.value(),"原名",0).isEmpty()){
            bool matchByAnimeWithLabel = false;
            QHash<QString,int> labelsHash;
            QVector<QString> labelsByAnimeWithLabel;
            if(i.value().contains("标签")){
                labelsByAnimeWithLabel = i.value()["标签"];
                if(i.value().contains("标签投票数") && i.value()["标签"].size() == i.value()["标签投票数"].size()){
                    for(int j = 0 ; j < i.value()["标签"].size() ; ++j){
                        labelsHash.insert(i.value()["标签"][j], i.value()["标签投票数"][j].toInt());
                    }
                }
            }
            if(vectorMatch(labelsByAnimeWithLabel, targetLabel))
                matchByAnimeWithLabel = true;

            if(needDateCheck){
                if(!getHash(i.value(), "首播").isEmpty()){
                    QDate tempDate = QDate::fromString(getHash(i.value(), "首播", 0), "yyyy年M月D日");
                    QDate checkDateStart = QDate::fromString(targetLabel[0], "yyyy年M月");
                    QDate checkDateEnd = checkDateStart.addMonths(3);
                    if(tempDate > checkDateStart && tempDate < checkDateEnd)
                        matchByAnimeWithLabel = true;
                }
            }

            int totalEpisode = getHash(i.value(), "总集数", 0).isEmpty()? 0 : getHash(i.value(), "总集数", 0).toInt();
            int votes = getHash(i.value(), "投票人数", 0).isEmpty()? 0 : getHash(i.value(), "投票人数", 0).toInt();

            if(totalEpisode < totalEpisodeLimit){
                matchByAnimeWithLabel = false;
            }
            if(votes < votesLimit){
                matchByAnimeWithLabel = false;
            }
            if(matchByAnimeWithLabel){
                tempAnimeTable.push_back(tableDataHandle(i.value(),i.key()));
            }
        }
    }

    TableWidget_Insert_End(3);
}

//  载入番剧
void AnimeNestWindow::LoadAnimeWithSeason(QString targetLabel, int votesLimit, int totalEpisodeLimit){

    TableWidget_Insert_Start(2,true)

    bool needDateCheck = false;
    if(QDateTime::fromString(targetLabel, "yyyy年M月") > Today){
        votesLimit = 0;
        totalEpisodeLimit = 0;
        needDateCheck = true;
    }

    for(auto i = RankData["Bangumi"].begin() ; i != RankData["Bangumi"].end() ; ++i){
        if(!getHash(i.value(),"番名",0).isEmpty() || !getHash(i.value(),"原名",0).isEmpty()){
            bool matchByAnimeWithLabel = false;

            if(getHash(i.value(), "更新年月").contains(targetLabel))
                matchByAnimeWithLabel = true;

            int totalEpisode = getHash(i.value(), "总集数", 0).isEmpty()? 0 : getHash(i.value(), "总集数", 0).toInt();
            int votes = getHash(i.value(), "投票人数", 0).isEmpty()? 0 : getHash(i.value(), "投票人数", 0).toInt();

            if(totalEpisode < totalEpisodeLimit){
                matchByAnimeWithLabel = false;
            }
            if(votes < votesLimit){
                matchByAnimeWithLabel = false;
            }

            if(matchByAnimeWithLabel){
                tempAnimeTable.push_back(tableDataHandle(i.value(),i.key()));
            }
        }
    }

    TableWidget_Insert_End(3);
}

void AnimeNestWindow::LoadAnimeWithName(QString Name, int votesLimit, int totalEpisodeLimit){
    TableWidget_Insert_Start(2,true)

    for(auto i = RankData["Bangumi"].begin() ; i != RankData["Bangumi"].end() ; ++i){
        if(!getHash(i.value(),"番名",0).isEmpty() || !getHash(i.value(),"原名",0).isEmpty()){
            bool matchLabel = false;
            QVector<QString> names;
            names.append(getHash(i.value(),"番名"));
            names.append(getHash(i.value(),"别名"));
            names.append(getHash(i.value(),"原名"));
            names = removeDuplicates(names);
            for(auto j : names){
                if(j.contains(Name)){
                    matchLabel = true;
                    break;
                }
            }

            int totalEpisode = getHash(i.value(), "总集数", 0).isEmpty()? 0 : getHash(i.value(), "总集数", 0).toInt();
            int votes = getHash(i.value(), "投票人数", 0).isEmpty()? 0 : getHash(i.value(), "投票人数", 0).toInt();

            if(totalEpisode < totalEpisodeLimit){
                matchLabel = false;
            }
            if(votes < votesLimit){
                matchLabel = false;
            }

            if(matchLabel){
                tempAnimeTable.push_back(tableDataHandle(i.value(),i.key()));
            }
        }
    }


    TableWidget_Insert_End(3);
}

bool AnimeNestWindow::OpenWebUrl(QString animeName, int targetEpisode) {
    bool isWebUrlOpened = false;
    int extraIndex = 0;
    QString webUrl;
    if (AnimeMangerMap.contains(animeName)) {
        extraIndex = AnimeMangerMap[animeName].extraOnlineEpisodes;
    }
    if (AnimeMangerMap.contains(animeName) && !AnimeMangerMap[animeName].onlineUrl.isEmpty()) {
        webUrl = AnimeMangerMap[animeName].onlineUrl;
        qDebug() << "MangerMap";
    }
    else{
        for(auto i : tempAnimeTable){
            if(i.Name == animeName){
                for(auto j : OpenOrder){
                    if(getHash(Setting, "打开方式", j , 0) == "1"){
                        QString marchLink = getHash(fastLinkBangumiToAll,i.BangumiLink,j,0);
                        qDebug() << getHash(fastLinkBangumiToAll,i.BangumiLink);
                        webUrl = getHash(OnlineStreamData,j,marchLink,QString(j+"路径"),0);
                        if(!webUrl.isEmpty()){
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    qDebug() << webUrl;
    if(!webUrl.isEmpty()){
        if(webUrl.contains(specialPlaceholder))
            webUrl.replace(specialPlaceholder, QString::number(targetEpisode + extraIndex));
        QDesktopServices::openUrl(QUrl(webUrl));
        isWebUrlOpened = true;
    }
    return isWebUrlOpened;
}

bool AnimeNestWindow::OpenLocalFile(QString animeName, int targetEpisode) {
    bool isLocalFileOpened = false;
    if(AnimeMangerMap.contains(animeName)){
        if (!AnimeMangerMap[animeName].localFilePath.isEmpty()) {
            // 获取目标路径下的文件列表
            QFileInfoList localFiles = getFiles(AnimeMangerMap[animeName].localFilePath, 1);
    //        qDebug() << AnimeMangerMap[animeName].localFilePath;
    //        qDebug() << localFiles;

            // 正则表达式匹配集数
            QRegularExpression episodeRegex("(?<=[\\[\\s第E#])(\\d+?)(?=[\\]\\s集话話v.])");

            QFileInfo preferredFile; // 优先级最高的文件
            QFileInfo fallbackFile;  // 非优先的文件

            for (const QFileInfo& fileInfo : localFiles) {
                QString fileName = fileInfo.fileName();
                QRegularExpressionMatch regexMatch = episodeRegex.match(fileName);
    //            qDebug() << fileInfo.absoluteFilePath();

                if (regexMatch.hasMatch()) {
                    int fileEpisode = regexMatch.captured(1).toInt();
                    if (fileEpisode == targetEpisode) {
                        if (fileName.contains(animeName, Qt::CaseInsensitive)) {
                            // 如果文件名包含 animeName，则优先选择
                            preferredFile = fileInfo;
                            break; // 找到优先文件，立即退出循环
                        } else if (fallbackFile.fileName().isEmpty()) {
                            // 记录第一个匹配的非优先文件
                            fallbackFile = fileInfo;
                        }
                    }
                }
            }

            // 尝试打开文件
            if (preferredFile.exists()) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(preferredFile.absoluteFilePath()));
                isLocalFileOpened = true;
            } else if (fallbackFile.exists()) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(fallbackFile.absoluteFilePath()));
                isLocalFileOpened = true;
            }
            else if(localFiles.size() == 1 && targetEpisode == 1){
                QDesktopServices::openUrl(QUrl::fromLocalFile(localFiles.front().absoluteFilePath()));
                isLocalFileOpened = true;
            }
        }
    }

    return isLocalFileOpened;
}

void AnimeNestWindow::CreateNewLocalUrl(QString Name){
    QProcess *proc = new QProcess(this);
    if(AnimeMangerMap[Name].hasLocalFile){
        proc->start(AnimeMangerMap[Name].localFilePath);
        QDir().mkdir(AnimeMangerMap[Name].localFilePath);
        proc->startDetached("explorer", QStringList() << QString("/root,") << QString("%1").arg(AnimeMangerMap[Name].localFilePath));
        return ;
    }
    QMessageBox::StandardButton reply;
    QString message = "创建新目录？";
    reply = QMessageBox::question(nullptr, "Message", message , QMessageBox::Yes | QMessageBox::No);
    QString NameUrl = "";

    // 根据用户的响应作出相应的操作
    if (reply == QMessageBox::Yes) {
        if(Setting["系统路径"].contains("本地收藏路径")){
            NameUrl = Setting["系统路径"]["本地收藏路径"][0] + "/" + Name;
            proc->start(NameUrl);
            NameUrl.replace("/", "\\");    //win32下替换斜杠
            QDir().mkdir(NameUrl);
            proc->startDetached("explorer", QStringList() << QString("/root,") << QString("%1").arg(NameUrl));
            AnimeMangerMap[Name].hasLocalFile = true;
            AnimeMangerMap[Name].localFilePath = NameUrl.replace("/", "\\");
        }
        else
            messageBoxWindow("未设置：本地收藏路径");
    }
    delete proc;
}
