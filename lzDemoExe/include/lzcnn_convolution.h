#ifndef LZCNN_CONVOLUTION_H
#define LZCNN_CONVOLUTION_H

#include "lzcnn_mat.h"

namespace lzcnn
{
    class Convolution
    {
    public:
        Convolution(int kw, int kh, int stridw, int stridh, int outchannel);
        ~Convolution();

        int forward(const zMat& bottom_blob, const zMat& kernel_blob, zMat& top_blob);

    private:
        void conv_ref_c(const zMat& bottom_blob, const zMat& kernel_blob, zMat& top_blob);
        void conv_im2col_sgemm(const zMat &bottom_blob, const zMat & kernel_blob, zMat &top_blob);
        void conv_im2row_sgemm(const zMat &bottom_blob, const zMat & kernel_blob, zMat &top_blob);
        int compare_data(const zMat& data_blob1, const zMat& data_blob2);

    private:
        int num_output;
        int kernel_w;
        int kernel_h;
        int stride_w;
        int stride_h;
    };
}

#endif
