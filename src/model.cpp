#include "model.h"

Model::Parcer::Parcer() {
  _count_of_vertex = 0;
  _count_of_facets = 0;
}

Model::Parcer::~Parcer() {
  _count_of_facets = 0;
  _count_of_vertex = 0;
  _size_facets = 0;

  delete[] _array_of_vertex;
  _array_of_vertex = nullptr;  // Устанавливаем в nullptr для безопасности
  delete[] _array_of_facets;
  _array_of_facets = nullptr;  // Устанавливаем в nullptr для безопасности
  _facets.clear();
}

void Model::Parcer::clear() {
  _count_of_facets = 0;
  _count_of_vertex = 0;
  _size_facets = 0;
  delete _array_of_vertex;
  delete _array_of_facets;
  _facets.clear();
}

float Model::Parcer::my_stof(std::string &str, int &i) {
  float int_part = 0;
  float result = 0;
  bool znak = 0;

  if (str[i] == '-') {
    znak = 1;
    ++i;
  }

  for (; str[i] >= '0' && str[i] <= '9'; ++i) {
    int_part *= 10;
    int_part += str[i] - '0';
  }
  result = int_part;

  if (str[i] == '.') {
    ++i;
    float real_part = 0;
    float tmp = 10;
    for (; str[i] >= '0' && str[i] <= '9'; ++i) {
      real_part += (str[i] - '0') / tmp;
      tmp *= 10;
    }
    result += real_part;
  }

  if (str[i] == 'e') {
    ++i;
    bool sign = 0;
    int e = 0;
    if (str[i] == '-') {
      sign = 1;
      ++i;
    }
    for (; str[i] >= '0' && str[i] <= '9'; ++i) {
      e *= 10;
      e += str[i] - '0';
    }
    if (sign) e = -e;
    result *= pow(10, e);
  }

  if (znak == 1) {
    result = -result;
  }
  return result;
}

void Model::Parcer::fill_facets_array() {
  _array_of_facets = new unsigned int[_facets.size()]();
  for (unsigned int i = 0; i < _facets.size(); ++i) {
    _array_of_facets[i] = _facets[i];
  }
}

void Model::Parcer::start_parcer(std::string str) {
  count_the_vertex_and_facets(str);
  fill_vertex_and_facets(str);
  fill_facets_array();
}

void Model::Parcer::count_the_vertex_and_facets(std::string str) {
  std::ifstream file(str);
  std::string tmp_str;
  _count_of_vertex = 0;
  _count_of_facets = 0;
  while (std::getline(file, tmp_str)) {
    if (tmp_str[0] == 'v' && tmp_str[1] == ' ') {
      ++_count_of_vertex;
    } else if (tmp_str[0] == 'f' && tmp_str[1] == ' ') {
      ++_count_of_facets;
    }
  }
  file.close();
}

void Model::Parcer::fill_vertex_and_facets(std::string str) {
  std::ifstream file(str);
  std::string tmp_str;

  _array_of_vertex = new float[_count_of_vertex * 3 + 3]();

  int ind_vertex = 3;
  while (std::getline(file, tmp_str)) {
    if (tmp_str[0] == 'v' && tmp_str[1] == ' ') {
      fill_vertex_array(tmp_str, &ind_vertex);
    } else if (tmp_str[0] == 'f' && tmp_str[1] == ' ') {
      fill_facets_vector(tmp_str);
    }
  }

  file.close();
}

void Model::Parcer::fill_facets_vector(std::string s) {
  for (int i = 2; s[i]; ++i) {
    if (s[i] >= '0' && s[i] <= '9') {
      _facets.push_back(std::stof(&s[i]));
    }
    while (!(s[i] == ' ' || s[i] == 0)) {
      ++i;
    }
    if (s[i] == 0) break;
  }
}

void Model::Parcer::fill_vertex_array(std::string s, int *ind) {
  int i = 0;
  while (s[i] != 0) {
    if (s[i] == ' ' || s[i] == 'v') {
      ++i;
    }
    if (s[i] == '-' || (s[i] >= '0' && s[i] <= '9')) {
      _array_of_vertex[(*ind)++] = my_stof(s, i);
    }
    while (!(s[i] == ' ' || s[i] == 0)) {
      ++i;
    }
  }
}

Model::Model_Matrix::Model_Matrix() : rows_{1}, cols_{1} {
  MemoryAllocation(rows_, cols_);
}

Model::Model_Matrix::Model_Matrix(int rows, int cols)
    : rows_{rows}, cols_{cols} {
  MemoryAllocation(rows_, cols_);
}

//  Конструктор копирования
Model::Model_Matrix::Model_Matrix(const Model_Matrix &other)
    : Model_Matrix(other.rows_, other.cols_) {
  CopyMatrix(other);
}

//  Конструктор перемещения
// Model::Model_Matrix::Model_Matrix(Model_Matrix &&other)
//     : rows_(0), cols_(0), matrix_(nullptr) {
//   std::swap(matrix_, other.matrix_);
//   std::swap(rows_, other.rows_);
//   std::swap(cols_, other.cols_);
// }

//  Деструктор
Model::Model_Matrix::~Model_Matrix() {
  if (matrix_ != nullptr) {
    for (int count = 0; count < rows_; count++) delete[] matrix_[count];
    delete[] matrix_;
  }

  delete[] data_;
}

void Model::Model_Matrix::MemoryAllocation(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw std::invalid_argument("INCORRECT_MATRIX");
  }

  matrix_ = new double *[rows];
  data_ = new float[16]();

  for (int count = 0; count < rows; count++) {
    matrix_[count] = new double[cols]{};
  }
}

void Model::Model_Matrix::CopyMatrix(const Model_Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("MATRIX_SIZE");
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      this->matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

bool Model::Model_Matrix::EqMatrix(const Model_Matrix &other) const {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (fabs(fabs(matrix_[i][j]) - fabs(other.matrix_[i][j])) >= 1e-06) {
        return false;
      }
    }
  }

  return true;
}

//  accessor
int Model::Model_Matrix::GetRows_() const { return rows_; }

int Model::Model_Matrix::GetCols_() const { return cols_; }

void Model::Model_Matrix::initialization_single_matrix_for_four() {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = 0;

      if (i == j) {
        matrix_[i][j] = 1;
      }
    }
  }
}

Model::Model_Matrix &Model::Model_Matrix::operator=(
    const std::initializer_list<double> &lst) {
  for (int i = 0, k = 0; i != rows_; ++i) {
    for (int j = 0; j != cols_; ++j, k++) {
      matrix_[i][j] = *(lst.begin() + k);
    }
  }

  return *this;
}

void Model::Model_Matrix::showMatrix() {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      std::cout << matrix_[i][j] << " ";
    }

    std::cout << std::endl;
  }
}

double degree_to_radian(double degree) { return degree * (M_PI / 180); }

void Model::Affine_Transform::move_matrix(double *TransformData) {
  matrixObj_->initialization_single_matrix_for_four();

  matrixObj_->setElement(0, 3, TransformData[0]);
  matrixObj_->setElement(1, 3, TransformData[1]);
  matrixObj_->setElement(2, 3, TransformData[2]);
}

void Model::Affine_Transform::rotation_by_ox(double *TransformData) {
  // Инициализация матрицы вращения
  matrixObj_->initialization_single_matrix_for_four();

  double x = degree_to_radian(TransformData[0]);

  matrixObj_->setElement(1, 1, cos(x));
  matrixObj_->setElement(1, 2, -sin(x));
  matrixObj_->setElement(2, 1, sin(x));
  matrixObj_->setElement(2, 2, cos(x));
}

void Model::Affine_Transform::rotation_by_oy(double *TransformData) {
  matrixObj_->initialization_single_matrix_for_four();

  double y = degree_to_radian(TransformData[1]);

  matrixObj_->setElement(0, 0, cos(y));
  matrixObj_->setElement(0, 2, sin(y));
  matrixObj_->setElement(2, 0, -sin(y));
  matrixObj_->setElement(2, 2, cos(y));
}

void Model::Affine_Transform::rotation_by_oz(double *TransformData) {
  matrixObj_->initialization_single_matrix_for_four();

  double z = degree_to_radian(TransformData[2]);

  matrixObj_->setElement(0, 0, cos(z));
  matrixObj_->setElement(0, 1, -sin(z));
  matrixObj_->setElement(1, 0, sin(z));
  matrixObj_->setElement(1, 1, cos(z));
}

void Model::Affine_Transform::scale_model(double *TransformData) {
  matrixObj_->initialization_single_matrix_for_four();

  if (TransformData[0] != 0) {
    matrixObj_->setElement(0, 0, TransformData[0]);
    matrixObj_->setElement(1, 1, TransformData[0]);
    matrixObj_->setElement(2, 2, TransformData[0]);

    matrixObj_->setElement(3, 3, 1);
  } else if (TransformData[0] < 0) {
    matrixObj_->setElement(0, 0, TransformData[0] + (-0.1));
    matrixObj_->setElement(1, 1, TransformData[0] + (-0.1));
    matrixObj_->setElement(2, 2, TransformData[0] + (-0.1));

    matrixObj_->setElement(3, 3, 1);
  } else {
    matrixObj_->setElement(0, 0, TransformData[0] + 0.1);
    matrixObj_->setElement(1, 1, TransformData[0] + 0.1);
    matrixObj_->setElement(2, 2, TransformData[0] + 0.1);

    matrixObj_->setElement(3, 3, 1);
  }
}
