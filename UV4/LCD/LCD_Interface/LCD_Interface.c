/*LCD 8 bit example
  rs => Register Select [Command : Data]
        0 => Command
        1 => Data
  rw => Read/Write [Write : Read]
        0 => Write
        1 => Read
  en => Enable signal [High to Low]
*/
#define LCD_data PORTC
//****************************************************
//    These statements doesn't apply in Mikroc
/*#define LCD_rs   PORTA.B0
#define LCD_rw   PORTA.B1
#define LCD_en   PORTA.B2
#define LCD_D7   PORTC.B7*/
//****************************************************
void LCD_busy() 
{
     //delay is in case you don't want to check the buzy flag
     delay_ms(1);//it is very important to return to the manual for it
     //in case of chicking the buzy flag
     /*PORTC.B7 = 1;           //Make D7th bit of LCD as i/p
     PORTA.B2 = 1;           //Make port pin as o/p
     PORTA.B0 = 0;
     PORTA.B1 = 1;
     while (PORTC.B7 = 1) {
           PORTA.B2 = 0;
           PORTA.B2 = 1;
     }*/
}
void LCD_write_command () {
     PORTA.F0   = 0;        //Selected command register
     PORTA.F1   = 0;        //We are writing in command register
     PORTA.F2   = 1;        //Enable H->L
     PORTA.F2   = 0;
     LCD_busy();          //Wait for LCD to process the command
}
void LCD_clear_display () {
     LCD_data = 0x01;     //Clear LCD
     LCD_write_command();
}
void LCD_return_homee () {
     LCD_data = 0x02;     //Return LCD to Home
     LCD_write_command();
}
void LCD_entry_mode_set (unsigned int ID,unsigned int S) {
     /*
     ID => [Decrement : Increment]
           0 => Decrement
           1 => Increment
     S  => accompanies display shift
           0 => OFF
           1 => ON
     */
     LCD_data = 0x00;
     ID = ID << 1;
     LCD_data = LCD_data + 0x04 + ID + S;
     LCD_write_command();
}
void LCD_display_control (unsigned int D,unsigned int C,unsigned int B) {
     /*
     D  => Display ON/OFF
           0 => OFF
           1 => ON
     C  => Cursor ON/OFF
           0 => OFF
           1 => ON
     B  => Blink the character at the cursor position
           0 => OFF
           1 => ON
     */
     LCD_data = 0x00;
     C = C << 1;
     D = D << 2;
     LCD_data = LCD_data + 0x08 + D + C + B;
     LCD_write_command();
}
void LCD_cursor_display_shift (unsigned int SC,unsigned int RL) {
     /*
     Moves the cursor and shifts the display without changing the DDRAM contents
     SC  => [Cursor : Display]
           0 => Cursor
           1 => Display
     RL  => [Left : Right]
           0 => Left
           1 => Right
     */
     LCD_data = 0x00;
     RL = RL << 2;
     SC = SC << 3;
     LCD_data = LCD_data + 0x10 + SC + RL;
     LCD_write_command();
}
void LCD_function_set (unsigned int DL,unsigned int N,unsigned int F) {
     /*
     DL  => Data Width [4 bit : 8 bit]
           0 => 4 bit
           1 => 8 bit
     N  => Number of lines in display[1 : 2]
           0 => 1 Line
           1 => 2 Lines
     F  => Character font[5x7 : 5x10]
           0 => 5x7
           1 => 5x10
     */
     LCD_data = 0x00;
     F = F << 2;
     N = N << 3;
     DL = DL << 4;
     LCD_data = LCD_data + 0x20 + DL + N + F;
     LCD_write_command();
}
void LCD_cgram_address (unsigned int cgadrs) {
     /*
     cgadrs => CGRAM Address Binary 6 bit
     i didn't use it yet
     */
     LCD_data = 0x00;
     LCD_data = cgadrs;
     LCD_write_command();
}
void LCD_ddram_address (unsigned int ddadrs) 
{
     /*
     ddadrs => DDRAM Address Binary 7 bit
               Line 1 Column 1
               Binary Address : 0B10000000
               HEX Address     : 0X80

               Line 1 Column 20
               Binary Address : 0B10001111
               HEX Address     : 0X93

               Line 2 Column 1
               Binary Address : 0B11000000
               HEX Address     : 0XC0

              Line 2 Column 20
               Binary Address : 0B11001111
               HEX Address     : 0XD3

               Line 3 Column 1
               Binary Address : 0B11000000
               HEX Address     : 0X94

              Line 3 Column 20
               Binary Address : 0B11001111
               HEX Address     : 0XA7

               Line 4 Column 1
               Binary Address : 0B11000000
               HEX Address     : 0XD4

              Line 4 Column 20
               Binary Address : 0B11001111
               HEX Address     : 0XE7
               [][][][][][][][][][][][][][][][][][][][]
               [][][][][][][][][][][][][][][][][][][][]
               [][][][][][][][][][][][][][][][][][][][]
               [][][][][][][][][][][][][][][][][][][][]
                    C1     C2     C3     C4     C5     C6      C7     C8     C9    C10     C11    C12    C13    C14   C15    C16    C17    C18     C19   C20
               L1 [0X80]-[0X81]-[0X82]-[0X83]-[0X84]-[0X85]-[0X86]-[0X87]-[0X88]-[0X89]-[0X8A]-[0X8B]-[0X8C]-[0X8D]-[0X8E]-[0X8F]-[0X90]-[0X91]-[0X92]-[0X93]
               L2 [0XC0]-[0XC1]-[0XC2]-[0XC3]-[0XC4]-[0XC5]-[0XC6]-[0XC7]-[0XC8]-[0XC9]-[0XCA]-[0XCB]-[0XCC]-[0XCD]-[0XCE]-[0XCF]-[0XD0]-[0XD1]-[0XD2]-[0XD3]
               L3 [0X94]-[0X95]-[0X96]-[0X97]-[0X98]-[0X99]-[0X9A]-[0X9B]-[0X9C]-[0X9D]-[0X9E]-[0X9F]-[0XA0]-[0XA1]-[0XA2]-[0XA3]-[0XA4]-[0XA5]-[0XA6]-[0XA7]
               L4 [0XD4]-[0XD5]-[0XD6]-[0XD7]-[0XD8]-[0XD9]-[0XDA]-[0XDB]-[0XDC]-[0XDD]-[0XDE]-[0XDF]-[0XE0]-[0XE1]-[0XE2]-[0XE3]-[0XE4]-[0XE5]-[0XE6]-[0XE7]
     */
     LCD_data = 0x00;
     LCD_data = ddadrs;
     LCD_write_command();
}
void LCD_write_data(unsigned int output) 
{
     LCD_data = output;     //
     PORTA.F0   = 1;        //Selected data register
     PORTA.F1   = 0;        //We are writing in data register
     PORTA.F2   = 1;        //Enable H->L
     PORTA.F2   = 0;
     LCD_busy();
}
/*void LCD_write_string(unsigned char output) {
     LCD_data = output;     //Entry mode, auto increment with no shift
     PORTA.B0   = 1;        //Selected data register
     PORTA.B1   = 0;        //We are writing in data register
     PORTA.B2   = 1;        //Enable H->L
     PORTA.B2   = 0;
     LCD_busy();
}*/
void LCD_initialize() {
     LCD_function_set (1,1,0);//Function set: 2 Line, 8-bit, 5x7 dots
     LCD_display_control (1,1,1);//Display on, Curson blinking command
     LCD_clear_display();
     LCD_entry_mode_set (1,0);//Entry mode, auto increment with no shift
     LCD_ddram_address(0X80);//Line 1 Column 1
}
void main() {

     ADCON1 = 0X06;
     TRISA = 0X00;
     TRISC = 0x00;
     LCD_initialize();
     LCD_ddram_address(0X80);
     LCD_write_data('H');
     LCD_ddram_address(0XC0);
     LCD_write_data('o');
     LCD_ddram_address(0X94);
     LCD_write_data('l');
     LCD_ddram_address(0XD4);
     LCD_write_data('a');
}