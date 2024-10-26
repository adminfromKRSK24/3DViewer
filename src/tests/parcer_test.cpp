#include "../model.h"
#include <gtest/gtest.h>
#include <string>

std::string path1 = "/Users/doduofor/goinfre/CPP4_3DViewer_v2.0-1/src/img/cube.obj";
std::string path2 = "/Users/doduofor/goinfre/CPP4_3DViewer_v2.0-1/src/img/Earth.obj";

TEST(parcer, test_count_vertex_2) {
  Model::Parcer p;
  p.start_parcer(path2);
  ASSERT_FALSE(p.get_count_vertex() == 1150);
  ASSERT_EQ(p.get_count_vertex(), 1158);
}

TEST(parcer, test_count_facets_2) {
  Model::Parcer p;
  p.start_parcer(path2);
  ASSERT_FALSE(p.get_count_facets() == 1150);
  ASSERT_EQ(p.get_count_facets(), 1152);
}

TEST(parcer, test_size_facets_array_2) {
  Model::Parcer p;
  p.start_parcer(path2);
  ASSERT_FALSE(p.get_size_facets_array() == 1234);
  ASSERT_EQ(p.get_size_facets_array(), 4608);
}

TEST(parcer, test_array_vertex_1) {
  float arr[] = {0, 0, 0,
                10.000000, 10.000000, 0.000000,
                0.000000, 10.000000, 0.000000,
                0.000000, 10.000000, 10.000000,
                10.000000, 10.000000, 10.000000,
                10.000000, 0.000000, 0.000000,
                10.000000, 0.000000, 10.000000,
                0.000000, 0.000000, 0.000000,
                0.000000, 0.000000, 10.000000};
  Model::Parcer p;
  p.start_parcer(path1);
  
  for (unsigned int i = 0; i < p.get_count_vertex() + 3; ++i) {
    ASSERT_FLOAT_EQ(p.get_array_of_vertex()[i], arr[i]);
  }
}

TEST(parcer, test_array_facets_1) {
  unsigned int arr[] = {1, 2, 4,
                        4, 2, 3,
                        5, 1, 6,
                        6, 1, 4,
                        7, 5, 8,
                        8, 5, 6,
                        2, 7, 3,
                        3, 7, 8,
                        8, 6, 3,
                        3, 6, 4,
                        2, 1, 7,
                        7, 1, 5};
  Model::Parcer p;
  p.start_parcer(path1);
  
  for (unsigned int i = 0; i < p.get_count_vertex() + 3; ++i) {
    ASSERT_EQ(p.get_array_of_facets()[i], arr[i]);
  }
}

TEST(affine_transforms, test_rotation_by_ox) {
  Model::Model_Matrix m(4, 4);
  m.initialization_single_matrix_for_four();
  Model::Affine_Transform A_m(&m);

  double arrRot_OX[] = {1, 0, 0};
  A_m.rotation_by_ox(arrRot_OX);

  Model::Model_Matrix m1(4, 4);
  m1 = {1, 0, 0, 0,
        0, 0.999848, -0.0174524, 0,
        0, 0.0174524, 0.999848, 0,
        0, 0, 0, 1};

  ASSERT_TRUE(true == m.EqMatrix(m1));
}

TEST(affine_transforms, test_rotation_by_oy) {
  Model::Model_Matrix m(4, 4);
  m.initialization_single_matrix_for_four();
  Model::Affine_Transform A_m(&m);

  double arrRot_OY[] = {0, 1, 0};
  A_m.rotation_by_oy(arrRot_OY);

  Model::Model_Matrix m1(4, 4);
  m1 = {0.999848, 0, 0.0174524, 0,
        0, 1, 0, 0,
        -0.0174524, 0, 0.999848, 0,
        0, 0, 0, 1};

  ASSERT_TRUE(true == m.EqMatrix(m1));
}

TEST(affine_transforms, test_rotation_by_oz) {
  Model::Model_Matrix m(4, 4);
  m.initialization_single_matrix_for_four();
  Model::Affine_Transform A_m(&m);

  double arrRot_OZ[] = {0, 0, 1};
  A_m.rotation_by_oz(arrRot_OZ);

  m.showMatrix();

  Model::Model_Matrix m1(4, 4);
  m1 = {0.999848, -0.0174524, 0, 0,
        0.0174524, 0.999848, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

  ASSERT_TRUE(true == m.EqMatrix(m1));
}

TEST(affine_transforms, test_scale) {
  Model::Model_Matrix m(4, 4);
  m.initialization_single_matrix_for_four();
  Model::Affine_Transform A_m(&m);

  double arrScale[] = {0.5};
  A_m.scale_model(arrScale);

  Model::Model_Matrix m1(4, 4);
  m1 = {0.5, 0, 0, 0,
        0, 0.5, 0, 0,
        0, 0, 0.5, 0,
        0, 0, 0, 1};

  ASSERT_TRUE(true == m.EqMatrix(m1));
}

TEST(affine_transforms, test_move) {
  Model::Model_Matrix m(4, 4);
  m.initialization_single_matrix_for_four();
  Model::Affine_Transform A_m(&m);

  double arrMove[] = {0.5, 0.7, 0.9};
  A_m.move_matrix(arrMove);

  m.showMatrix();

  Model::Model_Matrix m1(4, 4);
  m1 = {1, 0, 0, 0.5,
        0, 1, 0, 0.7,
        0, 0, 1, 0.9,
        0, 0, 0, 1};

  ASSERT_TRUE(true == m.EqMatrix(m1));
}

TEST(Model_Matrix, test_copy) {
  Model::Model_Matrix m1(4, 4);
  m1 = {1, 0, 0, 0.5,
        0, 1, 0, 0.7,
        0, 0, 1, 0.9,
        0, 0, 0, 1};

  Model::Model_Matrix m2(m1);
  
  ASSERT_TRUE(true == m2.EqMatrix(m1));
}

TEST(Model_Matrix, test_eqMatrix) {
  Model::Model_Matrix m1(4, 4);
  m1 = {1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

  m1.showMatrix();

  Model::Model_Matrix m2(4, 4);
  m2 = {1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

  ASSERT_TRUE(false == m2.EqMatrix(m1));
}

TEST(Model_Matrix, test_GetRows) {
  Model::Model_Matrix m1(4, 4);
  m1 = {1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

  m1.showMatrix();

  ASSERT_EQ(m1.GetRows_(), 4);
}

TEST(Model_Matrix, test_GetCols) {
  Model::Model_Matrix m1(4, 4);
  m1 = {1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

  m1.showMatrix();

  ASSERT_EQ(m1.GetCols_(), 4);
}

TEST(Model_Matrix, test_Throw) {

  ASSERT_THROW(Model::Model_Matrix m1(0, 0), std::invalid_argument);
}
