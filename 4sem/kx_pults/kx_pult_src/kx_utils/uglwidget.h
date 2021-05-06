#ifndef UGLWIDGET_H
#define UGLWIDGET_H

#include <QGLWidget>
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif


class UGLWidget: public QGLWidget
{
	Q_OBJECT
public:
	UGLWidget(QWidget * parent = 0): QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::AlphaChannel | QGL::DirectRendering | QGL::SampleBuffers), parent) {};
	UGLWidget(QGLContext * context, QWidget * parent = 0): QGLWidget(context, parent) {};

signals:
	void closeEvent(QCloseEvent * e);
	void dragEnterEvent(QDragEnterEvent * e);
	void dragLeaveEvent(QDragLeaveEvent * e);
	void dragMoveEvent(QDragMoveEvent * e);
	void dropEvent(QDropEvent * e);
	void enterEvent(QEvent * e);
	void hideEvent(QHideEvent * e);
	void keyPressEvent(QKeyEvent * e);
	void keyReleaseEvent(QKeyEvent * e);
	void leaveEvent(QEvent * e);
	void mouseDoubleClickEvent(QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);
	void moveEvent(QMoveEvent * e);
	void resizeEvent(QResizeEvent * e);
	void showEvent(QShowEvent * e);
	void wheelEvent(QWheelEvent * e);
	void paintEvent(QPaintEvent * e);
	void glDraw();
	void glInit();
	void initializeGL();
	void initializeOverlayGL();
	void paintGL();
	void paintOverlayGL();
	void resizeGL(int width, int height);
	void resizeOverlayGL(int width, int height);

};

#endif
