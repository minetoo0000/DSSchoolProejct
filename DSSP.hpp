/**
 * DSSP.hpp
 * 선린여학원 매점 재고 관리 프로그램 라이브러리
 * 
 * 2023-06-13
 * pm-6:03
*/

// --[[ setting ]]
// #define USING_DSSP


// --[[ include ]]
#include <stdint.h>
#include <malloc.h>


// --[[ interface ]]
// -- 상품 정보.
typedef struct t$dssp$goods_info
{
	// 가격.
	uint32_t price;
	// 재고 수.
	uint32_t count;
	// 물품 총 재고추가량.
	uint32_t total_upcount;
	// 물품 총 재고 소진량.
	uint32_t total_downcount;
	// 물품 총 판매액
	uint64_t total_sales;
	// 추가 증정량. "n+m 할인 중"
	uint8_t additional_n:4;
	uint8_t additional_m:4;
} t$dssp$goods_info;


// -- 매점 객체.
typedef struct t$dssp$store
{
	// 지점 이름.
	char* store_name;
	// 총 판매 개수.
	uint32_t total_sales_count;
	// 총 판매 액수.
	uint64_t total_sales_price;
	// 행사가 적용 상품 총 판매 개수.
	uint32_t total_sold_count;
	// 손실된 상품 총 개수.
	uint32_t total_loss_count;
	// 재고 종류 개수.
	uint32_t total_goods;
	// 재고 딕셔너리 객체.
	/////////////////////////////////////// 스킵리스트 - 딕셔너리 객체.
} t$dssp$store;


// --[[ 임시 함수 ]]
// -- 딕셔너리에 키:값 설정.
// 이미 키 값이 존재한다면 값만 적용.
// 새 키라면 키:값 쌍 추가.
// 개발선택사항 : 성공 시 1을, 실패 시 0을 반환.
int __set( const char*const key_name, const t$dssp$goods_info value_goods_info )
{
	return( 0 );
}


// -- 딕셔너리에서 키:값 쌍 삭제.
// 개발선택사항
// 해당 키가 발견되지 않거나 삭제에 성공하면 1을,
// 해당 키가 존재하지만 삭제에 실패한 경우에는 0을 반환.
int __remove( const char*const key_name )
{
	return( 0 );
}


// -- 키에 해당하는 상품 정보 객체 포인터 반환.
// 해당하는 키가 없다면 널포인터 0을 반환함.
t$dssp$goods_info* __find( const char*const key_name )
{
	t$dssp$goods_info* get = 0;
	return( get );
}


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
 *     [ 행사중! ]
 *     상품 할인가 : 750원 (50% 할인)
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
t$dssp$goods_info f$dssp$newProductInfo( const uint32_t price)
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
		}
	);
}


// -- 할인이 적용된 가격 구하기 함수.
uint64_t f$dssp$priceOfDiscount( const t$dssp$goods_info goods, const uint32_t buy_number )
{
	// 0. 선언.
	uint64_t result_price = 0;
	
	// 1. 예외처리.
	if ( !buy_number ) goto SKIP;

	// 2. 구하기.
	(buy_number/goods.additional_n)*goods.additional_m
}


// -- 할인이 적용된 물품의 개수 구하기 함수.
///////////////////////// 할인 증정량 구하기에서 막힘.
uint32_t XXf$dssp$numberOfDiscount( const t$dssp$goods_info goods, const uint32_t buy_number )
{
	// 1. 계산.
	if ( buy_number%goods.additional_n )
	{
		/
	}
	((buy_number/goods.additional_n)*goods.additional_m);
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


// -- 재고 n개 추가.
// 키에 해당하는 물품의 재고를 n개 추가한 매점 객체를 반환함.
// 예외처리 시 store 반환됨.
t$dssp$store f$dssp$goodsAdditional( const t$dssp$store store, const char*const key_name, const uint32_t n )
{
	// 0. 선언.
	t$dssp$store result_store = store;
	t$dssp$goods_info* current_goods = 0;
	uint32_t updated_count = 0;
	
	// 1. 예외처리.
	if ( n )
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 물품 이름으로 물품 정보 객체 포인터 가져오기.
	current_goods = __find(key_name);
	// 널 포인터 시 예외처리.
	if ( !current_goods ) goto SKIP;

	// 3. 추가된 량 구하기.
	updated_count = f$dssp$base$goodsAdditional(*current_goods, n);

	// 4. 딕셔너리의 재고량 업데이트.
	current_goods->count = updated_count;

	// 5. 매점 판매 개수 적용하기.
	result_store.total_sales_count += updated_count - current_goods->count;

	// 6. 매점 객체 반환.
	SKIP:return( result_store );
}


// -- 재고 n개 손실.
// 키에 해당하는 물품의 재고를 n개 감소함. 손실에 포함됨.
// 예외처리 시 변경 사항 없음, store 반환됨.
// f$dssp$goodsDecline 함수에 종속됨.
t$dssp$store f$dssp$goodsLoss( const t$dssp$store store, const char*const key_name, const uint32_t n )
{
	// 0. 선언.
	t$dssp$store result_store = store;
	t$dssp$goods_info* current_goods = 0;
	uint32_t updated_count = 0;
	
	// 1. 예외처리.
	if ( n )
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;
	
	// 2. 물품 이름으로 물품 정보 객체 포인터 받기.
	current_goods = __find(key_name);
	// 널 포인터 시 예외처리.
	if ( !current_goods ) goto SKIP;

	// 3. 감소된 재고량 가져오기.
	// 예외처리로 인해 값이 변경되지 않을 수 있다.
	updated_count = f$dssp$base$goodsDecline(*current_goods, n);
	
	// 4. 적용된 손실된 량 구하기.
	result_store.total_loss_count += current_goods->count - updated_count;

	// 5. 딕셔너리의 재고량 업데이트.
	current_goods->count = updated_count;

	// 6. 매점 객체 반환.
	SKIP:return( result_store );
}


// -- 물품 구매하기 함수.
t$dssp$store f$dssp$buyGoods( const t$dssp$store store, const char*const key_name, const uint32_t buy_number )
{
	// 0. 선언.
	t$dssp$store result_store = store;
	
	// 1. 예외처리.
	if ( buy_number )
	if ( key_name )
	goto KEEP;
	goto SKIP;
	KEEP:;

	// 2. 구매로 인해 buy_number 만큼 재고 소진.
	result_store = f$dssp$goodsLoss(result_store, key_name, buy_number);

	// 3. 
}