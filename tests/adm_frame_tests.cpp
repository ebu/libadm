#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/frame.hpp"
#include "adm/elements.hpp"
#include "adm/utilities/id_assignment.hpp"
#include "adm/utilities/object_creation.hpp"

#define BOOST_TEST_MODULE Frame
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(adm_frame_test) {
  using namespace adm;

  // create a adm frame
  {
    auto admFrame = Frame::create(FrameStart(std::chrono::nanoseconds(0)),
                                     FrameDuration(std::chrono::nanoseconds(1)),
                                     FrameType("full"));
    auto speechContent = AudioContent::create(AudioContentName("Speech"));

    FrameFormat frameFormat(FrameStart(std::chrono::seconds(10)),
                            FrameDuration(std::chrono::seconds(20)),
                            FrameType("header"));

    auto speechContentFr = AudioContent::create(AudioContentName("Speech"),
                                                AudioContentLanguage("fr"));
    auto speechContentEn = AudioContent::create(AudioContentName("Speech"),
                                                AudioContentLanguage("en"));

    BOOST_TEST(speechContentEn->has<AudioContentLanguage>());
    BOOST_TEST(speechContentEn->get<AudioContentLanguage>() == "en");

    auto musicContent = AudioContent::create(AudioContentName("Music"));
    BOOST_TEST(admFrame->add(speechContent));
    BOOST_TEST(admFrame->add(musicContent));
    auto narratorObject = AudioObject::create(AudioObjectName("Narrator"));
    BOOST_TEST(admFrame->add(narratorObject));
    BOOST_TEST(speechContent->addReference(narratorObject));

    auto stereoPack = AudioPackFormat::create(AudioPackFormatName("stereo"),
                                              TypeDescriptor(0));
    BOOST_TEST(admFrame->add(stereoPack));

    auto admFrame_2 = Frame::create(
        FrameStart(std::chrono::nanoseconds(0)),
        FrameDuration(std::chrono::nanoseconds(1)), FrameType("full"));
    BOOST_CHECK_THROW(admFrame_2->add(speechContent), std::runtime_error);
    BOOST_CHECK_THROW(admFrame_2->add(musicContent), std::runtime_error);
    BOOST_CHECK_THROW(admFrame_2->add(narratorObject), std::runtime_error);
    BOOST_CHECK_THROW(admFrame_2->add(stereoPack), std::runtime_error);
  }
}
