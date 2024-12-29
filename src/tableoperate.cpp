#include "animenestwindow.h"
#include "ui_animenestwindow.h"


void AnimeNestWindow::TableWidgetClick(){
    QString Name = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text();
    int row = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->data(Qt::UserRole).toInt();
    bool hasBangumiLink = false, hasgirigiriloveLink = false;
    QString BangumiLink = tempAnimeTable[row].BangumiLink;
    if(!BangumiLink.isEmpty())
        hasBangumiLink = true;


    ui->AnimeName->setText(Name);
//  设置开启链接

    if(hasBangumiLink){
        hasgirigiriloveLink = UpdateEpisodeListWidget(Name, BangumiLink);
    }

    // 更新自助页面
    UpdateBangumiSelfService(Name, BangumiLink, hasBangumiLink);

    UpdategirigiriloveSelfService(Name, BangumiLink, hasgirigiriloveLink);


//    mainBangumiLink = BangumiLink;

//    qDebug() << updateEpisodeInGirigirilove << " " << updateEpisodeInBangumi;

//  设置展示页面

    UpdateDetailDisplayANDFollowingAnimeData(Name, BangumiLink);

    UpdateSettingWidget(Name, BangumiLink);

    UpdateDetailInfo(Name, BangumiLink);


    if(TableType == 1){
        UpdateSplineChart(Name);
        UpdateHistoryViewDetailInput(Name);
        ui->FollowingThisAnime->setChecked(true);
    }
    else if(TableType == 2){
        int LinkMatchCount = 0;
        QString newName;
        for(auto i : AnimeMangerMap){
            if(i.thisBangumiLink == BangumiLink){
                newName = i.allAnimeNames[0];
                ++LinkMatchCount;
            }
        }

        if(AnimeMangerMap.contains(newName) && LinkMatchCount == 1){
            UpdateSplineChart(newName);

            UpdateHistoryViewDetailInput(newName);
        }

        QString isFollowing = ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text();
        QString tableBangumiUrl = ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text();
        if(isFollowing.contains("已追番")){
            ui->FollowingThisAnime->setChecked(true);
        }
        else
            ui->FollowingThisAnime->setChecked(false);
    }
}

tableData AnimeNestWindow::tableDataHandle(AnimeDataManger item){
    tableData ans;
    if(item.allAnimeNames.isEmpty()){
        return ans;
    }
    ans.Name = item.allAnimeNames[0];
    ans.insertData.push_back(item.allAnimeNames[0]);
    ans.insertData.push_back(QString::number(item.storyScore));
    ans.insertData.push_back(QString::number(item.screenScore));
    ans.insertData.push_back(QString::number(item.roleScore));
    ans.insertData.push_back(QString::number(item.sumScoreF));
    ans.insertData.push_back(item.displayContent);
    ans.BangumiLink = item.thisBangumiLink;
    return ans;
}

tableData AnimeNestWindow::tableDataHandle(QHash<QString, QVector<QString>> item, QString BangumiLink){
    tableData ans;
    QString Name = getHash(item, "番名",0);
    if(Name == "")
        Name = getHash(item, "原名",0);
    if(Name.isEmpty())
        return ans;

    QString AnilistScore = getHash(RankData,"Anilist",getHash(fastLinkBangumiToAll,BangumiLink, "Anilist", 0), "Anilist评分",0);
    QString AnilistPopularity = getHash(RankData,"Anilist",getHash(fastLinkBangumiToAll,BangumiLink, "Anilist", 0), "人气",0);
    QString AnilistFavourites = getHash(RankData,"Anilist",getHash(fastLinkBangumiToAll,BangumiLink, "Anilist", 0), "喜欢人数",0);
    QString BangumiScore = getHash(RankData,"Bangumi",BangumiLink, "Bangumi评分",0);
    QString BangumiRank = getHash(RankData,"Bangumi",BangumiLink, "排名",0);
    QString BangumiVotes = getHash(RankData,"Bangumi",BangumiLink, "投票人数",0);

    // 对Anilist喜欢数进行对数变换 (1 - 20000) -> (0.4 - 1.05)
    double favourites = log10(AnilistFavourites.toInt());
    if (favourites > log10(2000)) favourites = log10(2000);  // 限制最大值

    // 投票数归一化 (0.4 - 1.05)
    double normalizedFavourites = 0.75 * (favourites / log10(2000.0)) + 0.3;

    // 归一化排名 (1 - 10000) 到 (0 - 10)
    double normalizedRank = (10000 - BangumiRank.toDouble()) / 1000.0;
    if (normalizedRank > 10.0) normalizedRank = 10.0;

    // 对投票数进行对数变换 (1 - 12000) -> (0.4 - 1.2)
    double logVote = log10(BangumiVotes.toInt());
    if (logVote > log10(12000)) logVote = log10(12000);  // 限制最大值

    // 投票数归一化 (0.4 - 1.2)
    double normalizedVote = 0.75 * (logVote / log10(12000.0)) + 0.3;

    // 综合推荐度计算
    double recommendationA = normalizedFavourites * (0.5 * AnilistScore.toDouble()) + 0.5 * AnilistScore.toDouble();
    double recommendationB = normalizedVote * (0.2 * BangumiScore.toDouble() + 0.45 * normalizedRank) + 0.35 * BangumiScore.toDouble();

    double recommendation = recommendationB;
    if(recommendationA < 10 && recommendationA > 0)
        recommendation = 0.4*recommendationA + 0.6*recommendationB;

    // 确保最终推荐度在 0.0 到 10.0 之间
    if (recommendation > 10.0) recommendation = 10.0;
    if (recommendation < 0.0) recommendation = 0.0;
    recommendation = QString::number(recommendation,'f',2).toDouble();

    QString supportOnline = "/";
    auto findUrl = getHash(fastLinkBangumiToAll,BangumiLink);
    for(auto i = findUrl.begin() ; i!= findUrl.end() ; ++i){
        QString OnlineLink = getHash(fastLinkBangumiToAll,BangumiLink, i.key(),0);
        OnlineLink = getHash(OnlineStreamData, i.key(), OnlineLink ,i.key() + "路径" , 0);
        if(!OnlineLink.isEmpty())
            supportOnline = "支持";
    }

    QString LocalLink = getHash(fastLinkBangumiToAll,BangumiLink,"AnimeMangerMap",0);
    if(AnimeMangerMap.contains(LocalLink) && !AnimeMangerMap[LocalLink].onlineUrl.isEmpty())
        supportOnline = "支持";

    if(supportOnline == "/" && AnimeMangerMap.contains(LocalLink) && AnimeMangerMap[LocalLink].hasLocalFile)
        supportOnline = "本地";


    QString isFollowing = "/";

    for(auto k : AnimeMangerMap){
        if(k.thisBangumiLink != "" && k.thisBangumiLink == BangumiLink){
            isFollowing = "已追番";
            if(k.isAbandoned)
                isFollowing += " / 弃";
        }
    }

    ans.Name = Name;
    ans.insertData.push_back(Name);
    ans.insertData.push_back(BangumiScore);
    ans.insertData.push_back(AnilistScore);
    ans.insertData.push_back(QString::number(recommendation));
    ans.insertData.push_back(supportOnline);
    ans.insertData.push_back(isFollowing);
    ans.BangumiLink = BangumiLink;
    return ans;

}
//  向表格tablewidget插入数据
void AnimeNestWindow::InsertItem(tableData myItem , int &countInsertItem){
    if(myItem.Name.isEmpty())
        return;
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行

    for(int i = 0 ; i<myItem.insertData.size(); ++i){
        QTableWidgetItem  *WidgetAnime = new QTableWidgetItem();
        // 将原始行索引存储到每个单元格的 UserRole 中
        WidgetAnime->setData(Qt::UserRole, RowCont);
        if(myItem.insertData[i].toDouble() > 0){
            WidgetAnime->setData(Qt::DisplayRole,myItem.insertData[i].toDouble());
        }
        else
            WidgetAnime->setText(myItem.insertData[i]);
        ui->tableWidget->setItem(RowCont,i,WidgetAnime);
    }

    ++countInsertItem;
}
