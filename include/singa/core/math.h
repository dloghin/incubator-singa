/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SINGA_CORE_MATH_H_
#define SINGA_CORE_MATH_H_
#include <type_traits>
#include "singa/core/common.h"
#include "singa/utils/logging.h"

namespace singa {

/// \file math.h Math functions for linear algebra, neural net and random
/// operations.
/// All functions have a template argument, DType for DataType, Lib for the
/// backend library, e.g., lib::Cublas, lib::Cudnn, etc.

/// Some operations would have many config/hyper-parameters, e.g., Conv, and
/// these config vary among diff implementations, e.g., cuda/cudnn/opencl.
/// To separate the modules, we pass a OpConf pointer to the Tensor Op function.
/// The specific fields are implemented by inheriting OpConf, and casting the
/// pointer between the base and the sub-class.
class OpConf {
 public:
  template <typename T>
  T* CastTo() {
    static_assert(std::is_base_of<OpConf, T>::value,
                  "The cast type must be a sub-class of OpConf");
    return static_cast<T*>(this);
  }
};

// ================Linear algebra functions====================================
template <typename DType, typename Lib>
void Sum(int count, const Blob* input, DType* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Abs(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Sign(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// Base is e, Neper number
template <typename DType, typename Lib>
void Exp(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// Natual logarithm, the base is e, Neper number.
template <typename DType, typename Lib>
void Log(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Sqrt(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Tanh(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Sigmoid(int count, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// Do v^x for every v from the input tensor
template <typename DType, typename Lib>
void Pow(int count, DType x, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// Do v^x for every v from the lhs and every x from rhs
template <typename DType, typename Lib>
void Pow(int count, const Blob* lhs, const Blob* rhs, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// Clamp every element into [low, high]
template <typename DType, typename Lib>
void Clamp(int count, DType low, DType high, const Blob* input, Blob* ret,
           Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = x + input
template <typename DType, typename Lib>
void Add(int count, DType x, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = x * input
/// div could be enabled by calling Mult with 1/x
template <typename DType, typename Lib>
void Mult(int count, DType x, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = lhs + rhs
template <typename DType, typename Lib>
void Add(int count, const Blob* lhs, const Blob* rhs, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = lhs - rhs
template <typename DType, typename Lib>
void Sub(int count, const Blob* lhs, const Blob* rhs, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = lhs * rhs
template <typename DType, typename Lib>
void Mult(int count, const Blob* lhs, const Blob* rhs, Blob* ret,
          Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = lhs / rhs
template <typename DType, typename Lib>
void Div(int count, const Blob* lhs, const Blob* rhs, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// outer-product.
/// lhs and rhs are vectors of len m and n. ret is matrix of shape m * n
template <typename DType, typename Lib>
void Outer(int m, int n, const Blob* lhs, const Blob* rhs, Blob* ret,
           Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// TODO(wangwei) unify SumRow and SumCol.
/// Sum the rows of the input matrix into a vector
template <typename DType, typename Lib>
void SumRow(int nrow, int ncol, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}
/// Sum the rows of the input matrix into a vector
template <typename DType, typename Lib>
void SumCol(int nrow, int ncol, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// TODO(wangwei) unify AddRow and AddCol.
/// Add the vector v to every row of A as the row of ret
template <typename DType, typename Lib>
void AddRow(int nrow, int ncol, const Blob* A, const Blob* v, Blob* ret,
            Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// Add the vector v to every column of A as the column of ret
template <typename DType, typename Lib>
void AddCol(int nrow, int ncol, const Blob* A, const Blob* v, Blob* ret,
            Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// ===== BLAS functions, ref to http://docs.nvidia.com/cuda/cublas
// ===== Level 1
/// return the index of the element with the max value.
template <typename DType, typename Lib>
void Amax(int count, const Blob* input, int* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// return the index of the element with the min value.
template <typename DType, typename Lib>
void Amin(int count, const Blob* input, int* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}
/// ret = sum |x| for all x in input
template <typename DType, typename Lib>
void Asum(int count, const Blob* input, DType* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret = alpha * input + ret
template <typename DType, typename Lib>
void Axpy(int count, DType alpha, const Blob* input, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret *= x
template <typename DType, typename Lib>
void Scale(int count, DType x, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Dot(int count, const Blob* lhs, const Blob* rhs, DType* ret,
         Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// ===== Level 2
/// ret = alpha * op(A) * v + beta * ret.
/// op(A) = A if trans = false; A^T otherwise; rows(A) = m, cols(A) = n.
template <typename DType, typename Lib>
void GEMV(bool trans, int m, int n, DType alpha, const Blob* A, const Blob* v,
          DType beta, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// ===== Level 3
/// ret = alpha * op(A) * op(B) + beta * ret.
/// op(A) = A if trans = false; A^T otherwise; rows(A) = m, cols(A) = n.
template <typename DType, typename Lib>
void GEMV(bool transA, bool transB, int m, int n, int k, DType alpha,
          const Blob* A, const Blob* B, DType beta, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// ================Random functions===========================================
// The random generator should be extracted from ctx.
template <typename DType, typename Lib>
void Uniform(int count, DType low, DType high, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

template <typename DType, typename Lib>
void Gaussian(int count, DType mean, DType std, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// each element of ret would be 1 with prob p and 0 with 1-p. 0<= p <= 1
template <typename DType, typename Lib>
void Bernoulli(int count, DType p, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

/// ret[i] would be 1 with prob p[i] and 0 with 1-p[i]. 0<= p[i] <= 1
template <typename DType, typename Lib>
void Bernoulli(int count, const Blob* p, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}

// ================Neural net functions=======================================
/// Do 2D conv.
/// c is input image channel, w is input width, h is input height
/// nb_kernel is output channel, kw, and kh are kenerl width and height
/*
template <typename DType, typename Lib>
void Conv2D(int c, int w, int h, int nb_kernel, int kw, int kh,
           const Blob* input, const Blob* kernel, Blob* ret, Context* ctx) {
  LOG(FATAL) << "Not Implemented";
}
*/
}  // namespace singa

#endif  // SINGA_CORE_MATH_H_
