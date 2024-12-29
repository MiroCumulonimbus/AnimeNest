#include "animedatamanger.h"

AnimeDataManger::AnimeDataManger()
{

}

void AnimeDataManger::IniData(QHash<QString,QVector<QString>> &AnimeData){
    sumScoreF = 0;              // 保留固定小数的总评分
    sumScore = 0;                // 总评分
    storyScore = 0;              // 剧情评分
    screenScore = 0;             // 画面评分
    roleScore = 0;               // 人设评分

    viewStartDate = Today.date();     // 开始观看日期
    viewLastDate = QDate();      // 最后观看日期
    updateStartDate = QDate();   // 开始更新日期
    completionDate = QDate();    // 完成日期

    seasons.clear();              // 清空季节列表
    displayContent = " --- ";     // 用于表格显示的内容（时间或集数）

    downloadName = "";            // 下载使用的名称
    onlineUrl = "";               // 在线观看网址
//    localFilePath = "";           // 本地文件路径
//    floderSize = 0.0;             // 占用内存
//    thisBangumiLink = "";         // BangumiLink

    isAbandoned = false;          // 是否放弃观看
    isMovie = false;              // 是否为动画电影
    hasWatched = false;           // 是否已观看
    isCurrentSeason = false;      // 是否为当季新番
    isCurrentlyWatching = false;  // 是否正在观看
    canWatchNow = true;           // 当前是否可以观看
    isSeasonAnime = false;        // 是否为季番
    isCatchingUpAnime = false;    // 是否为在补番剧
//    hasLocalFile = false;         // 是否有本地文件

    episodesGivenUp = 0;          // 放弃观看的集数

    watchInterval = 0;            // 追番间隔时间
    totalWatchTime = 0;           // 追番总时间
    currentEpisode = 0;           // 当前追番集数
//    totalEpisodes = 0;            // 动漫总集数
    startEpisode = 100;           // 开始观看的集数
    updateDay = -1;               // 更新的星期几（1-7，分别代表星期一到星期日）

    extraOnlineEpisodes = 0;      // 在线搜索时额外添加的集数
    extraDownloadEpisodes = 0;    // 下载时额外添加的集数

    interpolatedEpisodeScores.clear(); // 清空经过插值处理的每集得分数据
    addedEpisodeScores.clear();        // 清空已添加的单集得分数据
    allCompleteScores.clear();         // 清空已添加的完结得分数据
    allAnimeNames.clear();             // 清空所有动漫名称列表，以便重新添加
    labels.clear();                    // 清空标签
//  初始化结束 去重并清除空名字
    allAnimeNames.append(AnimeData["番名"]);
    allAnimeNames = removeDuplicates(allAnimeNames);
    AnimeData["番名"] = allAnimeNames;

//    qDebug() << allAnimeNames.size();
    for(auto i = AnimeData.begin() ; i != AnimeData.end() ; ++i){
        if(i.key().toInt() > 0){
            currentEpisode = qMax(currentEpisode , i.key().toInt());
            startEpisode = qMin(startEpisode , i.key().toInt());
            sAnimeData temp;
            temp.Story = i.value()[0].toDouble();
            temp.Screen = i.value()[1].toDouble();
            temp.Role = i.value()[2].toDouble();
            temp.Time = QDate::fromString(i.value()[3], "yyyy.MM.dd");
            viewStartDate = std::min(viewStartDate, temp.Time);
            viewLastDate = std::max(viewLastDate, temp.Time);
            addedEpisodeScores.insert(i.key(), temp);
        }
    }

    if(currentEpisode > 0){
        interpolatedEpisodeScores.resize(currentEpisode);
        for(auto i = addedEpisodeScores.begin() ; i != addedEpisodeScores.end() ; ++i){
            interpolatedEpisodeScores[i.key().toInt() - 1] = i.value();
        }

        // 进行插值
        for (int idx = 0; idx < currentEpisode; ++idx) {
            if (!addedEpisodeScores.contains(QString::number(idx + 1))) {
                int leftIdx = idx - 1;
                int rightIdx = idx + 1;

                while (leftIdx >= 0 && !addedEpisodeScores.contains(QString::number(leftIdx + 1))) {
                    --leftIdx;
                }
                while (rightIdx < interpolatedEpisodeScores.size() &&
                       !addedEpisodeScores.contains(QString::number(rightIdx + 1))) {
                    ++rightIdx;
                }

                if (leftIdx >= -1 && rightIdx < interpolatedEpisodeScores.size()) {
                    QString leftString = QString::number(leftIdx + 1);
                    QString rightString = QString::number(rightIdx + 1);
                    if(leftIdx == -1)
                        leftString = rightString;

                    double totalDistance = rightIdx - leftIdx;
                    double distanceToLeft = idx - leftIdx;

                    interpolatedEpisodeScores[idx].Story =
                        addedEpisodeScores[leftString].Story +
                        (addedEpisodeScores[rightString].Story - addedEpisodeScores[leftString].Story) * (distanceToLeft / totalDistance);
                    interpolatedEpisodeScores[idx].Screen =
                        addedEpisodeScores[leftString].Screen +
                        (addedEpisodeScores[rightString].Screen - addedEpisodeScores[leftString].Screen) * (distanceToLeft / totalDistance);
                    interpolatedEpisodeScores[idx].Role =
                        addedEpisodeScores[leftString].Role +
                        (addedEpisodeScores[rightString].Role - addedEpisodeScores[leftString].Role) * (distanceToLeft / totalDistance);
                    interpolatedEpisodeScores[idx].Time =
                        addedEpisodeScores[rightString].Time;
                }
            }
        }
    }
//    else{
//        setLogsInfo("索引集数时出错：" + (allAnimeNames.empty() ? "未知动漫" : allAnimeNames[0]));
//    }

    if(AnimeData.contains("0")){
        sAnimeData temp;
        temp.Story = AnimeData["0"][0].toDouble();
        temp.Screen = AnimeData["0"][1].toDouble();
        temp.Role = AnimeData["0"][2].toDouble();
        temp.Time = QDate::fromString(AnimeData["0"][3], "yyyy.MM.dd");
        addedEpisodeScores.insert("预期", temp);
        if(viewStartDate == Today.date())
            viewStartDate = std::min(viewStartDate, temp.Time);
        viewLastDate = std::max(viewLastDate, temp.Time);
    }

    if(AnimeData.contains("完结得分")){
        allCompleteScores = AnimeData["完结得分"];
        hasWatched = true;
        sAnimeData temp;
        QVector<double> story, screen, role;
        for(int i = 0 ; i < AnimeData["完结得分"].size() ; i+=4)
        {
            QString scores[] = { AnimeData["完结得分"][i], AnimeData["完结得分"][i + 1], AnimeData["完结得分"][i + 2] };
            if (scores[0] != "/") story.push_back(scores[0].toDouble());
            if (scores[1] != "/") screen.push_back(scores[1].toDouble());
            if (scores[2] != "/") role.push_back(scores[2].toDouble());

            temp.Time = QDate::fromString(AnimeData["完结得分"][i+3], "yyyy.MM.dd");
            viewStartDate = std::min(viewStartDate, temp.Time);
            viewLastDate = std::max(viewLastDate, temp.Time);
            completionDate = std::min(completionDate, temp.Time);
        }
        auto calculateAverage = [](const QVector<double>& vec) {
            return vec.isEmpty() ? 0.0 : std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
        };

        temp.Story = calculateAverage(story);
        temp.Screen = calculateAverage(screen);
        temp.Role = calculateAverage(role);

        addedEpisodeScores.insert("完结得分", temp);
    }

    isCurrentlyWatching = !hasWatched;

    if(AnimeData.contains("集数") && AnimeData["集数"].size() > 0){
        totalEpisodes = AnimeData["集数"].back().toInt();
    }

    if(AnimeData.contains("首播") && AnimeData["首播"].size() > 0){
        QString handleStartTime = AnimeData["首播"][0];
        QDate parsedDate = QDate::fromString(handleStartTime, "yyyy年M月d日");
        if(parsedDate.isValid()){
            updateStartDate = parsedDate;
        }
        else{
            parsedDate = QDate::fromString(AnimeData["首播"].back(), "M.d");
            if(parsedDate.isValid()){
                updateStartDate = parsedDate;
                updateStartDate.setDate(Today.date().year(), updateStartDate.month(), updateStartDate.day());
            }
        }
    }

    if(AnimeData.contains("检索用名") && !AnimeData["检索用名"].empty()){
        downloadName = "(";
        for(auto i : AnimeData["检索用名"])
            downloadName += i + "|";
        downloadName.remove(downloadName.size()-1,1);
        downloadName += ")";
    }
    else if(allAnimeNames.size() > 0)
        downloadName = allAnimeNames[0];
    else if(allAnimeNames.size() == 0){
        qDebug() << AnimeData;
    }

    if(AnimeData.contains("检索路径") && !AnimeData["检索路径"].empty()){
        onlineUrl = AnimeData["检索路径"][0];
    }

    if(AnimeData.contains("Bangumi链接") && !AnimeData["Bangumi链接"].empty())
        thisBangumiLink = AnimeData["Bangumi链接"][0];

    if(AnimeData.contains("特殊在线添加集数") && AnimeData["特殊在线添加集数"].size() == 1)
        extraOnlineEpisodes = AnimeData["特殊在线添加集数"][0].toInt();
    if(AnimeData.contains("特殊下载添加集数") && AnimeData["特殊下载添加集数"].size() == 1)
        extraDownloadEpisodes = AnimeData["特殊下载添加集数"][0].toInt();

//  标签处理
    AnimeData["标签"] = removeDuplicates(AnimeData["标签"]);
    for(auto &i : AnimeData["标签"]){
        if(i.contains("/")){
            QStringList j = i.split("/");
            if(j.size() > 1)
                labels.push_back(qMakePair(j[0], j[1]));
            else
                labels.push_back(qMakePair(i, QString("")));
        }
        else
            labels.push_back(qMakePair(i, QString("")));

        if(i == "弃")
        {
            isAbandoned = true;
            episodesGivenUp = currentEpisode;
        }
        if(i == "剧场版")
            isMovie = true;
        if(i.contains("年") && i.contains("月"))
        {
            isSeasonAnime = true;
            seasons.push_back(i);
            if(LocalSeason == i){
                isCurrentSeason = true;
            }
        }
        if(i == "暂未更新" /*|| interval < 0*/)
            canWatchNow = false;
//        if(i.contains("开始集数")){
//            QRegExp rx("(\\d+)");  // 匹配数字
//            int pos = 0;
//            while ((pos = rx.indexIn(i, pos)) != -1) {
//                extraDownloadEpisodes = rx.cap(0).toInt();
//                pos += rx.matchedLength();
//            }
//        }
    }

    std::sort(seasons.begin(), seasons.end(), [&](QString a, QString b){return a > b;});
}

void AnimeDataManger::update(QHash<QString,QVector<QString>> &AnimeData){
    if(AnimeData.empty())
        return;
    IniData(AnimeData);

    if(viewLastDate.isValid()){
        watchInterval = daysBetween(viewLastDate, Today.date());
        if(viewStartDate.isValid())
            totalWatchTime = daysBetween(viewStartDate, viewLastDate);
    }

    if(hasWatched)
        displayContent = viewLastDate.toString("yyyy.MM.dd");
    else{
        if(currentEpisode != 0){
            displayContent = QString::number(currentEpisode);
            if(totalEpisodes != 0)
                displayContent += " / " + QString::number(totalEpisodes);
            if(isAbandoned)
                displayContent += " / 弃";
        }
    }

    int tempepisode = currentEpisode == 0 ? 1 : round(log10(currentEpisode)/log10(1.5)*0.6667+RfinalScore*currentEpisode+0.39);
    if(currentEpisode > 0){
        for (const auto& anime : interpolatedEpisodeScores) {
            storyScore += anime.Story;
            screenScore += anime.Screen;
            roleScore += anime.Role;
        }
    }
    if(hasWatched){
        storyScore += addedEpisodeScores["完结得分"].Story * tempepisode;
        screenScore += addedEpisodeScores["完结得分"].Screen * tempepisode;
        roleScore += addedEpisodeScores["完结得分"].Role * tempepisode;
    }
    else
        tempepisode = 0;

    double scoreFactor = currentEpisode + tempepisode;
    if(scoreFactor == 0 && addedEpisodeScores.contains("预期")){
        storyScore = addedEpisodeScores["预期"].Story;
        screenScore = addedEpisodeScores["预期"].Screen;
        roleScore = addedEpisodeScores["预期"].Role;
        sumScore = (storyScore * Rstory + screenScore * Rscreen +  roleScore * Rrole)/(double)(Rstory+Rscreen+Rrole);
    }
    else if(scoreFactor > 0){
        sumScore = (storyScore * Rstory + screenScore * Rscreen +  roleScore * Rrole)/(double)(Rstory+Rscreen+Rrole);
        sumScore /= scoreFactor;
        storyScore /= scoreFactor;              // 剧情评分
        screenScore /= scoreFactor;             // 画面评分
        roleScore /= scoreFactor;               // 人设评分
    }

    // 取小数
    storyScore = QString::number(storyScore,'f',precisePartScore).toDouble();
    screenScore = QString::number(screenScore,'f',precisePartScore).toDouble();
    roleScore = QString::number(roleScore,'f',precisePartScore).toDouble();
    sumScoreF = QString::number(sumScore,'f',preciseTotalScore).toDouble();              // 保留固定小数的总评分

}
