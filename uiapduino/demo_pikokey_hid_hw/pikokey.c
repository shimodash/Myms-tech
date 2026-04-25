#include "ch32fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

// ============================================================
// JIS日本語キーボード HID Usage ID リファレンス
// ============================================================
// modifier: 0x00=なし  0x02=左Shift
//
// [アルファベット小文字]
//   a=0x04  b=0x05  c=0x06  d=0x07  e=0x08  f=0x09
//   g=0x0a  h=0x0b  i=0x0c  j=0x0d  k=0x0e  l=0x0f
//   m=0x10  n=0x11  o=0x12  p=0x13  q=0x14  r=0x15
//   s=0x16  t=0x17  u=0x18  v=0x19  w=0x1a  x=0x1b
//   y=0x1c  z=0x1d
// [アルファベット大文字] → modifier=0x02(Shift) + 小文字と同じID
//
// [数字]
//   1=0x1e  2=0x1f  3=0x20  4=0x21  5=0x22
//   6=0x23  7=0x24  8=0x25  9=0x26  0=0x27
//
// [記号 JIS配列]
//   Enter =0x28  Space=0x2c  -=0x2d  ^=0x2e  ¥=0x89
//   @=0x2f(Shiftなし)  [=0x30(Shiftなし)
//   ;=0x33(Shiftなし)  :=0x34(Shiftなし)  ]=0x32(Shiftなし)
//   ,=0x36  .=0x37  /=0x38
//   Shift+1=!  Shift+2="  Shift+3=#  Shift+4=$  Shift+5=%
//   Shift+6=&  Shift+7='  Shift+8=(  Shift+9=)  Shift+0=~
//   Shift+,=<  Shift+.=>  Shift+/=?
//   Shift+-=_  Shift+^=~  Shift+¥=|
//   Shift+@=`  Shift+[={  Shift+]=}
//   Shift+;=+  Shift+:=*
//
// [特殊キー]
//   Tab=0x2b  Esc=0x29  BackSpace=0x2a  Delete=0x4c
//   F1=0x3a ... F12=0x45
//   ↑=0x52  ↓=0x51  ←=0x50  →=0x4f
// ============================================================

// USB HIDキーボードレポート
// [modifier, reserved, key0, key1, key2, key3, key4, key5]
static uint8_t kbd_report[8] = {0};

typedef struct {
    uint8_t modifier; // 0x00=なし, 0x02=Shift
    uint8_t keycode;  // HID Usage ID
} KeyEntry;

// "https://claude.ai/" + Enter  (JIS配列)
static const KeyEntry string_keys[] = {
    {0x00, 0x0b}, //       Usage ID 0x0b  'h'
    {0x00, 0x17}, //       Usage ID 0x17  't'
    {0x00, 0x17}, //       Usage ID 0x17  't'
    {0x00, 0x13}, //       Usage ID 0x13  'p'
    {0x00, 0x16}, //       Usage ID 0x16  's'
    {0x00, 0x34}, //       Usage ID 0x34  ':' (JIS独立キー Shiftなし)
    {0x00, 0x38}, //       Usage ID 0x38  '/'
    {0x00, 0x38}, //       Usage ID 0x38  '/'
    {0x00, 0x06}, //       Usage ID 0x06  'c'
    {0x00, 0x0f}, //       Usage ID 0x0f  'l'
    {0x00, 0x04}, //       Usage ID 0x04  'a'
    {0x00, 0x18}, //       Usage ID 0x18  'u'
    {0x00, 0x07}, //       Usage ID 0x07  'd'
    {0x00, 0x08}, //       Usage ID 0x08  'e'
    {0x00, 0x37}, //       Usage ID 0x37  '.'
    {0x00, 0x04}, //       Usage ID 0x04  'a'
    {0x00, 0x0c}, //       Usage ID 0x0c  'i'
    {0x00, 0x38}, //       Usage ID 0x38  '/'
    {0x00, 0x28}, //       Usage ID 0x28  Enter
};

#define KEY_COUNT (sizeof(string_keys) / sizeof(KeyEntry))

// 状態管理
static uint8_t key_index = 0;
static uint8_t send_phase = 0;  // 0=キー押下, 1=キー離す
static uint8_t sending = 0;
static uint8_t last_btn = 1;    // 1=HIGH(離している)

// ボタン: PC1
#define BTN_PIN  1

void GPIO_Init_Button(void)
{
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
    // CNF=10(入力プルアップ), MODE=00(入力)
    GPIOC->CFGLR &= ~(0xf << (BTN_PIN * 4));
    GPIOC->CFGLR |=  (0x8 << (BTN_PIN * 4));
    GPIOC->BSHR  =  (1 << BTN_PIN); // プルアップ有効
}

int main(void)
{
    SystemInit();
    Delay_Ms(100);

    GPIO_Init_Button();

    usb_setup();

    while(1)
    {
        uint8_t btn = (GPIOC->INDR >> BTN_PIN) & 1; // 1=HIGH, 0=LOW(押下)

        // 立下りエッジ検出
        if (last_btn == 1 && btn == 0 && !sending)
        {
            key_index = 0;
            send_phase = 0;
            sending = 1;
        }
        last_btn = btn;

        Delay_Ms(5);
    }
}

// USB IN エンドポイント処理
void usb_handle_user_in_request(struct usb_endpoint *e, uint8_t *scratchpad,
                                 int endp, uint32_t sendtok,
                                 struct rv003usb_internal *ist)
{
    if (endp == 1)
    {
        if (sending)
        {
            if (send_phase == 0)
            {
                // キー押下レポート
                kbd_report[0] = string_keys[key_index].modifier;
                kbd_report[1] = 0x00;
                kbd_report[2] = string_keys[key_index].keycode;
                kbd_report[3] = 0;
                kbd_report[4] = 0;
                kbd_report[5] = 0;
                kbd_report[6] = 0;
                kbd_report[7] = 0;
                send_phase = 1;
            }
            else
            {
                // キー離すレポート
                memset(kbd_report, 0, 8);
                send_phase = 0;
                key_index++;
                if (key_index >= KEY_COUNT)
                {
                    sending = 0;
                    key_index = 0;
                }
            }
            usb_send_data(kbd_report, 8, 0, sendtok);
        }
        else
        {
            memset(kbd_report, 0, 8);
            usb_send_data(kbd_report, 8, 0, sendtok);
        }
    }
    else
    {
        usb_send_empty(sendtok);
    }
}

int _write(int fd, const char *buf, int count) { return count; }
