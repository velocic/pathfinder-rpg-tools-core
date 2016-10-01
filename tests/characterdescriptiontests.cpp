#include <testcommon.h>
#include <characterdescription.h>

using namespace RulesEngine::Character;

TEST(NegativeLevels, AddTemporaryNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    CharacterDescription characterDescription;

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());

    characterDescription.addTemporaryNegativeLevelDebuff(
        "Dummy Debuff",
        3,
        "Dummy description"
    );

    EXPECT_EQ(3, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, AddPermanentNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    CharacterDescription characterDescription;

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());

    characterDescription.addPermanentNegativeLevelDebuff(
        "Dummy Permanent Debuff",
        5,
        "Dummy permanent description"
    );

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, AddTemporaryNegativeLevelDebuffOverwritesEntryWithSameName)
{
    CharacterDescription characterDescription;

    characterDescription.addTemporaryNegativeLevelDebuff(
        "Temp Debuff",
        2,
        "Desc."
    );

    EXPECT_EQ(2, characterDescription.getTotalNegativeLevels());

    characterDescription.addTemporaryNegativeLevelDebuff(
        "Temp Debuff",
        3,
        "Desc."
    );

    auto& tempNegLevelMap = characterDescription.getTemporaryNegativeLevelDebuffs();

    EXPECT_EQ(1, tempNegLevelMap.size());
    EXPECT_EQ(3, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, AddPermanentNegativeLevelDebuffOverwritesEntryWithSameName)
{
    CharacterDescription characterDescription;

    characterDescription.addPermanentNegativeLevelDebuff(
        "Perm Debuff",
        4,
        "desc."
    );

    EXPECT_EQ(4, characterDescription.getTotalNegativeLevels());

    characterDescription.addPermanentNegativeLevelDebuff(
        "Perm Debuff",
        1,
        "desc."
    );

    auto& permNegLevelMap = characterDescription.getPermanentNegativeLevelDebuffs();

    EXPECT_EQ(1, permNegLevelMap.size());
    EXPECT_EQ(1, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, AddTemporaryNegativeLevelsUpdatesTotalNegativeLevelCount)
{
    CharacterDescription characterDescription;
    characterDescription.addTemporaryNegativeLevelDebuff(
        "Dummy Temp Debuff",
        0,
        "Dummy temp description"
    );

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());

    characterDescription.addTemporaryNegativeLevels("Dummy Temp Debuff", 7);

    EXPECT_EQ(7, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, AddPermanentNegativeLevelsUpdatesTotalNegativeLevelCount)
{
    CharacterDescription characterDescription;
    characterDescription.addPermanentNegativeLevelDebuff(
        "Dummy Perm Debuff",
        0,
        "Dummy Perm Desc."
    );
    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());

    characterDescription.addPermanentNegativeLevels("Dummy Perm Debuff", 1);

    EXPECT_EQ(1, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, RemoveTemporaryNegativeLevelsWithRemainingCountAboveZero)
{
    CharacterDescription characterDescription;
    characterDescription.addTemporaryNegativeLevelDebuff(
        "TempDebuff",
        9,
        "Desc."
    );

    EXPECT_EQ(9, characterDescription.getTotalNegativeLevels());

    characterDescription.removeTemporaryNegativeLevels(4);

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());
    EXPECT_EQ(true, characterDescription.getTemporaryNegativeLevelDebuff("TempDebuff").enabled);
}

TEST(NegativeLevels, RemoveTemporaryNegativeLevelsWithRemainingCountExactlyZero)
{
    CharacterDescription characterDescription;
    characterDescription.addTemporaryNegativeLevelDebuff(
        "TempDebuff",
        9,
        "Desc."
    );

    EXPECT_EQ(9, characterDescription.getTotalNegativeLevels());

    characterDescription.removeTemporaryNegativeLevels(9);

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());
    EXPECT_EQ(false, characterDescription.getTemporaryNegativeLevelDebuff("TempDebuff").enabled);
}

TEST(NegativeLevels, RemoveTemporaryNegativeLevelsInExcessOfRemainingCount)
{
    CharacterDescription characterDescription;
    characterDescription.addTemporaryNegativeLevelDebuff(
        "TempDebuff",
        9,
        "Desc."
    );

    EXPECT_EQ(9, characterDescription.getTotalNegativeLevels());

    characterDescription.removeTemporaryNegativeLevels(25);

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());
    EXPECT_EQ(false, characterDescription.getTemporaryNegativeLevelDebuff("TempDebuff").enabled);
}

TEST(NegativeLevels, RemoveTemporaryNegativeLevelsCalculatesTotalCorrectlyWithMultipleDebuffs)
{
    CharacterDescription characterDescription;
    
    characterDescription.addTemporaryNegativeLevelDebuff("Temp1", 2, "desc1");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp2", 3, "desc1");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp3", 1, "desc1");

    EXPECT_EQ(6, characterDescription.getTotalNegativeLevels());

    characterDescription.removeTemporaryNegativeLevels(5);

    EXPECT_EQ(1, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsWithRemainingCountAboveZero)
{
    CharacterDescription characterDescription;
    characterDescription.addPermanentNegativeLevelDebuff(
        "PermDebuff",
        5,
        "desc."
    );

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());

    characterDescription.removePermanentNegativeLevels(3);

    EXPECT_EQ(2, characterDescription.getTotalNegativeLevels());
    EXPECT_EQ(true, characterDescription.getPermanentNegativeLevelDebuff("PermDebuff").enabled);
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsWithRemainingCountExactlyZero)
{
    CharacterDescription characterDescription;
    characterDescription.addPermanentNegativeLevelDebuff(
        "permDebuff",
        5,
        "desc"
    );

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());

    characterDescription.removePermanentNegativeLevels(5);

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());
    EXPECT_EQ(false, characterDescription.getPermanentNegativeLevelDebuff("permDebuff").enabled);
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsInExcessOfRemainingCount)
{
    CharacterDescription characterDescription;
    characterDescription.addPermanentNegativeLevelDebuff(
        "permDebuff",
        5,
        "desc"
    );

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());

    characterDescription.removePermanentNegativeLevels(12);

    EXPECT_EQ(0, characterDescription.getTotalNegativeLevels());
    EXPECT_EQ(false, characterDescription.getPermanentNegativeLevelDebuff("permDebuff").enabled);
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsCalculatesTotalCorrectlyWithMultipleDebuffs)
{
    CharacterDescription characterDescription;

    characterDescription.addPermanentNegativeLevelDebuff("Perm1", 1, "Desc1");
    characterDescription.addPermanentNegativeLevelDebuff("Perm2", 3, "Desc2");
    characterDescription.addPermanentNegativeLevelDebuff("Perm3", 2, "Desc3");
    characterDescription.addPermanentNegativeLevelDebuff("Perm4", 4, "Desc4");
    characterDescription.addPermanentNegativeLevelDebuff("Perm5", 2, "Desc5");

    EXPECT_EQ(12, characterDescription.getTotalNegativeLevels());

    characterDescription.removePermanentNegativeLevels(7);

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, RemoveTemporaryAndPermanentNegativeLevelsDontInterop)
{
    CharacterDescription characterDescription;
    characterDescription.addTemporaryNegativeLevelDebuff("Temp1", 3, "TempDesc");
    characterDescription.addPermanentNegativeLevelDebuff("Perm1", 4, "PermDesc");

    EXPECT_EQ(7, characterDescription.getTotalNegativeLevels());

    characterDescription.removeTemporaryNegativeLevels(1);
    EXPECT_EQ(6, characterDescription.getTotalNegativeLevels());

    characterDescription.removePermanentNegativeLevels(2);
    EXPECT_EQ(4, characterDescription.getTotalNegativeLevels());

    //Should find no matching object, so compare result name against empty string
    EXPECT_EQ("", characterDescription.getTemporaryNegativeLevelDebuff("Perm1").sourceName);
    EXPECT_EQ("", characterDescription.getPermanentNegativeLevelDebuff("Temp1").sourceName);

    //These values shouldn't change, so test that they're still enabled
    characterDescription.toggleTemporaryNegativeLevelDebuff("Perm1");
    characterDescription.togglePermanentNegativeLevelDebuff("Temp1");

    EXPECT_EQ(true, characterDescription.getTemporaryNegativeLevelDebuff("Temp1").enabled);
    EXPECT_EQ(true, characterDescription.getPermanentNegativeLevelDebuff("Perm1").enabled);
}

TEST(NegativeLevels, ToggleTemporaryNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    CharacterDescription characterDescription;

    characterDescription.addTemporaryNegativeLevelDebuff("Temp1", 4, "TempDesc");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp2", 2, "TempDesc");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp3", 1, "TempDesc");

    EXPECT_EQ(7, characterDescription.getTotalNegativeLevels());

    characterDescription.toggleTemporaryNegativeLevelDebuff("Temp2");

    EXPECT_EQ(5, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, TogglePermanentNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    CharacterDescription characterDescription;

    characterDescription.addPermanentNegativeLevelDebuff("Perm1", 3, "PermDesc");
    characterDescription.addPermanentNegativeLevelDebuff("Perm2", 5, "PermDesc");
    characterDescription.addPermanentNegativeLevelDebuff("Perm3", 1, "PermDesc");

    EXPECT_EQ(9, characterDescription.getTotalNegativeLevels());

    characterDescription.togglePermanentNegativeLevelDebuff("Perm1");

    EXPECT_EQ(6, characterDescription.getTotalNegativeLevels());
}

TEST(NegativeLevels, GetTemporaryNegativeLevelDebuffReturnsProperDebuffEntry)
{
    CharacterDescription characterDescription;

    characterDescription.addTemporaryNegativeLevelDebuff("Temp1", 8, "Desc1");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp2", 3, "Desc2");

    auto debuff = characterDescription.getTemporaryNegativeLevelDebuff("Temp1");

    EXPECT_EQ("Temp1", debuff.sourceName);
    EXPECT_EQ(8, debuff.numNegativeLevels);
    EXPECT_EQ("Desc1", debuff.description);
}

TEST(NegativeLevels, GetPermanentNegativeLevelDebuffReturnsProperDebuffEntry)
{
    CharacterDescription characterDescription;

    characterDescription.addPermanentNegativeLevelDebuff("Perm1", 8, "Desc1");
    characterDescription.addPermanentNegativeLevelDebuff("Perm2", 3, "Desc2");

    auto debuff = characterDescription.getPermanentNegativeLevelDebuff("Perm1");

    EXPECT_EQ("Perm1", debuff.sourceName);
    EXPECT_EQ(8, debuff.numNegativeLevels);
    EXPECT_EQ("Desc1", debuff.description);
}
