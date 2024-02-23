#include "lcd20x4.hpp"
#include "lcd_cmd.hpp"
#include "i2c_scanner.hpp"
#include "pico_errors.hpp"

#include <string.h>
#include <iostream>

Lcd20x4::Lcd20x4(i2c_inst_t *i2c, const uint8_t addr, const uint8_t cols, const uint8_t rows, const bool is4bit)
    : i2c_(i2c), addr_(addr), rows_(rows),
      cols_(cols), cur_line_(0), cur_col_(0), initialized_(false), is_4bit_(is4bit)
{
}

Lcd20x4::~Lcd20x4() throw()
{
}

void Lcd20x4::init()
{
    initialized_ = false;
    if (!is_present())
    {
        std::cout << "LCD not present" << std::endl;
        return;
    }

    write_raw(0);

    display_control_ = LCD_DISPLAYON; // | 0x30;
    if (rows_ > 1)
        display_control_ |= LCD_2LINE;

    if (is_4bit_)
    {
        init_4bit();
        return;
    }

    // SEE PAGE 45 / 46 FOR INITIALIZATION SPECIFICATION !
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.Arduino can turn on way befer 4.5V so we'll
    // wait 50
    sleep_ms(50);

    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | display_control_);
    sleep_us(4500);
    // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | display_control_);
    sleep_us(150);

    // third go
    command(LCD_FUNCTIONSET | display_control_);

    // finally, set #lines, font size, etc.
    command(LCD_FUNCTIONSET | display_control_);

    // turn the display on with no cursor or blinking default
    display_mode_ = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction(for romance languages)
    display_mode_ = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | display_mode_);

    initialized_ = true;
}

void Lcd20x4::init_4bit()
{
    write_nibble(LCD_FUNCTIONRESET);
    sleep_ms(5);

    write_nibble(LCD_FUNCTIONRESET);
    sleep_ms(1);

    write_nibble(LCD_FUNCTIONRESET);
    sleep_ms(1);

    write_nibble(LCD_FUNCTIONSET);

    command(LCD_DISPLAYCONTROL);
    command(LCD_BACKLIGHT);
    command(LCD_CLEARDISPLAY);
    command(LCD_RETURNHOME);
    command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    command(LCD_FUNCTIONSET | LCD_2LINE);

    initialized_ = true;
}

void Lcd20x4::deinit()
{
    initialized_ = false;
}

void Lcd20x4::clear()
{
    command(LCD_CLEARDISPLAY);
    sleep_ms(2);
    // this command takes a long time !
}

void Lcd20x4::home()
{
    command(LCD_RETURNHOME);
    // set cursor position to zero
    sleep_ms(2);
    // this command takes a long time !
}

void Lcd20x4::setCursor(const uint8_t &col, const uint8_t &row)
{
    uint8_t addr = 0x80;
    if (row & 1) // rows 1 & 3 add 0x40
        addr += 0x40;
    if (row & 2) // rows 2 & 3 add number of columns
        addr += cols_;
    addr = addr + col;
    command(addr);
}

// Turn the display on / off(quickly)
void Lcd20x4::display(bool d)
{
    if (d)
        display_control_ |= LCD_DISPLAYON;
    else
        display_control_ &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | display_control_);
}

void Lcd20x4::cursor(bool c)
{
    if (c)
        display_control_ |= LCD_CURSORON;
    else
        display_control_ &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | display_control_);
}

void Lcd20x4::blink(bool b)
{
    if (b)
        display_control_ |= LCD_BLINKON;
    else
        display_control_ &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | display_control_);
}

void Lcd20x4::autoscroll(bool s)
{
    if (s)
        display_control_ |= LCD_ENTRYSHIFTINCREMENT;
    else
        display_control_ &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_DISPLAYCONTROL | display_control_);
}

void Lcd20x4::create_char(uint8_t location, const uint8_t charmap[8])
{
    location &= 0x07;
    command(LCD_SETCGRAMADDR | (location << 3));
    if (is_4bit_)
    {
        sleep_us(40);
        if (initialized_)
        {
            write(charmap, 8);
        }
    }
    else
    {
        uint8_t buf[9];
        buf[0] = 0x40;
        memcpy(buf + 1, charmap, 8);
        if (initialized_)
        {
            write(buf, 9);
        }
    }
}

void Lcd20x4::print(const char *s)
{
    const uint8_t *buf(reinterpret_cast<const uint8_t *>(s));
    write(buf, strlen(s));
}

void Lcd20x4::write(const uint8_t bytes[], size_t n)
{
    if (is_4bit_)
    {
        write_4bit(bytes, n);
        return;
    }

    uint8_t buf[n + 1];
    buf[0] = LCD_SETCGRAMADDR;
    memcpy(buf + 1, bytes, n);
    if (initialized_)
    {
        int ret = i2c_write_timeout_us(i2c_, addr_, buf, n + 1, false, 10000);
        if (ret < 0)
            std::cout << __FUNCTION__ << " Error: " << pico_errors[ret] << std::endl;
    }
}

void Lcd20x4::backlight(bool o)
{
    uint8_t b = (o ? 0x08 : 0x00);
    write_raw(b);
}

void Lcd20x4::write_4bit(const uint8_t bytes[], size_t n)
{
    // std::cout << __FUNCTION__ << " " << std::endl;
    for (size_t i = 0; i < n; ++i)
    {
        char c = bytes[i];
        uint8_t byte = 0x01 | 0x08 | (c & 0xf0);
        write_nibble(byte);
        byte = 0x01 | 0x08 | ((c << 4) & 0xf0);
        write_nibble(byte);
    }
}

void Lcd20x4::command(const uint8_t &command)
{
    if (is_4bit_)
    {
        command_4bit(command);
        return;
    }
    uint8_t buf[2];
    buf[0] = LCD_SETDDRAMADDR;
    buf[1] = command;
    i2c_write_timeout_us(i2c_, addr_, buf, 2, false, 10000);
}

void Lcd20x4::command_4bit(const uint8_t &command)
{
    uint8_t byte = 0x08 | (command & 0xf0);
    write_nibble(byte);
    byte = 0x08 | ((command << 4) & 0xf0);
    write_nibble(byte);
    if (command <= 3)
        sleep_ms(5);
}

void Lcd20x4::write_nibble(uint8_t n)
{
    uint8_t buf = n | 0x04;
    i2c_write_blocking(i2c_, addr_, &buf, 1, false);
    buf = n;
    i2c_write_blocking(i2c_, addr_, &buf, 1, false);
}

void Lcd20x4::write_raw(uint8_t b)
{
    i2c_write_blocking(i2c_, addr_, &b, 1, false);
}

bool Lcd20x4::is_present() const
{
    bool f(i2c_device_present(i2c_, addr_));
    if (!f)
        initialized_ = false;
    return f;
}
