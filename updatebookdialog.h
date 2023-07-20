#ifndef UPDATEBOOKDIALOG_H
#define UPDATEBOOKDIALOG_H

#include <QDialog>
#include <QFileDialog>

class Config;

QT_BEGIN_NAMESPACE
namespace Ui {class UpdateBookDialog;}
QT_END_NAMESPACE


class UpdateBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateBookDialog(Config* config, QWidget *parent = nullptr);
    ~UpdateBookDialog();

private:
    Ui::UpdateBookDialog *ui;
    Config *config;
    QString filePath;
    QString mappingName;
    QString primaryKeyStr;

public:
    QString get_filePath();
    QString get_mappingName();
    QString get_primaryKeyStr();

private slots:
    void on_browseButton_clicked();
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();
    void on_mappingComboBox_currentTextChanged();
};

#endif // UPDATEBOOKDIALOG_H
