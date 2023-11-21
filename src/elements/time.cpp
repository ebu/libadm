#include "adm/elements/time.hpp"
#include <boost/integer/common_factor.hpp>
#include <iomanip>
#include <regex>
#include <sstream>

namespace adm {
  namespace detail {
    template class DefaultParameter<TimeReference>;
    template <>
    TimeReference getDefault<TimeReference>() {
      return TimeReference{TimeReferenceValue::TOTAL};
    };
  }  // namespace detail

  FractionalTime FractionalTime::normalised() const {
    int64_t gcd = boost::integer::gcd(numerator(), denominator());
    return {numerator() / gcd, denominator() / gcd};
  }

  struct AsNanosecondsVisitor
      : public boost::static_visitor<std::chrono::nanoseconds> {
    std::chrono::nanoseconds operator()(
        const std::chrono::nanoseconds& time) const {
      return time;
    }

    std::chrono::nanoseconds operator()(const FractionalTime& time) const {
      FractionalTime normalised = time.normalised();
      return std::chrono::nanoseconds{(1000000000 * normalised.numerator()) /
                                      normalised.denominator()};
    }
  };

  struct AsFractionalVisitor : public boost::static_visitor<FractionalTime> {
    FractionalTime operator()(const std::chrono::nanoseconds& time) const {
      return {time.count(), 1000000000};
    }

    FractionalTime operator()(const FractionalTime& time) const { return time; }
  };

  std::chrono::nanoseconds Time::asNanoseconds() const {
    return boost::apply_visitor(AsNanosecondsVisitor(), time);
  }

  FractionalTime Time::asFractional() const {
    return boost::apply_visitor(AsFractionalVisitor(), time);
  }

  Time parseTimecode(const std::string& timecode) {
    const static std::regex commonFormat("(\\d{2}):(\\d{2}):(\\d{2}).(\\d+)");
    const static std::regex fractionalFormat(
        "(\\d{2}):(\\d{2}):(\\d{2}).(\\d+)S(\\d+)");

    std::smatch timecodeMatch;
    if (std::regex_match(timecode, timecodeMatch, commonFormat)) {
      const std::string& ns_str = timecodeMatch[4];

      // parse number of nanoseconds as if it always had 9 digits
      int64_t ns = 0;
      int64_t place_value = 1;
      for (size_t i = 8; i != (size_t)-1; i--) {
        if (i < ns_str.size()) ns += place_value * (ns_str[i] - '0');
        place_value *= 10;
      }

      return std::chrono::hours(stoi(timecodeMatch[1])) +
             std::chrono::minutes(stoi(timecodeMatch[2])) +
             std::chrono::seconds(stoi(timecodeMatch[3])) +
             std::chrono::nanoseconds(ns);
    } else if (std::regex_match(timecode, timecodeMatch, fractionalFormat)) {
      int64_t seconds = 3600 * stoi(timecodeMatch[1]) +
                        60 * stoi(timecodeMatch[2]) +
                        1 * stoi(timecodeMatch[3]);

      int64_t numerator = stoi(timecodeMatch[4]);
      int64_t denominator = stoi(timecodeMatch[5]);

      if (denominator == 0) {
        std::stringstream errorString;
        errorString << "invalid timecode: " << timecode
                    << " has a zero denominator";
        throw std::runtime_error(errorString.str());
      }

      return FractionalTime{seconds * denominator + numerator, denominator};
    } else {
      std::stringstream errorString;
      errorString << "invalid timecode: " << timecode;
      throw std::runtime_error(errorString.str());
    }
  }

  struct FormatTimeVisitor : public boost::static_visitor<std::string> {
    std::string operator()(const std::chrono::nanoseconds& time) const {
      std::stringstream ss;
      ss << std::setw(2) << std::setfill('0')
         << std::chrono::duration_cast<std::chrono::hours>(time).count();
      ss << ":";
      ss << std::setw(2) << std::setfill('0')
         << std::chrono::duration_cast<std::chrono::minutes>(time).count() % 60;
      ss << ":";
      ss << std::setw(2) << std::setfill('0')
         << std::chrono::duration_cast<std::chrono::seconds>(time).count() % 60;
      ss << ".";

      {
        auto ns = time.count() % 1000000000;
        // drop trailing zero digits, while keeping at least 5 to satisfy BS.2076-2
        int precision = 9;
        while (ns % 10 == 0 && precision > 5) {
          ns /= 10;
          precision--;
        }
        ss << std::setw(precision) << std::setfill('0') << ns;
      }

      return ss.str();
    }

    std::string operator()(const FractionalTime& time) const {
      int64_t whole_seconds = time.numerator() / time.denominator();
      int64_t frac_numerator =
          time.numerator() - whole_seconds * time.denominator();

      std::stringstream ss;
      ss << std::setw(2) << std::setfill('0') << whole_seconds / 3600;
      ss << ":";
      ss << std::setw(2) << std::setfill('0') << (whole_seconds / 60) % 60;
      ss << ":";
      ss << std::setw(2) << std::setfill('0') << whole_seconds % 60;
      ss << ".";
      ss << frac_numerator << "S" << time.denominator();
      return ss.str();
    }
  };

  std::string formatTimecode(const Time& time) {
    return boost::apply_visitor(FormatTimeVisitor(), time.asVariant());
  }

}  // namespace adm
