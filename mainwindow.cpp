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

        dataTable = new DataTable;
        dataTable->readExcelFile(ui->bookFilePathNameTextBrowser->toPlainText(),
                             config->get_ptr_mapS2F(bookName),
                             config->get_sheetIndex(bookName),
                             config->get_columnNameRow(bookName),
                             config->get_dataStartRow(bookName));
    }

    delete dialog;
}

void MainWindow::on_updateBookButton_clicked()
{
    UpdateBookDialog * dialog = new UpdateBookDialog (config);
    dialog->exec();
    delete dialog;
}


void MainWindow::on_exportBookButton_clicked()
{
    ExportBookDialog * dialog = new ExportBookDialog;
    dialog->exec();
    delete dialog;
}


void MainWindow::update_bookFilePathNameTextBrowser(const QString& text)
{
    ui->bookFilePathNameTextBrowser->setText(text);
    QPalette palette = ui->bookFilePathNameTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->bookFilePathNameTextBrowser->setPalette(palette);
}


void MainWindow::update_bookNameTextBrowser(const QString& text)
{
    ui->bookNameTextBrowser->setText(text);
    QPalette palette = ui->bookNameTextBrowser->palette();
    palette.setColor(QPalette::Text, Qt::red);
    ui->bookNameTextBrowser->setPalette(palette);
}


