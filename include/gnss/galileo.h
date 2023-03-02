#pragma once

#include <stdint.h>

/**
 * @file gnss_subframes.h
 * @brief Content of the SFRBX words
 *
 * Note that the ‘semi-circle’ is not a SI unit but can be converted as:
 * 1 semi-circle = @f$ \pi @f$ rad.
 * @section Bit and Byte Ordering Criteria
 * All data values are encoded using the following bit and byte ordering criteria:
 *  - For numbering, the most significant bit/byte is numbered as bit/byte 0
 *  - For bit/byte ordering, the most significant bit/byte is transmitted first
 */

/**
 * F/NAV E5a-I Page Type 5.
 * The F-band (1278.75 MHz) is used for the transmission of Galileo System Time (GST) and other
 * system messages, such as almanacs and health status messages.
 *
 * The F/Nav message provides important information about the Galileo satellite's orbit, clock
 * corrections, and other parameters necessary for accurate navigation. These messages are
 * broadcast continuously by the satellite and are used by Galileo-compatible receivers to
 * determine their position and velocity.
 */
struct galileo_F_navigation_5
{
	uint64_t reserved        :18;
	uint64_t crc             :24; /**< Checksum */

	/** @name Space Vehicule 2 */
	/** @{ */
	uint64_t sv2_ascending_node :4;  /**< @f$\Omega_0@f$ */
	uint64_t sv2_delta_i        :11; /**< @f$\delta_i@f$ */
	uint64_t sv2_perigee        :16; /**< @f$\omega@f$ Argument of perigee */
	uint64_t sv2_eccentricity   :11; /**< @f$ e @f$ */
	uint64_t sv2_Delta_A        :13; /**< @f$\Delta(A^{1/2})@f$ */
	uint64_t sv2_satellite_id   :6;  /**< Should be @ref satellite_id + 1 */
	/** @} */

	/** @name Space Vehicule 1 */
	/** @{ */
	uint64_t signal_health   :2;  /**< @f$E5a_{HS}@f$ E5a Signal Health Status. */
	uint64_t a_f1            :13; /**< @f$a_{f1}@f$ */
	uint64_t a_f0            :16; /**< @f$a_{f0}@f$ */
	uint64_t mean_anomaly    :16; /**< @f$M_0@f$ Mean anomaly at reference time */
	uint64_t right_ascension :11; /**< @f$\dot{\Omega}@f$ Rate of change of right ascension */
	/** @f$\Omega_0@f$ Longitude of ascending node of orbital plane at weekly epoch. */
	uint64_t ascending_node  :16;
	uint64_t delta_i         :11; /**< @f$\delta_i@f$ */
	uint64_t perigee         :16; /**< @f$\omega@f$ Argument of perigee */
	uint64_t eccentricity    :11; /**< @f$ e @f$ */
	uint64_t Delta_A         :13; /**< @f$\Delta(A^{1/2})@f$ */
	uint64_t satellite_id    :6;  /**< 1 constellation of 36 satellites */
	/** @} */

	uint64_t reference_time  :10; /**< @f$t_{0a}@f$ Almanac reference time */
	uint64_t week_number     :2; /**< @f$WN_a@f$ Almanac reference Week Number */
	uint64_t issue_of_data   :4; /**< @f$IOD_a@f$ Almanac Issue Of Data */
	uint64_t page_type       :6; /**< From 1 to 6 */
} __attribute__((packed));

struct galileo_E5aI
{
	uint64_t reserved :18;
	uint64_t crc      :24;
	uint64_t page_type:6;
} __attribute__((packed));

/**
@var galileo_F_navigation_5::ascending_node
More precisely, @f$ \Omega_0 @f$ is the longitude of ascending node
of orbital plane at the weekly epoch propagated to the reference time
@f$ t_{0e} @f$ at the rate of change of right ascension.

@var galileo_F_navigation_5::Delta_A
Difference between the square root of the semi-major axis and the square root of the nominal semi-major axis
@f[ \Delta(A^{1/2}) = A^{1/2} - A^{1/2}_{nominal} @f]
Unit: @f$ meters^{1/2} @f$
*/

/**
@page Almanac
The almanac data is a reduced-precision subset of the clock and ephemeris parameters
of the active satellites in orbit. The user receiver utilises the algorithm described in section
5.1.1 to compute the positions of the Galileo satellites. All parameters appearing in the
equations of Table 61, but not included in the content of the almanac, are set to zero for
satellite position determination.

The Galileo almanac orbital parameters consist of
 - Semi-major axis
 - Eccentricity
 - Inclination
 - Longitude of the ascending node
 - Argument of perigee
 - Mean anomaly

The almanac time correction is provided by the coefficients af0 and af1 of a first order
polynomial, and is evaluated as per section 5.1.4 assuming the clock drift rate is set to zero.
Predicted signal health status is provided as well, which can be used to determine which
Galileo satellites should be tracked by the receiver.
The IODa disseminated with the almanac data allows detection of almanac batch changes.
The almanac reference time t0a and reference week number WNa apply to the orbital
parameters and to the clock correction parameters from the associated almanac batch.
The two-bit WNa provides the two least significant bits of the associated GST week number.

*/
