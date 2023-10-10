/// @file parse_sadm.hpp
#pragma once
#include <string>
#include <memory>
#include <iosfwd>
#include "adm/serial/frame_header.hpp"
#include "adm/export.h"
#include "adm/parse.hpp"

namespace adm {

  ADM_EXPORT FrameHeader parseFrameHeader(
    std::istream& stream, adm::xml::ParserOptions = adm::xml::ParserOptions::none);
  
  
  ADM_EXPORT FrameHeader parseFrameHeader(
    std::string const& fileName, adm::xml::ParserOptions = adm::xml::ParserOptions::none);
}  // namespace adm
