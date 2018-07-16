#pragma once

#include "adm/document.hpp"
#include "adm/frame.hpp"

namespace adm {

  LIBADM_EXPORT std::shared_ptr<Document> deepCopy(
      std::shared_ptr<const Document> document);

  LIBADM_EXPORT void deepCopyTo(std::shared_ptr<const Document> src,
                                std::shared_ptr<Document> dest);

  LIBADM_EXPORT void deepCopyTo(std::shared_ptr<const Document> src,
                                std::shared_ptr<Frame> dest);

}  // namespace adm
