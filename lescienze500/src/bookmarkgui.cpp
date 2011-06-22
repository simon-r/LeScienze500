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

    Bookmark bk ;
    bk.autoBackup() ;

    ui->setupUi(this);

    first_open = true ;

    current_favorites_item = 0 ;
    current_favorite = "" ;
    current_favorite_id = "" ;

    cutted_item = 0 ;
    cut_state = false ;

    bookmarkMenu = 0 ;

    buildMenuFavorites() ;
    buildMenuStates() ;

    this->setWindowTitle( tr("Preferiti") );

    ui->treeCategorie->setUpdatesEnabled( true );
    ui->treeStates->setUpdatesEnabled( true );
    ui->treeCategorie->setAnimated( true );

    ui->treeCategorie->setColumnCount(3);
    ui->treeCategorie->setColumnHidden( 1 , true ) ;
    ui->treeCategorie->setColumnHidden( 2 , true ) ;

    ui->treeStates->setColumnCount(2);
    ui->treeStates->setColumnHidden( 1 , true );
    ui->treeStates->setAnimated( true );

    ui->treeEvaluations->setColumnCount(2);
    ui->treeEvaluations->setColumnHidden( 1 , true );
    ui->treeEvaluations->setAnimated( true );

    ui->treeCategorie->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->treeStates->setContextMenuPolicy( Qt::CustomContextMenu );

    connect( ui->treeCategorie , SIGNAL(itemSelectionChanged()) , this , SLOT(on_selectedChanged()) ) ;
    connect( ui->treeCategorie ,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_contextMenu(QPoint))) ;

    connect( ui->treeStates ,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_contextMenuStates(QPoint))) ;

    connect( ui->AddFavorite , SIGNAL(clicked()) , this , SLOT(on_addFavorite()) ) ;
    connect( ui->OpenPDF , SIGNAL(clicked()) , this , SLOT(on_openPdf()) ) ;
    connect( ui->SavePDF , SIGNAL(clicked()) , this , SLOT(on_savePdf())) ;

    connect( ui->SaveComment , SIGNAL(clicked()) , this , SLOT(on_saveComment()) ) ;

    connect( ui->Comments , SIGNAL(textChanged()) , this , SLOT(on_commentChanged()) ) ;

    connect( ui->OpenReview , SIGNAL(clicked()) , this , SLOT(on_openBrowser()) ) ;

    connect( ui->State , SIGNAL(activated(int)) , this , SLOT(on_stateChanged(int)) ) ;
    connect( ui->treeStates , SIGNAL(itemSelectionChanged()) , this , SLOT(on_selectedChanged()) ) ;

    connect( ui->Evaluation , SIGNAL(activated(int)) , this , SLOT(on_evaluationChanged(int)) ) ;
    connect( ui->treeEvaluations , SIGNAL(itemSelectionChanged()) , this , SLOT(on_selectedChanged()) ) ;

    connect( ui->toolBox , SIGNAL(currentChanged(int)) , this , SLOT(on_currentChanged(int)) ) ;
}

//void BookmarkGui::open()
//{
//    current_favorite = "" ;
//    current_favorites_item = 0 ;
//    current_favorite_id = "" ;

//    ui->Title->clear();
//    ui->Abstract->clear();
//    ui->Comments->clear();

//    fillCategorie() ;
//    fillStates() ;
//    fillEvaluations() ;

//    ui->SaveComment->setDisabled( true );

//    this->disableEntryMenuFavorites( BookmarkGui::cancelCut
//                                     | BookmarkGui::cut
//                                     | BookmarkGui::paste
//                                     | BookmarkGui::renameFold
//                                     | BookmarkGui::remove );

//    setModal( true ) ;
//    show() ;
//    exec() ;
//}

void BookmarkGui::open( QString id )
{

    if ( first_open )
    {
        fillCategorie() ;
        fillStates() ;
        fillEvaluations() ;

//        this->disableEntryMenuFavorites( BookmarkGui::cancelCut
//                                         | BookmarkGui::cut
//                                         | BookmarkGui::paste
//                                         | BookmarkGui::renameFold
//                                         | BookmarkGui::remove );


        first_open = false ;
    }

//    current_favorite = "" ;
//    current_favorites_item = 0 ;
//    current_favorite_id = "" ;

//    ui->Title->clear();
//    ui->Abstract->clear();
//    ui->Comments->clear();

    if ( !id.isEmpty() )
    {
        this->clearFavoriteInfo();
        ui->treeCategorie->clearSelection();
        this->fillFavoriteInfo( id ) ;
        this->searchItem( id ) ;
    }
//    else
//    {
//        ui->treeCategorie->setC
//    }

    ui->SaveComment->setDisabled( true );


    setModal( true ) ;
    show() ;
    exec() ;
}

void BookmarkGui::buildMenuStates()
{
    QAction* add_state = new QAction( tr( "Aggiungi Stato" ) , 0 );
    menu_states_ptr.insert( "add_state" , add_state ) ;
    connect( add_state , SIGNAL(triggered()) , this , SLOT(on_newState()) ) ;
    this->menuStates.addAction( add_state ) ;

    QAction* rename = new QAction( tr( "Cambia nome" ) , 0 );
    menu_states_ptr.insert( "rename" , rename ) ;
    connect( rename , SIGNAL(triggered()) , this , SLOT(on_renameState()) ) ;
    this->menuStates.addAction( rename ) ;

    QAction* remove_state = new QAction( tr( "Rimuovi Stato" ) , 0 );
    menu_states_ptr.insert( "remove_state" , remove_state ) ;
    connect( remove_state , SIGNAL(triggered()) , this , SLOT(on_removeState()) ) ;
    this->menuStates.addAction( remove_state ) ;

    ui->stateMenu->setMenu( &this->menuStates );
    this->disableEntryMenuStates( RenameState | RemoveState ) ;
}

void BookmarkGui::buildMenuFavorites()
{
    QAction* new_folder = new QAction( tr( "Nuova Cartella" ) , 0 );
    menu_ptr.insert( "new_folder" , new_folder ) ;
    connect( new_folder , SIGNAL(triggered()) , this , SLOT(on_newFolder()) ) ;
    this->menuFavorites.addAction( new_folder ) ;

    this->menuFavorites.addSeparator() ;

    QAction* cut = new QAction( tr( "Taglia" ) , 0 );
    menu_ptr.insert( "cut" , cut ) ;
    connect( cut , SIGNAL(triggered()) , this , SLOT(on_cutItem()) ) ;
    this->menuFavorites.addAction( cut ) ;

    QAction* paste = new QAction( tr( "Incolla" ) , 0 );
    menu_ptr.insert( "paste" , paste ) ;
    connect( paste , SIGNAL(triggered()) , this , SLOT(on_pasteItem()) ) ;
    this->menuFavorites.addAction( paste ) ;

    QAction* cancel_cut = new QAction( tr( "Annulla Taglia" ) , 0 );
    menu_ptr.insert( "cancel_cut" , cancel_cut ) ;
    connect( cancel_cut , SIGNAL(triggered()) , this , SLOT(on_cancelCut()) ) ;
    this->menuFavorites.addAction( cancel_cut ) ;

    this->menuFavorites.addSeparator() ;

    QAction* remove = new QAction( tr( "Rimuovi" ) , 0 );
    menu_ptr.insert( "remove" , remove ) ;
    connect( remove , SIGNAL(triggered()) , this , SLOT(on_remove()) ) ;
    this->menuFavorites.addAction( remove ) ;

    this->menuFavorites.addSeparator() ;

    QAction* rename_folder = new QAction( tr( "Cambia nome" ) , 0 );
    menu_ptr.insert( "rename_folder" , rename_folder ) ;
    connect( rename_folder , SIGNAL(triggered()) , this , SLOT(on_remaneFolder()) ) ;
    this->menuFavorites.addAction( rename_folder ) ;

    ui->mainFavoritesMenu->setMenu( &this->menuFavorites );

    this->disableEntryMenuFavorites( BookmarkGui::cancelCut
                                     | BookmarkGui::cut
                                     | BookmarkGui::paste
                                     | BookmarkGui::renameFold
                                     | BookmarkGui::remove );
}

//enum menuFavoritesEntry { newFolder=1 , cut=2 , cancelCut=4 , paste=8 , remove=16 , ranameFolder=32 } ;

BookmarkGui::~BookmarkGui()
{
    delete ui;
}



//enum menuFavoritesEntry { newFolder=1 , cut=2 , cancelCut=4 , paste=8 , remove=16 , ranameFolder=32 } ;
void BookmarkGui::enableEntryMenuFavorites( int e )
{
    if ( e & newFolder )
        menu_ptr["new_folder"]->setEnabled( true );
    if ( e & cut )
        menu_ptr["cut"]->setEnabled( true );
    if ( e & cancelCut )
        menu_ptr["cancel_cut"]->setEnabled( true );
    if ( e & paste )
        menu_ptr["paste"]->setEnabled( true );
    if ( e & remove )
        menu_ptr["remove"]->setEnabled( true );
    if ( e & renameFold )
        menu_ptr["rename_folder"]->setEnabled( true );
}

void BookmarkGui::disableEntryMenuFavorites( int e )
{
    if ( e & newFolder )
        menu_ptr["new_folder"]->setDisabled( true );
    if ( e & cut )
        menu_ptr["cut"]->setDisabled( true );
    if ( e & cancelCut )
        menu_ptr["cancel_cut"]->setDisabled( true );
    if ( e & paste )
        menu_ptr["paste"]->setDisabled( true );
    if ( e & remove )
        menu_ptr["remove"]->setDisabled( true );
    if ( e & renameFold )
        menu_ptr["rename_folder"]->setDisabled( true );
}

//enum menuStatesEntry { NewState=1 , RenameState=2 , RemoveState=4 } ;
void  BookmarkGui::enableEntryMenuStates( int e )
{
    if ( e & NewState )
        menu_states_ptr["add_state"]->setEnabled( true ) ;
    if ( e & RenameState )
        menu_states_ptr["rename"]->setEnabled( true ) ;
    if ( e & RemoveState )
        menu_states_ptr["remove_state"]->setEnabled( true ) ;
}

void  BookmarkGui::disableEntryMenuStates( int e )
{
    if ( e & NewState )
        menu_states_ptr["add_state"]->setDisabled( true ) ;
    if ( e & RenameState )
        menu_states_ptr["rename"]->setDisabled( true ) ;
    if ( e & RemoveState )
        menu_states_ptr["remove_state"]->setDisabled( true ) ;
}

void BookmarkGui::fillBookmarkMenu( QAction* def_act )
{
    if ( this->bookmarkMenu == 0 ) return ;
    this->bookmarkMenu->clear() ;

    if ( def_act != 0 )
    {
        this->bookmarkMenu->addAction( def_act ) ;
        this->bookmarkMenu->addSeparator() ;
    }

    Bookmark bk ;
    QueryResult qr ;

    bk.getRootFolders( qr ) ;
    for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
    {
        QString name = qr.getField( "Folder" , itr ) ;
        QString id = qr.getField( "Id" , itr ) ;
        qDebug() << "BookmarkGui::fillBookmarkMenu" << name << id ;
        this->fillBookmarkMenuRec( bookmarkMenu , name , id ) ;
    }
}

void BookmarkGui::fillBookmarkMenuRec( QMenu *menu , QString name , QString parent_id )
{
    Bookmark bk ;
    QueryResult qr ;

    bk.getFoldersId( qr , parent_id ) ;
    for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
    {
        QString name = qr.getField( "Folder" , itr ) ;
        QString id = qr.getField( "Id" , itr ) ;
        QMenu* sub_menu = new QMenu( name , menu ) ;

        QIcon folder(":/icons/crystal/folder.png") ;
        sub_menu->setIcon( folder );
        menu->addMenu( sub_menu ) ;

        this->fillBookmarkMenuRec( sub_menu , name , id ) ;
    }

    bk.getFavoritesByParent( qr , name ) ;
    for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
    {
        QString id_entry = qr.getField( "IdEntry" , itr ) ;
        QString id = qr.getField( "Id" , itr ) ;
        QAction* entry = new QAction( 0 ) ;
        setArticleActionDecorations( entry , id_entry , id ) ;
        connect( entry , SIGNAL(triggered()) , this , SLOT(on_favoriteSelected()) ) ;
        menu->addAction( entry ) ;
    }
}

void BookmarkGui::setArticleActionDecorations( QAction* action , const QString& id_entry , const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id_entry ) ;

    QIcon doc_icon(":/icons/crystal/doc-icon.png") ;
    action->setIcon( doc_icon );

    if ( article.size() == 0 ) return ;
    action->setText( article.getField( "Titolo" , article.begin() ) ) ;
    action->setIcon( doc_icon ) ;
    action->setData( QVariant( id_entry ) ) ;
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
        QString id_entry = qr.getField( "IdEntry" , itr ) ;
        QString id = qr.getField( "Id" , itr ) ;
        QTreeWidgetItem* item = new QTreeWidgetItem( parent , BookmarkGui::item_article ) ;
        this->setArticleItemDecorations( item , id_entry , id ) ;
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

bool BookmarkGui::searchItem( QString id )
{
    QList<QTreeWidgetItem *> list_items =
            ui->treeCategorie->findItems ( id , Qt::MatchExactly | Qt::MatchRecursive , 1 ) ;

    qDebug() << list_items.size() ;

    if ( ! list_items.isEmpty() )
    {
        list_items.first()->parent()->setExpanded(true);
        ui->treeCategorie->setCurrentItem( list_items.first() ) ;
    }
}

void BookmarkGui::fillFavoriteInfo( const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id ) ;

    if ( article.size() == 0 ) return ;

    this->current_favorite = id ;

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

    res.clear() ;
    bk.getEvaluation( res , Id_f ) ;
    if ( !res.empty() )
    {
        int index = ui->Evaluation->findData( res.getField(0,0) ) ;
        ui->Evaluation->setCurrentIndex( index );
    }
    else
        ui->Evaluation->setCurrentIndex( 0 );
}


void BookmarkGui::fillEvaluations()
{
    QueryResult eval ;
    Bookmark bk ;


    QList<QTreeWidgetItem*> items;

    ui->Evaluation->clear();

    int index = 0 ;

    ui->Evaluation->setIconSize( QSize( 100 , 20 ) );
    ui->treeEvaluations->setIconSize( QSize( 100 , 20 ) );

    ui->Evaluation->insertItem( index , tr("Nessuna valutazione") , "-1" );

    bk.getEvaluations( eval );

    for ( QueryResult::iterator itr = eval.begin() ; itr < eval.end() ; itr++ )
    {
        QString Evaluation = eval.getField( "Evaluation" , itr ) ;

        QString stars_file = ":/stars/stars/" ;
        stars_file += Evaluation ;
        stars_file += "s.png" ;
        QIcon stars_icon( stars_file ) ;
        QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_evaluation ) ;
        item->setIcon( 0 , stars_icon ) ;

        item->setText( 1 , Evaluation ) ;
        qDebug() << item->text(1);


        QueryResult entries ;
        bk.getFavoritesByEvaluation( entries , Evaluation ) ;
        for ( QueryResult::iterator itr_b = entries.begin() ; itr_b < entries.end() ; itr_b++ )
        {
            QString id_entry = entries.getField( "IdEntry" , itr_b ) ;
            QString id = entries.getField( "Id" , itr_b ) ;
            QTreeWidgetItem* entry_item = new QTreeWidgetItem( item , BookmarkGui::item_article ) ;
            this->setArticleItemDecorations( entry_item , id_entry , id ) ;
        }


        items.append( item );

        QVariant stars ;
        stars.setValue( Evaluation );
        ui->Evaluation->insertItem( ++index , stars_icon , "" , stars ) ;
    }

    ui->treeEvaluations->clear();
    ui->treeEvaluations->addTopLevelItems( items );
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

    this->name_d.setMessage( tr( "Inserisci il nuovo nome della cartella" ) );
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

    if ( !fr )
    {
        QMessageBox msgBox;
        msgBox.setText( tr("Si possono rimuovere solo le cartelle vuote.") );
        msgBox.setIcon( QMessageBox::Information ) ;
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec() ;
        return false ;
    }

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

    if ( ui->AddFavorite->isEnabled() )
        ui->AddFavorite->setEnabled( false );
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

bool BookmarkGui::addState( QString new_state )
{
    Bookmark bk ;
    QueryResult favorite ;

    bk.getFavoriteFullData( favorite , this->current_favorite );

    QList<QTreeWidgetItem *> list_state =
            ui->treeStates->findItems ( new_state , Qt::MatchExactly ) ;

    if ( list_state.isEmpty() || list_state.size() > 1 ) return false ;

    QTreeWidgetItem *state = list_state.first() ;

    if ( state->type() != BookmarkGui::item_state ) return false ;

    QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidgetItem*)0 , BookmarkGui::item_article ) ;
    this->setArticleItemDecorations( item , this->current_favorite , this->current_favorite_id ) ;

    state->addChild( item ) ;
    state->setExpanded( true ) ;
    ui->treeStates->setCurrentItem( item );

    return true ;
}

bool BookmarkGui::removeState()
{
    Bookmark bk ;
    QueryResult favorite ;
    QTreeWidgetItem *item ;

    bk.getFavoriteFullData( favorite , this->current_favorite );

    QList<QTreeWidgetItem *> list =
            ui->treeStates->findItems ( favorite.getField( "Id" , favorite.begin() ) , Qt::MatchExactly|Qt::MatchRecursive , 1 ) ;

    if ( list.size() > 1 || list.isEmpty() ) return false ;

    item = list.first() ;

    if ( item->type() != BookmarkGui::item_article ) return false ;

    QTreeWidgetItem *parent = item->parent() ;
    parent->removeChild( item ); ;

    return true ;
}

bool BookmarkGui::changeState( QString new_state )
{
    Bookmark bk ;
    QueryResult favorite ;
    QTreeWidgetItem *item ;

    bk.getFavoriteFullData( favorite , this->current_favorite );

    QList<QTreeWidgetItem *> list =
            ui->treeStates->findItems ( favorite.getField( "Titolo" , favorite.begin() ) , Qt::MatchExactly|Qt::MatchRecursive ) ;

    if ( list.size() > 1 ) return false ;

    QList<QTreeWidgetItem *> list_state =
            ui->treeStates->findItems ( new_state , Qt::MatchExactly ) ;

    if ( list_state.isEmpty() || list_state.size() > 1 ) return false ;

    QTreeWidgetItem *state = list_state.first() ;

    if ( state->type() != BookmarkGui::item_state ) return false ;

    item = list.first() ;

    if ( item->type() != BookmarkGui::item_article ) return false ;

    int index = item->parent()->indexOfChild( item ) ;
    item = item->parent()->takeChild( index ) ;

    state->addChild( item );
    state->setExpanded( true );

    ui->treeStates->setCurrentItem( item );

    return true ;
}

bool BookmarkGui::addEvaluation( QString stars )
{
    Bookmark bk ;
    QueryResult favorite ;

    bk.getFavoriteFullData( favorite , this->current_favorite );

    QList<QTreeWidgetItem *> list_evaluations =
            ui->treeEvaluations->findItems ( stars , Qt::MatchExactly , 1 ) ;

    qDebug() << list_evaluations.isEmpty() << list_evaluations.size() << stars ;
    if ( list_evaluations.isEmpty() || list_evaluations.size() > 1 ) return false ;

    QTreeWidgetItem *evaluation = list_evaluations.first() ;

    if ( evaluation->type() != BookmarkGui::item_evaluation ) return false ;

    QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidgetItem*)0 , BookmarkGui::item_article ) ;
    this->setArticleItemDecorations( item , this->current_favorite , this->current_favorite_id ) ;

    evaluation->addChild( item ) ;
    evaluation->setExpanded( true ) ;

    ui->treeEvaluations->setCurrentItem( item );

    return true ;
}

bool BookmarkGui::changeEvaluation( QString stars )
{
    Bookmark bk ;
    QueryResult favorite ;
    QTreeWidgetItem *item ;

    bk.getFavoriteFullData( favorite , this->current_favorite );

    QList<QTreeWidgetItem *> list =
            ui->treeEvaluations->findItems ( favorite.getField( "Titolo" , favorite.begin() ) , Qt::MatchExactly|Qt::MatchRecursive ) ;

    if ( list.size() > 1 ) return false ;

    QList<QTreeWidgetItem *> list_evaluations =
            ui->treeEvaluations->findItems ( stars , Qt::MatchExactly , 1 ) ;

    if ( list_evaluations.isEmpty() || list_evaluations.size() > 1 ) return false ;

    QTreeWidgetItem *evaluation = list_evaluations.first() ;

    if ( evaluation->type() != BookmarkGui::item_evaluation ) return false ;

    item = list.first() ;

    if ( item->type() != BookmarkGui::item_article ) return false ;

    int index = item->parent()->indexOfChild( item ) ;
    item = item->parent()->takeChild( index ) ;

    evaluation->addChild( item );
    evaluation->setExpanded( true );

    ui->treeEvaluations->setCurrentItem( item );

    return true ;
}

bool BookmarkGui::removeEvaluation()
{
    Bookmark bk ;
    QueryResult favorite ;
    QTreeWidgetItem *item ;

    bk.getFavoriteFullData( favorite , this->current_favorite );

    QList<QTreeWidgetItem *> list =
            ui->treeEvaluations->findItems ( favorite.getField( "Id" , favorite.begin() ) , Qt::MatchExactly|Qt::MatchRecursive , 1 ) ;

    if ( list.size() > 1 || list.isEmpty() ) return false ;

    item = list.first() ;

    if ( item->type() != BookmarkGui::item_article ) return false ;

    QTreeWidgetItem *parent = item->parent() ;
    parent->removeChild( item ); ;

    return true ;
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

        this->enableEntryMenuFavorites( BookmarkGui::paste | BookmarkGui::cancelCut );
        this->disableEntryMenuFavorites( BookmarkGui::cut );
    }
    else if ( this->current_favorites_item->type() == BookmarkGui::item_article )
    {
        this->cutted_item = this->current_favorites_item ;
        this->cutted_item->setDisabled( true ) ;
        this->cut_state = true ;

        this->enableEntryMenuFavorites( BookmarkGui::paste | BookmarkGui::cancelCut );
        this->disableEntryMenuFavorites( BookmarkGui::cut );
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

        this->disableEntryMenuFavorites( BookmarkGui::paste | BookmarkGui::cancelCut );
        this->enableEntryMenuFavorites( BookmarkGui::cut );

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

        this->disableEntryMenuFavorites( BookmarkGui::paste | BookmarkGui::cancelCut );
        this->enableEntryMenuFavorites( BookmarkGui::cut );

        return ;
    }
}

void BookmarkGui::on_newFolder()
{
    this->name_d.setMessage( tr( "Inserisci il nome della nuova cartella" ) );
    this->name_d.open();

    if ( this->name_d.text().isEmpty() )
        return ;

    this->appendFolder( this->name_d.text() ) ;
}

void BookmarkGui::on_addFavorite()
{
    if ( this->current_favorite.isEmpty() ) return ;
    this->appendFavorite( this->current_favorite );
}

void BookmarkGui::on_remove()
{

    QMessageBox msgBox;
    msgBox.setMinimumWidth( 300 );
    msgBox.setText( tr("Vuoi veramente rimuovre l'oggetto selezionato?") );
    msgBox.setIcon( QMessageBox::Question ) ;
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int ret = msgBox.exec();

    if ( ret == QMessageBox::Cancel )
        return ;

    if ( this->current_favorites_item == 0 ) return ;

    if ( this->current_favorites_item->type() == BookmarkGui::item_article )
        this->removeFavorite() ;
    else
        this->removeFolder() ;
}

void BookmarkGui::on_openPdf()
{
    if ( this->current_favorite.isEmpty() ) return ;
    emit this->sig_openPdf( this->current_favorite.toInt() );
}


void BookmarkGui::on_savePdf()
{
    if ( this->current_favorite.isEmpty() ) return ;
    emit this->sig_savePdf( this->current_favorite.toInt() );
}

void BookmarkGui::on_contextMenu( const QPoint& pos )
{
    QPoint global_pos = ui->treeCategorie->mapToGlobal( pos ) ;
    this->menuFavorites.exec( global_pos ) ;
}

void BookmarkGui::on_contextMenuStates( const QPoint& pos )
{
    QPoint global_pos = ui->treeStates->mapToGlobal( pos ) ;
    this->menuStates.exec( global_pos ) ;
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

    if( ui->toolBox->currentIndex() == 0 )
    {
        item_list = ui->treeCategorie->selectedItems() ;
        this->enableEntryMenuFavorites( BookmarkGui::cut | BookmarkGui::remove );
    }
    else if ( ui->toolBox->currentIndex() == 1 )
        item_list = ui->treeStates->selectedItems() ;
    else if ( ui->toolBox->currentIndex() == 2 )
        item_list = ui->treeEvaluations->selectedItems() ;
    else
        return ;

    if ( item_list.isEmpty() )
    {
        return ;
    }

    QTreeWidgetItem *item = item_list.first() ;

    if ( item->type() == BookmarkGui::item_article )
    {
        this->fillFavoriteInfo( item->text(1) , item->text(2) );
    }
    else
    {
        Bookmark bk ;
        if ( bk.isFavoriteBookmarked( this->current_favorite ) )
            this->clearFavoriteInfo();
    }

    if ( ui->toolBox->currentIndex() == 0 && item->type() == BookmarkGui::item_folder )
        this->enableEntryMenuFavorites( BookmarkGui::renameFold ) ;

    if ( ui->toolBox->currentIndex() == 0 && item->type() == BookmarkGui::item_article )
        this->disableEntryMenuFavorites( BookmarkGui::renameFold ) ;

    if ( ui->toolBox->currentIndex() == 1 && item->type() == BookmarkGui::item_state )
        this->enableEntryMenuStates( RenameState | RemoveState ) ;

    if ( ui->toolBox->currentIndex() == 1 && item->type() == BookmarkGui::item_article )
        this->disableEntryMenuStates( RenameState | RemoveState ) ;

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
    qDebug() << ui->Comments->toPlainText() ;

    bk.setComment( ui->Comments->toPlainText() , this->current_favorite_id ) ;
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
    qDebug() << " BookmarkGui::on_stateChanged index:" << index ;

    Bookmark bk ;

    if ( this->current_favorite_id.isEmpty() )
    {
        ui->State->setCurrentIndex(0);
        return ;
    }

    if ( index == 0 )
    {
        bk.deleteState( this->current_favorite_id ) ;
        this->removeState() ;
        return ;
    }

    if ( this->current_favorite.isEmpty() ) return ;

    QString state_name = ui->State->itemText( index ) ;

    int f = bk.setState( state_name , this->current_favorite_id ) ;

    if ( f == 0  ) return ;
    else if ( f == 1 )
        this->changeState( state_name ) ;
    else if ( f == 2 )
        this->addState( state_name ) ;
}

void BookmarkGui::on_evaluationChanged( int index )
{
    Bookmark bk ;

    if ( this->current_favorite_id.isEmpty() )
    {
        ui->Evaluation->setCurrentIndex(0);
        return ;
    }

    if ( index == 0 )
    {
        bk.deleteEvaluation( this->current_favorite_id ) ;
        this->removeEvaluation() ;
        return ;
    }

    if ( this->current_favorite.isEmpty() ) return ;

    QString stars = ui->Evaluation->itemData( index ).toString() ;

    int f = bk.setEvaluation( stars , this->current_favorite_id ) ;

    if ( f == 0  ) return ;
    else if ( f == 1 )
        this->changeEvaluation( stars ) ;
    else if ( f == 2 )
        this->addEvaluation( stars ) ;
}

void BookmarkGui::on_newState()
{
    this->name_d.setMessage( tr( "Inserisci il nome del nuovo stato" ) );
    this->name_d.open();

    QString name = name_d.text() ;
    if ( name.isEmpty() ) return ;

    Bookmark bk ;

    if ( bk.addState( name ) )
    {
        QString id = bk.getStateId( name ) ;
        QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_state ) ;
        this->setFolderItemDecorations( item , name , id ) ;
        ui->treeStates->addTopLevelItem( item );

        ui->State->insertItem( ui->State->count() , name ) ;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText( tr("Impossibile creare il nuovo stato, il nome deve essere unico") );
        msgBox.setIcon( QMessageBox::Warning ) ;
        msgBox.exec();
    }
}

void  BookmarkGui::on_removeState()
{
    if ( this->current_favorites_item == 0 ) return ;
    if ( this->current_favorites_item->type() != BookmarkGui::item_state ) return ;

    Bookmark bk ;
    QString name = this->current_favorites_item->text(0) ;
    bool f = bk.removeState( name ) ;

    if ( f == false )
    {
        QMessageBox msgBox;
        msgBox.setText( tr("Impossibile rimuovere lo stato: deve essere vuoto") );
        msgBox.setIcon( QMessageBox::Warning ) ;
        msgBox.exec();

        return ;
    }

    int index = ui->treeStates->indexOfTopLevelItem( this->current_favorites_item ) ;
    QTreeWidgetItem* item = ui->treeStates->takeTopLevelItem( index ) ;
    delete item ;

    index = ui->State->findText( name ,Qt::MatchExactly ) ;
    ui->State->removeItem( index );

    this->current_favorites_item = 0 ;
    this->current_favorite_id = "" ;
    this->current_favorite = "" ;
}

void BookmarkGui::on_renameState()
{
    if ( this->current_favorites_item == 0 ) return ;
    if ( this->current_favorites_item->type() != BookmarkGui::item_state ) return ;

    Bookmark bk ;
    QString name = this->current_favorites_item->text(0) ;

    this->name_d.setMessage( tr( "Inserisci il nuovo nome dello stato" ) );
    this->name_d.open( name ) ;

    QString new_name = name_d.text() ;

    if( new_name.isEmpty() )
        return ;

    bool f =  bk.renameState( name , new_name ) ;

    if ( f == false )
    {
        QMessageBox msgBox;
        msgBox.setText( tr("Impossibile modificare il nome: deve essere unico") );
        msgBox.setIcon( QMessageBox::Warning ) ;
        msgBox.exec();

        return ;
    }

    this->current_favorites_item->setText( 0 , new_name ) ;

    int c_index = ui->State->currentIndex() ;
    int index = ui->State->findText( name ,Qt::MatchExactly ) ;
    ui->State->insertItem( index , new_name ) ;
    ui->State->removeItem( index + 1 );
    ui->State->setCurrentIndex( c_index );
}

void BookmarkGui::on_favoriteSelected()
{
    QObject* from = this->sender() ;

    QAction* from_ac = dynamic_cast<QAction*> ( from ) ;
    emit this->sig_openPdf( from_ac->data().toInt() );
}

void BookmarkGui::on_currentChanged( int c )
{
    QList<QTreeWidgetItem *> item_list ;

    if( c == 0 )
        item_list = ui->treeCategorie->selectedItems() ;
    else if ( c == 1 )
        item_list = ui->treeStates->selectedItems() ;
    else if ( c == 2 )
        item_list = ui->treeEvaluations->selectedItems() ;
    else
        return ;

    if ( item_list.isEmpty() )
    {
        this->clearFavoriteInfo();
        return ;
    }

    if ( item_list.size() > 1 ) return ;

    QTreeWidgetItem *item = item_list.first() ;

    if( item->type() == BookmarkGui::item_article )
    {
        this->fillFavoriteInfo( item->text(1) , item->text(2) );
    }
    else
    {
        this->clearFavoriteInfo();
    }
}

void BookmarkGui::clearFavoriteInfo()
{
    ui->Title->clear();
    ui->Abstract->clear();
    ui->Comments->clear();

    ui->State->setCurrentIndex(0);
    ui->Evaluation->setCurrentIndex(0);

    this->current_favorite.clear();
    this->current_favorite_id.clear();
}

