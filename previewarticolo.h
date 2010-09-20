#ifndef PREVIEWARTICOLO_H
#define PREVIEWARTICOLO_H

#include <QDialog>

namespace Ui {
    class PreviewArticolo;
}

class PreviewArticolo : public QDialog {
    Q_OBJECT
public:
    PreviewArticolo(QWidget *parent = 0);
    ~PreviewArticolo();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreviewArticolo *ui;
};

#endif // PREVIEWARTICOLO_H
