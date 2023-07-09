#include "updatebookdialog.h"
#include "ui_updatebookdialog.h"
#include "config.h"


UpdateBookDialog::UpdateBookDialog(Config* config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateBookDialog)
{
    ui->setupUi(this);
    QVector<QString> bookNames = config->getRecordBookNames();
    //ui->comboBox = new QComboBox;
    for (QString mappingName : bookNames) {
        qDebug() << "新增映射：" << mappingName;
        ui->comboBox->addItem(mappingName);
    }
    ui->comboBox->setCurrentIndex(-1);
}

UpdateBookDialog::~UpdateBookDialog()
{
    delete ui;
}


QString UpdateBookDialog::get_filePath() {
    return filePath;
}

QString UpdateBookDialog::get_mappingName() {
    return mappingName;
}

QString UpdateBookDialog::get_primaryKeyStr() {
    return primaryKeyStr;
}

void UpdateBookDialog::on_browseButton_clicked() {
    ui->filePathEdit->setText(QFileDialog::getOpenFileName(this, "选择打开文件", "./"));
}

void UpdateBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
    qDebug() << "选择读取更新文件：" << filePath;
    mappingName = ui->comboBox->currentText();
    qDebug() << "选择字段映射：" << mappingName;
    primaryKeyStr = ui->primaryKeyEdit->toPlainText();
    this->close();
    delete ui;
}

void UpdateBookDialog::on_cancelButton_clicked() {
    filePath = "";
    this->close();
    delete ui;
}

void UpdateBookDialog::on_comboBox_currentTextChanged() {
    //qDebug() << ui->comboBox->
}
