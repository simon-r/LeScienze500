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
    fillCategorie() ;
    buildMenuFavorites() ;
    connect( ui->treeCategorie , SIGNAL(itemClicked(QTreeWidgetItem*,int)) , this , SLOT(on_favoriteActivated(QTreeWidgetItem*,int)) ) ;

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

     bk.getRootCategorie( qr );

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
         QString name = qr.getField( "Categoria" , itr ) ;
         QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_folder ) ;
         setFolderItemDecorations( item , name ) ;
         fillCategorieRec( name , item ) ;
         items.append( item ) ;
     }

     bk.getFavoritesByParent( qr , "" ) ;
     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "IdArticolo" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*)0 , BookmarkGui::item_article ) ;
          this->setArticleItemDecorations( item , name ) ;
          items.append( item );
     }

     ui->treeCategorie->insertTopLevelItems(0, items);
}

void BookmarkGui::fillCategorieRec( const QString& name , QTreeWidgetItem* parent )
{
     Bookmark bk ;
     QueryResult qr ;

     bk.getCategorie( qr , name ) ;

     for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
     {
          QString name = qr.getField( "Categoria" , itr ) ;
          QTreeWidgetItem* item = new QTreeWidgetItem( parent , BookmarkGui::item_folder ) ;
          this->setFolderItemDecorations( item , name ) ;
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

void BookmarkGui::setFolderItemDecorations( QTreeWidgetItem* item , const QString& name )
{
    QIcon folder(":/icons/crystal/folder.png") ;
    item->setIcon( 0 , folder);
    item->setText( 0 , name );

    item->setText( 1 , "folder" ) ;
}

void BookmarkGui::fillFavoriteInfo( const QString& id )
{
    QueryResult article ;
    Bookmark bk ;
    bk.getFavoriteFullData( article , id ) ;

    if ( article.size() == 0 ) return ;

    ui->Title->setHtml( article.getField( "Titolo" , article.begin() ) );
    ui->Abstract->setHtml( article.getField( "Abstract" , article.begin() ) );
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


void BookmarkGui::on_favoriteActivated( QTreeWidgetItem * item, int column )
{
    if ( item->type() == BookmarkGui::item_article  )
    {
        this->fillFavoriteInfo( item->text( 1 ) );
    }
}

void BookmarkGui::on_newFolder()
{
    qDebug() << "new folder" ;
    Bookmark bk ;
    bk.addFolder( "" , "Ciao" );
}
