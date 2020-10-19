#ifndef SCALEDPIXMAP_H
#define SCALEDPIXMAP_H

#include <QWidget>

class ScaledPixmap : public QWidget {
  Q_OBJECT
public:
  ScaledPixmap(QWidget *parent = nullptr);
  void setScaledPixmap(const QPixmap &pixmap);
  QSize sizeHint() const override;
protected:
  void paintEvent(QPaintEvent *event) override;
private:
  QPixmap m_pixmap;
};

#endif // SCALEDPIXMAP_H
