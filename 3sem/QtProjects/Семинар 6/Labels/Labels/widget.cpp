#include "widget.h"

namespace LABEL {

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Немного о работе с QLabel");
    //создадим объект метки с тоекстом
    lblText = new QLabel ("Метка с текстом!");
    //с помощью метода setAlignment() можно задать выравнивание
    //текста метки Qt::AlignHCenter - задаст выравненивание по центру
    lblText->setAlignment(Qt::AlignHCenter);

    lblImgTxt = new QLabel;
    //текст на метке можно вывести и с помощью метода setText()
    //что может быть удобно при выводе какой-либо изменяющейся информации
    lblImgTxt->setText("Метка с картинкой:");
    //lblImgTxt->setAlignment(Qt::AlignHCenter);
    //когда вам нужно несколько выравниваний (напрмер по центру и
    //по нижнему краю, то нужно вызвать метод setAlignment
    // и перечислить нужные выравнивания, разделяя их оператором
    //побитового ИЛИ, т.е. "|")
    lblImgTxt->setAlignment( Qt::AlignBottom | Qt::AlignHCenter );

    //создадим объект метки с изрображением
    //для этого сначала создадим файл ресурсов, спрячем туда нашу
    //картинку для метки и создадим метку
    lblImg = new QLabel;
    //создадим объект QPixmap
    QPixmap pix(":/images/bmstu.png");
    //установим изображение в метке
    //При этом если автоматическое изменение размера позволено,
    //то метка сама поменяет размер и вы просто загружаете картинку
    lblImg->setPixmap(pix);

    //Если нужно изменить размер
    lblImgSized = new QLabel;
    QPixmap pixScaled(":/images/bmstu.png");
    //изменяем размер картинки методом scaled(), в котором задаем размер
    //и политику изменения масштаба картинки
    pixScaled=pixScaled.scaled(QSize(100,100),Qt::KeepAspectRatio);
    lblImgSized->setPixmap(pixScaled);
    //выравниваем картинку по центру
    lblImgSized->setAlignment(Qt::AlignCenter);
    lblImgSizedTxt = new QLabel ("Метка с масштабированной картинкой и \
                                автоматическим переносом текста на новую строку)):");
    lblImgSizedTxt->setFixedWidth(150);
    //текст длинный, так что добавим перенос строк
    lblImgSizedTxt->setWordWrap(true);

    mov = new QMovie (":/movies/giphy.gif");
    mov->setScaledSize(QSize(150,150));
    lblMovTxt = new QLabel("Метка с видео:");
    lblMov = new QLabel;
    lblMov->setMovie(mov);
    mov->start();
    lblMovTxt->setAlignment(Qt::AlignCenter);
    lblMov->setAlignment(Qt::AlignHCenter);

    //Добавляем надписи на layout
    QVBoxLayout *vlay = new QVBoxLayout;
    QGridLayout *glay = new QGridLayout;
    vlay->addWidget(lblText);
    vlay->setSpacing(20);
    glay->addWidget(lblImgTxt,0,0);
    //vlay->addWidget(lblImgTxt);
    glay->addWidget(lblImg,1,0);
    glay->addWidget(lblImgSizedTxt,0,1);
    glay->addWidget(lblImgSized,1,1);
    //vlay->addWidget(lblImgSized);
    vlay->addLayout(glay);
    vlay->addWidget(lblMovTxt);
    vlay->addWidget(lblMov);
    this->setLayout(vlay);
    //и шрифт получше поставим))
    QFont serifFont ("Times", 10, QFont::Bold);
    this->setFont(serifFont);


}

Widget::~Widget() {

}

} //namespace LABEL
