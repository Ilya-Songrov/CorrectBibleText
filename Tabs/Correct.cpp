#include "Correct.h"
#include "ui_mainwindow.h"

Correct::Correct(Ui::MainWindow *ui, QObject *parent) :
    TabBase(parent)
  , ui(ui)
{
    createConnects();
}

void Correct::createConnects()
{
    connect(ui->pushButtonLoadCorrect, &QPushButton::clicked, this, &Correct::on_pushButtonLoadCorrect_clicked);
    connect(ui->pushButtonSaveCorrect, &QPushButton::clicked, this, &Correct::on_pushButtonSaveCorrect_clicked);
    connect(ui->listViewCorrect, &QListView::clicked, this, &Correct::onClickedViewCorrect);
}

void Correct::on_pushButtonLoadCorrect_clicked()
{
#ifdef QT_DEBUG
    pathFileCorrect = "../CorrectBibleText/Resource/ResultAnalysis.json";
#else
    pathFileCorrect = QFileDialog::getOpenFileName();
#endif
    if (pathFileCorrect.isEmpty()) {
        return;
    }
    QVector<QPair<QString, QString> > vecPair;
    CorrectWorker::readFileCorrect(&vecPair, pathFileCorrect);
    Q_ASSERT(vecPair.size() > 0);

    QStandardItemModel *model = new QStandardItemModel(ui->listViewCorrect);
    for (const QPair<QString, QString> pair: vecPair) {
        QStandardItem *item = new QStandardItem(pair.first);
        item->setData(pair.second, Qt::UserRole+1);
        model->appendRow(item);
    }
    ui->listViewCorrect->setModel(model);
}

void Correct::on_pushButtonSaveCorrect_clicked()
{
    if (pathFileCorrect.isEmpty()) {
        QMessageBox::information(ui->listViewCorrect, "", "Nothing to save!");
        return;
    }
    QModelIndex currentIndex = ui->listViewCorrect->currentIndex();
    const QString newText = ui->textEditCorrectVerse->toPlainText().simplified();
    if (newText == currentIndex.data(Qt::UserRole+1).toString()) {
        QMessageBox::information(ui->listViewCorrect, "", "Please make your changes");
        return;
    }
    CorrectWorker::setCorrectText(currentIndex.data(Qt::DisplayRole).toString(),
                                  newText, pathFileCorrect);
    ui->listViewCorrect->model()->removeRow(currentIndex.row());
    onClickedViewCorrect();
}

void Correct::onClickedViewCorrect()
{
    ui->textEditCorrectVerse->setText(ui->listViewCorrect->currentIndex().data(Qt::UserRole+1).toString());
}

