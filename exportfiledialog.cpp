#include <QDebug>
#include <QFileDialog>

#include "exportfiledialog.h"
#include "ui_exportfiledialog.h"

ExportFileDialog::ExportFileDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ExportFileDialog)
{
    ui->setupUi(this);
}

ExportFileDialog::~ExportFileDialog() {
    delete ui;
}

void ExportFileDialog::on_browseButton_clicked() {
    //filePath = QFileDialog::getOpenFileName(this, "选择单个文件", "./");
    //filePath = QFileDialog::getSaveFileUrl(this, "选择文件");
    fileDir = QFileDialog::getExistingDirectory(this, "选择保存目录", "./");
    ui->fileDirEdit->setText(fileDir);
}

void ExportFileDialog::on_confirmButton_clicked() {
    this->close();
    delete ui;
}

void ExportFileDialog::on_cancelButton_clicked() {
    fileDir = "";
    this->close();
    delete ui;
}
