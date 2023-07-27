#include <QDebug>
#include <QFileDialog>

#include "exportbookdialog.h"
#include "ui_exportbookdialog.h"
#include "config.h"

ExportBookDialog::ExportBookDialog(Config* config, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ExportBookDialog)
{
    ui->setupUi(this);
    QVector<QString> mappingNames = config->getExportBookTypes();
    mappingNames.push_back("全部字段");
    for (QString mappingName : mappingNames) {
        ui->exportStyleComboBox->addItem(mappingName);
    }
}

ExportBookDialog::~ExportBookDialog() {
    delete ui;
}

QString ExportBookDialog::get_filePath(){
    return filePath;
}

QString ExportBookDialog::get_outputStyle(){
    return outputStyle;
}

void ExportBookDialog::on_browseButton_clicked() {
    //filePath = QFileDialog::getOpenFileName(this, "选择单个文件", "./");
    //filePath = QFileDialog::getSaveFileUrl(this, "选择文件");
    //fileDir = QFileDialog::getExistingDirectory(this, "选择保存目录", "./");
    filePath = QFileDialog::getSaveFileName(this, "保存文件", "./", "表格文件(*.xlsx)");
    ui->filePathEdit->setText(filePath);
}

void ExportBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
    outputStyle = ui->exportStyleComboBox->currentText();
    this->close();
    delete ui;
}

void ExportBookDialog::on_cancelButton_clicked() {
    filePath = "";
    outputStyle = "";
    this->close();
    delete ui;
}
