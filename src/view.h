#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

#include <QFileDialog>
#include <QMainWindow>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSizePolicy>
#include <cmath>
#include <iostream>

#include "model.h"

class glView : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit glView(QWidget *parent = 0);
  ~glView();
  void updateCubeVertices(float *newVertices);
  std::string file_name = "";
  float getScale();
  void setScale(int scale);
  void setxMov(int xMov);
  void setyMov(int yMov);
  void setzMov(int zMov);

  void setxRot(int xRot);
  void setyRot(int yRot);
  void setzRot(int zRot);
  void createBuffer();

  unsigned int get_count_vertex_for_view();
  unsigned int get_count_edges_for_view();

 private:
  Model::Parcer p;

  void drawPicture();

  float xRot_, yRot_, zRot_;
  float xMov_, yMov_, zMov_ = -30;
  QPoint mPos;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

  GLuint vertexBuffer;
  float m_scale = 0;

  QString address_file = "";
  unsigned int _count_vertex_for_view = 0;
  unsigned int _count_edges_for_view = 0;

 protected:
  void initializeGL() override;
  // void resizeGL(int w, int h) override;
  void paintGL() override;

  QPoint lastPos;
  bool isMousePressed;
};

void printMatrix(const QMatrix4x4 &matrix);
void printArray(const float *array, int size);
void init_Matrix4x4(QMatrix4x4 &modelViewProjectionMatrix,
                    Model::Model_Matrix result);

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

  QString address_file = "";

  int heightForWidth(int width) const override {
    return static_cast<int>(width * 12.0 / 16.0);  // Например, 16:9
  }

 private slots:
  void on_pushButton_clicked();

  void on_horizontalSlider_actionTriggered(int action);

  void on_horizontalSlider_2_actionTriggered(int xMov);

  void on_horizontalSlider_3_actionTriggered(int yMov);

  void on_horizontalSlider_4_actionTriggered(int zMov);

  void on_horizontalSlider_5_actionTriggered(int xRot);

  void on_horizontalSlider_6_actionTriggered(int yRot);

  void on_horizontalSlider_7_actionTriggered(int zRot);

 private:
  Ui::MainWindow *ui;
  glView *glViewInstance;
  unsigned int count_vertex = 0;
  unsigned int count_edges = 0;

 protected:
  void resizeEvent(QResizeEvent *event) override {
    QWidget::resizeEvent(event);
    int newWidth = event->size().width();
    int newHeight = heightForWidth(newWidth);
    resize(newWidth, newHeight);
  }
};


#endif  // VIEW_H
