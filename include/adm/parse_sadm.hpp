/// @file parse_sadm.hpp
#pragma once
#include <string>
#include <memory>
#include <iosfwd>
#include "adm/export.h"
#include "adm/frame.hpp"
#include "adm/parse.hpp"

namespace adm {

  class Frame;

  ADM_EXPORT std::shared_ptr<Frame> parseSadmXml(
    std::istream& stream,
    xml::ParserOptions options = xml::ParserOptions::none);
  
  
  ADM_EXPORT std::shared_ptr<Frame> parseSadmXmlWithCommonDefs(
    std::istream& stream,
    std::shared_ptr<Frame> cd_frame,
    xml::ParserOptions options = xml::ParserOptions::none);
}  // namespace adm
