#include <testcommon.h>
#include <baseattackbonus.h>
#include <characterdescription.h>

using namespace RulesEngine::Character;

TEST(BaseAttackBonus, CalculatesFullBABClassCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("fighter");
    charDescription.setClass("fighter", 1, 10, 2, 1);

    EXPECT_EQ(1, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("fighter", 2);
    EXPECT_EQ(2, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("fighter", 3);
    EXPECT_EQ(3, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("fighter", 4);
    EXPECT_EQ(4, bab.getTotalBaseAttackBonus());
}

TEST(BaseAttackBonus, CalculatesThreeFourthBABClassCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("rogue");
    charDescription.setClass("rogue", 1, 8, 8, .75);

    EXPECT_EQ(0, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("rogue", 2);
    EXPECT_EQ(1, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("rogue", 3);
    EXPECT_EQ(2, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("rogue", 4);
    EXPECT_EQ(3, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("rogue", 5);
    EXPECT_EQ(3, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("rogue", 6);
    EXPECT_EQ(4, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("rogue", 7);
    EXPECT_EQ(5, bab.getTotalBaseAttackBonus());
}

TEST(BaseAttackBonus, CalculatesHalfBABClassCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("wizard");
    charDescription.setClass("wizard", 1, 6, 2, .5);

    EXPECT_EQ(0, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 2);
    EXPECT_EQ(1, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 3);
    EXPECT_EQ(1, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 4);
    EXPECT_EQ(2, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 5);
    EXPECT_EQ(2, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 6);
    EXPECT_EQ(3, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 7);
    EXPECT_EQ(3, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 8);
    EXPECT_EQ(4, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 9);
    EXPECT_EQ(4, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 10);
    EXPECT_EQ(5, bab.getTotalBaseAttackBonus());

    charDescription.setClassLevel("wizard", 11);
    EXPECT_EQ(5, bab.getTotalBaseAttackBonus());
}

TEST(BaseAttackBonus, CalculatesFullBABClassIterativeBonusesCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("fighter");
    charDescription.setClass("fighter", 6, 10, 2, 1);

    auto iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(2, iterativeBonusList.size());
    EXPECT_EQ(6, iterativeBonusList[0]);
    EXPECT_EQ(1, iterativeBonusList[1]);

    charDescription.setClassLevel("fighter", 11);
    iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(3, iterativeBonusList.size());
    EXPECT_EQ(11, iterativeBonusList[0]);
    EXPECT_EQ(6, iterativeBonusList[1]);
    EXPECT_EQ(1, iterativeBonusList[2]);

    charDescription.setClassLevel("fighter", 16);
    iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(4, iterativeBonusList.size());
    EXPECT_EQ(16, iterativeBonusList[0]);
    EXPECT_EQ(11, iterativeBonusList[1]);
    EXPECT_EQ(6, iterativeBonusList[2]);
    EXPECT_EQ(1, iterativeBonusList[3]);

    charDescription.setClassLevel("fighter", 20);
    iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(4, iterativeBonusList.size());
    EXPECT_EQ(20, iterativeBonusList[0]);
    EXPECT_EQ(15, iterativeBonusList[1]);
    EXPECT_EQ(10, iterativeBonusList[2]);
    EXPECT_EQ(5, iterativeBonusList[3]);
}

TEST(BaseAttackBonus, CalculatesThreeFourthBABClassIterativeBonusesCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("rogue");
    charDescription.setClass("rogue", 8, 8, 8, .75);

    auto iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(2, iterativeBonusList.size());
    EXPECT_EQ(6, iterativeBonusList[0]);
    EXPECT_EQ(1, iterativeBonusList[1]);

    charDescription.setClassLevel("rogue", 15);
    iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(3, iterativeBonusList.size());
    EXPECT_EQ(11, iterativeBonusList[0]);
    EXPECT_EQ(6, iterativeBonusList[1]);
    EXPECT_EQ(1, iterativeBonusList[2]);

    charDescription.setClassLevel("rogue", 20);
    iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(3, iterativeBonusList.size());
    EXPECT_EQ(15, iterativeBonusList[0]);
    EXPECT_EQ(10, iterativeBonusList[1]);
    EXPECT_EQ(5, iterativeBonusList[2]);
}

TEST(BaseAttackBonus, CalculatesHalfBABClassIterativeBonusesCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("wizard", 12, 6, 2, .5);

    auto iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(2, iterativeBonusList.size());
    EXPECT_EQ(6, iterativeBonusList[0]);
    EXPECT_EQ(1, iterativeBonusList[1]);

    charDescription.setClassLevel("wizard", 20);
    iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(2, iterativeBonusList.size());
    EXPECT_EQ(10, iterativeBonusList[0]);
    EXPECT_EQ(5, iterativeBonusList[1]);
}

TEST(BaseAttackBonus, CalculatesComplexMulticlassBABCorrectly)
{
    BaseAttackBonus bab;

    CharacterDescription charDescription;
    charDescription.registerObserver("baseAttackBonus", &bab);

    charDescription.addClass("wizard", 12, 6, 2, .5);
    charDescription.addClass("rogue", 5, 8, 8, .75);
    charDescription.addClass("fighter", 3, 10, 2, 1);

    //wizard BAB - 6
    //rogue BAB - 3
    //fighter BAB - 3
    //TOTAL BAB - 12
    //ITERATIVES - 12/7/2

    EXPECT_EQ(12, bab.getTotalBaseAttackBonus());

    auto iterativeBonusList = bab.getIterativeAttackBonuses();

    ASSERT_EQ(3, iterativeBonusList.size());
    EXPECT_EQ(12, iterativeBonusList[0]);
    EXPECT_EQ(7, iterativeBonusList[1]);
    EXPECT_EQ(2, iterativeBonusList[2]);
}
