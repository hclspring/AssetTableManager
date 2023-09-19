#include <QFileDialog>

#include "importbookdialog.h"
#include "ui_importbookdialog.h"
#include "config.h"
#include "datatable.h"

ImportBookDialog::ImportBookDialog(Config* config, bool needPrimaryKey, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ImportBookDialog)
{
    this->config = config;
    ui->setupUi(this);
    ui->sheetNameComboBox->setDisabled(true);

    ui->updateFailedButton->setVisible(needPrimaryKey);
    ui->updateFailedPathEdit->setVisible(needPrimaryKey);
    ui->primaryKeyLabel->setVisible(needPrimaryKey);
    ui->primaryKeyComboBox->setVisible(needPrimaryKey);

    for (QString key : config->getPrimaryKeyColumnNames()) {
        ui->primaryKeyComboBox->addItem(key);
    }

    if (needPrimaryKey) {
        ui->primaryKeyComboBox->setCurrentIndex(-1);
    }

    ui->columnNameRowLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    ui->dataStartRowLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
}

ImportBookDialog::~ImportBookDialog() {
    delete ui;
    deleteLater();
}

const QString &ImportBookDialog::getSheetName() const
{
    return sheetName;
}

const QStringList &ImportBookDialog::getSheetNames() const
{
    return sheetNames;
}

const QString &ImportBookDialog::getPrimaryKey() const
{
    return primaryKey;
}

int ImportBookDialog::getDataStartRow() const
{
    return dataStartRow;
}

int ImportBookDialog::getColumnNameRow() const
{
    return columnNameRow;
}

const QString &ImportBookDialog::getFilePath() const {
    return filePath;
}

const QString &ImportBookDialog::getUpdateFailedPath() const {
    return updateFailedPath;
}

void ImportBookDialog::on_browseButton_clicked() {
    ui->filePathEdit->setText(QFileDialog::getOpenFileName(this, "选择打开文件", "./"));

    QXlsx::Document document(ui->filePathEdit->toPlainText());
    ui->sheetNameComboBox->clear();
    if (document.load()) {
        this->sheetNames = document.sheetNames();
        for (QString sheetName : sheetNames) {
            ui->sheetNameComboBox->addItem(sheetName);
        }
        ui->sheetNameComboBox->setCurrentIndex(-1);
        ui->sheetNameComboBox->setDisabled(false);
    } else {
        qCritical() << "无法按照Excel表格读取文件" << ui->filePathEdit->toPlainText();
    }
}

void ImportBookDialog::on_updateFailedButton_clicked() {
    ui->updateFailedPathEdit->setText(QFileDialog::getExistingDirectory(this, "选择更新失败文件保存路径", "./", QFileDialog::ShowDirsOnly));
}

void ImportBookDialog::on_confirmButton_clicked() {
    filePath = ui->filePathEdit->toPlainText();
    updateFailedPath = ui->updateFailedPathEdit->toPlainText();
    sheetName = ui->sheetNameComboBox->currentText();
    primaryKey = ui->primaryKeyComboBox->currentText();
    dataStartRow = ui->dataStartRowLineEdit->text().toInt();
    columnNameRow = ui->columnNameRowLineEdit->text().toInt();
    this->close();
}

void ImportBookDialog::on_cancelButton_clicked() {
    filePath = "";
    updateFailedPath = "";
    sheetName = "";
    primaryKey = "";
    dataStartRow = -1;
    columnNameRow = -1;
    this->close();
}

void ImportBookDialog::on_primaryKeyComboBox_currentTextChanged() {
    set_confirmButton();
}

void ImportBookDialog::on_sheetNameComboBox_currentTextChanged() {
    set_confirmButton();
}

void ImportBookDialog::on_columnNameRowLineEdit_textChanged(const QString &arg1)
{
    set_confirmButton();
}

void ImportBookDialog::on_dataStartRowLineEdit_textChanged(const QString &arg1)
{
    set_confirmButton();
}


bool ImportBookDialog::check_allInput_legal() {
    return check_sheetName_legal() && check_dataStartRow_legal() && check_primaryKey_legal() && check_sheetName_legal();
}

bool ImportBookDialog::check_sheetName_legal() {
    return (ui->sheetNameComboBox->currentIndex() >= 0);
}

bool ImportBookDialog::check_primaryKey_legal() {
    return (ui->primaryKeyComboBox->isVisible() == false || ui->primaryKeyComboBox->currentIndex() >= 0);
}

bool ImportBookDialog::check_dataStartRow_legal() {
    QString s1 = ui->dataStartRowLineEdit->text();
    QString s2 = ui->columnNameRowLineEdit->text();
    if (s1.length() > 0 && s2.length() > 0) {
        int x1 = ui->dataStartRowLineEdit->text().toInt();
        int x2 = ui->columnNameRowLineEdit->text().toInt();
        return x1 >= 1 && x1 > x2;
    } else {
        return false;
    }
}

bool ImportBookDialog::check_columnNameRow_legal() {
    QString s1 = ui->dataStartRowLineEdit->text();
    QString s2 = ui->columnNameRowLineEdit->text();
    if (s1.length() > 0 && s2.length() > 0) {
        int x1 = ui->dataStartRowLineEdit->text().toInt();
        int x2 = ui->columnNameRowLineEdit->text().toInt();
        return x2 >= 1 && x1 > x2;
    } else {
        return false;
    }
}


void ImportBookDialog::set_confirmButton() {
    if (check_allInput_legal()) {
        ui->confirmButton->setDisabled(false);
    } else {
        ui->confirmButton->setDisabled(true);
    }
}

void ImportBookDialog::test() {
    qDebug() << "test slot.";
}
