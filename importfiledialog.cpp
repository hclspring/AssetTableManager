#include <QFileDialog>

#include "importfiledialog.h"
#include "ui_importfiledialog.h"

ImportFileDialog::ImportFileDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ImportFileDialog)
{
    ui->setupUi(this);
}

ImportFileDialog::~ImportFileDialog() {
    delete ui;
}

void ImportFileDialog::on_browseButton_clicked() {
    filePath = QFileDialog::getOpenFileName(this, "选择打开文件", "./");
    ui->filePathEdit->setText(filePath);
}

void ImportFileDialog::on_confirmButton_clicked() {
    this->close();
    delete ui;
}

void ImportFileDialog::on_cancelButton_clicked() {
    filePath = "";
    this->close();
    delete ui;
}
