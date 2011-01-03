#include "dictionarygui.h"
#include "ui_dictionarygui.h"

DictionaryGui::DictionaryGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictionaryGui)
{
    ui->setupUi(this);
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
