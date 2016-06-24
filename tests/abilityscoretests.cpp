#include <gtest/gtest.h>
#include <abilityscores.h>

using namespace RulesEngine::Character;

class BaseScores : public ::testing::TestWithParam<AbilityScoreTypes>
{
};

TEST_P(BaseScores, TestBaseAbilityScoresSetCorrectly)
{
    AbilityScores abilityScores;

    //GetParam() is a gtest helper that lets you grab different values and run
    //this test over multiple runs with different data
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 12);

    EXPECT_EQ(12, abilityScores.getBaseAbilityScore(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypes, //Fixture class instantiation name
    BaseScores, //Fixture class/Test case name
    ::testing::Values( //Actual value set to pass to the test run
        AbilityScoreTypes::STR,
        AbilityScoreTypes::DEX,
        AbilityScoreTypes::CON,
        AbilityScoreTypes::INT,
        AbilityScoreTypes::WIS,
        AbilityScoreTypes::CHA
    )
);
