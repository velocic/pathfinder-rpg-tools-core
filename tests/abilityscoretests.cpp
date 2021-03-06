#include <testcommon.h>
#include <abilityscores.h>
#include <characterdescription.h>

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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);

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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto tempAdjustmentsCount = abilityScores.getTemporaryAdjustments(abilityScoreType).size();

    //Ensure list of tempAdjustments is empty at start
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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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

TEST_P(TemporaryAbilityScoreBonus, RemoveUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto totalScore = abilityScores.getTotalScore(abilityScoreType);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(2, totalScore);

    abilityScores.removeTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(0, totalScore);
}

TEST_P(TemporaryAbilityScoreBonus, ToggleUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto totalScore = abilityScores.getTotalScore(abilityScoreType);

    abilityScores.addTemporaryAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(2, totalScore);

    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");
    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(0, totalScore);

    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");
    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(2, totalScore);
}

TEST_P(TemporaryAbilityScoreBonus, AddUpdatesTotalAbilityScoreModifier)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
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
    EXPECT_EQ(0, abilityScores.getTotalScore(abilityScoreType));

    //Toggle back to true
    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus1");
    bonus = abilityScores.getTemporaryAdjustment(abilityScoreType, "bonus1");

    EXPECT_EQ(true, bonus.enabled);
    EXPECT_EQ(2, abilityScores.getTotalScore(abilityScoreType));
}

TEST_P(TemporaryAbilityScoreBonus, CalculatesTotalAndModifierCorrectly)
{
    //Utilizing a bunch of functionality to be closer to what an actual use-case is like for this test
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Alchemical, abilityScoreType, "bonus1", 3, "bonus1description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Alchemical, abilityScoreType, "bonus2", 1, "bonus2description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Enhancement, abilityScoreType, "bonus3", 1, "bonus3description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Enhancement, abilityScoreType, "bonus4", 2, "bonus4description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Inherent, abilityScoreType, "bonus5", 4, "bonus5description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Inherent, abilityScoreType, "bonus6", 1, "bonus6description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Morale, abilityScoreType, "bonus7", 1, "bonus7description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Morale, abilityScoreType, "bonus8", 2, "bonus8description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Size, abilityScoreType, "bonus9", 4, "bonus9description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "bonus10", 1, "bonus10description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "bonus11", 1, "bonus11description");

    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus7");
    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus4");
    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "bonus5");

    EXPECT_EQ(23, abilityScores.getTotalScore(abilityScoreType));
    EXPECT_EQ(13, abilityScores.getTotalAdjustment(abilityScoreType));
    EXPECT_EQ(6, abilityScores.getTotalAbilityModifier(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    TemporaryAbilityScoreBonus, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);

class PermanentAbilityScoreBonus : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for TemporaryAbilityScoreBonus test case
};

TEST_P(PermanentAbilityScoreBonus, AddActuallyAddsBonusToList)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto permanentAdjustmentsCount = abilityScores.getPermanentAdjustments(abilityScoreType).size();

    //Ensure list of permanentAdjustments is empty at start
    EXPECT_EQ(0, permanentAdjustmentsCount);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "testbonus",
        4,
        "dummy +4 modifier"
    );

    permanentAdjustmentsCount = abilityScores.getPermanentAdjustments(abilityScoreType).size();

    //Ensure we now have 1 permanentAdjustment in the collection now
    EXPECT_EQ(1, permanentAdjustmentsCount);
}

TEST_P(PermanentAbilityScoreBonus, BonusesOfSameModifierTypeDontStack)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Enhancement,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Enhancement,
        abilityScoreType,
        "bonus2",
        6,
        "bonus2description"
    );

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Enhancement,
        abilityScoreType,
        "bonus3",
        3,
        "bonus3description"
    );

    auto contributingPermanentAdjustmentsCount = abilityScores.getContributingAdjustments(abilityScoreType).size();

    //Should only have one bonus in the list, since same modifier sources shouldn't stack
    EXPECT_EQ(1, contributingPermanentAdjustmentsCount);

    auto contributingBonus = abilityScores.getContributingAdjustments(abilityScoreType)[0];

    EXPECT_EQ(AbilityScoreModifiers::Enhancement, contributingBonus.modifierType);
    EXPECT_EQ(abilityScoreType, contributingBonus.affectedScore);
    EXPECT_EQ("bonus2", contributingBonus.sourceName);
    EXPECT_EQ(6, contributingBonus.modifierValue);
    EXPECT_EQ("bonus2description", contributingBonus.description);
    EXPECT_EQ(true, contributingBonus.enabled);
}

TEST_P(PermanentAbilityScoreBonus, AddUpdatesTotalAbilityScoreAdjustment)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto totalAdjustment = abilityScores.getTotalAdjustment(abilityScoreType);

    //Check that total calculated ability score is 0 at start
    EXPECT_EQ(0, totalAdjustment);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Inherent,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    totalAdjustment = abilityScores.getTotalAdjustment(abilityScoreType);

    EXPECT_EQ(2, totalAdjustment);
}

TEST_P(PermanentAbilityScoreBonus, AddUpdatesTotalAbilityScoreDrain)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    //Add drain enough to put the character into a special condition
    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "bad",
        10,
        "you stood in bad stuff. how sad."
    );

    //A little setup just due to how we're running the same test once for
    //every ability score type, but need slightly different behavior in
    //this case
    CharacterStatus characterTestStatus;

    if (abilityScoreType == AbilityScoreTypes::STR || abilityScoreType == AbilityScoreTypes::WIS || abilityScoreType == AbilityScoreTypes::CHA) {
        characterTestStatus = CharacterStatus::Unconscious;
    } else if (abilityScoreType == AbilityScoreTypes::DEX) {
        characterTestStatus = CharacterStatus::Immobile;
    } else if (abilityScoreType == AbilityScoreTypes::INT) {
        characterTestStatus = CharacterStatus::Comatose;
    } else {
        //AbilityScoreTypes::CON
        characterTestStatus = CharacterStatus::Dead;
    }

    //Test ensure we're in a special character status depending on our abilityscoretype for this test run
    EXPECT_EQ(characterTestStatus, characterDescription.getCharacterStatus());

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Inherent,
        abilityScoreType,
        "bonus1",
        1,
        "now we should be ok again... barely"
    );

    EXPECT_EQ(CharacterStatus::Normal, characterDescription.getCharacterStatus());
}

TEST_P(PermanentAbilityScoreBonus, AddUpdatesBaseScoreWithPermanentAdjustments)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    auto baseWithPermAdjusts = abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType);

    EXPECT_EQ(10, baseWithPermAdjusts);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Inherent,
        abilityScoreType,
        "bonus1",
        2,
        "bonus1description"
    );

    baseWithPermAdjusts = abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType);

    EXPECT_EQ(12, baseWithPermAdjusts);
}

TEST_P(PermanentAbilityScoreBonus, AddUpdatesBaseModifierWithPermanentAdjustments)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    auto baseModWithPermAdjusts = abilityScores.getBaseModifierWithPermanentAdjustments(abilityScoreType);

    EXPECT_EQ(0, baseModWithPermAdjusts);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Morale,
        abilityScoreType,
        "bonus1",
        6,
        "bonus1description"
    );

    baseModWithPermAdjusts = abilityScores.getBaseModifierWithPermanentAdjustments(abilityScoreType);

    EXPECT_EQ(3, baseModWithPermAdjusts);
}

TEST_P(PermanentAbilityScoreBonus, AddUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    auto totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(10, totalScore);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Untyped,
        abilityScoreType,
        "bonus1",
        4,
        "bonus1description"
    );

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(14, totalScore);
}

TEST_P(PermanentAbilityScoreBonus, AddUpdatesTotalAbilityScoreModifier)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    auto totalModifier = abilityScores.getTotalAbilityModifier(abilityScoreType);

    EXPECT_EQ(0, totalModifier);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Untyped,
        abilityScoreType,
        "bonus1",
        4,
        "bonus1description"
    );

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus2",
        4,
        "bonus2description"
    );

    totalModifier = abilityScores.getTotalAbilityModifier(abilityScoreType);

    EXPECT_EQ(4, totalModifier);
}

TEST_P(PermanentAbilityScoreBonus, RemoveUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    auto totalScore = abilityScores.getTotalScore(abilityScoreType);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Untyped,
        abilityScoreType,
        "bonus1",
        4,
        "bonus1description"
    );

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(14, totalScore);

    abilityScores.removePermanentAbilityScoreBonus(abilityScoreType, "bonus1");

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(10, totalScore);
}

TEST_P(PermanentAbilityScoreBonus, ToggleUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    auto totalScore = abilityScores.getTotalScore(abilityScoreType);

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Untyped,
        abilityScoreType,
        "bonus1",
        4,
        "bonus1description"
    );

    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(14, totalScore);

    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus1");
    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(10, totalScore);

    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus1");
    totalScore = abilityScores.getTotalScore(abilityScoreType);

    EXPECT_EQ(14, totalScore);
}

TEST_P(PermanentAbilityScoreBonus, DoesSourceExistReturnsTrueIfFound)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus2",
        4,
        "bonus2description"
    );

    EXPECT_EQ(true, abilityScores.doesPermanentAbilityScoreBonusSourceExist(abilityScoreType, "bonus2"));
}

TEST_P(PermanentAbilityScoreBonus, DoesSourceExistReturnsFalseIfNotFound)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus2",
        4,
        "bonus2description"
    );

    abilityScores.removePermanentAbilityScoreBonus(abilityScoreType, "bonus2");

    EXPECT_EQ(false, abilityScores.doesPermanentAbilityScoreBonusSourceExist(abilityScoreType, "bonus2"));
}

TEST_P(PermanentAbilityScoreBonus, ToggleBonus)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addPermanentAbilityScoreBonus(
        AbilityScoreModifiers::Alchemical,
        abilityScoreType,
        "bonus2",
        4,
        "bonus2description"
    );

    //Permanent bonuses are true by default, so toggling it the first time disables it
    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus2");
    auto bonus = abilityScores.getPermanentAdjustment(abilityScoreType, "bonus2");

    EXPECT_EQ(false, bonus.enabled);
    EXPECT_EQ(0, abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType));

    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus2");
    bonus = abilityScores.getPermanentAdjustment(abilityScoreType, "bonus2");

    EXPECT_EQ(true, bonus.enabled);
    EXPECT_EQ(4, abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType));
}

TEST_P(PermanentAbilityScoreBonus, CalculatesModifierWithNegativeAbilityScoreCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 9);
    EXPECT_EQ(-1, abilityScores.getTotalAbilityModifier(abilityScoreType));

    abilityScores.setBaseAbilityScore(abilityScoreType, 7);
    EXPECT_EQ(-2, abilityScores.getTotalAbilityModifier(abilityScoreType));

    abilityScores.setBaseAbilityScore(abilityScoreType, 5);
    EXPECT_EQ(-3, abilityScores.getTotalAbilityModifier(abilityScoreType));

    abilityScores.setBaseAbilityScore(abilityScoreType, 3);
    EXPECT_EQ(-4, abilityScores.getTotalAbilityModifier(abilityScoreType));

    abilityScores.setBaseAbilityScore(abilityScoreType, 1);
    EXPECT_EQ(-5, abilityScores.getTotalAbilityModifier(abilityScoreType));
}

TEST_P(PermanentAbilityScoreBonus, CalculatesTotalAndModifierCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Alchemical, abilityScoreType, "bonus1", 3, "bonus1description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Alchemical, abilityScoreType, "bonus2", 1, "bonus2description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Enhancement, abilityScoreType, "bonus3", 1, "bonus3description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Enhancement, abilityScoreType, "bonus4", 2, "bonus4description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Inherent, abilityScoreType, "bonus5", 4, "bonus5description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Inherent, abilityScoreType, "bonus6", 1, "bonus6description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Morale, abilityScoreType, "bonus7", 1, "bonus7description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Morale, abilityScoreType, "bonus8", 2, "bonus8description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Size, abilityScoreType, "bonus9", 4, "bonus9description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "bonus10", 1, "bonus10description");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "bonus11", 1, "bonus11description");

    //Adding some temporary bonuses too, to generate a different TotalAdjustment than TotalScore
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "bonus12", 2, "bonus12description");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "bonus13", 4, "bonus13description");

    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus7");
    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus4");
    abilityScores.togglePermanentAbilityScoreBonus(abilityScoreType, "bonus5");

    EXPECT_EQ(29, abilityScores.getTotalScore(abilityScoreType));
    EXPECT_EQ(19, abilityScores.getTotalAdjustment(abilityScoreType));
    EXPECT_EQ(23, abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType));
    EXPECT_EQ(9, abilityScores.getTotalAbilityModifier(abilityScoreType));
    EXPECT_EQ(6, abilityScores.getBaseModifierWithPermanentAdjustments(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    PermanentAbilityScoreBonus, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);

class AbilityScoreDamageTests : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for TemporaryAbilityScoreBonus test case
};

//Ability score damage doesn't actually reduce the score. But, it adds a penalty
//to derived stats like CMB, CMD, AC, HP, spell DCs, etc based on the effected score.
//These shouldn't be calculated in AbilityScores as currently designed, though. Instead,
//most likely a whole bunch of other fields/sections are going to subscribe for updates
//when we add/remove/toggle ability score damage
TEST_P(AbilityScoreDamageTests, AddActuallyAddsDamageToList)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto abilityDamageCount = abilityScores.getAbilityDamage(abilityScoreType).size();

    //Ensure list of abilityDamage is empty at start
    EXPECT_EQ(0, abilityDamageCount);

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage1",
        2,
        "damage1description"
    );

    abilityDamageCount = abilityScores.getAbilityDamage(abilityScoreType).size();

    EXPECT_EQ(1, abilityDamageCount);

    auto testDamage = abilityScores.getAbilityDamage(abilityScoreType, "damage1");

    EXPECT_EQ(abilityScoreType, testDamage.affectedScore);
    EXPECT_EQ("damage1", testDamage.sourceName);
    EXPECT_EQ(2, testDamage.modifierValue);
    EXPECT_EQ("damage1description", testDamage.description);
    EXPECT_EQ(true, testDamage.enabled);
}

TEST_P(AbilityScoreDamageTests, AddUpdatesTotalAbilityScoreDamage)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    EXPECT_EQ(0, abilityScores.getTotalAbilityDamage(abilityScoreType));

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage1",
        2,
        "damage1description"
    );

    EXPECT_EQ(2, abilityScores.getTotalAbilityDamage(abilityScoreType));

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage2",
        6,
        "damage2description"
    );

    EXPECT_EQ(8, abilityScores.getTotalAbilityDamage(abilityScoreType));
}

TEST_P(AbilityScoreDamageTests, AbilityDamageAtOrBelowZeroChangesCharacterStatus)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 8);
    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage1",
        8,
        "damage2description"
    );

    CharacterStatus characterTestStatus;

    //Some setup for checking specific special status based on the current
    //ability score we're checking for this test run
    if (abilityScoreType == AbilityScoreTypes::STR || abilityScoreType == AbilityScoreTypes::WIS || abilityScoreType == AbilityScoreTypes::CHA) {
        characterTestStatus = CharacterStatus::Unconscious;
    } else if (abilityScoreType == AbilityScoreTypes::DEX) {
        characterTestStatus = CharacterStatus::Immobile;
    } else if (abilityScoreType == AbilityScoreTypes::INT) {
        characterTestStatus = CharacterStatus::Comatose;
    } else {
        //AbilityScoreTypes::CON
        characterTestStatus = CharacterStatus::Dead;
    }

    EXPECT_EQ(characterTestStatus, characterDescription.getCharacterStatus());
}

TEST_P(AbilityScoreDamageTests, DoesSourceExistReturnsTrueIfFound)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage2",
        6,
        "damage2description"
    );

    EXPECT_EQ(true, abilityScores.doesAbilityScoreDamageSourceExist(abilityScoreType, "damage2"));
}

TEST_P(AbilityScoreDamageTests, DoesSourceExistReturnsFalseIfNotFound)
{

    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage2",
        6,
        "damage2description"
    );

    abilityScores.removeAbilityScoreDamage(abilityScoreType, "damage2");

    EXPECT_EQ(false, abilityScores.doesAbilityScoreDamageSourceExist(abilityScoreType, "damage2"));
}

TEST_P(AbilityScoreDamageTests, ToggleAbilityScoreDamage)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage2",
        6,
        "damage2description"
    );

    auto testDamage = abilityScores.getAbilityDamage(abilityScoreType, "damage2");

    EXPECT_EQ(true, testDamage.enabled);

    abilityScores.toggleAbilityScoreDamage(abilityScoreType, "damage2");
    testDamage = abilityScores.getAbilityDamage(abilityScoreType, "damage2");

    EXPECT_EQ(false, testDamage.enabled);
}

TEST_P(AbilityScoreDamageTests, RemoveUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage2",
        6,
        "damage2description"
    );

    EXPECT_EQ(6, abilityScores.getTotalAbilityDamage(abilityScoreType));

    abilityScores.removeAbilityScoreDamage(abilityScoreType, "damage2");

    EXPECT_EQ(0, abilityScores.getTotalAbilityDamage(abilityScoreType));
}

TEST_P(AbilityScoreDamageTests, ToggleUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDamage(
        abilityScoreType,
        "damage2",
        6,
        "damage2description"
    );

    EXPECT_EQ(6, abilityScores.getTotalAbilityDamage(abilityScoreType));

    abilityScores.toggleAbilityScoreDamage(abilityScoreType, "damage2");

    EXPECT_EQ(0, abilityScores.getTotalAbilityDamage(abilityScoreType));
}


INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    AbilityScoreDamageTests, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);

class AbilityScorePenaltyTests : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for AbilityScorePenalty test case
};

TEST_P(AbilityScorePenaltyTests, AddActuallyAddsPenaltyToList)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto abilityPenaltyCount = abilityScores.getAbilityPenalties(abilityScoreType).size();

    EXPECT_EQ(0, abilityPenaltyCount);

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        3,
        "penalty1description"
    );

    abilityPenaltyCount = abilityScores.getAbilityPenalties(abilityScoreType).size();

    EXPECT_EQ(1, abilityPenaltyCount);

    auto testPenalty = abilityScores.getAbilityPenalty(abilityScoreType, "penalty1");

    EXPECT_EQ(abilityScoreType, testPenalty.affectedScore);
    EXPECT_EQ("penalty1", testPenalty.sourceName);
    EXPECT_EQ(3, testPenalty.modifierValue);
    EXPECT_EQ("penalty1description", testPenalty.description);
    EXPECT_EQ(true, testPenalty.enabled);
}

TEST_P(AbilityScorePenaltyTests, AddUpdatesTotalAbilityScorePenalty)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    EXPECT_EQ(0, abilityScores.getTotalAbilityPenalty(abilityScoreType));

    abilityScores.setBaseAbilityScore(abilityScoreType, 2);

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        1,
        "penalty1description"
    );

    EXPECT_EQ(1, abilityScores.getTotalAbilityPenalty(abilityScoreType));
}

TEST_P(AbilityScorePenaltyTests, AbilityPenaltyCantExceedTargetScoreWithPermanentBonusesMinusOne)
{

    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        3,
        "penalty1description"
    );
    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty2",
        3,
        "penalty2description"
    );
    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty3",
        3,
        "penalty3description"
    );
    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty4",
        5,
        "penalty4description"
    );

    EXPECT_EQ(9, abilityScores.getTotalAbilityPenalty(abilityScoreType));
}

TEST_P(AbilityScorePenaltyTests, DoesSourceExistReturnsTrueIfFound)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        1,
        "penalty1description"
    );

    EXPECT_EQ(true, abilityScores.doesAbilityScorePenaltySourceExist(abilityScoreType, "penalty1"));
}

TEST_P(AbilityScorePenaltyTests, DoesSourceExistReturnsFalseIfNotFound)
{

    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        1,
        "penalty1description"
    );

    abilityScores.removeAbilityScorePenalty(abilityScoreType, "penalty1");

    EXPECT_EQ(false, abilityScores.doesAbilityScorePenaltySourceExist(abilityScoreType, "penalty1"));
}

TEST_P(AbilityScorePenaltyTests, ToggleAbilityScorePenalty)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 14);

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        5,
        "penalty1description"
    );

    EXPECT_EQ(true, abilityScores.getAbilityPenalty(abilityScoreType, "penalty1").enabled);

    abilityScores.toggleAbilityScorePenalty(abilityScoreType, "penalty1");

    EXPECT_EQ(false, abilityScores.getAbilityPenalty(abilityScoreType, "penalty1").enabled);
}

TEST_P(AbilityScorePenaltyTests, RemoveUpdatesTotalAbilityScorePenalty)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 14);

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        5,
        "penalty1description"
    );

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty2",
        3,
        "penalty2description"
    );

    EXPECT_EQ(8, abilityScores.getTotalAbilityPenalty(abilityScoreType));

    abilityScores.removeAbilityScorePenalty(abilityScoreType, "penalty1");

    EXPECT_EQ(3, abilityScores.getTotalAbilityPenalty(abilityScoreType));
}

TEST_P(AbilityScorePenaltyTests, ToggleUpdatesTotalAbilityScorePenalty)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 14);

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty1",
        5,
        "penalty1description"
    );

    abilityScores.addAbilityScorePenalty(
        abilityScoreType,
        "penalty2",
        3,
        "penalty2description"
    );

    EXPECT_EQ(8, abilityScores.getTotalAbilityPenalty(abilityScoreType));

    abilityScores.toggleAbilityScorePenalty(abilityScoreType, "penalty2");

    EXPECT_EQ(5, abilityScores.getTotalAbilityPenalty(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    AbilityScorePenaltyTests, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);

class AbilityScoreDrainTests : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for AbilityScoreDrain test case
};

TEST_P(AbilityScoreDrainTests, AddActuallyAddsDrainToList)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    auto abilityDrainCount = abilityScores.getAbilityDrain(abilityScoreType).size();

    EXPECT_EQ(0, abilityDrainCount);

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        6,
        "drain1description"
    );

    abilityDrainCount = abilityScores.getAbilityDrain(abilityScoreType).size();

    EXPECT_EQ(1, abilityDrainCount);

    auto testDrain = abilityScores.getAbilityDrain(abilityScoreType, "drain1");

    EXPECT_EQ(abilityScoreType, testDrain.affectedScore);
    EXPECT_EQ("drain1", testDrain.sourceName);
    EXPECT_EQ(6, testDrain.modifierValue);
    EXPECT_EQ("drain1description", testDrain.description);
    EXPECT_EQ(true, testDrain.enabled);
}

TEST_P(AbilityScoreDrainTests, AddUpdatesTotalAbilityScoreDrain)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    EXPECT_EQ(0, abilityScores.getTotalAbilityDrain(abilityScoreType));

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        6,
        "drain1description"
    );

    EXPECT_EQ(6, abilityScores.getTotalAbilityDrain(abilityScoreType));
}

TEST_P(AbilityScoreDrainTests, AddUpdatesTotalAbilityScore)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    EXPECT_EQ(10, abilityScores.getTotalScore(abilityScoreType));

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        6,
        "drain1description"
    );

    EXPECT_EQ(4, abilityScores.getTotalScore(abilityScoreType));
}

TEST_P(AbilityScoreDrainTests, AddUpdatesTotalAbilityScoreModifier)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 14);

    EXPECT_EQ(2, abilityScores.getTotalAbilityModifier(abilityScoreType));

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        6,
        "drain1description"
    );

    EXPECT_EQ(-1, abilityScores.getTotalAbilityModifier(abilityScoreType));
}

TEST_P(AbilityScoreDrainTests, AddUpdatesBaseScoreWithPermanentAdjustments)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 10);

    EXPECT_EQ(10, abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType));

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        6,
        "drain1description"
    );

    EXPECT_EQ(4, abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType));
}

TEST_P(AbilityScoreDrainTests, AddUpdatesBaseModifierWithPermanentAdjustments)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 14);

    EXPECT_EQ(2, abilityScores.getBaseModifierWithPermanentAdjustments(abilityScoreType));

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        6,
        "drain1description"
    );

    EXPECT_EQ(-1, abilityScores.getBaseModifierWithPermanentAdjustments(abilityScoreType));
}

TEST_P(AbilityScoreDrainTests, AbilityDrainAtOrBelowZeroChangesCharacterStatus)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 12);

    EXPECT_EQ(CharacterStatus::Normal, characterDescription.getCharacterStatus());

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        12,
        "drain1description"
    );

    CharacterStatus characterTestStatus;

    //Some special setup for test-run dependent ability score type
    if (abilityScoreType == AbilityScoreTypes::STR || abilityScoreType == AbilityScoreTypes::WIS || abilityScoreType == AbilityScoreTypes::CHA) {
        characterTestStatus = CharacterStatus::Unconscious;
    } else if (abilityScoreType == AbilityScoreTypes::DEX) {
        characterTestStatus = CharacterStatus::Immobile;
    } else if (abilityScoreType == AbilityScoreTypes::INT) {
        characterTestStatus = CharacterStatus::Comatose;
    } else {
        //AbilityScoreTypes::CON
        characterTestStatus = CharacterStatus::Dead;
    }

    EXPECT_EQ(characterTestStatus, characterDescription.getCharacterStatus());
}

TEST_P(AbilityScoreDrainTests, DoesSourceExistReturnsTrueIfFound)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        12,
        "drain1description"
    );

    EXPECT_EQ(true, abilityScores.doesAbilityScoreDrainSourceExist(abilityScoreType, "drain1"));
}

TEST_P(AbilityScoreDrainTests, DoesSourceExistReturnsFalseIfNotFound)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        12,
        "drain1description"
    );

    abilityScores.removeAbilityScoreDrain(abilityScoreType, "drain1");

    EXPECT_EQ(false, abilityScores.doesAbilityScoreDrainSourceExist(abilityScoreType, "drain1"));
}

TEST_P(AbilityScoreDrainTests, ToggleAbilityScoreDrain)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        12,
        "drain1description"
    );

    auto testDrain = abilityScores.getAbilityDrain(abilityScoreType, "drain1");

    EXPECT_EQ(true, testDrain.enabled);

    abilityScores.toggleAbilityScoreDrain(abilityScoreType, "drain1");
    testDrain = abilityScores.getAbilityDrain(abilityScoreType, "drain1");

    EXPECT_EQ(false, testDrain.enabled);
}

TEST_P(AbilityScoreDrainTests, RemoveUpdatesTotalAbilityScoreDrain)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 15);

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        12,
        "drain1description"
    );

    EXPECT_EQ(3, abilityScores.getTotalScore(abilityScoreType));

    abilityScores.removeAbilityScoreDrain(abilityScoreType, "drain1");

    EXPECT_EQ(15, abilityScores.getTotalScore(abilityScoreType));
}

TEST_P(AbilityScoreDrainTests, ToggleUpdatesTotalAbilityScoreDrain)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 15);

    abilityScores.addAbilityScoreDrain(
        abilityScoreType,
        "drain1",
        12,
        "drain1description"
    );

    EXPECT_EQ(3, abilityScores.getTotalScore(abilityScoreType));

    abilityScores.toggleAbilityScoreDrain(abilityScoreType, "drain1");

    EXPECT_EQ(15, abilityScores.getTotalScore(abilityScoreType));

    abilityScores.toggleAbilityScoreDrain(abilityScoreType, "drain1");

    EXPECT_EQ(3, abilityScores.getTotalScore(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    AbilityScoreDrainTests, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);

class ComprehensiveAbilityScoreTest : public ::testing::TestWithParam<AbilityScoreTypes>
{
    //Fixture for comprehensive test case
};

TEST_P(ComprehensiveAbilityScoreTest, TestModifiersOfSeveralTypesThatCouldActuallyBeOnACharacterSheet)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    auto abilityScoreType = GetParam();

    abilityScores.setBaseAbilityScore(abilityScoreType, 18);
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Inherent, abilityScoreType, "temp1", 2, "temp1desc");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Inherent, abilityScoreType, "temp2", 3, "temp2desc");
    abilityScores.addTemporaryAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "temp3", 4, "temp3desc");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Alchemical, abilityScoreType, "perm1", 4, "perm1desc");
    abilityScores.addPermanentAbilityScoreBonus(AbilityScoreModifiers::Untyped, abilityScoreType, "perm2", 1, "perm2desc");
    abilityScores.addAbilityScoreDamage(abilityScoreType, "damage1", 3, "damage1desc");
    abilityScores.addAbilityScoreDamage(abilityScoreType, "damage2", 3, "damage2desc");
    abilityScores.addAbilityScoreDamage(abilityScoreType, "damage2", 5, "damage2desc");
    abilityScores.addAbilityScoreDrain(abilityScoreType, "drain1", 2, "drain1desc");
    abilityScores.addAbilityScoreDrain(abilityScoreType, "drain1", 1, "drain1desc");
    abilityScores.addAbilityScorePenalty(abilityScoreType, "penalty1", 1, "penalty1desc");
    abilityScores.addAbilityScorePenalty(abilityScoreType, "penalty2", 2, "penalty2desc");

    abilityScores.toggleTemporaryAbilityScoreBonus(abilityScoreType, "temp2");

    EXPECT_EQ(CharacterStatus::Normal, characterDescription.getCharacterStatus());
    EXPECT_EQ(18, abilityScores.getBaseAbilityScore(abilityScoreType));
    EXPECT_EQ(4, abilityScores.getBaseModifier(abilityScoreType));
    EXPECT_EQ(22, abilityScores.getBaseScoreWithPermanentAdjustments(abilityScoreType));
    EXPECT_EQ(6, abilityScores.getBaseModifierWithPermanentAdjustments(abilityScoreType));
    EXPECT_EQ(28, abilityScores.getTotalScore(abilityScoreType));
    EXPECT_EQ(9, abilityScores.getTotalAbilityModifier(abilityScoreType));
}

INSTANTIATE_TEST_CASE_P(
    AbilityScoreTypeFixture, //Fixture class instantiation name
    ComprehensiveAbilityScoreTest, //Fixture class/Test case name
    ::testing::ValuesIn(listOfPossibleAbilityScores) //Actual value set to pass to the test run
);
