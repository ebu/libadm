#pragma once

#include "adm/document.hpp"

namespace adm {

  ADM_EXPORT std::shared_ptr<Document> deepCopy(
      std::shared_ptr<const Document> document);

  ADM_EXPORT void deepCopyTo(std::shared_ptr<const Document> src,
                             std::shared_ptr<Document> dest);

}  // namespace adm
