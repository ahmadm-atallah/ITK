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

/**
 * Test program for itkGPUMeanImageFilter class
 *
 * This program creates a GPU Mean filter test pipelining.
 */

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMeanImageFilter.h"

#include "itkGPUImage.h"
#include "itkGPUKernelManager.h"
#include "itkGPUContextManager.h"
#include "itkGPUImageToImageFilter.h"
#include "itkGPUMeanImageFilter.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkTimeProbe.h"

/**
 * Testing GPU Mean Image Filter
 */
int itkGPUMeanImageFilterTest(int argc, char *argv[])
{
  // register object factory for GPU image and filter
  itk::ObjectFactoryBase::RegisterFactory( itk::GPUImageFactory::New() );
  itk::ObjectFactoryBase::RegisterFactory( itk::GPUMeanImageFilterFactory::New() );

  typedef   unsigned char  InputPixelType;
  typedef   unsigned char  OutputPixelType;

  typedef itk::Image< InputPixelType,  2 >   InputImageType;
  typedef itk::Image< OutputPixelType, 2 >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  if( argc <  3 )
    {
    std::cerr << "Error: missing arguments" << std::endl;
    std::cerr << "inputfile outputfile " << std::endl;
    return EXIT_FAILURE;
    }

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );

  //
  // Note: We use regular itk filter type here but factory will automatically create
  //       GPU filter for Median filter and CPU filter for threshold filter.
  //
  typedef itk::MeanImageFilter< InputImageType, OutputImageType > MeanFilterType;

  MeanFilterType::Pointer filter = MeanFilterType::New();

  // Mean filter kernel radius
  InputImageType::SizeType indexRadius;
  indexRadius[0] = 2; // radius along x
  indexRadius[1] = 2; // radius along y

  // Build pipeline
  filter->SetRadius( indexRadius );
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() ); // copy GPU->CPU implicilty

  // execute pipeline filter and write output
  writer->Update();

  return EXIT_SUCCESS;
}
