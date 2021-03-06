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
#ifndef __itkQuadEdgeMeshEulerOperatorJoinFacetFunction_h
#define __itkQuadEdgeMeshEulerOperatorJoinFacetFunction_h

#include "itkQuadEdgeMeshFunctionBase.h"

namespace itk
{
/**
 * \class EulerOperatorJoinFacetFunction
 * \ingroup QEMeshModifierFunctions
 *
 * \brief Join the two facets which are on both sides of a given internal edge.
 * \ingroup ITK-QuadEdgeMesh
 */
template< class TMesh, class TQEType >
class ITK_EXPORT QuadEdgeMeshEulerOperatorJoinFacetFunction:
  public QuadEdgeMeshFunctionBase< TMesh, TQEType * >
{
public:
  /** Standard class typedefs. */
  typedef QuadEdgeMeshEulerOperatorJoinFacetFunction   Self;
  typedef QuadEdgeMeshFunctionBase< TMesh, TQEType * > Superclass;
  typedef SmartPointer< Self >                         Pointer;
  typedef SmartPointer< const Self >                   ConstPointer;

  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(QuadEdgeMeshEulerOperatorJoinFacetFunction, QuadEdgeMeshFunctionBase);

  /** Type of QuadEdge with which to apply slicing. */
  typedef TQEType QEType;

  typedef typename Superclass::MeshType   MeshType;
  typedef typename Superclass::OutputType OutputType;

  /** Evaluate at the specified input position */
  virtual OutputType Evaluate(QEType *e);

protected:
  QuadEdgeMeshEulerOperatorJoinFacetFunction(){}
  ~QuadEdgeMeshEulerOperatorJoinFacetFunction(){}
private:
  QuadEdgeMeshEulerOperatorJoinFacetFunction(const Self &); //purposely not
                                                            // implemented
  void operator=(const Self &);                             //purposely not
                                                            // implemented
};
} // namespace itk

#include "itkQuadEdgeMeshEulerOperatorJoinFacetFunction.txx"

#endif

// eof - itkQuadEdgeMeshEulerOperatorJoinFacetFunction.h
