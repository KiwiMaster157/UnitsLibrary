#pragma once

#include <concepts>
#include <type_traits>

namespace rgf
{
   //! Boolean constant indicating if a type is a valid dimension base.
   //! Dimension bases are class template specializations with a single integer parameter.
   template<typename>
   constexpr bool is_dimension_base_v = false;
   template<template<int> typename BASE_T, int N>
   constexpr bool is_dimension_base_v<BASE_T<N>> = true;

   //! Concept version if is_dimension_base_v<T>
   template<typename T>
   concept dimension_base = is_dimension_base_v<T>;

   //! All valid dimensions are a specialization of this type.
   template<dimension_base... BASES>
   struct dimension_t;

   //! base_types_t is useful for defining aliases of dimension_t specializations.
   template<template<int> typename... BASE_TYPES>
   struct base_types_t;

   //! Boolean constant indicating if a type is a specialization of dimension<...>.
   template<typename>
   constexpr bool is_dimension_v = false;
   template<dimension_base... BASES>
   constexpr bool is_dimension_v<dimension_t<BASES...>> = true;

   //! Concept version of is_dimension_v<T>
   template<typename T>
   concept dimension_type = is_dimension_v<T>;

   namespace detail
   {
      //! Boolean constant indicating whether (X * NUM / DEN) will give an exact result for every X in SERIES.
      template<typename SERIES, int NUM, int DEN>
      constexpr bool is_valid_power = false;
      template<int... EXPONENTS, int NUM, int DEN>
      constexpr bool is_valid_power<std::integer_sequence<int, EXPONENTS...>, NUM, DEN>
         = (DEN != 0) && (((EXPONENTS * NUM) % DEN == 0) && ...);

      //! The 'type' member alias of dimension_product contains the result when two compatible dimension types are multiplied.
      //! Dimensions are considered compatible if they have the same base types in the same order.
      //! Users should generally prefer using rgf::dimension_product_t.
      template<dimension_type LEFT_DIM, dimension_type RIGHT_DIM>
      struct dimension_product;
      template<template<int> typename... BASE_TYPES, int... LEFT_EXPONENTS, int... RIGHT_EXPONENTS>
      struct dimension_product<dimension_t<BASE_TYPES<LEFT_EXPONENTS>...>, dimension_t<BASE_TYPES<RIGHT_EXPONENTS>...>>
      {
         using type = dimension_t<BASE_TYPES<LEFT_EXPONENTS + RIGHT_EXPONENTS>...>;
      };

      //! The 'type' member alias of dimension_exponent contains the result when a dimension type is raised to a rational power.
      //! The alias is only provided if the resulting dimension would have all integral exponents if computed with infinite precision.
      //! Users should generally prefer using rgf::dimension_exponent_t.
      template<dimension_type DIM, int POW_NUM, int POW_DEN = 1>
      struct dimension_exponent;
      template<template<int> typename... BASE_TYPES, int... EXPONENTS, int POW_NUM, int POW_DEN>
         requires rgf::detail::is_valid_power<std::integer_sequence<int, EXPONENTS...>, POW_NUM, POW_DEN>
      struct dimension_exponent<dimension_t<BASE_TYPES<EXPONENTS>...>, POW_NUM, POW_DEN>
      {
         using type = dimension_t<BASE_TYPES<(EXPONENTS* POW_NUM) / POW_DEN>...>;
      };

      //! The 'type' member alias of scalar_dimension_type contains the dimension type corresponding to BASE_TYPES_T with all zero exponents.
      //! The alias is only provided if BASE_TYPES_T is a specialization of rgf::base_types_t.
      //! Users should generally prefer using rgf::scalar_dimension_type_t.
      template<typename BASE_TYPES_T>
      struct scalar_dimension_type;
      template<template<int> typename... BASE_TYPES>
      struct scalar_dimension_type<rgf::base_types_t<BASE_TYPES...>>
      {
         using type = dimension_t<BASE_TYPES<0>...>;
      };

      //! The 'type' member alias of unit_dimension_type contains the dimension type corresponding to BASE_TYPES_T with all zero exponents,
      //!    except the exponent for BASE_TYPE is set to one.
      //! The alias is only provided if BASE_TYPES_T is a specialization of rgf::base_type_t.
      //! Requires that BASE_TYPE is a template parameter of BASE_TYPES_T's template specialzation.
      //! Users should generally prefer using rgf::unit_dimension_type_t.
      template<typename BASE_TYPES_T, template<int> typename BASE_TYPE>
      struct unit_dimension_type;
      template<template<int> typename... BASE_TYPES, template<int> typename BASE_TYPE>
      struct unit_dimension_type<rgf::base_types_t<BASE_TYPES...>, BASE_TYPE>
      {
         static_assert((std::is_same_v<BASE_TYPES<0>, BASE_TYPE<0>> || ...), "Cannot instantiate unit dimension for invalid base.");
         using type = dimension_t<BASE_TYPES<std::is_same_v<BASE_TYPES<0>, BASE_TYPE<0>>>...>;
      };
   }

   //! Alias representing the multiplicative inverse of DIM.
   template<dimension_type DIM>
   using dimension_inverse_t = typename rgf::detail::dimension_exponent<DIM, -1>::type;

   //! Alias representing the product of LEFT_DIM and RIGHT_DIM.
   template<dimension_type LEFT_DIM, dimension_type RIGHT_DIM>
   using dimension_product_t = typename rgf::detail::dimension_product<LEFT_DIM, RIGHT_DIM>::type;

   //! Alias representing the quotient of LEFT_DIM and RIGHT_DIM.
   template<dimension_type LEFT_DIM, dimension_type RIGHT_DIM>
   using dimension_quotient_t = dimension_product_t<LEFT_DIM, dimension_inverse_t<RIGHT_DIM>>;

   //! Alias representing DIM raised to the power POW_NUM / POW_DEN.
   //! Only valid if the resulting dimension has all-integer exponents.
   template<dimension_type DIM, int POW_NUM, int POW_DEN = 1>
   using dimension_exponent_t = typename rgf::detail::dimension_exponent<DIM, POW_NUM, POW_DEN>::type;

   //! Concept indicating that a dimension type has all zero exponents.
   template<typename DIM>
   concept empty_dimension = std::same_as<DIM, dimension_exponent_t<DIM, 0>>;

   //! Alias representing a dimension with all zero exponents.
   template<typename BASE_TYPES_T>
   using scalar_dimension_type_t = typename rgf::detail::scalar_dimension_type<BASE_TYPES_T>::type;

   //! Alias representing a dimension with all zero exponents, except one exponent set to one.
   template<typename BASE_TYPES_T, template<int> typename BASE_TYPE>
   using unit_dimension_type_t = typename rgf::detail::unit_dimension_type<BASE_TYPES_T, BASE_TYPE>::type;
}
