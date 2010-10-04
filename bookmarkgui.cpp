#include "bookmarkgui.h"
#include "ui_bookmarkgui.h"

BookmarkGui::BookmarkGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkGui)
{
    ui->setupUi(this);
}

BookmarkGui::~BookmarkGui()
{
    delete ui;
}
