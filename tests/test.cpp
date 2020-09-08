#include "gtest/gtest.h"
#include "stdlib.h"
#include "../cparser/inc/cparser.h"

/* Private function prototypes ---------------------------------------------*/
static void versionTrgCallback(Dictionary_t *params);
static void pumpTrgCallback(Dictionary_t *params);
static void moveTrgCallback(Dictionary_t *params);
static void delayTrgCallback(Dictionary_t *params);

/* Private variables -------------------------------------------------------*/
Cp_Trigger_t TriggerTable[] =
    {
        {"delay",
         {
             {.letter = 'T', .type = CP_PARAM_TYPE_REAL},
         },
         delayTrgCallback,
         1},
        {"versi",
         {
             {.letter = 'V', .type = CP_PARAM_TYPE_INTEGER},
         },
         versionTrgCallback,
         1},
        {"move",
         {{.letter = 'D', .type = CP_PARAM_TYPE_REAL},
          {.letter = 'S', .type = CP_PARAM_TYPE_REAL}},
         moveTrgCallback,
         2},
        {"pump",
         {
             {.letter = 'M', .type = CP_PARAM_TYPE_LETTER},
             {.letter = 'V', .type = CP_PARAM_TYPE_REAL},
         },
         pumpTrgCallback,
         2},
};

static Bool_t versionTrgTriggered;
static int32_t versionTrgVersion;
static Cp_ParamType_t versionTrgVersionType;

static Bool_t delayTrgTriggered;
static float delayTrgTime;
static Cp_ParamType_t delayTrgTimeType;

static Bool_t moveTrgTriggered;
static float moveTrgDisplacement;
static float moveTrgSpeed;
static Cp_ParamType_t moveTrgDisplacementType;
static Cp_ParamType_t moveTrgSpeedType;

static Bool_t pumpTrgTriggered;
static float pumpTrgVolume;
static char pumpTrgMode;
static Cp_ParamType_t pumpTrgVolumeType;
static Cp_ParamType_t pumpTrgModeType;

struct CparserTest : public ::testing::Test
{
public:
    virtual void SetUp() override
    {
        versionTrgTriggered = FALSE;
        versionTrgVersion = 0;

        delayTrgTriggered = FALSE;
        delayTrgTime = 0.0f;

        moveTrgTriggered = FALSE;
        moveTrgDisplacement = 0.0f;
        moveTrgSpeed = 0.0f;

        pumpTrgTriggered = FALSE;
        pumpTrgVolume = 0.0f;
        pumpTrgMode = ' ';

        // Register command table.
        Cp_Register(TriggerTable, (sizeof(TriggerTable) / sizeof(TriggerTable[0])));
    }

    virtual void TearDown() override
    {
        Cp_Reset();
    }
};

TEST_F(CparserTest, DelayTest)
{
    // Feed commands.
    char line[] = "delay T1E2";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(delayTrgTriggered, TRUE);
    EXPECT_FLOAT_EQ(delayTrgTime, 100.0f);
    EXPECT_EQ(delayTrgTimeType, CP_PARAM_TYPE_REAL);
}

TEST_F(CparserTest, VersionTest)
{
    // Feed command.
    char line[] = "versi V10";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(versionTrgTriggered, TRUE);
    EXPECT_EQ(versionTrgVersion, 10);
    EXPECT_EQ(versionTrgVersionType, CP_PARAM_TYPE_INTEGER);
}

TEST_F(CparserTest, MoveTest)
{
    // Feed commands.
    char line[] = "move D1.0 S5.0";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(moveTrgTriggered, TRUE);
    EXPECT_FLOAT_EQ(moveTrgDisplacement, 1.0f);
    EXPECT_FLOAT_EQ(moveTrgSpeed, 5.0f);
    EXPECT_EQ(moveTrgDisplacementType, CP_PARAM_TYPE_REAL);
    EXPECT_EQ(moveTrgSpeedType, CP_PARAM_TYPE_REAL);
}

TEST_F(CparserTest, PumpTest)
{
    // Feed commands.
    char line[] = "pump V1.25 MD";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(pumpTrgTriggered, TRUE);
    EXPECT_FLOAT_EQ(pumpTrgVolume, 1.25);
    EXPECT_EQ('D', pumpTrgMode);
    EXPECT_EQ(pumpTrgVolumeType, CP_PARAM_TYPE_REAL);
    EXPECT_EQ(pumpTrgModeType, CP_PARAM_TYPE_LETTER);
}

void versionTrgCallback(Dictionary_t *params)
{
    versionTrgTriggered = TRUE;
    versionTrgVersion = *((int32_t *)Dictionary_Get(params, 'V',
                                                    &versionTrgVersionType));
}

void pumpTrgCallback(Dictionary_t *params)
{
    pumpTrgTriggered = TRUE;
    pumpTrgVolume = *((float *)Dictionary_Get(params, 'V',
                                              &pumpTrgVolumeType));
    pumpTrgMode = *((char *)Dictionary_Get(params, 'M',
                                           &pumpTrgModeType));
}

void moveTrgCallback(Dictionary_t *params)
{
    moveTrgTriggered = TRUE;
    moveTrgDisplacement = *((float *)Dictionary_Get(params, 'D',
                                                    &moveTrgDisplacementType));
    moveTrgSpeed = *((float *)Dictionary_Get(params, 'S',
                                             &moveTrgSpeedType));
}

void delayTrgCallback(Dictionary_t *params)
{
    delayTrgTriggered = TRUE;
    delayTrgTime = *((float *)Dictionary_Get(params, 'T',
                                             &delayTrgTimeType));
}