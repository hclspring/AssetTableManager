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
    ui->inputStyleTextBrowser->setText("");
    ui->settingButton->setDisabled(true);
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
        QString inputStyle = dialog->get_inputStyle();
        update_inputStyleTextBrowser(QString("导入样式：") + inputStyle);

        qDebug() << "初始化dataTable……";
        if (dataTable != nullptr) delete dataTable;
        dataTable = new DataTable;
        qDebug() << "开始读取台账文件……";
        dataTable->readExcelFile(ui->bookFilePathNameTextBrowser->toPlainText(),
                                 config->getMappingImportSource2Target(),
                             config->get_sheetIndex(inputStyle),
                             config->get_columnNameRow(inputStyle),
                             config->get_dataStartRow(inputStyle));
        qDebug() << "结束读取台账文件……";
        append_updateProcessTextBrowser("已导入台账文件：" + dialog->get_filePath());
    }
    ui->updateBookButton->setDisabled(false);
    ui->exportBookButton->setDisabled(true);
    delete dialog;
}

void MainWindow::on_updateBookButton_clicked()
{
    qDebug() << "进入函数MainWindow::on_updateBookButton_clicked()";
    UpdateBookDialog * dialog = new UpdateBookDialog(config);
    qDebug() << "完成初始化UpdateBookDialog";
    dialog->exec();
    if (dialog->get_filePath().length() > 0) {
        QString filePath = dialog->get_filePath();
        QString bookName = dialog->get_mappingName();
        QString primaryKeyStr = dialog->get_primaryKeyStr();
        qDebug() << "读取更新台账用文件：" << filePath;
        update_updateFilePathNameTextBrowser(filePath);
        update_updateMappingTextBrowser(QString("字段映射：") + bookName);

        DataTable * updateTable = new DataTable;
        qDebug() << "开始读取更新文件……";
        updateTable->readExcelFile(ui->updateFilePathNameTextBrowser->toPlainText(),
                                   config->getMappingImportSource2Target(),
                             config->get_sheetIndex(bookName),
                             config->get_columnNameRow(bookName),
                             config->get_dataStartRow(bookName));
        qDebug() << "读取更新文件完毕，开始更新……";
        dataTable->updateWith(updateTable, primaryKeyStr);
        qDebug() << "更新后的列数：" << dataTable->get_columnNames()->size();
        append_updateProcessTextBrowser("已按照此文件更新台账：" + filePath);
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
        qDebug() << "输出格式：" << outputStyle;
        QVector<QString> exportColumnNames;
        if (config->getExportColumnNames(outputStyle) !=  nullptr) {
            dataTable->writeExcelFile(filePath, config->getExportColumnNames(outputStyle), config->getMappingExportTarget2Source());
            append_updateProcessTextBrowser("已导出台账文件：" + filePath);
        }
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


void MainWindow::update_inputStyleTextBrowser(const QString& text)
{
    updateTextBrowserContent(text, ui->inputStyleTextBrowser);
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

void MainWindow::append_updateProcessTextBrowser(const QString& text)
{
    appendTextBrowserContent(" * " + text, ui->updateProcessTextBrowser);
}

void MainWindow::updateTextBrowserContent(const QString& newText, QTextBrowser* qtb)
{
    qtb->setText(newText);
    QPalette palette = qtb->palette();
    palette.setColor(QPalette::Text, Qt::red);
    qtb->setPalette(palette);
}

void MainWindow::appendTextBrowserContent(const QString& newText, QTextBrowser* qtb)
{
    QString currentText = qtb->toPlainText();
    if (currentText.endsWith('\n') == false) {
        currentText.append("\n");
    }
    currentText.append(newText);
    updateTextBrowserContent(currentText, qtb);
}
