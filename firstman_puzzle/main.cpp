//
//  main.cpp
//  firstman_puzzle
//
//  Created by line.ash on 2021/09/19.
//

#include <iostream>
#include <bangtal>
#include <time.h>
#include <ctime>

using namespace bangtal;

ScenePtr scene;
ObjectPtr game_board[25], original_board[25];
ObjectPtr start_button;

//auto shuffle = Timer::create(0.1f);
int blank;
int mixCount;
int saveBlank;
bool game = false;
int endTime = 0;

void delay(clock_t n) //애니메이션을 위한 시간지연 함수
{
  clock_t start = clock();
  while(clock() - start < n);
}

int indexToX(int i){ //x좌표값 반환
    return 210 + 206 * ( i % 5 );
}

int indexToY(int i){ //y좌표값 반환
    return 725 - 163 * ( i / 5 );
}

int game_index(ObjectPtr object){ //선택한 조각의 인덱스를 구함.
    
    for(int i = 0; i < 25 ; i++)
    {
        if(game_board[i] == object) return i;
    }
    return -1; //error, invalid object
}

void game_move(int i){
    //인덱스와 blank object 교환
    ObjectPtr object = game_board[i];
    game_board[i] = game_board[blank];
    game_board[blank] = object;
}


void change(ObjectPtr object){
    int i = game_index(object);    //object의 인덱스 구함
    game_move(i);    //인덱스와 blank object 교환
    game_board[i]->locate(scene, indexToX(i), indexToY(i));
    game_board[blank]->locate(scene, indexToX(blank), indexToY(blank));
    blank = i;
}

int random_move(){
    
    int i = -1; //not determined
    
    while(i == -1){
        
        switch (rand()%4) {
            case 0:if(blank % 5 > 0)i = blank - 1;
                break;
            case 1:if(blank % 5 < 4)i = blank + 1;
                break;
            case 2:if(blank / 5 > 0)i = blank - 5;
                break;
            case 3:if(blank / 5 < 4)i = blank + 5;
                break;
        }
    }
    return i;
}
bool game_end(){ //원본과 비교
    
    for ( int i = 0; i<25; i++) {
        if(game_board[i] != original_board[i]) return false;
    }
    return true;
}

ScenePtr game_init()
{
    
    scene = Scene::create("배경", "cut.png");
    
    //조각생성
    char buf[50];
    for( int i = 0 ; i < 25 ; i++){
        
        sprintf(buf, "%d.png", i+1);
        game_board[i] = Object::create(buf);
        original_board[i] = game_board[i];
        game_board[i]->locate(scene, indexToX(i), indexToY(i));
        game_board[i]->show();
    }
    
    start_button = Object::create("start.png");
    start_button->locate(scene, 680, 300);
    start_button->Object::setScale(0.3);
    start_button->show();
    start_button->setOnMouseCallback([&] (ObjectPtr object, int x, int y, MouseAction action)->bool {
        start_button->hide();
        game = true;
        //blank 생성
        blank = rand()%25;
        game_board[blank]->hide();
        mixCount = 4;
        
        //셔플
        while(mixCount>0)
        {
            change(game_board[random_move()]);
            mixCount--;
        }
        
        if(game == true)
        {
            for(int i = 0; i < 25; i++) //조각 이동
            {
                game_board[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
                    if((game == true) &&(game_index(object)+5 == blank or game_index(object)-5 == blank or game_index(object)+1 == blank or game_index(object)-1 == blank)){
                        change(object);
                        if(game_end())
                        {
                            game = false;
                            game_board[blank]->show();
                            char endMessage[200];
                            sprintf(endMessage, "축하합니다!! 해결까지 %d초 걸렸습니다!", endTime);
                            showMessage(endMessage);
                            start_button->show();
                        }
                    }
                    return true;
                });
            }
        }

        return true;
    });
    
    
    return scene;
}

int main()
{
    srand((int)time(NULL));
    
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    
    startGame(game_init());
    
}
