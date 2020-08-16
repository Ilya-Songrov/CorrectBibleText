#pragma once

#include <QDialog>

namespace Ui {
class DialogMergeCorrectFile;
}

class DialogMergeCorrectFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMergeCorrectFile(QWidget *parent = nullptr);
    ~DialogMergeCorrectFile();

    void setCorrectFilePath (const QString &file);
    void setAllBibleFile    (const QString &file);
    QString getCorrectFilePath();
    QString getAllBibleFile();
private:
    Ui::DialogMergeCorrectFile *ui;
};

