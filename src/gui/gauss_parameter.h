#ifndef GAUSS_PARAMETER_H
#define GAUSS_PARAMETER_H

#include <QDialog>

namespace Ui {
class gauss_parameter;
}

class gauss_parameter : public QDialog
{
    Q_OBJECT
    
public:
    explicit gauss_parameter(QWidget *parent = 0);
    ~gauss_parameter();

public slots:
    void cancel_pushed();
    int ok_pushed();
    int slider_change_value();
    int get_parameter();
    
private:
    Ui::gauss_parameter *ui;
};

#endif // GAUSS_PARAMETER_H
