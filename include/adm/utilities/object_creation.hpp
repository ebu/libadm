/// @file object_creation.hpp
#pragma once

#include <memory>
#include "adm/document.hpp"
#include "adm/libadm_export.h"

namespace adm {

  /**
   * @brief Simple holder used as return type for `createSimpleObject()`.
   * @headerfile object_creation.hpp <adm/utilities/object_creation.hpp>
   *
   * Gives access to all elements created by `createSimpleObject()`.
   * This exists basically to give quick and convenient direct access to the
   * elements after creation.
   */
  struct SimpleObjectHolder {
    std::shared_ptr<AudioObject> audioObject;
    std::shared_ptr<AudioPackFormat> audioPackFormat;
    std::shared_ptr<AudioChannelFormat> audioChannelFormat;
    std::shared_ptr<AudioStreamFormat> audioStreamFormat;
    std::shared_ptr<AudioTrackFormat> audioTrackFormat;
    std::shared_ptr<AudioTrackUid> audioTrackUid;
  };

  /**
   * @brief `AudioObject` hierarchie creation for single
   * `TypeDefinition::OBJECTS`-type element
   *
   * Create an `AudioObject` including refrenced `AudioPackFormat` and
   * `AudioChannelFormat` of type `TypeDefinition::OBJECTS`, as well an
   * `AudioTrackUid`, the referenced `AudioTrackFormat` and
   * `AudioStreamFormat` of type `FormatDefinition::PCM`.
   *
   * @param name Name that will be used for the created
   * `Audio{Object,PackFormat,ChannelFormat}`.
   */
  LIBADM_EXPORT SimpleObjectHolder createSimpleObject(const std::string& name);

}  // namespace adm
