#include "adm/frame.hpp"
#include "adm/document.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/private/copy.hpp"

namespace adm {

  std::shared_ptr<Document> deepCopy(std::shared_ptr<const Document> document) {
    auto copy = Document::create();
    deepCopyTo(document, copy);
    return copy;
  }

  std::shared_ptr<Frame> deepCopy(std::shared_ptr<const Frame> frame) {
    auto copy = Frame::create(frame->frameHeader());
    deepCopyTo(frame, copy);
    return copy;
  }

  void deepCopyTo(std::shared_ptr<const Document> src,
                  std::shared_ptr<Document> dest) {
    auto copiedElements = copyAllElements(src);
    addElements(copiedElements, dest);
  }

}  // namespace adm
