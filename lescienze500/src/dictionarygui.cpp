#include "dictionarygui.h"
#include "ui_dictionarygui.h"

DictionaryGui::DictionaryGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictionaryGui)
{
    ui->setupUi(this);

    connect( ui->search , SIGNAL(clicked()) , this , SLOT(on_searchClicked()) ) ;

    menu_search = new QMenu() ;

    menu_search_word = new QAction( tr("Word") , 0 ) ;
    menu_search_word->setCheckable(true);
    menu_search_word->setChecked(true);
    menu_search->addAction( menu_search_word ) ;

    menu_search_word_year = new QAction( tr("Year - Word") , 0 ) ;
    menu_search_word_year->setCheckable(true) ;
    menu_search->addAction( menu_search_word_year ) ;

    menu_search_word_categoria = new QAction( tr("Categoria - Word") , 0 ) ;
    menu_search_word_categoria->setCheckable(true) ;
    menu_search->addAction( menu_search_word_categoria ) ;

//    menu_OR = new QAction( tr("OR") , 0 ) ;
//    menu_OR->setCheckable(true);
//    menu_logical->addAction( menu_search_word ) ;

    set_search = new QActionGroup(0) ;
    set_search->addAction(menu_search_word) ;
    set_search->addAction(menu_search_word_year) ;
    set_search->addAction(menu_search_word_categoria) ;

    //set_search->addAction(menu_OR) ;
    set_search->setExclusive(true);


    ui->search->setMenu( menu_search );

}

DictionaryGui::~DictionaryGui()
{
    delete ui;
}

void DictionaryGui::openDictionary()
{
    this->setWindowTitle( "Dizionario" );

    this->setModal(true);
    this->setFocus();
    this->show();

    if ( !dict.exists() )
    {
        ui->frame->setDisabled( true );
    }
}

void DictionaryGui::search( QString word )
{
    if ( word.isEmpty() ) return ;

    QueryResult qr ;
    dict.getWord( qr , word ) ;

    if( qr.empty() ) return ;

    ui->textBrowser->clear();
    QString text , full_text ;
    full_text = "<TABLE BORDER=\"1\">" ;
    full_text += " <TR><td> Parola </td> <td> Cnt </td></TR>" ;
    full_text += " <TR><td>----------</td> <td>----------</td></TR>" ;
    for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
    {
        text = " <td>%1</td> <td>%2 </td>" ;
        full_text += "<TR>" ;
        full_text += text.arg( qr.getField("Word",itr) ).arg( qr.getField("Cnt",itr) ) ;
        full_text += "</TR>" ;
    }
    full_text += "</TABLE>" ;

    ui->textBrowser->setHtml( full_text );
}

void DictionaryGui::searchWordYear( QString word )
{
    if ( word.isEmpty() ) return ;

    QueryResult qr ;
    dict.getYearsFromWord( qr , word ) ;

    if( qr.empty() ) return ;

    ui->textBrowser->clear();
    QString text , full_text ;
    full_text = "<TABLE BORDER=\"1\">" ;
    full_text += QString(" <TR><td> Parola: </td> <td> %1 </td></TR>").arg(word) ;
    full_text += " <TR><td>-----------------------------</td></TR>" ;
    full_text += " <TR><td> Anno </td> <td> Conteggio </td></TR>" ;
    for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
    {
        text = " <td>%1</td> <td>%2 </td>" ;
        full_text += "<TR>" ;
        full_text += text.arg( qr.getField("Anno",itr) ).arg( qr.getField("Cnt",itr) ) ;
        full_text += "</TR>" ;
    }
    full_text += "</TABLE>" ;

    ui->textBrowser->setHtml( full_text );
}


void DictionaryGui::searchWordCategoria ( QString word )
{
    if ( word.isEmpty() ) return ;

    QueryResult qr ;
    dict.getCategorieFromWord( qr , word ) ;

    if( qr.empty() ) return ;

    ui->textBrowser->clear();
    QString text , full_text ;
    full_text = "<TABLE BORDER=\"1\">" ;
    full_text += QString(" <TR><td> Parola: </td> <td> %1 </td></TR>").arg(word) ;
    full_text += " <TR><td>-----------------------------</td></TR>" ;
    full_text += " <TR><td> Categoria </td> <td> Conteggio </td></TR>" ;
    for ( QueryResult::iterator itr = qr.begin() ; itr < qr.end() ; itr++ )
    {
        text = " <td>%1</td> <td>%2 </td>" ;
        full_text += "<TR>" ;
        full_text += text.arg( qr.getField("Categoria",itr) ).arg( qr.getField("Cnt",itr) ) ;
        full_text += "</TR>" ;
    }
    full_text += "</TABLE>" ;

    ui->textBrowser->setHtml( full_text );
}

void DictionaryGui::on_searchClicked()
{
    if ( this->menu_search_word->isChecked() )
        this->search( ui->lineEdit->text() ) ;
    else if ( this->menu_search_word_year->isChecked() )
        this->searchWordYear( ui->lineEdit->text() ) ;
    else if ( this->menu_search_word_categoria->isChecked() )
        this->searchWordCategoria( ui->lineEdit->text() );
}

void DictionaryGui::on_searchStatusChanged()
{
}
