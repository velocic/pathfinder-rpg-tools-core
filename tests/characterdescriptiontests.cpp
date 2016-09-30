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

TEST(NegativeLevels, RemoveTemporaryNegativeLevelsTogglesMultipleDebuffsDisabled)
{
    CharacterDescription characterDescription;
    
    characterDescription.addTemporaryNegativeLevelDebuff("Temp1", 2, "desc1");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp2", 3, "desc1");
    characterDescription.addTemporaryNegativeLevelDebuff("Temp3", 1, "desc1");

    EXPECT_EQ(6, characterDescription.getTotalNegativeLevels());

    characterDescription.removeTemporaryNegativeLevels(5);

    EXPECT_EQ(1, characterDescription.getTotalNegativeLevels());

    auto debuff1 = characterDescription.getTemporaryNegativeLevelDebuff("Temp1");
    auto debuff2 = characterDescription.getTemporaryNegativeLevelDebuff("Temp2");
    auto debuff3 = characterDescription.getTemporaryNegativeLevelDebuff("Temp3");

    //This block may not be easily testable. Passes at time of writing, but depends on
    //the order elements are iterated through in an UNORDERED map. Useful for now to
    //tell that the proper number of nodes are disabled at time of development, though.
    EXPECT_EQ(1, debuff1.numNegativeLevels);
    EXPECT_EQ(true, debuff1.enabled);

    EXPECT_EQ(0, debuff2.numNegativeLevels);
    EXPECT_EQ(false, debuff2.enabled);

    EXPECT_EQ(0, debuff3.numNegativeLevels);
    EXPECT_EQ(false, debuff3.enabled);
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsWithRemainingCountAboveZero)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsWithRemainingCountExactlyZero)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsInExcessOfRemainingCount)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, RemovePermanentNegativeLevelsTogglesMultipleDebuffsDisabled)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, ToggleTemporaryNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, TogglePermanentNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, GetTemporaryNegativeLevelDebuffReturnsProperDebuffEntry)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, GetPermanentNegativeLevelDebuffReturnsProperDebuffEntry)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, GetTemporaryNegativeLevelDebuffsReturnsEntireCollectionProperly)
{
    throw std::logic_error("Unimplemented");
}

TEST(NegativeLevels, GetPermanentNegativeLevelDebuffsReturnsEntireCollectionProperly)
{
    throw std::logic_error("Unimplemented");
}
