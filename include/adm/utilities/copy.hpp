#pragma once
#include "adm/export.h"
#include <memory>

namespace adm {
  class Document;
  ADM_EXPORT std::shared_ptr<Document> deepCopy(
      std::shared_ptr<const Document> document);

  ADM_EXPORT void deepCopyTo(std::shared_ptr<const Document> src,
                             std::shared_ptr<Document> dest);

}  // namespace adm
