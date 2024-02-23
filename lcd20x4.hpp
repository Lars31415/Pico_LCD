#ifndef LCD20X4_H
#define LCD20X4_H

#include <hardware/i2c.h>

/**
 * @brief Represents a 20x4 LCD display using I2C communication.
 *
 * This driver is for I2C LCD displays on Raspberry Pi Pico. 16x2, 20x2, 20x4
 *
 * It supports:
 *
 * JOY-IT: SBC-LCD16x2 in 4-bit mode.
 *
 * Midas: MC21605C6W-BNMLWI-V2, MC22005A6W-FPTLWI-V2, MC42005A6W-BNMLWI-V2
 *
 * seeed: Grove-16x2 LCD (White on Blue)
 */
class Lcd20x4
{
public:
    /**
     * @brief Constructor for the Lcd20x4 class.
     * @param i2c I2C instance to be used for communication.
     * @param addr I2C address of the LCD.
     * @param cols Number of columns in the LCD (default: 20).
     * @param rows Number of rows in the LCD (default: 4).
     * @param is4bit Set to true for 4-bit communication mode, false for 8-bit mode (default: false).
     */
    Lcd20x4(i2c_inst_t *i2c, const uint8_t addr, const uint8_t cols = 20, const uint8_t rows = 4, const bool is4bit = false);

    /**
     * @brief Destructor for the Lcd20x4 class.
     */
    virtual ~Lcd20x4() throw();

    /**
     * @brief Initializes the LCD.
     */
    void init();

    /**
     * @brief Deinitializes the LCD.
     */
    void deinit();

    /**
     * @brief Clears the LCD screen.
     */
    void clear();

    /**
     * @brief Moves the cursor to the home position.
     */
    void home();

    /**
     * @brief Controls the visibility of the LCD display.
     * @param d Set to true to turn on the display, false to turn it off (default: true).
     */
    void display(bool d = true);

    /**
     * @brief Controls the visibility of the cursor.
     * @param c Set to true to show the cursor, false to hide it (default: true).
     */
    void cursor(bool c = true);

    /**
     * @brief Controls the blinking of the cursor.
     * @param b Set to true to enable cursor blinking, false to disable it (default: true).
     */
    void blink(bool b = true);

    /**
     * @brief Controls the autoscroll feature.
     * @param s Set to true to enable autoscroll, false to disable it (default: false).
     */
    void autoscroll(bool s = false);

    /**
     * @brief Controls the backlight of the LCD.
     * @param o Set to true to turn on the backlight, false to turn it off (default: true).
     */
    void backlight(bool o = true);

    /**
     * @brief Creates a custom character for the LCD.
     * @param location The location (0-7) where the custom character will be stored.
     * @param charmap An array representing the custom character pattern.
     */
    void create_char(uint8_t location, const uint8_t charmap[]);

    /**
     * @brief Sets the cursor position on the LCD.
     * @param col The column position (0-based).
     * @param row The row position (0-based).
     */
    void setCursor(const uint8_t &col, const uint8_t &row);

    /**
     * @brief Prints a string on the LCD.
     * @param s The string to be printed.
     */
    void print(const char *s);

    /**
     * @brief Checks if the LCD is present.
     * @return true if the LCD is present, false otherwise.
     */
    bool is_present() const;

    /**
     * @brief Checks if the LCD is ready for communication.
     * @return true if the LCD is ready, false otherwise.
     */
    bool is_ready() const { return initialized_; }

private:
    i2c_inst_t *i2c_;  /**< I2C instance. */
    uint8_t addr_;     /**< I2C address of the LCD. */
    uint8_t rows_;     /**< Number of rows in the LCD. */
    uint8_t cols_;     /**< Number of columns in the LCD. */
    uint8_t cur_line_; /**< Current line position. */
    uint8_t cur_col_;  /**< Current column position. */
    bool initialized_; /**< Flag indicating if the LCD is initialized. */
    bool is_4bit_;     /**< Flag indicating if 4-bit communication mode is used. */

    uint8_t display_control_; /**< Display control settings. */
    uint8_t display_mode_;    /**< Display mode settings. */

    /**
     * @brief Initializes the LCD in 4-bit communication mode.
     */
    void init_4bit();

    /**
     * @brief Sends a command to the LCD.
     * @param command The command to be sent.
     */
    void command(const uint8_t &command);

    /**
     * @brief Sends a command to the LCD in 4-bit communication mode.
     * @param command The command to be sent.
     */
    void command_4bit(const uint8_t &command);

    /**
     * @brief Writes an array of bytes to the LCD.
     * @param bytes The array of bytes to be written.
     * @param n The number of bytes to write.
     */
    void write(const uint8_t bytes[], size_t n);

    /**
     * @brief Writes a array of bytes to the LCD in 4-bit communication mode.
     * @param bytes The array of bytes to be written.
     * @param n The number of bytes to write.
     */
    void write_4bit(const uint8_t bytes[], size_t n);

    /**
     * @brief Writes a raw byte to the LCD.
     * @param byte The byte to be written.
     */
    void write_raw(const uint8_t byte);

    /**
     * @brief Writes a 4-bit nibble to the LCD in 4-bit communication mode.
     * @param n The 4-bit nibble to be written.
     */
    void write_nibble(uint8_t n);

    /**
     * @brief Copy constructor (disabled).
     */
    Lcd20x4(const Lcd20x4 &);

    /**
     * @brief Assignment operator (disabled).
     */
    Lcd20x4 &operator=(const Lcd20x4 &);
};

#endif // LCD20X4_H
