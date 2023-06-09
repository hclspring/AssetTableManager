#ifndef EXPORTBOOKDIALOG_H
#define EXPORTBOOKDIALOG_H

#include <QWidget>
#include <QDialog>

class Config;

QT_BEGIN_NAMESPACE
namespace Ui { class ExportBookDialog; }
QT_END_NAMESPACE

class ExportBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportBookDialog(Config* config, QWidget *parent = nullptr);
    ~ExportBookDialog();

private:
    Ui::ExportBookDialog *ui;
    QString filePath;
    QString outputStyle;

public:
    QString get_filePath();
    QString get_outputStyle();

private slots:
    void on_browseButton_clicked();
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();


};


#endif // EXPORTBOOKDIALOG_H
