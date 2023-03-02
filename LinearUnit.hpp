#pragma once

#include "Quantity.hpp"

#include <utility>

namespace rgf
{
   //! linear_unit is a unit that may be linearly multiplied and divided by other linear units.
   //! This encompasses most units common in everyday use, except for Fahrenheit, Celcius, and decibels.
   //! For these other units, specialized classes should be written that provides, at a minimum, from_standard_value(quantity_type).
   //! It is intended that most instances will be declared 'inline constexpr' or be intermediate results during code execution.
   template<rgf::dimension_type UNIT_DIMENSION, rgf::arithmetic UNIT_VALUE_TYPE = double>
   class linear_unit
   {
   public:
      using dimension = UNIT_DIMENSION;
      using value_type = UNIT_VALUE_TYPE;

      using quantity_type = quantity<dimension, value_type>;

      constexpr explicit linear_unit() noexcept = default;

      constexpr linear_unit(std::in_place_t, value_type aConversionFactor) noexcept
         : mConversionFactor(aConversionFactor)
      {}

      //! Returns the unit's conversion factor stored under the hood.
      constexpr value_type conversion_factor() const noexcept
      {
         return mConversionFactor;
      }

      //! The call operator converts a value to a quantity with that value.
      //! E.g. if kilometers is a unit, kilometers(5) would create a quantity with 5000 meters.
      constexpr quantity_type operator()(value_type aValue) const noexcept
      {
         return { std::in_place, to_standard_value(aValue) };
      }
      constexpr value_type to_standard_value(value_type aValue) const noexcept
      {
         return aValue * mConversionFactor;
      }
      //! Converts from standard units into *this's unit.
      constexpr value_type from_standard_value(value_type aValue) const noexcept
      {
         return aValue / mConversionFactor;
      }
      //! Converts a quantity from standard units into *this's unit.
      constexpr value_type get(quantity_type aQuantity) const noexcept
      {
         return from_standard_value(aQuantity.get_standard());
      }

      //! Creates a new linear_unit scaled up in size.
      //! E.g. inline constexpr auto kilometers = meters.scaled_up(1000);
      constexpr linear_unit scaled_up(value_type aFactor) const noexcept
      {
         return linear_unit(std::in_place, mConversionFactor * aFactor);
      }
      //! Creates a new linear_unit scaled down in size.
      //! E.g. inline constexpr auto millimeters = meters.scaled_down(1000);
      constexpr linear_unit scaled_down(value_type aFactor) const noexcept
      {
         return linear_unit(std::in_place, mConversionFactor / aFactor);
      }

      //! Multiplies two linear_units together.
      //! They must have the same value_type, but can have different dimensions.
      //! The result is a new linear_unit representing a compound unit, and 
      //!    can be used for converting values to/from the standard units.
      //! E.g. torque.get(newtons * meters);
      template<dimension_type RDIM>
      constexpr friend linear_unit<dimension_product_t<dimension, RDIM>, value_type>
         operator*(const linear_unit& aLeft, const linear_unit<RDIM, value_type>& aRight) noexcept
      {
         return { std::in_place, aLeft.conversion_factor() * aRight.conversion_factor() };
      }
      //! Divides two linear_units.
      //! They must have the same value_type, but can have different dimensions.
      //! The result is a new linear_unit representing a compound unit, and 
      //!    can be used for converting values to/from the standard units.
      //! E.g. speed.get(meters / seconds);
      template<dimension_type RDIM>
      constexpr friend linear_unit<dimension_quotient_t<dimension, RDIM>, value_type>
         operator/(const linear_unit& aLeft, const linear_unit<RDIM, value_type>& aRight) noexcept
      {
         return { std::in_place, aLeft.conversion_factor() / aRight.conversion_factor() };
      }

   private:
      value_type mConversionFactor = 1;
   };
}
