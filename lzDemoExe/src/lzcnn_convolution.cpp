#include <iostream>
#include <boost/timer.hpp>
#include "lzcnn_convolution.h"

namespace lzcnn
{
    Convolution::Convolution(int kw, int kh, int stridw, int stridh, int outchannel)
        :kernel_w(kw), kernel_h(kh), stride_w(stridw), stride_h(stridh), num_output(outchannel)
    {}

    Convolution::~Convolution()
    {}

    int Convolution::forward(const zMat& bottom_blob, const zMat& kernel_blob, zMat& top_blob)
    {
        int w = bottom_blob.w;
        int h = bottom_blob.h;
        int channels = bottom_blob.c;
        size_t elemsize = bottom_blob.elemsize;
        const int kernel_extent_w = (kernel_w - 1) + 1;
        const int kernel_extent_h = (kernel_h - 1) + 1;
        int outw = (w - kernel_extent_w) / stride_w + 1;
        int outh = (h - kernel_extent_h) / stride_h + 1;

        // float32
        top_blob.create(outw, outh, num_output, elemsize);

        boost::timer ter;
        conv_ref_c(bottom_blob, kernel_blob, top_blob);
        std::cout << "conv_ref_c        : " << ter.elapsed()*1000 << std::endl;

        // float32
        zMat top_blob1(outw, outh, num_output, elemsize);
        ter.restart();
        conv_im2col_sgemm(bottom_blob, kernel_blob, top_blob1);
        std::cout << "conv_im2col_sgemm : " << ter.elapsed() * 1000 << std::endl;

        compare_data(top_blob, top_blob1);

        // float32
        zMat top_blob2(outw, outh, num_output, elemsize);
        ter.restart();
        conv_im2row_sgemm(bottom_blob, kernel_blob, top_blob2);
        std::cout << "conv_im2col_sgemm : " << ter.elapsed() * 1000 << std::endl;

        compare_data(top_blob, top_blob2);

        return 0;
    }

    void Convolution::conv_ref_c(const zMat& bottom_blob, const zMat& kernel_blob, zMat& top_blob)
    {
        int inw  = bottom_blob.w;
        int inc  = bottom_blob.c;
        int outw = top_blob.w;
        int outh = top_blob.h;
        int outc = top_blob.c;
        const int maxk = kernel_w * kernel_h;

        for (int p = 0; p < outc; p++)
        {
            float* outptr = top_blob.channel(p);

            for (int i = 0; i < outh; i++)
            {
                for (int j = 0; j < outw; j++)
                {
                    float sum = 0.0f;
                    const float *kptr = (const float*)kernel_blob + maxk * inc * p;
                    
                    for (int q = 0; q < inc; q++)
                    {
                        const zMat m = bottom_blob.channel(q);
                        const float *sptr = m.row(i*stride_h) + j*stride_w;

                        for (int ky = 0; ky < kernel_h; ky++)
                        {
                            const float *ysptr = sptr + ky*inw;
                            for (int kx = 0; kx < kernel_w; kx++)
                            {
                                float val = ysptr[kx];
                                float w = kptr[ky*kernel_w + kx];
                                sum += val*w;
                            }
                        }
                        kptr += maxk;
                    }
                    outptr[j] = sum;
                }
                outptr += outw;
            }
        }

        return;
    }

    void Convolution::conv_im2col_sgemm(const zMat &bottom_blob, const zMat & kernel_blob, zMat &top_blob)
    {
        int inw = bottom_blob.w;
        int inc = bottom_blob.c;
        size_t elemsize = bottom_blob.elemsize;
        int outw = top_blob.w;
        int outh = top_blob.h;
        int outc = top_blob.c;

        zMat bottom_im2col(outw*outh, kernel_w*kernel_h*inc, elemsize);
        {
            const int stride = kernel_w*kernel_h*outw*outh;
            float *pfret = (float *)bottom_im2col;

            for (int p = 0; p < inc; p++)
            {
                const float *input = bottom_blob.channel(p);
                int retID = stride*p;

                for (int u = 0; u < kernel_h; u++)
                {
                    for (int v = 0; v < kernel_w; v++)
                    {
                        for (int i = 0; i < outh; i++)
                        {
                            for (int j = 0; j < outw; j++)
                            {
                                int row = u + i * stride_h;
                                int col = v + j * stride_w;
                                int index = row * inw + col;
                                pfret[retID] = input[index];
                                retID++;
                            }
                        }
                    }
                }
            }
        }

        int N = outw * outh;                // outsize
        int L = kernel_w * kernel_h * inc;  // ksize * inch
        for (int p = 0; p < outc; p++)
        {
            float *pfoutput = top_blob.channel(p);
            const float *kptr = (const float*)kernel_blob + L * p;

            for (int q = 0; q < N; q++)
            {
                float sum = 0.0f;
                const float *pfin = (const float *)bottom_im2col + q;

                for (int i = 0; i < L; i++)
                {
                    float w = kptr[i];
                    float val = pfin[0];
                    sum += val*w;
                    pfin += N;
                }
                pfoutput[q] = sum;
            }
        }

        return;
    }

    void Convolution::conv_im2row_sgemm(const zMat &bottom_blob, const zMat & kernel_blob, zMat &top_blob)
    {
        int inw = bottom_blob.w;
        int inc = bottom_blob.c;
        size_t elemsize = bottom_blob.elemsize;
        int outw = top_blob.w;
        int outh = top_blob.h;
        int outc = top_blob.c;

        zMat bottom_im2row(kernel_w*kernel_h*inc, outw*outh, elemsize);
        {
            float *pfret = (float *)bottom_im2row;
            int retID = 0;

            for (int i = 0; i < outh; i++)
            {
                for (int j = 0; j < outw; j++)
                {
                    for (int p = 0; p < inc; p++)
                    {
                        const float *pfindata = bottom_blob.channel(p);
                        for (int u = 0; u < kernel_h; u++)
                        {
                            for (int v = 0; v < kernel_h; v++)
                            {
                                int row = u + i*stride_h;
                                int col = v + j*stride_w;
                                int index = row*inw + col;
                                pfret[retID] = pfindata[index];
                                retID++;
                            }
                        }
                    }
                }
            }
        }

        int N = outw * outh;                // outsize
        int L = kernel_w * kernel_h * inc;  // ksize * inch
        for (int p = 0; p < outc; p++)
        {
            float *pfout = top_blob.channel(p);
            const float *pfkptr = (const float *)kernel_blob + L*p;
            const float *pfin = bottom_im2row;
            int retID = 0;
            
            for (int i = 0; i < N; i++)
            {
                float sum = 0.0f;
                for (int j = 0; j < L; j++)
                {
                    float val = pfin[j];
                    float w = pfkptr[j];
                    sum += val*w;
                }
                pfout[retID] = sum;
                retID++;
                pfin += L;
            }
        }

        return;
    }

    int Convolution::compare_data(const zMat& data_blob1, const zMat& data_blob2)
    {
        int size = data_blob1.w * data_blob1.h;
        int channels = data_blob1.c;

        for (int p = 0; p < channels; p++)
        {
            const float *pfdata1 = data_blob1.channel(p);
            const float *pfdata2 = data_blob2.channel(p);

            for (int i = 0; i < size; i++)
            {
                if (abs(pfdata1[i] - pfdata2[i]) > 1e-3)
                {
                    std::cout << "ERROR :  having different data, check up!!! (" << p << " , " << i << ")" << std::endl;
                    return 1;
                }
            }
        }

        return 0;
    }
}