#pragma once

template <typename T1, typename T2>
void autoParent(T1 t1, T2 t2) {
  auto parent1 = t1->getParent().lock();
  auto parent2 = t2->getParent().lock();
  if (parent1 && !parent2) {
    parent1->add(t2);
  }
  if (!parent1 && parent2) {
    parent2->add(t1);
  }
}
