/*
 * File:   password_lock.c
 * Brief:  Six-digit matrix keypad password lock state machine.
 * Author: CaelusAn
 * Date:   2026-10-04
 */

#include "password_lock.h"
#include "lcd1602.h"
#include "matrix_key.h"
#include "timer.h"

#define PASSWORD_LENGTH             6U
#define PASSWORD_UNLOCK_SECONDS     5U
#define PASSWORD_RELOCK_SECONDS     1U
#define PASSWORD_FAILED_SECONDS     2U
#define PASSWORD_MAX_WRONG_ATTEMPTS 30U
#define PASSWORD_LOCK_OUTPUT        P2_0

typedef enum
{
    PASSWORD_STATE_INPUT,
    PASSWORD_STATE_SUCCESS,
    PASSWORD_STATE_FAILED,
    PASSWORD_STATE_LOCKED,
    PASSWORD_STATE_RELOCK,
    PASSWORD_STATE_PERMANENT
} PasswordLockState;

typedef struct
{
    u8 wrongAttempts;
    u16 lockSeconds;
} PasswordLockRule;

/*
 * Change this array to set the built-in password.
 * Digits are stored separately so leading zeros are preserved.
 */
static const u8 code STORED_PASSWORD[PASSWORD_LENGTH] =
{
    1U, 2U, 3U, 4U, 5U, 6U
};

static const PasswordLockRule code PASSWORD_LOCK_RULES[] =
{
    { 5U,    60U },
    { 10U,  300U },
    { 15U,  600U },
    { 20U, 1800U },
    { 25U, 3600U },
    { 30U, 18000U }
};

#define PASSWORD_LOCK_RULE_COUNT \
    (sizeof(PASSWORD_LOCK_RULES) / sizeof(PASSWORD_LOCK_RULES[0]))

static u8 inputDigits[PASSWORD_LENGTH];
static u8 inputLength;
static u8 wrongAttempts;
static u16 stateStartSeconds;
static u16 activeDurationSeconds;
static u16 pendingLockSeconds;
static u16 lastDisplayedSeconds;
static PasswordLockState currentState;

static void PasswordLock_SetState(PasswordLockState newState,
                                  u16 durationSeconds);
static void PasswordLock_UpdateInputDisplay(void);
static void PasswordLock_ShowSuccessScreen(void);
static void PasswordLock_ShowFailedScreen(void);
static void PasswordLock_ShowLockedScreen(u16 durationSeconds);
static void PasswordLock_ShowRelockScreen(void);
static void PasswordLock_ShowPermanentScreen(void);
static void PasswordLock_HandleKey(u8 keyValue);
static u8 PasswordLock_IsPasswordCorrect(void);
static u16 PasswordLock_GetLockoutSeconds(u8 attempts);
static void PasswordLock_UpdateTimedState(void);
static void PasswordLock_UpdateLockScreen(void);
static void PasswordLock_FormatTime(u16 totalSeconds, char *text);

/*
 * @brief  Return the number of seconds elapsed since a stored timestamp.
 * @param  startSeconds Timestamp returned by Timer0_GetElapsedSeconds().
 * @retval Elapsed seconds.
 */
static u16 PasswordLock_GetElapsedSeconds(u16 startSeconds)
{
    return (u16)(Timer0_GetElapsedSeconds() - startSeconds);
}

/*
 * @brief  Clear the input password buffer and display all empty positions.
 * @param  None
 * @retval None
 */
static void PasswordLock_ClearInput(void)
{
    u8 index;

    inputLength = 0U;
    for (index = 0U; index < PASSWORD_LENGTH; index++)
    {
        inputDigits[index] = 0U;
    }
}

/*
 * @brief  Display the current number of entered password digits as asterisks.
 * @param  None
 * @retval None
 */
static void PasswordLock_UpdateInputDisplay(void)
{
    char displayText[PASSWORD_LENGTH + 1U];
    u8 index;

    for (index = 0U; index < PASSWORD_LENGTH; index++)
    {
        if (index < inputLength)
        {
            displayText[index] = '*';
        }
        else
        {
            displayText[index] = ' ';
        }
    }

    displayText[PASSWORD_LENGTH] = '\0';
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_2,
                       displayText);
}

/*
 * @brief  Display the password input screen.
 * @param  None
 * @retval None
 */
static void PasswordLock_ShowInputScreen(void)
{
    LCD1602_Clear();
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_1,
                       "Please enter...");
    PasswordLock_UpdateInputDisplay();
}

/*
 * @brief  Display the successful unlock screen.
 * @param  None
 * @retval None
 */
static void PasswordLock_ShowSuccessScreen(void)
{
    PASSWORD_LOCK_OUTPUT = 1;
    LCD1602_Clear();
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_1,
                       "Success");
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_2,
                       "Unlocked");
}

/*
 * @brief  Display the failed password screen.
 * @param  None
 * @retval None
 */
static void PasswordLock_ShowFailedScreen(void)
{
    LCD1602_Clear();
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_1,
                       "Failed");
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_2,
                       "Try again");
}

/*
 * @brief  Display the timed lock screen and its initial countdown.
 * @param  durationSeconds Duration of the active lock.
 * @retval None
 */
static void PasswordLock_ShowLockedScreen(u16 durationSeconds)
{
    char timeText[9];

    LCD1602_Clear();
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_1,
                       "Locked");

    PasswordLock_FormatTime(durationSeconds, timeText);
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_2,
                       timeText);

    lastDisplayedSeconds = durationSeconds;
}

/*
 * @brief  Display the short relock message after a successful unlock.
 * @param  None
 * @retval None
 */
static void PasswordLock_ShowRelockScreen(void)
{
    LCD1602_Clear();
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_1,
                       "Locked");
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_2,
                       "Returning...");
}

/*
 * @brief  Display the permanent disabled screen.
 * @param  None
 * @retval None
 */
static void PasswordLock_ShowPermanentScreen(void)
{
    LCD1602_Clear();
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_1,
                       "Locked");
    LCD1602_ShowString(LCD1602_COLUMN_START,
                       LCD1602_ROW_2,
                       "Permanent");
}

/*
 * @brief  Return the lock duration associated with the current failure count.
 * @param  attempts Number of consecutive wrong attempts.
 * @retval Lock duration in seconds, or zero when no lock is required.
 */
static u16 PasswordLock_GetLockoutSeconds(u8 attempts)
{
    u8 index;

    for (index = 0U; index < PASSWORD_LOCK_RULE_COUNT; index++)
    {
        if (attempts == PASSWORD_LOCK_RULES[index].wrongAttempts)
        {
            return PASSWORD_LOCK_RULES[index].lockSeconds;
        }
    }

    return 0U;
}

/*
 * @brief  Change the password lock state and update the LCD or lock output.
 * @param  newState New state to enter.
 * @param  durationSeconds Duration for timed states.
 * @retval None
 */
static void PasswordLock_SetState(PasswordLockState newState,
                                  u16 durationSeconds)
{
    currentState = newState;
    stateStartSeconds = Timer0_GetElapsedSeconds();
    activeDurationSeconds = durationSeconds;
    lastDisplayedSeconds = 0xFFFFU;

    if (newState != PASSWORD_STATE_SUCCESS)
    {
        PASSWORD_LOCK_OUTPUT = 0;
    }

    if (newState == PASSWORD_STATE_INPUT)
    {
        PasswordLock_ClearInput();
        PasswordLock_ShowInputScreen();
    }
    else if (newState == PASSWORD_STATE_SUCCESS)
    {
        PasswordLock_ShowSuccessScreen();
    }
    else if (newState == PASSWORD_STATE_FAILED)
    {
        PasswordLock_ShowFailedScreen();
    }
    else if (newState == PASSWORD_STATE_LOCKED)
    {
        PasswordLock_ShowLockedScreen(durationSeconds);
    }
    else if (newState == PASSWORD_STATE_RELOCK)
    {
        PasswordLock_ShowRelockScreen();
    }
    else
    {
        PasswordLock_ShowPermanentScreen();
    }
}

/*
 * @brief  Append one digit to the password input buffer.
 * @param  digit Digit value from 0 to 9.
 * @retval None
 */
static void PasswordLock_AddDigit(u8 digit)
{
    if (inputLength >= PASSWORD_LENGTH)
    {
        return;
    }

    inputDigits[inputLength] = digit;
    inputLength++;
    PasswordLock_UpdateInputDisplay();
}

/*
 * @brief  Remove the final digit from the password input buffer.
 * @param  None
 * @retval None
 */
static void PasswordLock_DeleteDigit(void)
{
    if (inputLength == 0U)
    {
        return;
    }

    inputLength--;
    inputDigits[inputLength] = 0U;
    PasswordLock_UpdateInputDisplay();
}

/*
 * @brief  Compare the input password with the built-in password.
 * @param  None
 * @retval Nonzero when all six digits match.
 */
static u8 PasswordLock_IsPasswordCorrect(void)
{
    u8 index;

    if (inputLength != PASSWORD_LENGTH)
    {
        return 0U;
    }

    for (index = 0U; index < PASSWORD_LENGTH; index++)
    {
        if (inputDigits[index] != STORED_PASSWORD[index])
        {
            return 0U;
        }
    }

    return 1U;
}

/*
 * @brief  Process one keypad key while the lock is waiting for input.
 * @param  keyValue Value returned by MatrixKey().
 * @retval None
 */
static void PasswordLock_HandleKey(u8 keyValue)
{
    if ((keyValue >= MATRIX_KEY_DIGIT_MIN) &&
        (keyValue <= MATRIX_KEY_DIGIT_MAX))
    {
        PasswordLock_AddDigit(keyValue);
    }
    else if (keyValue == MATRIX_KEY_ZERO)
    {
        PasswordLock_AddDigit(0U);
    }
    else if (keyValue == MATRIX_KEY_DELETE)
    {
        PasswordLock_DeleteDigit();
    }
    else if (keyValue == MATRIX_KEY_CONFIRM)
    {
        if (inputLength != PASSWORD_LENGTH)
        {
            return;
        }

        if (PasswordLock_IsPasswordCorrect() != 0U)
        {
            wrongAttempts = 0U;
            PasswordLock_SetState(PASSWORD_STATE_SUCCESS,
                                  PASSWORD_UNLOCK_SECONDS);
        }
        else
        {
            wrongAttempts++;
            pendingLockSeconds = PasswordLock_GetLockoutSeconds(wrongAttempts);
            PasswordLock_SetState(PASSWORD_STATE_FAILED,
                                  PASSWORD_FAILED_SECONDS);
        }
    }
}

/*
 * @brief  Format a duration as MM:SS or H:MM:SS.
 * @param  totalSeconds Duration in seconds.
 * @param  text Output buffer with room for at least nine characters.
 * @retval None
 */
static void PasswordLock_FormatTime(u16 totalSeconds, char *text)
{
    u16 hours;
    u8 minutes;
    u8 seconds;

    hours = totalSeconds / 3600U;
    minutes = (u8)((totalSeconds % 3600U) / 60U);
    seconds = (u8)(totalSeconds % 60U);

    if (hours > 0U)
    {
        text[0] = (char)('0' + hours);
        text[1] = ':';
        text[2] = (char)('0' + (minutes / 10U));
        text[3] = (char)('0' + (minutes % 10U));
        text[4] = ':';
        text[5] = (char)('0' + (seconds / 10U));
        text[6] = (char)('0' + (seconds % 10U));
        text[7] = '\0';
    }
    else
    {
        text[0] = (char)('0' + (minutes / 10U));
        text[1] = (char)('0' + (minutes % 10U));
        text[2] = ':';
        text[3] = (char)('0' + (seconds / 10U));
        text[4] = (char)('0' + (seconds % 10U));
        text[5] = '\0';
    }
}

/*
 * @brief  Refresh the lock countdown display.
 * @param  None
 * @retval None
 */
static void PasswordLock_UpdateLockScreen(void)
{
    char timeText[9];
    u16 elapsedSeconds;
    u16 remainingSeconds;

    elapsedSeconds = PasswordLock_GetElapsedSeconds(stateStartSeconds);
    if (elapsedSeconds >= activeDurationSeconds)
    {
        PasswordLock_SetState(PASSWORD_STATE_INPUT, 0U);
        return;
    }

    remainingSeconds = activeDurationSeconds - elapsedSeconds;
    if (remainingSeconds != lastDisplayedSeconds)
    {
        PasswordLock_FormatTime(remainingSeconds, timeText);
        LCD1602_ShowString(LCD1602_COLUMN_START,
                           LCD1602_ROW_2,
                           timeText);
        lastDisplayedSeconds = remainingSeconds;
    }
}

/*
 * @brief  Process a timed state such as Success, Failed, or Relock.
 * @param  None
 * @retval None
 */
static void PasswordLock_UpdateTimedState(void)
{
    if (PasswordLock_GetElapsedSeconds(stateStartSeconds) <
        activeDurationSeconds)
    {
        return;
    }

    if (currentState == PASSWORD_STATE_SUCCESS)
    {
        PasswordLock_SetState(PASSWORD_STATE_RELOCK,
                              PASSWORD_RELOCK_SECONDS);
    }
    else if (currentState == PASSWORD_STATE_FAILED)
    {
        if (wrongAttempts > PASSWORD_MAX_WRONG_ATTEMPTS)
        {
            PasswordLock_SetState(PASSWORD_STATE_PERMANENT, 0U);
        }
        else if (pendingLockSeconds > 0U)
        {
            PasswordLock_SetState(PASSWORD_STATE_LOCKED,
                                  pendingLockSeconds);
        }
        else
        {
            PasswordLock_SetState(PASSWORD_STATE_INPUT, 0U);
        }
    }
    else
    {
        PasswordLock_SetState(PASSWORD_STATE_INPUT, 0U);
    }
}

/*
 * @brief  Initialize the password lock module.
 * @param  None
 * @retval None
 */
void PasswordLock_Init(void)
{
    wrongAttempts = 0U;
    pendingLockSeconds = 0U;
    PASSWORD_LOCK_OUTPUT = 0;
    PasswordLock_ClearInput();
    PasswordLock_SetState(PASSWORD_STATE_INPUT, 0U);
}

/*
 * @brief  Run one password lock state machine update.
 * @param  None
 * @retval None
 */
void PasswordLock_Update(void)
{
    u8 keyValue;

    if (currentState == PASSWORD_STATE_INPUT)
    {
        keyValue = MatrixKey();
        if (keyValue != MATRIX_KEY_NONE)
        {
            PasswordLock_HandleKey(keyValue);
        }
    }
    else if (currentState == PASSWORD_STATE_LOCKED)
    {
        PasswordLock_UpdateLockScreen();
    }
    else if ((currentState == PASSWORD_STATE_SUCCESS) ||
             (currentState == PASSWORD_STATE_FAILED) ||
             (currentState == PASSWORD_STATE_RELOCK))
    {
        PasswordLock_UpdateTimedState();
    }
}
