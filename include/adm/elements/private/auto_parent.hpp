#pragma once

/// check and update the parents of self and other
///
/// if only one has no parent, set it to the parent of the other
///
/// returns true if they both have the same parent after this (or both have no
/// parent)
template <typename T1, typename T2>
bool autoParent(T1& self, const std::shared_ptr<T2>& other) {
  auto selfParent = self.getParent().lock();
  auto otherParent = other->getParent().lock();
  if (selfParent && !otherParent) {
    selfParent->add(other);
    return true;
  }
  if (!selfParent && otherParent) {
    otherParent->add(self.shared_from_this());
    return true;
  }

  return selfParent == otherParent;
}
