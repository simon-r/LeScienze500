//     Copyright 2010 Simone Riva
//     mail:  simone (dot) rva (lumachina) gmail.com
//
//    This file is part of LeScienze500.
//
//    LeScienze500 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    LeScienze500 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with LeScienze500.  If not, see <http://www.gnu.org/licenses/>.

#ifndef BOOKMARKGUI_H
#define BOOKMARKGUI_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QMenu>
#include "setname.h"

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
    void appendFolder( QString name ) ;
    void appendFavorite( QString id ) ;

    void open() ;
    void open( QString id ) ;

private:
    Ui::BookmarkGui *ui;
    QMenu menuFavorites ;
    QTreeWidgetItem* current_favorites_item ;
    QString current_favorite ;

    void buildMenuFavorites() ;

    static const int item_folder = 1001 ;
    static const int item_article = 1002 ;

    void fillCategorieRec( const QString& name , QTreeWidgetItem* parent ) ;
    void setFolderItemDecorations( QTreeWidgetItem* item , const QString& name , const QString& id ) ;
    void setArticleItemDecorations( QTreeWidgetItem* item , const QString& id ) ;

    SetName name_d ;

public slots:
    void on_favoriteActivated( QTreeWidgetItem * item, int column ) ;
    void on_newFolder() ;
    void on_addFavorite() ;
    void on_openPdf() ;

signals:
    void sig_openPdf( int id ) ;
};

#endif // BOOKMARKGUI_H
