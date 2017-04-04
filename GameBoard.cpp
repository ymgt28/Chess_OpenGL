/*--------------------------------------
	GameBoard.cpp
	ゲーム盤管理クラス
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
----------------------------------------*/

#include <iostream>
#include "GameBoard.h"

GameBoard::GameBoard(){			//コンストラクタ
}

//デバッグ用
void GameBoard::Print(void){
	for(int i=0; i<BNUM; ++i){
		for(int j=BNUM-1; j>=0; --j){
			std::cout << (cbo[i][j].Isel ? "True  " : "False ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//コピーメソッド
void GameBoard::Copy(const GameBoard *gb){

	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; j++){
			cbo[i][j].pi = gb->cbo[i][j].pi;
			cbo[i][j].cmove = yet;
			cbo[i][j].Dsel = false;
			cbo[i][j].Isel = false;
			cbo[i][j].initP = gb->cbo[i][j].initP;
		}
	}
}

//盤の駒の初期化
void GameBoard::InitBoardPiece(void){

	//初期状態の盤
	ChessPiece initBoard[BNUM][BNUM] =
	{{ BRook, BKnight, BBishop, BKing, BQueen, BBishop, BKnight, BRook },
	{ BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn },
	{ WRook, WKnight, WBishop, WKing, WQueen, WBishop, WKnight, WRook }};
	/*ChessPiece initBoard[BNUM][BNUM] =
	{{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, BKing, WKing, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty },
	{ empty, empty, empty, empty, empty, empty, empty, empty }};*/

	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			cbo[i][j].pi = initBoard[i][j];
			cbo[i][j].Dsel = false;
			cbo[i][j].Isel = false;
			if( cbo[i][j].pi == empty )			//中央部分は常にinitPがfalse
				cbo[i][j].initP = false;
			else
				cbo[i][j].initP = true;
		}
	}
}

//クリアフラグ
void GameBoard::ClearFlag(void){
	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			cbo[i][j].Dsel = false;
			cbo[i][j].Isel = false;
			cbo[i][j].cmove = yet;
		}
	}
}

//駒の移動
void GameBoard::MovePiece(int afterX, int afterY, int beforeX, int beforeY){
	
	cbo[afterX][afterY].pi = cbo[beforeX][beforeY].pi;
	cbo[beforeX][beforeY].pi = empty;
	if( cbo[beforeX][beforeY].initP )
		cbo[beforeX][beforeY].initP = false;
}

//OKフラグからIsel->trueフラグへ変更
void GameBoard::ChangeOK(void){

	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			if(cbo[i][j].cmove == ok)
				cbo[i][j].Isel = true;
			
			cbo[i][j].cmove = yet;
		}
	}
}

//その位置にある駒のプレイヤー情報を得る(White/Black)
Player GameBoard::GetPiecePlayer(int x, int y){

	if( x < 0 || y < 0 || BNUM <= x || BNUM <= y )
		return Yet;

	ChessPiece pi = GetPiece(x,y);
	switch(pi){
		case BKing:
		case BQueen:
		case BRook:
		case BBishop:
		case BKnight:
		case BPawn:
			return Black;
		case WKing:
		case WQueen:
		case WRook:
		case WBishop:
		case WKnight:
		case WPawn:
			return White;
		case empty:
		default:
			return Yet;
	}
}

//YetフラグをすべてNGに変更
void GameBoard::ChangeYetNG(void){

	if( !OKpoint.empty() )		//空でないなら
		OKpoint.clear();		//全クリア

	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			if(cbo[i][j].cmove == yet)
				cbo[i][j].cmove = ng;

			if(cbo[i][j].cmove == ok){
				Point temp;
				temp.x = i;
				temp.y = j;
				OKpoint.push_back(temp);
			}
		}
	}
}

//プロモーション処理
void GameBoard::Promotion(int Y, Player player){

	//とりあえずクイーン固定
	if( player == White){
		cbo[0][Y].pi = WQueen;
	}else{
		cbo[7][Y].pi = BQueen;
	}
}