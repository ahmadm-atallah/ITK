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
#ifndef __itkGenericUtilities_h
#define __itkGenericUtilities_h

/*
 *  This code was contributed in the Insight Journal paper:
 *  "A Streaming IO Base Class and Support for Streaming the MRC and VTK File Format"
 *  by Lowekamp B., Chen D.
 *  http://www.insight-journal.org/browse/publication/729
 *  http://hdl.handle.net/10380/3171
 *
 */

namespace itk
{
///////////////////////////////////////////////////

///////////////////////////////////////////////////
// Generic Programming Algorithms
///////////////////////////////////////////////////

/// \brief returns the min and max of a sequence defined by two iterators
///
/// Uses the < operator to determin ordering
/// If first == last then return is pair(first,first);
/// otherwise is it pair(min, max)
template< class TInputIter >
std::pair< TInputIter, TInputIter > min_max_element(TInputIter first, TInputIter last)
{
  std::pair< TInputIter, TInputIter > result(first, first);

  if ( first == last )
    {
    return result;
    }

  while ( ++first != last )
    {
    TInputIter prev = first;
    if ( ++first == last )
      {
      if ( *prev < *( result.first ) )
        {
        result.first = prev;
        }
      if ( *( result.second ) < *prev )
        {
        result.second = prev;
        }
      break;
      }
    else if ( *first < *prev )
      {
      if ( *first < *( result.first ) )
        {
        result.first = first;
        }
      if ( *( result.second ) < *prev )
        {
        result.second = prev;
        }
      }
    else
      {
      if ( *prev < *( result.first ) )
        {
        result.first = prev;
        }
      if ( *( result.second ) < *first )
        {
        result.second = first;
        }
      }
    }
  return result;
}

/// \brief returns the min and max of a
///
/// Uses the provided binary functor
/// If first == last then return is pair(first,first);
/// otherwise is it pair(min, max)
template< class TInputIter, class TCompare >
std::pair< TInputIter, TInputIter > min_max_element(TInputIter first, TInputIter last, TCompare comp)
{
  std::pair< TInputIter, TInputIter > result(first, first);

  if ( first == last )
    {
    return result;
    }

  while ( ++first != last )
    {
    TInputIter prev = first;
    if ( ++first == last )
      {
      if ( comp( *prev, *( result.first ) ) )
        {
        result.first = prev;
        }
      if ( comp(*( result.second ), *prev) )
        {
        result.second = prev;
        }
      break;
      }
    else if ( comp(*first, *prev) )
      {
      if ( comp( *first, *( result.first ) ) )
        {
        result.first = first;
        }
      if ( comp(*( result.second ), *prev) )
        {
        result.second = prev;
        }
      }
    else
      {
      if ( comp( *prev, *( result.first ) ) )
        {
        result.first = prev;
        }
      if ( comp(*( result.second ), *first) )
        {
        result.second = first;
        }
      }
    }
  return result;
}
} // end itk namespace

#endif //__itkGenericUtilities_h
