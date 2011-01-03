#ifndef DICTIONARYGUI_H
#define DICTIONARYGUI_H

#include <QDialog>
#include "dictionary.h"

namespace Ui {
    class DictionaryGui;
}

class DictionaryGui : public QDialog
{
    Q_OBJECT

public:
    explicit DictionaryGui(QWidget *parent = 0);
    ~DictionaryGui();

    void openDictionary() ;
private:
    Ui::DictionaryGui *ui;

    Dictionary dict ;
};

#endif // DICTIONARYGUI_H
