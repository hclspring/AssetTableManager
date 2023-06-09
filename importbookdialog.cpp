#include <QFileDialog>

#include "importbookdialog.h"
#include "ui_importbookdialog.h"
#include "config.h"
#include "datatable.h"

ImportBookDialog::ImportBookDialog(Config* config, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ImportBookDialog)
{
    qDebug() << "开始构建importBookDialog";
    QVector<QString> bookNames = config->getRecordBookNames();
    qDebug() << "从配置中获取bookNames，共有" << bookNames.size() << "个bookName.";
    ui->setupUi(this);
    qDebug() << "结束构建importBookDialog";
}

ImportBookDialog::~ImportBookDialog() {
    delete ui;
}

QString ImportBookDialog::get_filePath() {
    return filePath;
}

void ImportBookDialog::on_browseButton_clicked() {
    ui->filePathEdit->setText(QFileDialog::getOpenFileName(this, "选择打开文件", "./"));
}

void ImportBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
    this->close();
    delete ui;
}

void ImportBookDialog::on_cancelButton_clicked() {
    filePath = "";
    this->close();
    delete ui;
}

void ImportBookDialog::test() {
    qDebug() << "test slot.";
}

/*
QVector<QString> ImportBookDialog::read_book_names(const QString& bookRootPath)
{
    ;
}
*/
