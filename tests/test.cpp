#include "gtest/gtest.h"
#include "stdlib.h"
#include "../cparser/inc/cparser.h"

/* Private function prototypes ---------------------------------------------*/
static void versionCmdCallback(Dictionary_Dictionary_t *params);
static void pumpCmdCallback(Dictionary_Dictionary_t *params);
static void moveCmdCallback(Dictionary_Dictionary_t *params);
static void delayCmdCallback(Dictionary_Dictionary_t *params);

/* Private variables -------------------------------------------------------*/
Cp_Command_t CommandTable[] =
    {
        {"delay",
         {
             {.letter = 'T', .type = CP_PARAM_TYPE_REAL},
         },
         delayCmdCallback,
         1},
        {"versi",
         {
             {.letter = 'V', .type = CP_PARAM_TYPE_INTEGER},
         },
         versionCmdCallback,
         1},
        {"move",
         {{.letter = 'D', .type = CP_PARAM_TYPE_REAL},
          {.letter = 'S', .type = CP_PARAM_TYPE_REAL}},
         moveCmdCallback,
         2},
        {"pump",
         {
             {.letter = 'M', .type = CP_PARAM_TYPE_LETTER},
             {.letter = 'V', .type = CP_PARAM_TYPE_REAL},
         },
         pumpCmdCallback,
         2},
};

static Bool_t versionCmdTriggered;
static int32_t versionCmdVersion;

static Bool_t delayCmdTriggered;
static float delayCmdTime;

static Bool_t moveCmdTriggered;
static float moveCmdDisplacement;
static float moveCmdSpeed;

static Bool_t pumpCmdTriggered;
static float pumpCmdVolume;
static char pumpCmdMode;

struct CparserTest : public ::testing::Test
{
public:
    virtual void SetUp() override
    {
        versionCmdTriggered = FALSE;
        versionCmdVersion = 0;

        delayCmdTriggered = FALSE;
        delayCmdTime = 0.0f;

        moveCmdTriggered = FALSE;
        moveCmdDisplacement = 0.0f;
        moveCmdSpeed = 0.0f;

        pumpCmdTriggered = FALSE;
        pumpCmdVolume = 0.0f;
        pumpCmdMode = ' ';

        // Register command table.
        for (uint8_t i = 0; i < (sizeof(CommandTable) / sizeof(CommandTable[0])); i++)
        {
            Cp_Register(&CommandTable[i]);
        }
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

    EXPECT_EQ(delayCmdTriggered, TRUE);
    EXPECT_FLOAT_EQ(delayCmdTime, 100.0f);
}

TEST_F(CparserTest, VersionTest)
{
    // Feed command.
    char line[] = "versi V10";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(versionCmdTriggered, TRUE);
    EXPECT_EQ(versionCmdVersion, 10);
}

TEST_F(CparserTest, MoveTest)
{
    // Feed commands.
    char line[] = "move D1.0 S5.0";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(moveCmdTriggered, TRUE);
    EXPECT_FLOAT_EQ(moveCmdDisplacement, 1.0f);
    EXPECT_FLOAT_EQ(moveCmdSpeed, 5.0f);
}

TEST_F(CparserTest, PumpTest)
{
    // Feed commands.
    char line[] = "pump V1.25 MD";
    Cp_FeedLine(line, sizeof(line) - 1);

    EXPECT_EQ(pumpCmdTriggered, TRUE);
    EXPECT_FLOAT_EQ(pumpCmdVolume, 1.25);
    EXPECT_EQ('D', pumpCmdMode);
}

void versionCmdCallback(Dictionary_Dictionary_t *params)
{
    versionCmdTriggered = TRUE;
    versionCmdVersion = *((int32_t *)Dictionary_Get(params, 'V'));
}

void pumpCmdCallback(Dictionary_Dictionary_t *params)
{
    pumpCmdTriggered = TRUE;
    pumpCmdVolume = *((float *)Dictionary_Get(params, 'V'));
    pumpCmdMode = *((char *)Dictionary_Get(params, 'M'));
}

void moveCmdCallback(Dictionary_Dictionary_t *params)
{
    moveCmdTriggered = TRUE;
    moveCmdDisplacement = *((float *)Dictionary_Get(params, 'D'));
    moveCmdSpeed = *((float *)Dictionary_Get(params, 'S'));
}

void delayCmdCallback(Dictionary_Dictionary_t *params)
{
    delayCmdTriggered = TRUE;
    delayCmdTime = *((float *)Dictionary_Get(params, 'T'));
}