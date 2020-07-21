#include "notepad.hpp"
#include "ui_notepad.h"

#include <QWidget>
#include <QMainWindow>
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <QTabBar>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QColorDialog>
#include <QPalette>
#include <QFontDialog>

#include "file_text_edit.hpp"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Notepad),
      tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);

    this->setCentralWidget(tabWidget);

    filesTextEdit.push_back(new FileTextEdit(this));

    tabWidget->addTab(filesTextEdit[0],
                      "plik " + QString::number(++totalFilesNumber));
    tabWidget->widget(0)->setFocus();

    tabWidget->setMovable(false);
    tabWidget->setTabsClosable(true);

    tabWidget->addTab(new QLabel("+"), QString("+"));
    tabWidget->tabBar()->tabButton(1, QTabBar::RightSide)->resize(0, 0);

    connect(tabWidget, &QTabWidget::currentChanged,
            this, &Notepad::on_actionChangeTab_triggered);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(on_actionCloseTab_triggered(int)));
}

Notepad::~Notepad()
{
    for (int i {0}; i < filesTextEdit.size(); ++i) {
        delete filesTextEdit[i];
    }

    delete tabWidget;
    delete ui;
}

void Notepad::on_actionNew_triggered()
{
    newFile();
}

void Notepad::on_actionOpen_triggered()
{
    const QString filename {QFileDialog::getOpenFileName(this, "Otwieranie")};
    QFile file(filename);

    for(int i {0}; i < filesTextEdit.size(); ++i) {
        if(filesTextEdit[i]->getFilename().compare(filename) == 0) {
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ostrzeżenie",
                             "Nie można otworzyć pliku: " +
                             file.errorString());
        return;
    }

    const int filesNumber {filesTextEdit.size()};

    filesTextEdit.push_back(new FileTextEdit(this));

    tabWidget->insertTab(filesNumber, filesTextEdit[filesNumber],
                         "plik " + QString::number(++totalFilesNumber));
    tabWidget->setCurrentIndex(filesNumber);

    if(filename.contains("/")) {
        tabWidget->setTabText(filesNumber, filename.split("/").last());
    } else if(filename.contains("\\")) {
        tabWidget->setTabText(filesNumber, filename.split("\\").last());
    } else {
        tabWidget->setTabText(filesNumber, filename);
    }

    filesTextEdit[filesNumber]->setText(QTextStream(&file).readAll());
    filesTextEdit[filesNumber]->setTextChanged(false);

    setWindowTitle(filename);

    file.close();
}

void Notepad::on_actionSave_triggered()
{
    const QString filename {
        filesTextEdit[tabWidget->currentIndex()]->getFilename().isEmpty() ?
        QFileDialog::getSaveFileName(this, "Zapisywanie jako") :
        filesTextEdit[tabWidget->currentIndex()]->getFilename()
    };

    saveFile(filename);
    setWindowTitle(filename);
}

void Notepad::on_actionSaveAs_triggered()
{
    const QString filename {
        QFileDialog::getSaveFileName(this, "Zapisywanie jako")
    };

    saveFile(filename);
    setWindowTitle(filename);
}

void Notepad::on_actionPrinter_triggered()
{
    QPrinter printer;
    QPrintDialog printerDialog(&printer, this);

    if (printerDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Ostrzeżenie",
                             "Nie można uzyskać dostępu do drukarki");
        return;
    }

    filesTextEdit[tabWidget->currentIndex()]->print(&printer);
}

void Notepad::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply {
        QMessageBox::question(this, "Notatnik",
                              "Jesteś pewien że chcesz wyjść?\n\n"
                              "Niezapisane zmiany zostaną utracone!",
                              QMessageBox::No | QMessageBox::Yes,
                              QMessageBox::Yes)
    };

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void Notepad::on_actionUndo_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->undo();
}

void Notepad::on_actionRedo_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->redo();
}

void Notepad::on_actionCopy_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->copy();
}

void Notepad::on_actionCut_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->cut();
}

void Notepad::on_actionPaste_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->paste();
}

void Notepad::on_actionDeleteSelectedText_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->textCursor().removeSelectedText();
}

void Notepad::on_actionSelectAll_triggered()
{
    filesTextEdit[tabWidget->currentIndex()]->selectAll();
}

void Notepad::on_actionBackgroundColor_triggered()
{
    QColor color {QColorDialog::getColor(Qt::white, this)};
    QPalette palette {filesTextEdit[tabWidget->currentIndex()]->palette()};

    palette.setColor(QPalette::Base, color);

    filesTextEdit[tabWidget->currentIndex()]->setPalette(palette);
}

void Notepad::on_actionTextColor_triggered()
{
    QColor color {QColorDialog::getColor(Qt::white, this)};
    QTextCharFormat textCharFormat {
        filesTextEdit[tabWidget->currentIndex()]->textCursor().charFormat()
    };

    textCharFormat.setForeground(color);

    filesTextEdit[tabWidget->currentIndex()]->textCursor().setCharFormat(textCharFormat);
    filesTextEdit[tabWidget->currentIndex()]->setCurrentCharFormat(textCharFormat);
}

void Notepad::on_actionFont_triggered()
{
    bool ok;
    QFont font {QFontDialog::getFont(&ok, this)};

    if (ok) {
        QTextCharFormat textCharFormat {
            filesTextEdit[tabWidget->currentIndex()]->textCursor().charFormat()
        };

        textCharFormat.setFont(font);

        filesTextEdit[tabWidget->currentIndex()]->textCursor().setCharFormat(textCharFormat);
        filesTextEdit[tabWidget->currentIndex()]->setCurrentCharFormat(textCharFormat);
    }
}

void Notepad::on_actionAboutAuthor_triggered()
{
    QMessageBox::information(this, "Informacje o autorze",
                             "autor: Damian Marciniak\n"
                             "rok:    2020");
}

void Notepad::on_actionChangeTab_triggered(int index)
{
    if (index == tabWidget->count() - 1) {
        newFile();
    }
}

void Notepad::on_actionCloseTab_triggered(int index)
{
    QMessageBox::StandardButton reply {QMessageBox::Yes};

    if (index == -1 || (index == 0 && tabWidget->count() == 2)) {
        return;
    }

    if (!filesTextEdit[index]->toPlainText().isEmpty() &&
        filesTextEdit[index]->getTextChanged()) {
        reply = QMessageBox::question(this, "Notatnik",
                                      "Jesteś pewien że chcesz zamknąć plik?\n\n"
                                      "Niezapisane zmiany zostaną utracone!",
                                      QMessageBox::No | QMessageBox::Yes);
    }

    if (reply == QMessageBox::Yes) {
        if (tabWidget->currentIndex() == index &&
            index == tabWidget->count() - 2) {
            tabWidget->setCurrentIndex(index - 1);
        }

        delete filesTextEdit[index];
        filesTextEdit.removeAt(index);
    }
}


void Notepad::on_actionToLeft_triggered()
{
    textAlign(Qt::AlignLeft);
}

void Notepad::on_actionToCenter_triggered()
{
    textAlign(Qt::AlignCenter);
}

void Notepad::on_actionToRight_triggered()
{
    textAlign(Qt::AlignRight);
}

void Notepad::on_actionJustification_triggered()
{
    textAlign(Qt::AlignJustify);
}

void Notepad::on_actionLineSpacing1_0_triggered()
{
    lineSpacing(1.0);
}

void Notepad::on_actionLineSpacing1_15_triggered()
{
    lineSpacing(1.15);
}

void Notepad::on_actionLineSpacing1_5_triggered()
{
    lineSpacing(1.5);
}

void Notepad::on_actionLineSpacing2_0_triggered()
{
    lineSpacing(2.0);
}

void Notepad::on_actionLineSpacing2_5_triggered()
{
    lineSpacing(2.5);
}

void Notepad::on_actionLineSpacing3_0_triggered()
{
    lineSpacing(3.0);
}

void Notepad::newFile()
{
    const int filesNumber {filesTextEdit.size()};

    filesTextEdit.push_back(new FileTextEdit(this));

    tabWidget->insertTab(filesNumber, filesTextEdit[filesNumber],
                         "plik " + QString::number(++totalFilesNumber));
    tabWidget->setCurrentIndex(filesNumber);
}

void Notepad::saveFile(const QString &filename) {
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ostrzeżenie",
                             "Nie można zapisać pliku: " +
                             file.errorString());
        return;
    }

    if (filename.contains(".html")) {
        QTextStream(&file)
            << filesTextEdit[tabWidget->currentIndex()]->toHtml();
    } else {
        QTextStream(&file)
            << filesTextEdit[tabWidget->currentIndex()]->toPlainText();
    }

    tabWidget->setTabText(tabWidget->currentIndex(),
                          filename.split("/").last());

    filesTextEdit[tabWidget->currentIndex()]->setFilename(filename);
    filesTextEdit[tabWidget->currentIndex()]->setTextChanged(false);

    file.close();
}

void Notepad::textAlign(const Qt::Alignment &aligment)
{
    QTextCursor textCursor {
        filesTextEdit[tabWidget->currentIndex()]->textCursor()
    };
    QTextBlockFormat textBlockFormat {textCursor.blockFormat()};

    textBlockFormat.setAlignment(aligment);
    textCursor.mergeBlockFormat(textBlockFormat);

    filesTextEdit[tabWidget->currentIndex()]->setTextCursor(textCursor);
}

void Notepad::lineSpacing(qreal lineSpacing)
{
    QTextCursor textCursor {
        filesTextEdit[tabWidget->currentIndex()]->textCursor()
    };
    QTextBlockFormat textBlockFormat {textCursor.blockFormat()};

    textBlockFormat.setLineHeight(lineSpacing,
                                  QTextBlockFormat::LineDistanceHeight);
    textCursor.mergeBlockFormat(textBlockFormat);

    filesTextEdit[tabWidget->currentIndex()]->setTextCursor(textCursor);
}

void Notepad::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply {
        QMessageBox::question(this, "Notatnik",
                              "Jesteś pewien że chcesz wyjść?\n\n"
                              "Niezapisane zmiany zostaną utracone!",
                              QMessageBox::No | QMessageBox::Yes,
                              QMessageBox::Yes)
    };

    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}
