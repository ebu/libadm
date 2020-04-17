#pragma once

/**
 * @file elements.hpp
 *
 * Convenience header to include all ADM element headers.
 */

/**
 * @defgroup main Main ADM elements
 *
 * @brief C++ representations of the main elements in the ADM
 *
 * These classes can be directly mapped to the elements of the Audio Definition
 * Model.
 *
 * If you want to work (create/analyse/edit) with the bare-metal ADM model these
 * classes are a good starting point.
 */

#include "adm/elements/audio_programme.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/elements/audio_pack_format.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/audio_track_format.hpp"
#include "adm/elements/audio_stream_format.hpp"
#include "adm/elements/audio_track_uid.hpp"

#include "adm/elements/audio_block_format_direct_speakers.hpp"
#include "adm/elements/audio_block_format_matrix.hpp"
#include "adm/elements/audio_block_format_objects.hpp"
#include "adm/elements/audio_block_format_hoa.hpp"
#include "adm/elements/audio_block_format_binaural.hpp"

#include "adm/elements/audio_pack_format_hoa.hpp"

#include "adm/elements/audio_programme_id.hpp"
#include "adm/elements/audio_content_id.hpp"
#include "adm/elements/audio_object_id.hpp"
#include "adm/elements/audio_pack_format_id.hpp"
#include "adm/elements/audio_channel_format_id.hpp"
#include "adm/elements/audio_track_format_id.hpp"
#include "adm/elements/audio_stream_format_id.hpp"
#include "adm/elements/audio_track_uid_id.hpp"
#include "adm/elements/audio_block_format_id.hpp"

#include "adm/elements/time.hpp"
#include "adm/elements/audio_programme_ref_screen.hpp"
#include "adm/elements/channel_lock.hpp"
#include "adm/elements/dialogue.hpp"
#include "adm/elements/format_descriptor.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/elements/importance.hpp"
#include "adm/elements/jump_position.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/object_divergence.hpp"
#include "adm/elements/position.hpp"
#include "adm/elements/position_types.hpp"
#include "adm/elements/screen_edge_lock.hpp"
#include "adm/elements/speaker_position.hpp"
#include "adm/elements/type_descriptor.hpp"
