#ifndef IMPORTFILEDIALOG_H
#define IMPORTFILEDIALOG_H

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ImportFileDialog; }
QT_END_NAMESPACE

class ImportFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportFileDialog(QWidget *parent = nullptr);
    ~ImportFileDialog();

private:
    Ui::ImportFileDialog *ui;
    QString filePath;

public:

private slots:
    void on_browseButton_clicked();
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();


};

#endif // IMPORTFILEDIALOG_H
