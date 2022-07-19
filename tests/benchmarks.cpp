#include <catch2/catch.hpp>
#include "adm/common_definitions.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/parse.hpp"
#include "adm/write.hpp"
#include <sstream>

using namespace adm;

TEST_CASE("common_definitions") {
  BENCHMARK("parse") { return getCommonDefinitions(); };

  auto common_defs = adm::getCommonDefinitions();
  BENCHMARK("copy") { return adm::deepCopy(common_defs); };
}

TEST_CASE("lots of blocks") {
  auto generate = []() {
    auto doc = Document::create();
    auto channel = AudioChannelFormat::create(AudioChannelFormatName{"c"},
                                              TypeDefinition::OBJECTS);
    doc->add(channel);

    const size_t n = 3600 * 20;

    for (size_t i = 0; i < n; i++)
      channel->add(AudioBlockFormatObjects{SphericalPosition{}});

    return doc;
  };

  BENCHMARK("generate") { return generate(); };

  auto document = generate();

  BENCHMARK("copy") { return adm::deepCopy(document); };

  BENCHMARK("write") {
    std::ostringstream stream;
    writeXml(stream, document);
    return stream;
  };

  std::stringstream stream;
  writeXml(stream, document);

  BENCHMARK("parse") {
    stream.seekg(0);
    return parseXml(stream);
  };
}

TEST_CASE("IDs") {
  AudioBlockFormatId bfId(TypeDefinition::OBJECTS, AudioBlockFormatIdValue(1),
                          AudioBlockFormatIdCounter(2));

  BENCHMARK("format") { return formatId(bfId); };
  std::string bfIdStr = formatId(bfId);

  BENCHMARK("parse") { return parseAudioBlockFormatId(bfIdStr); };
}
