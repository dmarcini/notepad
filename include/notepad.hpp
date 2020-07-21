#ifndef NOTEPAD_HPP_
#define NOTEPAD_HPP_

#include <QMainWindow>
#include <QTabWidget>
#include <QVector>
#include <QString>
#include <QCloseEvent>

#include "file_text_edit.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT
public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();
private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionPrinter_triggered();
    void on_actionExit_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionDeleteSelectedText_triggered();
    void on_actionSelectAll_triggered();
    void on_actionBackgroundColor_triggered();
    void on_actionTextColor_triggered();
    void on_actionFont_triggered();
    void on_actionAboutAuthor_triggered();
    void on_actionChangeTab_triggered(int index);
    void on_actionCloseTab_triggered(int index);
    void on_actionToLeft_triggered();
    void on_actionToCenter_triggered();
    void on_actionToRight_triggered();
    void on_actionJustification_triggered();
    void on_actionLineSpacing1_0_triggered();
    void on_actionLineSpacing1_15_triggered();
    void on_actionLineSpacing1_5_triggered();
    void on_actionLineSpacing2_0_triggered();
    void on_actionLineSpacing2_5_triggered();
    void on_actionLineSpacing3_0_triggered();
private:  
    void newFile();
    void saveFile(const QString &filename);

    void textAlign(const Qt::Alignment &aligment);
    void lineSpacing(qreal lineSpacing);

    void closeEvent (QCloseEvent *event);

    Ui::Notepad *ui;

    QTabWidget *tabWidget;
    QVector<FileTextEdit*> filesTextEdit;

    int totalFilesNumber {0};
};

#endif // NOTEPAD_HPP_
