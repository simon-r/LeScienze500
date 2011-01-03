#ifndef DICTIONARYGUI_H
#define DICTIONARYGUI_H

#include <QDialog>
#include <QMenu>


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

    void search( QString word ) ;
    void searchWordYear( QString word ) ;
    void searchWordCategoria ( QString word ) ;

private:
    Ui::DictionaryGui *ui;

    Dictionary dict ;

    QMenu* menu_search ;
    QActionGroup* set_search ;


    QAction *menu_search_word ;
    QAction *menu_search_word_year ;
    QAction *menu_search_word_categoria ;


private slots:
    void on_searchClicked() ;
    void on_searchStatusChanged() ;
};

#endif // DICTIONARYGUI_H
