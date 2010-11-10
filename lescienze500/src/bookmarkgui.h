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
#include <QHash>
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
    void fillFavoriteInfo( const QString& id , const QString& Id_f ) ;

    void clearFavoriteInfo() ;

    void fillStates() ;
    void fillEvaluations() ;

    void fillBookmarkMenu( QAction* def_act = 0 ) ;


    void appendFolder( QString name ) ;
    void appendFavorite( QString id ) ;

    bool removeFavorite() ;
    bool removeFolder() ;

    bool addState( QString new_state ) ;
    bool changeState( QString new_state ) ;
    bool removeState() ;

    bool addEvaluation( QString stars ) ;
    bool changeEvaluation( QString stars ) ;
    bool removeEvaluation() ;

    bool renameFolder() ;

   // void open() ;
    void open( QString id = "" ) ;

    void setMenuFavorites( QMenu* menu_f ) { if ( bookmarkMenu == 0 ) bookmarkMenu = menu_f ; }

private:



    Ui::BookmarkGui *ui;

    QMenu menuFavorites ;
    QMenu menuStates ;

    QMenu *bookmarkMenu ;

    QTreeWidgetItem* current_favorites_item ;

    QTreeWidgetItem* cutted_item ;
    bool cut_state ;

    QString current_favorite ;
    QString current_favorite_id ;

    bool first_open ;

    QHash<QString,QAction*> menu_ptr ;
    QHash<QString,QAction*> menu_states_ptr ;

    void buildMenuFavorites() ;
    void buildMenuStates() ;

    enum item_types { item_folder=1001 , item_article , item_state , item_evaluation } ;
    enum menuFavoritesEntry { newFolder=1 , cut=2 , cancelCut=4 , paste=8 , remove=16 , renameFold=32 } ;
    enum menuStatesEntry { NewState=1 , RenameState=2 , RemoveState=4 } ;

    void enableEntryMenuFavorites( int e ) ;
    void disableEntryMenuFavorites( int e ) ;

    void enableEntryMenuStates( int e ) ;
    void disableEntryMenuStates( int e ) ;

    void fillCategorieRec( const QString& name , const QString& id , QTreeWidgetItem* parent ) ;

    void fillBookmarkMenuRec( QMenu *menu , QString name , QString id ) ;

    void setFolderItemDecorations( QTreeWidgetItem* item , const QString& name , const QString& id ) ;
    void setArticleItemDecorations( QTreeWidgetItem* item , const QString& id_articolo , const QString& id ) ;

    void setArticleActionDecorations( QAction* action , const QString& id_articolo , const QString& id ) ;

    bool searchItem( QString id ) ;

    void showDataBaseMessage() ;

    SetName name_d ;

public slots:
    void on_favoriteActivated( QTreeWidgetItem * item, int column ) ;
    void on_newFolder() ;
    void on_remaneFolder() ;
    void on_addFavorite() ;
    void on_remove() ;
    void on_openPdf() ;
    void on_contextMenu( const QPoint& pos ) ;
    void on_contextMenuStates( const QPoint& pos ) ;
    void on_saveComment() ;
    void on_stateChanged( int index ) ;
    void on_evaluationChanged( int index ) ;

    void on_currentChanged( int c ) ;

    void on_newState() ;
    void on_removeState() ;
    void on_renameState() ;

    void on_selectedChanged() ;
    void on_cutItem() ;
    void on_pasteItem() ;
    void on_cancelCut() ;

    void on_commentChanged() ;
    void on_openBrowser() ;

    void on_favoriteSelected() ;

signals:
    void sig_openPdf( int id ) ;
    void sig_openBrowser( int id ) ;
};

#endif // BOOKMARKGUI_H
