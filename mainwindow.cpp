#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exportbookdialog.h"
#include "importbookdialog.h"
#include "config.h"
#include "datatable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    config = new Config;
    ui->setupUi(this);
    ui->settingButton->setDisabled(true);
    ui->exportBookButton->setDisabled(true);
    ui->updateBookButton->setDisabled(true);
#ifdef USE_DEFAULT_MENU_BAR
//添加菜单栏(此处添加为的为QMainWindow自带的菜单）
        menuBar = this->menuBar();
#else
    //添加自定义菜单
        menuBar = new QMenuBar(this);
#endif
    menuHelp = new QMenu("帮助(&h)", this);
    actionAbout = new QAction("关于(&A)", this);
    menuHelp->addAction(actionAbout);
    menuBar->addMenu(menuHelp);
#ifndef USE_DEFAULT_MENU_BAR
    //当不使用QMainWindow自带的菜单栏时，必须要加上此行
    setMenuBar(menuBar);
#endif
    connect(actionAbout, SIGNAL(triggered(bool)), this, SLOT(on_actionAbout_triggered()));
}

MainWindow::~MainWindow()
{
    qDebug() << "Enter ~MainWindow()";
    if (dataTable != nullptr) {
        delete dataTable;
    }
    delete config;
    delete ui;
    deleteLater();
    qDebug()  << "Exit ~MainWindow()";
}

void MainWindow::on_importBookButton_clicked()
{
    ImportBookDialog * dialog = new ImportBookDialog(config, false);
    dialog->exec();
    if (dialog->getFilePath().length() > 0) {
        qDebug() << "读取台账文件：" << dialog->getFilePath();
        inputFilePath = dialog->getFilePath();
        updateSheetName = dialog->getSheetName();
        qDebug() << "初始化dataTable……";
        if (dataTable != nullptr) delete dataTable;
        dataTable = new DataTable;
        qDebug() << "开始读取台账文件……";
        dataTable->readExcelFile(dialog->getFilePath(),
                                 config->getMappingImportSource2Target(),
                             dialog->getSheetName(),
                             dialog->getColumnNameRow(),
                             dialog->getDataStartRow());
        qDebug() << "结束读取台账文件……";
        append_updateProcessTextBrowser("已导入台账文件：" + dialog->getFilePath());
    }
    ui->updateBookButton->setDisabled(false);
    ui->exportBookButton->setDisabled(true);
    dialog->deleteLater();
}

void MainWindow::on_updateBookButton_clicked()
{
    qDebug() << "进入函数MainWindow::on_updateBookButton_clicked()";
    ImportBookDialog * dialog = new ImportBookDialog(config, true);
    dialog->exec();
    if (dialog->getFilePath().length() > 0) {
        QString filePath = dialog->getFilePath();
        QString primaryKey = dialog->getPrimaryKey();
        qDebug() << "读取更新台账用文件：" << filePath;
        DataTable * updateTable = new DataTable;
        qDebug() << "开始读取更新文件……";
        updateTable->readExcelFile(filePath,
                                 config->getMappingImportSource2Target(),
                             dialog->getSheetName(),
                             dialog->getColumnNameRow(),
                             dialog->getDataStartRow());
        qDebug() << "读取更新文件完毕，开始更新……";
        DataTable* updateIgnoredTable = dataTable->updateWith(updateTable, primaryKey);
        if (updateIgnoredTable) {
            qDebug() << "更新后的列数：" << dataTable->get_columnNameCellVecPtr()->size();
            qDebug() << "更新失败的行数：" << updateIgnoredTable->get_dataPtr()->size();
            append_updateProcessTextBrowser("已按照此文件更新台账：" + filePath + "; 更新主键为：" + dialog->getPrimaryKey());
        } else {
            qDebug() << "更新失败";
            append_updateProcessTextBrowser("未能成功按照此文件更新台账：" + filePath + "; 拟更新主键为" + dialog->getPrimaryKey());
        }
        QString updateFailedFilePath = QDir::toNativeSeparators(dialog->getUpdateFailedPath())
                + QDir::separator() + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")
                + ".xlsx";
        qDebug() << "准备将未能成功更新数据写入文件";
        updateIgnoredTable->writeExcelFile(updateFailedFilePath);
        qDebug() << "已将未能成功更新数据写入文件";
        delete updateIgnoredTable;
        delete updateTable;
        ui->exportBookButton->setDisabled(false);
    }
    dialog->deleteLater();
}


void MainWindow::on_exportBookButton_clicked()
{
    ExportBookDialog * dialog = new ExportBookDialog(config);
    dialog->exec();
    if (dialog->get_filePath().length() > 0) {
        QString filePath = dialog->get_filePath();
        QString outputStyle = dialog->get_outputStyle();
        qDebug() << "输出格式：" << outputStyle;
        if (dataTable->writeExcelFile(filePath, GetCellsPtr(config->getExportColumnNames(outputStyle)), config->getMappingExportTarget2Source(),
                                      inputFilePath, updateSheetName)) {
            qDebug() << "导出成功！";
            append_updateProcessTextBrowser("已导出台账文件：" + filePath);
        } else {
            qDebug() << "导出失败！";
            append_updateProcessTextBrowser("导出台账文件失败：" + filePath);
        }
    }
    dialog->deleteLater();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("关于本软件");
    QString aboutMessage("本软件由黄春林开发，版本为%1");
    //aboutMessage = aboutMessage.arg(QDateTime::currentDateTime().toString("yyyyMMdd")); //获取使用软件终端的当前时间
    QDate compileDate = QDate::fromString(__DATE__);
    aboutMessage = aboutMessage.arg(compileDate.toString("yyyyMMdd")); //获取编译时的当前时间
    messageBox.setText(aboutMessage);
    messageBox.exec();
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
