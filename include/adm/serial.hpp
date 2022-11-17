#pragma once

/**
 * @file serial.hpp
 *
 * Convenience header to inlcude all ADM element headers.
 */

/**
 * @defgroup sadm Serial ADM elements
 *
 * @brief C++ representations of the elements required by the serialized version
 * of ADM
 *
 * These extend the classes of @ref main by the additional elements introducted
 * in ITU-R BS.[ADM-Serial]
 *
 */
#include "adm/serial/frame_header.hpp"
#include "adm/serial/frame_format.hpp"
#include "adm/serial/transport_track_format.hpp"
#include "adm/serial/transport_id.hpp"
#include "adm/serial/audio_track.hpp"
