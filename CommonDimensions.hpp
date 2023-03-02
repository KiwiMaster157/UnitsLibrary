#pragma once

#include "Dimension.hpp"
#include "LinearUnit.hpp"
#include "Quantity.hpp"

namespace rgf
{
#define DEFINE_ALIASES(NAME)                                                                  \
   template<typename T> using NAME##_quantity_t = rgf::quantity<NAME##_dimension, T>;         \
   template<typename T> using linear_##NAME##_unit_t = rgf::linear_unit<NAME##_dimension, T>; \
   using NAME##_quantity = NAME##_quantity_t<double>;                                         \
   using linear_##NAME##_unit = rgf::linear_##NAME##_unit_t<double>

#define DEFINE_PRODUCT(NAME, FIRST, SECOND) \
   using NAME##_dimension = rgf::dimension_product_t<FIRST##_dimension, SECOND##_dimension>; \
   DEFINE_ALIASES(NAME)
#define DEFINE_QUOTIENT(NAME, FIRST, SECOND) \
   using NAME##_dimension = rgf::dimension_quotient_t<FIRST##_dimension, SECOND##_dimension>; \
   DEFINE_ALIASES(NAME)

   template<int> struct length;
   template<int> struct time;
   template<int> struct mass;
   template<int> struct angle;
   template<int> struct data;
   template<int> struct charge;
   template<int> struct temperature;

   using standard_bases = rgf::base_types_t<length, time, mass, angle, data, charge, temperature>;

   using scalar_dimension = rgf::scalar_dimension_type_t<standard_bases>;
   using length_dimension = rgf::unit_dimension_type_t<standard_bases, length>;
   using time_dimension = rgf::unit_dimension_type_t<standard_bases, time>;
   using mass_dimension = rgf::unit_dimension_type_t<standard_bases, mass>;
   using angle_dimension = rgf::unit_dimension_type_t<standard_bases, angle>;
   using data_dimension = rgf::unit_dimension_type_t<standard_bases, data>;
   using charge_dimension = rgf::unit_dimension_type_t<standard_bases, charge>;
   using temperature_dimension = rgf::unit_dimension_type_t<standard_bases, temperature>;

   DEFINE_ALIASES(scalar);
   DEFINE_ALIASES(length);
   DEFINE_ALIASES(time);
   DEFINE_ALIASES(mass);
   DEFINE_ALIASES(angle);
   DEFINE_ALIASES(data);
   DEFINE_ALIASES(charge);
   DEFINE_ALIASES(temperature);

   DEFINE_PRODUCT(area, length, length);
   DEFINE_PRODUCT(volume, area, length);

   DEFINE_QUOTIENT(frequency, scalar, time);

   DEFINE_QUOTIENT(velocity, length, time);
   DEFINE_QUOTIENT(acceleration, velocity, time);
   DEFINE_QUOTIENT(jerk, acceleration, time);

   DEFINE_PRODUCT(momentum, mass, velocity);
   DEFINE_PRODUCT(force, mass, acceleration);
   DEFINE_PRODUCT(energy, force, length);
   DEFINE_QUOTIENT(power, energy, time);

   DEFINE_QUOTIENT(density, mass, volume);
   DEFINE_QUOTIENT(pressure, force, area);

   DEFINE_QUOTIENT(current, charge, time);

   // ...
}