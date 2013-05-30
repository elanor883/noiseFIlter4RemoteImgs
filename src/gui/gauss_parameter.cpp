#include "gauss_parameter.h"
#include "ui_gauss_parameter.h"

gauss_parameter::gauss_parameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gauss_parameter)
{
    ui->setupUi(this);
    connect(ui->ok_button, SIGNAL(clicked()), this, SLOT(ok_pushed()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(cancel_pushed()));
    connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(slider_change_value()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slider_change_value()));


    ui->horizontalSlider->setMaximum(6);
    ui->horizontalSlider->setMinimum(1);
    ui->sigma_value->setText(QString::number(ui->horizontalSlider->value()));
}

int gauss_parameter::slider_change_value()
{
ui->sigma_value->setText(QString::number(ui->horizontalSlider->value()));

return ui->horizontalSlider->value();
}

void gauss_parameter::cancel_pushed()
{
close();
}

int gauss_parameter::ok_pushed()
{
    close();
return 1;
}

int gauss_parameter::get_parameter()
{
return ui->horizontalSlider->value();
}


gauss_parameter::~gauss_parameter()
{
    delete ui;
}
