#pragma once

/**
 * @file serialized.hpp
 *
 * Convenience header to inlcude all ADM element headers.
 */

/**
 * @defgroup sadm Serialized ADM elements
 *
 * @brief C++ representations of the elements required by the serialized version
 * of ADM
 *
 * These extend the classes of @ref main by the additional elements introducted
 * in ITU-R BS.[ADM-Serial]
 *
 */
#include "adm/serialized/frame_header.hpp"
#include "adm/serialized/frame_format.hpp"
#include "adm/serialized/transport_track_format.hpp"
#include "adm/serialized/transport_track_format_id.hpp"
#include "adm/serialized/audio_track.hpp"
