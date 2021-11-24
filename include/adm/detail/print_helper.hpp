#pragma once
#include <ostream>
#include <string>

namespace adm {
  namespace detail {
    /// print ADM objects to an ostream
    ///
    /// The constructor and destructor print '(' and ')' respectively, and
    /// methods like print can be used to print parameters between,
    /// automatically inserting separators if necessary
    template <typename T>
    class PrintHelper {
     public:
      PrintHelper(T const& obj, std::ostream& os) : obj(obj), os(os) {
        os << "(";
      }

      ~PrintHelper() { os << ")"; }

      /// prints name=get<Param>() if has<Param>()
      template <typename Param>
      void print(std::string const& name) {
        if (obj.template has<Param>()) {
          if (print_sep) os << ", ";

          os << name << "=" << obj.template get<Param>();

          print_sep = true;
        }
      }

     private:
      T const& obj;
      std::ostream& os;

      // does a separator need to be printed before the next component?
      bool print_sep = false;
    };

    /// make a PrintHelper, with type inference
    template <typename T>
    PrintHelper<T> make_print_helper(T const& obj, std::ostream& os) {
      return {obj, os};
    }

  }  // namespace detail
}  // namespace adm
