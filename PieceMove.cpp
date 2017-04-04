/*-------------------------------------
	PieceMove.cpp
	チェス駒の動きクラス群
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
---------------------------------------*/

#include "PieceMove.h"

//キング
void King::MovePoint(int x, int y, Player player){

	const int NMOVE = 8;			//動ける最大のマス数

	int diff[NMOVE][2] = {		//差分データ（0→x, 1→y）
		{0,1},{0,-1},
		{1,0},{-1,0},
		{1,1},{1,-1},
		{-1,1},{-1,-1}
	};

	for(int i=0; i<NMOVE; ++i){
		if( x+diff[i][0] < 0 || (BNUM-1) < x+diff[i][0]
			|| y+diff[i][1] < 0 || (BNUM-1) < y+diff[i][1] )	//範囲外は弾く
				continue;
			if( bo->GetPiecePlayer(x+diff[i][0], y+diff[i][1]) != player )
				bo->SetOK(x+diff[i][0], y+diff[i][1]);
			else
				bo->SetNG(x+diff[i][0], y+diff[i][1]);
	}
}

//クイーン
void Queen::MovePoint(int x, int y, Player player){

	const int NVECTMOVE = 8;	//動ける方向が8方位

	int diff[NVECTMOVE][2] = {	//8方向に対する差分データ（0→x, 1→y）
		{0,1},{0,-1},
		{1,0},{-1,0},
		{1,1},{1,-1},
		{-1,1},{-1,-1}
	};

	int tempX, tempY;			//計算上の仮XY位置
	bool jump;					//駒を飛び越えないフラグ(trueでそれ以上進められない)

	for(int i=0; i<NVECTMOVE; ++i){		//8方位に対するループ
		jump = false;
		tempX = x;
		tempY = y;
		for(int j=0; j<8; ++j){			//最大で8マス進む
			tempX += diff[i][0];
			tempY += diff[i][1];

			if( tempX < 0 || (BNUM-1) < tempX 
				|| tempY < 0 || (BNUM-1) < tempY)	//範囲外に到達するとループを抜ける
				break;

			if( jump ){													//飛び越えられないフラグON
				bo->SetNG(tempX,tempY);
			}else if( bo->GetPiecePlayer(tempX,tempY) == player ){		//その場所に味方の駒がある場合
				bo->SetNG(tempX,tempY);
				jump = true;
			}else if( bo->GetPiecePlayer(tempX,tempY) == Yet ){			//その場所には何もない場合
				bo->SetOK(tempX,tempY);
			}else{														//その場所に敵の駒がある場合
				bo->SetOK(tempX,tempY);
				jump = true;
			}
		}
	}
}

//ルーク
void Rook::MovePoint(int x, int y, Player player){

	const int NVECTMOVE = 4;	//動ける方向が4方位

	int diff[NVECTMOVE][2] = {	//4方向に対する差分データ（0→x, 1→y）
		{0,1},{0,-1},
		{1,0},{-1,0},
	};

	int tempX, tempY;			//計算上の仮XY位置
	bool jump;					//駒を飛び越えないフラグ(trueでそれ以上進められない)

	for(int i=0; i<NVECTMOVE; ++i){		//4方位に対するループ
		jump = false;
		tempX = x;
		tempY = y;
		for(int j=0; j<8; ++j){			//最大で8マス進む
			tempX += diff[i][0];
			tempY += diff[i][1];

			if( tempX < 0 || (BNUM-1) < tempX 
				|| tempY < 0 || (BNUM-1) < tempY)	//範囲外に到達するとループを抜ける
				break;

			if( jump ){													//飛び越えられないフラグON
				bo->SetNG(tempX,tempY);
			}else if( bo->GetPiecePlayer(tempX,tempY) == player ){		//その場所に味方の駒がある場合
				bo->SetNG(tempX,tempY);
				jump = true;
			}else if( bo->GetPiecePlayer(tempX,tempY) == Yet ){			//その場所には何もない場合
				bo->SetOK(tempX,tempY);
			}else{														//その場所に敵の駒がある場合
				bo->SetOK(tempX,tempY);
				jump = true;
			}
		}
	}
}

//ビショップ
void Bishop::MovePoint(int x, int y, Player player){

	const int NVECTMOVE = 4;	//動ける方向が4方位

	int diff[NVECTMOVE][2] = {	//4方向に対する差分データ（0→x, 1→y）
		{1,1},{1,-1},
		{-1,1},{-1,-1}
	};

	int tempX, tempY;			//計算上の仮XY位置
	bool jump;					//駒を飛び越えないフラグ(trueでそれ以上進められない)

	for(int i=0; i<NVECTMOVE; ++i){		//4方位に対するループ
		jump = false;
		tempX = x;
		tempY = y;
		for(int j=0; j<8; ++j){			//最大で8マス進む
			tempX += diff[i][0];
			tempY += diff[i][1];

			if( tempX < 0 || (BNUM-1) < tempX 
				|| tempY < 0 || (BNUM-1) < tempY)	//範囲外に到達するとループを抜ける
				break;

			if( jump ){													//飛び越えられないフラグON
				bo->SetNG(tempX,tempY);
			}else if( bo->GetPiecePlayer(tempX,tempY) == player ){		//その場所に味方の駒がある場合
				bo->SetNG(tempX,tempY);
				jump = true;
			}else if( bo->GetPiecePlayer(tempX,tempY) == Yet ){			//その場所には何もない場合
				bo->SetOK(tempX,tempY);
			}else{														//その場所に敵の駒がある場合
				bo->SetOK(tempX,tempY);
				jump = true;
			}
		}
	}
}

//ナイト
void Knight::MovePoint(int x, int y, Player player){

	const int NMOVE = 8;			//動ける最大のマス数

	int diff[NMOVE][2] = {		//差分データ（0→x, 1→y）
		{2,1},{2,-1},
		{1,2},{-1,2},
		{-2,1},{-2,-1},
		{1,-2},{-1,-2}
	};

	for(int i=0; i<NMOVE; ++i){
		if( x+diff[i][0] < 0 || (BNUM-1) < x+diff[i][0]
			|| y+diff[i][1] < 0 || (BNUM-1) < y+diff[i][1] )	//範囲外は弾く
				continue;

			if( bo->GetPiecePlayer(x+diff[i][0], y+diff[i][1]) != player)
				bo->SetOK(x+diff[i][0], y+diff[i][1]);
			else
				bo->SetNG(x+diff[i][0], y+diff[i][1]);
	}
}

//ポーン
void Pawn::MovePoint(int x, int y, Player player){

	if( player == White ){
		if( bo->GetPiece(x-1,y) != empty ){		//前に駒があるとき
			bo->SetNG(x-1,y);				//前に進めない
		}else{
			bo->SetOK(x-1,y);
			if( bo->CheckPieceInitP(x,y) && bo->GetPiece(x-2,y) == empty )		//初期位置かつ2個前に駒がないとき
				bo->SetOK(x-2,y);			//2マス進める
		}
		if( bo->GetPiecePlayer(x-1,y-1) != player && bo->GetPiecePlayer(x-1,y-1) != Yet )
			bo->SetOK(x-1,y-1);

		if( bo->GetPiecePlayer(x-1,y+1) != player && bo->GetPiecePlayer(x-1,y+1) != Yet )
			bo->SetOK(x-1,y+1);

	}else if( player == Black ){
		if( bo->GetPiece(x+1,y) != empty ){
			bo->SetNG(x+1,y);
		}else{
			bo->SetOK(x+1,y);
			if( bo->CheckPieceInitP(x,y) && bo->GetPiece(x+2,y) == empty )
				bo->SetOK(x+2,y);
		}
		if( bo->GetPiecePlayer(x+1,y+1) != player && bo->GetPiecePlayer(x+1,y+1) != Yet )
			bo->SetOK(x+1,y+1);

		if( bo->GetPiecePlayer(x+1,y-1) != player && bo->GetPiecePlayer(x+1,y-1) != Yet )
			bo->SetOK(x+1,y-1);

	}
}

//ポーンが敵駒を取れる場所をokフラグとしてデータに入れる
void Pawn::ActPoint(int x, int y, Player player){

	if( player == White ){
		if( bo->GetPiecePlayer(x-1,y-1) != player && bo->GetPiecePlayer(x-1,y-1) != Yet )
			bo->SetOK(x-1,y-1);

		if( bo->GetPiecePlayer(x-1,y+1) != player && bo->GetPiecePlayer(x-1,y+1) != Yet )
			bo->SetOK(x-1,y+1);

	}else if( player == Black ){
		if( bo->GetPiecePlayer(x+1,y+1) != player && bo->GetPiecePlayer(x+1,y+1) != Yet )
			bo->SetOK(x+1,y+1);

		if( bo->GetPiecePlayer(x+1,y-1) != player && bo->GetPiecePlayer(x+1,y-1) != Yet )
			bo->SetOK(x+1,y-1);

	}
}