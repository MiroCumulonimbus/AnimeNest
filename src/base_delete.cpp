#include "animenestwindow.h"
#include "ui_animenestwindow.h"

void AnimeNestWindow::DeleteSigEpisode(QString Name , QString Episode){
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";
    if (!LocalData[addKey].contains(Name)) {
        bool isOtherData = false;
        for(auto i : LocalData){
            if(i.contains(Name))
                isOtherData = true;
        }
        if(isOtherData)
            messageBoxWindow("非本地数据, 无法删除: " + Name);
        else
            messageBoxWindow("未找到: " + Name);
        return;
    }

    QMessageBox::StandardButton reply;
    QString hintMessage = "";
    if(Episode.toInt() < 1000)
        hintMessage = "将要删除: " + Name + "\n第" + Episode + "集";
    else
        hintMessage = "将要删除: " + Name + "\n第" + QString::number(Episode.toInt() - 1000) + "次总分记录";
    reply = QMessageBox::question(nullptr, "Message",  hintMessage,
        QMessageBox::Yes | QMessageBox::No);

    // 根据用户的响应作出相应的操作
    if (reply == QMessageBox::Yes) {
        int epis = Episode.toInt();
        if(epis > 1000){
            epis -= 1001;
            Episode = "完结得分";
        }
        if(!LocalData[addKey][Name].contains(Episode)){
            messageBoxWindow("未找到要删除的集数：" + Episode);
            return;
        }
        if(Episode == "完结得分"){
            epis *= 4;
            LocalData[addKey][Name]["完结得分"].removeAt(epis);
            LocalData[addKey][Name]["完结得分"].removeAt(epis);
            LocalData[addKey][Name]["完结得分"].removeAt(epis);
            LocalData[addKey][Name]["完结得分"].removeAt(epis);
            LocalData[addKey][Name]["完结得分"].squeeze();
            if(LocalData[addKey][Name]["完结得分"].size() == 0){
                qDebug() << "AnimeJsonMap[Name][完结得分].size() " <<LocalData[addKey][Name]["完结得分"].size();
                LocalData[addKey][Name].remove("完结得分");
            }
        }
        else
            LocalData[addKey][Name].remove(Episode);
//        qDebug() << AllHash[Name];
        UpdateAnimeMangerMap(Name);
        UpdateDetailInfo(Name, AnimeMangerMap[Name].thisBangumiLink);
        UpdateHistoryViewDetailInput(Name);
    } else {
        messageBoxWindow("取消了操作");
    }
}

void AnimeNestWindow::DeleteLabel(QString Name , QString Label){
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";

    if (!LocalData[addKey].contains(Name)) {
        bool isOtherData = false;
        for(auto i : LocalData){
            if(i.contains(Name))
                isOtherData = true;
        }
        if(isOtherData)
            messageBoxWindow("非本地数据, 无法删除: " + Name);
        else
            messageBoxWindow("未找到: " + Name);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Message", "删除 " + Name + " 的标签:" + Label ,
        QMessageBox::Yes | QMessageBox::No);

    // 根据用户的响应作出相应的操作
    if (reply == QMessageBox::Yes) {
        if(LocalData[addKey][Name]["标签"].contains(Label)){
            LocalData[addKey][Name]["标签"].removeOne(Label);
            UpdateAnimeMangerMap(Name);

            QList<QListWidgetItem*> items = ui->listWidget->findItems(Label, Qt::MatchExactly);

            // 移除这些项
            for (int i = 0; i < items.size(); ++i) {
                ui->listWidget->takeItem(ui->listWidget->row(items.at(i)));
                delete items.at(i);
            }

//            ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
        }
        UpdateAnimeMangerMap(Name);
    } else {
        messageBoxWindow("取消了操作");
    }
}

void AnimeNestWindow::SigFullDelete(QString Name){
    QString addKey = LocalData.size() == 1 ? LocalData.begin().key() : "AnimeNest";

    if (!LocalData[addKey].contains(Name)) {
        bool isOtherData = false;
        for(auto i : LocalData){
            if(i.contains(Name))
                isOtherData = true;
        }
        if(isOtherData)
            messageBoxWindow("非本地数据, 无法删除: " + Name);
        else
            messageBoxWindow("未找到: " + Name);
        return;
    }
    LocalData[addKey][Name]["已删除"] = {"1"};
    UpdateAnimeMangerMap(Name);
//    messageBoxWindow("已取消追番：" + Name);

//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(nullptr, "Message", "将要完全删除:" + Name ,
//        QMessageBox::Yes | QMessageBox::No);

//    // 根据用户的响应作出相应的操作
//    if (reply == QMessageBox::Yes) {

////        if(!Setting["系统路径"]["备份路径"].empty() && Setting["系统路径"]["备份路径"][0] != "")
////            BasebackupFolder(sourceFilePath , Setting["系统路径"]["备份路径"][0]);
////        else{
////            messageBoxWindow("未设置备份路径，删除失败");
////            return;
////        }
////        QHash<QString,QHash<QString,QVector<QString>>>::iterator it = AllHash.begin();
////        for(; it != AllHash.end() ; it++){
////            if(it.key() == Name)
////                AllHash.erase(it);
////        }
////        QHash<QString, MyAnimeLabel>::iterator labelit = AllMyAnimeLabel.begin();
////        for(; labelit != AllMyAnimeLabel.end() ; ++labelit){
////            if(labelit.key() == Name)
////                AllMyAnimeLabel.erase(labelit);
////        }
//    } else {
//        messageBoxWindow("取消了操作");
//    }
}
