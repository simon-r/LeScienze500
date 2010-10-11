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

#include "bookmarkgui.h"
#include "ui_bookmarkgui.h"
#include "bookmark.h"
#include "QDebug"
#include <QAction>

BookmarkGui::BookmarkGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkGui)
{
    ui->setupUi(this);

    current_favorites_item = 0 ;
    current_favorite = "" ;

    buildMenuFavorites() ;
    this->setWindowTitle( "Preferiti (alpha release)" );

    connect( ui->treeCategorie , SIGNAL(itemClicked(QTreeWidgetItem*,int)) , this , SLOT(on_favoriteActivated(QTreeWidgetItem*,int)) ) ;
    connect( ui->AddFavorite , SIGNAL(clicked()) , this , SLOT(on_addFavorite()) ) ;
    connect( ui->OpenPDF , SIGNAL(clicked()) , this , SLOT(on_openPdf()) ) ;
}

void BookmarkGui::open()
{
    current_favorite = "" ;
    current_favorites_item = 0 ;

    fillCategorie() ;

    setModal( true ) ;
    show() ;
    exec() ;
}

void BookmarkGui::open( QString id )
{
    current_favorites_item = 0 ;

    fillCategorie() ;

    setModal( true ) ;
    show() ;

    current_favorite = id ;
    this->fillFavoriteInfo( id ) ;

    exec() ;
}

void BookmarkGui::buildMenuFavorites()
{
    QAction* new_folder = new QAction( tr( "Nuova Cartella" ) , 0 );
    connect( new_folder , SIGNAL(triggered()) , this , SLOT(on_newFolder()) ) ;
    this->menuFavorites.addAction( new_folder ) ;


    this->menuFavorites.addSeparator() ;
    this->menuFavorites.addAction( tr( "Taglia" ) ) ;
    this->menuFavorites.addAction( tr( "Copia" ) ) ;
    this->menuFavorites.addAction( tr( "Incolla" ) ) ;
    this->menuFavorites.addSeparator() ;
    this->menuFavorites.addAction(tr( "Rimuovi" ) ) ;
    this->menuFavorites.addSeparator() ;

    ui->mainFavoritesMenu->setMenu( &this->menuFavorites );
}

BookmarkGui::~BookmarkGui()
{
    delete ui;
}

void BookmarkGui::fillCategorie()
{
     QList<QTreeWidgetItem *> items;
     Bookmark bk ;
     QueryResult qr ;

     bk.getRootFolders( qr );
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
         QString name = qr.getField( "Categoria" , itr ) ;
         QString id = qr.getField( "Id" , itr ) ;
         QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_folder ) ;
         setFolderItemDecorations( item , name , id ) ;
         fillCategorieRec( name , item ) ;
         items.append( item ) ;
     }

     // ATTENIONE!
     bk.getFavoritesByParentId( qr , "" ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdArticolo" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_article ) ;
          this->setArticleItemDecorations( item , name ) ;
          items.append( item );
     }

     ui->treeCategorie->clear();
     current_favorites_item = items.first() ;
     ui->treeCategorie->insertTopLevelItems(0, items);
}

void BookmarkGui::fillCategorieRec( const QString& name , QTreeWidgetItem* parent )
{
     Bookmark bk ;
     QueryResult qr ;

     bk.getFolders( qr , name ) ;

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "Categoria" , itr ) ;
          QString id = qr.getField( "Id" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent , BookmarkGui::item_folder ) ;
          this->setFolderItemDecorations( item , name , id ) ;
          this->fillCategorieRec( name , item ) ;
     }

     bk.getFavoritesByParent( qr , name ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdArticolo" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent , BookmarkGui::item_article ) ;
          this->setArticleItemDecorations( item , name ) ;
     }
}

void BookmarkGui::setFolderItemDecorations( QTreeWidgetItem* item , const QString& name , const QString& id )
{
    QIcon folder(":/icons/crystal/folder.png") ;
    item->setIcon( 0 , folder);
    item->setText( 0 , name );

    item->setText( 1 , id ) ;
}

void BookmarkGui::setArticleItemDecorations( QTreeWidgetItem* item , const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id ) ;

    QIcon doc_icon(":/icons/crystal/doc-icon.png") ;
    item->setIcon( 0 , doc_icon );

    if ( article.size() == 0 ) return ;
    item->setText( 0 , article.getField( "Titolo" , article.begin() ) );

    item->setText( 1 , id );
}


void BookmarkGui::fillFavoriteInfo( const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id ) ;

    if ( article.size() == 0 ) return ;

    ui->Title->setHtml( article.getField( "Titolo" , article.begin() ) );
    ui->Abstract->setHtml( article.getField( "Abstract" , article.begin() ) );

    this->current_favorite = id ;
}


void BookmarkGui::on_favoriteActivated( QTreeWidgetItem * item, int column )
{
    if ( item->type() == BookmarkGui::item_article  )
    {
        this->fillFavoriteInfo( item->text( 1 ) );
    }

    this->current_favorites_item = item ;
}

void BookmarkGui::appendFolder( QString name )
{
    Bookmark bk ;

    QString parent = "" ;
    QString parent_id = "" ;

    QTreeWidgetItem* parent_item ;
    QTreeWidgetItem* new_folder ;

    if ( this->current_favorites_item == 0 )
    {
        parent = "" ; // Unused !!!!
        parent_id = "1" ; // Unused !!!!
    }
    else if ( this->current_favorites_item->type() == BookmarkGui::item_folder )
    {
        parent = this->current_favorites_item->text( 0 ) ;
        parent_id = this->current_favorites_item->text( 1 ) ;
        parent_item = this->current_favorites_item ;
    }
    else if ( this->current_favorites_item->type() == BookmarkGui::item_article )
    {
        parent = this->current_favorites_item->parent()->text( 0 ) ;
        parent_id = this->current_favorites_item->parent()->text( 1 ) ;
        parent_item = this->current_favorites_item->parent() ;
    }

     QPair<QString,QString> folder_info = bk.addFolderId( parent_id , name ) ;

     new_folder = new QTreeWidgetItem( parent_item , BookmarkGui::item_folder ) ;
     this->setFolderItemDecorations( new_folder , folder_info.second , folder_info.first ) ;
}

void BookmarkGui::appendFavorite( QString id )
{
    Bookmark bk ;

    QString parent = "" ;
    QString parent_id = "" ;

    QTreeWidgetItem* parent_item = 0 ;
    QTreeWidgetItem* new_favorite ;

    if ( this->current_favorites_item == 0 )
    {
        parent = "" ;
        parent_id = "1" ;
    }
    else if ( this->current_favorites_item->type() == BookmarkGui::item_folder )
    {
        parent = this->current_favorites_item->text( 0 ) ;
        parent_id = this->current_favorites_item->text( 1 ) ;

        parent_item = this->current_favorites_item ;
    }
    else if ( this->current_favorites_item->type() == BookmarkGui::item_article )
    {
        parent = this->current_favorites_item->parent()->text( 0 ) ;
        parent_id = this->current_favorites_item->parent()->text( 1 ) ;

        parent_item = this->current_favorites_item->parent() ;
    }

    QString title = bk.addFavoriteId( parent_id , id ) ;

    new_favorite = new QTreeWidgetItem( parent_item , BookmarkGui::item_article ) ;
    this->setArticleItemDecorations( new_favorite , id ) ;
}

void BookmarkGui::on_newFolder()
{
    this->name_d.open();

    if ( this->name_d.text().isEmpty() )
        return ;

    this->appendFolder( this->name_d.text() ) ;
}

void BookmarkGui::on_addFavorite()
{
    if ( this->current_favorite == "" ) return ;
    this->appendFavorite( this->current_favorite );
}

void BookmarkGui::on_openPdf()
{
    emit this->sig_openPdf( this->current_favorite.toInt() );
}
