/*!
 * \file Automation.h
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#pragma once

#include <list>

#include "Editor/TimeProcess.h"
#include "Misc/Container.h"

namespace OSSIA {

class Address;
class AddressValue;
template <typename T> class Curve;
class TimeValue;

template <typename T>
class Automation : public virtual TimeProcess, public virtual Container<Address> {

    public:

      // Factories, destructor
      static std::shared_ptr<Automation> create();
      virtual std::shared_ptr<Automation> clone() const = 0;
      virtual ~Automation() = default;

      // Lecture
      virtual void play(bool log = false, std::string name = "") const override = 0;

      // Accessors
      virtual AddressValue getStartValue() const = 0; //TODO doublon avec Curve ?
      virtual void setStartValue(AddressValue) = 0;
      virtual AddressValue getEndValue() const = 0;
      virtual void setEndValue(AddressValue) = 0;
      virtual const std::shared_ptr<Curve<T>> & getCurve() const = 0;
      virtual void setCurve(std::shared_ptr<Curve<T>>) = 0;
      virtual const std::shared_ptr<Address> & getInputAdress() const = 0;
      virtual void setInputAddress(std::shared_ptr<Address>) = 0;

      // Std container
      // Output Addresses
      //TODO maybe possess a Container<Address> better than inherit ? for all ?

};

}
