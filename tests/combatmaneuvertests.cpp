#include <testcommon.h>
#include <characterdescription.h>
#include <abilityscores.h>
#include <baseattackbonus.h>
#include <combatmaneuverbonus.h>

using namespace RulesEngine::Character;

TEST(CombatManeuverBonus, BABFactoredCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    BaseAttackBonus baseAttackBonus;
    characterDescription.registerObserver("baseAttackBonus", &baseAttackBonus);
    CombatManeuverBonus combatManeuvers(characterDescription, abilityScores, baseAttackBonus);

    characterDescription.addClass("fighter", 5, 10, 2, 1);
    abilityScores.setBaseAbilityScore(AbilityScoreTypes::STR, 10);
    characterDescription.setSizeCategory(SizeCategories::Medium);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 5);

    characterDescription.addClass("rogue", 5, 8, 6, .75);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 8);

    characterDescription.addClass("wizard", 5, 6, 4, .5);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 10);
}

TEST(CombatManeuverBonus, STRFactoredCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    BaseAttackBonus baseAttackBonus;
    characterDescription.registerObserver("baseAttackBonus", &baseAttackBonus);
    CombatManeuverBonus combatManeuvers(characterDescription, abilityScores, baseAttackBonus);

    abilityScores.setBaseAbilityScore(AbilityScoreTypes::STR, 18);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 4);

    abilityScores.setBaseAbilityScore(AbilityScoreTypes::STR, 6);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), -2);
}

TEST(CombatManeuverBonus, SizeFactoredCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    BaseAttackBonus baseAttackBonus;
    characterDescription.registerObserver("baseAttackBonus", &baseAttackBonus);
    CombatManeuverBonus combatManeuvers(characterDescription, abilityScores, baseAttackBonus);

    characterDescription.setSizeCategory(SizeCategories::Fine);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), -8);

    characterDescription.setSizeCategory(SizeCategories::Medium);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 0);

    characterDescription.setSizeCategory(SizeCategories::Colossal);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 8);
}

TEST(CombatManeuverBonus, ModifiersFactoredCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    BaseAttackBonus baseAttackBonus;
    characterDescription.registerObserver("baseAttackBonus", &baseAttackBonus);
    CombatManeuverBonus combatManeuvers(characterDescription, abilityScores, baseAttackBonus);

    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Circumstance, "Stuff1", "SomeDescrip", 2);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 2);

    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Circumstance, "Stuff2", "SomeDescrip", -3);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), -1);

    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Enhancement, "Stuff3", "SomeDescrip", 4);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 3);

    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Enhancement, "Stuff4", "SomeDescrip", 2);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 3);
}

TEST(CombatManeuverBonus, ComplexCharacterDerivedCorrectly)
{
    CharacterDescription characterDescription;
    AbilityScores abilityScores(characterDescription);
    BaseAttackBonus baseAttackBonus;
    characterDescription.registerObserver("baseAttackBonus", &baseAttackBonus);
    CombatManeuverBonus combatManeuvers(characterDescription, abilityScores, baseAttackBonus);

    characterDescription.addClass("fighter", 5, 10, 2, 1);
    characterDescription.addClass("rogue", 5, 8, 6, .75);
    characterDescription.addClass("wizard", 5, 6, 4, .5);

    abilityScores.setBaseAbilityScore(AbilityScoreTypes::STR, 18);

    characterDescription.setSizeCategory(SizeCategories::Small);

    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Circumstance, "Stuff1", "SomeDescrip", 2);
    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Circumstance, "Stuff2", "SomeDescrip", -3);
    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Enhancement, "Stuff3", "SomeDescrip", 4);
    combatManeuvers.addCMBModifier(CombatManeuverModifierType::Enhancement, "Stuff4", "SomeDescrip", 2);

    EXPECT_EQ(combatManeuvers.getCombatManeuverBonus(), 16);
}
