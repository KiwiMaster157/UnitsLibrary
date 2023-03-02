#pragma once

#include "Dimension.hpp"
#include "Quantity.hpp"

#include <utility>

namespace rgf
{
   //! absolute<DIMENSION, VALUE_TYPE> represents an absolute with the given dimension.
   //! Examples of absolutes include temperatures such as 32F = 0C.
   //! A quantity would represent a difference in temperature, where 32F = 17.78C,
   //! It may be added or subtracted with quantities that have the same dimension to yield an absolute.
   //! Subtracting two absolutes of the same dimension yields a quantity.
   //! It may not be multiplied or divided.
   template<dimension_type ABSOLUTE_DIMENSION, arithmetic ABSOLUTE_VALUE_TYPE = double>
   class absolute
   {
   public:
      using dimension = ABSOLUTE_DIMENSION;
      using value_type = ABSOLUTE_VALUE_TYPE;

      using quantity_type = quantity<dimension, value_type>;

      constexpr static bool is_scalar = rgf::empty_dimension<dimension>;

      //! When default-constructed, the standard value is value-initialized.
      //! For arithmetic types, this is equivalent to zero-initialization.
      constexpr absolute() = default;
      constexpr absolute(const absolute&) = default;

      //! Constructor for setting the standard value of non-scalar absolutes.
      //! std::in_place_t may be replaced with a custom type if it proves to cause problems.
      constexpr absolute(std::in_place_t, value_type aStandardValue) noexcept
         : mStandardValue(aStandardValue)
      {}

      //! Explicitly convertible from values that have the same dimension but different value_type.
      template<std::convertible_to<value_type> OTHER_VALUE_TYPE>
      constexpr explicit absolute(const absolute<dimension, OTHER_VALUE_TYPE>& aOther) noexcept
         : mStandardValue(aOther.get_standard())
      {}

      //! Scalar values may be assigned from value_type directly.
      constexpr absolute& operator=(const absolute&) = default;

      //! Accessor for the value in standard units.
      //! Generally not necessary. Use other APIs when possible.
      constexpr value_type get_standard() const noexcept
      {
         return mStandardValue;
      }
      //! Mutator for the value in standard units.
      //! Generally not necessary. Use other APIs when possible.
      constexpr void set_standard_value(value_type aValue) noexcept
      {
         mStandardValue = aValue;
      }

      template<unit_type<absolute> UNIT>
      constexpr auto get(const UNIT& aUnit) const
      {
         return aUnit.from_standard_value(*this);
      }

      //! In-place addition operators.
      //! Overload 1 is only available to scalar values.
      constexpr absolute& operator+=(value_type aValue) noexcept requires is_scalar
      {
         mStandardValue += aValue;
         return *this;
      }
      constexpr absolute& operator+=(const quantity_type& aOther) noexcept
      {
         mStandardValue += aOther.get_standard();
         return *this;
      }

      //! In-place subtraction operators.
      //! Overload 1 is only available to scalar values.
      constexpr absolute& operator-=(value_type aValue) noexcept requires is_scalar
      {
         mStandardValue -= aValue;
         return *this;
      }
      constexpr absolute& operator-=(const quantity_type& aOther) noexcept
      {
         mStandardValue -= aOther.get_standard();
         return *this;
      }

      //! Addition operators.
      //! Overloads 1 and 3 are available for all values.
      //! Overloads 2 and 4 are only available to scalar values.
      constexpr friend absolute operator+(const absolute& aLeft, const quantity_type& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() + aRight.get_standard() };
      }
      template<arithmetic T>
      constexpr friend absolute<dimension, sum_t<value_type, T>>
         operator+(const absolute& aLeft, T aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft.get_standard() + aRight };
      }
      template<arithmetic T>
      constexpr friend absolute<dimension, sum_t<T, value_type>>
         operator+(const quantity_type<dimension, T>& aLeft, const absolute& aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft + aRight.get_standard() };
      }
      template<arithmetic T>
      constexpr friend absolute<dimension, sum_t<T, value_type>>
         operator+(T aLeft, const absolute& aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft + aRight.get_standard() };
      }

      //! Subtraction operators.
      //! Overloads 1 and 2 are available for all values.
      //! Overload 3 is only available to scalar values.
      constexpr friend quantity_type operator-(const absolute& aLeft, const absolute& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() - aRight.get_standard() };
      }
      constexpr friend absolute operator-(const absolute& aLeft, const quantity_type& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() - aRight.get_standard() };
      }
      constexpr friend absolute operator-(const absolute& aLeft, value_type aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft.get_standard() - aRight };
      }

      //! Comparison operators.
      constexpr friend auto operator==(const absolute& aLeft, const absolute& aRight) noexcept = default;

   private:
      value_type mStandardValue = value_type();
   };

   //! Converts a quantity into an absolute with the same value.
   template<dimension_type DIMENSION, arithmetic VALUE_TYPE>
   absolute<DIMENSION, VALUE_TYPE> to_absolute(const quantity<DIMENSION, VALUE_TYPE>& aQuantity) noexcept
   {
      return { std::in_place, aQuantity.get_standard() };
   }

   //! Converts an absolute into a quantity with the same value.
   template<dimension_type DIMENSION, arithmetic VALUE_TYPE>
   quantity<DIMENSION, VALUE_TYPE> to_quantity(const absolute<DIMENSION, VALUE_TYPE>& aAbsolute) noexcept
   {
      return { std::in_place, aAbsolute.get_standard() };
   }
}
