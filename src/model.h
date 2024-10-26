#ifndef MODEL_H
#define MODEL_H

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Model {
 public:
  Model();
  class Parcer {
   public:
    Parcer();
    ~Parcer();
    void start_parcer(std::string str);
    float *get_array_of_vertex() { return _array_of_vertex; }
    unsigned int get_count_vertex() { return _count_of_vertex; }
    unsigned int get_count_facets() { return _count_of_facets; }
    unsigned int *get_array_of_facets() { return _array_of_facets; }
    int get_size_facets_array() { return _facets.size(); }
    void clear();

    //    unsigned int get_count_of_vertex();

   private:
    unsigned int _count_of_vertex;
    unsigned int _count_of_facets;
    unsigned int _size_facets;
    float *_array_of_vertex;
    unsigned int *_array_of_facets;
    std::vector<unsigned int> _facets;

    void count_the_vertex_and_facets(std::string str);
    void fill_vertex_and_facets(std::string str);
    void fill_vertex_array(std::string s, int *ind);
    void fill_facets_vector(std::string s);
    void fill_facets_array();
    float my_stof(std::string &str, int &i);
  };

  class Model_Matrix {
   public:
    //  default constructor
    Model_Matrix();
    //  parameterized constructor
    Model_Matrix(int rows, int cols);
    //  copy costructor
    Model_Matrix(const Model_Matrix &other);
    //  move costructor
    Model_Matrix(Model_Matrix &&other);
    //  destructor
    ~Model_Matrix();

    void initialization_single_matrix_for_four();
    Model_Matrix &operator=(const std::initializer_list<double> &lst);
    bool EqMatrix(const Model_Matrix &other) const;

    int GetRows_() const;
    int GetCols_() const;

    // Метод для получения указателя на данные (константный)
    const float *constData() const { return data_; }
    void showMatrix();

    double &getElement(int row, int col) { return matrix_[row][col]; }

    void setElement(int row, int col, double value) {
      matrix_[row][col] = value;
    }

   private:
    // void SetRowCol(int i, int j);
    void CopyMatrix(const Model_Matrix &other);
    void MemoryAllocation(int rows, int cols);

    double **matrix_;
    int rows_, cols_;
    float *data_;  // Массив для хранения элементов матрицы 4x4
  };

  class Affine_Transform {
   public:
    Affine_Transform(Model_Matrix *data) : matrixObj_{data} {}
    ~Affine_Transform() = default;

    void move_matrix(double *TransformData);
    void rotation_by_ox(double *TransformData);
    void rotation_by_oy(double *TransformData);
    void rotation_by_oz(double *TransformData);
    void scale_model(double *TransformData);

   private:
    Model_Matrix *matrixObj_;
  };
};

#endif  // MODEL_H
