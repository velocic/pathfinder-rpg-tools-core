#include <gtest/gtest.h>
#include <abilityscores.h>

using namespace RulesEngine::Character;

class BaseScores : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for BaseScores test case
};

TEST_P(BaseScores, SetBaseAbilityScore)
{
    AbilityScores abilityScores;

    //GetParam() is a gtest helper that lets you grab different values and run
    //this test over multiple runs with different data
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 12);

    EXPECT_EQ(12, abilityScores.getBaseAbilityScore(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
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

class TemporaryAbilityScoreBonus : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for TemporaryAbilityScoreBonus test case
};

TEST_P(TemporaryAbilityScoreBonus, AddActuallyAddsBonusToList)
{
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    auto tempAdjustmentsCount = abilityScores.getTemporaryAdjustments(abilityScoreType).size();

    //Ensure list of tempAdjustments is empty at astart
    EXPECT_EQ(0, tempAdjustmentsCount);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "A Dummy Alchemical Bonus",
        2,
        "This jar of dirt gives +2 to an ability score... temporarily."
    );

    tempAdjustmentsCount = abilityScores.getTemporaryAdjustments(abilityScoreType).size();

    //Ensure we have 1 tempAdjustment in the collection now
    EXPECT_EQ(1, tempAdjustmentsCount);

    auto testBonus = abilityScores.getTemporaryAdjustment(abilityScoreType, "A Dummy Alchemical Bonus");

    EXPECT_EQ(AbilityScoreModifiers::Alchemical, testBonus.modifierType);
    EXPECT_EQ(abilityScoreType, testBonus.affectedScore);
    EXPECT_EQ("A Dummy Alchemical Bonus", testBonus.sourceName);
    EXPECT_EQ(2, testBonus.modifierValue);
    EXPECT_EQ("This jar of dirt gives +2 to an ability score... temporarily.", testBonus.description);
    EXPECT_EQ(true, testBonus.enabled);
}

TEST_P(TemporaryAbilityScoreBonus, BonusesOfSameModifierTypeDontStack)
{
}

TEST_P(TemporaryAbilityScoreBonus, OnlyHighestBonusAppliesWhenMultipleOfSameModifierType)
{
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScoreAdjustment)
{
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScore)
{
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScoreModifier)
{
}

TEST_P(TemporaryAbilityScoreBonus, DoesSourceExistReturnsTrueIfFound)
{
}

TEST_P(TemporaryAbilityScoreBonus, DoesSourceExistReturnsFalseIfNotFound)
{
}

TEST_P(TemporaryAbilityScoreBonus, RemoveDeletesBonusIfExists)
{
}

TEST_P(TemporaryAbilityScoreBonus, ToggleEnablesBonus)
{
}

TEST_P(TemporaryAbilityScoreBonus, ToggleDisablesBonus)
{
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    TemporaryAbilityScoreBonus, //Fixture class/Test case name
    ::testing::Values( //Actual value set to pass to the test run
        AbilityScoreTypes::STR,
        AbilityScoreTypes::DEX,
        AbilityScoreTypes::CON,
        AbilityScoreTypes::INT,
        AbilityScoreTypes::WIS,
        AbilityScoreTypes::CHA
    )
);
