#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QGraphicsDropShadowEffect>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);
  connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &AboutDialog::onOkButton);

  QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
  ui->verticalFrame->setFrameStyle(QFrame::Panel | QFrame::Plain);
  ui->verticalFrame->setLineWidth(1);
  effect->setOffset(-1, -1);
  effect->setColor(Qt::black);
  ui->label->setGraphicsEffect(effect);
}

AboutDialog::~AboutDialog() {
  delete ui;
}

void AboutDialog::onOkButton() {
  this->hide();
}
