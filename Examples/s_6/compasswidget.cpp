#include "compasswidget.h"
#include "ui_compasswidget.h"

CompassWidget::CompassWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompassWidget)
{
    setupUi(this);
    currentYaw = 0;
    scene = new QGraphicsScene(view);
    view->setScene(scene);
    view->setStyleSheet("background: transparent");
    view->setRenderHint(QPainter::Antialiasing);
    picDial = scene->addPixmap(QPixmap(":/images/dial.png"));
    picROV = scene->addPixmap(QPixmap(":/images/rov.png"));
    txtCurrentYaw = scene->addText("360", QFont("Times New Roman", 14));
    picROV->setTransform(QTransform::fromScale(0.5, 0.5));
    picROV->setPos(picDial->pixmap().width()/2 - picROV->pixmap().width()/4,
                   picDial->pixmap().height()/2 - picROV->pixmap().height()/4);
    QTransform t;
    t.translate(picDial->pixmap().width()/2-14, picDial->pixmap().height()/2-14);
    txtCurrentYaw->setTransform(t);
    picROV->setTransformOriginPoint(picROV->pixmap().width()/2, picROV->pixmap().height()/2);

    connect(btnMinus, SIGNAL(clicked()), this, SLOT(decreaseCurrentYaw()));
    connect(btnPlus , SIGNAL(clicked()), this, SLOT(increaseCurrentYaw()));
}

CompassWidget::~CompassWidget()
{
    delete ui;
}

void CompassWidget::increaseCurrentYaw()
{

}

void CompassWidget::decreaseCurrentYaw()
{

}

void CompassWidget::resetCurrentYaw()
{

}
