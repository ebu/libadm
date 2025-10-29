#pragma once

#include "adm/document.hpp"
#include "adm/serial/frame_header.hpp"
#include "adm/route.hpp"
#include "adm/utilities/comparator.hpp"
#include <boost/range/iterator_range.hpp>
#include "adm/route.hpp"
#include "adm/route_tracer.hpp"

#include <chrono>
#include <map>
#include <memory>

namespace adm {

  namespace detail {
    struct SegmenterItem {
      SegmenterItem(std::shared_ptr<const AudioProgramme> programme,
                    std::shared_ptr<const AudioContent> content,
                    std::vector<std::shared_ptr<const AudioObject>> objects,
                    std::vector<std::shared_ptr<const AudioPackFormat>> packFormats,
                    std::shared_ptr<const AudioChannelFormat> channelFormat,
                    std::shared_ptr<const AudioStreamFormat> streamFormat,
                    std::shared_ptr<const AudioTrackFormat> trackFormat,
                    std::shared_ptr<const AudioTrackUid> trackUid,
                    std::chrono::nanoseconds start,
                    boost::optional<std::chrono::nanoseconds> end)
          : programme(programme),
            content(content),
            objects(objects),
            packFormats(packFormats),
            channelFormat(channelFormat),
            streamFormat(streamFormat),
            trackFormat(trackFormat),
            trackUid(trackUid),
            start(start), end(end){};
      std::shared_ptr<const AudioProgramme> programme;
      std::shared_ptr<const AudioContent> content;
      std::vector<std::shared_ptr<const AudioObject>> objects;
      std::vector<std::shared_ptr<const AudioPackFormat>> packFormats;
      std::shared_ptr<const AudioChannelFormat> channelFormat;
      std::shared_ptr<const AudioStreamFormat> streamFormat;
      std::shared_ptr<const AudioTrackFormat> trackFormat;
      std::shared_ptr<const AudioTrackUid> trackUid;
      std::chrono::nanoseconds start;
      boost::optional<std::chrono::nanoseconds> end;
      bool use;
    };
  }  // namespace detail

  /// @brief Tag for NamedType ::SegmentStart
  struct SegmentStartTag {};
  /// @brief NamedType for the audioContentName attribute
  using SegmentStart =
      detail::NamedType<std::chrono::nanoseconds, SegmentStartTag>;
  /// @brief Tag for NamedType ::SegmentDuration
  struct SegmentDurationTag {};
  /// @brief NamedType for the audioContentName attribute
  using SegmentDuration =
      detail::NamedType<std::chrono::nanoseconds, SegmentDurationTag>;

  struct TrackUid {
    uint16_t trackIndex;
    std::string uid;
  };

  /// @brief This could be filled from the chna chunk.
  struct TrackUidList {
    std::vector<TrackUid> trackUid;
  };


  // This routing strategy ensures audioTrackUid is included.
  struct CustomStrategy {
    template <typename SubElement, typename Element>
    bool shouldRecurse(std::shared_ptr<Element>, std::shared_ptr<SubElement>) {
      return true;
    }
    bool shouldRecurse(std::shared_ptr<const adm::AudioStreamFormat>,
                      std::shared_ptr<const adm::AudioChannelFormat>) {
      return false;
    }

    template <typename Element>
    bool shouldAdd(std::shared_ptr<Element>) {
      return true;
    }

    template <typename Element>
    bool isEndOfRoute(std::shared_ptr<Element>) {
      return false;
    }

    bool isEndOfRoute(std::shared_ptr<const adm::AudioChannelFormat>) {
      return true;
    }

    bool isEndOfRoute(
        const std::shared_ptr<const adm::AudioTrackUid> &track_uid) {
      return track_uid->isSilent();
    }
  };

  /**
   * @brief Segment Document into Frames
   *
   * For now this class can only create full frames.
   *
   * The segmenter adds all AudioBlockFormats which *could* be relevant to the
   * requested segment. It does take into account the duration of the
   * AudioObject and AudioProgramme, but ignores the duration of the
   * AudioBlockFormat. Consider the following example:
   *
   * @code
   0s    1s    2s    3s    4s    5s    6s
   |-----|-----|-----|-----|-----|-----|

         +-----+-----+-----+-----+
         | AO1                   |
         +-----+-----+-----+-----+
         | AB1 | AB2 | AB3 | AB4 |
         +-----+-----+-----+-----+
   @endcode
   *
   * For the following segment ranges the behaviour will be as follows:
   *
   *  - [0s; 0.5s]: no AudioBlockFormat
   *  - [1s; 2s]: AB1, AB2
   *    - AB2 to calc the duration of AB1
   *  - [2s; 2.5s]: AB1, AB2, AB3
   *    - AB1 to calc the start values of an interpolation
   *    - AB2 to calc the end values of an interpolation
   *    - AB3 to calc the duration of AB2
   *  - [2.25s; 2.75s]: AB1, AB2, AB3
   *    - AB1 to calc the start values of an interpolation
   *    - AB2 to calc the end values of an interpolation
   *    - AB3 to calc the duration of AB2
   *  - [2s; 3s]: AB1, AB2, AB3
   *    - AB1 to calc the start values of an interpolation
   *    - AB2 to calc the end values of an interpolation
   *    - AB3 to calc the duration of AB2
   *    - AB4 is not relevant yet!
   *  - [4.5s; 5s]: AB3, AB4
   *    - AB3 to calc the start values of an interpolation
   *    - AB4 to calc the end values of an interpolation
   */
  class Segmenter {
   public:
    /**
     * @brief Segmenter Ctor
     *
     * @warning All the audioBlockFormats in the adm::document, which is passed
     * to the ctor will be sorted by time!
     */
    ADM_EXPORT Segmenter(std::shared_ptr<Document> document, boost::optional<Time> fileLength);

    /**
     * @brief Build a Frame with the given interval
     *
     * With every call the FrameFormatId is incremented.
     *
     * Similar to the Start and Duration of an AudioObject SegmentStart and
     * SegmentDuration are relative to the Start of the file.
     */
    ADM_EXPORT void buildFrame(SegmentStart start,
                               SegmentDuration segDuration,
                               unsigned int frameNum);

    /**
     * @brief Get the Frame document
     */
    ADM_EXPORT std::shared_ptr<Document> getFrame() {
      return baseFrame_;
    }

    /**
     * @brief Get the Frame Header 
     */
    ADM_EXPORT std::shared_ptr<FrameHeader> getFrameHeader() {
      return frameHeader_;
    }

    /**
     * @brief Generates the transportTrackFormat element
     *
     * Uses the TrackUidList to get the information on the UIDs allocated to each track.
     */
    ADM_EXPORT TransportTrackFormat generateTransportTrackFormat(
                                   TrackUidList trackUidList,
                                   SegmentStart segStart,
                                   SegmentDuration segDuration);

    /**
     * @brief Checks if audioTrackUID is present this frame
     *
     * Checks if the given audioTrackUidId_ref is present in the specified segment time
     * using the audioObject start and duration that references it.
     */
    ADM_EXPORT bool checkAudioObjectTimes(AudioTrackUidId audioTrackUidId_ref,
                            SegmentStart segStart,
                            SegmentDuration segDuration);


   private:
    void fixReferences();

    std::shared_ptr<Document> document_;
    std::shared_ptr<Document> baseFrame_;
    std::shared_ptr<FrameHeader> frameHeader_;
    std::vector<detail::SegmenterItem> segmenterItems_;
  };

  // ---- Implementation ---- //

  // FIXME: Make private or move to detail !!!

  /**
   * @brief Add relevant AudioBlocks for the given timespan to
   * AudioChannelFormat. This uses local time mode.
   */
  template <typename AudioBlockFormat>
  uint32_t addItemToChannelFormat(const detail::SegmenterItem& item,
                              SegmentStart segStart,
                              SegmentDuration segDuration,
                              std::shared_ptr<AudioChannelFormat> dest) {
    auto foundBlockFormats =
        findBlockFormats(item.channelFormat->getElements<AudioBlockFormat>(),
                         segStart, segDuration, item.start, item.end);

    // Add valid blocks to destination channel
    uint32_t numBlocks = 0;
    for (auto blockFormat : foundBlockFormats) {
      // For blocks with rtime and duration
      if (blockFormat.template has<Rtime>() && blockFormat.template has<Duration>()) {
        auto rtime = blockFormat.template get<Rtime>().get().asNanoseconds();
        auto duration = blockFormat.template get<Duration>().get().asNanoseconds();
        auto lstart = rtime - segStart.get();
        auto lduration = duration; 
        // Generate the extra initial block 
        if (numBlocks == 0) {
          auto blockFormat_init = blockFormat;
          blockFormat_init.set(InitializeBlock(true));
          blockFormat_init.set(Rtime(lstart));
          blockFormat_init.template unset<Duration>();
          auto block_format_id_co = AudioBlockFormatIdCounter(0);
          auto block_format_id = blockFormat_init.template get<AudioBlockFormatId>();
          block_format_id.set(block_format_id_co);
          blockFormat_init.set(block_format_id);
          dest->add(blockFormat_init);
        } 
        blockFormat.set(Rtime(lstart));
        blockFormat.set(Duration(lduration));
        dest->add(blockFormat);
      } else {
        // For static blocks
        dest->add(blockFormat);
      }

      numBlocks++;
    }

    return numBlocks;
  }

  /**
   * @brief Find relevant AudioBlocks for the given timespan
   *
   * NOTE: The AudioBlockFormatRange has to be sorted.
   */
  template <typename AudioBlockFormatRange>
  boost::iterator_range<typename AudioBlockFormatRange::iterator>
  findBlockFormats(AudioBlockFormatRange blockFormatSrc, SegmentStart segStart,
                   SegmentDuration segDuration,
                   std::chrono::nanoseconds objectStart,
                   boost::optional<std::chrono::nanoseconds> objectEnd) {
    typedef typename AudioBlockFormatRange::value_type AudioBlockFormat;

    // object already ended before segment start -> return no blocks
    if (objectEnd && objectEnd < segStart.get()) {
      return boost::make_iterator_range(blockFormatSrc.end(),
                                        blockFormatSrc.end());
    }

    // object did not begin before segment end -> return no blocks
    if (objectStart > segStart.get() + segDuration.get()) {
      return boost::make_iterator_range(blockFormatSrc.end(),
                                        blockFormatSrc.end());
    }

    // shrink search range to object bounds
    std::chrono::nanoseconds effectiveStart = segStart.get();
    if (effectiveStart < objectStart) {
      effectiveStart = objectStart;
    }
    std::chrono::nanoseconds effectiveEnd = segStart.get() + segDuration.get();
    if (objectEnd && objectEnd.get() < effectiveEnd) {
      effectiveEnd = objectEnd.get();
    }

    // Find the first and last valid blocks
    auto begin = blockFormatSrc.end();
    auto end = blockFormatSrc.begin();
    for (auto it = blockFormatSrc.begin() ; it != blockFormatSrc.end(); it++) {
      auto blockFormat = *it;
      auto seg_end = segStart.get() + segDuration.get();
      auto block_start = objectStart + blockFormat.template get<Rtime>().get().asNanoseconds();
      auto block_end = seg_end;
      if (blockFormat.template has<Duration>()) {
        block_end = objectStart + blockFormat.template get<Rtime>().get().asNanoseconds() + blockFormat.template get<Duration>().get().asNanoseconds();
      }

      if (block_start < seg_end && block_end > segStart.get()) {
        if (begin == blockFormatSrc.end()) {
          begin = it;
        }
        end = it;
      }
    }
    // If last block has been set move past one to end
    if (end != blockFormatSrc.begin()) {
      end++;
    } else {
      // Otherwise set to end of iterator
      end = blockFormatSrc.end();
    }

    return boost::make_iterator_range(begin, end);
  }

  template <typename AudioBlockFormat>
  void sortBlockFormats(std::shared_ptr<AudioChannelFormat> channelFormat) {
    auto blockFormats = channelFormat->getElements<AudioBlockFormat>();
    std::sort(blockFormats.begin(), blockFormats.end(),
              CompareRtimeDurationLess());
  }

}  // namespace adm
