#ifndef LCD_CMD_HPP
#define LCD_CMD_HPP

/**
 * @file lcd_cmd.hpp
 * @brief Header file containing command definitions for LCD control.
 */

/**
 * @defgroup LCDCommands LCD Commands
 * @{
 */

// commands
#define LCD_CLEARDISPLAY 0x01   ///< Clear display command
#define LCD_RETURNHOME 0x02     ///< Return home command
#define LCD_ENTRYMODESET 0x04   ///< Entry mode set command
#define LCD_DISPLAYCONTROL 0x08 ///< Display control command
#define LCD_CURSORSHIFT 0x10    ///< Cursor shift command
#define LCD_FUNCTIONSET 0x20    ///< Function set command
#define LCD_FUNCTIONRESET 0x30  ///< Function reset command
#define LCD_SETCGRAMADDR 0x40   ///< Set CGRAM address command
#define LCD_SETDDRAMADDR 0x80   ///< Set DDRAM address command

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00          ///< Entry mode: Move cursor to the right
#define LCD_ENTRYLEFT 0x02           ///< Entry mode: Move cursor to the left
#define LCD_ENTRYSHIFTINCREMENT 0x01 ///< Entry mode: Increment cursor position
#define LCD_ENTRYSHIFTDECREMENT 0x00 ///< Entry mode: Decrement cursor position

// flags for display on / off control
#define LCD_DISPLAYON 0x04  ///< Display on control
#define LCD_DISPLAYOFF 0x00 ///< Display off control
#define LCD_CURSORON 0x02   ///< Cursor on control
#define LCD_CURSOROFF 0x00  ///< Cursor off control
#define LCD_BLINKON 0x01    ///< Blinking cursor on control
#define LCD_BLINKOFF 0x00   ///< Blinking cursor off control

// for display / cursor shift
#define LCD_DISPLAYMOVE 0x08 ///< Display shift command
#define LCD_CURSORMOVE 0x00  ///< Cursor move command
#define LCD_MOVERIGHT 0x04   ///< Move cursor/display to the right
#define LCD_MOVELEFT 0x00    ///< Move cursor/display to the left

// flags for function set
#define LCD_8BITMODE 0x10    ///< 8-bit data mode
#define LCD_4BITMODE 0x00    ///< 4-bit data mode
#define LCD_2LINE 0x08       ///< Two-line display
#define LCD_1LINE 0x00       ///< One-line display
#define LCD_5x10DOTS 0x04    ///< 5x10 dot character font
#define LCD_5x8DOTS 0x00     ///< 5x8 dot character font
#define LCD_BACKLIGHT 0x08   ///< Backlight control
#define LCD_NOBACKLIGHT 0x00 ///< No backlight control

// PCF8574 pin definitions
#define MASK_RS 0x01 ///< Mask for RS (Register Select) pin
#define MASK_RW 0x02 ///< Mask for RW (Read/Write) pin
#define MASK_E 0x04  ///< Mask for E (Enable) pin

/**
 * @}
 */

#endif // LCD_CMD_HPP
