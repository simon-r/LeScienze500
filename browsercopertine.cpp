#include "browsercopertine.h"
#include "ui_browsercopertine.h"

BrowserCopertine::BrowserCopertine(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::BrowserCopertine)
{
    ui->setupUi(this);
}

BrowserCopertine::~BrowserCopertine()
{
    delete ui ;
}

void BrowserCopertine::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
