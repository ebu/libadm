#include "adm/utilities/copy.hpp"
#include "adm/private/copy.hpp"
#include <boost/variant.hpp>

namespace adm {

  std::shared_ptr<Document> deepCopy(std::shared_ptr<const Document> document) {
    auto copy = Document::create();
    deepCopyTo(document, copy);
    return copy;
  }

  void deepCopyTo(std::shared_ptr<const Document> src,
                  std::shared_ptr<Document> dest) {
    auto copiedElements = copyAllElements(src);
    addElements(copiedElements, dest);
  }

  void deepCopyTo(std::shared_ptr<const Document> src,
                  std::shared_ptr<Frame> dest) {
    auto copiedElements = copyAllElements(src);
    addElements(copiedElements, dest);
  }

}  // namespace adm
