#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "adm/elements/audio_object_interaction.hpp"

#define BOOST_TEST_MODULE AudioObjectInteraction
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(audio_object_interaction,
                     *boost::unit_test::tolerance(1e-6)) {
  using namespace adm;
  AudioObjectInteraction interaction(OnOffInteract(true));

  BOOST_TEST(interaction.has<OnOffInteract>() == true);
  BOOST_TEST(interaction.has<GainInteract>() == false);
  BOOST_TEST(interaction.has<PositionInteract>() == false);
  BOOST_TEST(interaction.has<GainInteractionRange>() == false);
  BOOST_TEST(interaction.has<PositionInteractionRange>() == false);

  BOOST_TEST(interaction.isDefault<OnOffInteract>() == false);
  BOOST_TEST(interaction.isDefault<GainInteract>() == false);
  BOOST_TEST(interaction.isDefault<PositionInteract>() == false);
  BOOST_TEST(interaction.isDefault<GainInteractionRange>() == false);
  BOOST_TEST(interaction.isDefault<PositionInteractionRange>() == false);

  BOOST_TEST(interaction.get<OnOffInteract>() == true);

  interaction.set(OnOffInteract(false));
  interaction.set(GainInteract(true));
  interaction.set(PositionInteract(true));
  interaction.set(GainInteractionRange());
  interaction.set(PositionInteractionRange());

  BOOST_TEST(interaction.has<OnOffInteract>() == true);
  BOOST_TEST(interaction.has<GainInteract>() == true);
  BOOST_TEST(interaction.has<PositionInteract>() == true);
  BOOST_TEST(interaction.has<GainInteractionRange>() == true);
  BOOST_TEST(interaction.has<PositionInteractionRange>() == true);

  interaction.unset<GainInteract>();
  interaction.unset<PositionInteract>();
  interaction.unset<GainInteractionRange>();
  interaction.unset<PositionInteractionRange>();

  BOOST_TEST(interaction.has<OnOffInteract>() == true);
  BOOST_TEST(interaction.has<GainInteract>() == false);
  BOOST_TEST(interaction.has<PositionInteract>() == false);
  BOOST_TEST(interaction.has<GainInteractionRange>() == false);
  BOOST_TEST(interaction.has<PositionInteractionRange>() == false);
}
