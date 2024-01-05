#include "adm/utilities/copy.hpp"
#include "adm/private/copy.hpp"
#include <boost/variant.hpp>

namespace adm {

  std::shared_ptr<Document> deepCopy(std::shared_ptr<const Document> document) {
    return document->deepCopy();
  }

  void deepCopyTo(std::shared_ptr<const Document> src,
                  std::shared_ptr<Document> dest) {
    auto copiedElements = copyAllElements(src);
    addElements(copiedElements, dest);
  }

}  // namespace adm
