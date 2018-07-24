#pragma once

#include <adm/elements_fwd.hpp>
#include <chrono>
#include <memory>
#include "adm/libadm_export.h"

namespace adm {

  /**
   * @brief Set or update durations of all `AudioBlockFormats`
   *
   * According to ITU-R BS. 2076, multiple `AudioBlockFormats` in an
   * `AudioChannelFormat` should all have a rtime and a duration.
   *
   * As these durations might be linked to the duration of referencing
   * `AudioObject`s, the length of the parent `AudioProgramme` and/or the length
   * of a BW64 file, it is hard or impossible to set the correct duration during
   * `AudioBlockFormat` creation.
   *
   * This utility function will update the `AudioBlockFormat` durations to match
   * the lifetime of the referencing `AudioObject`(s) or, if not set, the length
   * of the audio file given by `fileLength`.
   *
   * An exception will be raised if there's any ambiguity in the resulting
   * duration, for example due to multiple `AudioObject`s with different
   * durations referencing the same `AudioChannelFormat`. Differences between
   * the duration of an `AudioProgramme` and `fileLength` will also be
   * considered an error.
   *
   * @param document The document to update, durations will be adapted in-place.
   * @param fileLength The length of the BW64 audio file
   * @sa void updateBlockFormatDurations(std::shared_ptr<Document>)
   */
  LIBADM_EXPORT void updateBlockFormatDurations(
      std::shared_ptr<Document> document, std::chrono::nanoseconds fileLength);

  /**
   * @brief Set or update durations of all `AudioBlockFormats`
   *
   * This function provides essentially the same functionality as
   * adm::updateBlockFormatDurations(std::shared_ptr<Document>,
   * std::chrono::nanoseconds), with the only difference that the duration of
   * the `AudioProgramme` will be to determine the lifetime of `AudioObject`s.
   *
   * @param document The document to update, durations will be adapted in-place.
   * @sa void updateBlockFormatDurations(std::shared_ptr<Document>,
   * std::chrono::nanoseconds)
   */
  LIBADM_EXPORT void updateBlockFormatDurations(
      std::shared_ptr<Document> document);

}  // namespace adm
