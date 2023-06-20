#pragma once

/**
 * DSSP.hpp
 * 선린여학원 매점 재고 관리 프로그램 라이브러리
 * 
 * 2023-06-13
 * pm-6:03
*/

// --[[ setting ]]
// #define USING_DSSP
#define VERSION_DSSP "1.0"


// --[[ include ]]
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <map>
#include "./skiplist.hpp"


// --[[ interface ]]
// -- 물품 정보.
typedef struct t$dssp$goods_info
{
	// 가격.
	uint32_t price;
	
	// 재고 수.
	uint32_t count;
	// 물품 총 재고 추가량.
	uint32_t total_upcount;
	// 물품 총 재고 소진량.
	uint32_t total_downcount;
	// 물품 총 재고 손실량.
	uint64_t total_losscount;

	// 물품 총 판매액
	// uint64_t total_sales;

	// 추가 증정량. "n+1 할인 중"
	uint8_t additional_n;
} t$dssp$goods_info;

// -- 매점 객체.
typedef struct t$dssp$store
{
	// 지점 이름.
	char* store_name;

	// 총 판매 개수.
	uint32_t total_sales_count;
	// 손실된 상품 총 개수.
	uint32_t total_loss_count;
	// 행사가 적용 상품 총 판매 개수.
	uint32_t total_gifts_count;

	// 총 판매 액수.
	uint64_t total_sales_price;
	// 총 손실 액수.
	uint64_t total_losses;

	// 재고 종류 개수.
	uint32_t total_goods;

	// 재고 딕셔너리 객체.
	Skiplist<Key, t$dssp$goods_info> dict;
} t$dssp$store;


// -- 단축 정의.
typedef Skiplist<Key,t$dssp$goods_info>::Node t$dssp$node;


// -- 명령 인터페이스 상태 구조체.
typedef union t$dssp$loop_state
{
	uint64_t bin;

	struct State
	{
		// 기본 사용자 설정 마침.
		uint64_t already_init:1;
		// 메뉴 띄우기.
		uint64_t show_menu:1;

		// 0. 물품 리스트.
		uint64_t show_list:1;
		// 1. 물품 선택.
		uint64_t select:1;
		// 2. 물품 추가.
		uint64_t add:1;
		// 3. 물품 삭제.
		uint64_t remove:1;
		// 4. 매점 통계.
		uint64_t store_status:1;

		// 확인 불가능한 오류로 인해 다음 작업 처리 실패.
		uint64_t task_fail:1;
	} state;
} t$dssp$loop_state;
// -- 부트 상태.
const LoopState v$dssp$BOOTTIME = {
	.bin=0,
};
// -- 기본 상태.
const LoopState v$dssp$DEFAULT = {
	.state = {
		.already_init=1,
	},
};
// -- 조작 도움말.
const LoopState v$dssp$MENU = {
	.state = {
		.already_init=1,
		.show_menu=1,
	},
};
// -- 오류 상태.
const LoopState v$dssp$TASK_FAIL = {
	.state = {
		.already_init=1,
		.task_fail=1,
	},
};
// -- 0. 물품 리스트.
const LoopState v$dssp$0SHOW_LIST = {
	.state = {
		.already_init=1,
		.show_list=1,
	},
};
// -- 1. 물품 선택.
const LoopState v$dssp$1SEELCT = {
	.state = {
		.already_init=1,
		.select=1,
	},
};
// -- 2. 물품 추가.
const LoopState v$dssp$2ADD = {
	.state = {
		.already_init=1,
		.add=1,
	},
};
// -- 3. 물품 삭제.
const LoopState v$dssp$3REMOVE = {
	.state = {
		.already_init=1,
		.remove=1,
	},
};
// -- 4. 매점 통계.
const LoopState v$dssp$4STORE_STATUS = {
	.state = {
		.already_init=1,
		.store_status=1,
	},
};


// -- 매점 재고 정보 및 현황 출력 예시.
/**
 * 
 * [ "전주비빔 삼각김밥" ]
 *     상품 정가 : 1200원
 *     상품 현재 재고 수 : 3개
 *     물품 총 재고 추가량 : 7개
 *     물품 총 재고 소진량 : 4개
 *     물품 총 손실량 : 0개
 *     물품 총 손실액 : 0원
 *     물품 총 판매액 : 4800원
 * 
 * [ "신라면 사발면" ]
 *     상품 정가 : 1500원
 *     상품 현재 재고 수 : 9개
 *     물품 총 재고 추가량 : 15개
 *     물품 총 재고 소진량 : 6개
 *     물품 총 손실량 : 0개
 *     물품 총 손실액 : 0원
 *     물품 총 판매액 : 9000원
 * 
 * [ "환타 오렌지향" ]
 *     상품 정가 : 1400원
 *     상품 현재 재고 수 : 11개
 *     물품 총 재고 추가량 : 14개
 *     물품 총 재고 소진량 : 2개
 *     물품 총 손실량 : 1개
 *     물품 총 손실액 : 1400원
 *     물품 총 판매액 : 2400원
 *     [ 행사중! ]
 *     추가 증정 : 2 + 1
 *     정가 3개 :  4200원
 *     행사가 2+1개 : 2800원
*/


// -- 매점 정보 출력 예시.
/**
 * [ 매점 정보 ]
 *     지점 이름 : "선린여학원 매점"
 * 
 *     판매 개수 : 100개
 *     손실 개수 : 1개
 *     행사 할인 적용 상품 판매 개수 : 10개
 * 
 *     판매 액수 : 89000원
 *     손실 액수 : 1000원
 * 
 *     재고 종류 개수 : 2개
*/


// --[[ function ]]
// -- 새 재고 정보.
t$dssp$goods_info f$dssp$newGoodsInfo( const uint32_t price)
{
	return(
		(t$dssp$goods_info)
		{
			.price = price,
		}
	);
}


// -- 새 매점 객체.
// 항상 모든 수치가 0인 객체를 반환함.
// 예외처리 시 반환되는 객체의 store_name은 0이 됨.
t$dssp$store f$dssp$newStore( const char*const store_name )
{
	// 0. 선언.
	char* store_name_copy = 0;
	int get_strlen = 0;
	Skiplist<Key, t$dssp$goods_info> goods_info(0.5, 20);

	// 1. 예외처리.
	if ( store_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 1. 널 포함, 문자열 크기 구하기.
	for ( ; store_name[get_strlen++]; );
	
	// 2. 메모리 할당 시도.
	// 처음 실패 시 최대 2번 재시도.
	for ( uint8_t i=0; !(store_name_copy=(char*)malloc(get_strlen)) && i<3; i++ );
	// 모두 실패 시 예외처리.
	if ( !store_name_copy ) goto SKIP;
	
	// 3. 이름 복제.
	for ( int i=0; i<get_strlen; i++ ) store_name_copy[i]=store_name[i];
	
	// 4. 매점 객체 반환.
	SKIP:return(
		(t$dssp$store)
		{
			.store_name = store_name_copy,
			.dict = goods_info,
		}
	);
}


// -- 물품 정가 설정.
// price는 uint32_t 범위의 모든 수.
// 정상 작업 시 1을, 예외처리 시 0을 반환함.
int f$dssp$setPrice( t$dssp$store store, char*const key_name, const uint64_t price )
{
	// 0. 선언.
	t$dssp$goods_info* selected_goods = 0;
	t$dssp$node* tmp = 0;
	
	// 1. 예외처리.
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 물품 정보 객체 포인터 가져오기.
	tmp = store.dict.find(Key(key_name));
	// 찾기 실패 예외처리.
	if ( !tmp ) goto SKIP;
	// 해당 value 포인터 꺼내기.
	selected_goods = &tmp->value;

	// 3. 물품 정가 업데이트.
	selected_goods->price = price;

	// 4. 성공 여부 반환.
	return( 1 );
	// 예외처리.
	SKIP:return( 0 );
}


// -- 물품 추가 증정량 설정.
/// "n + 1, n개 사면 1개 더 줍니다."
// ken_name, n이 0인 경우 증정 할인 적용되지 않음.
// 정상 작업 시 1을, 예외처리 시 0을 반환함.
int f$dssp$setGifts( t$dssp$store store, char*const key_name, const uint8_t n )
{
	// 0. 선언.
	t$dssp$goods_info* selected_goods = 0;
	Key goods_name(key_name);
	t$dssp$node* get_node;
	
	// 1. 예외처리.	
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 물품 객체 포인터 가져오기.
	get_node = store.dict.find(goods_name);
	// 존재하지 않으면 예외처리.
	if ( get_node == 0 ) goto SKIP;

	// 3. 증정량 업데이트.
	get_node->value.additional_n = n;

	// 4. 성공 여부 반환.
	return( 1 );
	// 예외처리.
	SKIP:return( 0 );
}


// -- 상품 추가.
int f$dssp$insertGoodsInfo( t$dssp$store store, char*const key_name, t$dssp$goods_info goods_info )
{
    // 1. 예외처리.
    if ( key_name == 0 ) return 0;

    // 2. 키값으로 변환.
	Key goods_name(key_name);

    // 3. 해당 키 값 존재하는지 찾기.
    auto tmp = store.dict.find(goods_name);
    if ( tmp == nullptr ) return 0;

    // 4. 삽입.
    store.dict.insert(goods_name, goods_info);
    return 1;
}

// -- 상품 삭제.
int f$dssp$removeGoodsInfo( t$dssp$store store, char*const key_name )
{
    // 1. 예외처리.
    if ( key_name == 0 ) return( 0 );

    // 2. 키값으로 변환.
	Key goods_name(key_name);
	
    // 3. 해당 키 값 존재하는지 찾기.
    auto tmp = store.dict.find(goods_name);
    if ( tmp == nullptr ) return( 0 );

    // 4. 삭제.
    store.dict.remove(goods_name);
    return 1;
}


// -- 상품 이름 변경.
int f$dssp$renameGoods( t$dssp$store store, char*const key_name, char*const rename )
{
	// 0. 선언.
	t$dssp$goods_info* selected_goods = 0;
	Key goods_name(key_name);
	t$dssp$goods_info tmp_goods = { 0 };
	// 성공 상태.
	int state = 1;
	
	// 1. 예외처리.
	if ( key_name )
	if ( rename )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 해당 키가 존재하는 지 확인.
	selected_goods = &(store.dict.find(goods_name)->value);
	// 해당 키가 없으면 예외처리.
	if ( selected_goods == 0 ) goto SKIP;

	// 3. 지우기 전에 물품 정보 객체 복사해두기.
	tmp_goods = *selected_goods;

	// 4. 키 지우기.
	state &= f$dssp$removeGoodsInfo(store, key_name);

	// 5. rename을 키값으로 물품 정보 객체 생성 및 매점에 삽입.
	state &= f$dssp$insertGoodsInfo(store, rename, tmp_goods);

	// 6. 성공 여부 반환.
	return( state );
	// 예외처리.
	SKIP:return( 0 );
}


// -- 할인이 적용된 물품의 개수 구하기 함수.
uint32_t f$dssp$numberOfDiscount( const t$dssp$goods_info goods, const uint32_t buy_number )
{
	// 0. 선언.
	uint32_t result_count = 0;

	// 1. 계산하기.
	result_count = buy_number%(goods.additional_n+1);
	// ((buy_number/goods.additional_n)*goods.additional_m);

	// 2. 증정품 개수 반환.
	return( result_count );
}


// -- 할인이 적용된 가격 구하기 함수.
uint64_t f$dssp$priceOfDiscount( const t$dssp$goods_info goods, const uint32_t buy_number )
{
	// 0. 선언.
	uint64_t result_price = 0;
	
	// 1. 예외처리.
	if ( !buy_number ) goto SKIP;

	// 2. 구하기.
	result_price = goods.price*(buy_number-f$dssp$numberOfDiscount(goods, buy_number));

	// 3. 가격 반환.
	SKIP:return( result_price );
}


// -- base - 재고 n개 추가.
// 정상 작업 시 재고 n개 추가된 값 반환.
// 오버플로우 발생 시 예외처리.
// 예외처리 시 goods.count 반환.
uint32_t f$dssp$base$goodsAdditional( t$dssp$goods_info goods, const uint32_t n )
{
	// 1. 예외처리.
	if ( n )
	// 1. 값+더할값 > 값 : 값을 더하면 원래 값보다 더 커져야 함.
	// 2. 값+더할값 > 더할값 : 더해진 값은 더한 값보다 커야 함.
	// (max-1) + (1) = max > max-1 : true
	// (max-1) + (1) = max > 1 : true
	//
	// (max-1) + (2) = 0 > max-1 : false
	// (max-1) + (2) = 0 > 2 : false
	//
	// (max) + (max) = max-1 > max : false
	if ( goods.count+n > n )
	if ( goods.count+n > goods.count )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 계산 후 반환.
	return( goods.count+n );
	// -- 예외처리.
	SKIP:return( goods.count );
}


// -- base - 재고 n개 갑소.
// 정상 작업 시 재고 n개 감소된 값 반환.
// 현재 재고 수보다 n이 더 큰 경우 예외처리됨.
// 예외처리 시 goods.count 반환.
uint32_t f$dssp$base$goodsDecline( t$dssp$goods_info goods, const uint32_t n )
{
	// 1. 예외처리.
	if ( goods.count >= n )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 결과 반환.
	return( goods.count-n );
	// -- 예외처리.
	SKIP:return( goods.count );
}


// -- base - 매점 총 판매 개수 n개 추가.
// 정상 작업 시 n개 추가된 값 반환.
// 오버플로우 발생 시 예외처리.
// 예외처리 시 store.total_sales_count 반환.
uint32_t f$dssp$base$storeTotalSalesAdditional( const t$dssp$store store, const uint32_t n )
{
	// 1. 예외처리.
	if ( n )
	if( store.total_sales_count+n > n )
	if( store.total_sales_count+n > store.total_sales_count )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 추가된 값 반환.
	return( store.total_sales_count + n );
	// 예외처리.
	SKIP:return( store.total_sales_count );
}


// -- base - 매장 - 총 판매액 추가 함수.
// 예외를 검사하지 않는 함수.
// 넘겨받은 매점 객체의 값을 수정함.
// 추가된 후의 금액을 반환함.
uint64_t f$dssp$base$addTotalPrice( t$dssp$store* store_p, const uint64_t add_price )
{
	// 1. 추가하기.
	store_p->total_sales_price += add_price;

	// 2. 종료.
	SKIP:return( store_p->total_sales_price );
}


// -- 재고 n개 추가.
// 키에 해당하는 물품의 재고를 n개 추가한 매점 객체를 반환함.
// 예외처리 시 store 반환됨.
t$dssp$store f$dssp$goodsAdditional( t$dssp$store store, char*const key_name, const uint32_t n )
{
	// 0. 선언.
	t$dssp$store result_store = store;
	t$dssp$goods_info* selected_goods = { 0 };
	uint32_t updated_count = 0;
	t$dssp$node* get_node = 0;
	
	// 1. 예외처리.
	if ( n )
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 물품 이름으로 물품 정보 객체 포인터 가져오기.
	get_node = store.dict.find(Key(key_name));
	// 널 포인터 시 예외처리.
	if ( get_node == 0 ) goto SKIP;
	// 포인터 적용하기.
	selected_goods = &get_node->value;

	// 3. 추가된 량 구하기.
	updated_count = f$dssp$base$goodsAdditional(*selected_goods, n);

	// 4. 딕셔너리의 재고량 업데이트.
	selected_goods->count = updated_count;

	// 5. 매점 판매 개수 적용하기.
	result_store.total_sales_count += updated_count - selected_goods->count;

	// 6. 매점 객체 반환.
	SKIP:return( result_store );
}


// -- 재고 n개 손실.
// 키에 해당하는 물품의 재고를 n개 손실시킴.
// 예외처리 시 변경 사항 없음, store 반환됨.
// f$dssp$goodsDecline 함수에 종속됨.
t$dssp$store f$dssp$goodsLoss( t$dssp$store store, char*const key_name, const uint32_t n )
{
	// 0. 선언.
	t$dssp$store result_store = store;
	t$dssp$goods_info* selected_goods = 0;
	uint32_t updated_count = 0;
	t$dssp$node* get_node = 0;
	
	// 1. 예외처리.
	if ( n )
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;
	
	// 2. 물품 이름으로 물품 정보 객체 포인터 받기.
	get_node = store.dict.find(Key(key_name));
	// 널 포인터 시 예외처리.
	if ( get_node == 0 ) goto SKIP;
	// 포인터 적용하기.
	selected_goods = &get_node->value;

	// 3. 감소된 재고량 가져오기.
	// 예외처리로 인해 값이 변경되지 않을 수 있다.
	updated_count = f$dssp$base$goodsDecline(*selected_goods, n);
	
	// 4. 적용된 손실된 량 구하기.
	result_store.total_loss_count += selected_goods->count - updated_count;

	// 5. 딕셔너리의 재고량 업데이트.
	selected_goods->count = updated_count;

	// 6. 매점 객체 반환.
	SKIP:return( result_store );
}


// -- 물품 구매하기 함수.
///////////////////////// 미완성.
t$dssp$store XXf$dssp$buyGoods( t$dssp$store store, char*const key_name, const uint32_t buy_number )
{
	// 0. 선언.
	t$dssp$store result_store = store;
	t$dssp$node* get_node = 0;
	t$dssp$goods_info* selected_goods = 0;
	
	// 1. 예외처리.
	if ( buy_number )
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 물품 정보 객체 포인터 구하기.
	get_node = store.dict.find(Key(key_name));
	// 예외처리.
	if ( get_node == 0 ) goto SKIP;
	// 포인터 적용시키기.
	selected_goods = &get_node->value;

	// 2. 구매로 인해 buy_number 만큼 재고 소진.
	// 총 판매 개수. 행사가 적용 총 판매 개수.
	// 총 판매 액수. 행사가 적용 총 판매 액수.
	{
		// 0. 선언.
		uint32_t gifts_count = 0;
		uint64_t gifts_price = 0;
		uint32_t updated_count = 0;
		
		// 1. 증정품 개수 구하기.
		gifts_count = f$dssp$numberOfDiscount(*selected_goods, buy_number);
		// 2. 할인된 비용 구하기.
		gifts_price = f$dssp$priceOfDiscount(*selected_goods, buy_number);

		// 3. 감소된 개수 구하기.
		updated_count = f$dssp$base$goodsDecline(*selected_goods, buy_number);
		// 4. 총 판매 개수 업데이트.
		result_store.total_sales_count += selected_goods->count - updated_count;
		// 5. 물품 객체 정보 업데이트.
		// 카운트 중인 감소되는 재고 개수.
		selected_goods->total_downcount += selected_goods->count - updated_count;
		// 현재 재고 개수.
		selected_goods->count = updated_count;

		// 6. 총 판매 액수 업데이트.
		// 단순히 개수만큼 더해주는 것이 아님.
		// 증정되는 개수를 제외한 개수 만큼만 계산에 적용된다.
		// 선택된 물품의 가격을 곱한다.
		// 다만 증정품 개수는 제외해서 가격에 곱할 것.
		f$dssp$base$addTotalPrice(&result_store, selected_goods->price*(buy_number-gifts_count));

		// 7. 행사가 적용 총 판매 액수 업데이트.
		
	}

	// 3. 
	SKIP:;
}


// -- 매점 시스템 이름 설정.
// store_name이 0인 경우 unamed 상태가 됨.
int f$dssp$setStoreName( t$dssp$store& store_r, char*const store_name )
{
	// 0. 선언.
	int len = 0;
	char* new_str = 0;
	
	// 1. 예외처리.
	if ( store_name == 0 ) goto SKIP;
	
	// 2. 문자열 길이 구하기.
	len = strlen(store_name)+1;

	// 3. 메모리 할당.
	for ( uint8_t c=0; !( new_str=(char*)malloc(len) ) && c<3; c++ );
	if ( new_str == 0 ) goto SKIP;

	// 4. 문자열 복사.
	strcpy(new_str, store_name);

	// 5. 이름 설정.
	store_r.store_name = new_str;

	// 6. 성공 여부 반환.
	return( 1 );
	SKIP:return( 0 );
}


// -- 메뉴 번호 to 고유 메뉴 번호.
t$dssp$loop_state f$dssp$numToMenu( const int num )
{
	switch ( num )
	{
		// -- 0. 물품 리스트.
		case 0:return( v$dssp$2ADD );
	}
}


// --[[ cli system function ]]
// -- print + 개행 함수.
int f$dssp$cli$print( const char*const str, int return_n )
{
	int result = 0;
	
	result += printf("-  %s\n", str);
	for ( ; return_n!=0; return_n-- )
		result += printf("-\n");

	return( result );
}


// -- get num 함수.
int f$dssp$cli$read( const char*const prompt_str, int& num )
{
	f$dssp$cli$print(prompt_str,0);
	printf(" > "); scanf("%d", &num);
	f$dssp$cli$print("",0);
	return( 1 );
}


// -- 시스템 시작 화면.
int f$dssp$cli$bootScreen()
{
	f$dssp$cli$print("매점 재고 관리 시스템", 0);
	f$dssp$cli$print("관리버전 : " VERSION_DSSP, 2);
	return( 1 );
}


// -- 조작 메뉴 출력 화면.
int f$dssp$menuScreen()
{
	f$dssp$cli$print("[ 제어 ]",0);
	f$dssp$cli$print("0.  물품 리스트 : 등록된 물품 정보들을 보여줍니다.",0);
	f$dssp$cli$print("1.  물품 선택 : 물품의 이름으로 물품을 선택합니다.",0);
	f$dssp$cli$print("2.  물품 추가 : 새 물품을 등록합니다.",0);
	f$dssp$cli$print("3.  물품 제거 : 선택된 물품을 등록에서 완전히 제거합니다.",0);
	f$dssp$cli$print("4.  매점 통계 : 현재 매점에 대한 통계 정보를 제공합니다.",2);
	return( 1 );
}


// --[[ class ]]
struct class$DSSP
{
	t$dssp$goods_info(*const newGoodsInfo)( const uint32_t price);
	t$dssp$store(*const newStore)( const char*const store_name );
	int(*const setPrice)( t$dssp$store store, char*const key_name, const uint64_t price );
	int(*const setGifts)( t$dssp$store store, char*const key_name, const uint8_t n );
	int(*const insertGoodsInfo)( t$dssp$store store, char*const key_name, t$dssp$goods_info goods_info );
	int(*const removeGoodsInfo)( t$dssp$store store, char*const key_name );
	int(*const renameGoods)( t$dssp$store store, char*const key_name, char*const rename );
	t$dssp$store(*const goodsAdditional)( t$dssp$store store, char*const key_name, const uint32_t n );
	t$dssp$store(*const goodsLoss)( t$dssp$store store, char*const key_name, const uint32_t n );
	int(*const setStoreName)( t$dssp$store& store_r, char*const store_name );
	struct Cli
	{
		int(*const print)( const char*const str, int return_n );
		int(*const read)( const char*const prompt_str, int& num );
		int(*const bootScreen)();
		int(*const menuScreen)();
	} cli;
} DSSP = {
	.newGoodsInfo = f$dssp$newGoodsInfo,
	.newStore = f$dssp$newStore,
	.setPrice = f$dssp$setPrice,
	.setGifts = f$dssp$setGifts,
	.insertGoodsInfo = f$dssp$insertGoodsInfo,
	.removeGoodsInfo = f$dssp$removeGoodsInfo,
	.renameGoods = f$dssp$renameGoods,
	.goodsAdditional = f$dssp$goodsAdditional,
	.goodsLoss = f$dssp$goodsLoss,
	.setStoreName = f$dssp$setStoreName,
	.cli = {
		.print = f$dssp$cli$print,
		.read = f$dssp$cli$read,
		.bootScreen = f$dssp$cli$bootScreen,
		.menuScreen = f$dssp$menuScreen,
	},
};

#ifdef USING_DSSP

typedef t$dssp$loop_state LoopState;
typedef t$dssp$goods_info GoodsInfo;
typedef t$dssp$store Store;

// -- 부트 상태.
const LoopState dssp_BOOTTIME = v$dssp$BOOTTIME;

// -- 기본 상태.
const LoopState dssp_DEFAULT = v$dssp$DEFAULT;

// -- 조작 도움말.
const LoopState dssp_MENU = v$dssp$MENU;

// -- 메뉴 처리 - 물품 리스트.
const LoopState dssp_SHOW_LIST = v$dssp$0SHOW_LIST;

// -- 오류 상태.
const LoopState dssp_TASK_FAIL = v$dssp$TASK_FAIL;

// -- 0. 물품 리스트.
const LoopState dssp_SHOW_LIST = v$dssp$0SHOW_LIST;

// -- 1. 물품 선택.
const LoopState dssp_SELECT = v$dssp$1SEELCT;

// -- 2. 물품 추가.
const LoopState dssp_ADD = v$dssp$2ADD;

// -- 3. 물품 추가.
const LoopState dssp_ADD = v$dssp$2ADD;

// -- 3. 물품 삭제.
const LoopState dssp_REMOVE = v$dssp$3REMOVE;


#endif