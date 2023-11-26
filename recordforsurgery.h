#ifndef RECORDFORSURGERY_H
#define RECORDFORSURGERY_H

#include <QDialog>

namespace Ui {
class recordforsurgery;
}

class recordforsurgery : public QDialog
{
    Q_OBJECT

public:
    explicit recordforsurgery(QWidget *parent = nullptr);
    ~recordforsurgery();

private:
    Ui::recordforsurgery *ui;
};

#endif // RECORDFORSURGERY_H
