#include "view.h"

#include "ui_view.h"

glView::glView(QWidget *parent) : QOpenGLWidget(parent) {}

glView::~glView() {}

GLuint program;

// Обновленный вершинный шейдер
const char *vertexShader3D =
    "attribute vec3 position;\n"
    "uniform mat4 modelViewProjectionMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projectionMatrix * modelViewProjectionMatrix * "
    "vec4(position, 1.0);\n"
    "}\n";

// Обновленный фрагментный шейдер
const char *fragmentShader3D =
    "void main()\n"
    "{\n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

void glView::createBuffer() {
  p.clear();
  p.start_parcer(file_name);
  _count_vertex_for_view = p.get_count_vertex();
  _count_edges_for_view = p.get_count_vertex() + p.get_count_facets() - 2;

  //  создание буфера вершин
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, p.get_count_vertex(), p.get_array_of_vertex(),
               GL_DYNAMIC_DRAW);  // Используйте GL_DYNAMIC_DRAW для возможности
                                  // обновления данных
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int glView::get_count_vertex_for_view() {
  return _count_vertex_for_view;
}

unsigned int glView::get_count_edges_for_view() {
  return _count_edges_for_view;
}

void glView::initializeGL() {
  initializeOpenGLFunctions();

  // Включаем буфер глубины
  glEnable(GL_DEPTH_TEST);

  // Отключаем проверку лиц обратно
  glDisable(GL_CULL_FACE);

  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShaderId, 1, &vertexShader3D, NULL);

  glCompileShader(vertexShaderId);

  GLint compileOk = GL_FALSE;

  glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compileOk);

  if (compileOk == GL_FALSE) {
    std::cout << "shader compile error"
              << "\n";
  } else {
    std::cout << "shader compile ok"
              << "\n";
  }

  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShaderId, 1, &fragmentShader3D, NULL);

  glCompileShader(fragmentShaderId);

  glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compileOk);

  if (compileOk == GL_FALSE) {
    std::cout << "shader compile error"
              << "\n";
  } else {
    std::cout << "shader compile ok"
              << "\n";
  }

  program = glCreateProgram();
  glAttachShader(program, vertexShaderId);
  glAttachShader(program, fragmentShaderId);
  glLinkProgram(program);

  GLint linkOk = GL_FALSE;

  glGetProgramiv(program, GL_LINK_STATUS, &linkOk);
  if (linkOk == GL_FALSE) {
    std::cout << "link error\n";
  } else {
    std::cout << "link ok\n";
  }

  glClearColor(0.8, 1.0, 0.6, 1.0);
}

// void glView::resizeGL(int w, int h) {}

// Отрисовка трехмерного объекта
void glView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  QMatrix4x4 modelViewProjectionMatrix;
  QMatrix4x4 projectionMatrix;

  projectionMatrix.perspective(60.0f, width() / height(), 0.1f, 100.0f);
  GLint projectionMatrixUniform =
      glGetUniformLocation(program, "projectionMatrix");
  glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE,
                     projectionMatrix.constData());

  Model::Model_Matrix translationMatrix(4, 4);
  translationMatrix.initialization_single_matrix_for_four();

  Model::Model_Matrix rotationMatrix_OX(4, 4);
  rotationMatrix_OX.initialization_single_matrix_for_four();
  Model::Model_Matrix rotationMatrix_OY(4, 4);
  rotationMatrix_OY.initialization_single_matrix_for_four();
  Model::Model_Matrix rotationMatrix_OZ(4, 4);
  rotationMatrix_OZ.initialization_single_matrix_for_four();

  Model::Model_Matrix scaleMatrix(4, 4);
  scaleMatrix.initialization_single_matrix_for_four();

  Model::Affine_Transform a_translationMatrix(&translationMatrix);
  double arrMove[] = {xMov_, yMov_, zMov_};

  a_translationMatrix.move_matrix(arrMove);

  Model::Affine_Transform a_rotationMatrix_OX(&rotationMatrix_OX);
  double arrRot_OX[] = {xRot_, 0, 0};
  a_rotationMatrix_OX.rotation_by_ox(arrRot_OX);

  Model::Affine_Transform a_rotationMatrix_OY(&rotationMatrix_OY);
  double arrRot_OY[] = {0, yRot_, 0};
  a_rotationMatrix_OY.rotation_by_oy(arrRot_OY);

  Model::Affine_Transform a_rotationMatrix_OZ(&rotationMatrix_OZ);
  double arrRot_OZ[] = {0, 0, zRot_};
  a_rotationMatrix_OZ.rotation_by_oz(arrRot_OZ);

  Model::Affine_Transform a_scaleMatrix(&scaleMatrix);
  double arrScale[] = {m_scale};
  a_scaleMatrix.scale_model(arrScale);

  QMatrix4x4 translationMatrix4x4;
  init_Matrix4x4(translationMatrix4x4, translationMatrix);

  QMatrix4x4 rotationMatrix_OX4x4;
  init_Matrix4x4(rotationMatrix_OX4x4, rotationMatrix_OX);
  QMatrix4x4 rotationMatrix_OY4x4;
  init_Matrix4x4(rotationMatrix_OY4x4, rotationMatrix_OY);
  QMatrix4x4 rotationMatrix_OZ4x4;
  init_Matrix4x4(rotationMatrix_OZ4x4, rotationMatrix_OZ);

  QMatrix4x4 rotationMatrix_O =
      rotationMatrix_OX4x4 * rotationMatrix_OY4x4 * rotationMatrix_OZ4x4;

  QMatrix4x4 scaleMatrix4x4;
  init_Matrix4x4(scaleMatrix4x4, scaleMatrix);

  modelViewProjectionMatrix =
      translationMatrix4x4 * rotationMatrix_O * scaleMatrix4x4;

  GLint modelViewProjectionMatrixUniform =
      glGetUniformLocation(program, "modelViewProjectionMatrix");
  glUniformMatrix4fv(modelViewProjectionMatrixUniform, 1, GL_FALSE,
                     modelViewProjectionMatrix.constData());

  // Установка атрибутов вершин
  GLint positionAttribute = glGetAttribLocation(program, "position");
  glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                        p.get_array_of_vertex());

  glEnableVertexAttribArray(positionAttribute);

  glLineWidth(1);
  glDrawElements(GL_LINE_LOOP, p.get_size_facets_array(), GL_UNSIGNED_INT,
                 p.get_array_of_facets());

  glDisableVertexAttribArray(positionAttribute);
}

void glView::updateCubeVertices(float *newVertices) {
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertices),
                  newVertices);  // Обновляет данные в буфере вершин
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glView::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void glView::mouseMoveEvent(QMouseEvent *mo) {
  xRot_ = 1 / M_PI * (mo->pos().y() - mPos.y());
  yRot_ = 1 / M_PI * (mo->pos().x() - mPos.x());
  update();
}

void glView::setScale(int scale) {
  m_scale =
      scale;  // Преобразование значения слайдера в коэффициент масштабирования
  update();   // Запрос на перерисовку
}

float glView::getScale() { return m_scale; }

void glView::setxMov(int xMov) {
  xMov_ =
      xMov;  // Преобразование значения слайдера в коэффициент масштабирования
  update();  // Запрос на перерисовку
}

void glView::setyMov(int yMov) {
  yMov_ =
      yMov;  // Преобразование значения слайдера в коэффициент масштабирования
  update();  // Запрос на перерисовку
}

void glView::setzMov(int zMov) {
  zMov_ =
      zMov;  // Преобразование значения слайдера в коэффициент масштабирования
  update();  // Запрос на перерисовку
}

void glView::setxRot(int xRot) {
  xRot_ =
      xRot;  // Преобразование значения слайдера в коэффициент масштабирования
  update();  // Запрос на перерисовку
}

void glView::setyRot(int yRot) {
  yRot_ =
      yRot;  // Преобразование значения слайдера в коэффициент масштабирования
  update();  // Запрос на перерисовку
}

void glView::setzRot(int zRot) {
  zRot_ =
      zRot;  // Преобразование значения слайдера в коэффициент масштабирования
  update();  // Запрос на перерисовку
}

void printMatrix(const QMatrix4x4 &matrix) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << matrix(i, j) << " ";
    }
    std::cout
        << std::endl;  // Переход на новую строку после каждой строки матрицы
  }
}

void printArray(const float *array, int size) {
  for (int i = 0; i < size; ++i) {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;
}

void init_Matrix4x4(QMatrix4x4 &modelViewProjectionMatrix,
                    Model::Model_Matrix result) {
  modelViewProjectionMatrix = QMatrix4x4(
      result.getElement(0, 0), result.getElement(0, 1), result.getElement(0, 2),
      result.getElement(0, 3), result.getElement(1, 0), result.getElement(1, 1),
      result.getElement(1, 2), result.getElement(1, 3), result.getElement(2, 0),
      result.getElement(2, 1), result.getElement(2, 2), result.getElement(2, 3),
      result.getElement(3, 0), result.getElement(3, 1), result.getElement(3, 2),
      result.getElement(3, 3));
}

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  glViewInstance = ui->widget;
  connect(ui->horizontalSlider, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_actionTriggered);
  connect(ui->horizontalSlider_2, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_2_actionTriggered);
  connect(ui->horizontalSlider_3, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_3_actionTriggered);
  connect(ui->horizontalSlider_4, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_4_actionTriggered);
  connect(ui->horizontalSlider_5, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_5_actionTriggered);
  connect(ui->horizontalSlider_6, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_6_actionTriggered);
  connect(ui->horizontalSlider_7, &QSlider::valueChanged, this,
          &View::on_horizontalSlider_7_actionTriggered);

  QSizePolicy policy = sizePolicy();
  policy.setHeightForWidth(true);
  setSizePolicy(policy);
}

View::~View() { delete ui; }

void View::on_pushButton_clicked() {
  ui->widget->update();

  address_file = QFileDialog::getOpenFileName(0, "Open file", "", "*.obj");
  ui->widget->file_name = address_file.toStdString();
  qDebug() << address_file << "\n";

  // Разбиваем строку по символу '/'
  QStringList parts = address_file.split('/');

  // Получаем последний элемент
  QString lastElement = parts.last();
  ui->label_name_file->setText(lastElement);

  ui->widget->createBuffer();

  QString count_vertex_text =
      QString::number(glViewInstance->get_count_vertex_for_view());
  QString count_edges_text =
      QString::number(glViewInstance->get_count_edges_for_view());

  ui->label_set_vertex->setText(count_vertex_text);
  ui->label_set_edges->setText(count_edges_text);

  ui->widget->update();
}

void View::on_horizontalSlider_actionTriggered(int action) {
  glViewInstance->setScale(action);
  qDebug() << action;
}

void View::on_horizontalSlider_2_actionTriggered(int xMov) {
  glViewInstance->setxMov(xMov);
  qDebug() << xMov;
}

void View::on_horizontalSlider_3_actionTriggered(int yMov) {
  glViewInstance->setyMov(yMov);
  qDebug() << yMov;
}

void View::on_horizontalSlider_4_actionTriggered(int zMov) {
  glViewInstance->setzMov(zMov);
  qDebug() << zMov;
}

void View::on_horizontalSlider_5_actionTriggered(int xRot) {
  glViewInstance->setxRot(xRot);
  qDebug() << xRot;
}

void View::on_horizontalSlider_6_actionTriggered(int yRot) {
  glViewInstance->setyRot(yRot);
  qDebug() << yRot;
}

void View::on_horizontalSlider_7_actionTriggered(int zRot) {
  glViewInstance->setzRot(zRot);
  qDebug() << zRot;
}
