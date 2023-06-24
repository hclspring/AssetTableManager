#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exportfiledialog.h"
#include "importfiledialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->importFileButton, SIGNAL(clicked()), this, SLOT(on_importFileButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_importBookButton_clicked()
{
    ImportFileDialog * dialog = new ImportFileDialog;
    dialog->exec();
    delete dialog;
}

void MainWindow::on_updateBookButton_clicked()
{
    ImportFileDialog * dialog = new ImportFileDialog;
    dialog->exec();
    delete dialog;
}


void MainWindow::on_exportBookButton_clicked()
{
    ExportFileDialog * dialog = new ExportFileDialog;
    dialog->exec();
    delete dialog;
}
