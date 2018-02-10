  #include <LiquidCrystal.h>
  LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
  //LiquidCrystal library used for lcd
  int adc_key_val[5] ={30, 150, 360, 535, 760 };
  int NUM_KEYS = 5;
  int adc_key_in;
  int key=-1;
  int oldkey=-1;
  int lcdrank = 6;
  int lcdfile = 0;
  int pin = 13;
  int pin2 = 3;
  
  int type[8][8] = {  //matrix that shows type of piece, 1-pawn,2-knight,etc
    {4,2,3,5,6,3,2,4},
    {1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1},
    {4,2,3,5,6,3,2,4}};
  int color[8][8] = {  //matrix that shows color of piece, 1-white,2-black
    {2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}};
  int counter[8][8] = {  //matrix that shows number of moves made by each piece
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}};
  //boolean variables show state of whether player is in check, is castling, etc  
  boolean legal = true;
  boolean check = false;
  boolean wcheck = false;
  boolean bcheck = false;
  boolean take = false;
  boolean enpassant = false;
  boolean prom = false;
  boolean castle = false;
  boolean castle6 = false;
  boolean castle2 = false;
  //rank and file variables give old and new postion of piece
  int owrank;
  int owfile;
  int nwrank;
  int nwfile;
  int obrank;
  int obfile;
  int nbrank;
  int nbfile;
  //king rank and file variables help in determining check
  int wkingrank;
  int wkingfile;
  int bkingrank;
  int bkingfile;
  int btaketype;
  int btakecolor;
  int btakecounter;
  int wtaketype;
  int wtakecolor;
  int wtakecounter;
  //determines which turn it is, odd-white,even-black
  int turn = 1;

void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  //pinMode(pin, OUTPUT);
  //pinMode(pin2, OUTPUT);
}

void loop(){
  if ((turn % 2) == 1){ //white's turn to move
    
    //digitalWrite(pin, HIGH);
    
    //initial boolean values
    legal = true;
    check = false;
    wcheck = false;
    bcheck = false;
    take = false;
    enpassant = false;
    
    //prompting white to select a piece to move
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("white ");
    lcd.setCursor(9,0);
    lcd.print("select ");
    lcd.setCursor(9,1);
    lcd.print("piece  ");
    delay(2000);
    moveselection(1);
    
    //prompting to select square to move to
    lcd.setCursor(9,0);
    lcd.print("make   ");
    lcd.setCursor(9,1);
    lcd.print("move   ");
    moveselection(2);
    
    Serial.println("White - Enter rank, press ENTER");
    while(Serial.available()==0){}
      owrank = Serial.parseInt();
      owrank = 8 - owrank;
      Serial.println(owrank);
      
    Serial.println("White - Enter file, press ENTER");
    while(Serial.available()==0){}
      owfile = Serial.parseInt();
      owfile = owfile - 1;
      Serial.println(owfile);
      
    Serial.println("White - Enter new rank, press ENTER");
    while(Serial.available()==0){}
      nwrank = Serial.parseInt();
      nwrank = 8 - nwrank;
      Serial.println(nwrank);

    Serial.println("White - Enter new file, press ENTER");
    while(Serial.available()==0){}
      nwfile = Serial.parseInt();
      nwfile = nwfile - 1;
      Serial.println(nwfile);

    //illegal move if white piece not selected
    if (color[owrank][owfile] != 1){ 
      legal = false;
      Serial.println("Chose piece of wrong color");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Chose Wrong");
      lcd.setCursor(0,1);
      lcd.print("Color");
      delay(2000);
    }
    
    //piece must move to new square
    if (owrank == nwrank && owfile == nwfile){ 
      legal = false;
      Serial.println("Must move piece");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Must Move Piece");
      delay(2000);
    }
    
    //old or new square selected must be on board
    if (owrank < 0 || owrank > 7){ 
      legal = false;
      Serial.println("Selection not on board");
    }
    else if (owfile < 0 || owfile > 7){
      legal = false;
      Serial.println("Selection not on board");
    }
    else if (owrank < 0 || owrank > 7){
      legal = false;
      Serial.println("Selection not on board");
    }
    else if (owrank < 0 || owrank > 7){
      legal = false;
      Serial.println("Selection not on board");
    }
    
    //type of piece selected determines which function will be used to test if the move is legal
    if (type[owrank][owfile] == 1){
      wpawn(owrank,owfile,nwrank,nwfile);
    }
    else if (type[owrank][owfile] == 2){
      knightm(owrank,owfile,nwrank,nwfile);
    }
    else if (type[owrank][owfile] == 3){
      bishopm(owrank,owfile,nwrank,nwfile);
    }
    else if (type[owrank][owfile] == 4){
      rookm(owrank,owfile,nwrank,nwfile);
    }
    else if (type[owrank][owfile] == 5){
      queenm(owrank,owfile,nwrank,nwfile);
    }
    else if (type[owrank][owfile] == 6){
      kingm(owrank,owfile,nwrank,nwfile);
    }
    
    //if a piece was taken or en pessant employed it must be known so it is known how to manipulate the board
    if (color[nwrank][nwfile] == 2 && !enpassant){
      take = true;
    }
    else{
      take = false;
    }
      
    //if the move is legal pieces can be moved to new locations
    //if a piece was taken, it needs to be stored in temporary variables
    //so that it can be moved back if it is determined that the move was illegal since it put player in check
    if (legal){
      if (take){
        btaketype = type[nwrank][nwfile];
        btakecolor = color[nwrank][nwfile];
        btakecounter = counter[nwrank][nwfile];
      }
      else if (enpassant){
        btaketype = type[nbrank][nbfile];
        btakecolor = color[nbrank][nbfile];
        btakecounter = counter[nbrank][nbfile];
        type[nbrank][nbfile] = 0;
        color[nbrank][nbfile] = 0;
        counter[nbrank][nbfile] = 0;
      }
      else if (castle6){
        type[7][5] = 4;
        type[7][7] = 0;
        color[7][5] = 1;
        color[7][7] = 0;
        counter[7][5] = 1;
        castle6 = false;
      }
      else if (castle2){
        type[7][3] = 4;
        type[7][0] = 0;
        color[7][3] = 1;
        color[7][0] = 0;
        counter[7][3] = 1;
        castle2 = false;
      }
      type[nwrank][nwfile] = type[owrank][owfile];
      type[owrank][owfile] = 0;
      color[nwrank][nwfile] = color[owrank][owfile];
      color[owrank][owfile] = 0;
      counter[nwrank][nwfile] = counter[owrank][owfile] + 1;
      counter[owrank][owfile] = 0;

      //the white king is found and checked for check
      //if check, move is illegal
      for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
          if (color[i][j] == 1){
            if (type[i][j] == 6){
              wkingrank = i;
              wkingfile = j;
                break;
            }
          }
        }
      }
      checkcheck(wkingrank,wkingfile);
      if (check){
        wcheck = true;
        check = false;
      }
      
      //black king found and checked for check
      //if check, other player is in check  
      for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
          if (color[i][j] == 2){
            if (type[i][j] == 6){
              bkingrank = i;
              bkingfile = j;
              break;
            }
          }
        }
      }
      checkcheck(bkingrank,bkingfile);
      if (check){
        bcheck = true;
          check = false;
      }
      
    }
    
    //if move put yourself in check, move is illegal and pieces have to be moved back
    //if a piece was taken, pieces have to be recovered from temporary variables
    if (wcheck){
      type[owrank][owfile] = type[nwrank][nwfile];
      color[owrank][owfile] = color[nwrank][nwfile];
      counter[owrank][owfile] = counter[nwrank][nwfile] - 1;
      if (take){
        type[nwrank][nwfile] = btaketype;
        color[nwrank][nwfile] = btakecolor;
        counter[nwrank][nwfile] = btakecounter;
      }
      else if (enpassant){
        type[nbrank][nbfile] = btaketype;
        color[nbrank][nbfile] = btakecolor;
        counter[nbrank][nbfile] = btakecounter;
        type[nwrank][nwfile] = 0;
        color[nwrank][nwfile] = 0;
        counter[nwrank][nwfile] = 0;
      }
      else{
        type[nwrank][nwfile] = 0;
        color[nwrank][nwfile] = 0;
        counter[nwrank][nwfile] = 0;
      }
    }
    
    //if move is legal and doesn't put player in check, move is valid
    //matrices displayed in serial monitor
    if(legal && !wcheck){
      turn++;
      //digitalWrite(pin,LOW);
      for (int i=0;i<8;i++){
        Serial.println("");
        for (int j=0;j<8;j++){
          Serial.print(counter[i][j]);
        }
      }
      Serial.println("");
      for (int i=0;i<8;i++){
        Serial.println("");
        for (int j=0;j<8;j++){
          Serial.print(color[i][j]);
        }
      }
      Serial.println("");
      for (int i=0;i<8;i++){
        Serial.println("");
        for (int j=0;j<8;j++){
          Serial.print(type[i][j]);
        }
      }
      Serial.println("");
    }
    //if move is illegal, player is told and new move must be chosen
    else if(!legal){
      Serial.println("Illegal Move");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Illegal Move");
      delay(2000);
      }
    //if move puts yourself in check, illegal and new move must be chosen
    else if (wcheck){
      Serial.println("Illegal - You're in Check!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Illegal You're");
      lcd.setCursor(0,1);
      lcd.print("in Check!");
      delay(2000);
    }
    //if other player put in check, player is told
    if (bcheck){
      Serial.println("Check!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Check!");
      delay(2000);
    }   
  }
  
  //if turn even, black is to move
  //works same as above code, but for black instead of white
  if ((turn % 2) == 0){
    
    //digitalWrite(pin2,HIGH);
    
    legal = true;
    check = false;
    wcheck = false;
    bcheck = false;
    take = false;
    enpassant = false;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("black ");
    lcd.setCursor(9,0);
    lcd.print("select ");
    lcd.setCursor(9,1);
    lcd.print("piece  ");
    delay(2000);
    moveselection(1);
    
    lcd.setCursor(9,0);
    lcd.print("make   ");
    lcd.setCursor(9,1);
    lcd.print("move   ");
    moveselection(2);
    
    Serial.println("White - Enter rank, press ENTER");
    while(Serial.available()==0){}
      owrank = Serial.parseInt();
      owrank = 8 - owrank;
      Serial.println(owrank);
      
    Serial.println("White - Enter file, press ENTER");
    while(Serial.available()==0){}
      owfile = Serial.parseInt();
      owfile = owfile - 1;
      Serial.println(owfile);
      
    Serial.println("White - Enter new rank, press ENTER");
    while(Serial.available()==0){}
      nwrank = Serial.parseInt();
      nwrank = 8 - nwrank;
      Serial.println(nwrank);

    Serial.println("White - Enter new file, press ENTER");
    while(Serial.available()==0){}
      nwfile = Serial.parseInt();
      nwfile = nwfile - 1;
      Serial.println(nwfile);

    if (color[obrank][obfile] != 2){ //when it is white's move, player must select white piece
      legal = false;
      Serial.println("Chose piece of wrong color");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Chose Wrong");
      lcd.setCursor(0,1);
      lcd.print("Color");
      delay(2000);
    }
    
    if (obrank == nbrank && obfile == nbfile){ //piece must move to new square
      legal = false;
      Serial.println("Must move piece");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Must Move Piece");
      delay(2000);
    }
    
    if (type[obrank][obfile] == 1){
      bpawn(obrank,obfile,nbrank,nbfile);
    }
    else if (type[obrank][obfile] == 2){
      knightm(obrank,obfile,nbrank,nbfile);
    }
    else if (type[obrank][obfile] == 3){
      bishopm(obrank,obfile,nbrank,nbfile);
    }
    else if (type[obrank][obfile] == 4){
      rookm(obrank,obfile,nbrank,nbfile);
    }
    else if (type[obrank][obfile] == 5){
      queenm(obrank,obfile,nbrank,nbfile);
    }
    else if (type[obrank][obfile] == 6){
      kingm(obrank,obfile,nbrank,nbfile);
    }
    
    if (color[nbrank][nbfile] == 2 && !enpassant){
      take = true;
    }
    else{
      take = false;
    }
      
    
    if (legal){
      if (take){
        wtaketype = type[nbrank][nbfile];
        wtakecolor = color[nbrank][nbfile];
        wtakecounter = counter[nbrank][nbfile];
      }
      else if (enpassant){
        wtaketype = type[nwrank][nwfile];
        wtakecolor = color[nwrank][nwfile];
        wtakecounter = counter[nwrank][nwfile];
        type[nwrank][nwfile] = 0;
        color[nwrank][nwfile] = 0;
        counter[nwrank][nwfile] = 0;
      }
      else if (castle6){
        type[0][5] = 4;
        type[0][7] = 0;
        color[0][5] = 2;
        color[0][7] = 0;
        counter[0][5] = 1;
        castle6 = false;
      }
      else if (castle2){
        type[0][3] = 4;
        type[0][0] = 0;
        color[0][3] = 2;
        color[0][0] = 0;
        counter[0][3] = 1;
        castle2 = false;
      }
      type[nbrank][nbfile] = type[obrank][obfile];
      type[obrank][obfile] = 0;
      color[nbrank][nbfile] = color[obrank][obfile];
      color[obrank][obfile] = 0;
      counter[nbrank][nbfile] = counter[obrank][obfile] + 1;
      counter[obrank][obfile] = 0;

      for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
          if (color[i][j] == 1){
            if (type[i][j] == 6){
              wkingrank = i;
              wkingfile = j;
              break;
            }
          }
        }
      }
      checkcheck(wkingrank,wkingfile);
      if (check){
        wcheck = true;
        check = false;
      }
      
      for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
          if (color[i][j] == 2){
            if (type[i][j] == 6){
              bkingrank = i;
              bkingfile = j;
              break;
            }
          }
        }
      }
      checkcheck(bkingrank,bkingfile);
      
      if (check){
        bcheck = true;
        check = false;
      }
      
    }
    
    if (bcheck){
      type[obrank][obfile] = type[nbrank][nbfile];
      color[obrank][obfile] = color[nbrank][nbfile];
      counter[obrank][obfile] = counter[nbrank][nbfile] - 1;
      if (take){
        type[nbrank][nbfile] = wtaketype;
        color[nbrank][nbfile] = wtakecolor;
        counter[nbrank][nbfile] = wtakecounter;
      }
      else if (enpassant){
        type[nwrank][nwfile] = wtaketype;
        color[nwrank][nwfile] = wtakecolor;
        counter[nwrank][nwfile] = wtakecounter;
        type[nbrank][nbfile] = 0;
        color[nwrank][nbfile] = 0;
        counter[nbrank][nbfile] = 0;
      }
      else{
        type[nbrank][nbfile] = 0;
        color[nbrank][nbfile] = 0;
        counter[nbrank][nbfile] = 0;
      }
    }
    
    if(legal && !bcheck){
      turn++;
      //digitalWrite(pin2,LOW);
      for (int i=0;i<8;i++){
        Serial.println("");
        for (int j=0;j<8;j++){
          Serial.print(counter[i][j]);
        }
      }
      Serial.println("");
      for (int i=0;i<8;i++){
        Serial.println("");
        for (int j=0;j<8;j++){
          Serial.print(color[i][j]);
        }
      }
      Serial.println("");
      for (int i=0;i<8;i++){
        Serial.println("");
        for (int j=0;j<8;j++){
          Serial.print(type[i][j]);
        }
      }
      Serial.println("");
    }
    else if(!legal){
      Serial.println("Illegal Move");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Illegal Move");
      delay(2000);
      }
    else if (bcheck){
      Serial.println("Illegal - You're in Check!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Illegal You're");
      lcd.setCursor(0,1);
      lcd.print("in Check!");
      delay(2000);
    }
    if (wcheck){
      Serial.println("Check!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Check!");
      delay(2000);
    }   
  }
}

//function used to compute validity of a white pawn move
void wpawn(int wpoldrank, int wpoldfile, int wpnewrank, int wpnewfile){
  //if pawn moves forward, it must move only 1 square (or 2 if hasn't moved yet)
  if (wpnewfile == wpoldfile){
    if ((wpoldrank - wpnewrank) == 2){
      if (counter[wpoldrank][wpoldfile] != 0){
        legal = false;
        Serial.println("Illegal WP1");
      }
      if (type[wpoldrank-1][wpoldfile] != 0 || type[wpnewrank][wpnewfile] != 0){
        legal = false;
        Serial.println("Illegal WP2");
      }
    }
    else if ((wpoldrank - wpnewrank) == 1){
      if (type[wpoldrank-1][wpoldfile] != 0){
        legal = false;
        Serial.println("Illegal WP3");
      }
    }
    else{
      legal = false;
      Serial.println("Illegal WP4");
    }
  }
  
  //if pawn moves diagonally, it must move only 1 square and must be taking a piece, either normally or with en passant
  if (wpnewfile != wpoldfile){
    if (abs(wpnewfile - wpoldfile) == 1){
      if (color[wpnewrank][wpnewfile] != 2){
        if (color[wpnewrank][wpnewfile] == 0 && color[wpnewrank+1][wpnewfile] == 2 && type[wpnewrank+1][wpnewfile] == 1 && counter[wpnewrank+1][wpnewfile] == 1 && obrank == wpnewrank-1 && obfile == wpnewfile){
          enpassant = true;
        }
        else{
          legal = false;
          Serial.println("Illegal WP5");
        }
      }
    }
    else{
      legal = false;
      Serial.println("Illegal WP6");
    }
  }
  
  //if pawn reaches last rank, it can promote (not yet working)
  if (wpnewrank == 0){
    prom = true;
  }
  else{
    prom = false;
  }
}

//determines validity of black pawn move
//works same as wpawn, but for black pawn instead of white
void bpawn(int bpoldrank, int bpoldfile, int bpnewrank, int bpnewfile){
  if (bpnewfile == bpoldfile){
    if ((bpnewrank - bpoldrank) == 2){
      if (counter[bpoldrank][bpoldfile] != 0){
        legal = false;
        Serial.println("Illegal BP1");
      }
      if (type[bpoldrank+1][bpoldfile] != 0 || type[bpnewrank][bpnewfile] != 0){
        legal = false;
        Serial.println("Illegal BP2");
      }
    }
    else if ((bpnewrank - bpoldrank) == 1){
      if (type[bpoldrank+1][bpoldfile] != 0){
        legal = false;
        Serial.println("Illegal BP3");
      }
    }
    else{
      legal = false;
      Serial.println("Illegal BP4");
    }
  }
  
  if (bpnewfile != bpoldfile){
    if (abs(bpnewfile - bpoldfile) == 1){
      if (color[bpnewrank][bpnewfile] != 1){
        if (color[bpnewrank][bpnewfile] == 0 && color[bpnewrank-1][bpnewfile] == 1 && type[bpnewrank-1][bpnewfile] == 1 && counter[bpnewrank-1][bpnewfile] == 1 && owrank == bpnewrank+1 && owfile == bpnewfile){
          enpassant = true;
        }
        else{
          legal = false;
          Serial.println("Illegal WP5");
        }
      }
    }
    else{
      legal = false;
      Serial.println("Illegal WP6");
    }
  }
  
  if (bpnewrank == 7){
    prom = true;
  }
  else{
    prom = false;
  }
}

//determines validity of a knight move (white or black)
void knightm(int knoldrank, int knoldfile, int knnewrank, int knnewfile){
  //can't move to square occupied by piece of own color
  if (color[knoldrank][knoldfile] == color[knnewrank][knnewfile]){
    legal = false;
    Serial.println("Illegal N1");
  }
  
  //knight must move 2 squares in one direction and 1 sqaure in the other
  if (abs(knnewfile - knoldfile) == 2){
    if (abs(knnewrank - knoldrank) != 1){
      legal = false;
      Serial.println("Illegal N2");
    }
  }
  
  if (abs(knnewfile - knoldfile) == 1){
    if (abs(knnewrank - knoldrank) != 2){
      legal = false;
      Serial.println("Illegal N3");
    }
  }
}

//determines validity of a bishop move (white or black)
void bishopm(int boldrank, int boldfile, int bnewrank, int bnewfile){
  //piece can't move to square occupied by piece of own color
  if (color[boldrank][boldfile] == color[bnewrank][bnewfile]){
    legal = false;
    Serial.println("Illegal B1");
  }
  
  //piece must move diagonally
  //if it does not move the same number of ranks as files, it is illegal
  if (abs(bnewfile - boldfile) != abs(boldrank - bnewrank)){
    legal = false;
    Serial.println("Illegal B2");
  }
  
  
  //there cannot be any piece in the way of the initial and final squares of the move
  //the square moved to can have an enemy piece, but not own piece
  //if an in-between square is occupied, move is illegal
  //logic repeated for each potential diagonal direction
  if(bnewfile > boldfile){
    if (bnewrank > boldrank){
      int sep = bnewfile - boldfile;
        for(int i=1; i<sep ; i++){
          if (type[boldrank+i][boldfile+i] != 0){
            legal = false;
            Serial.println("Illegal B3");
          }
        }
    }
    else if (boldrank > bnewrank){
      int sep = boldrank - bnewrank;
        for(int i=1; i<sep ; i++){
          if (type[boldrank-i][boldfile+i] != 0){
            legal = false;
            Serial.println("Illegal B4");
          }
        }
    }
  }
  else if(boldfile > bnewfile){
    if (bnewrank > boldrank){
      int sep = bnewfile - boldfile;
        for(int i=1; i<sep ; i++){
          if (type[boldrank+i][boldfile-i] != 0){
            legal = false;
            Serial.println("Illegal B5");
          }
        }
    }
    else if (boldrank > bnewrank){
      int sep = boldrank - bnewrank;
        for(int i=1; i<sep ; i++){
          if (type[boldrank-i][boldfile-i] != 0){
            legal = false;
            Serial.println("Illegal B6");
          }
        }
    }
  }
}

//determines validity of rook move (white or black)
void rookm(int roldrank, int roldfile, int rnewrank, int rnewfile){
  //piece can't move to square occupied by piece of own color
  if (color[roldrank][roldfile] == color[rnewrank][rnewfile]){
    legal = false;
    Serial.println("Illegal R1");
  }
  
  //piece must stay on either same rank or same file
  if (roldrank != rnewrank && roldfile != rnewfile){
    legal = false;
    Serial.println("Illegal R2");
  }
  
  //can't move through any pieces
  //if piece in between new and old postion, move is illegal
  //logic carried through for each potential move direction
  if (roldrank == rnewrank){
    if (roldfile > rnewfile){
      int sep = roldfile - rnewfile;
      for(int i=1; i<sep ; i++){
        if (type[roldrank][roldfile-i] != 0){
          legal = false;
          Serial.println("Illegal R3");
        }
      }
    }
    else if (rnewfile > roldfile){
      int sep = rnewfile - roldfile;
      for(int i=1; i<sep ; i++){
        if (type[roldrank][roldfile+i] != 0){
          legal = false;
          Serial.println("Illegal R4");
        }
      }
    }
  }
  else if (roldfile == rnewfile){
    if (roldrank > rnewrank){
      int sep = roldrank - rnewrank;
      for(int i=1; i<sep ; i++){
        if (type[roldrank-i][roldfile] != 0){
          legal = false;
          Serial.println("Illegal R5");
        }
      }
    }
    else if (rnewrank > roldrank){
      int sep = rnewrank - roldrank;
      for(int i=1; i<sep ; i++){
        if (type[roldrank+i][roldfile] != 0){
          legal = false;
          Serial.println("Illegal R6");
        }
      }
    }
  }
}

//determines validity of queen move (white or black)
void queenm(int qoldrank, int qoldfile, int qnewrank, int qnewfile){
  //piece can't move to square occupied by piece of own color
  if (color[qoldrank][qoldfile] == color[qnewrank][qnewfile]){
    legal = false;
    Serial.println("Illegal Q1");
  }
  
  //there cannot be a piece in the way of old and new square
  //each diagonal and each rank and file is checked (each potential move direction)
  //if there is a piece in the way in direction of move made, move is illegal
  if(abs(qnewfile-qoldfile) == abs(qnewrank-qoldrank)){
    if(qnewfile > qoldfile){
      if (qnewrank > qoldrank){
        int sep = qnewfile - qoldfile;
        for(int i=1; i<sep ; i++){
          if (type[qoldrank+i][qoldfile+i] != 0){
            legal = false;
            Serial.println("Illegal Q2");
          }
        }
      }
      else if (qoldrank > qnewrank){
        int sep = qoldrank - qnewrank;
        for(int i=1; i<sep ; i++){
          if (type[qoldrank-i][qoldfile+i] != 0){
            legal = false;
            Serial.println("Illegal Q3");
          }
        }
      }
    }
    else if(qoldfile > qnewfile){
      if (qnewrank > qoldrank){
        int sep = qnewfile - qoldfile;
        for(int i=1; i<sep ; i++){
          if (type[qoldrank+i][qoldfile-i] != 0){
            legal = false;
            Serial.println("Illegal Q4");
          }
        }
      }
      else if (qoldrank > qnewrank){
      int sep = qoldrank - qnewrank;
        for(int i=1; i<sep ; i++){
          if (type[qoldrank-i][qoldfile-i] != 0){
            legal = false;
            Serial.println("Illegal Q5");
          }
        }
      }
    }
   }
   else if (qnewrank == qoldrank){
     if (qoldfile > qnewfile){
       int sep = qoldfile - qnewfile;
       for(int i=1; i<sep ; i++){
         if (type[qoldrank][qoldfile-i] != 0){
         legal = false;
         Serial.println("Illegal Q6");
         }
       }
     }
     else if (qnewfile > qoldfile){
       int sep = qnewfile - qoldfile;
       for(int i=1; i<sep ; i++){
         if (type[qoldrank][qoldfile+i] != 0){
         legal = false;
         Serial.println("Illegal Q7");
         }
       }
     }
   }
   else if (qnewfile == qoldfile){
     if (qoldrank > qnewrank){
       int sep = qoldrank - qnewrank;
       for(int i=1; i<sep ; i++){
         if (type[qoldrank-i][qoldfile] != 0){
           legal = false;
           Serial.println("Illegal Q8");
         }
       }
     }
     else if (qnewrank > qoldrank){
       int sep = qnewrank - qoldrank;
       for(int i=1; i<sep ; i++){
         if (type[qoldrank+i][qoldfile] != 0){
           legal = false;
           Serial.println("Illegal Q9");
         }
       }
     }
   }
}

//determines validity of king move (white or black)
void kingm(int koldrank, int koldfile, int knewrank, int knewfile){
  //piece can't move to square occupied by piece of own color
  if (color[koldrank][koldfile] == color[knewrank][knewfile]){
    legal = false;
    Serial.println("Illegal K1");
  }
  
  //if king moves to new rank, it must move only 1 square
  if (koldfile == knewfile){
    if (abs(knewrank - koldrank) != 1){
      legal = false;
      Serial.println("Illegal K2");
    }
  }
  //if king moves to new file, it must move only 1 square unless it is castling
  //to castle, king and rook involved must not have moved
  //also king cannot move out of, across, or into check
  else if (koldrank == knewrank){
    if (abs(knewfile - koldfile) != 1){
      if (knewfile == 6){
        if (counter[koldrank][koldfile] == 0 && type[koldrank][knewfile+1] == 4 && counter[koldrank][knewfile+1] == 0 && type[koldrank][5] == 0 && type[koldrank][6] == 0){
          castle = true;
          checkcheck(koldrank,4);
          checkcheck(koldrank,5);
          checkcheck(koldrank,6);
          if (!check){
            castle6 = true;
            castle = false;
          }
          else{
            legal = false;
            castle = false;
          }
        }
        else{
          legal = false;
        }
      }
      else if (knewfile == 2){
        if (counter[koldrank][koldfile] == 0 && type[koldrank][0] == 4 && counter[koldrank][0] == 0 && type[koldrank][3] == 0 && type[koldrank][2] == 0 && type[koldrank][1] == 0){
          castle = true;          
          checkcheck(koldrank,4);
          checkcheck(koldrank,3);
          checkcheck(koldrank,2);
          if (!check){
            castle2 = true;
            castle = false;
          }
          else{
            legal = false;
            castle = false;
          }
        }
        else{
          legal = false;
        }
      }
      else{
        legal = false;
      }
    }
    else if (abs(knewfile - koldfile) == 1){
    }
    else{
      legal = false;
    }
  }
  //if king moves diagonally, it must move only 1 square
  else if (abs(koldrank - knewrank) == abs(koldfile - knewfile)){
    if (abs(koldrank - knewrank) != 1 || abs(koldfile - knewfile) != 1){
      legal = false;
      Serial.println("Illegal K4");
    }
  }

}

//function to determine check
//either for own king, other king, or to determine validity of castling
void checkcheck(int kingrank, int kingfile){
  
  //the function must look out from the square in question in each direction and determine the closest piece
  //if the closest piece is an enemy piece of a certain type, check is true
  //for instance, in a given diagonal direction, if the closest piece is an enemy king or pawn (1 square away only)
  //or an enemy queen or bishop at any distance, it is check
  int rank = kingrank + 1; int file = kingfile + 1;
    while (color[rank][file] == 0 && rank < 8 && file < 8 && rank >= 0 && file >= 0){
      rank++;
      file++;
    }
    if (rank < 8 && file < 8 && rank >= 0 && file >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if ((type[rank][file] == 1 && turn%2 == 0) || type[rank][file] == 6){
          if (abs(rank - kingrank) == 1 && abs(file - kingfile) == 1){
            check = true;
            Serial.println("check 1");
            Serial.println(rank);
            Serial.println(file);
            Serial.println(type[rank][file]);
            Serial.println(color[rank][file]);
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 3){
          check = true;
          Serial.println("check 2");
        }
        else{}
    }
    else{}
    }
  
  rank = kingrank + 1; file = kingfile - 1;
    while (color[rank][file] == 0 && rank < 8 && file < 8 && rank >= 0 && file >= 0){
      rank++;
      file--;
    }
    if (rank < 8 && file < 8 && rank >= 0 && file >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if (type[rank][file] == 1 || type[rank][file] == 6){
          if ((abs(rank - kingrank) == 1 && turn%2 == 0) && abs(file - kingfile) == 1){
            check = true;
            Serial.println("check 3");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 3){
          check = true;
          Serial.println("check 4");
        }
        else{}
      }
      else{}
    }
    

  
  rank = kingrank - 1; file = kingfile - 1;
    while (color[rank][file] == 0 && rank < 8 && file < 8 && rank >= 0 && file >= 0){
      rank--;
      file--;
    }
    if (rank < 8 && file < 8 && rank >= 0 && file >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if ((type[rank][file] == 1 && turn%2 == 1) || type[rank][file] == 6){
          if (abs(rank - kingrank) == 1 && abs(file - kingfile) == 1){
            check = true;
            Serial.println("check 5");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 3){
          check = true;
          Serial.println("check 6");
        }
        else{}
      }
      else{}
    }
  
  rank = kingrank - 1; file = kingfile + 1;
    while (color[rank][file] == 0 && rank < 8 && file < 8 && rank >= 0 && file >= 0){
      rank--;
      file++;
    }
    if (rank < 8 && file < 8 && rank >= 0 && file >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if ((type[rank][file] == 1 && turn%2 == 1) || type[rank][file] == 6){
          if (abs(rank - kingrank) == 1 && abs(file - kingfile) == 1){
            check = true;
            Serial.println("check 7");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 3){
          check = true;
          Serial.println("check 8");
        }
        else{}
      }
      else{}
    }
  
  rank = kingrank + 1; file = kingfile;
    while (color[rank][file] == 0 && rank < 8 && rank >= 0){
      rank++;
    }
    if (rank < 8 && rank >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if (type[rank][file] == 6){
          if (abs(rank - kingrank) == 1){
            check = true;
            Serial.println("check 9");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 4){
          check = true;
          Serial.println("check 10");
        }
        else{}
      }
      else{}
    }
  
  rank = kingrank - 1; file = kingfile;
    while (color[rank][file] == 0 && rank < 8 && rank >= 0){
      rank--;
    }
    if (rank < 8 && rank >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if (type[rank][file] == 6){
          if (abs(rank - kingrank) == 1){
            check = true;
            Serial.println("check 11");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 4){
          check = true;
          Serial.println("check 12");
        }
        else{}
      }
      else{}
    }
  
  rank = kingrank; file = kingfile + 1;
    while (color[rank][file] == 0 && file < 8 && file >= 0){
      file++;
    }
    if (file < 8 && file >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if (type[rank][file] == 6){
          if (abs(rank - kingrank) == 1){
            check = true;
            Serial.println("check 13");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 4){
          check = true;
          Serial.println("check 14");
        }
        else{}
      }
      else{}
    }
  
  rank = kingrank; file = kingfile - 1;
    while (color[rank][file] == 0 && file < 8 && file >= 0){
      file--;
    }
    if (file < 8 && file >= 0){
      if ((color[rank][file] != color[kingrank][kingfile] && color[rank][file] != 0 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1) || (castle && kingrank == 7 && color[rank][file] == 2)){
        if (type[rank][file] == 6){
          if (abs(rank - kingrank) == 1){
            check = true;
            Serial.println("check 15");
          }
          else{}
        }
        else if (type[rank][file] == 5 || type[rank][file] == 4){
          check = true;
          Serial.println("check 16");
        }
        else{}
      }
      else{}
    }
  
  if ((color[kingrank+2][kingfile+1] != color[kingrank][kingfile] && type[kingrank+2][kingfile+1] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank+2][kingfile+1] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank+2][kingfile+1] == 2)){
    check = true;
    Serial.println("check 17");
  }
  else if ((color[kingrank+1][kingfile+2] != color[kingrank][kingfile] && type[kingrank+1][kingfile+2] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank+1][kingfile+2] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank+1][kingfile+2] == 2)){
    check = true;
    Serial.println("check 18");
  }
  else if ((color[kingrank+1][kingfile-2] != color[kingrank][kingfile] && type[kingrank+1][kingfile-2] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank+1][kingfile-2] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank+1][kingfile-2] == 2)){
    check = true;
    Serial.println("check 19");
  }
  else if ((color[kingrank-1][kingfile-2] != color[kingrank][kingfile] && type[kingrank-1][kingfile-2] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank-1][kingfile-2] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank-1][kingfile-2] == 2)){
    check = true;
    Serial.println("check 20");
  }
  else if ((color[kingrank-1][kingfile+2] != color[kingrank][kingfile] && type[kingrank-1][kingfile+2] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank-1][kingfile+2] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank-1][kingfile+2] == 2)){
    check = true;
    Serial.println("check 21");
  }
  else if ((color[kingrank+2][kingfile-1] != color[kingrank][kingfile] && type[kingrank+2][kingfile-1] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank+2][kingfile-1] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank+2][kingfile-1] == 2)){
    check = true;
    Serial.println("check 22");
  }
  else if ((color[kingrank-2][kingfile-1] != color[kingrank][kingfile] && type[kingrank-2][kingfile-1] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank-2][kingfile-1] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank-2][kingfile-1] == 2)){
    check = true;
    Serial.println("check 23");
  }
  else if ((color[kingrank-2][kingfile+1] != color[kingrank][kingfile] && type[kingrank-2][kingfile+1] == 2 && !castle) || (castle && kingrank == 0 && color[rank][file] == 1 && type[kingrank-2][kingfile+1] == 2) || (castle && kingrank == 7 && color[rank][file] == 2 && type[kingrank-2][kingfile+1] == 2)){
    check = true;
    Serial.println("check 24");
  }
  
  
}

//function that displays the board (type matrix) on the lcd
//allows player to scroll through to select old and new squares
void moveselection(int sm){
  boolean selection = false;
  int curs = 0;
  lcdrank = 6;
  lcdfile = 0;
  
  //initially displays bottom 2 ranks
  lcd.setCursor(0,0);
  for (lcdfile=0;lcdfile<8;lcdfile++){
    lcd.print(type[lcdrank][lcdfile]);
  }
  
  lcd.setCursor(0,1);
  for (lcdfile=0;lcdfile<8;lcdfile++){
    lcd.print(type[lcdrank+1][lcdfile]);
  }
  
  //loop runs while selection still needs to be made (waiting for select button to be pushed)
  while(!selection){
    //if the cursor is over a white piece, W is displayed in the corner of the screen
    if (color[lcdrank+1][curs] == 1){
      lcd.setCursor(15,1);
      lcd.print("W");
    }
    //if over a black piece, B
    else if (color[lcdrank+1][curs] == 2){
      lcd.setCursor(15,1);
      lcd.print("B");
    }
    //if over an empty square, nothing
    else if (color[lcdrank+1][curs] == 0){
      lcd.setCursor(15,1);
      lcd.print(" ");
    }
    
    lcd.blink(); //blinks cursor
    
    lcd.setCursor(curs, 1);
    adc_key_in = analogRead(0); // read the value from the sensor
    
    key = get_key(adc_key_in); // convert into key press
    if (key != oldkey){ // if keypress is detected
      delay(50); // wait for debounce time
      adc_key_in = analogRead(0); // read the value from the sensor
      key = get_key(adc_key_in); // convert into key press
      if (key != oldkey){
        oldkey = key;
        
          //if up key pressed, board display scrolls up
          //will not scroll up any more if already displaying top of board
          if (key == 1 && lcdrank >= 0){
            lcdrank--;
            if (lcdrank >= 0){
              lcd.setCursor(0,0);
              for (lcdfile=0;lcdfile<8;lcdfile++){
                lcd.print(type[lcdrank][lcdfile]);
              }
              lcdfile = 0;
      
              lcd.setCursor(0,1);
              for (lcdfile=0;lcdfile<8;lcdfile++){
                lcd.print(type[lcdrank+1][lcdfile]);
              }
              lcdfile = 0;
            }
            else{
              lcd.setCursor(0,1);
              for (lcdfile=0;lcdfile<8;lcdfile++){
                lcd.print(type[lcdrank+1][lcdfile]);
              }
              lcdfile = 0;
              
              lcd.setCursor(0,0);
              lcd.print("        ");
            }
          }
    
          //if down key pressed, board display scrolls down
          //will not scroll down any more if bottom of board already displayed
          else if (key == 2 && lcdrank != 6){
            lcdrank++;
            lcd.setCursor(0,0);
            for (lcdfile=0;lcdfile<8;lcdfile++){
              lcd.print(type[lcdrank][lcdfile]);
            }
            lcdfile = 0;
    
            lcd.setCursor(0,1);
            for (lcdfile=0;lcdfile<8;lcdfile++){
            lcd.print(type[lcdrank+1][lcdfile]);
            }
            lcdfile = 0;
          }
          
          //if right key pressed, cursor moves right
          //will not move further if already on right side of board
          else if (key == 0 && curs != 7){
            curs++;
           }
          
          //if left key pressed, cursor moves left
          //will not move further if already on left side of board
          else if (key == 3 && curs!= 0){
            curs--;
          }
          
          //if select key pressed, selection is made
          //the board position is stored in the correct variables
          //depending on turn and whether the player is selection or moving the piece
          else if (key == 4){
            lcd.noBlink();
            selection = true;
            if ((turn % 2) == 1){
              if (sm == 1){
                owrank = lcdrank+1;
                owfile = curs;
              }
              else if (sm == 2){
                nwrank = lcdrank+1;
                nwfile = curs; 
              }
            }
            else if ((turn % 2) == 0){
              if (sm == 1){
                obrank = lcdrank+1;
                obfile = curs;
              }
              else if (sm == 2){
                nbrank = lcdrank+1;
                nbfile = curs; 
              }
            }
          
       
        }
      }
    }
}
}



// Convert ADC value to key number
int get_key(unsigned int input)
{ int k;
for (k = 0; k < NUM_KEYS; k++)
{
if (input < adc_key_val[k])
{ return k; }
}
if (k >= NUM_KEYS)
k = -1; // No valid key pressed
return k;
}
