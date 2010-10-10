#ifndef BOOKMARKGUI_H
#define BOOKMARKGUI_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QMenu>

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
    void fillFavoriteInfo( const QString& id ) ;

private:
    Ui::BookmarkGui *ui;
    QMenu menuFavorites ;

    void buildMenuFavorites() ;

    static const int item_folder = 1001 ;
    static const int item_article = 1002 ;

    void fillCategorieRec( const QString& name , QTreeWidgetItem* parent ) ;
    void setFolderItemDecorations( QTreeWidgetItem* item , const QString& name ) ;
    void setArticleItemDecorations( QTreeWidgetItem* item , const QString& id ) ;

public slots:
    void on_favoriteActivated( QTreeWidgetItem * item, int column ) ;
    void on_newFolder() ;
};

#endif // BOOKMARKGUI_H
