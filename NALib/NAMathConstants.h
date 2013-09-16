
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_MATH_CONSTANTS_INCLUDED
#define NA_MATH_CONSTANTS_INCLUDED

#ifdef __cplusplus 
  #include <cfloat>
  #include <cmath>
#else
  #include <float.h>
  #include <math.h>
#endif

// Fundamental mathematical constants. All macros are defined as float, double
// and long double with the corresponding suffix.
//
// Note that there usually exist constants in math.h like M_PI but they
// are not standardized and so far, the author has not found a difference
// in computation.
//
// Note that the constants are written with approximately 30 digits which is
// more than enough even for long double.

// The famous pi constant
#define NA_PIf             3.14159265358979323846264338328f
#define NA_PI              3.14159265358979323846264338328
#define NA_PIl             3.14159265358979323846264338328L

// 2 times Pi. The full circumference of the unit circle.
#define NA_PI2f            6.28318530717958647692528676656f
#define NA_PI2             6.28318530717958647692528676656
#define NA_PI2l            6.28318530717958647692528676656L

// The half of Pi. Corresponds to an angle of 90 Degrees
#define NA_PI_HALFf        1.57079632679489661923132169164f
#define NA_PI_HALF         1.57079632679489661923132169164
#define NA_PI_HALFl        1.57079632679489661923132169164L

// Square root of 2.
#define NA_SQRT2f          1.41421356237309504880168872421f
#define NA_SQRT2           1.41421356237309504880168872421
#define NA_SQRT2l          1.41421356237309504880168872421L

// Natural Logarithm of 2.
// Used on systems which do not defined naLog2 natively.
#define NA_LOGOF2f         0.693147180559945309417232121458f
#define NA_LOGOF2          0.693147180559945309417232121458
#define NA_LOGOF2l         0.693147180559945309417232121458L

// Natural Logarithm of 10.
// Used on systems which do not defined naLog10 natively.
#define NA_LOGOF10f        2.30258509299404568401799145468f
#define NA_LOGOF10         2.30258509299404568401799145468
#define NA_LOGOF10l        2.30258509299404568401799145468L

// The smaller part of the golden ratio
#define NA_GOLD_RATIOf     0.38196601125010515179541316563f
#define NA_GOLD_RATIO      0.38196601125010515179541316563
#define NA_GOLD_RATIOl     0.38196601125010515179541316563L

// The gigger part of the golden ratio
#define NA_GOLD_RATIO_BIGf 0.618033988749894848204586834370f
#define NA_GOLD_RATIO_BIG  0.618033988749894848204586834370
#define NA_GOLD_RATIO_BIGl 0.618033988749894848204586834370L

// The conversion factor between degrees and radiants
#define NA_DEGREESf        0.0174532925199432957692369076849f
#define NA_DEGREES         0.0174532925199432957692369076849
#define NA_DEGREESl        0.0174532925199432957692369076849L

// The conversion factor between radiants and degrees = 1/NA_DEGREES
#define NA_DEG_PER_RADf    57.2957795130823208767981548141f
#define NA_DEG_PER_RAD     57.2957795130823208767981548141
#define NA_DEG_PER_RADl    57.2957795130823208767981548141L

#define NA_THIRDf          0.33333333333333333333333333333f
#define NA_THIRD           0.33333333333333333333333333333
#define NA_THIRDl          0.33333333333333333333333333333l

#define NA_ARBITRARY_BUT_CAREFULLY_CHOSEN_VALUE 42

// The infinity definition is done here with the HUGE_VAL macros defined
// in <math.h> or <cmath> but it is unclear whether they correspond to infinity
// or just to a very large value. So far, the author never had problems with
// this definition though.
#ifndef INFINITY
  #ifndef HUGE_VALF
    #define NA_INFINITYf       ((float)HUGE_VAL)
    #define NA_INFINITY        HUGE_VAL
    #define NA_INFINITYl       ((long double)HUGE_VAL)
  #else
    #define NA_INFINITYf       HUGE_VALF
    #define NA_INFINITY        HUGE_VAL
    #define NA_INFINITYl       HUGE_VALL
  #endif
#else
  #define NA_INFINITYf       ((float)INFINITY)
  #define NA_INFINITY        INFINITY
  #define NA_INFINITYl       ((long double)INFINITY)
#endif

// The singularity is a very small number which corresponds to 10 times the
// smallest decimal digit accurately distinguishable by the corresponding
// floating-point-type. This is different than the machine-epsilon, found in
// the <float.h> or <cfloat> header files and has been useful to the author
// many times already.
// The sub- and sup-norm macros correspond to 1 minus or 1 plus NA_SINGULARITY.
#if (FLT_DIG < 6) || (DBL_DIG < 15)
  #warning "FLT and DBL digits for NA_SINGULARITY not achieved on this system"
#endif

#define NA_SINGULARITYf    1e-5f
#define NA_SUB_NORMf       0.99999f
#define NA_SUP_NORMf       1.00001f

#define NA_SINGULARITY     1e-14
#define NA_SUB_NORM        0.99999999999999
#define NA_SUP_NORM        1.00000000000001

#if (LDBL_DIG < 18)
  #if(LDBL_DIG == DBL_DIG)
    // On some systems, long doubles are supported but only have the
    // accuracy of doubles.
    #define NA_SINGULARITYl    NA_SINGULARITY
    #define NA_SUB_NORMl       NA_SUB_NORM
    #define NA_SUP_NORMl       NA_SUP_NORM
  #else
    #warning "LDBL digits for NA_SINGULARITY not achieved on this system"
  #endif
#else
  #define NA_SINGULARITYl    1e-17L
  #define NA_SUB_NORMl       0.99999999999999999L
  #define NA_SUP_NORMl       1.00000000000000001L
#endif


// The following constants are constants from physics, chemistry, mathematics,
// all mixed together. These constants are directly from NIST based on
// CODATA 2006:
// http://physics.nist.gov/constants
//
// All following constants are provided only as double constants.


// Universal Constants
// - CHAR_IMP_VAC       characteristic impedance of vacuum, Z_0 in [Ohm]
// - ELEC_CONST         electric constant, epsilon_0 in [Fm-1]
// - MAG_CONST          magnetic constant, mu_0 in [NA-2], = 4*PI*10^-7
// - GRAV               Newtonian constant of gravitation, G in [m3kg-1s-2]
// - GRAV_HBARC         Newtonian constant of gravitation over h-bar c,
//                      G/(h_bar c) in [(GeV/c2)-2]
// - PLANCK             Planck constant, h in [Js]
// - PLANCK_EVS         Planck constant in eV s, h in [eVs]
// - PLANCK_2PI         Planck constant over 2 pi, h_bar in [Js]
// - PLANCK_2PI_EVS     Planck constant over 2 pi in eV s, h_bar in [eVs]
// - PLANCK_2PIC        Planck constant over 2 pi times c in MeV fm, h_bar c
//                      in [MeV fm]
// - PLANCK_LEN         Planck length, l_P in [m]
// - PLANCK_MASS        Planck mass, m_P in [kg]
// - PLANCK_MASS_GEV    Planck mass energy equivalent in GeV, m_P c2 in [GeV]
// - PLANCK_TEMP        Planck temperature, T_P in [K]
// - PLANCK_TIME        Planck time, t_P in [s]
// - LIGHT_SPEED_VAC    speed of light in vacuum, c_0 in [ms-1]

#define NA_CHAR_IMP_VAC        376.730313461
#define NA_ELEC_CONST          8.854187817e-12
#define NA_MAG_CONST           12.566370614e-7
#define NA_GRAV                6.67428e-11
#define NA_GRAV_HBARC          6.70881e-39
#define NA_PLANCK              6.62606896e-34
#define NA_PLANCK_EVS          4.13566733e-15
#define NA_PLANCK_2PI          1.054571628e-34
#define NA_PLANCK_2PI_EVS      6.58211899e-16
#define NA_PLANCK_2PIC         197.3269631
#define NA_PLANCK_LEN          1.616252e-35
#define NA_PLANCK_MASS         2.17644e-8
#define NA_PLANCK_MASS_GEV     1.220892e19
#define NA_PLANCK_TEMP         1.416785e32
#define NA_PLANCK_TIME         5.39124e-44
#define NA_LIGHT_SPEED_VAC     299792458.

// Electromagnetic constants
// - BOHR               Bohr magneton, mu_B in [JT-1]
// - BOHR_EVT           Bohr magneton in eV/T, mu_B in [eVT-1]
// - BOHR_HZT           Bohr magneton in Hz/T, mu_B/h in [HzT-1]
// - BOHR_MTESLA        Bohr magneton in inverse meters per tesla, mu_B/hc
//                      in [m-1T-1]
// - BOHR_KT            Bohr magneton in K/T, mu_B/k in [KT-1]
// - COND_QUANT         conductance quantum, G_0 in [S]
// - COND_QUANT_INV     inverse of conductance quantum, G_0-1 in [Ohm]
// - E_CHARGE           elementary charge, e in [C]
// - E_CHARGE_H         elementary charge over h, e/h in [AJ-1]
// - JOSEPHSON          Josephson constant, K_J in [HzV-1]
// - MAG_FLUX_QUANT     magnetic flux quantum, Phi_0 in [Wb]
// - NUC_MAG            nuclear magneton, mu_N in [JT-1]
// - NUC_MAG_EVT        nuclear magneton in eV/T, mu_N in [eVT-1]
// - NUC_MAG_MTESLA     nuclear magneton in inverse meters per tesla, mu_N/hc
//                      in [m-1T-1]
// - NUC_MAG_KT         nuclear magneton in K/T, mu_N/k in [KT-1]
// - NUC_MAG_MHZT       nuclear magneton in MHz/T, mu_N/h in [MHzT-1]
// - VON_KLITZING       von Klitzing constant, R_K in [Ohm]

#define NA_BOHR                927.400915e-26
#define NA_BOHR_EVT            5.7883817555e-5
#define NA_BOHR_HZT            13.99624604e9
#define NA_BOHR_MTESLA         46.6864515
#define NA_BOHR_KT             0.6717131
#define NA_COND_QUANT          7.7480917004e-5
#define NA_COND_QUANT_INV      12906.4037787
#define NA_E_CHARGE            1.602176487e-19
#define NA_E_CHARGE_H          2.417989454e14
#define NA_JOSEPHSON           483597.891e9
#define NA_MAG_FLUX_QUANT      2.067833667e-15
#define NA_NUC_MAG             5.05078324e-27
#define NA_NUC_MAG_EVT         3.1524512326e-8
#define NA_NUC_MAG_MTESLA      2.542623616e-2
#define NA_NUC_MAG_KT          3.6582637e-4
#define NA_NUC_MAG_MHZT        7.62259384
#define NA_VON_KLITZING        25812.807557

// Physico-chemical constants
// - ATOM_MASS          atomic mass constant, m_u in [kg]
// - ATOM_MASS_ENERGY   atomic mass constant energy equivalent, m_uc2 in [J]
// - ATOM_MASS_MEV      atomic mass constant energy equivalent in MeV, m_uc2
//                      in [MeV]
// - AVOGADRO           Avogadro constant, N_A or L in [mol-1]
// - BOLTZMANN          Boltzmann constant, k in [JK-1]
// - BOLTZMANN_EVK      Boltzmann constant in eV/K, k in [eVK-1]
// - BOLTZMANN_HZK      Boltzmann constant in Hz/K, k/h in [HzK-1]
// - BOLTZMANN_MK       Boltzmann constant in inverse meters per kelvin, k/hc
//                      in [m-1K-1]
// - FARADAY            Faraday constant, F in [Cmol-1]
// - FIRST_RAD          first radiation constant, c_1 in [Wm2]
// - FIRST_RAD_SPECT    first radiation constant for spectral radiance, c_1L
//                      in [Wm2sr-1]
// - SECOND_RAD         second radiation constant, c_2 in [mK]
//                      = PLANCK*LIGHT_SPEED_VAC/BOLTZMANN
// - LOSCHMIDT          Loschmidt constant (273.15 K, 101.325 kPa), n_0 in [m-3]
// - MOL_GAS            molar gas constant, R in [Jmol-1K-1]
// - MOL_PLANCK         molar Planck constant, N_Ah in [Jsmol-1]
// - MOL_PLANCK_C       molar Planck constant times c, N_Ahc in [Jm*mol-1]
// - MOL_IDEAL_GAS_100  molar volume of ideal gas (273.15 K, 100 kPa), V_m
//                      in [m3mol-1]
// - MOL_IDEAL_GAS_101  molar volume of ideal gas (273.15 K, 101.325 kPa), V_m
//                      in [m3mol-1]
// - SACKUR_TETRODE_100 Sackur-Tetrode constant (1 K, 100 kPa), S_0/R, no unit
// - SACKUR_TETRODE_101 Sackur-Tetrode constant (1 K, 101.325 kPa), S_0/R,
//                      no unit
// - STEF_BOLTZ         Stefan-Boltzmann constant, sigma in [Wm-2K-4]
// - WIEN_FREQ          Wien frequency displacement law constant, b prime
//                      in [HzK-1]
// - WIEN_WAVE          Wien wavelength displacement law constant, b in [mK]

#define NA_ATOM_MASS           1.660538782e-27
#define NA_ATOM_MASS_ENERGY    1.492417830e-10
#define NA_ATOM_MASS_MEV       931.494028
#define NA_AVOGADRO            6.02214179e23
#define NA_BOLTZMANN           1.3806504e-23
#define NA_BOLTZMANN_EVK       8.617343e-5
#define NA_BOLTZMANN_HZK       2.0836644e10
#define NA_BOLTZMANN_MK        69.50356
#define NA_FARADAY             96485.3399
#define NA_FIRST_RAD           3.74177118e-16
#define NA_FIRST_RAD_SPECT     1.191042759e-16
#define NA_SECOND_RAD          1.4387752e-2
#define NA_LOSCHMIDT           2.6867774e25
#define NA_MOL_GAS             8.314472
#define NA_MOL_PLANCK          3.9903126821e-10
#define NA_MOL_PLANCK_C        0.11962656472
#define NA_MOL_IDEAL_GAS_100   22.710981e-3
#define NA_MOL_IDEAL_GAS_101   22.413996e-3
#define NA_SACKUR_TETRODE_100  -1.1517047
#define NA_SACKUR_TETRODE_101  -1.1648677
#define NA_STEF_BOLTZ          5.670400e-8
#define NA_WIEN_FREQ           5.878933e10
#define NA_WIEN_WAVE           2.8977685e-3

// Mathematical constants
// - MERSENNE_PRIME_2       Mersenne Prime 2^2 -1
// - MERSENNE_PRIME_3       Mersenne Prime 2^3 -1
// - MERSENNE_PRIME_5       Mersenne Prime 2^5 -1
// - MERSENNE_PRIME_7       Mersenne Prime 2^7 -1
// - MERSENNE_PRIME_13      Mersenne Prime 2^13-1
// - MERSENNE_PRIME_17      Mersenne Prime 2^17-1
// - MERSENNE_PRIME_19      Mersenne Prime 2^19-1
// - MERSENNE_PRIME_31      Mersenne Prime 2^31-1
// - MERSENNE_PRIME_61      Mersenne Prime 2^61-1
// - FERMAT_PRIME_1         Fermat Prime 2^1 +1
// - FERMAT_PRIME_2         Fermat Prime 2^2 +1
// - FERMAT_PRIME_4         Fermat Prime 2^4 +1
// - FERMAT_PRIME_8         Fermat Prime 2^8 +1
// - FERMAT_PRIME_16        Fermat Prime 2^16+1
// - FERMAT_PRIME_32        Fermat Prime 2^32+1
#define NA_MERSENNE_PRIME_2    3
#define NA_MERSENNE_PRIME_3    7
#define NA_MERSENNE_PRIME_5    31
#define NA_MERSENNE_PRIME_7    127
#define NA_MERSENNE_PRIME_13   8191
#define NA_MERSENNE_PRIME_17   131071
#define NA_MERSENNE_PRIME_19   524287
#define NA_MERSENNE_PRIME_31   2147483647
#define NA_MERSENNE_PRIME_61   2305843009213693951LL
#define NA_FERMAT_PRIME_0      3
#define NA_FERMAT_PRIME_2      5
#define NA_FERMAT_PRIME_4      17
#define NA_FERMAT_PRIME_8      257
#define NA_FERMAT_PRIME_16     65537
#define NA_FERMAT_PRIME_32     4294967297LL


#endif // NA_MATH_CONSTANTS_INCLUDED

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
