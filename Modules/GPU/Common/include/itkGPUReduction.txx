/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkGPUReduction_txx
#define __itkGPUReduction_txx

#include "itkGPUReduction.h"
#include "itkMacro.h"

namespace itk
{
/**
 * Default constructor
 */
template< class TElement >
GPUReduction< TElement >
::GPUReduction()
{
  /*** Prepare GPU opencl program ***/
  m_GPUKernelManager = GPUKernelManager::New();
  m_GPUDataManager = NULL;

}
template< class TElement >
GPUReduction< TElement >
::~GPUReduction()
{
  this->ReleaseGPUInputBuffer();
}

/**
 * Standard "PrintSelf" method.
 */
template< class TElement >
void
GPUReduction< TElement >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  //GetTypenameInString( typeid(TElement), os);
}

template< class TElement >
unsigned int
GPUReduction< TElement >
::NextPow2( unsigned int x ) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

template< class TElement >
bool
GPUReduction< TElement >
::isPow2(unsigned int x)
{
    return ((x&(x-1)) == 0);
}

template< class TElement >
void
GPUReduction< TElement >
::GetNumBlocksAndThreads(int whichKernel, int n, int maxBlocks, int maxThreads, int &blocks, int &threads)
{
    if (whichKernel < 3)
    {
        threads = (n < maxThreads) ? this->NextPow2(n) : maxThreads;
        blocks = (n + threads - 1) / threads;
    }
    else
    {
        threads = (n < maxThreads*2) ? this->NextPow2((n + 1)/ 2) : maxThreads;
        blocks = (n + (threads * 2 - 1)) / (threads * 2);
    }


    if (whichKernel == 6)
      {
      if (maxBlocks < blocks)
        {
        blocks = maxBlocks;
        }
      }
}

template< class TElement >
unsigned int
GPUReduction< TElement >
::GetReductionKernel(int whichKernel, int blockSize, int isPowOf2)
{
  if ( whichKernel != 5 && whichKernel != 6 )
    {
    itkExceptionMacro(<< "Reduction kernel undefined!");
    return 0;
    }

  std::ostringstream defines;
  std::string oclSrcPath = "./../OpenCL/GPUReduction.cl";

  defines << "#define blockSize " << blockSize << std::endl;
  defines << "#define nIsPow2 " << isPowOf2 << std::endl;

  defines << "#define T ";
  GetTypenameInString( typeid ( TElement ), defines );

  std::cout << "Defines: " << defines.str() << "Source code path: " << oclSrcPath << std::endl;

  // load and build program
  this->m_GPUKernelManager->LoadProgramFromFile( oclSrcPath.c_str(), defines.str().c_str() );

  std::ostringstream kernelName;
  kernelName << "reduce" << whichKernel;
  unsigned int handle = this->m_GPUKernelManager->CreateKernel(kernelName.str().c_str());

  size_t wgSize;
  this->m_GPUKernelManager->GetKernelWorkGroupInfo(handle, CL_KERNEL_WORK_GROUP_SIZE, &wgSize);

  m_SmallBlock = (wgSize == 64);

  // NOTE: the program will get deleted when the kernel is also released
  //this->m_GPUKernelManager->ReleaseProgram();

  return handle;
}

template< class TElement >
void
GPUReduction< TElement >
::AllocateGPUInputBuffer(TElement *h_idata)
{
  unsigned int bytes = m_Size * sizeof(TElement);

  m_GPUDataManager = GPUDataManager::New();
  m_GPUDataManager->SetBufferSize( bytes );
  m_GPUDataManager->SetCPUBufferPointer( h_idata );
  m_GPUDataManager->Allocate();

  if (h_idata)
    {
    m_GPUDataManager->SetGPUDirtyFlag(true);
    }
}

template< class TElement >
void
GPUReduction< TElement >
::ReleaseGPUInputBuffer()
{
  if (m_GPUDataManager == (GPUDataPointer)NULL)
    {
    return;
    }

  m_GPUDataManager->Initialize();
}

template< class TElement >
TElement
GPUReduction< TElement >
::RandomTest()
{
  int size = (1<<24)-1917;    // number of elements to reduce

  this->InitializeKernel(size);

  unsigned int bytes = size * sizeof(TElement);
  TElement* h_idata = (TElement*)malloc(bytes);

  for(int i=0; i<size; i++)
  {
      // Keep the numbers small so we don't get truncation error in the sum
      h_idata[i] = (TElement)(rand() & 0xFF);
  }

  this->AllocateGPUInputBuffer(h_idata);

  TElement gpu_result = this->GPUGenerateData();
  std::cout << "GPU result = " << gpu_result << std::endl << std::flush;

  TElement cpu_result = this->CPUGenerateData(h_idata, size);
  std::cout << "CPU result = " << cpu_result << std::endl;

  this->ReleaseGPUInputBuffer();

  free(h_idata);

  return 0;
}

template< class TElement >
void
GPUReduction< TElement >
::InitializeKernel(unsigned int size)
{
  m_Size = size;

  // Create a testing kernel to decide block size
  //m_TestGPUKernelHandle = this->GetReductionKernel(6, 64, 1);
  //m_GPUKernelManager->ReleaseKernel(kernelHandle);

  // number of threads per block
  m_SmallBlock = true;
  int maxThreads = m_SmallBlock ? 64 : 128;

  int whichKernel = 6;
  int maxBlocks = 64;

  int numBlocks = 0;
  int numThreads = 0;

  this->GetNumBlocksAndThreads(whichKernel, size, maxBlocks, maxThreads, numBlocks, numThreads);
  m_ReduceGPUKernelHandle = this->GetReductionKernel(whichKernel, numThreads, isPow2(size) );

}

template< class TElement >
TElement
GPUReduction< TElement >
::GPUGenerateData()
{
  unsigned int size = m_Size;

  // number of threads per block
  int maxThreads = m_SmallBlock ? 64 : 128;

  int whichKernel = 6;
  int maxBlocks = 64;
  bool cpuFinalReduction = true;
  int cpuFinalThreshold = 1;

  int numBlocks = 0;
  int numThreads = 0;

  this->GetNumBlocksAndThreads(whichKernel, size, maxBlocks, maxThreads, numBlocks, numThreads);

  if (numBlocks == 1) cpuFinalThreshold = 1;

  // allocate output data for the result
  TElement* h_odata = (TElement*)malloc(numBlocks * sizeof(TElement));

  GPUDataPointer odata = GPUDataManager::New();
  odata->SetBufferSize( numBlocks * sizeof(TElement) );
  odata->SetCPUBufferPointer( h_odata );
  odata->Allocate();
  odata->SetCPUDirtyFlag(true);

  double dTotalTime = 0.0;

  m_GPUResult = 0;
  m_GPUResult = this->GPUReduce(size, numThreads, numBlocks, maxThreads, maxBlocks,
                                  whichKernel, cpuFinalReduction,
                                  cpuFinalThreshold, &dTotalTime,
                                  m_GPUDataManager, odata);

  // cleanup
  free(h_odata);

  return m_GPUResult;
}

template< class TElement >
TElement
GPUReduction< TElement >
::GPUReduce(  cl_int  n,
                  int  numThreads,
                  int  numBlocks,
                  int  maxThreads,
                  int  maxBlocks,
                  int  whichKernel,
                  bool cpuFinalReduction,
                  int  cpuFinalThreshold,
                  double* dTotalTime,
                  GPUDataPointer idata,
                  GPUDataPointer odata)
{
  TElement gpu_result = 0;

  // arguments set up
  int argidx = 0;
  this->m_GPUKernelManager->SetKernelArgWithImage(m_ReduceGPUKernelHandle, argidx++, idata);
  this->m_GPUKernelManager->SetKernelArgWithImage(m_ReduceGPUKernelHandle, argidx++, odata);

  this->m_GPUKernelManager->SetKernelArg(m_ReduceGPUKernelHandle, argidx++, sizeof(cl_int), &n);
  //shared memory below
  this->m_GPUKernelManager->SetKernelArg(m_ReduceGPUKernelHandle, argidx++, sizeof(TElement) * numThreads, NULL);

  size_t globalSize[1];
  size_t localSize[1];

  gpu_result = 0;

  // execute the kernel
  globalSize[0] = numBlocks * numThreads;
  localSize[0] = numThreads;

  this->m_GPUKernelManager->LaunchKernel(m_ReduceGPUKernelHandle, 1, globalSize, localSize );

  odata->SetCPUDirtyFlag(true);
  TElement* h_odata = (TElement*)odata->GetCPUBufferPointer();

  for(int i=0; i<numBlocks; i++)
  {
      gpu_result += h_odata[i];
  }

  // Release the kernels
  // clReleaseKernel(reductionKernel);

  return gpu_result;
}

template< class TElement >
TElement
GPUReduction< TElement >
::CPUGenerateData(TElement *data, int size)
{
    TElement sum = data[0];
    TElement c = (TElement)0.0;
    for (int i = 1; i < size; i++)
    {
        //TElement y = data[i] - c;
        //TElement t = sum + y;
        //c = (t - sum) - y;
        sum = sum + data[i];
    }
    m_CPUResult = sum;
    return sum;
}

} // end namespace itk

#endif
