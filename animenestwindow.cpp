#include "animenestwindow.h"
#include "ui_animenestwindow.h"

AnimeNestWindow::AnimeNestWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AnimeNestWindow),
    m_chart(new QChart),
    storyseries(new QSplineSeries),
    screenseries(new QSplineSeries),
    roleseries(new QSplineSeries),
    chartView(new QChartView(m_chart)),
    axisX(new QValueAxis),
    axisY(new QValueAxis)
{
    ui->setupUi(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->AnimeName->installEventFilter(this);
    ui->FindLabel->installEventFilter(this);
//    ui->keyWord->installEventFilter(this);
    ui->AnimeLabel->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->setMouseTracking(true);

    IniData();
    IniTable(1);
    IniChartANDTables();
    IniCalendarWidget();

    // 设置控件的可见性
    ui->widget_12->setVisible(false);
    ui->DetailData->setVisible(false);
    ui->ToolSelect->setVisible(false);
    ui->ToolExtention->setText(">\n>\n>");
    this->setWindowTitle("AnimeNest");
    this->setMinimumWidth(this->minimumWidth()-406);
    resize(QMainWindow::size().width()-406 , QMainWindow::size().height());

    qDebug() << dataFilesSaveUrl;
//    for(auto i = localData.begin(); i != localData.end() ; ++i){
//        saveData(i.key(), i.value());
//    }



//// 函数测试： displayHTML
//    QHash<QString, QHash<QString, QVector<QString>>> savetofile, matchresult;
//    loadData("savetofile.json" ,savetofile);
//    savetofile.clear();
//    NetworkFetch fetcher;
//    QString htmlUrl = "https://www.741dm.com/view/2486.html";

//    qDebug() << QString::number(1).rightJustified(2, '0');

////    savetofile = fetcher.fetchAnilistDataWithSeason("2024年10月");

//    QHash<QString, QString> regexMap;
//    regexMap.insert("番名", R"(<title>《(.*?)》)");
//    regexMap.insert("Sakura路径", R"(<a href=\"(/player/\d+-\d+-)\d+.html)");
//    regexMap.insert("更新进度", R"(<a href=\"/player/\d+-\d+-\d+.html\">第(\d+)集</a>)");
//    regexMap.insert("更新日期", R"(更新：</span>(.*?)</p>)");

////    regexMap1.insert("girigirilove路径", R"(class=\"public-list-exp\" href=\"/(GV\d+).*?class=\"lazy lazy1 gen-movie-img mask-1\" alt=\"(.*?)封面图\")");
//    savetofile["test"] = fetcher.fetchLinks(htmlUrl, regexMap);
//    savetofile["test"]["html"].push_back(fetcher.displayHTML(htmlUrl));
//    for(auto i : BangumiRef){
//        QVector<QString> labels = getHash(i,"标签"), names = getHash(i, "番名"), addSeasons;
//        names.append(getHash(i, "别名"));
//        if(vectorMatch(labels, {QString::number(2021)})){
//            if(vectorMatch(names, getHash(savetofile["test"], "番名")) ||
//                           vectorMatchBlur(names,getHash(savetofile["test"], "番名"),50) > 65){
//                for(auto j : labels){
//                    if(j.contains("年") && j.contains("月")){
//                        addSeasons.push_back(j);
//                    }
//                }
//                if(!getHash(i,"更新年月",0).isEmpty())
//                    addSeasons.append(getHash(i,"更新年月"));
//                qDebug() << names;
//            }

//        }
//        addSeasons = removeDuplicates(addSeasons);
//        if(!addSeasons.isEmpty())
//            savetofile["test"]["更新年月"] = addSeasons;
//    }
////    savetofile = fetcher.fetchSakuraWithSeason("2024年10月");
////    dataMatch( savetofile, fetcher.fetchGirigiriloveWithNames(test), savetofile, matchresult);
////    qDebug() << savetofile;
//    saveData("savetofile.json" ,savetofile);
//// 函数测试： displayHTML


//// 函数测试： fetchBangumiWithNames
//    QHash<QString, QHash<QString, QVector<QString>>> matchResult;
//    NetworkFetch fetcher;
//    QVector<QString> htmlUrl;
//    for(auto i : AnimeMangerMap){
////        if(i.isCurrentSeason)
//            htmlUrl.push_back(i.allAnimeNames[0]);
//    }
////    htmlUrl.push_back("大欺诈师 razbliuto");
//    setLogsInfo( dataMatch(AnimeFetchedData, fetcher.fetchBangumiWithNames(htmlUrl), AnimeFetchedData, matchResult));

////    for(auto i = getAnimeData.begin() ; i != getAnimeData.end() ; ++i){
////        for(auto &j : AnimeMangerMap){
////            if(vectorMatch(j.allAnimeNames, i.value()["番名"]) || vectorMatch(j.allAnimeNames, i.value()["别名"])){
    
////                break;
////            }
////        QString animeUrl = "https://anime.girigirilove.com/play";
////        if(i.value().contains("路径") && i.value()["路径"].size() > 0)
////            animeUrl += i.value()["路径"][0] + "-1-" + specialPlaceholder + "/";
////        if(AnimeJsonMap.contains(i.key()))
////            AnimeJsonMap[i.key()]["检索路径"] = {animeUrl};
////        }
////    }

//    saveData("getAnimeData.json" ,getAnimeData);
//// 函数测试： fetchBangumiWithNames  --------------

// 函数测试： fetchGirigiriloveWithNames

//    NetworkFetch fetcher;
//    QVector<QString> htmlUrl;
//    for(auto i : AnimeMangerMap){
//        htmlUrl.append(i.allAnimeNames);
//    }

//    setLogsInfo( dataMatch(AnimeFetchedData, fetcher.fetchGirigiriloveWithNames(htmlUrl), AnimeFetchedData, matchResult));
//    saveData("AnimeFetchedData.json" ,AnimeFetchedData);
//    for(auto i = getUrl.begin() ; i != getUrl.end() ; ++i){
//        QString animeUrl = "https://anime.girigirilove.com/play";
//        if(i.value().contains("路径") && i.value()["路径"].size() > 0)
//            animeUrl += i.value()["路径"][0] + "-1-" + specialPlaceholder + "/";
//        if(AnimeJsonMap.contains(i.key()))
//            AnimeJsonMap[i.key()]["检索路径"] = {animeUrl};
//    }
//    saveData("savetofile.json" ,savetofile);
// 函数测试： fetchGirigiriloveWithNames  --------------

//    QHash<QString, QHash<QString, QVector<QString>>> savetofile;
//    loadData("savetofile.json" ,savetofile);
//    savetofile.clear();
//    savetofile["test"] = fetcher.fetchGirigiriloveWithNames(htmlUrl);
//    saveData("savetofile.json" ,savetofile);

//  查找番剧 按名称
    connect(ui->FindItem , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        GetFindAnime(Name);
    });
//  按标签查找：在输入框输入要查找的标签，返回标签匹配数大于 最少匹配 的数据
    connect(ui->FindAnimeInLabel , &QPushButton::clicked , [&](){
        QString Label = ui->FindLabel->toPlainText();
        GetLabelAnime(Label , ui->LabelCount->value());
        return;
    });
//  点击listwidget查找对应标签
    connect(ui->listWidget , &QListWidget::itemClicked , [&](){
        ui->FindLabel->setText(ui->listWidget->currentItem()->text());
        ui->FindAnimeInLabel->click();
    });
//  使用 listwidget 中的全部数据做标签匹配
    connect(ui->FindSimilarAnimeInLabel , &QPushButton::clicked , [&](){
        QVector<QString> Labels;
        int itemCount = ui->listWidget->count();  // 获取列表项的数量
        for (int i = 0; i < itemCount; ++i) {
            Labels.push_back(ui->listWidget->item(i)->text());
        }
        for(QString s : Labels){
            QRegularExpression re("\\d{4}年\\d{1,2}月");
            if(re.match(s).hasMatch())
                Labels.removeOne(s);
        }
        if(Labels.size() != 0)
            GetLabelAnime(stringVectorToString(Labels, "、") , ui->LabelCount->value());
        else
            messageBoxWindow("标签栏没有标签");
        return ;
    });


    //  绑定RadioButton
    connect(ui->LoadSeasonAnime , &QRadioButton::clicked , [&](){
        ui->LocalSeason->setText(QDate::fromString(LocalSeason, "yyyy年M月")
            .addMonths(ui->LoadSeasonAnimeInterval->text().toInt() * 3)
            .toString("yyyy年M月"));

        QString season = QDate::fromString(LocalSeason, "yyyy年M月")
                .addMonths(ui->LoadSeasonAnimeInterval->text().toInt() * 3)
                .toString("yyyy年M月");
        LoadAnimeWithSeason(season);
    });

    connect(ui->AnimeProducer , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeProducer->text();
        LoadAnimeWithProducer(Name, 1);
    });

    connect(ui->Supervisor , &QPushButton::clicked , [&](){
        QString Name = ui->Supervisor->text();
        LoadAnimeWithProducer(Name, 2);
    });

    connect(ui->OpenBangumiLink , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeDetailName->text();
        QString BangumiLink;
        for(auto i : tempAnimeTable){
            if(i.Name == Name){
                BangumiLink = i.BangumiLink;
                if(!BangumiLink.isEmpty())
                    QDesktopServices::openUrl(QUrl(BangumiLink));
            }

        }

    });

    connect(ui->FollowingThisAnime , &QRadioButton::clicked , [&](){
        QString Name = ui->AnimeDetailName->text();
        int row = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->data(Qt::UserRole).toInt();
        QString BangumiLink = tempAnimeTable[row].BangumiLink;
        if(ui->FollowingThisAnime->isChecked()){
            for(auto &i : LocalData){
                if(i.contains(Name)){
                    if(!getHash(i, Name, "已删除").isEmpty()){
                        i[Name].remove("已删除");
                        UpdateAnimeMangerMap(Name);
                    }
                }
            }
            UpdateDataFromBangumi(Name, BangumiLink);
            UpdateAnimeMangerMap(Name);

        }
        else{
            SigFullDelete(Name);
        }

    });

    connect(ui->UpdateFromBangumi , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeDetailName->text();
        int row = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->data(Qt::UserRole).toInt();
        QString BangumiLink = tempAnimeTable[row].BangumiLink;
        UpdateDataFromBangumi(Name, BangumiLink);
        UpdateAnimeMangerMap(Name);

    });

    connect(ui->AllAnime , &QRadioButton::clicked , [&](){
        GetAllAnime();
    });

    connect(ui->RecentUpdate , &QRadioButton::clicked , [&](){
        GetRecentUpdate();
    });

    connect(ui->DiscardAnime , &QCheckBox::clicked , [&](){
        GetDiscardAnime();
    });

    connect(ui->DiscardAnime2 , &QCheckBox::clicked , [&](){
        GetDiscardAnime2();
    });

    connect(ui->AnimeMovie , &QCheckBox::clicked , [&](){
        GetAnimeMovie();
    });

    connect(ui->LocalSeasonAnime , &QRadioButton::clicked , [&](){
        GetLocalSeasonAnime();
    });

    connect(ui->ShelvedAnime , &QRadioButton::clicked , [&](){
        GetShelvedAnime();
    });

    connect(ui->WatchedAnime , &QRadioButton::clicked , [&](){
        GetWatchedAnime();
    });

    connect(ui->WatchingAnime , &QRadioButton::clicked , [&](){
        GetWatchingAnime();
    });

    connect(ui->WatchingLastAnime , &QRadioButton::clicked , [&](){
        GetWatchingLastAnime();
    });

    connect(ui->CopyName , &QPushButton::clicked , [&](){
        SetCopyName(false);
    });

    connect(ui->Initialization , &QPushButton::clicked , [&](){
        for (auto i = LocalData.begin(); i!= LocalData.end(); ++i) {
            saveData(dataFilesSaveUrl[i.key()], i.value());
        }
        for (auto i = RankData.begin(); i!= RankData.end(); ++i) {
            saveData(dataFilesSaveUrl[i.key()], i.value());
        }
        for (auto i = OnlineStreamData.begin(); i!= OnlineStreamData.end(); ++i) {
            saveData(dataFilesSaveUrl[i.key()], i.value());
        }
        IniData();
        IniCalendarWidget();
    });

//  本地路径
    connect(ui->OpenLocalUrl , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QProcess *proc = new QProcess(this);
        qDebug() << AnimeMangerMap[Name].localFilePath;
        if(AnimeMangerMap[Name].hasLocalFile){
            proc->start(AnimeMangerMap[Name].localFilePath);
            proc->startDetached("explorer", QStringList() << AnimeMangerMap[Name].localFilePath);
        }
        else if(ui->CreateNewFile->isChecked()){
            CreateNewLocalUrl(Name);
        }
    });

    connect(ui->backupFile , &QPushButton::clicked , [&](){
        QString origPath = "JsonFiles\\LocalData";
        QString timestamp = QDateTime::currentDateTime().toString("yyyy年MM月dd日_HH-mm-ss");
        QString backupPath = "backup/" + timestamp;
        QDir backupDir;
        if (!backupDir.mkpath(backupPath)) {
            qDebug() << "Failed to create backup directory:" << backupPath;
            return;
        }
        QFileInfoList saveFiles = getFiles(origPath, 1, {"*.json"});
        QString backupList = "", failList = "";
        for (const QFileInfo &fileInfo : saveFiles) {
            QString srcFilePath = fileInfo.absoluteFilePath();
            QString destFilePath = backupPath + "/" + fileInfo.fileName();
            if (QFile::copy(srcFilePath, destFilePath)) {
                backupList += "\n" + fileInfo.fileName();
            } else {
                failList += "\n" + fileInfo.fileName();
            }
        }
        messageBoxWindow("已备份数据: " + backupList);
        if(!failList.isEmpty())
            messageBoxWindow("备份失败: " + failList);
    });

//  绑定滑块，显示滑块数值
    connect(ui->StoryScore , &QSlider::valueChanged , [&](){
        ui->StoryScoreLabel->setText(QString::number(ui->StoryScore->value()));
    });

    connect(ui->ScreenScore , &QSlider::valueChanged , [&](){
        ui->ScreenScoreLabel->setText(QString::number(ui->ScreenScore->value()));
    });

    connect(ui->RoleScore , &QSlider::valueChanged , [&](){
        ui->RoleScoreLabel->setText(QString::number(ui->RoleScore->value()));
    });

//  添加新词条
    connect(ui->AddNew, &QPushButton::clicked, [&](){
        QString Name = ui->AnimeName->toPlainText(), Time;

        if(ui->UseLocalTime->isChecked())
            Time = ui->dataToday->dateTime().toString("yyyy.MM.dd");
        else
            Time = getLocalTime();
        QString Story = QString::number(ui->StoryScore->value());
        QString Screen = QString::number(ui->ScreenScore->value());
        QString Role = QString::number(ui->RoleScore->value());
        QString episode = QString::number(ui->Episode->value());

        int currentRow = ui->tableWidget->currentRow();
        if (currentRow >= 0) {
            QTableWidgetItem *item = ui->tableWidget->item(currentRow, 0);
            if (item != nullptr) {
                QVariant userData = item->data(Qt::UserRole);
                if (userData.isValid() && userData.canConvert<int>() && userData.toInt() >= 0) {
                    int row = userData.toInt();
                    QString BangumiLink = "";
                    if(tempAnimeTable.size() > row)
                        BangumiLink= tempAnimeTable[row].BangumiLink;
                    if(!BangumiLink.isEmpty()){
                        QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
                        fastLinkBangumiToAll[BangumiLink]["AnimeMangerMap"] = {Name};
                        LocalData[addKey][Name]["Bangumi链接"] = {BangumiLink};
                    }
                }
            }
        }

        if(ui->SumRank->isChecked())
            episode = "完结得分";
        QHash<QString, QVector<QString> > addScore;
        addScore.insert(episode, QVector<QString>{Story, Screen, Role, Time});
        AddNewScore(Name, addScore);
    });

//  添加标签
    connect(ui->AddLabel , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QString InputLabel = ui->AnimeLabel->toPlainText();
        AddNewLabel(Name , InputLabel);
    });

//  添加检索用名
    connect(ui->AddAnimeNameForFind , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QString NameForFind = ui->AnimeNameForFind->toPlainText();
        AddUrlOrName(Name , NameForFind , 1);
    });

//  添加检索路径
    connect(ui->AddAnimeSearchUrl , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QString AnimeSearchUrl = ui->AnimeSearchUrl->toPlainText();
        AddUrlOrName(Name , AnimeSearchUrl , 2);
    });

//  添加别名
    connect(ui->AddAlias , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QString AliasName = ui->AnimeAlias->toPlainText();
        AddUrlOrName(Name , AliasName , 3);
    });

//  添加总集数
    connect(ui->AddTotalEpisode , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
        if(LocalData[addKey].contains(Name)){
            LocalData[addKey][Name]["集数"] = {QString::number(ui->TotalEpisode->value())};
            UpdateAnimeMangerMap(Name);
        }
    });

//  添加首播日期
    connect(ui->AddStartUpdateTime , &QPushButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
        if(LocalData[addKey].contains(Name)){
            LocalData[addKey][Name]["首播"] = {ui->StartUpdateTime->date().toString("yyyy年M月d日")};
            UpdateAnimeMangerMap(Name);
        }
    });

//  修改权重
    connect(ui->Rstory , &myLabel::textChanged , [&](){
        Rstory = ui->Rstory->text().toInt();
        UpdateAnimeMangerMap();
    });

    connect(ui->Rscreen , &myLabel::textChanged , [&](){
        Rscreen = ui->Rscreen->text().toInt();
        UpdateAnimeMangerMap();
    });

    connect(ui->Rrole , &myLabel::textChanged , [&](){
        Rrole = ui->Rrole->text().toInt();
        UpdateAnimeMangerMap();
    });

//  修改显示分段
    connect(ui->minScore , &myLabel::textChanged , [&](){
        UpdateAnimeMangerMap();
    });

    connect(ui->maxScore , &myLabel::textChanged , [&](){
        UpdateAnimeMangerMap();
    });

//  删除整个词条
    connect(ui->FullDelete , &QPushButton::clicked, [&](){
        QString Name = ui->AnimeName->toPlainText();
        SigFullDelete(Name);
        UpdateAnimeMangerMap(Name);
    });

//  tablewidget 单击
    connect(ui->tableWidget , &QTableWidget::itemClicked , [&](){
        QString Name = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text();
        TableWidgetClick();
    });
//  tablewidget 双击
    connect(ui->tableWidget , &QTableWidget::itemDoubleClicked , [&](){
//        if(TableType == 1){
            TableWidgetClick();
            QString Name = ui->AnimeName->toPlainText();
            int episode = 1;
            if(AnimeMangerMap.contains(Name))
                episode += AnimeMangerMap[Name].currentEpisode;
            bool isLocalFileOpened = false;
            if(ui->OpenLocalFile->isChecked()){
                isLocalFileOpened = OpenLocalFile(Name, episode);
                if (!isLocalFileOpened) {
                    isLocalFileOpened = OpenWebUrl(Name, episode);
                }
            }
            else
                isLocalFileOpened = OpenWebUrl(Name, episode);
//        }
    });

//  HistoryViewDetail 单击
    connect(ui->HistoryViewDetail , &QTableWidget::itemClicked , [&](){
        QString Name = ui->HistoryViewDetail->item(ui->HistoryViewDetail->currentRow(),0)->text();
        QStringList list = Name.split(" ");
        if(list.size() <= 2)
            Name = list[0];
        else{
            list.removeLast();
            Name.clear();
            for(auto i : list)
                Name += i + " ";
            Name.remove(Name.size() - 1 , 1);
        }
        ui->AnimeName->setText(Name);
        GetFindAnime(ui->AnimeName->toPlainText());
    });
//  添加详细观看日志 calendarWidget & HistoryViewDetail
    connect(ui->calendarWidget , &QCalendarWidget::clicked , [&](){
        UpdateHistoryView();
    });
    connect(ui->calendarDay , &QRadioButton::clicked , [&](){
        UpdateHistoryView();
    });
    connect(ui->calendarWeek , &QRadioButton::clicked , [&](){
        UpdateHistoryView();
    });
    connect(ui->calendarMonth , &QRadioButton::clicked , [&](){
        UpdateHistoryView();
    });
//  HistoryViewDetailInput 是否显示全部集数
    connect(ui->displayAllEpisode , &QRadioButton::clicked , [&](){
        QString Name = ui->AnimeName->toPlainText();
        UpdateHistoryViewDetailInput(Name);
    });

////  载入指定番剧
//    connect(ui->StartFollowing , &QPushButton::clicked , [&](){
//        QVector<QString> followingLabel;
//        if(ui->FollowingTheseLabel->isChecked()){
//            followingLabel.append(ui->FollowingLabel->toPlainText().split("、").toVector());
//        }
//        QString loadSeason = ui->LoadSeason->text();
//        if(ui->FollowingSeasonAnime->isChecked()){
//            followingLabel.push_back(loadSeason);
//        }
//        if(!followingLabel.isEmpty())
//            LoadAnimeWithLabel(followingLabel, ui->VotesCount->value(), ui->TotalEpisodeCount->value());

//        QString followingName;
//        if(ui->FollowingTheseName->isChecked()){
//            followingName = ui->FollowingWithName->toPlainText();
//            LoadAnimeWithName(followingName, ui->VotesCount->value(), ui->TotalEpisodeCount->value());
//        }

//    });


//  打开动漫QListWidget 单击
    connect(ui->EpisodeListWidget, &QListWidget::itemClicked, [&](){
        QString Name = ui->AnimeName->toPlainText();
        int episode = ui->EpisodeListWidget->currentItem()->text().toInt();
        bool isLocalFileOpened = false;
        if(ui->OpenLocalFile->isChecked()){
            isLocalFileOpened = OpenLocalFile(Name, episode);
            if (!isLocalFileOpened) {
                isLocalFileOpened = OpenWebUrl(Name, episode);
            }
        }
        else
            isLocalFileOpened = OpenWebUrl(Name, episode);
    });

//  HistoryViewDetailInput 设置右键删除
    connect(ui->HistoryViewDetailInput, &QTableWidget::customContextMenuRequested,
            this, &AnimeNestWindow::RightClickDetailWidget);
//  listWidget 设置右键删除
    connect(ui->listWidget, &QListWidget::customContextMenuRequested,
            this, &AnimeNestWindow::RightClickLabelWidget);

    connect(ui->AnimeDataStartDisplay , &QPushButton::clicked , [&](){
        QHash<QString,QHash<QString,QVector<QString>>> displayData, matchResult;
        if(ui->DisplayRecycleBin->isChecked()){
            for(auto i : LocalData){
                for(auto j = i.begin() ; j != i.end() ; ++j){
                    if(getHash(j.value(), "已删除", 0) == "1")
                        displayData.insert(j.key(), j.value());
                }
            }
        }
//        else{
//            if(ui->DisplayAnimeNestData->isChecked()){
//                dataMatch(displayData, AnimeJsonMap, displayData, matchResult);
//            }
//            if(ui->DisplayAnimeFetchedData->isChecked()){
//                dataMatch(displayData, AnimeFetchedBangumiData, displayData, matchResult);
//                dataMatch(displayData, AnimeFetchedGirigiriloveData, displayData, matchResult);
//            }
//        }

        QHash<QString,QHash<QString,QVector<QString>>> tempJsonMap;

        for(auto i = displayData.begin(); i != displayData.end() ; ++i){
            for(auto j = i.value().begin(); j != i.value().end() ; ++j){
                QVector<QString> nodisplayTitle;
                if(ui->NotDisplayTheseTitle->isChecked()){
                    nodisplayTitle = ui->NotDisplayTitles->toPlainText().split("、").toVector();
                }
                if((!ui->NotDisplayNumberTitle->isChecked() || (j.key().toInt() == 0 && j.key() != "0")) &&
                        (!ui->NotDisplayTheseTitle->isChecked() || !nodisplayTitle.contains(j.key())))
                    tempJsonMap[i.key()][j.key()] = displayData[i.key()][j.key()];
            }
        }

        QVector<QString> allnamesFetched, repeatNameFetched;
        for(auto i : tempJsonMap){
            for(auto j : i["番名"]){
                if(!allnamesFetched.contains(j))
                    allnamesFetched.push_back(j);
                else
                    repeatNameFetched.push_back(j);
            }
        }
        qDebug() << repeatNameFetched;
        ui->TotalMatched->setText(QString::number(tempJsonMap.size()));
        dataDisplay(tempJsonMap, ui->AnimeDataDisplay);
    });

    connect(ui->DisplayHighlight, &QPushButton::clicked, [&](){
        QString checkString = ui->DisplayKeyWord->toPlainText();
        QTreeWidgetItemIterator it(ui->AnimeDataDisplay);
        while (*it) {
            (*it)->setBackground(0, QBrush());
            if ((*it)->text(0).contains(checkString)) {
                ui->AnimeDataDisplay->setCurrentItem(*it);
                (*it)->setBackground(0, Qt::yellow); // 设置背景色为黄色
            }
            ++it;
        }
    });

    QStringList extTag = {"详细信息", "日程表", "高级筛选", "数据查询", "Debug"};
    ui->ToolSelect->addItems(extTag);
    ui->DetailData->setCurrentIndex(ui->ToolSelect->currentIndex());
    connect(ui->ToolSelect, &QComboBox::currentTextChanged, [&](){
        int newIndex = ui->ToolSelect->currentIndex();
        ui->DetailData->setCurrentIndex(newIndex);
    });

    connect(ui->ToolExtention , &QPushButton::clicked , [&](){
        bool isVisible = !ui->DetailData->isVisible(); // 获取当前可见性状态

        // 设置控件的可见性
        ui->widget_12->setVisible(isVisible);
        ui->DetailData->setVisible(isVisible);
        ui->ToolSelect->setVisible(isVisible);
        ui->ToolExtention->setText(isVisible ? "<\n<\n<" : ">\n>\n>");
        if(!isVisible){
            this->setMinimumWidth(this->minimumWidth()-400);
            resize(QMainWindow::size().width()-400 , QMainWindow::size().height());
        }
        else{
            resize(QMainWindow::size().width()+400 , QMainWindow::size().height());
            this->setMinimumWidth(this->minimumWidth()+400);
        }
    });




    connect(ui->SeasonCountCheck , &QPushButton::clicked , [&](){
        QVector<QString> fetchseason = {ui->FetchSeason->text()};
        int BangumiCount = 0, girigiriloveCount = 0, AnilistCount = 0, IyxdmCount = 0, SakuraCount = 0;
        for(auto i = RankData.begin() ; i != RankData.end() ; ++i){
            for(auto j : i.value()){
                if(vectorMatch( getHash(j, "更新年月"), fetchseason)){
                    if(i.key().contains("Bangumi"))
                        BangumiCount++;
                    if(i.key().contains("Anilist"))
                        AnilistCount++;
                }
            }
        }
        for(auto i = OnlineStreamData.begin() ; i != OnlineStreamData.end() ; ++i){
            for(auto j : i.value()){
                if(vectorMatch( getHash(j, "更新年月"), fetchseason)){
                    if(i.key().contains("girigirilove"))
                        girigiriloveCount++;
                    if(i.key().contains("Iyxdm"))
                        IyxdmCount++;
                    if(i.key().contains("Sakura"))
                        SakuraCount++;
                }
            }
        }
        ui->FetchBangumiBySeasonCount->setText(QString::number(BangumiCount));
        ui->FetchAnilistBySeasonCount->setText(QString::number(AnilistCount));
        ui->FetchGirigiriloveBySeasonCount->setText(QString::number(girigiriloveCount));
        ui->FetchIyxdmBySeasonCount->setText(QString::number(IyxdmCount));
        ui->FetchSakuraBySeasonCount->setText(QString::number(SakuraCount));
    });

}

AnimeNestWindow::~AnimeNestWindow()
{
    delete ui;
}

