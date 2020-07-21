#include "file_text_edit.hpp"

#include <QWidget>
#include <QTextEdit>
#include <QFont>
#include <QString>

FileTextEdit::FileTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    setFont(QFont("Arial", 12));

    connect(this, SIGNAL(textChanged()),
            this, SLOT(on_actionTextChanged_changed()));
}

void FileTextEdit::on_actionTextChanged_changed()
{
    textChanged = true;
}

void FileTextEdit::setTextChanged(bool textChanged)
{
    this->textChanged = textChanged;
}

bool FileTextEdit::getTextChanged()
{
    return textChanged;
}

void FileTextEdit::setFilename(QString filename)
{
    this->filename = filename;
}

QString FileTextEdit::getFilename()
{
    return filename;
}
