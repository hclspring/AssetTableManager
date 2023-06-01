#ifndef EXPORTFILEDIALOG_H
#define EXPORTFILEDIALOG_H

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ExportFileDialog; }
QT_END_NAMESPACE

class ExportFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportFileDialog(QWidget *parent = nullptr);
    ~ExportFileDialog();

private:
    Ui::ExportFileDialog *ui;
    QString fileDir;

public:

private slots:
    void on_browseButton_clicked();
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();


};


#endif // EXPORTFILEDIALOG_H
