#include <iostream>
#include "lzcnn_mat.h"
#include "lzcnn_convolution.h"

using namespace lzcnn;

int main()
{
    std::cout << "test convolution " << std::endl;
    int outch = 32;
    zMat input(320, 180, 4, sizeof(float));
    zMat kernel(3*3*4*outch, sizeof(float));
    zMat output;

    Convolution conv_opt(3, 3, 1, 1, outch);

    /* input */
    int size = (int)input.total();
    float *pfdata = input;
    for (int i = 0; i < size; i++)
    {
        pfdata[i] = (float)rand()/RAND_MAX;
    }

    /* kernel */
    size = (int)kernel.total();
    pfdata = kernel;
    for (int i = 0; i < size; i++)
    {
        pfdata[i] = (float)rand()/RAND_MAX;
    }

    for (int i = 0; i < 5; i++)
    {
        conv_opt.forward(input, kernel, output);
    }
    const float *pfodata = output;

    std::cout << "output info : " << output.w << "*" << output.h << "*" << output.c << std::endl;
    std::cout << "pfodata[10]=" << pfodata[10] << std::endl;

    return 0;
}