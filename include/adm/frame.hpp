/// @file frame.hpp
#pragma once

#include <memory>
#include <vector>
#include "adm/document.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/export.h"
#include "adm/serial.hpp"

namespace adm {
  /**
   * @ingroup sadm
   * @brief Class representation of a whole adm frame
   * @headerfile frame.hpp <adm/frame.hpp>
   */
  class Frame : public Document {
   public:
    /**
     * @brief Static helper function to create an Frame
     *
     * The constructor is private. This way it is ensured, that an Frame
     * can only be created as a `std::shared_ptr`.
     */
    ADM_EXPORT static std::shared_ptr<Frame> create(FrameStart start,
                                                    FrameDuration duration,
                                                    FrameType frameType,
                                                    FrameFormatId frameFormatId);
    ADM_EXPORT static std::shared_ptr<Frame> create(FrameHeader frameHeader);

    /// @brief FrameStart getter
    ADM_EXPORT FrameStart start() const;
    /// @brief FrameDuration getter
    ADM_EXPORT FrameDuration duration() const;

    /**
     * @brief Create a copy of the Frame including all elements
     */
    ADM_EXPORT std::shared_ptr<Frame> deepCopy() const;

    /// @brief FrameHeader getter
    ADM_EXPORT const FrameHeader &frameHeader() const;
    ADM_EXPORT FrameHeader &frameHeader();
    ADM_EXPORT void setFrameHeader(FrameHeader frameHeader);

    ADM_EXPORT std::shared_ptr<Document> getDocument() { return audioFormatExtended_; }
    
   private:
    ADM_EXPORT Frame(FrameStart start, FrameDuration duration,
                     FrameType frameType, FrameFormatId frameFormatId);
    ADM_EXPORT Frame(FrameHeader frameHeader);
    ADM_EXPORT Frame(const Frame &) = default;
    ADM_EXPORT Frame(Frame &&) = default;

    std::shared_ptr<Document> audioFormatExtended_;
    FrameHeader frameHeader_;
  };

}  // namespace adm
