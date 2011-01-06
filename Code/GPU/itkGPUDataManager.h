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

//
// Base class for GPU Data Management
//
// This class will take care of synchronization between CPU and GPU memory
//

#ifndef ___ITKGPUDATAMANAGER_H__
#define ___ITKGPUDATAMANAGER_H__

#include <itkObject.h>
#include <itkLightObject.h>
#include <itkObjectFactory.h>
#include <CL/opencl.h>
#include "itkOclUtil.h"
#include "itkGPUContextManager.h"

namespace itk
{
class ITK_EXPORT GPUDataManager : public LightObject
{
// allow GPUKernelManager to access GPU buffer pointer
friend class GPUKernelManager;

public:

typedef GPUDataManager            Self;
typedef LightObject               Superclass;
typedef SmartPointer<Self>        Pointer;
typedef SmartPointer<const Self>  ConstPointer;

itkNewMacro(Self);
itkTypeMacro(GPUDataManager, LightObject);

// total buffer size in bytes
void SetBufferSize( unsigned int num );

void SetBufferFlag( cl_mem_flags flags );

void SetCPUBufferPointer( void* ptr );

void SetCPUDirtyFlag( bool isDirty );

void SetGPUDirtyFlag( bool isDirty );

void SetCPUBufferDirty();

void SetGPUBufferDirty();

void MakeCPUBufferUpToDate();

void MakeGPUBufferUpToDate();

void Allocate();

void SetCurrentCommandQueue( int queueid );

int  GetCurrentCommandQueueID();

//
// Synchronize CPU and GPU buffers (using dirty flags)
//
bool MakeUpToDate();

protected:

GPUDataManager();
virtual ~GPUDataManager();

//
// Get GPU buffer pointer
//
cl_mem* GetGPUBufferPointer();

private:

GPUDataManager(const Self&); //purposely not implemented
void operator=(const Self&);

unsigned int m_bufferSize; // # of bytes

GPUContextManager *m_manager;

int m_command_queue_id;

// buffer type
cl_mem_flags m_mem_flags;

// buffer pointers
cl_mem m_gpu_buffer;
void*  m_cpu_buffer;

// tells if buffer needs to be updated
bool m_isGPUBufferDirty, m_isCPUBufferDirty;

cl_platform_id* m_platform;
cl_context*     m_context;
};
}

#endif
