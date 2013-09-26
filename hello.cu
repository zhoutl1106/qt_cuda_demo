#include <iostream>
using namespace std;
#include <helper_cuda.h>
#include <cuda.h>
#include <cufft.h>

#define NX 8192
#define BATCH 128


extern "C" void* runCudaPart(void* _src);
extern "C" void cudaInit();
extern "C" void cudaExit();

float *pfHost;
float *pfDevice;
float *pfSrcDevice;
int nBlocks,nThreads,nSize,n;

__global__ void my_kernel(float*x , void* src)
{
    int tid = threadIdx.x + blockDim.x*blockIdx.x;
    x[tid] = *((float*)src + tid);
}

void cudaInit()
{
    gpuDeviceInit(0);
    nBlocks = 2;
    nThreads = 8;
    nSize = nBlocks * nThreads;
    cudaHostAlloc(&pfHost,nSize*sizeof(float),cudaHostAllocDefault);
    cudaMalloc((void**)&pfDevice,nSize*sizeof(float));
    cudaMalloc((void**)&pfSrcDevice,nSize*sizeof(float));
    cout<<"cudaInit"<<endl;
}

void cudaExit()
{
    cudaFree(pfHost);
    cudaFree(pfDevice);
    cudaFree(pfSrcDevice);
    cout<<"cudaExit"<<endl;
}

void* runCudaPart(void *_src)
{
    cudaMemcpy(pfSrcDevice,_src,nSize * sizeof(float),cudaMemcpyHostToDevice);
    my_kernel <<< nBlocks, nThreads >>> (pfDevice,pfSrcDevice);
    cudaMemcpy(pfHost,pfDevice,nSize * sizeof(float),cudaMemcpyDeviceToHost);

    cufftHandle plan;
    cufftComplex *data,*pHostData;
    cudaMalloc((void**)&data,sizeof(cufftComplex)*NX*BATCH);
    cudaHostAlloc(&pHostData,sizeof(cufftComplex)*NX*BATCH,cudaHostAllocDefault);

    for(int i=0;i<NX;i++)
    {
        pHostData[i].x  = 1000*((float *)_src)[i];
        pHostData[i].y = 0;
    }

    if ( cudaGetLastError() != cudaSuccess )
        cout<<"Cuda e r r o r : F a i l e d t o a l l o c a t e "<<endl  ;

    if ( cufftPlan1d(&plan , NX , CUFFT_C2C , BATCH ) != CUFFT_SUCCESS )
        cout<<"CUFFT e r r o r : Plan c r e a t i o n f a i l e d "<<endl  ;

    cudaMemcpy(data,pHostData,sizeof(cufftComplex)*NX*BATCH,cudaMemcpyHostToDevice);
    if ( cufftExecC2C ( plan , data , data , CUFFT_FORWARD ) != CUFFT_SUCCESS )
        cout<<"CUFFT e r r o r : ExecC2C Forward f a i l e d "<<endl ;

    if ( cufftExecC2C ( plan , data , data , CUFFT_INVERSE ) != CUFFT_SUCCESS )
        cout<<"CUFFT e r r o r : ExecC2C I n v e r s e f a i l e d "<<endl  ;

    if ( cudaThreadSynchronize() != cudaSuccess )
        cout<<"Cuda e r r o r : F a i l e d t o s y n c h r o n i z e " <<endl ;

    cudaMemcpy(pHostData,data,sizeof(cufftComplex)*NX*BATCH,cudaMemcpyDeviceToHost);

    for(int i=0;i<NX;i++)
    {
        //pfHost[i] = sqrt(pHostData[i].x*pHostData[i].x + pHostData[i].y*pHostData[i].y);
        cout<<i<<' '<<pHostData[i].x <<' '<<pHostData[i].y<<' '<<pfHost[i]<<endl;
        pfHost[i]=pHostData[i].y;
    }

    cufftDestroy ( plan ) ;
    cudaFree ( data ) ;


    cout<<"runCuda"<<endl;
    return pfHost;
}
