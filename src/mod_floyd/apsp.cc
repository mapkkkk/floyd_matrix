#include "graph.hh"
#include <omp.h>

Graph Graph::apsp()
{
    Graph result(*this);
    int line_len = vertex_num_ * vertex_num_;
    int n = vertex_num_;
    int done = 1;
    int* l = new int[line_len];
    int* lnew = new int[line_len];
    // 将二维邻接矩阵转换为一维数组
#pragma omp parallel for
    for (int i = 0; i < x; i++) {
#pragma omp parallel for
        for (int j = 0; j < y; j++) {
            l[j * x + i] = result[i][j];
        }
    }
    // 将l复制到lnew
    std::memcpy(lnew, l, sizeof(int) * x * y);
#pragma omp parallel for shared(l, lnew) reduction(&& : done)
    // 模平方的Floyd
    for (int j = 0; j < n; ++j)
    {
        for (int i = 0; i < n; ++i)
        {
            int lij = lnew[j * n + i];
            for (int k = 0; k < n; ++k)
            {
                int lik = l[k * n + i];
                int lkj = l[j * n + k];
                if (lik + lkj < lij)
                {
                    lij = lik + lkj;
                    done = 0;
                }
            }
            lnew[j * n + i] = lij;
        }
    }
    // 将一维数组l转换为二维邻接矩阵
    for (int i = 0; i < n; i++) {
#pragma omp parallel for
        for (int j = 0; j < n; j++) {
#pragma omp parallel for
            result[i][j] = lnew[j * x + i];
        }
    }
    return result;
}