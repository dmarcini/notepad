#ifndef FILE_TEXT_EDIT_HPP_
#define FILE_TEXT_EDIT_HPP_

#include <QTextEdit>
#include <QWidget>
#include <QString>

class FileTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FileTextEdit(QWidget *parent = nullptr);

    void setTextChanged(bool textChanged);
    bool getTextChanged();

    void setFilename(QString filename);
    QString getFilename();
private slots:
    void on_actionTextChanged_changed();
private:
    bool textChanged {false};
    QString filename {""};
};

#endif // FILE_TEXT_EDIT_HPP_
