#pragma once

#include "CommonDimensions.hpp"

#include <numbers>

namespace rgf
{
#define DEFINE_LARGE_SI_PREFIX(BASE_NAME) \
   inline constexpr auto deca##BASE_NAME  = BASE_NAME.scaled_up(10);       \
   inline constexpr auto hecto##BASE_NAME = BASE_NAME.scaled_up(100);      \
   inline constexpr auto kilo##BASE_NAME  = BASE_NAME.scaled_up(1000);     \
   inline constexpr auto mega##BASE_NAME  = BASE_NAME.scaled_up(1000'000); \
   inline constexpr auto giga##BASE_NAME  = BASE_NAME.scaled_up(1000'000'000)

#define DEFINE_SMALL_SI_PREFIX(BASE_NAME) \
   inline constexpr auto deci##BASE_NAME  = BASE_NAME.scaled_down(10);       \
   inline constexpr auto centi##BASE_NAME = BASE_NAME.scaled_down(100);      \
   inline constexpr auto milli##BASE_NAME = BASE_NAME.scaled_down(1000);     \
   inline constexpr auto micro##BASE_NAME = BASE_NAME.scaled_down(1000'000); \
   inline constexpr auto nano##BASE_NAME  = BASE_NAME.scaled_down(1000'000'000)

#define DEFINE_ALL_SI_PREFIX(BASE_NAME) DEFINE_LARGE_SI_PREFIX(BASE_NAME); DEFINE_SMALL_SI_PREFIX(BASE_NAME)

   inline constexpr linear_scalar_unit ul{};

   inline constexpr linear_length_unit meters{};
   DEFINE_ALL_SI_PREFIX(meters);
   inline constexpr auto inches = meters.scaled_down(39.3701);
   inline constexpr auto feet = inches.scaled_up(12);
   inline constexpr auto yards = inches.scaled_up(36);
   inline constexpr auto miles = feet.scaled_up(5280);

   inline constexpr linear_time_unit seconds{};
   DEFINE_SMALL_SI_PREFIX(seconds);
   inline constexpr auto minutes = seconds.scaled_up(60);
   inline constexpr auto hours = minutes.scaled_up(60);
   inline constexpr auto days = hours.scaled_up(24);
   inline constexpr auto weeks = days.scaled_up(7);
   inline constexpr auto years = days.scaled_up(365.25);
   inline constexpr auto months = years.scaled_down(12);

   inline constexpr linear_mass_unit grams{ std::in_place, 0.001 };
   DEFINE_ALL_SI_PREFIX(grams);

   inline constexpr linear_angle_unit radians{};
   inline constexpr auto degrees = radians.scaled_down(180 / std::numbers::pi);

   // ...
}