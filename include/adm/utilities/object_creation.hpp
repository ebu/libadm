/// @file object_creation.hpp
#pragma once

#include <map>
#include <memory>
#include <string>
#include "adm/document.hpp"
#include "adm/export.h"

namespace adm {

  /**
   * @brief Simple holder used as return type for `createSimpleObject()` and
   * `addSimpleObjectTo()`.
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
   * @brief Simple holder used as return type for
   * `addSimpleCommonDefinitionsObjectTo()`.
   * @headerfile object_creation.hpp <adm/utilities/object_creation.hpp>
   *
   * Gives access to all elements created by
   * `addSimpleCommonDefinitionsObjectTo()`. This exists basically to give quick
   * and convenient direct access to the elements after creation.
   */
  struct SimpleCommonDefinitionsObjectHolder {
    std::shared_ptr<AudioObject> audioObject;
    std::map<std::string, std::shared_ptr<AudioTrackUid>> audioTrackUids;
  };

  /**
   * @brief Create `AudioObject` hierarchie for single
   * `TypeDefinition::OBJECTS`-type element
   *
   * Creates an `AudioObject` including referenced `AudioPackFormat` and
   * `AudioChannelFormat` of type `TypeDefinition::OBJECTS`, as well an
   * `AudioTrackUid`, the referenced `AudioTrackFormat` and
   * `AudioStreamFormat` of type `FormatDefinition::PCM`.
   *
   * @param name Name that will be used for the created
   * `Audio{Object,PackFormat,ChannelFormat}`.
   */
  ADM_EXPORT SimpleObjectHolder createSimpleObject(const std::string& name);

  /**
   * @brief Create and add `AudioObject` hierarchie for single
   * `TypeDefinition::OBJECTS`-type element
   *
   * same as `createSimpleObject`, but the elements are automatically added to
   * the given document
   */
  ADM_EXPORT SimpleObjectHolder addSimpleObjectTo(
      std::shared_ptr<Document> document, const std::string& name);

  /**
   * @brief Create and add `AudioObject` with common definitions direct speakers
   * channel bed to document
   *
   * Creates an `AudioObject` and corresponding `AudioTrackUids` and connects it
   * to the common definition ADM elements for the given speaker layout. The
   * created ADM elements are added to the given document.
   *
   * @note The document must already have the common definition elements added.
   *
   * @param document The document where the `AudioObject` and the
   * `AudioTrackUids` should be added to and whose common definition ADM
   * elements should be used.
   * @param name Name that will be used for the created `AudioObject`.
   * @param spakerLayout Speaker layout which will be created. For possible
   * values @see adm::audioPackFormatLookupTable
   */
  ADM_EXPORT SimpleCommonDefinitionsObjectHolder
  addSimpleCommonDefinitionsObjectTo(std::shared_ptr<Document> document,
                                     const std::string& name,
                                     const std::string& speakerLayout);

}  // namespace adm
