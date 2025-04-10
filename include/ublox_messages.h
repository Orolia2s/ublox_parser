#pragma once

/**
 * @file ublox_messages.h
 * Define Ublox messages as C structures.
 *
 * This code assumes a Little Endian host.
 */

#include "ublox.h"

#include <o2s/string.h> // string_t

#include <stdint.h> // uint*_t

/**
 * Navigation position, velocity and time solution.
 *
 * This message combines position, velocity and time solution, including accuracy figures.
 * Note that during a leap second there may be more or less than 60 seconds in a minute.
 */
struct ublox_position_time
{
	struct ublox_header header; /**< Inherit from ublox message base struct */

	uint32_t            time_of_week;           /**< GPS time of week of the navigation epoch, in milliseconds */
	uint16_t            year;                   /**< UTC year */
	uint8_t             month;                  /**< UTC month, from 1 to 12 */
	uint8_t             day;                    /**< UTC day of the month, from 1 to 31 */
	uint8_t             hour;                   /**< UTC hour of the day, from 0 to 23 */
	uint8_t             minute;                 /**< UTC minute of the hour, from 0 to 59 */
	uint8_t             second;                 /**< UTC second of the minute, from 0 to 60 */
	uint8_t             is_date_valid     :1;   /**< Date validity */
	uint8_t             is_time_valid     :1;   /**< Time validity */
	uint8_t             is_time_resolved  :1;   /**< UTC time of day has been fully resolved */
	uint8_t             is_magnetic_valid :1;   /**< Magnetic declination validity */
	uint32_t            time_accuracy;          /**< UTC time accuracy estimate, in nanoseconds */
	int32_t             nanosecond;             /**< Fraction of a second */
	uint8_t             gnss_fix;               /**< GNSS fix type */
	uint8_t             flags1;                 /* Fix status flags */
	uint8_t             flags2;                 /* Additional flags */
	uint8_t             satellite_count;        /**< Number of satellites used in Nav Solution */
	int32_t             longitude;              /**< in degrees */
	int32_t             latitude;               /**< in degrees */
	int32_t             height;                 /**< Height above ellipsoid, in millimeters */
	int32_t             height_above_sea_level; /**< Height above mean sea level, in millimeters */
	uint32_t            horizontal_accuracy;    /**< Horizontal accuracy estimate, in millimeters */
	uint32_t            vertical_accuracy;      /**< Vertical accuracy estimate, in millimeters */
	int32_t             velocity_north;         /**< NED north velocity, in millimeters per second */
	int32_t             velocity_east;          /**< NED east velocity, in millimeters per second */
	int32_t             velocity_down;          /**< NED down velocity, in millimeters per second */
	int32_t             ground_speed;           /**< 2D speed, in millimeters per second */
	int32_t             heading;                /**< 2D heading of motion, in degrees */
	uint32_t            speed_accuracy;         /**< Speed accuracy estimate , in millimeters / second*/
	uint32_t            heading_accuracy;
	uint16_t            dop;
	uint16_t            flags3; /* Additional flags */
	uint32_t            _reserved;
	int32_t             vehicle_heading;
	int16_t             magnetic_declination;
	uint16_t            magnetic_accuracy;
};

/**
 * Broadcast Navigation Data Subframe.
 *
 * This message reports a complete subframe of broadcast navigation data
 * decoded from a single signal. The number of data words reported in each
 * message depends on the nature of the signal.
 */
struct ublox_navigation_data
{
	struct ublox_header header;/**< Inherit from ublox message base struct */

	uint8_t constellation;     /**< GNSS identifier */
	uint8_t satellite;         /**< Satellite identifier */
	uint8_t signal;            /**< Signal identifier */
	uint8_t glonass_frequency; /**< Only used for GLONASS */
	uint8_t word_count;        /**< The number of data words contained in this message. */
	uint8_t channel;           /**< The tracking channel number the message was received on. */
	uint8_t version;           /**< Message version (=2 for this version). */
	uint8_t _reserved;
};

/**
 * Radio Frequency Information.
 *
 * Information for each RF block.
 * There are as many RF blocks reported as bands supported by this receiver.
 */
struct ublox_monitoring_rf
{
	struct ublox_header header; /**< Inherit from ublox message base struct */

	uint8_t  version;     /**< Message version (=0 in this case) */
	uint8_t  block_count; /**< Number of RF blocks included */
	uint16_t _reserved;
};

struct ublox_monitoring_rf_block
{
	uint8_t  id;               /**< RF block ID */
	uint8_t  jamming_state :2; /**< Output from Jamming/Interference Monitor */
	uint8_t  antenna_status;   /**< Status of the antenna supervisor state machine */
	uint8_t  antenna_power;    /**< Current power status of the antenna */
	uint32_t post_status;      /**< Power-On Self-Test status word */
	uint32_t _reserved1;
	uint16_t noise_per_ms;    /**< Noise level as measured by the GPS core */
	uint16_t agc_count;       /**< Automatic Gain Control */
	uint8_t  jam_level;       /**< Continuous Wave interference suppression level */
	int8_t   offset_i;        /**< Imbalance of the I-part of the complex signal */
	uint8_t  magnitude_i;     /**< Magnitude of the I-Part of the complex signal */
	int8_t   offset_q;        /**< Imbalance of the Q-part of the complex signal */
	uint8_t  magnitude_q;     /**< Magnitude of the Q-Part of the complex signal */
	uint8_t  _reserved2[3];
};

struct ublox_monitoring_hardware
{
	struct ublox_header header; /**< Inherit from ublox message base struct */

	uint32_t pins_selected;  /**< Mask of pins set as peripheral/PIO */
	uint32_t pins_bank;      /**< Mask of pins set as bank A/B */
	uint32_t pins_direction; /**< Mask of pins set as input/output */
	uint32_t pins_value;     /**< Mask of pins value low/high */
	uint16_t noise_per_ms;   /**< Noise level as measured by the GPS core */
	uint16_t agc_count;      /**< Automatic Gain Control */
	/** Status of the antenna supervisor state machine */
	uint8_t  antenna_status;
	uint8_t  antenna_power;        /**< Current power status of the antenna */
	uint8_t  is_rtc_calibrated :1; /**< is the Real-Time Clock calibrated ? */
	uint8_t  in_safe_boot_mode :1;
	/** Output from Jamming/Interference Monitor */
	uint8_t  jamming_state     :2;
	/** Has the real-time clock's crystal been determined to be absent ? */
	uint8_t  is_crystal_absent :1;
	uint8_t  _reserved0;
	/** Mask of pins that are used by the virtual pin manager */
	uint32_t used_mask;
	/** Array of pin mappings for each of the 17 physical pins */
	uint8_t  virtual_pins[17];
	uint8_t  jam_level; /**< Continuous Wave interference suppression level */
	uint8_t  _reserved1[2];
	uint32_t pins_interrupt; /**< Mask of pins value using the PIO Irq */
	uint32_t pins_pull_high; /**< Mask of pins value using the PIO pull high resistor */
	uint32_t pins_pull_low;  /**< Mask of pins value using the PIO pull low resistor */
};

string_t ublox_navigation_data_tostring(const struct ublox_navigation_data* message);
string_t ublox_monitoring_hardware_tostring(const struct ublox_monitoring_hardware* message);
string_t ublox_monitoring_rf_tostring(const struct ublox_monitoring_rf* message);
string_t ublox_position_time_tostring(const struct ublox_position_time* message);

/**
@var ublox_monitoring_rf_block::id
 - `0`: L1 band
 - `1`: L2 or L5 band (depending on product configuration)

@var ublox_monitoring_rf_block::jamming_state
@see ublox_jamming_state
This ﬂag is deprecated in protocol versions that support `UBX-SEC-SIG` (version 0x02);
instead `jammingState` in `UBX-SEC-SIG` should be monitored.

@var ublox_monitoring_rf_block::agc_count
The AGC monitor counts the number of times the signal level crosses a certain threshold, indicated
by the signals SIGHI and SIGLO
*/

/**
@var ublox_monitoring_hardware::agc_count
The AGC monitor counts the number of times the signal level crosses a certain threshold, indicated
by the signals SIGHI and SIGLO
*/

/**
@var ublox_navigation_data::glonass_frequency
This is the frequency slot + 7 (range from 0 to 13)

@var ublox_navigation_data::word_count
Up to 10 for currently supported signals
*/

/**
@struct ublox_navigation_data

@section integration More details from the Integration manual

UBX-RXM-SFRBX reports the broadcast navigation data message the receiver has
collected from each tracked signal.

When enabled, a separate message is generated each time the receiver decodes a
complete subframe of data from a tracked signal. The data bits are reported as
received, including preambles and error checking bits as appropriate. However,
because there is considerable variation in the data structure of the different
GNSS signals, the form of the reported data also varies.

This document uses the term "subframe", but other GNSS data structures might use
different terms, for example, GLONASS uses "strings" and Galileo uses "pages".

@subsection sub1 Parsing navigation data subframes

Each UBX-RXM-SFRBX message contains a subframe of data bits appropriate for the
relevant GNSS, delivered in a number of 32-bit words, as indicated by @ref
word_count field.

Due to the variation in data structure between different GNSS, the most important
step in parsing a UBX-RXM-SFRBX message is to identify the form of the data. This
should be done by reading the @ref constellation field, which indicates which GNSS
the data was decoded from. In almost all cases, this is sufficient to indicate
the structure. Because of this, the following sections are organized by GNSS.

However, in some cases the identity of the GNSS is not sufficient, and this is
described, where appropriate, in the following sections. In most cases, the data
does not map perfectly into a number of 32-bit words and, consequently, some of
the words reported in UBX-RXM-SFRBX messages contain fields marked as "Pad". These
fields should be ignored and no assumption should be made about their contents.
*/
