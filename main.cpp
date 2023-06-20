// DSSP 라이브러리 사용.
#define USING_DSSP

#include <iostream>
#include <stdlib.h>
#include "./DSSP.hpp"


int sys()
{
    // 0. 선언 및 정의.
    LoopState state = { 0 };
    int global_menu = 0;
    
    // loop block.
    LOOP:;

    // -- 기본 대기 상태.
    // 입력을 기다립니다.
    if ( state.bin == dssp_DEFAULT.bin )
    {
        // 1. 상태 업데이트.
        // 미리 실패 상태.
        state = dssp_TASK_FAIL;
        
        // 2. 글로벌 메뉴 입력.
        DSSP.cli.read("메뉴의 각 번호를 입력하여 선택.", global_menu);

        // 3. 메뉴 번호에 대응하는 상태 값으로 상태 업데이트.
        ///////////////////////////// 함수 제작.
        state = 
    }

    // -- 맨 처음 부팅 상태.
    // 매점의 이름을 입력받아야 함.
    else if ( state.bin == dssp_BOOTTIME.bin )
    {
        // 1. 상태 업데이트.
        global_menu = 0;
        state = dssp_MENU;
        
        // 2. 최초 시작 화면 출력.
        DSSP.cli.bootScreen();

        // 3. 딜레이.
        _sleep(1);
    }

    // -- 도움말 출력.
    else if ( state.bin == dssp_MENU.bin )
    {
        // 1. 상태 업데이트.
        state = dssp_DEFAULT;
        
        // 2. 도움말.
        DSSP.cli.menuScreen();
    }

    goto LOOP;
}


int main()
{
    sys();
    return( 0 );
}