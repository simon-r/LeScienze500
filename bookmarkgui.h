#ifndef BOOKMARKGUI_H
#define BOOKMARKGUI_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
    class BookmarkGui;
}

class BookmarkGui : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkGui(QWidget *parent = 0);
    ~BookmarkGui();

    void fillCategorie() ;

private:
    Ui::BookmarkGui *ui;

    void fillCategorieRec( const QString& name , QTreeWidgetItem* parent ) ;
    void setFolderItemDecorations( QTreeWidgetItem* item , const QString& name ) ;
    void setArticleItemDecorations( QTreeWidgetItem* item , const QString& name ) ;
};

#endif // BOOKMARKGUI_H
