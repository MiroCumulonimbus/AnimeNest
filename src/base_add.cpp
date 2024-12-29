#include "animenestwindow.h"
#include "ui_animenestwindow.h"

void AnimeNestWindow::AddNewScore(QString Name, QHash<QString, QVector<QString> > Info) {  // 添加标签
    ui->tableWidget->setSortingEnabled(false);

    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";

    LocalData[addKey][Name]["番名"].append(Name);

    QString messageHint;
    for(auto i = Info.begin() ; i != Info.end() ; ++i){
        if(LocalData[addKey][Name].contains(i.key())){
            if(i.key() == "完结得分"){
                LocalData[addKey][Name][i.key()].append(i.value());
            }
            else{
                if(LocalData[addKey][Name][i.key()] != i.value()){
                    messageHint += "已修改：" + Name + " 的集数：" + i.key() + "\n由：";
                    messageHint += stringVectorToString(LocalData[addKey][Name][i.key()], ", ") + "\n修改为：";
                    messageHint += stringVectorToString(i.value(), ", ") + "\n";
                    LocalData[addKey][Name][i.key()] = i.value();
                }
            }
        }
        else{
            LocalData[addKey][Name][i.key()] = i.value();
        }
    }
    if(messageHint != "")
        messageBoxWindow(messageHint);
    ui->SumRank->setChecked(false);
    UpdateAnimeMangerMap(Name);
    UpdateSplineChart(Name);

    ui->tableWidget->setSortingEnabled(true);

    IniCalendarWidget();
    UpdateDetailInfo(Name, AnimeMangerMap[Name].thisBangumiLink);
    UpdateHistoryViewDetailInput(Name);
}


void AnimeNestWindow::AddNewLabel(QString Name , QString Label) {  // 添加标签
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
    if (!LocalData[addKey].contains(Name)) {
        bool isOtherData = false;
        for(auto i : LocalData){
            if(i.contains(Name))
                isOtherData = true;
        }
        if(isOtherData)
            messageBoxWindow("非本地数据, 无法添加: " + Name);
        else
            messageBoxWindow("未找到: " + Name);
        return;
    }

    // 标签非空处理
    if (Label.size() > 0) {
        // 使用 QRegExp 按照分隔符分割标签，忽略空部分
        QStringList labelList = Label.split(QRegExp("[\\、,，\\\\]"), QString::SkipEmptyParts);
        for (const QString& label : labelList) {
            LocalData[addKey][Name]["标签"].push_back(label);
        }
        LocalData[addKey][Name]["标签"] = removeDuplicates(LocalData[addKey][Name]["标签"]);
        UpdateAnimeMangerMap(Name);
    }
    ui->listWidget->clear();
    for (const auto& label : LocalData[addKey][Name]["标签"]) {
        ui->listWidget->addItem(label);
    }
}


void AnimeNestWindow::AddUrlOrName(QString Name , QString Info , int index){
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";

    if (!LocalData[addKey].contains(Name)) {
        bool isOtherData = false;
        for(auto i : LocalData){
            if(i.contains(Name))
                isOtherData = true;
        }
        if(isOtherData)
            messageBoxWindow("非本地数据, 无法添加: " + Name);
        else
            messageBoxWindow("未找到: " + Name);
        return;
    }

    QVector<QString> AliasNameList = Info.split("\n").toVector();
    switch(index){
        case 1:{
            LocalData[addKey][Name]["检索用名"] = removeDuplicates(AliasNameList);
            if(LocalData[addKey][Name]["检索用名"].empty())
                LocalData[addKey][Name].remove("检索用名");
            LocalData[addKey][Name]["特殊下载添加集数"] = {QString::number(ui->SpecialDownloadAddEpisode->value())};
            break;
        }
        case 2:{
            LocalData[addKey][Name]["检索路径"] = removeDuplicates(AliasNameList);
            if(LocalData[addKey][Name]["检索路径"].empty())
                LocalData[addKey][Name].remove("检索路径");
            LocalData[addKey][Name]["特殊在线添加集数"] = {QString::number(ui->SpecialWebsiteAddEpisode->value())};
            break;
        }
        case 3:{
            if(!AliasNameList.isEmpty()){
                QString newKey = AliasNameList[0];
                if(newKey == ""){
                    messageBoxWindow("不允许更换一个 "" 空的名字");
                    return;
                }
                if(newKey != Name && AliasNameList.size() > 1){
                    if(LocalData[addKey].contains(newKey)){
                        messageBoxWindow("该番名已存在，无法更换：" + newKey);
                    } else {
                        QMessageBox::StandardButton reply;
                        QString message = QString("是否更换显示名为") + newKey + "？";
                        reply = QMessageBox::question(nullptr, "Message", message , QMessageBox::Yes | QMessageBox::No);

                        // 根据用户的响应作出相应的操作
                        if (reply == QMessageBox::Yes) {
                            LocalData[addKey][newKey] = LocalData[addKey][Name];
                            LocalData[addKey].remove(Name);
                            AnimeMangerMap.remove(Name);
                            Name = newKey;
                        }
                    }
                }
            } else {
                messageBoxWindow("别名列表为空，更换失败");
            }
            AliasNameList.push_front(Name);
            LocalData[addKey][Name]["番名"] = removeDuplicates(AliasNameList);
            break;
        }
    }
    UpdateAnimeMangerMap(Name);
}
