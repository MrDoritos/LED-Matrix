const int FIRST_LAYER = 2;
const int FIRST_ANODE = 14;
const int LAYER_COUNT = 6;
const int ANODE_COUNT = 36;
const float LAYER_ON_TIME = 750.0f;
const int MATRIX_SIZE = ANODE_COUNT * LAYER_COUNT;
unsigned long current_millis;
unsigned long start_micros;
unsigned long last_frame;
unsigned long tick = 0;

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  Serial.begin(115200);
  Serial.setTimeout(10);
  tick = 0;
  last_frame = 0;
  start_micros = micros();
  for (int i = 0; i < LAYER_COUNT; i++) {
    pinMode(i + FIRST_LAYER, OUTPUT);
  }
  for (int i = 0; i < ANODE_COUNT; i++) {
    pinMode(i + FIRST_ANODE, OUTPUT);  
  }
}

byte matrix[MATRIX_SIZE];
byte matrix_buffer[256];

void clear_matrix() {
    memset(&matrix, 0, MATRIX_SIZE);  
}

void all_matrix() {
    memset(&matrix, 1, MATRIX_SIZE);  
}

void set_led(int x, int y, int z, bool value) {
  matrix[(z * 36) + (y * 6) + x] = value;
}

void draw_line(int x1, int y1, int z1, int x2, int y2, int z2, bool value)
{
  //vector<vector<int> > ListOfPoints;
  //ListOfPoints.push_back({ x1, y1, z1 });
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
      //ListOfPoints.push_back({ x1, y1, z1 });
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
      //ListOfPoints.push_back({ x1, y1, z1 });
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
     // ListOfPoints.push_back({ x1, y1, z1 });
     set_led(x1, y1, z1, value);
    }
  }
}

void idle_anim(unsigned long tick) {
  unsigned long animation_length = 15000;
  unsigned long animation_count = 8;
  unsigned long total_length = animation_count * animation_length;
  unsigned long sequence = (tick % total_length) / animation_length;
  unsigned long anim_prog = tick % animation_length;

  
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
    case 7: {
      clear_matrix();
      int i = (anim_prog) / 100;
      for (int x = 0; x < 6; x++) 
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            float val = (sinf((i / 3.0f) + (x * (3.14 / 6.0f))) * 3) + 2;
            if (z > val && z < val + 1)
              set_led(x,y,z,true);
          }
      break;
    }
    case 6: { //sideways wave
      clear_matrix();
      int i = (anim_prog % 1500) / 100;
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
      int i = (anim_prog % 500) / 100;
      draw_line(i, 0, 0, i, 0, 5, true);
      draw_line(5 - i, 5, 0, 5 - i, 5, 5, true);
      draw_line(0, 5 - i, 0, 0, 5 - i, 5, true);
      draw_line(5, i, 0, 5, i, 5, true);
      break;
    }
    case 1: {
      if (anim_prog < 100) {
        all_matrix(); 
      } else {
        set_led(random(0,6), random(0,6), random(0,6), false);
      }
      break;
    }
    case 2: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%2)&&!((y+off)%1)&&!((z+off)%1));
          }
      break;
    }
    case 3: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%1)&&!((y+off)%2)&&!((z+off)%1));
          }
      break;
    }
    case 4: {
      for (int x = 0; x < 6; x++)
        for (int y = 0; y < 6; y++)
          for (int z = 0; z < 6; z++) {
            int off = anim_prog / 200;
            set_led(x,y,z, !((x+off)%1)&&!((y+off)%1)&&!((z+off)%2));
          }
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
  }
}

void draw_matrix() {
  for (int layer = 0; layer < LAYER_COUNT; layer++) {
    digitalWrite(layer + FIRST_LAYER, HIGH);
    
    for (int anode = 0; anode < ANODE_COUNT; anode++) {
        if (matrix[(layer * 36) + anode])
          digitalWrite(anode + FIRST_ANODE, HIGH);
    }    
    
    start_micros = micros();
    
    while (micros() - start_micros < LAYER_ON_TIME);
    
    for (int anode = 0; anode < ANODE_COUNT; anode++) {
         if (matrix[(layer * 36) + anode])
            digitalWrite(anode + FIRST_ANODE, LOW);
    }    
    digitalWrite(layer + FIRST_LAYER, LOW);
  }  
}

void loop() {
  long _last_recv, _millis;
  clear_matrix();

  _last_recv = -5000;
  
  while (true) {  
    if (Serial.available() >= 27) {
      Serial.readBytes(&matrix_buffer[0], 27);
     
      for (int i = 0; i < 27; i++) {
        for (int j = 0; j < 8; j++) {
          int bitIndex = i * 8 + j;
          matrix[bitIndex] = bitRead(matrix_buffer[i], 7 - j);
        }
      }
      
      _last_recv = millis();
    }
    else 
    if (millis() - _last_recv > 5000) { //play idle animation after 5 seconds
      if (millis() - last_frame > 40) { //update animation every 40 ms
        idle_anim(millis() + 14000);
        last_frame = millis();
      }
    }

    draw_matrix();
  }
}
