#include <RGBmatrixPanel.h>
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define PLAYER1 1
#define PLAYER2 -1

int board[7][8] = {};
// 盤面の状況
int state = 0;
// 0 : 初期状態(モード選択)
// 1 : バトルモード
int point_answer[5] = {};
// [0]:player
// [1]:x1
// [2]:y1
// [3]:x2
// [4]:y2
int player = PLAYER1;

int x_ctr1;
int y_ctr1;
int prevx_ctr1;
int prevy_ctr1;
int x_ctr2;
int y_ctr2;
int prevx_ctr2;
int prevy_ctr2;

int droppoint = 3;

int drop_board(int player, int x);
int drop_print(int x, int player);
void check();
void reverse_board();
void reverse_print();
void reverse_miniprint(int x, int y,int player);
void reverse_miniprint2(int x, int y,int player);

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  matrix.begin();
  Serial.begin(9600);
  pinMode(8, INPUT);
  //pinMode(9,INPUT);

  //matrix.setCursor(4, 10);
  //matrix.println("  TIME");
}

void loop() {
  x_ctr1 = analogRead(12);
  y_ctr1 = analogRead(13);
  x_ctr2 = analogRead(14);
  y_ctr2 = analogRead(15);


  if (state == 0) {
    matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(0, 0, 7));
    
    matrix.drawLine(29, 0, 29, 31, matrix.Color333(7, 7, 7));
    state = 1;
  }
  else if (state == 1) {
    if (prevx_ctr1 < 800 && x_ctr1 >= 800) {
      if (droppoint > 0) {
        Serial.println("Left");
        droppoint--;
        matrix.fillRect(0, 0, 27, 7, matrix.Color333(0, 0, 0));
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(0, 0, 7));
        //matrix.drawLine(droppoint*3, 4, droppoint*3+2, 4, matrix.Color333(0, 7, 0));
      }
    }
    else if (prevx_ctr1 > 200 && x_ctr1 < 200) {
      if (droppoint < 6) {
        Serial.println("Right");
        droppoint++;
        matrix.fillRect(0, 0, 28, 7, matrix.Color333(0, 0, 0));
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(0, 0, 7));
        //matrix.drawLine(droppoint*3, 4, droppoint*3+2, 4, matrix.Color333(0, 7, 0));
      }
    }
    else if (prevy_ctr1 < 800 && y_ctr1 > 800) {
      Serial.println("Up");
      reverse_print();
      check();
      droppoint = 3;
      state = 2;
      if (point_answer[0] == 0) {
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(7, 0, 0));
        player = 1;
      }
      else {
        Serial.println("line is built.");
        state = 3;
      }
    }
    else if (prevy_ctr1 > 200 && y_ctr1 < 200) {
      Serial.println("Down");
      drop_board(PLAYER1, droppoint);
      drop_print(droppoint, PLAYER1);
      droppoint = 3;
      player = PLAYER2;
      state = 2;

      check();
      for (int y = 0; y <= 6; y++) {
        for (int x = 0; x <= 7; x++) {
          Serial.print(board[y][x]);
        }
        Serial.println("");
      }
      for (int j = 0; j < 5; j++) {
        Serial.println(point_answer[j]);
      }

      if (point_answer[0] == 0) {
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(7, 0, 0));
        player = -1;
      }
      else {
        Serial.println("line is built.");
        state = 3;
      }
    }
  }
  else if (state == 2) {
    if (prevx_ctr2 < 800 && x_ctr2 >= 800) {
      if (droppoint > 0) {
        Serial.println("Left2");
        droppoint--;
        matrix.fillRect(0, 0, 28, 7, matrix.Color333(0, 0, 0));
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(7, 0, 0));
        //matrix.drawLine(droppoint*3, 4, droppoint*3+2, 4, matrix.Color333(0, 7, 0));
      }
    }
    else if (prevx_ctr2 > 200 && x_ctr2 < 200) {
      if (droppoint < 6) {
        Serial.println("Right2");
        droppoint++;
        matrix.fillRect(0, 0, 28, 7, matrix.Color333(0, 0, 0));
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(7, 0, 0));
        //matrix.drawLine(droppoint*3, 4, droppoint*3+2, 4, matrix.Color333(0, 7, 0));
      }
    }
    else if (prevy_ctr2 < 800 && y_ctr2 > 800) {

    }
    else if (prevy_ctr2 > 200 && y_ctr2 < 200) {
      Serial.println("Down2");
      drop_board(PLAYER2, droppoint);
      drop_print(droppoint, PLAYER2);
      droppoint = 3;

      check();

      if (point_answer[0] == 0) {
        matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(0, 0, 7));
        player = PLAYER1;
        state = 1;
      }
      else {
        Serial.println("line is built.");
        state = 3;
      }
    }
  }

  else if (state == 3) {
    delay(500);
    matrix.drawLine(point_answer[1] * 4 + 1, 9 + point_answer[2] * 4, point_answer[3] * 4 + 1, 9 + point_answer[4] * 4, matrix.Color333(0, 0, 0));
    delay(400);
    matrix.drawLine(point_answer[1] * 4 + 1, 9 + point_answer[2] * 4, point_answer[3] * 4 + 1, 9 + point_answer[4] * 4, matrix.Color333(7, 7, 7));
    delay(400);
  }

  prevx_ctr1 = x_ctr1;
  prevy_ctr1 = y_ctr1;
  prevx_ctr2 = x_ctr2;
  prevy_ctr2 = y_ctr2;
}

int drop_board(int player, int x) {
  /* まだすべて埋まっていないとき */
  if (board[6][x] <= 6) {
    board[5 - board[6][x]][x] = player;
    board[5 - board[6][x]][7]++;
    board[6][x]++;
    return 6 - board[6][x];
  }
  /* すべて埋まっている列 */
  else {
    return -1;
  }
}

int drop_print(int x, int player) {
  int i;
  int goalpointy = 32 - board[6][x] * 4;
  int r_color ;
  int b_color ;

  if (player == PLAYER1) {
    r_color = 0;
    b_color = 7;
  }
  else {
    r_color = 7;
    b_color = 0;
  }

  for (i = 1; i < goalpointy; i++) {
    matrix.fillRect(x * 4, i, 3, 1, matrix.Color333(0, 0, 0));
    matrix.fillRect(x * 4, i + 1, 3, 3, matrix.Color333(r_color, 0, b_color));
    delay(5);
  }
}

void check() {
  int x = 0;
  int y = 0;
  // board参照用ループカウンタ

  /*縦の確認*/
  for (x = 0; x <= 6; x++) {
    if (board[6][x] >= 4) {
      // 埋数が4以上のみ
      for (y = 0; y <= 2; y++) {
        if (board[y][x] + board[y + 1][x] + board[y + 2][x] + board[y + 3][x] == 4) {
          point_answer[0] = 1;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x;
          point_answer[4] = y + 3;
        }
        else if (board[y][x] + board[y + 1][x] + board[y + 2][x] + board[y + 3][x] == -4) {
          point_answer[0] = -1;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x;
          point_answer[4] = y + 3;
        }
      }
    }
  }

  /*横の確認*/
  for (y = 0; y <= 5; y++) {
    if (board[y][7] >= 4) {
      for (x = 0; x <= 3; x++) {
        if (board[y][x] + board[y][x + 1] + board[y][x + 2] + board[y][x + 3] == 4) {
          point_answer[0] = PLAYER1;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x + 3;
          point_answer[4] = y;
        }
        else if (board[y][x] + board[y][x + 1] + board[y][x + 2] + board[y][x + 3] == -4) {
          point_answer[0] = PLAYER2;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x + 3;
          point_answer[4] = y;
        }
      }
    }
  }

  /*左上から右下の確認*/
  for (x = 0; x <= 3; x++) {
    for (y = 0; y <= 2; y++) {
      /*始点が埋まっているときのみ*/
      if (board[y][x] != 0) {
        if (board[y][x] + board[y + 1][x + 1] + board[y + 2][x + 2] + board[y + 3][x + 3] == 4) {
          point_answer[0] = PLAYER1;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x + 3;
          point_answer[4] = y + 3;
        }
        else if (board[y][x] + board[y + 1][x + 1] + board[y + 2][x + 2] + board[y + 3][x + 3] == -4) {
          point_answer[0] = PLAYER2;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x + 3;
          point_answer[4] = y + 3;
        }
      }
    }
  }

  /*右上から左下への確認*/
  for (x = 0; x <= 3; x++) {
    for (y = 3; y <= 5; y++) {
      if (board[y][x] != 0) {
        if (board[y][x] + board[y - 1][x + 1] + board[y - 2][x + 2] + board[y - 3][x + 3] == 4) {
          point_answer[0] = PLAYER1;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x + 3;
          point_answer[4] = y - 3;
        }
        else if (board[y][x] + board[y - 1][x + 1] + board[y - 2][x + 2] + board[y - 3][x + 3] == -4) {
          point_answer[0] = PLAYER2;
          point_answer[1] = x;
          point_answer[2] = y;
          point_answer[3] = x + 3;
          point_answer[4] = y - 3;
        }
      }
    }
  }
}

void reverse_board() {
  int x;
  int y;
  int temp[7][8];
  memcpy(temp, board, sizeof(board));

  /*盤面を初期化(埋数はそのままでよい)*/
  for (x = 0; x <= 6; x++) {
    for (y = 0; y <= 5; y++) {
      board[y][x] = 0;
    }
  }

  for (x = 0; x <= 6; x++) {
    for (y = 0; y < board[6][x]; y++) {
      board[5 - y][x] = temp[6 - temp[6][x] + y][x];
    }
  }
}

void reverse_print() {
  matrix.fillRect(droppoint * 4, 1, 3, 3, matrix.Color333(0, 0, 0));
  for (int i = 0; i <= 5; i++){ 
    for(int j = 0; j <= 6;j++){
      if(board[i][j]==1) reverse_miniprint(j,i,1);
      else if(board[i][j]==-1) reverse_miniprint(j,i,-1);
    }
  }
  
  reverse_board();

  for (int i = 5; i >= 0; i--){ 
    for(int j = 0; j <= 6;j++){
      if(board[i][j]==1) reverse_miniprint2(j,i,1);
      else if(board[i][j]==-1) reverse_miniprint2(j,i,-1);
    }
  }
}

void reverse_miniprint(int x, int y,int player) {
  int x_start = 4 * x;
  int y_start = 8 + 4 * y;
  int r_color;
  int b_color;

  if(player == 1){
    r_color = 0;
    b_color = 7;
  }
  else{
    r_color = 7;
    b_color = 0;
  }

  for (int i = y_start ; i > -3; i--) {
    matrix.fillRect(x_start, i+2, 3, 1, matrix.Color333(0, 0, 0));
    matrix.fillRect(x_start, i-1, 3, 3, matrix.Color333(r_color, 0, b_color));
    delay(3);
 }
}

void reverse_miniprint2(int x, int y,int player){
  int x_start = 4 * x;
  int y_end = 8 + 4 * y;
  int r_color;
  int b_color;

  if(player == 1){
    r_color = 0;
    b_color = 7;
  }
  else{
    r_color = 7;
    b_color = 0;
  }

  for (int i = -2 ; i <= y_end; i++) {
    matrix.fillRect(x_start, i-1, 3, 1, matrix.Color333(0, 0, 0));
    matrix.fillRect(x_start, i, 3, 3, matrix.Color333(r_color, 0, b_color));
    delay(3);
  }
}
