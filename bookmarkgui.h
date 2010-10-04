#ifndef BOOKMARKGUI_H
#define BOOKMARKGUI_H

#include <QDialog>

namespace Ui {
    class BookmarkGui;
}

class BookmarkGui : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkGui(QWidget *parent = 0);
    ~BookmarkGui();

private:
    Ui::BookmarkGui *ui;
};

#endif // BOOKMARKGUI_H
