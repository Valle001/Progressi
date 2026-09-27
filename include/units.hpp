#pragma once
#include <cmath>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

// TODO:
// - from/to string (string_view)
// - .convert(), .to() or .as() methods
// - multiplicative prefix handling

namespace units {
class Unit;
}

namespace measurements {
class Measurement;
}

namespace units {

    const std::string unitsBasicsPrefixes[4] = {"c","d","da","h"};
    const int nbOfExtendedPrefixes = 14;
    const std::string unitsExtendedPrefixes[nbOfExtendedPrefixes] = {"z","a","f","p","n","u","m","k","M","G","T","P","E","Z"};
    const std::string basicsUnits[7] = {"s","m","g","A","K","mol","cd"};
    const int nbOfDerivedUnits = 5;
    const std::string derivedUnits[nbOfDerivedUnits] = {"Hz","N","Pa","J","W"};//.....au cas où y'a un moyen plus intelligent je note pas tout
class Unit {
  private:
    std::int8_t s;
    std::int8_t m;
    std::int8_t kg;
    std::int8_t A;
    std::int8_t K;
    std::int8_t mol;
    std::int8_t cd;

  public:
    constexpr Unit();
    constexpr Unit(const std::int8_t s, const std::int8_t m,
                   const std::int8_t kg, const std::int8_t A,
                   const std::int8_t K, const std::int8_t mol,
                   const std::int8_t cd);
    constexpr Unit(const std::string_view &s, int &pfx);
    constexpr Unit(Unit &&) = default;
    constexpr Unit(const Unit &) = default;
    constexpr Unit &operator=(Unit &&) = default;
    constexpr Unit &operator=(const Unit &) = default;
    constexpr ~Unit();

    friend constexpr bool operator==(const Unit &lhs, const Unit &rhs) {
        return lhs.s == rhs.s && lhs.m == rhs.m && lhs.kg == rhs.kg &&
               lhs.A == rhs.A && lhs.K == rhs.K && lhs.mol == rhs.mol &&
               lhs.cd == rhs.cd;
    }

    friend constexpr bool operator!=(const Unit &lhs, const Unit &rhs) {
        return !(lhs == rhs);
    }

    friend constexpr Unit operator*(const Unit &lhs, const Unit &rhs) {
        return Unit(lhs.s + rhs.s, lhs.m + rhs.m, lhs.kg + rhs.kg,
                    lhs.A + rhs.A, lhs.K + rhs.K, lhs.mol + rhs.mol,
                    lhs.cd + rhs.cd);
    }

    friend constexpr Unit operator/(const Unit &lhs, const Unit &rhs) {
        return Unit(lhs.s - rhs.s, lhs.m - rhs.m, lhs.kg - rhs.kg,
                    lhs.A - rhs.A, lhs.K - rhs.K, lhs.mol - rhs.mol,
                    lhs.cd - rhs.cd);
    }

    friend constexpr Unit operator^(const Unit &unit, const std::int8_t power) {
        return Unit(unit.s * power, unit.m * power, unit.kg * power,
                    unit.A * power, unit.K * power, unit.mol * power,
                    unit.cd * power);
    }

    friend constexpr std::ostream &operator<<(std::ostream &os,
                                              const Unit &unit) {
        if (unit.s != 0) {
            os << "_s";
            if (unit.s != 1)
                os << static_cast<int>(unit.s);
        }
        if (unit.m != 0) {
            os << "_m";
            if (unit.m != 1)
                os << static_cast<int>(unit.m);
        }
        if (unit.kg != 0) {
            os << "_kg";
            if (unit.kg != 1)
                os << static_cast<int>(unit.kg);
        }
        if (unit.A != 0) {
            os << "_A";
            if (unit.A != 1)
                os << static_cast<int>(unit.A);
        }
        if (unit.K != 0) {
            os << "_K";
            if (unit.K != 1)
                os << static_cast<int>(unit.K);
        }
        if (unit.mol != 0) {
            os << "_mol";
            if (unit.mol != 1)
                os << static_cast<int>(unit.mol);
        }
        if (unit.cd != 0) {
            os << "_cd";
            if (unit.cd != 1)
                os << static_cast<int>(unit.cd);
        }
        return os;
    }
};

constexpr Unit::Unit() {}

constexpr Unit::Unit(const std::int8_t _s, const std::int8_t _m,
                     const std::int8_t _kg, const std::int8_t _A,
                     const std::int8_t _K, const std::int8_t _mol,
                     const std::int8_t _cd)
    : s(_s), m(_m), kg(_kg), A(_A), K(_K), mol(_mol), cd(_cd) {}



constexpr Unit::~Unit() {}
namespace base {

inline constexpr auto noUnit = Unit(0, 0, 0, 0, 0, 0, 0);
inline constexpr auto s = Unit(1, 0, 0, 0, 0, 0, 0);
inline constexpr auto m = Unit(0, 1, 0, 0, 0, 0, 0);
inline constexpr auto kg = Unit(0, 0, 1, 0, 0, 0, 0);
inline constexpr auto A = Unit(0, 0, 0, 1, 0, 0, 0);
inline constexpr auto K = Unit(0, 0, 0, 0, 1, 0, 0);
inline constexpr auto mol = Unit(0, 0, 0, 0, 0, 1, 0);
inline constexpr auto cd = Unit(0, 0, 0, 0, 0, 0, 1);

} // namespace base

namespace derived {
using namespace base;

inline constexpr auto Hz = s ^ -1;
inline constexpr auto N = kg * m * (s ^ -2);
inline constexpr auto Pa = N * (m ^ -2);
inline constexpr auto J = N * m;
inline constexpr auto W = J / s;
inline constexpr auto C = s * A;
inline constexpr auto V = W / A;
inline constexpr auto Ohm = V / A;
inline constexpr auto S = A / V;
inline constexpr auto F = C / V;
inline constexpr auto Wb = V * s;
inline constexpr auto H = Wb / A;
inline constexpr auto T = Wb * (m ^ -2);

} // namespace derived

constexpr Unit GetUnit(std::string_view inp)
{
    if (inp == "s") { return base::s;}
    else if (inp == "m") { return base::m;}
    else if (inp == "g") { 
      // Mulplication by 1e-3 already done line 156
      return base::kg;
    }
    else if (inp == "A") { return base::A;}
    else if (inp == "K") { return base::K;}
    else if (inp == "mol") { return base::mol;}
    else if (inp == "cd") { return base::cd;}
    else {
        // TODO: handle other units
    }
}

constexpr void ParsePrefixes(const std::string_view &inp, int &pfx)
{
        if (inp == "")
        {
            return;
        }

        switch (inp[0])
        {
        case 'c':
            pfx -= 2;
            break;
        case 'd':
            if (!(inp[1] == *"a")) {
            pfx -= 1;
        }
        else {
            pfx += 1;
        }
            break;
        case 'h':
            pfx += 2;
            break;
        
        default:
            for(std::size_t i = 0; i < nbOfExtendedPrefixes; i++){
                if(unitsExtendedPrefixes[i] == inp.substr(0,1))
                {
                    pfx *= 3*(i-nbOfExtendedPrefixes/2);
                    break;
                }
            }   
            break;
        }
    
}

constexpr Unit::Unit(const std::string_view &str, int &pfx) {
    
    Unit finalUnit = base::noUnit;
    std::size_t unit_start = 0;
    std::size_t unit_end = 0;
    while (str.find("_", unit_start + 1) != std::string::npos)
    {
        unit_start = str.find("_", unit_start + 1);
        if (str.find("_", unit_start + 1) == std::string::npos)
        {
            unit_end = str.length();
        }
        else {  
            unit_end = str.find("_", unit_start + 1);
        }
        finalUnit = finalUnit; // ???

        auto curr = str.substr(unit_start + 1, unit_end - unit_start - 1);
        for (std::size_t i; i < 7; i++)
        {   
            std::size_t n = basicsUnits[i].length();
            if (curr.substr(curr.length() - n) == std::string_view(basicsUnits[i]).substr(basicsUnits[i].length() - n))
            {
                ParsePrefixes( str.substr(0, curr.length() - n), pfx);

                if (basicsUnits[i] == "g") {
                    pfx -= 3;
                }
                finalUnit = finalUnit * GetUnit(basicsUnits[i]);
                break;
            }
        }

        

    }
    *this = finalUnit; // Je crois qu'on a le droit ?
}
} // namespace units

namespace measurements {

class Measurement {
  private:
    double value;
    units::Unit unit;

  public:
    constexpr Measurement();
    constexpr Measurement(const std::string_view &s);
    constexpr Measurement(const double scalar, const units::Unit &unit);
    constexpr Measurement(Measurement &&) = default;
    constexpr Measurement(const Measurement &) = default;
    constexpr Measurement &operator=(Measurement &&) = default;
    constexpr Measurement &operator=(const Measurement &) = default;
    constexpr ~Measurement();

    friend constexpr bool operator==(const Measurement &lhs,
                                     const Measurement &rhs) {
        return lhs.value == rhs.value && lhs.unit == rhs.unit;
    }

    friend constexpr bool operator!=(const Measurement &lhs,
                                     const Measurement &rhs) {
        return !(lhs == rhs);
    }

    friend constexpr Measurement operator+(const Measurement &lhs,
                                           const Measurement &rhs) {
        if (lhs.unit != rhs.unit) {
            throw std::runtime_error(
                "Cannot add measurements with mismatched units");
        }

        return Measurement(lhs.value + rhs.value, lhs.unit);
    }

    friend constexpr Measurement operator-(const Measurement &lhs,
                                           const Measurement &rhs) {

        if (lhs.unit != rhs.unit) {
            throw std::runtime_error(
                "Cannot subviewact measurements with mismatched units");
        }

        return Measurement(lhs.value - rhs.value, lhs.unit);
    }

    friend constexpr Measurement operator*(const Measurement &lhs,
                                           const Measurement &rhs) {
        return Measurement(lhs.value * rhs.value, lhs.unit * rhs.unit);
    }

    friend constexpr Measurement operator/(const Measurement &lhs,
                                           const Measurement &rhs) {

        return Measurement(lhs.value / rhs.value, lhs.unit / rhs.unit);
    }

    friend constexpr Measurement operator^(const Measurement &mes,
                                           const std::int8_t power) {

        return Measurement(std::pow(mes.value, power), mes.unit ^ power);
    }

    friend std::ostream &operator<<(std::ostream &os, const Measurement &mes) {
        os << mes.value << mes.unit;

        return os;
    }
};

constexpr Measurement::Measurement() {}

constexpr Measurement::Measurement(const double scalar, const units::Unit &unit)
    : value(scalar), unit(unit) {}

constexpr Measurement::Measurement(const std::string_view &s) {
    // TODO: parse measurement by splitting value and unit,
    // and calling the unit constructor on the unit string
}

constexpr Measurement::~Measurement() {}

constexpr Measurement operator*(const double scalar, const units::Unit &unit) {
    return Measurement(scalar, unit);
}

constexpr Measurement operator/(const double scalar, const units::Unit &unit) {
    return Measurement(scalar, units::base::noUnit / unit);
}

constexpr Measurement operator*(const units::Unit &unit, const double scalar) {
    return Measurement(scalar, unit);
}

constexpr Measurement operator/(const units::Unit &unit, const double scalar) {
    return Measurement(1.0 / scalar, unit);
}

namespace constants {

using namespace units;
inline constexpr auto c = Measurement(299792458.0, base::m / base::s);
inline constexpr auto h = Measurement(6.62607015e-34, derived::J *base::s);
inline constexpr auto e = Measurement(1.602176634e-19, derived::C);
inline constexpr auto k = Measurement(1.380649e-23, derived::J / base::K);
inline constexpr auto Na =
    Measurement(6.02214076e+23, base::noUnit / base::mol);
inline constexpr auto G =
    Measurement(6.6743e-11, (base::m ^ 3) * (base::kg ^ -1) * (base::s ^ -2));
} // namespace constants

} // namespace measurements

namespace units {

namespace literals {

using measurements::Measurement;

// Base literals

constexpr Measurement operator""_s(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::s);
}
constexpr Measurement operator""_m(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::m);
}
constexpr Measurement operator""_kg(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::kg);
}
constexpr Measurement operator""_A(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::A);
}
constexpr Measurement operator""_K(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::K);
}
constexpr Measurement operator""_mol(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::mol);
}
constexpr Measurement operator""_cd(const long double scalar) {
    return Measurement(static_cast<double>(scalar), base::cd);
}

// Derived literals

} // namespace literals

} // namespace units
