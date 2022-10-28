#include "adm/elements/audio_block_format_id.hpp"
#include <sstream>
#include "adm/detail/id_parser.hpp"
#include "adm/detail/optional_comparison.hpp"

namespace adm {

  // ---- Defaults ---- //
  const TypeDescriptor AudioBlockFormatId::channelTypeDefault_ =
      TypeDefinition::UNDEFINED;
  const AudioBlockFormatIdValue AudioBlockFormatId::valueDefault_ =
      AudioBlockFormatIdValue(0);
  const AudioBlockFormatIdCounter AudioBlockFormatId::counterDefault_ =
      AudioBlockFormatIdCounter(0);

  // ---- Getter ---- //
  TypeDescriptor AudioBlockFormatId::get(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return boost::get_optional_value_or(channelType_, channelTypeDefault_);
  }
  AudioBlockFormatIdValue AudioBlockFormatId::get(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const {
    return boost::get_optional_value_or(value_, valueDefault_);
  }
  AudioBlockFormatIdCounter AudioBlockFormatId::get(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const {
    return boost::get_optional_value_or(counter_, counterDefault_);
  }

  // ---- Has ---- //
  bool AudioBlockFormatId::has(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return true;
  }
  bool AudioBlockFormatId::has(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const {
    return true;
  }
  bool AudioBlockFormatId::has(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const {
    return true;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatId::isDefault(
      detail::ParameterTraits<TypeDescriptor>::tag) const {
    return channelType_ == boost::none;
  }
  bool AudioBlockFormatId::isDefault(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) const {
    return value_ == boost::none;
  }
  bool AudioBlockFormatId::isDefault(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) const {
    return counter_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatId::set(TypeDescriptor channelType) {
    channelType_ = channelType;
  }
  void AudioBlockFormatId::set(AudioBlockFormatIdValue value) {
    value_ = value;
  }
  void AudioBlockFormatId::set(AudioBlockFormatIdCounter counter) {
    counter_ = counter;
  }

  // ---- Unsetter ---- //
  void AudioBlockFormatId::unset(detail::ParameterTraits<TypeDescriptor>::tag) {
    channelType_ = boost::none;
  }
  void AudioBlockFormatId::unset(
      detail::ParameterTraits<AudioBlockFormatIdValue>::tag) {
    value_ = boost::none;
  }
  void AudioBlockFormatId::unset(
      detail::ParameterTraits<AudioBlockFormatIdCounter>::tag) {
    counter_ = boost::none;
  }

  // ---- Operators ---- //
  using Compare =
      detail::comparison::OptionalComparator<TypeDescriptor,
                                             AudioBlockFormatIdValue,
                                             AudioBlockFormatIdCounter>;

  bool AudioBlockFormatId::operator==(const AudioBlockFormatId& other) const {
    return Compare::allEqual(*this, other);
  }

  bool AudioBlockFormatId::operator!=(const AudioBlockFormatId& other) const {
    return !(*this == other);
  }

  bool AudioBlockFormatId::operator<(const AudioBlockFormatId& other) const {
    return Compare::allLess(*this, other);
  }

  // ---- Common ---- //
  void AudioBlockFormatId::print(std::ostream& os) const {
    os << formatId(*this);
  }

  namespace detail {
    template <>
    struct IdTraits<AudioBlockFormatId> {
      static constexpr const char* name{"audioBlockFormatID"};
      static constexpr const char* format{"AB_yyyyxxxx_zzzzzzzz"};
      static constexpr std::size_t sections{3};
    };
    template <>
    struct IdSection<AudioBlockFormatId, 0> {
      using type = TypeDescriptor;
      static constexpr char identifier{'y'};
    };
    template <>
    struct IdSection<AudioBlockFormatId, 1> {
      using type = AudioBlockFormatIdValue;
      static constexpr char identifier{'x'};
    };
    template <>
    struct IdSection<AudioBlockFormatId, 2> {
      using type = AudioBlockFormatIdCounter;
      static constexpr char identifier{'z'};
    };

  }  // namespace detail

  AudioBlockFormatId parseAudioBlockFormatId(const std::string& id) {
    detail::IDParser<AudioBlockFormatId> parser{id};
    parser.validate();
    return parser.parse();
  }

  std::string formatId(const AudioBlockFormatId& id) {
    return detail::formatId(id);
  }

}  // namespace adm
