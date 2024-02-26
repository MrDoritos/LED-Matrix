#include <Arduino.h>

const int FIRST_LAYER = 2;
const int FIRST_ANODE = 14;
const int LAYER_COUNT = 6;
const int ANODE_COUNT = 36;
const float LAYER_ON_TIME = 1.0f;
const int MATRIX_SIZE = ANODE_COUNT * LAYER_COUNT;
unsigned long current_millis;
unsigned long start_millis;
unsigned long tick = 0;

void setup() {
  // PORTut your setuPORT code here, to run once:
  randomSeed(analogRead(0));
  Serial.begin(115200);
  Serial.setTimeout(10);
  start_millis = millis();
  for (int i = 0; i < LAYER_COUNT; i++) {
    pinMode(i + FIRST_LAYER, OUTPUT);
  }
  for (int i = 0; i < ANODE_COUNT; i++) {
    pinMode(i + FIRST_ANODE, OUTPUT);  
  }
}

byte matrix[MATRIX_SIZE];
byte matrix_buffer[256];

byte* digital_pin[] = {
    // &PORTORTLIST             
    // -------------------------------------------          
    &PORTE  , // &PORTE 0 ** 0 ** USART0_RX     
    &PORTE  , // &PORTE 1 ** 1 ** USART0_TX     
    &PORTE  , // &PORTE 4 ** 2 ** &PORTWM2  
    &PORTE  , // &PORTE 5 ** 3 ** &PORTWM3  
    &PORTG  , // &PORTG 5 ** 4 ** &PORTWM4  
    &PORTE  , // &PORTE 3 ** 5 ** &PORTWM5  
    &PORTH  , // &PORTH 3 ** 6 ** &PORTWM6  
    &PORTH  , // &PORTH 4 ** 7 ** &PORTWM7  
    &PORTH  , // &PORTH 5 ** 8 ** &PORTWM8  
    &PORTH  , // &PORTH 6 ** 9 ** &PORTWM9  
    &PORTB  , // &PORTB 4 ** 10 ** &PORTWM10        
    &PORTB  , // &PORTB 5 ** 11 ** &PORTWM11        
    &PORTB  , // &PORTB 6 ** 12 ** &PORTWM12        
    &PORTB  , // &PORTB 7 ** 13 ** &PORTWM13        
    &PORTJ  , // &PORTJ 1 ** 14 ** USART3_TX    
    &PORTJ  , // &PORTJ 0 ** 15 ** USART3_RX    
    &PORTH  , // &PORTH 1 ** 16 ** USART2_TX    
    &PORTH  , // &PORTH 0 ** 17 ** USART2_RX    
    &PORTD  , // &PORTD 3 ** 18 ** USART1_TX    
    &PORTD  , // &PORTD 2 ** 19 ** USART1_RX    
    &PORTD  , // &PORTD 1 ** 20 ** I2C_SDA      
    &PORTD  , // &PORTD 0 ** 21 ** I2C_SCL      
    &PORTA  , // &PORTA 0 ** 22 ** D22  
    &PORTA  , // &PORTA 1 ** 23 ** D23  
    &PORTA  , // &PORTA 2 ** 24 ** D24  
    &PORTA  , // &PORTA 3 ** 25 ** D25  
    &PORTA  , // &PORTA 4 ** 26 ** D26  
    &PORTA  , // &PORTA 5 ** 27 ** D27  
    &PORTA  , // &PORTA 6 ** 28 ** D28  
    &PORTA  , // &PORTA 7 ** 29 ** D29  
    &PORTC  , // &PORTC 7 ** 30 ** D30  
    &PORTC  , // &PORTC 6 ** 31 ** D31  
    &PORTC  , // &PORTC 5 ** 32 ** D32  
    &PORTC  , // &PORTC 4 ** 33 ** D33  
    &PORTC  , // &PORTC 3 ** 34 ** D34  
    &PORTC  , // &PORTC 2 ** 35 ** D35  
    &PORTC  , // &PORTC 1 ** 36 ** D36  
    &PORTC  , // &PORTC 0 ** 37 ** D37  
    &PORTD  , // &PORTD 7 ** 38 ** D38  
    &PORTG  , // &PORTG 2 ** 39 ** D39  
    &PORTG  , // &PORTG 1 ** 40 ** D40  
    &PORTG  , // &PORTG 0 ** 41 ** D41  
    &PORTL  , // &PORTL 7 ** 42 ** D42  
    &PORTL  , // &PORTL 6 ** 43 ** D43  
    &PORTL  , // &PORTL 5 ** 44 ** D44  
    &PORTL  , // &PORTL 4 ** 45 ** D45  
    &PORTL  , // &PORTL 3 ** 46 ** D46  
    &PORTL  , // &PORTL 2 ** 47 ** D47  
    &PORTL  , // &PORTL 1 ** 48 ** D48  
    &PORTL  , // &PORTL 0 ** 49 ** D49  
    &PORTB  , // &PORTB 3 ** 50 ** S&PORTI_MISO     
    &PORTB  , // &PORTB 2 ** 51 ** S&PORTI_MOSI     
    &PORTB  , // &PORTB 1 ** 52 ** S&PORTI_SCK      
    &PORTB  , // &PORTB 0 ** 53 ** S&PORTI_SS       
    &PORTF  , // &PORTF 0 ** 54 ** A0   
    &PORTF  , // &PORTF 1 ** 55 ** A1   
    &PORTF  , // &PORTF 2 ** 56 ** A2   
    &PORTF  , // &PORTF 3 ** 57 ** A3   
    &PORTF  , // &PORTF 4 ** 58 ** A4   
    &PORTF  , // &PORTF 5 ** 59 ** A5   
    &PORTF  , // &PORTF 6 ** 60 ** A6   
    &PORTF  , // &PORTF 7 ** 61 ** A7   
    &PORTK  , // &PORTK 0 ** 62 ** A8   
    &PORTK  , // &PORTK 1 ** 63 ** A9   
    &PORTK  , // &PORTK 2 ** 64 ** A10  
    &PORTK  , // &PORTK 3 ** 65 ** A11  
    &PORTK  , // &PORTK 4 ** 66 ** A12  
    &PORTK  , // &PORTK 5 ** 67 ** A13  
    &PORTK  , // &PORTK 6 ** 68 ** A14  
    &PORTK  , // &PORTK 7 ** 69 ** A15  
    // added stuff
    &PORTD  , // &PORTD 4 ** 70 ** D70
    &PORTD  , // &PORTD 5 ** 71 ** D71
    &PORTD  , // &PORTD 6 ** 72 ** D72
    &PORTG  , // &PORTG 3 ** 73 ** D73
    &PORTG  , // &PORTG 4 ** 74 ** D74
    &PORTH  , // &PORTH 2 ** 75 ** D75
    &PORTH  , // &PORTH 7 ** 76 ** D76
    &PORTJ  , // &PORTJ 2 ** 77 ** D77
    &PORTJ  , // &PORTJ 3 ** 78 ** D78
    &PORTJ  , // &PORTJ 4 ** 79 ** D79
    &PORTJ  , // &PORTJ 5 ** 80 ** D80
    &PORTJ  , // &PORTJ 6 ** 81 ** D81
    &PORTJ  , // &PORTJ 7 ** 82 ** D82
    &PORTE  , // &PORTE 6 ** 83 ** D83
    &PORTE  , // &PORTE 7 ** 84 ** D84
};

/*
 * 7 PH4
 * 6 PH3
 * 5 PE3
 * 4 PG5
 * 3 PE5 
 * 2 PE4
 *  
 * 14 PJ1
 * 15 PJ0
 * 16 PH1
 * 17 PH0
 * 18 PD3
 * 19 PD2
 * 20 PD1
 * 21 PD0
 * 
 * 22 PA0
 * 23 PA1
 * 24 PA2
 * 25 PA3
 * 26 PA4
 * 27 PA5
 * 28 PA6
 * 29 PA7
 * 
 * 30 PC7
 * 31 PC6
 * 32 PC5
 * 33 PC4
 * 34 PC3
 * 35 PC2
 * 36 PC1
 * 37 PC0
 * 
 * 38 PD7
 * 39 PG2
 * 40 PG1
 * 41 PG0
 * 
 * 42 PL7
 * 43 PL6
 * 44 PL5
 * 45 PL4
 * 46 PL3
 * 47 PL2
 * 48 PL1
 * 49 PL0
 * 
 * 
 * 
 */

inline void fast_dwrite(int pin, bool state) {  
  //port A C L okay
  //port G D H E J not okay

  int pin_bit = 0;

  switch ((int)digital_pin[pin]) {
    case (int)&PORTA:
      pin_bit = pin - 22;
      break;
    case (int)&PORTC:
      pin_bit = 7 - (pin - 30);
      break;
    case (int)&PORTL:
      pin_bit = 7 - (pin - 42);
      break;
    case (int)&PORTG:
      pin_bit = 2 - (pin - 39);
      break;
    case (int)&PORTD:
      if (pin == 38) 
        pin_bit = 7;
      else
        pin_bit = 3 - (pin - 18);
      break;
    case (int)&PORTH:
        pin_bit = 1 - (pin - 16);
      break;
    case (int)&PORTJ:
        pin_bit = 1 - (pin - 14);
      break;
  }
  
  if (state)
    *digital_pin[pin] |= (0x1 << pin_bit);
  else
    *digital_pin[pin] &= ~(0x1 << pin_bit);
    /*
  if (state) 
    *digital_pin[pin] |= (0x1 << (pin & 0b111));
    //*(int*)&digital_pin_to_port_PGM[pin] |= (0x1 << (pin & 0b111));
  else
    *digital_pin[pin] &= ~(0x1 << (pin & 0b111));
    //*(int*)&digital_pin_to_port_PGM[pin] &= ~(0x1 << (pin & 0b111));
    */
}

void draw_matrix() {
  for (int layer = 0; layer < LAYER_COUNT; layer++) {
    digitalWrite(layer + FIRST_LAYER, HIGH);
    //fast_dwrite(layer + FIRST_LAYER, 1);
    for (int anode = 0; anode < ANODE_COUNT; anode++) {
        int x = anode % 6;
        int y = anode / 6;
        int z = layer;
        bool state = matrix[(z * 36) + (y * 6) + x];
        //digitalWrite(anode + FIRST_ANODE, state);
        fast_dwrite(anode + FIRST_ANODE, state);
    }    

    start_millis = millis();
    
    while (millis() - start_millis < LAYER_ON_TIME);
    
    for (int anode = 0; anode < ANODE_COUNT; anode++) {
         if (matrix[(layer * 36) + anode])
            //digitalWrite(anode + FIRST_ANODE, LOW);
            fast_dwrite(anode + FIRST_ANODE, 0);
    }    
    digitalWrite(layer + FIRST_LAYER, LOW);
    //fast_dwrite(layer + FIRST_LAYER, 0);
  }  
}

void clear_matrix() {
    memset(&matrix, 0, MATRIX_SIZE);  
}

void all_matrix() {
    memset(&matrix, 1, MATRIX_SIZE);  
}

void set_led(int x, int y, int z, bool value) {
  matrix[(z * 36) + (y * 6) + x] = value;
}

void idle_anim(unsigned long tick) {
  int sequence = (tick % 200000) / 20000;
  int anim_prog = tick % 20000;
  switch (sequence) {
    case 2: {
      all_matrix();
      break;
    }
    case 9: {
      clear_matrix();
      break;
      }
    default: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%2)&&!((y+off)%2)&&!((z+off)%2));
          }
      break;
    }
    case 3: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%2)&&!((y+off)%1)&&!((z+off)%1));
          }
      break;
    }
    case 4: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%1)&&!((y+off)%2)&&!((z+off)%1));
          }
      break;
    }
    case 5: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%1)&&!((y+off)%1)&&!((z+off)%2));
          }
      break;
    }
  }
}

void loop() {
  unsigned long _last_recv, _millis;
  clear_matrix();
  
  while (true) {
    /*
    draw_matrix();
    if (Serial.available() > 0) {
      memset(&matrix_buffer[0], 0, 256);
      Serial.readBytesUntil('\n', &matrix_buffer[0], 256);
      memcpy(&matrix[0], &matrix_buffer[0], MATRIX_SIZE);      
    }
    */
    if (millis() - _last_recv > 1000) {
      idle_anim(millis());
    }
    
    _millis = millis();
    unsigned long recv_start = millis();
    if (Serial.available() > 0) {
      memset(&matrix_buffer[0], 0, 256);
      int i = 0, c;
      while (true) {
        if (millis() - _millis > 1) {
          draw_matrix();
          _millis = millis();
        }
        if ((c = Serial.peek()) != -1) {
          matrix_buffer[i++] = Serial.read();
        }
        //i >= 255 || 
        //if (millis() - recv_start > 100) {
        if (i > 216) {
          while (Serial.peek() != -1) Serial.read();
          break;
        }
      }
      _last_recv = millis();
      memcpy(&matrix[0], &matrix_buffer[0], MATRIX_SIZE);
    } else {
      draw_matrix();
    }
  }
}
