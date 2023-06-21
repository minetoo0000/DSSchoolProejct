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
    GoodsInfo* global_goods_info_p = 0;
    char global_goods_name[30] = {0};
    
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
        state = DSSP.menuidToState(global_menu);
    }

    // -- 맨 처음 부팅 상태.
    else if ( state.bin == dssp_BOOTTIME.bin )
    {
        // 1. 상태 업데이트.
        global_menu = 0;
        global_goods_info_p = 0;
        state = dssp_SELECT_NULL;
        
        // 2. 최초 시작 화면 출력.
        DSSP.cli.print("",100);
        DSSP.cli.bootScreen();

        // 3. 딜레이.
        sleep(2);
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
        global_goods_info_p = 0;
        global_goods_name[0] = 0;
    }

    // -- 메뉴 - 물품 리스트.
    else if ( state.bin == dssp_SHOW_LIST.bin )
    {
        // 1. 상태 업데이트.
        state = dssp_MENU;

        // 2. 출력.
        store.dict.print(DSSP.cli.callback.printGoodsInfo);
        DSSP.cli.print("물품 목록 출력이 끝났습니다.",0);
        DSSP.cli.print("Enter를 누르면 홈으로 이동합니다.",0);
        getchar();
        getchar();
        DSSP.cli.print("",0);
    }

    // -- 메뉴 - 물품 선택.
    else if ( state.bin == dssp_SELECT.bin )
    {
        // 0. 선언.
        Node* node_p = 0;
        
        // 1. 상태 업데이트.
        state = dssp_WAIT_ENTER;

        // 2. 이름 입력 받기.
        DSSP.cli.print("선택할 물품의 이름을 입력하세요.",0);
        cout<<" > "; cin>>global_goods_name;
        if ( global_goods_name[0] == 0 ) goto LOOP;
        DSSP.cli.print("",0);

        // 3. 찾기.
        node_p = store.dict.find(Key(global_goods_name));
        if ( node_p == 0 )
        {
            DSSP.cli.print("해당하는 물품이 없습니다.", 1);
            DSSP.cli.print("아무것도 선택되지 않습니다.", 1);
            state = dssp_SELECT_NULL;
            goto LOOP;
        }

        // 4. 선택 시키기.
        global_goods_info_p = &node_p->value;

        // 5. 정보 출력.
        DSSP.cli.callback.printGoodsInfo(global_goods_name, *global_goods_info_p);
    }

    // -- 메뉴 - 물품 추가.
    else if ( state.bin == dssp_ADD.bin )
    {
        // 0. 선언.
        char goods_name[30];
        int price = 0;
        Node* node_p = 0;
        
        // 1. 상태 업데이트.
        state = dssp_MENU;

        // 2. 물품 이름 입력받기.
        DSSP.cli.print("새로 추가할 물품의 이름을 입력해주세요.",0);
        cout<<" > "; cin>>goods_name;
        DSSP.cli.print("",0);

        node_p = store.dict.find(Key(goods_name));
        if ( node_p != 0 )
        {
            DSSP.cli.print("이미 존재하는 물품입니다.",0);
            DSSP.cli.print("변경사항은 없습니다.",1);
            state = dssp_WAIT_ENTER;
            goto LOOP;
        }

        // 3. 가격 입력 받기.
        DSSP.cli.read("설정할 가격을 입력해주세요", price);

        // 2. 추가.
        store = DSSP.insertGoodsInfo(store, goods_name, DSSP.newGoodsInfo(price));
    }

    // -- 메뉴 - 물품 삭제.
    else if ( state.bin == dssp_REMOVE.bin )
    {
        //1. 상태 업데이트
        state = dssp_NOT_SELECTED;

        // 2. 예외처리.
        if ( global_goods_name[0] == 0 )
            goto LOOP;

        // 3. 삭제.
        DSSP.removeGoodsInfo(store , global_goods_name);

        DSSP.cli.print("선택된 물품 테이블이 완전히 제거되었습니다.",1);

        // 4. 성공 상태.
        state = dssp_SELECT_NULL;
    }

    // -- 메뉴 - 매점 통계.
    else if ( state.bin == dssp_STORE_STATUS.bin )
    {
        // 1. 상태 업데이트.
        state = dssp_WAIT_ENTER;

        // 2. 통계 출력.
        DSSP.cli.showStoreStatus(store);
        DSSP.cli.print("",0);
    }

    // -- 물품 구매.
    else if ( state.bin == dssp_BUY.bin )
    {
        // 0. 선언.
        int buy_count = 0;
        Store tmp_store = store;
        
        // 1. 실패 상태.
        state = dssp_NOT_SELECTED;

        // 2. 물품 선택 여부 확인.
        if ( global_goods_info_p == 0 )
            goto LOOP;

        // 3. 구매 수량 입력받기.
        DSSP.cli.read("구매하실 수량을 입력하세요.", buy_count);
    
        // 4. 물품 구매 함수 호출.
        tmp_store = DSSP.buyGoods(store, global_goods_name, buy_count);
        if ( tmp_store.total_sales_count == store.total_sales_count )
        {
            DSSP.cli.print("구매가 적용된 것 같지 않습니다.",0);
            DSSP.cli.print("재고가 구매량보다 많은 경우 이런 일이 발생할 수 있습니다.",0);
            state = dssp_WAIT_ENTER;
            goto LOOP;
        }
        store = tmp_store;

        // 5. 정보 출력.
        DSSP.cli.callback.printGoodsInfo(global_goods_name, *global_goods_info_p);

        // 5. 성공 상태.
        state = dssp_WAIT_ENTER;
    }

    // -- 메뉴 - 물품 재고 추가.
    else if ( state.bin == dssp_GOODS_ADDITIONAL.bin )
    {
        // 0. 선언.
        int n = 0;
        
        // 1. 실패 상태.
        state = dssp_NOT_SELECTED;

        // 2. 선택 여부 확인.
        if ( global_goods_info_p == 0 )
            goto LOOP;

        // 3. 추가 재고량 입력받기.
        DSSP.cli.read("추가할 재고 수를 입력하세요.", n);

        // 4. 재고 추가 함수.
        DSSP.goodsAdditional(store, global_goods_name, n);
        DSSP.cli.callback.printGoodsInfo(global_goods_name, *global_goods_info_p);

        // 5. 성공 상태.
        state = dssp_WAIT_ENTER;
    }

    // -- 메뉴 - 물품 재고 손실.
    else if ( state.bin == dssp_GOODS_LOSS.bin )
    {
        // 0. 선언.
        int n = 0;
        
        // 1. 실패 상태.
        state = dssp_NOT_SELECTED;

        // 2. 선택 여부 확인.
        if ( global_goods_info_p == 0 )
            goto LOOP;

        // 3. 손실량 입력 받기.
        DSSP.cli.read("손실된 재고 수를 입력하세요.", n);
        
        // 4. 재고 손실 적용.
        store = DSSP.goodsLoss(store, global_goods_name, n);
        DSSP.cli.callback.printGoodsInfo(global_goods_name, *global_goods_info_p);

        // 5. 성공 상태.
        state = dssp_WAIT_ENTER;
    }

    // -- 메뉴 - 물품 증정품 설정.
    else if ( state.bin == dssp_SET_GIFTS.bin )
    {
        // 0. 선언.
        int additional_n = 0;
        
        // 1. 실패 상태.
        state = dssp_NOT_SELECTED;

        // 2. 예외처리.
        if ( global_goods_info_p == 0 )
            goto LOOP;

        // 3. 증정 수 입력 받기.
        DSSP.cli.print("추가 증정품을 설정합니다.",0);
        DSSP.cli.print("추가 증정은 n+1이며 n을 직접 설정할 수 있습니다.",0);
        DSSP.cli.read("0을 입력하면 추가 증정이 되지 않습니다.", additional_n);

        // 3. 증정품 설정.
        DSSP.setGifts(store, global_goods_name, additional_n);

        // 4. 성공 상태.
        state = dssp_MENU;
    }

    // -- 선턱되지 않음 오류.
    else if ( state.bin == dssp_NOT_SELECTED.bin )
    {
        // 1. 다음 상태.
        state = dssp_WAIT_ENTER;

        DSSP.cli.print("아무것도 선택되지 않아 작업을 계속할 수 없습니다.",0);
        DSSP.cli.print("물품을 선택한 후에 다시 시도하세요.",1);
    }

    // -- enter 입력 대기.
    else if ( state.bin == dssp_WAIT_ENTER.bin )
    {
        // 1. 다음 상태.
        state = dssp_MENU;

        DSSP.cli.print("Enter 키를 누르면 홈으로 이동합니다.",0);
        getchar();
        getchar();
        DSSP.cli.print("",0);
    }

    // -- 작업 실패.
    else if ( state.bin == dssp_TASK_FAIL.bin )
    {
        // 1. 다음 상태.
        state = dssp_BOOTTIME;

        DSSP.cli.print("[ Error : 1 ]",0);
        DSSP.cli.print("작업에 예외가 발생했습니다.",0);
        DSSP.cli.print("Enter를 누르면 프로그램을 부분 초기화합니다.",0);
        getchar();
        getchar();
        DSSP.cli.print("설정을 초기화합니다 . . .",1);
    }

    // -- 약속되지 않은 상태 코드.
    else
    {
        DSSP.cli.print("[ Error : 2 ]",0);
        DSSP.cli.print("뭔가 크게 잘못되었습니다.",0);
        DSSP.cli.print("Enter를 누르면 프로그램이 재시작 됩니다.",0);
        getchar();
        getchar();
        DSSP.cli.print("",100);
        goto RESTART;
    }

    goto LOOP;

    RESTART:return( 1 );
    RETURN:return( 0 );
}


int main()
{
    while ( sys() );
    return( 0 );
}