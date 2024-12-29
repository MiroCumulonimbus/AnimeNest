#include "animenestwindow.h"
#include "ui_animenestwindow.h"

void AnimeNestWindow::UpdateSplineChart(const QString &Name) {
    if (!AnimeMangerMap.contains(Name)) {
        setLogsInfo("调用UpdateSplineChart时使用了无效的番名: " + Name);
        return;
    }

    const auto &episodeScores = AnimeMangerMap[Name].addedEpisodeScores;
    if (episodeScores.isEmpty()) {
//        setLogsInfo("调用UpdateSplineChart时没有评分数据，番名: " + Name);
        return;
    }

    // 初始化范围值
    double xMax = 0, xMin = 26, yMax = 9, yMin = 10;
    QVector<QPointF> storyData, screenData, roleData;

    // 遍历评分数据
    for (auto it = episodeScores.begin(); it != episodeScores.end(); ++it) {
        int episodeNumber = it.key().toInt();
        if (episodeNumber > 0) {
            double episode = it.key().toDouble();
            xMax = qMax(episode, xMax);
            xMin = qMin(episode, xMin);

            // 比较 Story, Screen, Role，更新 yMin
            yMin = qMin(yMin, qMin(it.value().Story, qMin(it.value().Screen, it.value().Role)));

            storyData.append({episode, it.value().Story});
            screenData.append({episode, it.value().Screen});
            roleData.append({episode, it.value().Role});
        }
    }

    // 限制 yMin 的下限值
    yMin = (yMin >= 5) ? 5 : yMin;

    // 对数据进行排序
    auto sortByX = [](const QPointF &a, const QPointF &b) { return a.x() < b.x(); };
    std::sort(storyData.begin(), storyData.end(), sortByX);
    std::sort(screenData.begin(), screenData.end(), sortByX);
    std::sort(roleData.begin(), roleData.end(), sortByX);

    // 更新图表数据
    storyseries->replace(storyData);
    screenseries->replace(screenData);
    roleseries->replace(roleData);

    // 更新坐标轴范围
    axisX->setRange(xMin, xMax);
    axisY->setRange(yMin, yMax + 1);
}

// 更新 HistoryViewDetail 统计的 当日/当周/当月 番剧
void AnimeNestWindow::UpdateHistoryView(){
    ui->HistoryViewDetail->clearContents();
    ui->HistoryViewDetail->setRowCount(0);
    QDate selectedDate = ui->calendarWidget->selectedDate();
    int countDay = 0 , countWeek = 0 , countMonth = 0 ;

    QHash<QString , QVector<QString>>::iterator j;
    QString SelectedMonth = ui->calendarWidget->selectedDate().toString("yyyy.MM");
    int RowCont;
    for(j = HistoryInDetail.begin() ; j != HistoryInDetail.end() ; ++j){
        QDate localTime = QDate::fromString(j.key(),"yyyy.MM.dd");
        bool addToViewDetail = false;
        if(localTime.year()==selectedDate.year())            {
            if(localTime.month()==selectedDate.month()){
                countMonth += j.value().size();
                if(ui->calendarMonth->isChecked()){
                    addToViewDetail = true;
                }
                if(localTime.day()==selectedDate.day()){
                    countDay += j.value().size();
                    if(ui->calendarDay->isChecked()){
                        addToViewDetail = true;
                    }
                }
            }
            if(abs(localTime.month()-selectedDate.month()) < 2 && localTime.weekNumber()==selectedDate.weekNumber()){
                countWeek += j.value().size();
                if(ui->calendarWeek->isChecked())
                    addToViewDetail = true;
            }
        }
        if(addToViewDetail){
            for(auto k : j.value()){
                RowCont=ui->HistoryViewDetail->rowCount();
                ui->HistoryViewDetail->insertRow(RowCont);//增加一行
                QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(k);
                ui->HistoryViewDetail->setItem(RowCont,0,WidgetAnime0);
            }
        }
    }

//  更新历史记录 calendarWidget修改颜色
    QTextCharFormat a;
    for(auto o = HistoryInDetail.begin() ; o != HistoryInDetail.end() ; ++o)
    {
        int colorValue = o.value().size()*10 > 255 ? 255 : o.value().size()*10;
        a.setBackground(QBrush(QColor(qRgb(255 - colorValue, 255 , 255 - colorValue))));
        ui->calendarWidget->setDateTextFormat( QDate::fromString(o.key() , "yyyy.MM.dd") , a);
    }

    ui->calendarDayCount->setText(QString::number(countDay));
    ui->calendarWeekCount->setText(QString::number(countWeek));
    ui->calendarMonthCount->setText(QString::number(countMonth));

    ui->HistoryViewDetail->setSortingEnabled(true);
    ui->HistoryViewDetail->sortByColumn(0, Qt::AscendingOrder);

}

bool AnimeNestWindow::UpdateEpisodeListWidget(const QString &Name, const QString &BangumiLink){
    ui->EpisodeListWidget->clear();
    ui->EpisodeListWidget->setSpacing(2);
    int displayEpisode = 0;
    int updateEpisodeInBangumi = 0, updateEpisodeInGirigirilove = 0;
    bool hasgirigiriloveLink = false;
    if(AnimeMangerMap.contains(Name)){
        displayEpisode = qMax(AnimeMangerMap[Name].totalEpisodes, AnimeMangerMap[Name].currentEpisode);
        if(!AnimeMangerMap[Name].onlineUrl.isEmpty())
            hasgirigiriloveLink = true;
    }

    if(!getHash(RankData, "Bangumi", BangumiLink, "总集数").isEmpty()){
        displayEpisode = getHash(RankData, "Bangumi", BangumiLink, "总集数").back().toInt();
    }
    if(!getHash(RankData, "Bangumi", BangumiLink, "首播", 0).isEmpty()){
        QDate startDate = QDate::fromString(getHash(RankData, "Bangumi", BangumiLink, "首播", 0), "yyyy年M月d日");
        int playEpisode = 1;
        while(playEpisode < 40){
            startDate = startDate.addDays(7);
            if(startDate > Today.date()){
                updateEpisodeInBangumi = playEpisode;
            }
            playEpisode++;
        }
    }
    if(!getHash(fastLinkBangumiToAll, BangumiLink, "girigirilove",0).isEmpty())
        hasgirigiriloveLink = true;


//  fix  AnimeFetchedData可能存在不同时期但同名的旧作翻新，这里以自己填入AnimeMangerMap的数据为准
    if(AnimeMangerMap.contains(Name) && AnimeMangerMap[Name].totalEpisodes != 0){
        displayEpisode = AnimeMangerMap[Name].totalEpisodes;
    }

//    if(AnimeMangerMap.contains(Name) && AnimeMangerMap[Name].hasLocalFile)
//        updateEpisodeInGirigirilove = std::max(updateEpisodeInGirigirilove, AnimeMangerMap[Name].totalEpisodes);
    for (int i = 1; i <= displayEpisode; ++i) {
        QString episodeText = QString::number(i);  // 使用 "第i集" 的格式表示集数
        QListWidgetItem* newitem = new QListWidgetItem(episodeText);
        newitem->setSizeHint(QSize(42,42));
        newitem->setTextAlignment(Qt::AlignCenter);
        if(updateEpisodeInBangumi > 0 || updateEpisodeInGirigirilove > 0){
            // 根据条件设置字体颜色
            /*if (i <= updateEpisodeInGirigirilove) {
                newitem->setForeground(QBrush(Qt::blue));  // 已更新，蓝色，girigirilove
            }
            else */if (i <= updateEpisodeInBangumi){
                newitem->setForeground(QBrush(QColor(225, 103, 113)));  // 已更新，红色，bangumi
            }
            else {
                newitem->setForeground(QBrush(Qt::gray));    // 未更新，灰色
            }
        }
        ui->EpisodeListWidget->addItem(newitem);
    }

    return hasgirigiriloveLink;
}

void AnimeNestWindow::UpdateBangumiSelfService(const QString &Name, const QString &BangumiLink, bool hasBangumiLink){
    ui->BangumiSelfService->setSortingEnabled(false);
    ui->BangumiSelfService->clearContents();
    ui->BangumiSelfService->setRowCount(0);

    QVector<QString> seasonLabel;
    if(AnimeMangerMap.contains(Name) && !AnimeMangerMap[Name].seasons.empty())
        seasonLabel = AnimeMangerMap[Name].seasons;

    if(!hasBangumiLink){
        QVector<QString> displayCandidate;
        for(auto q : noFastLinkItem["Bangumi"]){
            if(!getHash(RankData, "Bangumi", q,"番名",0).isEmpty() || !getHash(RankData, "Bangumi", q,"原名",0).isEmpty()){
                if(vectorMatch(getHash(RankData, "Bangumi", q,"更新年月"), seasonLabel)){
                    QString displayName = getHash(RankData, "Bangumi", q,"番名",0);
                    if(displayName.isEmpty())
                        displayName = getHash(RankData, "Bangumi", q,"原名",0);
                    int RowCont;
                    RowCont=ui->BangumiSelfService->rowCount();
                    ui->BangumiSelfService->insertRow(RowCont);//增加一行
                    QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(displayName);
                    QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem(q);
                    ui->BangumiSelfService->setItem(RowCont,0,WidgetAnime0);
                    ui->BangumiSelfService->setItem(RowCont,1,WidgetAnime1);

                    QPushButton *btn = new QPushButton("使用该行数据");
                    ui->BangumiSelfService->setCellWidget(RowCont, 2, btn);
                    connect(btn, &QPushButton::clicked, this, [=]() {
                        if(AnimeMangerMap.contains(Name)){
                            QString SelfanimeName = ui->BangumiSelfService->item(RowCont, 0)->text();
                            QString SelfBangumiLink = ui->BangumiSelfService->item(RowCont, 1)->text();
    //                            qDebug() << "按钮被点击, 番名:" << SelfanimeName << ", 番ID:" << SelfBangumiLink;
                            AnimeMangerMap[Name].thisBangumiLink = SelfBangumiLink;
                            if(!getHash(LocalData, "AnimeNest", Name).isEmpty())
                                LocalData["AnimeNest"][Name]["Bangumi链接"] = {SelfBangumiLink};
                            messageBoxWindow("更新Bangumi链接成功");
                        }
                        else
                            messageBoxWindow("请先选择追番");
                    });
                }
            }
        }

    }
    else{
        if(RankData["Bangumi"].contains(BangumiLink)){
            int RowCont = ui->BangumiSelfService->rowCount();
            ui->BangumiSelfService->insertRow(RowCont);//增加一行
            QString displayName = getHash(RankData, "Bangumi", BangumiLink, "番名", 0);
            if(displayName.isEmpty())
                displayName = getHash(RankData, "Bangumi", BangumiLink, "原名", 0);
            QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(displayName);
            QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem(BangumiLink);
            ui->BangumiSelfService->setItem(RowCont,0,WidgetAnime0);
            ui->BangumiSelfService->setItem(RowCont,1,WidgetAnime1);

            QPushButton *btn = new QPushButton("移除数据");
            ui->BangumiSelfService->setCellWidget(RowCont, 2, btn);
            connect(btn, &QPushButton::clicked, this, [=]() {
                if(AnimeMangerMap.contains(Name)){
                    AnimeMangerMap[Name].thisBangumiLink = "";
                    if(!getHash(LocalData, "AnimeNest", Name).isEmpty())
                        LocalData["AnimeNest"][Name].remove("Bangumi链接");
                    messageBoxWindow("已移除Bangumi链接");
                }
                else
                    messageBoxWindow("请先选择追番");
            });
        }
    }
    ui->BangumiSelfService->setSortingEnabled(true);
}

void AnimeNestWindow::UpdategirigiriloveSelfService(const QString &Name, const QString &BangumiLink, bool hasgirigiriloveLink){
    ui->girigiriloveSelfService->setSortingEnabled(false);
    ui->girigiriloveSelfService->clearContents();
    ui->girigiriloveSelfService->setRowCount(0);
    QString seasonLabel;
    if(AnimeMangerMap.contains(Name) && !AnimeMangerMap[Name].seasons.empty())
        seasonLabel = AnimeMangerMap[Name].seasons.back();
    else if(!BangumiLink.isEmpty() && RankData["Bangumi"].contains(BangumiLink) && !RankData["Bangumi"][BangumiLink]["标签"].empty()){
        for(auto j : RankData["Bangumi"][BangumiLink]["标签"]){
            if(j.contains("年") && j.contains("月")){
                QDate temp = QDate::fromString(j, "yyyy年M月");
                if(temp.isValid()){
                    seasonLabel = j;
                    break;
                }
            }
        }
    }
    if(!hasgirigiriloveLink){
        if(!seasonLabel.isEmpty()){
            QVector<QString> displayCandidate;
            for(auto k = OnlineStreamData["girigirilove"].begin(); k != OnlineStreamData["girigirilove"].end() ; ++k){
                if(k.value().contains("更新年月") && k.value()["更新年月"].contains(seasonLabel))
                    displayCandidate.push_back(k.key());
            }
            for(auto k = RankData["Bangumi"].begin(); k != RankData["Bangumi"].end() ; ++k){
                if(k.value().contains("标签") && k.value()["标签"].contains(seasonLabel) &&
                   k.value().contains("girigirilove") && !k.value()["girigirilove"].isEmpty())
                    displayCandidate.removeOne(k.value()["girigirilove"][0]);
            }

            for(auto q : displayCandidate){
                if(OnlineStreamData["girigirilove"][q].contains("番名") && !OnlineStreamData["girigirilove"][q]["番名"].isEmpty()){
                    int RowCont;
                    RowCont=ui->girigiriloveSelfService->rowCount();
                    ui->girigiriloveSelfService->insertRow(RowCont);//增加一行
                    QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(OnlineStreamData["girigirilove"][q]["番名"][0]);
                    QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem(q);
                    ui->girigiriloveSelfService->setItem(RowCont,0,WidgetAnime0);
                    ui->girigiriloveSelfService->setItem(RowCont,1,WidgetAnime1);

                    QPushButton *btn = new QPushButton("使用该行数据");
                    ui->girigiriloveSelfService->setCellWidget(RowCont, 2, btn);
                    connect(btn, &QPushButton::clicked, this, [=]() {
                        if(RankData["Bangumi"].contains(BangumiLink)){
                            QString SelfanimeName = ui->girigiriloveSelfService->item(RowCont, 0)->text();
                            QString SelfgirigiriloveiLink = ui->girigiriloveSelfService->item(RowCont, 1)->text();
//                            qDebug() << "按钮被点击, 番名:" << SelfanimeName << ", 番ID:" << SelfgirigiriloveiLink;
                            RankData["Bangumi"][BangumiLink]["girigirilove"] = {SelfgirigiriloveiLink};
                            fastLinkBangumiToAll[BangumiLink]["girigirilove"].append(SelfgirigiriloveiLink);
//                                AnimeFetchedBangumiData[AnimeJsonMap[Name]["Bangumi链接"][0]]["别名"].push_back(SelfanimeName);
                            messageBoxWindow("更新girigirilove链接成功");
                        }
                        else{
                            messageBoxWindow("需要先添加Bangumi链接");
                        }
                    });
                }
            }
        }
    }
    else{
        if(!BangumiLink.isEmpty() && !getHash(fastLinkBangumiToAll, BangumiLink, "girigirilove", 0).isEmpty() &&
                !getHash(OnlineStreamData, "girigirilove", getHash(fastLinkBangumiToAll, BangumiLink, "girigirilove", 0), "番名", 0).isEmpty()){
            int RowCont = ui->girigiriloveSelfService->rowCount();
            ui->girigiriloveSelfService->insertRow(RowCont);//增加一行
            QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(getHash(OnlineStreamData, "girigirilove", getHash(fastLinkBangumiToAll, BangumiLink, "girigirilove", 0), "番名", 0));
            QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem(getHash(fastLinkBangumiToAll, BangumiLink, "girigirilove", 0));
            ui->girigiriloveSelfService->setItem(RowCont,0,WidgetAnime0);
            ui->girigiriloveSelfService->setItem(RowCont,1,WidgetAnime1);

            QPushButton *btn = new QPushButton("移除数据");
            ui->girigiriloveSelfService->setCellWidget(RowCont, 2, btn);
            connect(btn, &QPushButton::clicked, this, [=]() {
                RankData["Bangumi"][BangumiLink]["girigirilove"].clear();
                noFastLinkItem["girigirilove"].append(getHash(fastLinkBangumiToAll, BangumiLink, "girigirilove", 0));
                fastLinkBangumiToAll[BangumiLink].remove("girigirilove");
                messageBoxWindow("已移除girigirilove链接");
            });
        }
        else if(AnimeMangerMap.contains(Name) && !AnimeMangerMap[Name].onlineUrl.isEmpty()){
            int RowCont = ui->girigiriloveSelfService->rowCount();
            ui->girigiriloveSelfService->insertRow(RowCont);//增加一行
            QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem(Name);
            QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem(AnimeMangerMap[Name].onlineUrl);
            ui->girigiriloveSelfService->setItem(RowCont,0,WidgetAnime0);
            ui->girigiriloveSelfService->setItem(RowCont,1,WidgetAnime1);

            QPushButton *btn = new QPushButton("移除数据");
            ui->girigiriloveSelfService->setCellWidget(RowCont, 2, btn);
            connect(btn, &QPushButton::clicked, this, [=]() {
                QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
                if(!getHash(LocalData, addKey, Name,"girigirilove",0).isEmpty()){
                    LocalData[addKey][Name]["girigirilove链接"].clear();
                    AnimeMangerMap[Name].onlineUrl = "";
                    if(!getHash(fastLinkBangumiToAll, BangumiLink, "AnimeMangerMap",0).isEmpty())
                        fastLinkBangumiToAll[BangumiLink].remove("AnimeMangerMap");
                    noFastLinkItem["AnimeMangerMap"].append(Name);
                    messageBoxWindow("已移除girigirilove链接");
                }
                else
                    messageBoxWindow("查找girigirilove链接时出错");
            });
        }
    }

    ui->girigiriloveSelfService->setSortingEnabled(true);
}

void AnimeNestWindow::UpdateDetailDisplayANDFollowingAnimeData(const QString &Name, const QString &BangumiLink){
    QHash<QString,QHash<QString,QVector<QString>>> displayDetail1, displayDetail2;
    if(ui->DetailDisplayAnimeNestData->isChecked() && AnimeMangerMap.contains(Name))
        displayDetail1.insert(Name, getHash(LocalData, "AnimeNest", Name));
    if(ui->DetailDisplayAnimeFetchedData->isChecked()){
        if(!BangumiLink.isEmpty()){
            displayDetail2.insert(Name, getHash(RankData, "Bangumi", BangumiLink));
            dataMatch(displayDetail1, displayDetail2, displayDetail1, displayDetail2);
            displayDetail2.clear();
            if(!getHash(RankData, "Bangumi", BangumiLink,"girigirilove",0).isEmpty() ){
                QString girigiriloveLink = getHash(RankData, "Bangumi", BangumiLink,"girigirilove链接",0);
                displayDetail2.insert(Name, getHash(OnlineStreamData, "girigirilove", girigiriloveLink));
                dataMatch(displayDetail1, displayDetail2, displayDetail1, displayDetail2);
                displayDetail2.clear();
            }
        }
    }
    dataDisplay(displayDetail1, ui->DetailDisplay);
//    dataDisplay(displayDetail1, ui->FollowingAnimeData);
//    ui->FollowingAnimeData->itemAt(0,0)->setExpanded(1);
    ui->DetailDisplay->itemAt(0,0)->setExpanded(1);
}

void AnimeNestWindow::UpdateSettingWidget(const QString &Name, const QString &BangumiLink){
    ui->AnimeSearchUrl->clear();
    ui->listWidget->clear();
    ui->AnimeNameForFind->clear();

    ui->AnimeSearchUrl->clear();
    ui->SpecialWebsiteAddEpisode->setValue(0);
    ui->SpecialDownloadAddEpisode->setValue(0);
    ui->AnimeNameForFind->clear();
    ui->AnimeAlias->clear();
    ui->TotalEpisode->setValue(0);
    ui->StartUpdateTime->setDate(QDate::fromString( "0000.00.00" , "yyyy.MM.dd"));

    if(!AnimeMangerMap.contains(Name)){
        if(RankData["Bangumi"].contains(BangumiLink)){
            QString getScore = getHash(RankData["Bangumi"], BangumiLink, "Bangumi评分", 0);
            int BangumiScore = getScore.isEmpty() ? 0 : qRound(getScore.toDouble());
            ui->StoryScore->setValue(BangumiScore);
            ui->ScreenScore->setValue(BangumiScore);
            ui->RoleScore->setValue(BangumiScore);
            if(!getHash(RankData["Bangumi"], BangumiLink, "标签").isEmpty()){
                QVector<QString> metaLabel = RankData["Bangumi"][BangumiLink]["标签"];
                for (auto j : metaLabel) {
                    // 创建 QListWidgetItem
                    auto *tempItem = new QListWidgetItem();
                    tempItem->setText(j);
                    ui->listWidget->addItem(tempItem);
                }
            }
//            if(!getHash(AnimeFetchedBangumiData, BangumiLink, "girigirilove", 0).isEmpty()){
//                QString girigiriLink = getHash(AnimeFetchedBangumiData, BangumiLink, "girigirilove链接", 0);
//                if(!getHash(AnimeFetchedGirigiriloveData, girigiriLink, "girigirilove路径", 0).isEmpty())
//                    ui->AnimeSearchUrl->setText(getHash(AnimeFetchedGirigiriloveData, girigiriLink, "girigirilove路径", 0));
//            }
            if(!getHash(RankData["Bangumi"], BangumiLink, "总集数", 0).isEmpty()){
                ui->TotalEpisode->setValue(getHash(RankData["Bangumi"], BangumiLink, "总集数", 0).toInt());
            }
            if(!getHash(RankData["Bangumi"], BangumiLink, "首播", 0).isEmpty()){
                ui->StartUpdateTime->setDate(QDate::fromString(getHash(RankData["Bangumi"], BangumiLink, "首播", 0) , "yyyy年M月d日"));
            }
        }
        return;
    }

//  设置当前分数
    ui->StoryScore->setValue(qRound(AnimeMangerMap[Name].storyScore));
    ui->ScreenScore->setValue(qRound(AnimeMangerMap[Name].screenScore));
    ui->RoleScore->setValue(qRound(AnimeMangerMap[Name].roleScore));
//  设置当前更新集数下一集
    ui->Episode->setValue(AnimeMangerMap[Name].currentEpisode + 1);
//  设置标签

    if(!AnimeMangerMap[Name].seasons.empty()){
        for(auto j : AnimeMangerMap[Name].seasons)
            ui->listWidget->addItem(j);
    }
    QVector<QString> localLabels;
    for(int i = 0 ; i < AnimeMangerMap[Name].labels.size() ; ++i){
        QString tempLabel = AnimeMangerMap[Name].labels[i].first;
        if(AnimeMangerMap[Name].labels[i].second != "")
            tempLabel += "/" + AnimeMangerMap[Name].labels[i].second;
        if(tempLabel.contains("年") && tempLabel.contains("月"))
            continue;
        ui->listWidget->addItem(tempLabel);
        localLabels.push_back(tempLabel);
    }
    if(AnimeMangerMap.contains(Name) && !AnimeMangerMap[Name].thisBangumiLink.isEmpty()){
        if(!getHash(RankData["Bangumi"], AnimeMangerMap[Name].thisBangumiLink, "meta标签").isEmpty()){
            QVector<QString> metaLabel = RankData["Bangumi"][AnimeMangerMap[Name].thisBangumiLink]["meta标签"];
            for (auto j : metaLabel) {
                if (!localLabels.contains(j)) {
                    // 创建 QListWidgetItem
                    auto *tempItem = new QListWidgetItem();
                    tempItem->setText(j);
                    ui->listWidget->addItem(tempItem);
                }
            }
        }
    }

//  设置详细搜索页面

    ui->AnimeSearchUrl->setText(AnimeMangerMap[Name].onlineUrl);
    ui->SpecialWebsiteAddEpisode->setValue(AnimeMangerMap[Name].extraOnlineEpisodes);
    ui->SpecialDownloadAddEpisode->setValue(AnimeMangerMap[Name].extraDownloadEpisodes);
    ui->AnimeNameForFind->setText(AnimeMangerMap[Name].downloadName);
    // 别名
    ui->AnimeAlias->setText(stringVectorToString(AnimeMangerMap[Name].allAnimeNames, "\n"));
    // 总集数
    ui->TotalEpisode->setValue(AnimeMangerMap[Name].totalEpisodes);
    // 首播日期
    ui->StartUpdateTime->setDate(AnimeMangerMap[Name].updateStartDate);
}


void AnimeNestWindow::UpdateDetailInfo(QString Name, QString BangumiLink){
    ui->AnimeDetailName->setText(Name);
    ui->AnimeStartTime->setDate(QDate::fromString( "0000.00.00" , "yyyy.MM.dd"));
    ui->AnimeEndTime->setDate(QDate::fromString( "0000.00.00" , "yyyy.MM.dd"));
    ui->WatchedAnime_2->setChecked(false);
    ui->WatchingAnime_2->setChecked(false);
    ui->AnimeLocalSeason->setText("0000年0月");
    ui->AnimeEpisode->clear();
    ui->AnimeIntervalDay->clear();
    ui->AnimeTime->clear();
    ui->FloderSize->setText("0GB");
    ui->AnimeProducer->setText("");
    ui->Supervisor->setText("");

    ui->GiveUp->setChecked(false);
    ui->AnimeMovie_2->setChecked(false);
    ui->LocalSeasonAnime_2->setChecked(false);
    ui->NowCanWatch->setChecked(false);
    ui->SeasonAnime->setChecked(false);
    ui->LocalFile->setChecked(false);

    if(RankData["Bangumi"].contains(BangumiLink)){
        if(!getHash(RankData["Bangumi"], BangumiLink, "总导演", 0).isEmpty()){
            ui->Supervisor->setText(getHash(RankData["Bangumi"], BangumiLink, "总导演", 0));
        }
        else if(!getHash(RankData["Bangumi"], BangumiLink, "导演").isEmpty()){
            ui->Supervisor->setText(RankData["Bangumi"][BangumiLink]["导演"].back());
        }
        if(!getHash(RankData["Bangumi"], BangumiLink, "动画制作", 0).isEmpty()){
            ui->AnimeProducer->setText(getHash(RankData["Bangumi"], BangumiLink, "动画制作", 0));
        }
        QVector<QString> season = getHash(RankData["Bangumi"], BangumiLink, "更新年月");
        if(!season.isEmpty()){
            std::sort(season.begin(), season.end());
            ui->AnimeLocalSeason->setText(season.back());
        }
    }

    if(!AnimeMangerMap.contains(Name)){
        int LinkMatchCount = 0;
        for(auto i : AnimeMangerMap){
            if(i.thisBangumiLink == BangumiLink){
                Name = i.allAnimeNames[0];
                ++LinkMatchCount;
            }
        }
        if(!AnimeMangerMap.contains(Name) || LinkMatchCount > 1)
            return;
    }

    if(!AnimeMangerMap[Name].viewStartDate.isNull())
        ui->AnimeStartTime->setDate(AnimeMangerMap[Name].viewStartDate);

    if(!AnimeMangerMap[Name].viewLastDate.isNull())
        ui->AnimeEndTime->setDate(AnimeMangerMap[Name].viewLastDate);

    ui->WatchedAnime_2->setChecked(AnimeMangerMap[Name].hasWatched);
    ui->WatchingAnime_2->setChecked(AnimeMangerMap[Name].isCurrentlyWatching);

    if(!AnimeMangerMap[Name].seasons.isEmpty())
        ui->AnimeLocalSeason->setText(AnimeMangerMap[Name].seasons[0]);
    QString AnimeEpisode = QString::number(AnimeMangerMap[Name].currentEpisode);
    if(AnimeMangerMap[Name].totalEpisodes > 0)
        AnimeEpisode += "/" + QString::number(AnimeMangerMap[Name].totalEpisodes);
    ui->AnimeEpisode->setText(AnimeEpisode);
    ui->AnimeIntervalDay->setText(QString::number(AnimeMangerMap[Name].watchInterval));
    ui->AnimeTime->setText(QString::number(AnimeMangerMap[Name].totalWatchTime));
    ui->GiveUp->setChecked(AnimeMangerMap[Name].isAbandoned);
    ui->AnimeMovie_2->setChecked(AnimeMangerMap[Name].isMovie);
    ui->LocalSeasonAnime_2->setChecked(AnimeMangerMap[Name].isCurrentSeason);
    ui->NowCanWatch->setChecked(AnimeMangerMap[Name].canWatchNow);
    ui->SeasonAnime->setChecked(AnimeMangerMap[Name].isSeasonAnime);
    ui->LocalFile->setChecked(AnimeMangerMap[Name].hasLocalFile);
    ui->FloderSize->setText(QString::number(AnimeMangerMap[Name].floderSize) + "GB");

}

void AnimeNestWindow::UpdateDataFromBangumi(QString Name, QString BangumiLink){
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
// 重新追番会更新数据
    if(getHash(LocalData, addKey, Name, "0").isEmpty())
        LocalData[addKey][Name]["0"] = {"6", "6", "6", getLocalTime()};
    if(getHash(LocalData, addKey, Name, "番名").isEmpty())
        LocalData[addKey][Name]["番名"] = {Name};
    if(getHash(LocalData, addKey, Name, "Bangumi链接").isEmpty())
        LocalData[addKey][Name]["Bangumi链接"] = {BangumiLink};
    if(getHash(LocalData, addKey, Name, "集数").isEmpty()){
        if(!getHash(RankData["Bangumi"], BangumiLink, "总集数", 0).isEmpty()){
            LocalData[addKey][Name]["集数"] = getHash(RankData["Bangumi"], BangumiLink, "总集数");
        }
    }

    if(RankData["Bangumi"].contains( BangumiLink)){
        QVector<QString> waitAddLabels;
        if(!getHash(RankData["Bangumi"], BangumiLink, "更新年月", 0).isEmpty()){
            waitAddLabels.append(getHash(RankData["Bangumi"], BangumiLink, "更新年月"));
        }

        if(!getHash(RankData["Bangumi"], BangumiLink, "首播", 0).isEmpty()){
            if(getHash(LocalData, addKey, Name, "首播").isEmpty())
                LocalData[addKey][Name]["首播"] = getHash(RankData["Bangumi"], BangumiLink, "首播");
            int totalEpisodeTemp = 1;
            if(!getHash(RankData["Bangumi"], BangumiLink, "总集数", 0).isEmpty())
                totalEpisodeTemp = getHash(RankData["Bangumi"], BangumiLink, "总集数", 0).toInt();
            waitAddLabels.append(getQStringSeasons(LocalData[addKey][Name]["首播"].back(), totalEpisodeTemp));
        }
        waitAddLabels.append(getHash(RankData["Bangumi"], BangumiLink, "meta标签"));
        waitAddLabels = removeDuplicates(waitAddLabels);
        AddNewLabel(Name, stringVectorToString(waitAddLabels, "、"));
    }
}

void AnimeNestWindow::UpdateHistoryViewDetailInput(QString Name){
    if(Name == "")
        return;
    ui->HistoryViewDetailInput->clearContents();
    ui->HistoryViewDetailInput->setRowCount(0);
    ui->HistoryViewDetailInput->setSortingEnabled(false);

    QHash<QString, sAnimeData> insertData;
    if(ui->displayAllEpisode->isChecked()){
        for(int i = 0 ; i < AnimeMangerMap[Name].interpolatedEpisodeScores.size() ; ++i){
            insertData.insert(QString::number(i+1), AnimeMangerMap[Name].interpolatedEpisodeScores[i]);
        }
    }
    else{
        insertData = AnimeMangerMap[Name].addedEpisodeScores;
    }

    int RowCont;
    for(auto it = insertData.begin();it != insertData.end() ; it++){
        if(it.key().toInt() < 1)
            continue;
        RowCont=ui->HistoryViewDetailInput->rowCount();
        ui->HistoryViewDetailInput->insertRow(RowCont);//增加一行
        QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem();
        QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem();
        QTableWidgetItem  *WidgetAnime2 = new QTableWidgetItem();
        QTableWidgetItem  *WidgetAnime3 = new QTableWidgetItem();
        QTableWidgetItem  *WidgetAnime4 = new QTableWidgetItem(it.value().Time.toString("yyyy.MM.dd"));
        WidgetAnime0->setData(Qt::DisplayRole,it.key().toInt());
        WidgetAnime1->setData(Qt::DisplayRole,it.value().Story);
        WidgetAnime2->setData(Qt::DisplayRole,it.value().Screen);
        WidgetAnime3->setData(Qt::DisplayRole,it.value().Role);
        ui->HistoryViewDetailInput->setItem(RowCont,0,WidgetAnime0);
        ui->HistoryViewDetailInput->setItem(RowCont,1,WidgetAnime1);
        ui->HistoryViewDetailInput->setItem(RowCont,2,WidgetAnime2);
        ui->HistoryViewDetailInput->setItem(RowCont,3,WidgetAnime3);
        ui->HistoryViewDetailInput->setItem(RowCont,4,WidgetAnime4);
    }
    if(AnimeMangerMap[Name].hasWatched)
    {
        for(int i = 0 ; i < AnimeMangerMap[Name].allCompleteScores.size() ; i += 4){
            RowCont=ui->HistoryViewDetailInput->rowCount();
            ui->HistoryViewDetailInput->insertRow(RowCont);//增加一行
            QTableWidgetItem  *WidgetAnime0 = new QTableWidgetItem();
            QTableWidgetItem  *WidgetAnime1 = new QTableWidgetItem();
            QTableWidgetItem  *WidgetAnime2 = new QTableWidgetItem();
            QTableWidgetItem  *WidgetAnime3 = new QTableWidgetItem();
            QTableWidgetItem  *WidgetAnime4 = new QTableWidgetItem(AnimeMangerMap[Name].allCompleteScores[i+3]);
            WidgetAnime0->setData(Qt::DisplayRole,1000 + i/4+1);
            WidgetAnime1->setData(Qt::DisplayRole,AnimeMangerMap[Name].allCompleteScores[i].toDouble());
            WidgetAnime2->setData(Qt::DisplayRole,AnimeMangerMap[Name].allCompleteScores[i+1].toDouble());
            WidgetAnime3->setData(Qt::DisplayRole,AnimeMangerMap[Name].allCompleteScores[i+2].toDouble());
            ui->HistoryViewDetailInput->setItem(RowCont,0,WidgetAnime0);
            ui->HistoryViewDetailInput->setItem(RowCont,1,WidgetAnime1);
            ui->HistoryViewDetailInput->setItem(RowCont,2,WidgetAnime2);
            ui->HistoryViewDetailInput->setItem(RowCont,3,WidgetAnime3);
            ui->HistoryViewDetailInput->setItem(RowCont,4,WidgetAnime4);
        }
    }
    ui->HistoryViewDetailInput->setSortingEnabled(true);
    ui->HistoryViewDetailInput->sortByColumn(0, Qt::AscendingOrder);
    //    qDebug() << "HistoryViewDetailInput : size" << ui->HistoryViewDetailInput->rowCount();
}


//  更新AllMyAnimeLabel
void AnimeNestWindow::UpdateAnimeMangerMap(const QString &Name)
{
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
    if (Name.isEmpty()) {
        // 更新所有AnimeDataManger
        for (AnimeDataManger &i : AnimeMangerMap) {
            if (i.allAnimeNames.isEmpty()) continue;
            if(getHash(LocalData, addKey, i.allAnimeNames[0], "已删除", 0) == "1" )
                AnimeMangerMap.remove(i.allAnimeNames[0]);
            else
                i.update(getHash(LocalData, addKey)[i.allAnimeNames[0]]);
        }
    } else {
        // 更新单个AnimeDataManger
        if(getHash(LocalData, addKey, Name, "已删除", 0) == "1"){
            if (AnimeMangerMap.contains(Name)) {
                AnimeMangerMap.remove(Name);
            }
        }
        else{
            if (AnimeMangerMap.contains(Name)) {
                AnimeMangerMap[Name].update(LocalData[addKey][Name]);
            } else {
                AnimeMangerMap.insert(Name, AnimeDataManger(LocalData[addKey][Name], Name));
            }
        }
    }

    if (ui->LoadSeasonAnime->isChecked()) LoadAnimeWithLabel({ui->LocalSeason->text()});
    if (ui->AllAnime->isChecked()) GetAllAnime();
    if (ui->RecentUpdate->isChecked()) GetRecentUpdate();
    if (ui->LocalSeasonAnime->isChecked()) GetLocalSeasonAnime();
    if (ui->ShelvedAnime->isChecked()) GetShelvedAnime();
    if (ui->WatchedAnime->isChecked()) GetWatchedAnime();
    if (ui->WatchingAnime->isChecked()) GetWatchingAnime();
    if (ui->WatchingLastAnime->isChecked()) GetWatchingLastAnime();
    if (ui->DiscardAnime->isChecked()) GetDiscardAnime();
    if (ui->DiscardAnime2->isChecked()) GetDiscardAnime2();
    if (ui->AnimeMovie->isChecked()) GetAnimeMovie();
}
