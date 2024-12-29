#include "animenestwindow.h"
#include "ui_animenestwindow.h"


//  退出事件，退出时保存数据
void AnimeNestWindow::closeEvent(QCloseEvent*){
//    saveData("AnimeNestData.json", AnimeJsonMap);
//    saveData("BangumiData.json", AnimeFetchedBangumiData);
//    saveData("GirigiriloveData.json", AnimeFetchedGirigiriloveData);
//    saveData("IyxdmData.json", AnimeFetchedIyxdmData);
//    saveData("AnilistData.json", AnimeFetchedAnilistData);

    for (auto i = LocalData.begin(); i!= LocalData.end(); ++i) {
        saveData(dataFilesSaveUrl[i.key()], i.value());
    }
    for (auto i = RankData.begin(); i!= RankData.end(); ++i) {
        saveData(dataFilesSaveUrl[i.key()], i.value());
    }
    for (auto i = OnlineStreamData.begin(); i!= OnlineStreamData.end(); ++i) {
        saveData(dataFilesSaveUrl[i.key()], i.value());
    }

    saveData("config.json", Setting);
}

//  回车事件, 输入窗口为AnimeName时 按回车触发事件
bool AnimeNestWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->AnimeName && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast <QKeyEvent *>(event);
        if(keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
        {
            ui->settingWidget->setCurrentIndex(1);
            GetFindAnime(ui->AnimeName->toPlainText());
//            emit ui->FindItem->click();
            return true;
        }
    }
    if(obj == ui->FindLabel && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast <QKeyEvent *>(event);
        if(keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
        {
            emit ui->FindAnimeInLabel->click();
            return true;
        }
    }
    if(obj == ui->AnimeLabel && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast <QKeyEvent *>(event);
        if(keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
        {
            emit ui->AddLabel->click();
            return true;
        }
    }
//    if(obj == ui->keyWord && event->type() == QEvent::KeyPress)
//    {
//        QKeyEvent *keyEvent = static_cast <QKeyEvent *>(event);
//        if(keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
//        {
//            emit ui->FindWithKeyWord->click();
//            return true;
//        }
//    }

//    if(obj == ui->tableWidget && event->type() == QEvent::MouseMove) {
//        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//        int row = ui->tableWidget->rowAt(mouseEvent->pos().y()) - 1;
//        if(row == -2)
//            return QObject::eventFilter(obj, event);
//        QString Name = ui->tableWidget->item(row,0)->text();
//        qDebug() << row << Name;
//        for (int i = 0; i < ui->listWidget->count(); i++) {
//            QListWidgetItem *item = ui->listWidget->item(i);
//            QString Label = item->text();
//            int splitIndex = Label.indexOf("/");
//            Label = Label.left(splitIndex);
//            bool iscontains = false;
//            for(QString s : AllHash[Name]["标签"]){
//                if(s.contains(Label)){
//                    iscontains = true;
//                    item->setBackground(QColor(144, 238, 144));
//                    break;
//                }
//            }
//            if(!iscontains)
//                item->setBackground(Qt::transparent);
//        }
//    }
    return QObject::eventFilter(obj, event);
}
