#include "adm/path.hpp"

namespace adm {

  struct AddElementVisitor : public boost::static_visitor<> {
    AddElementVisitor(Path* path) : path_(path) {}

    template <typename T>
    void operator()(T v) const {
      path_->add(v);
    }

   private:
    Path* path_;
  };

  struct AddIdVisitor : public boost::static_visitor<> {
    AddIdVisitor(Path* path) : path_(path) {}

    template <typename T>
    void operator()(T v) const {
      path_->add(v);
    }

   private:
    Path* path_;
  };

  void Path::add(ElementConstVariant elementVariant) {
    boost::apply_visitor(AddElementVisitor(this), elementVariant);
  }

  void Path::add(ElementIdVariant elementIdVariant) {
    boost::apply_visitor(AddIdVisitor(this), elementIdVariant);
  }

}  // namespace adm
