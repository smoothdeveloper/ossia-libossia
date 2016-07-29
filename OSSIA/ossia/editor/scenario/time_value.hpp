/*!
 * \file TimeValue.h
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
#include <cmath>
#include <ossia_export.h>
namespace ossia
{

class OSSIA_EXPORT time_value
{

public:

#if 0
# pragma mark -
# pragma mark Life cycle
#endif

  /*! constructor
   \param int value */
  time_value(double d = 0.): m_value{d} { }

  /*! destructor */
  ~time_value() = default;

#if 0
# pragma mark -
# pragma mark Operator
#endif

  /*! assignation operator */
  time_value & operator= (double d)
  {
    m_value = d;
    return *this;
  }

  time_value & operator= (time_value t)
  {
      m_value = t.m_value;
      return *this;
  }

  /*! self addition operator */
  time_value & operator+= (double d)
  {
    if (isInfinite())
      m_value = 0.;
    else
      m_value += d;

    return *this;
  }

  time_value & operator+= (time_value t)
  {
    if (isInfinite() || t.isInfinite())
      m_value = 0.;
    else
      m_value += t.m_value;

    return *this;
  }

  /*! self substraction operator */
  time_value & operator-= (double d)
  {
    if (isInfinite())
      m_value = 0.;
    else
      m_value -= d;

    return *this;
  }

  time_value & operator-= (time_value t)
  {
    if (isInfinite() || t.isInfinite())
      m_value = 0.;
    else
      m_value -= t.m_value;

    return *this;
  }


  /*! addition operator */
  time_value operator+ (double d) const
  {
    return time_value(m_value + d);
  }

  time_value operator+ (time_value t) const
  {
    if (isInfinite() || t.isInfinite())
    {
      return time_value(INFINITY);
    }

    return time_value(m_value + t.m_value);
  }

  /*! substraction operator */
  time_value operator- (double d) const
  {
    return time_value(m_value - d);
  }

  time_value operator- (time_value t) const
  {
    if (isInfinite() || t.isInfinite())
    {
      return time_value(INFINITY);
    }

    return time_value(m_value - t.m_value);
  }

  /*! double casting operator */
  operator double() const { return m_value; }

#if 0
# pragma mark -
# pragma mark Accessors
#endif

  /*! is the time value infinite ?
   \return bool infinite */
  bool isInfinite() const
  {
    return std::isinf(m_value);
  }


protected:

  double  m_value;
};

const time_value Infinite{INFINITY};
const time_value Zero{0.};
const time_value One{1.};

}
