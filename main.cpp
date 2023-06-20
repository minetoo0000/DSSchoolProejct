// DSSP 라이브러리 사용.
#define USING_DSSP

#include <iostream>
#include <unistd.h>
#include "./DSSP.hpp"


int sys()
{
    // 0. 선언 및 정의.
    LoopState state = { 0 };
    int global_menu = 0;
    Store store = DSSP.newStore("선린여학원매점");
    GoodsInfo* selected_goods_p = 0;
    char global_goods[30] = {0};
    
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
        DSSP.cli.read("메뉴의 각 번호를 입력하여 선택하세요.", global_menu);

        // 3. 메뉴 번호에 대응하는 상태 값으로 상태 업데이트.
        ///////////////////////////// 함수 제작.
        state = DSSP.menuidToState(global_menu);
    }

    // -- 맨 처음 부팅 상태.
    else if ( state.bin == dssp_BOOTTIME.bin )
    {
        // 1. 상태 업데이트.
        global_menu = 0;
        selected_goods_p = 0;
        state = dssp_MENU;
        
        // 2. 최초 시작 화면 출력.
        DSSP.cli.bootScreen();

        // 3. 딜레이.
        sleep(1);
    }

    // -- 도움말 출력.
    else if ( state.bin == dssp_MENU.bin )
    {
        // 1. 상태 업데이트.
        state = dssp_DEFAULT;
        
        // 2. 도움말.
        DSSP.cli.menuScreen();
    }

    // -- 아무 물품도 선택하지 않기.
    else if ( state.bin == dssp_SELECT_NULL.bin )
    {
        // 1. 상태 업데이트.
        state = dssp_MENU;

        // 2. 가리키지 않기.
        selected_goods_p = 0;
        global_goods[0] = 0;
    }

    // -- 메뉴 - 물품 선택.
    else if ( state.bin == dssp_SELECT.bin )
    {
        // 0. 선언.
        Node* node_p = 0;
        
        // 1. 상태 업데이트.
        state = dssp_MENU;

        // 2. 이름 입력 받기.
        DSSP.cli.print("선택할 물품의 이름을 입력하세요.",0);
        cout<<" > "; cin>>global_goods;
        if ( global_goods[0] == 0 ) goto LOOP;

        // 3. 찾기.
        node_p = store.dict.find(Key(global_goods));
        if ( node_p == 0 )
        {
            DSSP.cli.print("해당하는 물품이 없습니다.", 0);
            state = dssp_SELECT_NULL;
            goto LOOP;
        }

        // 4. 선택 시키기.
        selected_goods_p = &node_p->value;
    }

    // -- 메뉴 - 물품 추가.
    else if ( state.bin == dssp_ADD.bin )
    {
        // 0. 선언.
        char goods_name[30];
        int price = 0;
        
        // 1. 상태 업데이트.
        state = dssp_MENU;

        // 2. 물품 이름 입력받기.
        DSSP.cli.print("새로 추가할 물품의 이름을 입력해주세요.",0);
        DSSP.cli.print("현재 입력 취소는 빈 문자열로 Enter",0);
        printf(" > ");
        cin>>goods_name;
        DSSP.cli.print("",0);
        // 예외처리.
        if ( goods_name[0] == 0 ) goto LOOP;

        // 3. 가격 입력 받기.
        DSSP.cli.read("설정할 가격을 입력해주세요", price);

        // 2. 추가.
        DSSP.insertGoodsInfo(store, goods_name, DSSP.newGoodsInfo(price));
    }

    // -- 메뉴 - 물품 삭제.
    else if ( state.bin == dssp_REMOVE.bin )
    {
        //1. 상태 업데이트
        state =  dssp_MENU;

        // 2. 예외처리.
        if ( global_goods[0] == 0 ) goto LOOP;


        // 3. 삭제 
        DSSP.removeGoodsInfo(store , global_goods);
    }

    // -- 매점 통계.
    else if ( state.bin == dssp_STORE_STATUS.bin )
    {
        // 1. 상태 업데이트.
        state = dssp_MENU;

        // 2. 통계 출력.
        DSSP.cli.showStoreStatus(store);
    }

    goto LOOP;

    return( 0 );
}


int main()
{
    sys();
    return( 0 );
}