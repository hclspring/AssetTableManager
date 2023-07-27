#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>

class Config;
class DataTable;
class QTextBrowser;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Config* config = nullptr;
    DataTable* dataTable = nullptr;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_importBookButton_clicked();
    void on_updateBookButton_clicked();
    void on_exportBookButton_clicked();

private:
    Ui::MainWindow *ui;

private:
    void update_bookFilePathNameTextBrowser(const QString& text);
    //void update_inputStyleTextBrowser(const QString& text);
    void update_updateFilePathNameTextBrowser(const QString& text);
    //void update_updateMappingTextBrowser(const QString& text);
    void update_updatePrimaryKeyTextBrowser(const QString& text);
    void append_updateProcessTextBrowser(const QString& text);
    void update_exportInfoTextBrowser(const QString& text);

    void updateTextBrowserContent(const QString& text, QTextBrowser* qtb);
    void appendTextBrowserContent(const QString& newText, QTextBrowser* qtb);

};
#endif // MAINWINDOW_H
