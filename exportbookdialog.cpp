#include <QDebug>
#include <QFileDialog>

#include "exportbookdialog.h"
#include "ui_exportbookdialog.h"

ExportBookDialog::ExportBookDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ExportBookDialog)
{
    ui->setupUi(this);
}

ExportBookDialog::~ExportBookDialog() {
    delete ui;
}

void ExportBookDialog::on_browseButton_clicked() {
    //filePath = QFileDialog::getOpenFileName(this, "选择单个文件", "./");
    //filePath = QFileDialog::getSaveFileUrl(this, "选择文件");
    fileDir = QFileDialog::getExistingDirectory(this, "选择保存目录", "./");
    ui->fileDirEdit->setText(fileDir);
}

void ExportBookDialog::on_confirmButton_clicked() {
    this->close();
    delete ui;
}

void ExportBookDialog::on_cancelButton_clicked() {
    fileDir = "";
    this->close();
    delete ui;
}
