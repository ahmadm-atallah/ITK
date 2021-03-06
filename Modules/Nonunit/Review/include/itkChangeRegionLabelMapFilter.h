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
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef __itkChangeRegionLabelMapFilter_h
#define __itkChangeRegionLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"

namespace itk
{
/** \class ChangeRegionLabelMapFilter
 * \brief Change the region of a label map.
 *
 * Change the region of a label map. If the output can't contain some of the objects' lines
 * they are truncated or removed. All objects fully outside the output region are removed.
 *
 *
 * This code was contributed in the Insight Journal paper:
 * "Label object representation and manipulation with ITK"
 * by Lehmann G.
 * http://hdl.handle.net/1926/584
 * http://www.insight-journal.org/browse/publication/176
 *
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 * \ingroup ITK-Review
 */
template< class TInputImage >
class ITK_EXPORT ChangeRegionLabelMapFilter:public InPlaceLabelMapFilter< TInputImage >
{
public:
  /** Standard class typedefs. */
  typedef ChangeRegionLabelMapFilter           Self;
  typedef InPlaceLabelMapFilter< TInputImage > Superclass;
  typedef SmartPointer< Self >                 Pointer;
  typedef SmartPointer< const Self >           ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ChangeRegionLabelMapFilter, InPlaceImageFilter);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef typename InputImageType::PixelType      PixelType;
  typedef typename InputImageType::IndexType      IndexType;
  typedef typename InputImageType::IndexValueType IndexValueType;
  typedef typename InputImageType::SizeType       SizeType;
  typedef typename InputImageType::RegionType     RegionType;

  typedef TInputImage TOutputImage;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** The output region to use */
  itkSetMacro(Region, OutputImageRegionType);
  itkGetConstReferenceMacro(Region, OutputImageRegionType);
protected:
  ChangeRegionLabelMapFilter() {}
  ~ChangeRegionLabelMapFilter() {}

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  virtual void ThreadedProcessLabelObject(LabelObjectType *labelObject);

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion( DataObject *itkNotUsed(output) );

  virtual void GenerateOutputInformation();

  void GenerateData();

private:
  ChangeRegionLabelMapFilter(const Self &); //purposely not implemented
  void operator=(const Self &);             //purposely not implemented

  OutputImageRegionType m_Region;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkChangeRegionLabelMapFilter.txx"
#endif

#endif
