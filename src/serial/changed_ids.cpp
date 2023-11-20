#include "adm/serial/changed_ids.hpp"

std::string adm::formatValue(adm::StatusValue status) {
  switch (status) {
    case StatusValue::NEW:
      return "new";
    case StatusValue::CHANGED:
      return "changed";
    case StatusValue::EXTENDED:
      return "extended";
    case StatusValue::EXPIRED:
      return "expired";
  }
  throw std::runtime_error("Unsupported StatusValue");
}
