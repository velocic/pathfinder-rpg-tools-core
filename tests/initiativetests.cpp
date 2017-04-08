#include <testcommon.h>
#include <abilityscores.h>
#include <initiative.h>

using namespace RulesEngine::Character;

TEST(Initiative, DerivedCorrectlyFromAbilityModifier)
{
    {
        CharacterDescription characterDescription;
        AbilityScores abilityScores(characterDescription);
        abilityScores.setBaseAbilityScore(AbilityScoreTypes::DEX, 18);
        Initiative init(abilityScores);

        EXPECT_EQ(init.getInitiative(), 4);
    }

    {
        CharacterDescription characterDescription;
        AbilityScores abilityScores(characterDescription);
        abilityScores.setBaseAbilityScore(AbilityScoreTypes::DEX, 6);
        Initiative init(abilityScores);

        EXPECT_EQ(init.getInitiative(), -2);
    }
}

TEST(Initiative, DerivedCorrectlyFromMiscModifiers)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::DEX, 10);
    Initiative init(abilityScores);

    init.addInitiativeModifier(
        "Improved Initiative",
        "Grants an extra +4 to Initiative score",
        4
    );

    EXPECT_EQ(init.getInitiative(), 4);
}

TEST(Initiative, DerivedCorrectlyFromBoth)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::DEX, 18);
    Initiative init(abilityScores);

    init.addInitiativeModifier(
        "Improved Initiative",
        "Grants an extra +4 to Initiative score",
        4
    );
    init.addInitiativeModifier(
        "Broken Leg",
        "Ouch",
        -2
    );

    EXPECT_EQ(init.getInitiative(), 6);
}
