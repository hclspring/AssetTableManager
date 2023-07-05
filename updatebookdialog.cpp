#include "updatebookdialog.h"
#include "ui_updatebookdialog.h"
#include "config.h"

UpdateBookDialog::UpdateBookDialog(Config* config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateBookDialog)
{
    QVector<QString> bookNames = config->getRecordBookNames();
    //ui->comboBox = new QComboBox;
    for (QString mappingName : bookNames) {
        qDebug() << "新增映射：" << mappingName;
        ui->comboBox->addItem(mappingName);
    }
    ui->setupUi(this);
}

UpdateBookDialog::~UpdateBookDialog()
{
    delete ui->comboBox;
    delete ui;
}


QString UpdateBookDialog::get_filePath() {
    return filePath;
}

void UpdateBookDialog::on_browseButton_clicked() {
    ui->filePathEdit->setText(QFileDialog::getOpenFileName(this, "选择打开文件", "./"));
}

void UpdateBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
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
