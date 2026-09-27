#include "../include/units.hpp"
#include <iostream>

using namespace measurements::constants;
using namespace units::literals;

int main(int argc, char *argv[]) {
    constexpr auto mass_earth = 5.9722e24_kg;
    constexpr auto mass_sun = 1.9891e30_kg;
    constexpr auto earth_sun_distance = 150e9_m;

    // pow is not constexpr, therefore ^2 cannot be evaluated
    // constexpr auto force = G * mass_earth * mass_sun / (earth_sun_distance ^
    // 2);

    constexpr auto force =
        G * mass_earth * mass_sun / (earth_sun_distance * earth_sun_distance);

    std::cout << force << std::endl;
    return 0;
}
