#ifndef IMPORTBOOKDIALOG_H
#define IMPORTBOOKDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QList>

#include "xlsxdocument.h"

class Config;
class DataTable;

QT_BEGIN_NAMESPACE
namespace Ui { class ImportBookDialog; }
QT_END_NAMESPACE

class ImportBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportBookDialog(Config* config, bool needPrimaryKey = false, QWidget *parent = nullptr);
    ~ImportBookDialog();

private:
    Ui::ImportBookDialog *ui;
    Config *config;
    QString filePath;
    QString sheetName;
    QString primaryKey;
    int dataStartRow;
    int columnNameRow;

public:
    const QString &getFilePath() const;

    const QString &getSheetName() const;
    const QString &getPrimaryKey() const;
    int getDataStartRow() const;
    int getColumnNameRow() const;

private slots:
    void on_browseButton_clicked();
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();
    void on_primaryKeyComboBox_currentTextChanged();
    void on_sheetNameComboBox_currentTextChanged();
    void on_columnNameRowLineEdit_textChanged(const QString &arg1);
    void on_dataStartRowLineEdit_textChanged(const QString &arg1);

private:
    bool check_allInput_legal();
    bool check_sheetName_legal();
    bool check_primaryKey_legal();
    bool check_dataStartRow_legal();
    bool check_columnNameRow_legal();
    void set_confirmButton();

private slots:
    void test();

};

#endif // IMPORTBOOKDIALOG_H
