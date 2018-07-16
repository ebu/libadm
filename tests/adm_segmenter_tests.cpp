#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/format.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/document.hpp"
#include "adm/elements.hpp"
#include "adm/frame.hpp"
#include "adm/segmenter.hpp"
#include "adm/detail/hex_values.hpp"
#include "adm/utilities/copy.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"
#include "adm/utilities/comparator.hpp"
#include <tuple>
#include <string>
#include <boost/optional.hpp>

#define BOOST_TEST_MODULE Segmenter
#include <boost/test/included/unit_test.hpp>

using namespace adm;

// AudioBlockFormatIdCounter; Start; Duration
using BlockformatHolder =
    std::tuple<uint32_t, uint32_t, boost::optional<uint32_t>>;

void addBlockFormats(std::shared_ptr<AudioChannelFormat> channelFormat,
                     std::vector<BlockformatHolder> blockFormats) {
  for (auto blockFormatHolder : blockFormats) {
    AudioBlockFormatId id(
        TypeDefinition::OBJECTS, AudioBlockFormatIdValue(0x1001),
        AudioBlockFormatIdCounter(std::get<0>(blockFormatHolder)));
    AudioBlockFormatObjects blockFormat{
        SphericalPosition{}, id,
        Rtime(std::chrono::seconds(std::get<1>(blockFormatHolder)))};
    if (std::get<2>(blockFormatHolder)) {
      blockFormat.set(
          Duration(std::chrono::seconds(std::get<2>(blockFormatHolder).get())));
    }
    channelFormat->add(blockFormat);
  }
}

std::shared_ptr<Document> createDocument(
    Start audioProgrammeStart, boost::optional<End> audioProgrammeEnd,
    Start audioObjectStart, boost::optional<Duration> audioObjectDuration,
    std::vector<BlockformatHolder> blockFormats) {
  auto document = Document::create();
  auto programme = AudioProgramme::create(AudioProgrammeName("Programme"));
  programme->set(audioProgrammeStart);
  if (audioProgrammeEnd) {
    programme->set(audioProgrammeEnd.get());
  }
  auto content = AudioContent::create(AudioContentName("Content"));
  auto holder = createSimpleObject("Object");
  auto object = holder.audioObject;
  object->set(audioObjectStart);
  if (audioObjectDuration) {
    object->set(audioObjectDuration.get());
  }
  auto object1 = AudioObject::create(AudioObjectName("Object1"));
  object1->set(Start(std::chrono::seconds(100)));
  object1->addReference(object);
  auto object2 = AudioObject::create(AudioObjectName("Object2"));
  object2->set(Start(std::chrono::seconds(300)));
  object2->addReference(object1);
  programme->addReference(content);
  content->addReference(object2);
  document->add(programme);
  auto channelFormat = document->getElements<AudioChannelFormat>()[0];
  addBlockFormats(channelFormat, blockFormats);

  BOOST_TEST_MESSAGE(" AudioProgramme:");
  BOOST_TEST_MESSAGE("   Start: " << audioProgrammeStart.get().count() / 1e9
                                  << "s");
  if (audioProgrammeEnd) {
    BOOST_TEST_MESSAGE("   End: " << audioProgrammeEnd.get().get().count() / 1e9
                                  << "s");
  } else {
    BOOST_TEST_MESSAGE("   End: not set");
  }
  BOOST_TEST_MESSAGE(" AudioObject:");
  BOOST_TEST_MESSAGE("   Start: " << audioObjectStart.get().count() / 1e9
                                  << "s");
  if (audioObjectDuration) {
    BOOST_TEST_MESSAGE("   Duration: "
                       << audioObjectDuration.get().get().count() / 1e9 << "s");
  } else {
    BOOST_TEST_MESSAGE("   Duration: not set");
  }
  BOOST_TEST_MESSAGE("-------------------------------------------------------");

  return document;
}

boost::test_tools::predicate_result check_frame_blockformat_ids(
    std::shared_ptr<Frame> frame,
    const std::vector<std::string> &blockFormatIds,
    uint32_t channelFormatIndex = 0) {
  boost::test_tools::predicate_result failure(false);

  auto channelFormatFrame =
      frame->getElements<AudioChannelFormat>()[channelFormatIndex];
  auto blockFormats =
      channelFormatFrame->getElements<AudioBlockFormatObjects>();
  if (blockFormats.size() != blockFormatIds.size()) {
    failure.message() << "Different number of blockFormats ["
                      << blockFormats.size() << "!=" << blockFormatIds.size()
                      << "]\n";
    failure.message() << "[";
    for (auto blockFormat : blockFormats) {
      failure.message() << formatId(blockFormat.get<AudioBlockFormatId>())
                        << "' ";
    }
    failure.message() << "!=";
    for (auto blockFormatId : blockFormatIds) {
      failure.message() << "'" << blockFormatId << "' ";
    }
    failure.message() << "]";

    return failure;
  }
  bool id_error = false;
  uint32_t index = 0;
  auto blockFormatIt = blockFormats.begin();
  auto blockFormatIdIt = blockFormatIds.begin();
  while (blockFormatIt != blockFormats.end() &&
         blockFormatIdIt != blockFormatIds.end()) {
    if (formatId((*blockFormatIt).get<AudioBlockFormatId>()) !=
        (*blockFormatIdIt)) {
      failure.message() << "Mismatch at index (" << index << "): '"
                        << formatId((*blockFormatIt).get<AudioBlockFormatId>())
                        << "' != '" << (*blockFormatIdIt) << "'\n";
      id_error = true;
    }
    ++blockFormatIt;
    ++blockFormatIdIt;
    ++index;
  };

  if (id_error) {
    return failure;
  }
  return true;
}

#define CHECK_FRAME_BLOCKFORMAT_IDS(frame, blocks) \
  BOOST_CHECK(check_frame_blockformat_ids(frame, blocks))

BOOST_AUTO_TEST_CASE(basic_test) {
  auto document = createDocument(
      Start(std::chrono::seconds(0)), End(std::chrono::seconds(15)),
      Start(std::chrono::seconds(0)), Duration(std::chrono::seconds(5)),
      std::vector<BlockformatHolder>{
          std::make_tuple(1, 1, boost::none),
          std::make_tuple(2, 2, boost::none),
          std::make_tuple(3, 3, boost::none),
          std::make_tuple(4, 4, 1),
      });
  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(0)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(1000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000001", "AB_00031001_00000002"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(1000)),
                         SegmentDuration(std::chrono::milliseconds(1000))),
      std::vector<std::string>(
          {"AB_00031001_00000001", "AB_00031001_00000002"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2000)),
                         SegmentDuration(std::chrono::milliseconds(1000))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2250)),
                         SegmentDuration(std::chrono::milliseconds(1500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003",
                                "AB_00031001_00000004"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(4500)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000003", "AB_00031001_00000004"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(5250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
}

BOOST_AUTO_TEST_CASE(with_object_start_time) {
  auto document = createDocument(
      Start(std::chrono::seconds(0)), End(std::chrono::seconds(10)),
      Start(std::chrono::seconds(5)), Duration(std::chrono::seconds(5)),
      std::vector<BlockformatHolder>{
          std::make_tuple(1, 1, boost::none),
          std::make_tuple(2, 2, boost::none),
          std::make_tuple(3, 3, boost::none),
          std::make_tuple(4, 4, 1),
      });
  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(0)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(5000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(6000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000001", "AB_00031001_00000002"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(7000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(7250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(9500)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000003", "AB_00031001_00000004"}));
}

BOOST_AUTO_TEST_CASE(without_object_end_time) {
  auto document = createDocument(Start(std::chrono::seconds(0)),
                                 End(std::chrono::seconds(10)),
                                 Start(std::chrono::seconds(5)), boost::none,
                                 std::vector<BlockformatHolder>{
                                     std::make_tuple(1, 1, boost::none),
                                     std::make_tuple(2, 2, boost::none),
                                     std::make_tuple(3, 3, boost::none),
                                     std::make_tuple(4, 4, 1),
                                 });
  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(0)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(5000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(6000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000001", "AB_00031001_00000002"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(7000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(7250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(9500)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000003", "AB_00031001_00000004"}));
}

BOOST_AUTO_TEST_CASE(without_object_and_programe_end_time) {
  auto document = createDocument(Start(std::chrono::seconds(0)), boost::none,
                                 Start(std::chrono::seconds(5)), boost::none,
                                 std::vector<BlockformatHolder>{
                                     std::make_tuple(1, 1, boost::none),
                                     std::make_tuple(2, 2, boost::none),
                                     std::make_tuple(3, 3, boost::none),
                                     std::make_tuple(4, 4, 1),
                                 });
  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(0)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(5000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(6000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000001", "AB_00031001_00000002"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(7000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(7250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(9500)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000003", "AB_00031001_00000004"}));
}

BOOST_AUTO_TEST_CASE(unsorted_audio_block_formats) {
  auto document = createDocument(
      Start(std::chrono::seconds(0)), End(std::chrono::seconds(15)),
      Start(std::chrono::seconds(0)), Duration(std::chrono::seconds(5)),
      std::vector<BlockformatHolder>{
          std::make_tuple(3, 3, boost::none),
          std::make_tuple(4, 4, 1),
          std::make_tuple(2, 2, boost::none),
          std::make_tuple(1, 1, boost::none),
      });
  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(0)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(1000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000001", "AB_00031001_00000002"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2000)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2250)),
                         SegmentDuration(std::chrono::milliseconds(1500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003",
                                "AB_00031001_00000004"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(4500)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>(
          {"AB_00031001_00000003", "AB_00031001_00000004"}));
  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(5250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>());
}

BOOST_AUTO_TEST_CASE(multiple_programmes) {
  auto document = Document::create();
  auto programme1 = AudioProgramme::create(AudioProgrammeName("Programme1"),
                                           Start(std::chrono::seconds(10)),
                                           End(std::chrono::seconds(15)));
  auto programme2 = AudioProgramme::create(AudioProgrammeName("Programme2"),
                                           Start(std::chrono::seconds(10)),
                                           End(std::chrono::seconds(15)));
  auto content = AudioContent::create(AudioContentName("Content"));
  auto holder = createSimpleObject("Object");

  addBlockFormats(holder.audioChannelFormat,
                  std::vector<BlockformatHolder>{
                      std::make_tuple(1, 1, boost::none),
                      std::make_tuple(2, 2, boost::none),
                      std::make_tuple(3, 3, boost::none),
                      std::make_tuple(4, 4, 1),
                  });

  programme1->addReference(content);
  programme2->addReference(content);
  content->addReference(holder.audioObject);

  document->add(programme1);
  document->add(programme2);

  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
}

BOOST_AUTO_TEST_CASE(multiple_programmes_different_object_start) {
  auto document = Document::create();
  auto programme1 = AudioProgramme::create(AudioProgrammeName("Programme1"),
                                           Start(std::chrono::seconds(10)),
                                           End(std::chrono::seconds(15)));
  auto programme2 = AudioProgramme::create(AudioProgrammeName("Programme2"),
                                           Start(std::chrono::seconds(10)),
                                           End(std::chrono::seconds(15)));
  auto content = AudioContent::create(AudioContentName("Content"));
  auto audioObject1 = AudioObject::create(AudioObjectName("Object1"));
  audioObject1->set(Start(std::chrono::seconds(0)));
  auto audioObject2 = AudioObject::create(AudioObjectName("Object2"));
  audioObject2->set(Start(std::chrono::seconds(1)));

  auto audioPackFormat = AudioPackFormat::create(AudioPackFormatName("Pack"),
                                                 TypeDefinition::OBJECTS);
  auto audioStreamFormat = AudioStreamFormat::create(
      AudioStreamFormatName("Stream"), FormatDefinition::PCM);
  auto audioTrackFormat = AudioTrackFormat::create(
      AudioTrackFormatName("Track"), FormatDefinition::PCM);
  auto audioChannelFormat = AudioChannelFormat::create(
      AudioChannelFormatName("Channel"), TypeDefinition::OBJECTS);
  addBlockFormats(audioChannelFormat, std::vector<BlockformatHolder>{
                                          std::make_tuple(1, 1, boost::none),
                                          std::make_tuple(2, 2, boost::none),
                                          std::make_tuple(3, 3, boost::none),
                                          std::make_tuple(4, 4, 1),
                                      });
  auto audioTrackUid = AudioTrackUid::create();

  // reference
  audioObject1->addReference(audioPackFormat);
  audioObject2->addReference(audioPackFormat);
  audioPackFormat->addReference(audioChannelFormat);
  audioStreamFormat->setReference(audioChannelFormat);
  audioTrackFormat->setReference(audioStreamFormat);
  audioObject1->addReference(audioTrackUid);
  audioObject2->addReference(audioTrackUid);
  audioTrackUid->setReference(audioTrackFormat);
  audioTrackUid->setReference(audioPackFormat);

  programme1->addReference(content);
  programme2->addReference(content);
  content->addReference(audioObject1);
  content->addReference(audioObject2);

  document->add(programme1);
  document->add(programme2);

  Segmenter segmenter(document);

  CHECK_FRAME_BLOCKFORMAT_IDS(
      segmenter.getFrame(SegmentStart(std::chrono::milliseconds(2250)),
                         SegmentDuration(std::chrono::milliseconds(500))),
      std::vector<std::string>({"AB_00031001_00000001", "AB_00031001_00000002",
                                "AB_00031001_00000003"}));
}
