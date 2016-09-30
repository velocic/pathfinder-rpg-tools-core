#include <testcommon.h>
#include <characterdescription.h>

using namespace RulesEngine::Character;

TEST(CharacterDescription, AddTemporaryNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
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

TEST(CharacterDescription, AddPermanentNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
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

TEST(CharacterDescription, AddTemporaryNegativeLevelsUpdatesTotalNegativeLevelCount)
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

TEST(CharacterDescription, AddPermanentNegativeLevelsUpdatesTotalNegativeLevelCount)
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

TEST(CharacterDescription, RemoveTemporaryNegativeLevelsUpdatesTotalNegativeLevelCount)
{
    //Test cases: count > 0, count reduced to exactly 0, try to reduce to a negative number
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, RemovePermanentNegativeLevelsUpdatesTotalNegativeLevelCount)
{
    //Test cases: count > 0, count reduced to exactly 0, try to reduce to a negative number
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, ToggleTemporaryNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, TogglePermanentNegativeLevelDebuffUpdatesTotalNegativeLevelCount)
{
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, GetTemporaryNegativeLevelDebuffReturnsProperDebuffEntry)
{
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, GetPermanentNegativeLevelDebuffReturnsProperDebuffEntry)
{
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, GetTemporaryNegativeLevelDebuffsReturnsEntireCollectionProperly)
{
    throw std::logic_error("Unimplemented");
}

TEST(CharacterDescription, GetPermanentNegativeLevelDebuffsReturnsEntireCollectionProperly)
{
    throw std::logic_error("Unimplemented");
}
