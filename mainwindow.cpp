#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
    delete mFile;
}

void MainWindow::on_actionSave_triggered()
{
    if (!mFile) {
        on_actionSave_As_triggered();
    } else {
        mFile->open(QFile::WriteOnly);
        QTextStream stream(mFile);
        stream << ui->textEdit->toPlainText();
        mFile->close();
    }
}


void MainWindow::on_actionOpen_triggered()
{
    // https://doc.qt.io/qt-5/qfiledialog.html#details
    QString filePath = QFileDialog::getOpenFileName(
                this,                    // parent
                tr("Open File"),         // dialog title
                tr(""),                  // starting directory
                tr("Text files (*.txt)") // filter
                );
    mFile = new QFile(filePath);
    updateTextEdit();
}

void MainWindow::on_actionSave_As_triggered()
{

    QString namefile = QFileDialog::getSaveFileName(this, "File save");
          QFile file(namefile);
          QTextStream out(&file);
          QString text = ui->textEdit->toPlainText();
          out << text;
          file.close();

//    // comment the following call after implementing the method
//    QMessageBox::warning(this,
//                         tr("\"on_actionSave_As_triggered\" method is not implemented."),
//                         tr("It is one of the methods you need to implement\n"
//                            "to complete the assignment.")
//                         );
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionFont_triggered()
{

    bool tmp;
    QFont font = QFontDialog::getFont(&tmp, QFont(), this);
    if (tmp) {
            ui->textEdit->setFont(font);
    }

//    // comment the following call after implementing the method
//    QMessageBox::warning(this,
//                         tr("\"on_actionFont_triggered\" method is not implemented."),
//                         tr("It is one of the methods you need to implement\n"
//                            "to complete the assignment.")
//                         );


}


void MainWindow::on_actionColor_triggered()
{
    auto color = QColorDialog::getColor();

    if (!ui->textEdit->textCursor().hasSelection()){
        ui->textEdit->selectAll();
        ui->textEdit->setTextColor(color);
        ui->textEdit->textCursor().clearSelection();
    }

    ui->textEdit->setTextColor(color);
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Title", "Message");
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::updateTextEdit(QColor * color /* = nullptr */, QFont * font /* = nullptr */) {
    if (mFile){
        mFile->open(QFile::ReadOnly);

        QTextStream stream(mFile);
        QString text = stream.read(10000);

        mFile->close();

        ui->textEdit->setText(text);
    }

    if (color) {
        ui->textEdit->selectAll();
        ui->textEdit->setTextColor(*color);
        ui->textEdit->textCursor().clearSelection();
    }
    if (font) {
        ui->textEdit->setFont(*font);
    }
}

void MainWindow::saveSettings() {
    QSettings settings("org", "app");
    settings.beginGroup("textEdit");
    if (mFile) settings.setValue("file_name", mFile->fileName());
    settings.setValue("color", ui->textEdit->textColor());
    settings.setValue("font", ui->textEdit->font());
    settings.endGroup();

    settings.beginGroup("window");
    settings.setValue("size", this->size());
    settings.endGroup();
}

void MainWindow::loadSettings() {
    QSettings settings("org", "app");
    settings.beginGroup("window");
    QSize size_ = settings.value("size",QSize()).toSize();
    ui->textEdit->resize(size_);
    settings.endGroup();

    settings.beginGroup("textEdit");
    QString file_name = settings.value("file_name",QString()).toString();

    QColor color = settings.value("color", QColor()).value<QColor>();
    ui->textEdit->setTextColor(color);
    QFont font = settings.value("font",QFont()).value<QFont>();
    ui->textEdit->setFont(font);
    settings.endGroup();

//    // comment the following call after implementing the method
//    QMessageBox::warning(this,
//                         tr("\"loadSettings\" method is not implemented."),
//                         tr("It is one of the methods you need to implement\n"
//                            "to complete the assignment.")
//                         );
}

void MainWindow::on_actionCut_triggered(){
    ui->textEdit->cut();
}
void MainWindow::on_actionPaste_triggered(){
    ui->textEdit->paste();
}
void MainWindow::on_actionRedo_triggered(){
    ui->textEdit->redo();
}
