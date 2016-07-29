/*!
 * \file Curve.h
 *
 * \defgroup Editor
 *
 * \brief
 *
 * \details
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * \copyright This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#pragma once

#include <map>
#include <utility>
#include <memory>
#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/curve/curve_abstract.hpp>
#include <ossia/editor/curve/curve_segment.hpp>
#include <ossia_export.h>
#include <functional>
namespace ossia
{
class Destination;

template <typename X, typename Y>
class curve : public curve_abstract
{

public:

  typedef X abscissa_type;
  typedef Y ordinate_type;

#if 0
# pragma mark -
# pragma mark Life cycle
#endif

  /*! factory
   \return std::shared_ptr<#Curve> */
  static std::shared_ptr<curve> create();

  /*! destructor */
  virtual ~curve();

#if 0
# pragma mark -
# pragma mark Edition
#endif

  /*! add a segment to reach a target point to the curve
   \param std::shared_ptr<#CurveSegment<Y>> segment to target point
   \param X target point abscissa
   \param Y target point ordinate
   \return bool */
  virtual bool addPoint(curve_segment<Y>, X, Y) = 0;

  /*! remove a point from the curve
   \param X point abscissa
   \return bool */
  virtual bool removePoint(X) = 0;

#if 0
# pragma mark -
# pragma mark Execution
#endif

  /*! get value at an abscissa
   \param X abscissa.
   \return Y ordinate */
  virtual Y valueAt(X) const = 0;

#if 0
# pragma mark -
# pragma mark Accessors
#endif


  /*! get initial point abscissa
   \details if there is an initial abcissa destination, it will return the value of the address
   \return X value */
  virtual X getInitialPointAbscissa() const = 0;

  /*! get initial point ordinate
   \details if there is an initial ordinate destination, it will return the value of the address
   \return Y value */
  virtual Y getInitialPointOrdinate() const = 0;

  /*! set initial point abscissa
   \details if there is an initial abscissa destination, this accessor is useless
   \param X abscissa */
  virtual void setInitialPointAbscissa(X) = 0;

  /*! set initial point ordinate
   \details if there is an initial ordinate destination, this accessor is useless
   \param Y ordinate */
  virtual void setInitialPointOrdinate(Y) = 0;

  /*! get initial point abscissa destination
   \return const Destination* */
  virtual const Destination& getInitialPointAbscissaDestination() const = 0;

  /*! get initial point ordinate destination
   \return const Destination* */
  virtual const Destination& getInitialPointOrdinateDestination() const = 0;

  /*! set initial curve abscissa using a Destination
   \param const Destination* */
  virtual void setInitialPointAbscissaDestination(const Destination&) = 0;

  /*! set initial curve ordinate using a Destination
   \param const Destination* */
  virtual void setInitialPointOrdinateDestination(const Destination&) = 0;

  /*! get initial curve value
   \return std::map<X, pair<Y, CurveSegment<Y>>> map of {abscissa, {value, previous segment}

 */
  virtual std::map<X, std::pair<Y, curve_segment<Y>>> getPointsMap() const = 0;

};

}

#if !defined(APIJamoma_EXPORTS)
extern template class OSSIA_EXPORT ossia::curve<double, bool>;
extern template class OSSIA_EXPORT ossia::curve<double, int>;
extern template class OSSIA_EXPORT ossia::curve<double, float>;

extern template class OSSIA_EXPORT ossia::curve<bool, bool>;
extern template class OSSIA_EXPORT ossia::curve<bool, int>;
extern template class OSSIA_EXPORT ossia::curve<bool, float>;

extern template class OSSIA_EXPORT ossia::curve<int, bool>;
extern template class OSSIA_EXPORT ossia::curve<int, int>;
extern template class OSSIA_EXPORT ossia::curve<int, float>;

extern template class OSSIA_EXPORT ossia::curve<float, bool>;
extern template class OSSIA_EXPORT ossia::curve<float, int>;
extern template class OSSIA_EXPORT ossia::curve<float, float>;
#endif
