#pragma once

#include "Dimension.hpp"

#include <utility>

namespace rgf
{
   template<typename T>
   concept arithmetic = std::is_arithmetic_v<T>;

   
   template<typename L, typename R>
   using sum_t = decltype(L() + R());
   template<typename L, typename R>
   using difference_t = decltype(L() - R());
   template<typename L, typename R>
   using product_t = decltype(L() * R());
   template<typename L, typename R>
   using quotient_t = decltype(L() / R());

   //! Helper concept for qualifying quantity.get().
   //! In the future, this may need to be modified to allow for more generic unit types.
   template<typename T, typename Q>
   concept unit_type = requires (const T t, const Q q)
   {
      requires std::same_as<typename T::dimension, typename Q::dimension>;
      { t.get(q) } -> std::semiregular;
   };

   //! quantity<DIMENSION, VALUE_TYPE> represents a quantity with the given dimension.
   //! It may be added or subtracted with other quantities that have the same dimension.
   //! It may be multiplied or divided with other quantities with compatible dimensions.
   //! If it represents a scalar quantity, additional operations are enabled and a static constant is_scalar is true.
   template<dimension_type QUANTITY_DIMENSION, arithmetic QUANTITY_VALUE_TYPE = double>
   class quantity
   {
   public:
      using dimension = QUANTITY_DIMENSION;
      using value_type = QUANTITY_VALUE_TYPE;

      using scalar_dimension = dimension_exponent_t<dimension, 0>;
      using scalar_type = quantity<scalar_dimension, value_type>;

      constexpr static bool is_scalar = rgf::empty_dimension<dimension>;

      //! When default-constructed, the standard value is value-initialized.
      //! For arithmetic types, this is equivalent to zero-initialization.
      constexpr quantity() = default;
      constexpr quantity(const quantity&) = default;

      //! Scalar values may be implicitly converted to and from the corresponding value type.
      constexpr quantity(value_type aValue) noexcept requires is_scalar
         : mStandardValue(aValue)
      {}

      //! Constructor for setting the standard quantity of non-scalar values.
      //! std::in_place_t may be replaced with a custom type if it proves to cause problems.
      constexpr quantity(std::in_place_t, value_type aStandardValue) noexcept
         : mStandardValue(aStandardValue)
      {}

      //! Explicitly convertible from values that have the same dimension but different value_type.
      template<std::convertible_to<value_type> OTHER_VALUE_TYPE>
      constexpr explicit quantity(const quantity<dimension, OTHER_VALUE_TYPE>& aOther) noexcept
         : mStandardValue(aOther.get_standard())
      {}

      //! Scalar values may be assigned from value_type directly.
      constexpr quantity& operator=(const quantity&) = default;
      constexpr quantity& operator=(value_type aValue) noexcept requires is_scalar
      {
         mStandardValue = aValue;
         return *this;
      }

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
      //! Scalar values are implicitly convertible to their underlying data type.
      constexpr operator value_type() const noexcept requires is_scalar
      {
         return mStandardValue;
      }

      template<unit_type<quantity> UNIT>
      constexpr auto get(const UNIT& aUnit) const
      {
         return aUnit.get(*this);
      }

      //! In-place addition operators.
      //! Overload 1 is only available to scalar values.
      constexpr quantity& operator+=(value_type aValue) noexcept requires is_scalar
      {
         mStandardValue += aValue;
         return *this;
      }
      constexpr quantity& operator+=(const quantity& aOther) noexcept
      {
         mStandardValue += aOther.get_standard();
         return *this;
      }

      //! In-place subtraction operators.
      //! Overload 1 is only available to scalar values.
      constexpr quantity& operator-=(value_type aValue) noexcept requires is_scalar
      {
         mStandardValue -= aValue;
         return *this;
      }
      constexpr quantity& operator-=(const quantity& aOther) noexcept
      {
         mStandardValue -= aOther.get_standard();
         return *this;
      }

      //! In-place multiplication operators.
      //! Only scalar values allowed on the right side of the expression.
      constexpr quantity& operator*=(value_type aValue) noexcept
      {
         mStandardValue *= aValue;
         return *this;
      }
      constexpr quantity& operator*=(const scalar_type& aOther) noexcept
      {
         mStandardValue *= aOther.get_standard();
         return *this;
      }

      //! In-place division operators.
      //! Only scalar values allowed on the right side of the expression.
      constexpr quantity& operator/=(value_type aValue) noexcept
      {
         mStandardValue /= aValue;
         return *this;
      }
      constexpr quantity& operator/=(const scalar_type& aOther) noexcept
      {
         mStandardValue /= aOther.get_standard();
         return *this;
      }

      //! Unary minus operator.
      constexpr quantity operator-() const noexcept
      {
         return { std::in_place, -mStandardValue };
      }

      //! Addition operators.
      //! Overload 1 is available for all values.
      //! Overloads 2 and 3 are only available to scalar values.
      template<arithmetic T>
      constexpr friend quantity<dimension, sum_t<value_type, T>>
         operator+(const quantity& aLeft, const quantity<dimension, T>& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() + aRight.get_standard() };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, sum_t<value_type, T>>
         operator+(const quantity& aLeft, T aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft.get_standard() + aRight };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, sum_t<T, value_type>>
         operator+(T aLeft, const quantity& aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft + aRight.get_standard() };
      }

      //! Subtraction operators.
      //! Overload 1 is available for all values.
      //! Overloads 2 and 3 are only available to scalar values.
      template<arithmetic T>
      constexpr friend quantity<dimension, difference_t<value_type, T>>
         operator-(const quantity& aLeft, const quantity<dimension, T>& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() - aRight.get_standard() };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, difference_t<value_type, T>>
         operator-(const quantity& aLeft, T aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft.get_standard() - aRight };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, difference_t<T, value_type>>
         operator-(T aLeft, const quantity& aRight) noexcept requires is_scalar
      {
         return { std::in_place, aLeft - aRight.get_standard() };
      }

      //! Multiplication operators.
      template<dimension_type RDIM, arithmetic T>
      constexpr friend quantity<dimension_product_t<dimension, RDIM>, product_t<value_type, T>>
         operator*(const quantity& aLeft, const quantity<RDIM, T>& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() * aRight.get_standard() };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, product_t<value_type, T>>
         operator*(const quantity& aLeft, T aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() * aRight };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, product_t<T, value_type>>
         operator*(T aLeft, const quantity& aRight) noexcept
      {
         return { std::in_place, aLeft * aRight.get_standard() };
      }

      //! Division operators.
      template<dimension_type RDIM, arithmetic T>
      constexpr friend quantity<dimension_quotient_t<dimension, RDIM>, quotient_t<value_type, T>>
         operator/(const quantity& aLeft, const quantity<RDIM, T>& aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() / aRight.get_standard() };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension, quotient_t<value_type, T>>
         operator/(const quantity& aLeft, T aRight) noexcept
      {
         return { std::in_place, aLeft.get_standard() / aRight };
      }
      template<arithmetic T>
      constexpr friend quantity<dimension_inverse_t<dimension>, quotient_t<T, value_type>>
         operator/(T aLeft, const quantity& aRight) noexcept
      {
         return { std::in_place, aLeft / aRight.get_standard() };
      }

      //! Comparison operators.
      template<arithmetic T>
      constexpr friend auto operator<=>(const quantity& aLeft, const quantity<dimension, T>& aRight) noexcept
      {
         return aLeft.get_standard() <=> aRight.get_standard();
      }
      template<arithmetic T>
      constexpr friend auto operator<=>(const quantity& aLeft, T aRight) noexcept requires is_scalar
      {
         return aLeft.get_standard() <=> aRight;
      }
      template<arithmetic T>
      constexpr friend auto operator<=>(T aLeft, const quantity& aRight) noexcept requires is_scalar
      {
         return aLeft <=> aRight.get_standard();
      }

   private:
      value_type mStandardValue = value_type();
   };
}
