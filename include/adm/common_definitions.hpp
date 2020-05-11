#pragma once
#include <memory>
#include <map>
#include "adm/elements/type_descriptor.hpp"
#include "adm/document.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Load embedded common definitions file and create Document
  ADM_EXPORT std::shared_ptr<Document> getCommonDefinitions();

  /// @brief Add embedded common definitions file to a Document
  ADM_EXPORT void addCommonDefinitionsTo(std::shared_ptr<Document> document);

  /**
   * @brief Lookup table for AudioPackFormats
   *
   * Specified loudspeaker ids:
   *
   *  - 0+1+0
   *  - 0+2+0
   *  - 0+5+0
   *  - 2+5+0
   *  - 4+5+0
   *  - 4+5+1
   *  - 3+7+0
   *  - 4+9+0
   *  - 9+10+3
   *  - 0+7+0
   *  - 4+7+0
   *
   * @return Map with the loudspeaker id specified in ITU-R BS.2051 as key and
   * the corresponding AudioPackFormatID as value.
   */
  ADM_EXPORT const std::map<std::string, adm::AudioPackFormatId>
  audioPackFormatLookupTable();

  /**
   * @brief Lookup table for AudioTrackFormats
   *
   * Specified speaker labels:
   *
   *  - M+000, M+022, M-022, M+SC, M-SC, M+030, M-030, M+045, M-045,
   *    M+060, M-060, M+090, M-090, M+110, M-110, M+135, M-135, M+180
   *  - U+000, U+030, U-030, U+045, U-045, U+090, U-090, U+110, U-110,
   *    U+135, U-135, U+180, UH+180
   *  - T+000, B+000, B+045, B-045
   *  - LFE1, LFE2
   *
   * @return Map with the speaker label specified in ITU-R BS.2051 as key and
   * the corresponding AudioTrackFormatID as value.
   */
  ADM_EXPORT const std::map<std::string, adm::AudioTrackFormatId>
  audioTrackFormatLookupTable();

  /**
   * @brief Lookup table for SpeakerLabels of loudspeaker setup
   *
   * Specified loudspeaker ids:
   *
   *  - 0+1+0
   *  - 0+2+0
   *  - 0+5+0
   *  - 2+5+0
   *  - 4+5+0
   *  - 4+5+1
   *  - 3+7+0
   *  - 4+9+0
   *  - 9+10+3
   *  - 0+7+0
   *  - 4+7+0
   *
   * @return Map with the speaker label specified in ITU-R BS.2051 as key and
   * a vector of the corresponding speaker labels as value.
   */
  ADM_EXPORT const std::map<std::string, std::vector<std::string>>
  speakerLabelsLookupTable();

  /**
   * @brief Lookup function for AudioTrackFormats of type HOA, using order,
   * degree, and normalization
   * @param order
   * @param degree
   * @param normalization
   * @return The corresponding AudioTrackFormatID as specified in ITU-R BS.2094
   */
  ADM_EXPORT const adm::AudioTrackFormatId audioTrackFormatHoaLookup(
      int order, int degree, std::string normalization);

}  // namespace adm
