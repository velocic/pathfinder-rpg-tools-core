#include <testcommon.h>
#include <abilityscores.h>
#include <characterdescription.h>
#include <hitpoints.h>

using namespace RulesEngine::Character;

TEST(HitPointGeneration, AllDieRollTotalsBetweenOneAndHDMaxWithConModifierZero)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::CON, 10); //+0 modifier
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    //Ridiculous level 1000 character so we can be sure we basically roll all hit die
    //amounts possible
    characterDescription.addClass("Fighter", 1000, 10, 2, 1);

    auto& dieRollsByClass = hitPoints.getHpDieRollsByClass();

    for (auto& charClassDieRollPair : dieRollsByClass) {
        for (auto& dieRoll : charClassDieRollPair.second) {
            EXPECT_GE(dieRoll, 1);
            EXPECT_LE(dieRoll, 10);
        }
    }
}

TEST(HitPointGeneration, ProperNumberOfDieRollsCollectedWithSingleClass)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::CON, 10); //+0 modifier
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    characterDescription.addClass("Fighter", 12, 10, 2, 1);

    auto& dieRollsByClass = hitPoints.getHpDieRollsByClass();
    EXPECT_EQ(12, dieRollsByClass.find("Fighter")->second.size());
}

TEST(HitPointGeneration, ProperNumberOfDieRollsCollectedWithMultipleClasses)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::CON, 10); //+0 modifier
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    characterDescription.addClass("Fighter", 12, 10, 2, 1);
    characterDescription.addClass("Wizard", 4, 6, 4, .5);
    characterDescription.addClass("Rogue", 1, 8, 6, .75);

    auto& dieRollsByClass = hitPoints.getHpDieRollsByClass();
    EXPECT_EQ(12, dieRollsByClass.find("Fighter")->second.size());
    EXPECT_EQ(4, dieRollsByClass.find("Wizard")->second.size());
    EXPECT_EQ(1, dieRollsByClass.find("Rogue")->second.size());
}

TEST(HitPointGeneration, ProperNumberOfDieRollsRetainedAfterClassDeletion)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::CON, 10); //+0 modifier
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    characterDescription.addClass("Fighter", 12, 10, 2, 1);
    characterDescription.addClass("Wizard", 4, 6, 4, .5);
    characterDescription.addClass("Rogue", 1, 8, 6, .75);

    characterDescription.removeClass("Fighter");

    auto& dieRollsByClass = hitPoints.getHpDieRollsByClass();
    EXPECT_EQ(4, dieRollsByClass.find("Wizard")->second.size());
    EXPECT_EQ(1, dieRollsByClass.find("Rogue")->second.size());
}

TEST(HitPointGeneration, ProperNumberOfDieRollsRetainedAfterClassLevelReduced)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::CON, 10); //+0 modifier
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    characterDescription.addClass("Fighter", 12, 10, 2, 1);
    characterDescription.addClass("Wizard", 4, 6, 4, .5);
    characterDescription.addClass("Rogue", 1, 8, 6, .75);

    characterDescription.setClassLevel("Fighter", 5);

    auto& dieRollsByClass = hitPoints.getHpDieRollsByClass();
    EXPECT_EQ(5, dieRollsByClass.find("Fighter")->second.size());
    EXPECT_EQ(4, dieRollsByClass.find("Wizard")->second.size());
    EXPECT_EQ(1, dieRollsByClass.find("Rogue")->second.size());
}

TEST(HitPointGeneration, ProperNumberOfDieRollsAddedAfterClassLevelIncrease)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::CON, 10); //+0 modifier
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    characterDescription.addClass("Fighter", 12, 10, 2, 1);
    characterDescription.addClass("Wizard", 4, 6, 4, .5);
    characterDescription.addClass("Rogue", 1, 8, 6, .75);

    characterDescription.setClassLevel("Rogue", 3);

    auto& dieRollsByClass = hitPoints.getHpDieRollsByClass();
    EXPECT_EQ(12, dieRollsByClass.find("Fighter")->second.size());
    EXPECT_EQ(4, dieRollsByClass.find("Wizard")->second.size());
    EXPECT_EQ(3, dieRollsByClass.find("Rogue")->second.size());
}
