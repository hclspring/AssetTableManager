#include "updatebookdialog.h"
#include "ui_updatebookdialog.h"
#include "config.h"


UpdateBookDialog::UpdateBookDialog(Config* config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateBookDialog)
{
    ui->setupUi(this);
    this->config = config;
    for (QString key : config->getPrimaryKeyColumnNames()) {
        ui->primaryKeyComboBox->addItem(key);
    }
    ui->primaryKeyComboBox->setCurrentIndex(-1);
    /*
    qDebug() << "测试点2";
    QVector<QString> bookNames = config->getExportBookTypes();
    qDebug() << "测试点1";
    for (QString mappingName : bookNames) {
        qDebug() << "新增映射：" << mappingName;
        ui->mappingComboBox->addItem(mappingName);
    }
    ui->mappingComboBox->setCurrentIndex(-1);
    qDebug() << "测试点";
    qDebug() << "函数UpdateDialog()已结束。";
    */
}

UpdateBookDialog::~UpdateBookDialog()
{
    delete ui;
}


QString UpdateBookDialog::get_filePath() {
    return filePath;
}

/*
QString UpdateBookDialog::get_mappingName() {
    return mappingName;
}
*/

QString UpdateBookDialog::get_primaryKeyStr() {
    return primaryKeyStr;
}

void UpdateBookDialog::on_browseButton_clicked() {
    ui->filePathEdit->setText(QFileDialog::getOpenFileName(this, "选择打开文件", "./"));
}

void UpdateBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
    qDebug() << "选择读取更新文件：" << filePath;
    //mappingName = ui->mappingComboBox->currentText();
    //qDebug() << "选择字段映射：" << mappingName;
    primaryKeyStr = ui->primaryKeyComboBox->currentText();
    this->close();
    delete ui;
}

void UpdateBookDialog::on_cancelButton_clicked() {
    filePath = "";
    //mappingName = "";
    primaryKeyStr = "";
    this->close();
    delete ui;
}


void UpdateBookDialog::on_primaryKeyComboBox_currentTextChanged() {
    if (ui->primaryKeyComboBox->currentIndex() == -1) {
        ui->confirmButton->setDisabled(true);
    } else {
        ui->confirmButton->setDisabled(false);
    }
}

/*
void UpdateBookDialog::on_mappingComboBox_currentTextChanged() {
    qDebug() << "进入UpdateBookDialog::on_mappingComboBox_currentTextChanged()";
    QString curMapping = ui->mappingComboBox->currentText();
    qDebug() << "获取到当前mappingComboBox文本值：" << curMapping;
    if (curMapping.length() > 0) {
        PtrQMapF2S ptrMap = config->get_ptr_mapF2S(curMapping);
        qDebug() << "获取到当前mapping指针引用数" << ptrMap.use_count();
        if (ptrMap != nullptr) {
            ui->primaryKeyComboBox->clear();
            for (enum FieldType ft : DISTINCTFIELDS) {
                if (ptrMap->find(ft) != ptrMap->end()) {
                    ui->primaryKeyComboBox->addItem(ptrMap->find(ft).value());
                }
            }
        }
        ui->primaryKeyComboBox->setCurrentIndex(-1);
    }
}
*/
