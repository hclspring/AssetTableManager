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
    mappingNames.push_back("");
    for (QString mappingName : mappingNames) {
        ui->exportStyleComboBox->addItem(mappingName);
    }
    ui->exportStyleComboBox->setCurrentIndex(-1);
}

ExportBookDialog::~ExportBookDialog() {
    delete ui;
    deleteLater();
}

QString ExportBookDialog::get_filePath(){
    return filePath;
}

QString ExportBookDialog::get_outputStyle(){
    return outputStyle;
}

void ExportBookDialog::on_browseButton_clicked() {
    filePath = QFileDialog::getSaveFileName(this, "保存文件", "./", "表格文件(*.xlsx)");
    ui->filePathEdit->setText(filePath);
}

void ExportBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
    if (ui->exportStyleComboBox->currentIndex() < 0) {
        outputStyle = "";
    } else {
        outputStyle = ui->exportStyleComboBox->currentText();
    }
    this->close();
}

void ExportBookDialog::on_cancelButton_clicked() {
    filePath = "";
    outputStyle = "";
    this->close();
}
