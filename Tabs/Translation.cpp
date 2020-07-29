#include "Translation.h"
#include "ui_mainwindow.h"

Translation::Translation(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent)
  , ui(ui)
{

}

void Translation::on_pushButtonLoad_clicked()
{
    QVector<QPair<QString, QString> > vecPair;
    Content::getVectorVersesAllBible(&vecPair, Content::ProviderContent::GETBIBLE_NET);
    Q_ASSERT(vecPair.size() > 0);
    QStandardItemModel *model = new QStandardItemModel(ui->listViewTranslation);
    for (const QPair<QString, QString> pair: vecPair) {
        QStandardItem *item = new QStandardItem(pair.first);
        item->setData(pair.second);
        model->appendRow(item);
    }
    ui->listViewTranslation->setModel(model);
    connect(ui->listViewTranslation, &QListView::clicked, this, [this](){
        ui->textEditVerse->setText(ui->listViewTranslation->currentIndex().data(Qt::UserRole+1).toString());
    });

    qDebug() << "finish" << Qt::endl;
}
