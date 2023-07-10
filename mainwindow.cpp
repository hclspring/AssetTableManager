#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exportbookdialog.h"
#include "importbookdialog.h"
#include "updatebookdialog.h"
#include "config.h"
#include "datatable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    config = new Config;
    ui->setupUi(this);
    ui->bookFilePathNameTextBrowser->setText("");
    ui->bookNameTextBrowser->setText("");
    ui->exportBookButton->setDisabled(true);
    ui->updateBookButton->setDisabled(true);
    //connect(ui->importFileButton, SIGNAL(clicked()), this, SLOT(on_importFileButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete config;
    delete dataTable;
}

void MainWindow::on_importBookButton_clicked()
{
    ImportBookDialog * dialog = new ImportBookDialog(config);
    dialog->exec();
    if (dialog->get_filePath().length() > 0) {
        qDebug() << "读取台账文件：" << dialog->get_filePath();
        update_bookFilePathNameTextBrowser(dialog->get_filePath());
        QString bookName = config->get_bookName(ui->bookFilePathNameTextBrowser->toPlainText());
        update_bookNameTextBrowser(QString("台账类型：") + bookName);

        qDebug() << "开始读取台账文件……";
        if (dataTable != nullptr) delete dataTable;
        dataTable = new DataTable;
        dataTable->readExcelFile(ui->bookFilePathNameTextBrowser->toPlainText(),
                             config->get_ptr_mapS2F(bookName),
                             config->get_sheetIndex(bookName),
                             config->get_columnNameRow(bookName),
                             config->get_dataStartRow(bookName));
    }
    ui->updateBookButton->setDisabled(false);
    ui->exportBookButton->setDisabled(true);
    delete dialog;
}

void MainWindow::on_updateBookButton_clicked()
{
    UpdateBookDialog * dialog = new UpdateBookDialog(config);
    dialog->exec();
    if (dialog->get_filePath().length() > 0) {
        QString filePath = dialog->get_filePath();
        QString bookName = dialog->get_mappingName();
        QString primaryKeyStr = dialog->get_primaryKeyStr();
        qDebug() << "读取更新台账用文件：" << filePath;
        update_updateFilePathNameTextBrowser(filePath);
        update_updateMappingTextBrowser(QString("字段映射：") + bookName);

        qDebug() << "开始读取更新文件……";
        DataTable * updateTable = new DataTable;
        updateTable->readExcelFile(ui->updateFilePathNameTextBrowser->toPlainText(),
                             config->get_ptr_mapS2F(bookName),
                             config->get_sheetIndex(bookName),
                             config->get_columnNameRow(bookName),
                             config->get_dataStartRow(bookName));
        dataTable->updateWith(updateTable, config->get_field_type(bookName, primaryKeyStr), config->get_ptr_mapS2F(bookName));
        update_updateProcessTextBrowser("已按照文件" + filePath + "进行更新。");
        delete updateTable;
        ui->exportBookButton->setDisabled(false);
    }
    delete dialog;
}


void MainWindow::on_exportBookButton_clicked()
{
    ExportBookDialog * dialog = new ExportBookDialog(config);
    dialog->exec();
    if (dialog->get_filePath().length() > 0) {
        QString filePath = dialog->get_filePath();
        QString outputStyle = dialog->get_outputStyle();
        dataTable->writeExcelFile(filePath, config->get_field_types(outputStyle), config->get_ptr_mapF2S(outputStyle));
    }
    delete dialog;
}


void MainWindow::update_bookFilePathNameTextBrowser(const QString& text)
{
    updateTextBrowserContent(text, ui->bookFilePathNameTextBrowser);
    /*
    ui->bookFilePathNameTextBrowser->setText(text);
    QPalette palette = ui->bookFilePathNameTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->bookFilePathNameTextBrowser->setPalette(palette);
    */
}


void MainWindow::update_bookNameTextBrowser(const QString& text)
{
    updateTextBrowserContent(text, ui->bookNameTextBrowser);
    /*
    ui->bookNameTextBrowser->setText(text);
    QPalette palette = ui->bookNameTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->bookNameTextBrowser->setPalette(palette);
    */
}

void MainWindow::update_updateFilePathNameTextBrowser(const QString& text)
{
    updateTextBrowserContent(text, ui->updateFilePathNameTextBrowser);
    /*
    ui->updateFilePathNameTextBrowser->setText(text);
    QPalette palette = ui->updateFilePathNameTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->updateFilePathNameTextBrowser->setPalette(palette);
    */
}

void MainWindow::update_updateMappingTextBrowser(const QString& text)
{
    updateTextBrowserContent(text, ui->updateMappingTextBrowser);
    /*
    ui->updateMappingTextBrowser->setText(text);
    QPalette palette = ui->updateMappingTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->updateMappingTextBrowser->setPalette(palette);
    */
}

void MainWindow::update_updatePrimaryKeyTextBrowser(const QString& text)
{
    updateTextBrowserContent(text, ui->updatePrimaryKeyTextBrowser);
    /*
    ui->updatePrimaryKeyTextBrowser->setText(text);
    QPalette palette = ui->updatePrimaryKeyTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->updatePrimaryKeyTextBrowser->setPalette(palette);
    */
}

void MainWindow::update_updateProcessTextBrowser(const QString& text)
{
    QString currentText = ui->updateProcessTextBrowser->toPlainText();
    QString newText = currentText + text;
    updateTextBrowserContent(newText, ui->updateProcessTextBrowser);
}

void MainWindow::updateTextBrowserContent(const QString& text, QTextBrowser* qtb)
{
    qtb->setText(text);
    QPalette palette = qtb->palette();
    palette.setColor(QPalette::Text, Qt::red);
    qtb->setPalette(palette);
}
