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
#include <QMessageBox>
#include "querydb.h"

BookmarkGui::BookmarkGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkGui)
{
    ui->setupUi(this);

    current_favorites_item = 0 ;
    current_favorite = "" ;
    current_favorite_id = "" ;

    cutted_item = 0 ;
    cut_state = false ;

    buildMenuFavorites() ;
    this->setWindowTitle( "Preferiti (beta release)" );

    ui->treeCategorie->setUpdatesEnabled( true );
    ui->treeCategorie->setContextMenuPolicy( Qt::CustomContextMenu );

    connect( ui->treeCategorie , SIGNAL(itemSelectionChanged()) , this , SLOT(on_selectedChanged()) ) ;
    connect( ui->treeCategorie ,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_contextMenu(QPoint))) ;

    connect( ui->AddFavorite , SIGNAL(clicked()) , this , SLOT(on_addFavorite()) ) ;
    connect( ui->OpenPDF , SIGNAL(clicked()) , this , SLOT(on_openPdf()) ) ;

    connect( ui->SaveComment , SIGNAL(clicked()) , this , SLOT(on_saveComment()) ) ;

    connect( ui->Comments , SIGNAL(textChanged()) , this , SLOT(on_commentChanged()) ) ;

    connect( ui->OpenReview , SIGNAL(clicked()) , this , SLOT(on_openBrowser()) ) ;

    connect( ui->State , SIGNAL(activated(int)) , this , SLOT(on_stateChanged(int)) ) ;
    connect( ui->treeStates , SIGNAL(itemSelectionChanged()) , this , SLOT(on_selectedChanged()) ) ;
}

void BookmarkGui::open()
{
    current_favorite = "" ;
    current_favorites_item = 0 ;
    current_favorite_id = "" ;

    ui->Title->clear();
    ui->Abstract->clear();
    ui->Comments->clear();

    fillCategorie() ;
    fillStates() ;

    ui->SaveComment->setDisabled( true );

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

    ui->Title->clear();
    ui->Abstract->clear();
    ui->Comments->clear();

    current_favorite = id ;
    this->fillFavoriteInfo( id ) ;

    ui->SaveComment->setDisabled( true );

    exec() ;
}

void BookmarkGui::buildMenuFavorites()
{
    QAction* new_folder = new QAction( tr( "Nuova Cartella" ) , 0 );
    connect( new_folder , SIGNAL(triggered()) , this , SLOT(on_newFolder()) ) ;
    this->menuFavorites.addAction( new_folder ) ;

    this->menuFavorites.addSeparator() ;

    QAction* cut_item = new QAction( tr( "Taglia" ) , 0 );
    connect( cut_item , SIGNAL(triggered()) , this , SLOT(on_cutItem()) ) ;
    this->menuFavorites.addAction( cut_item ) ;

    QAction* copy_item = new QAction( tr( "Copia" ) , 0 );
    copy_item->setDisabled( true );
    this->menuFavorites.addAction( copy_item ) ;

    QAction* paste = new QAction( tr( "Incolla" ) , 0 );
    connect( paste , SIGNAL(triggered()) , this , SLOT(on_pasteItem()) ) ;
    this->menuFavorites.addAction( paste ) ;

    QAction* cancel_cut = new QAction( tr( "Annulla Taglia" ) , 0 );
    connect( cancel_cut , SIGNAL(triggered()) , this , SLOT(on_cancelCut()) ) ;
    this->menuFavorites.addAction( cancel_cut ) ;

    this->menuFavorites.addSeparator() ;

    QAction* remove = new QAction( tr( "Rimuovi" ) , 0 );
    connect( remove , SIGNAL(triggered()) , this , SLOT(on_remove()) ) ;
    this->menuFavorites.addAction( remove ) ;

    this->menuFavorites.addSeparator() ;

    QAction* rename_folder = new QAction( tr( "Cambia nome" ) , 0 );
    connect( rename_folder , SIGNAL(triggered()) , this , SLOT(on_remaneFolder()) ) ;
    this->menuFavorites.addAction( rename_folder ) ;

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
         QString name = qr.getField( "Folder" , itr ) ;
         QString id = qr.getField( "Id" , itr ) ;
         QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_folder ) ;
         setFolderItemDecorations( item , name , id ) ;
         fillCategorieRec( name , id , item ) ;
         items.append( item ) ;
     }

     // ATTENIONE!
     bk.getFavoritesByParentId( qr , "" ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdArticolo" , itr ) ;
          QString id = qr.getField( "Id" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_article ) ;
          this->setArticleItemDecorations( item , name , id ) ;
          items.append( item );
     }

     ui->treeCategorie->clear();

     if ( items.isEmpty() )
     {
         showDataBaseMessage() ;
         return ;
     }

     current_favorites_item = items.first() ;
     ui->treeCategorie->insertTopLevelItems(0, items);
     current_favorites_item->setExpanded( true ) ;
}

void BookmarkGui::fillCategorieRec( const QString& name , const QString& parent_id , QTreeWidgetItem* parent )
{
     Bookmark bk ;
     QueryResult qr ;

     bk.getFoldersId( qr , parent_id ) ;

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "Folder" , itr ) ;
          QString id = qr.getField( "Id" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent , BookmarkGui::item_folder ) ;
          this->setFolderItemDecorations( item , name , id ) ;
          this->fillCategorieRec( name , id , item ) ;
     }

     bk.getFavoritesByParent( qr , name ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdEntry" , itr ) ;
          QString id = qr.getField( "Id" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent , BookmarkGui::item_article ) ;
          this->setArticleItemDecorations( item , name , id ) ;
     }
}

void BookmarkGui::setFolderItemDecorations( QTreeWidgetItem* item , const QString& name , const QString& id )
{
    QIcon folder(":/icons/crystal/folder.png") ;
    item->setIcon( 0 , folder);
    item->setText( 0 , name );

    item->setText( 1 , id ) ;
}

void BookmarkGui::setArticleItemDecorations( QTreeWidgetItem* item , const QString& id_articolo , const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id_articolo ) ;

    QIcon doc_icon(":/icons/crystal/doc-icon.png") ;
    item->setIcon( 0 , doc_icon );

    if ( article.size() == 0 ) return ;
    item->setText( 0 , article.getField( "Titolo" , article.begin() ) );
    item->setText( 1 , id_articolo ) ;
    item->setText( 2 , id ) ;
}


void BookmarkGui::fillFavoriteInfo( const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id ) ;

    if ( article.size() == 0 ) return ;

    QString query_autori = "SELECT autore FROM autori WHERE id in (SELECT idautore FROM articoli_autori WHERE idarticolo = " ;
    query_autori += id ;
    query_autori += " ) " ;

    QueryDB db ;
    QueryResult qr_autori =  db.execQuery( query_autori ) ;

    QString query_data = "SELECT mese,anno FROM riviste WHERE id = " ;
    query_data += article.getField( "IdRivista" , article.begin() ) ;

    QueryResult qr_data =  db.execQuery( query_data ) ;

    QString abstract ;
    abstract += article.getField( "Abstract" , article.begin() ) ;
    abstract += "<br/> <br/> <i>" ;

    for ( QueryResult::iterator itr = qr_autori.begin() ; itr < qr_autori.end() ; itr++ )
    {
        abstract += qr_autori.getField( "Autore" , itr ) ;
        if ( itr < qr_autori.end() - 1 ) abstract += "; " ;
    }

    abstract += "</i><br/>" ;
    abstract += qr_data.getField( "Mese" , qr_data.begin() ) ;
    abstract += " " ;
    abstract += qr_data.getField( "Anno" , qr_data.begin() ) ;

    QString title = "<b>" ;
    title += article.getField( "Titolo" , article.begin() ) ;
    title += "</b>" ;

    ui->Title->setHtml( title ) ;
    ui->Abstract->setHtml( abstract ) ;

    this->current_favorite = id ;

    if ( bk.isFavoriteBookmarked(id) )
        ui->AddFavorite->setDisabled( true );
    else
        ui->AddFavorite->setDisabled( false );
}

void BookmarkGui::fillFavoriteInfo( const QString& id , const QString& Id_f )
{
    this->fillFavoriteInfo( id ) ;
    this->current_favorite_id = Id_f ;

    Bookmark bk ;
    QueryResult res ;

    bk.getComment( res , Id_f ) ;

    if ( res.empty() )
        ui->Comments->setHtml( "" ) ;
    else
        ui->Comments->setHtml( res.getField(0,0) ) ;

     ui->SaveComment->setDisabled( true );

     res.clear() ;
     bk.getState( res , Id_f ) ;
     if ( !res.empty() )
     {
         int index = ui->State->findText( res.getField(0,0) ) ;
         ui->State->setCurrentIndex( index );
     }
     else
         ui->State->setCurrentIndex( 0 );
 }


void BookmarkGui::fillStates()
{
    QueryResult states ;
    Bookmark bk ;

    bk.getStates( states );
    QList<QTreeWidgetItem*> items;

    ui->State->clear();

    int index = 0 ;
    ui->State->insertItem( index , tr("Non definito") ) ;

    for ( QueryResult::iterator itr = states.begin() ; itr < states.end() ; itr++ )
    {
        QString name = states.getField( "StateName" , itr ) ;
        QString id = states.getField( "Id" , itr ) ;
        QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_state ) ;

        QueryResult entries ;
        bk.getFavoritesByState( entries , name ) ;
        for ( QueryResult::iterator itr_b = entries.begin() ; itr_b < entries.end() ; itr_b++ )
        {
            QString id_entry = entries.getField( "IdEntry" , itr_b ) ;
            QString id = entries.getField( "Id" , itr_b ) ;
            QTreeWidgetItem* entry_item = new QTreeWidgetItem( item , BookmarkGui::item_article ) ;
            this->setArticleItemDecorations( entry_item , id_entry , id ) ;
        }

        this->setFolderItemDecorations( item , name , id ) ;
        items.append( item );

        ui->State->insertItem( ++index , name ) ;
    }

    ui->treeStates->clear();
    ui->treeStates->addTopLevelItems( items ) ;
}

void BookmarkGui::appendFolder( QString name )
{
    Bookmark bk ;

    QString parent = "" ;
    QString parent_id = "" ;

    QTreeWidgetItem* parent_item = 0 ;
    QTreeWidgetItem* new_folder ;

    if ( this->current_favorites_item == 0 )
    {
        parent = "" ; // Unused !!!!
        parent_id = "1" ; // Unused !!!!
        parent_item = 0 ;
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
     new_folder->setExpanded( true );
     ui->treeCategorie->setCurrentItem( new_folder );
}

bool  BookmarkGui::renameFolder()
{
    if ( this->current_favorites_item == 0 ) return false;
    if ( this->current_favorites_item->type() == BookmarkGui::item_article ) return false ;

    QString curr_name = this->current_favorites_item->text(0) ;
    QString folder_id = this->current_favorites_item->text(1) ;

    name_d.open( curr_name );

    QString new_name = name_d.text() ;

    if ( new_name.isEmpty() ) return false ;

    Bookmark bk ;
    bool flag = bk.renameFolder( folder_id , new_name ) ;

    if ( flag ) this->current_favorites_item->setText( 0 , new_name ) ;
    return flag ;
}

bool BookmarkGui::removeFolder()
{
    if ( this->current_favorites_item == 0 ) return false;
    if ( this->current_favorites_item->type() == BookmarkGui::item_article ) return false ;

    QTreeWidgetItem* tmp_parent = this->current_favorites_item->parent() ;

    Bookmark bk ;
    bool fr = bk.removeFolder( this->current_favorites_item->text( 1 ) ) ;

    if ( !fr ) return false ;

    tmp_parent->removeChild( this->current_favorites_item ) ;
    this->current_favorites_item = tmp_parent ;
    ui->treeCategorie->setCurrentItem( tmp_parent ) ;

    return true ;
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

    QPair<QString,QString> title ;
    title = bk.addFavoriteId( parent_id , id ) ;

    if ( title.first.isEmpty() )
        return ;

    new_favorite = new QTreeWidgetItem( parent_item , BookmarkGui::item_article ) ;
    this->setArticleItemDecorations( new_favorite , id , title.first  ) ;
    parent_item->setExpanded( true ) ;
    ui->treeCategorie->setCurrentItem( new_favorite );
}

 bool BookmarkGui::removeFavorite()
 {
     if ( this->current_favorites_item == 0 ) return false;
     if ( this->current_favorites_item->type() == BookmarkGui::item_folder ) return false ;

     QTreeWidgetItem* tmp_parent = this->current_favorites_item->parent() ;

     Bookmark bk ;
     bk.removeFavorite( tmp_parent->text( 1 ) , this->current_favorites_item->text( 2 ) ) ;

     tmp_parent->removeChild( this->current_favorites_item ) ;
     this->current_favorites_item = tmp_parent ;
     ui->treeCategorie->setCurrentItem( tmp_parent );

     return true ;
 }

 bool BookmarkGui::changeState( QString new_state )
 {

     qDebug() << "BookmarkGui::changeState" ;
     Bookmark bk ;
     QueryResult favorite ;

     bk.getFavoriteFullData( favorite , this->current_favorite );

     QList<QTreeWidgetItem *> list =
             ui->treeStates->findItems ( favorite.getField( "Titolo" , favorite.begin() ) , Qt::MatchExactly ) ;

     QList<QTreeWidgetItem *> list_state =
             ui->treeStates->findItems ( new_state , Qt::MatchExactly ) ;

    qDebug() << "BookmarkGui::changeState 0.5" <<
            favorite.getField( "Titolo" , favorite.begin() ) << list.size() << list_state.size() ;

    if ( list.isEmpty() || list.size() > 1 ) return false ;

     if ( list_state.isEmpty() || list_state.size() > 1 ) return false ;

      qDebug() << "BookmarkGui::changeState 1" ;

     QTreeWidgetItem *item = list.first() ;
     QTreeWidgetItem *state = list_state.first() ;
     if ( item->type() != BookmarkGui::item_article ) return false ;
     if ( state->type() != BookmarkGui::item_state ) return false ;

     qDebug() << "BookmarkGui::changeState 2" ;

     int index = item->parent()->indexOfChild( item ) ;
     item = item->parent()->takeChild( index ) ;

     state->addChild( item );

     state->setExpanded( true );
 }


 void BookmarkGui::on_cutItem()
 {
     if ( this->cut_state == true )
     {
         this->cutted_item->setDisabled( false );
         this->cut_state = false ;
     }

     if ( this->current_favorites_item->type() == BookmarkGui::item_folder )
     {
         this->cutted_item = this->current_favorites_item ;
         this->cutted_item->setDisabled( true ) ;
         this->cut_state = true ;
         qDebug() << "cut folder" ;
     }
     else if ( this->current_favorites_item->type() == BookmarkGui::item_article )
     {
         this->cutted_item = this->current_favorites_item ;
         this->cutted_item->setDisabled( true ) ;
         this->cut_state = true ;
     }
     else
     {
         return ;
     }
 }



 void BookmarkGui::on_pasteItem()
 {
     if ( this->cut_state == true
          && this->current_favorites_item->type() == BookmarkGui::item_folder )
     {
         Bookmark bk ;

         QTreeWidgetItem* old_parent = this->cutted_item->parent() ;

         if ( old_parent != this->current_favorites_item
              && this->cutted_item != this->current_favorites_item )
         {
             if ( this->cutted_item->type() == BookmarkGui::item_folder )
                bk.moveFolder( this->cutted_item->text(1) , this->current_favorites_item->text(1) ) ;
             else if ( this->cutted_item->type() == BookmarkGui::item_article )
                bk.moveFavorite( this->cutted_item->text(2) , this->current_favorites_item->text(1) ) ;
             else
                 return ;

             int index = old_parent->indexOfChild( this->cutted_item ) ;
             this->cutted_item = old_parent->takeChild( index ) ;
             this->current_favorites_item->addChild( this->cutted_item );
             this->cutted_item->setDisabled( false ) ;
             this->cutted_item->parent()->setExpanded( true ) ;
             ui->treeCategorie->setCurrentItem( this->cutted_item );
         }

         this->cutted_item->setDisabled( false ) ;
         this->cutted_item = 0 ;
         this->cut_state = false ;
         return ;
     }
 }

 void BookmarkGui::showDataBaseMessage()
 {
     QMessageBox msgBox;
     msgBox.setMinimumWidth( 300 );
     msgBox.setText( tr("Bookmark inutillizabile") );
     msgBox.setInformativeText( tr("Sembra esistere un file di bookmark ma e' inutilizzabile. <br/> Ne vuoi creare uno nuovo?")  );
     msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

     int ret = msgBox.exec();

     Bookmark bk ;

     switch (ret) {
     case QMessageBox::Ok:
           bk.initBookmarkForce() ;
           this->fillCategorie() ;
           break;
       case QMessageBox::Cancel:
           // Cancel was clicked
           break;
       default:
           // should never be reached
           break;
     }
 }

 void BookmarkGui::on_cancelCut()
 {
     if ( this->cutted_item != 0 )
     {
         this->cutted_item->setDisabled( false ) ;
         this->cutted_item = 0 ;
         this->cut_state = false ;
         return ;
     }
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

void BookmarkGui::on_remove()
{
    this->removeFavorite() ;
    this->removeFolder() ;
}

void BookmarkGui::on_openPdf()
{
    emit this->sig_openPdf( this->current_favorite.toInt() );
}

void BookmarkGui::on_contextMenu( const QPoint& pos )
{
    QPoint global_pos = ui->treeCategorie->mapToGlobal( pos ) ;
    this->menuFavorites.exec( global_pos ) ;
}


void BookmarkGui::on_favoriteActivated( QTreeWidgetItem * item, int column )
{
    if ( item->type() == BookmarkGui::item_article  )
    {
        this->fillFavoriteInfo( item->text( 1 ) );
    }

    this->current_favorites_item = item ;
}


void BookmarkGui::on_selectedChanged()
{
    QList<QTreeWidgetItem *> item_list ;

    item_list = ui->treeCategorie->selectedItems() ;

    if ( item_list.isEmpty() )
    {
        item_list = ui->treeStates->selectedItems() ;
        if ( item_list.isEmpty() ) return ;
    }

    QTreeWidgetItem *item = item_list.first() ;

    if ( item->type() == BookmarkGui::item_article )
    {
        this->fillFavoriteInfo( item->text(1) , item->text(2) );
    }

    this->current_favorites_item = item ;
}

void BookmarkGui::on_remaneFolder()
{
    this->renameFolder() ;
}

void BookmarkGui::on_saveComment()
{
    Bookmark bk ;

    if ( this->current_favorite.isEmpty() ) return ;

    bk.setComment( ui->Comments->toPlainText().toUtf8() , this->current_favorite_id ) ;
    ui->SaveComment->setDisabled( true );
}

void BookmarkGui::on_commentChanged()
{
    ui->SaveComment->setDisabled( false );
}

void BookmarkGui::on_openBrowser()
{
    if ( this->current_favorite.isEmpty() ) return ;
    emit sig_openBrowser( this->current_favorite.toInt() ) ;
}

void BookmarkGui::on_stateChanged( int index )
{
    qDebug() << index ;
    if ( index == 0 ) return ;
    if ( this->current_favorite.isEmpty() ) return ;

    QString state_name = ui->State->itemText( index ) ;

    Bookmark bk ;
    bk.setState( state_name , this->current_favorite_id ) ;
    this->changeState( state_name ) ;
}
