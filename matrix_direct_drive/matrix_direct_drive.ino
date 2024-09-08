const int FIRST_LAYER = 2; //first layer pin
const int FIRST_ANODE = 14; //first anode pin
const int LAYER_COUNT = 6; 
const int ANODE_COUNT = 36;
const int MATRIX_SIZE = ANODE_COUNT * LAYER_COUNT;
const float DISPLAY_US = 4000.0f; //us for each display (4ms)
const float LAYER_ON_TIME = DISPLAY_US / LAYER_COUNT; //us between each layer
const int ANIM_MS = 25; //ms between animation updates
const int ANIM_FRAME = 100; //ms between an animation frame
const int SEQ_MS = 7000; //ms between each sequence
const int REC_MS = 2000; //ms to wait for more serial data
unsigned long current_millis;
unsigned long start_micros;
unsigned long last_frame = 0;
unsigned long tick = 0;

byte matrix[MATRIX_SIZE];
byte matrix_buffer[256];
byte converted_matrix[LAYER_COUNT][5];

inline void clear_matrix() {
    memset(&matrix, 0, MATRIX_SIZE);  
}

inline void all_matrix() {
    memset(&matrix, 1, MATRIX_SIZE);  
}

inline void set_led(int x, int y, int z, bool value) {
  matrix[(z * ANODE_COUNT) + (y * LAYER_COUNT) + x] = value;
}

inline bool get_led(int x, int y, int z) {
  return matrix[(z * ANODE_COUNT) + (y * LAYER_COUNT) + x];
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  Serial.setTimeout(10);
  start_micros = micros();
  for (int i = 0; i < LAYER_COUNT; i++) {
    pinMode(i + FIRST_LAYER, OUTPUT);
  }
  for (int i = 0; i < ANODE_COUNT; i++) {
    pinMode(i + FIRST_ANODE, OUTPUT);  
  }
  DDRA = DDRB = DDRC = DDRL = 0xFF;
  DDRD = 0b00001111;
}


void draw_line(int x1, int y1, int z1, int x2, int y2, int z2, bool value)
{
  set_led(x1, y1, z1, value);
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int dz = abs(z2 - z1);
  int xs;
  int ys;
  int zs;
  if (x2 > x1)
    xs = 1;
  else
    xs = -1;
  if (y2 > y1)
    ys = 1;
  else
    ys = -1;
  if (z2 > z1)
    zs = 1;
  else
    zs = -1;
 
  // Driving axis is X-axis"
  if (dx >= dy && dx >= dz) {
    int p1 = 2 * dy - dx;
    int p2 = 2 * dz - dx;
    while (x1 != x2) {
      x1 += xs;
      if (p1 >= 0) {
        y1 += ys;
        p1 -= 2 * dx;
      }
      if (p2 >= 0) {
        z1 += zs;
        p2 -= 2 * dx;
      }
      p1 += 2 * dy;
      p2 += 2 * dz;
      set_led(x1, y1, z1, value);
    }
 
    // Driving axis is Y-axis"
  }
  else if (dy >= dx && dy >= dz) {
    int p1 = 2 * dx - dy;
    int p2 = 2 * dz - dy;
    while (y1 != y2) {
      y1 += ys;
      if (p1 >= 0) {
        x1 += xs;
        p1 -= 2 * dy;
      }
      if (p2 >= 0) {
        z1 += zs;
        p2 -= 2 * dy;
      }
      p1 += 2 * dx;
      p2 += 2 * dz;
      set_led(x1, y1, z1, value);
    }
 
    // Driving axis is Z-axis"
  }
  else {
    int p1 = 2 * dy - dz;
    int p2 = 2 * dx - dz;
    while (z1 != z2) {
      z1 += zs;
      if (p1 >= 0) {
        y1 += ys;
        p1 -= 2 * dz;
      }
      if (p2 >= 0) {
        x1 += xs;
        p2 -= 2 * dz;
      }
      p1 += 2 * dy;
      p2 += 2 * dx;
     set_led(x1, y1, z1, value);
    }
  }
}

void idle_anim(unsigned long tick) {
  unsigned long animation_count = 9;
  unsigned long total_length = animation_count * SEQ_MS;
  unsigned long sequence = (tick % total_length) / SEQ_MS;
  unsigned long anim_prog = tick % SEQ_MS;

  if (anim_prog % ANIM_FRAME > ANIM_MS * 2 - 1)
     return;

  unsigned long total_frames = SEQ_MS / ANIM_FRAME;
  unsigned long frames = anim_prog / ANIM_FRAME;
  
  switch (sequence) {
    /*
    case 0:
    case 1: {
      clear_matrix();
      int i = (anim_prog) / 100;
      for (int x = 0; x < 6; x++) 
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            float xval = (sinf((i / 3.0f) + (x * (3.14 / 6.0f))) * 3) + 2;
            float yval = (sinf((i / 3.0f) + (y * (3.14 / 6.0f))) * 3) + 2;
            float val = (xval + yval) * 0.5f;
            if (z > val && z < val + 1)
              set_led(x,y,z,true);
          }
      break;
    }
    */
    /*
    case 8: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            if (z == 0 && (anim_prog % 5) == 1) {
              if (random(0,20) == 0)
                set_led(x,y,z,true);
            }
            if (z > 0 && (anim_prog % 15) == 0) {
              if (get_led(x,y,z-1) && !get_led(x,y,z)) {
                set_led(x,y,z-1, false);
                set_led(x,y,z,true); 
              }
            }
          }
      break;  
    }
    */
    case 7: {
      clear_matrix();
      int i = (anim_prog) / ANIM_FRAME;
      for (int x = 0; x < 6; x++) { 
        float vx = sinf((i / 3.0f) + (x * 3.14 / 6.0f)) * 3.0f + 2.0f;
        for (int y = 0; y < 6; y++) {
          for (int z = 0; z < 6; z++) {
            if (z > vx && z < vx + 1)
              set_led(x,y,z,true);
          }
        }
      }
      break;
    }
    case 6: { //sideways wave
      clear_matrix();
      int i = (anim_prog % (ANIM_FRAME * 15)) / ANIM_FRAME;
      for (int x = 0; x < 6; x++) 
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int v = x - i + z;
            if (v > -5 && v < 0)
              set_led(x,y,z,true);
          }
          break;
    }
    case 5: { //columns
      clear_matrix();
      int i = (anim_prog % (ANIM_FRAME * 5)) / ANIM_FRAME;
      draw_line(i, 0, 0, i, 0, 5, true);
      draw_line(5 - i, 5, 0, 5 - i, 5, 5, true);
      draw_line(0, 5 - i, 0, 0, 5 - i, 5, true);
      draw_line(5, i, 0, 5, i, 5, true);
      break;
    }
    case 1: {
      if (anim_prog < ANIM_FRAME) {
        all_matrix(); 
      } else {
        int area = 6 * 6 * 6;
        float fact = area / total_frames;
        float cnt = 0;
        while (true) {
          int x = random(0, 6);
          int y = random(0, 6);
          int z = random(0, 6);
          if (get_led(x,y,z)) {
            cnt += 1;
            set_led(x,y,z,0);
          }
          if (cnt > fact)
            break;
        }
      }
      break;
    }
    case 2: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / (ANIM_FRAME * 2);
            set_led(x,y,z, !((x+off)%2)&&!((y+off)%1)&&!((z+off)%1));
          }
      break;
    }
    case 3: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / (ANIM_FRAME * 2);
            set_led(x,y,z, !((x+off)%1)&&!((y+off)%2)&&!((z+off)%1));
          }
      break;
    }
    case 4: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / (ANIM_FRAME * 2);
            set_led(x,y,z, !((x+off)%1)&&!((y+off)%1)&&!((z+off)%2));
          }
      break;
    }
    default: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / (ANIM_FRAME * 2);
            set_led(x,y,z, !((x+off)%2)&&!((y+off)%2)&&!((z+off)%2));
          }
      break;
    }
  }
}

inline void fill_bytes(byte *output, byte *input, int count) {
  *output = 0;
  for (int i = 0; i < count; i++)
    if (input[i])
      *output |= (1 << i);
}

void convert_matrix() {
  for (int layer = 0; layer < LAYER_COUNT; layer++) {
    int a_off = layer * ANODE_COUNT;
    fill_bytes(&converted_matrix[layer][0], &matrix[a_off     ], 8);
    fill_bytes(&converted_matrix[layer][1], &matrix[a_off + 8 ], 8);
    fill_bytes(&converted_matrix[layer][2], &matrix[a_off + 16], 8);
    fill_bytes(&converted_matrix[layer][3], &matrix[a_off + 24], 8);
    fill_bytes(&converted_matrix[layer][4], &matrix[a_off + 32], 4);
  }
}

void draw_matrix() {
  for (int layer = 0; layer < LAYER_COUNT; layer++) {
    digitalWrite(layer + FIRST_LAYER, HIGH);
       
    PORTA =  converted_matrix[layer][0];
    PORTB =  converted_matrix[layer][1];
    PORTC =  converted_matrix[layer][2];
    PORTL =  converted_matrix[layer][3];
    PORTD |= converted_matrix[layer][4];
    
    //start_micros = micros();
    //while (micros() - start_micros < LAYER_ON_TIME);
    delayMicroseconds(LAYER_ON_TIME);

    PORTA = PORTB = PORTC = PORTL = 0;
    PORTD &= 0b11110000;
   
    digitalWrite(layer + FIRST_LAYER, LOW);
  }  
}

void loop() {
  long _last_recv, _millis;
  int matrix_count = 27;
  clear_matrix();

  _last_recv = -REC_MS;
  last_frame = 0;
  
  while (true) {  
    if (Serial.available() >= matrix_count) {
      Serial.readBytes(&matrix_buffer[0], matrix_count);
      
      for (int i = 0; i < matrix_count; i++) {
        for (int j = 0; j < 8; j++) {
          int bitIndex = i * 8 + j;
          //matrix[bitIndex] = bitRead(matrix_buffer[i], 7 - j);
          matrix[bitIndex] = (matrix_buffer[i] >> (7 - j)) & 1;
        }
      }
      
      convert_matrix();
      _last_recv = millis();
    } else {
      if (millis() - _last_recv > REC_MS) { //play idle animation after REC_MS
        if (millis() - last_frame > ANIM_MS) { //update animation every ANIM_MS
          idle_anim(millis());
          
          convert_matrix();
          last_frame = millis();
        }
      }
    }

    draw_matrix();
  }
}
