#include "Correct.h"
#include "ui_mainwindow.h"

Correct::Correct(Ui::MainWindow *ui, QObject *parent) :
    TabBase(parent)
  , ui(ui)
{
    setGeneralSettings();
}

void Correct::setGeneralSettings()
{
    connect(ui->pushButtonLoadCorrect, &QPushButton::clicked, this, &Correct::on_pushButtonLoadCorrect_clicked);
    connect(ui->pushButtonSaveCorrect, &QPushButton::clicked, this, &Correct::on_pushButtonSaveCorrect_clicked);
    connect(ui->listViewCorrect, &QListView::clicked, this, &Correct::onClickedViewCorrect);
    connect(ui->actionReset_Correct_File, &QAction::triggered, this, &Correct::on_actionReset_CorrectFile_triggered);
    connect(ui->actionMerge_Correct_File, &QAction::triggered, this, &Correct::on_actionMerge_CorrectFile_triggered);
    connect(ui->actionMerge_Correct_File, &QAction::triggered, this, &Correct::on_actionMerge_CorrectFile_triggered);
    ui->textEditCorrectVerse->setFontPointSize(18);
}

void Correct::on_pushButtonLoadCorrect_clicked()
{
#ifdef QT_DEBUG
    pathCorrectFile = "../CorrectBibleText/Resource/Analysis/CorrectBibleSynodal.json";
#else
    pathCorrectFile = QFileDialog::getOpenFileName();
#endif
    if (pathCorrectFile.isEmpty()) {
        return;
    }
    ui->lineEditLoadCorrect->setText(pathCorrectFile);
    updateListViewCorrect();
}

void Correct::on_pushButtonSaveCorrect_clicked()
{
    if (pathCorrectFile.isEmpty()) {
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
                                  newText, pathCorrectFile);
    ui->listViewCorrect->model()->removeRow(currentIndex.row());
    onClickedViewCorrect();
}

void Correct::onClickedViewCorrect()
{
    ui->textEditCorrectVerse->setText(ui->listViewCorrect->currentIndex().data(Qt::UserRole+1).toString());
}

void Correct::on_actionReset_CorrectFile_triggered()
{
    if (pathCorrectFile.isEmpty()) {
        QMessageBox::information(ui->listViewCorrect, "", "Please, load correct file");
        return;
    }
    CorrectWorker::resetCorrectFile(pathCorrectFile);
    updateListViewCorrect();
    QMessageBox::information(ui->listViewCorrect, "", QString("The %1 file is already reseted").arg(QFileInfo(pathCorrectFile).fileName()));
}

void Correct::on_actionMerge_CorrectFile_triggered()
{
    DialogMergeCorrectFile dialogMerge;
    dialogMerge.setCorrectFilePath(pathCorrectFile);
    dialogMerge.setAllBibleFile(CorrectWorker::getPathAllBible(pathCorrectFile));
    if (dialogMerge.exec() == QDialog::Rejected) {
        return;
    }
    pathCorrectFile = dialogMerge.getCorrectFilePath();
    const QString pathAllBibleFile = dialogMerge.getAllBibleFile();

    CorrectWorker::mergeCorrectFile(pathCorrectFile, pathAllBibleFile);
    QMessageBox::information(ui->listViewCorrect, "", QString("The %1 file is already merged").arg(QFileInfo(pathCorrectFile).fileName()));
}

void Correct::updateListViewCorrect()
{
    QVector<QPair<QString, QString> > vecPair;
    CorrectWorker::readCorrectFile(&vecPair, pathCorrectFile);
    Q_ASSERT(vecPair.size() > 0);

    QStandardItemModel *model = new QStandardItemModel(ui->listViewCorrect);
    for (const QPair<QString, QString> pair: vecPair) {
        QStandardItem *item = new QStandardItem(pair.first);
        item->setData(pair.second, Qt::UserRole+1);
        model->appendRow(item);
    }
    ui->listViewCorrect->setModel(model);
}

