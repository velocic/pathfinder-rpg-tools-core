#include <gtest/gtest.h>
#include <abilityscores.h>

using namespace RulesEngine::Character;

std::vector<AbilityScoreTypes> listOfPossibleAbilityScores = {
    AbilityScoreTypes::STR,
    AbilityScoreTypes::DEX,
    AbilityScoreTypes::CON,
    AbilityScoreTypes::INT,
    AbilityScoreTypes::WIS,
    AbilityScoreTypes::CHA
};

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
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
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
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    auto tempAdjustmentsCount = abilityScores.getTemporaryAdjustments(abilityScoreType).size();

    //Ensure list of tempAdjustments is empty at astart
    EXPECT_EQ(0, tempAdjustmentsCount);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus2",
        6,
        "bonus2description"
    );

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus3",
        3,
        "bonus3description"
    );

    auto contributingTempAdjustmentsCount = abilityScores.getContributingAdjustments(abilityScoreType).size();

    //Should only have one bonus in the list, since same modifier sources shouldn't stack
    EXPECT_EQ(1, contributingTempAdjustmentsCount);

    auto contributingBonus = abilityScores.getContributingAdjustments(abilityScoreType)[0];

    EXPECT_EQ(AbilityScoreModifiers::Alchemical, contributingBonus.modifierType);
    EXPECT_EQ(abilityScoreType, contributingBonus.affectedScore);
    EXPECT_EQ("bonus2", contributingBonus.sourceName);
    EXPECT_EQ(6, contributingBonus.modifierValue);
    EXPECT_EQ("bonus2description", contributingBonus.description);
    EXPECT_EQ(true, contributingBonus.enabled);
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScoreAdjustment)
{
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    auto totalAdjustment = abilityScores.getTotalAdjustment(abilityScoreType);

    //Check that total calculated ability score is 0 at start
    EXPECT_EQ(0, totalAdjustment);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    totalAdjustment = abilityScores.getTotalAdjustment(abilityScoreType);

    EXPECT_EQ(2, totalAdjustment);
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScore)
{
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    auto totalScore = abilityScores.getTotalScore(abilityScoreType);

    //Check that total calculated ability score is 0 at start
    EXPECT_EQ(0, totalScore);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(2, totalScore);
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScoreModifier)
{
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    auto totalModifier = abilityScores.getTotalAbilityModifier(abilityScoreType);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    totalModifier = abilityScores.getTotalAbilityModifier(abilityScoreType);

    EXPECT_EQ(-4, totalModifier);
}

TEST_P(TemporaryAbilityScoreBonus, DoesSourceExistReturnsTrueIfFound)
{
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    EXPECT_EQ(true, abilityScores.doesTemporaryAbilityScoreBonusSourceExist(abilityScoreType, "bonus1"));
}

TEST_P(TemporaryAbilityScoreBonus, DoesSourceExistReturnsFalseIfNotFound)
{
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    abilityScores.removeTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");

    EXPECT_EQ(false, abilityScores.doesTemporaryAbilityScoreBonusSourceExist(abilityScoreType, "bonus1"));
}

TEST_P(TemporaryAbilityScoreBonus, ToggleBonus)
{
    //Temporary Bonuses are enabled by default
    AbilityScores abilityScores;
    auto abilityScoreType = GetParam();

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    //Toggle to false
    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");

    auto bonus = abilityScores.getTemporaryAdjustment(abilityScoreType, "bonus1");
    EXPECT_EQ(false, bonus.enabled);

    //Toggle back to true
    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");

    bonus = abilityScores.getTemporaryAdjustment(abilityScoreType, "bonus1");
    EXPECT_EQ(true, bonus.enabled);
}

TEST_P(TemporaryAbilityScoreBonus, CalculatesTotalAndModifierCorrectly)
{
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    TemporaryAbilityScoreBonus, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);
