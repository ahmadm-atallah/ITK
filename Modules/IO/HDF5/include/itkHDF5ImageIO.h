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
 *         The specification for this file format is taken from the
 *         web site http://analyzedirect.com/support/10.0Documents/Analyze_Resource_01.pdf
 * \author Hans J. Johnson
 *         The University of Iowa 2002
 */

#ifndef __itkHDF5ImageIO_h
#define __itkHDF5ImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

// itk namespace first suppresses
// kwstyle error for the H5 namespace below
namespace itk
{
}
namespace H5
{
class H5File;
class DataSpace;
class DataSet;
}

#include "itkStreamingImageIOBase.h"

namespace itk
{
class MetaDataObjectBase;
class MetaDataDictionary;
/** \class HDF5ImageIO
 *
 * \author Kent Williams
 * \brief Class that defines how to read HDF5 file format.
 * HDF5 IMAGE FILE FORMAT - As much information as I can determine from sourceforge.net/projects/HDF5lib
 *
 * \ingroup ITK-IO-HDF5
 *
 * HDF5 paths for elements in file
 * N is dimension of image
 * /ITKVersion                 ITK Library Version string
 * /HDFVersion                 HDF Version String
 * /ITKImage                   Root Image Group
 * /ITKImage/<name>            name is arbitrary, but to parallel Transform I/O
 *                             keep an image in a subgroup. The idea is to
 *                             parallel transform file structure.
 * /ITKIMage/<name>/Origin     N-D point double
 * /ITKImage/<name>/Directions N N-vectors double
 * /ITKImage/<name>/Spacing    N-vector double
 * /ITKImage/<name>/Dimensions N-vector ::itk::SizeValueType
 * /ITKImage/<name>/VoxelType  String representing voxel type.
 *                             This can be inferred from the VoxelData
 *                             type info, but it makes the file more
 *                             user friendly with respect to HDF5 viewers.
 * /ITKImage/<name>/VoxelData  multi-dim array of voxel data
 *                             in the case of non-scalar voxel type,
 *                             keep voxel components together, to make
 *                             loading possible without
 * /ITKImage/<name>/MetaData   Group for storing metadata from
 *                             MetaDataDictionary
 * /ITKImage/<name>/MetaData/<item-name>
 *                             Dataset containing data for item-name
 *                             in the MetaDataDictionary
 * re-arrangement.
 *
 *
 */

class ITK_EXPORT HDF5ImageIO:public StreamingImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef HDF5ImageIO          Self;
  typedef ImageIOBase          Superclass;
  typedef SmartPointer< Self > Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HDF5ImageIO, Superclass);

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine if the file can be read with this ImageIO implementation.
   * \author Hans J Johnson
   * \param FileNameToRead The name of the file to test for reading.
   * \post Sets classes ImageIOBase::m_FileName variable to be FileNameToWrite
   * \return Returns true if this ImageIO can read the file specified.
   */
  virtual bool CanReadFile(const char *FileNameToRead);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void *buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine if the file can be written with this ImageIO implementation.
   * \param FileNameToWrite The name of the file to test for writing.
   * \author Hans J. Johnson
   * \post Sets classes ImageIOBase::m_FileName variable to be FileNameToWrite
   * \return Returns true if this ImageIO can write the file specified.
   */
  virtual bool CanWriteFile(const char *FileNameToWrite);

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void *buffer);

protected:
  HDF5ImageIO();
  ~HDF5ImageIO();

  virtual SizeType GetHeaderSize(void) const;

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  HDF5ImageIO(const Self &);   //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  void WriteString(const std::string &path,
                   const std::string &value);
  void WriteString(const std::string &path,
                   const char *s);
  std::string ReadString(const std::string &path);

  void WriteScalar(const std::string &path,
                   const bool &value);
  void WriteScalar(const std::string &path,
                   const long &value);
  void WriteScalar(const std::string &path,
                   const unsigned long &value);

  template <typename TScalar>
  void WriteScalar(const std::string &path,
                   const TScalar &value);

  template <typename TScalar>
  TScalar ReadScalar(const std::string &DataSetName);

  template <typename TScalar>
  void WriteVector(const std::string &path,
                   const std::vector<TScalar> &vec);

  template <typename TScalar>
  std::vector<TScalar> ReadVector(const std::string &DataSetName);

  void WriteDirections(const std::string &path,
                       const std::vector<std::vector<double> > &dir);

  std::vector<std::vector<double> > ReadDirections(const std::string &path);

  template <typename TType>
    bool WriteMeta(const std::string &name,
                   MetaDataObjectBase *metaObj);
  template <typename TType>
    bool WriteMetaArray(const std::string &name,
                   MetaDataObjectBase *metaObj);
  template <typename TType>
    void StoreMetaData(MetaDataDictionary *metaDict,
                       const std::string &HDFPath,
                       const std::string &name,
                       unsigned long numElements);
  void SetupStreaming(H5::DataSpace *imageSpace,
                      H5::DataSpace *slabSpace);
  H5::H5File  *m_H5File;
  H5::DataSet *m_VoxelDataSet;
  bool         m_ImageInformationWritten;
};
} // end namespace itk

#endif // __itkHDF5ImageIO_h
