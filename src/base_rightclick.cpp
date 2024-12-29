#include "animenestwindow.h"
#include "ui_animenestwindow.h"

void AnimeNestWindow::RightClickDetailWidget(const QPoint &pos)
{
    // Get the item under the cursor
    QTableWidget* tableWidget = qobject_cast<QTableWidget*>(sender());
    QTableWidgetItem *item = ui->HistoryViewDetailInput->itemAt(pos);

    // Check if the item exists
    if (!item) {
        return;
    }

// Create a context menu
    QMenu *menu = new QMenu(this);
    QAction *action = new QAction("删除", this);

    // Connect the action to a slot
    connect(action, &QAction::triggered, this, [=](){
        // Do something with the selected item
        qDebug() << "Selected item: " << item->text();
        QString Epsi = this->ui->HistoryViewDetailInput->item(item->row() , 0)->text();
        AnimeNestWindow::DeleteSigEpisode(this->ui->AnimeName->toPlainText() , Epsi);
    });

    // Add the action to the context menu
    menu->addAction(action);

    // Show the context menu at the cursor position
    menu->popup(tableWidget->viewport()->mapToGlobal(pos));
}


void AnimeNestWindow::RightClickLabelWidget(const QPoint &pos){
    // Get the item under the cursor
    QListWidget* listWidget = qobject_cast<QListWidget*>(sender());
    QListWidgetItem *item = ui->listWidget->itemAt(pos);

    // Check if the item exists
    if (!item) {
        return;
    }

// Create a context menu
    QMenu *menu = new QMenu(this);
    QAction *action = new QAction("删除", this);

    // Connect the action to a slot
    connect(action, &QAction::triggered, this, [=](){
        // Do something with the selected item
        qDebug() << "Selected item: " << item->text();
        QString Name = ui->AnimeName->toPlainText();
        QString Label = item->text();
        AnimeNestWindow::DeleteLabel(Name , Label);
    });

    // Add the action to the context menu
    menu->addAction(action);

    // Show the context menu at the cursor position
    menu->popup(listWidget->viewport()->mapToGlobal(pos));
}
