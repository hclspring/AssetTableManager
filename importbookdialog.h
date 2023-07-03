#ifndef IMPORTBOOKDIALOG_H
#define IMPORTBOOKDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QList>

class Config;

QT_BEGIN_NAMESPACE
namespace Ui { class ImportBookDialog; }
QT_END_NAMESPACE

class ImportBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportBookDialog(Config* config, QWidget *parent = nullptr);
    ~ImportBookDialog();

private:
    Ui::ImportBookDialog *ui;
    QString filePath;

public:
    QString get_filePath();

private slots:
    void on_browseButton_clicked();
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    //QVector<QString> read_book_names(const QString& bookRootPath);

private slots:
    void test();


};

#endif // IMPORTBOOKDIALOG_H
