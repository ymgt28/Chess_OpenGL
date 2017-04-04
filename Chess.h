/*-------------------------------------
	Chess.h
	チェスゲーム共通ヘッダ
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
---------------------------------------*/

#pragma once

//#define DEGUB_SelectHits	//SelectHitsの処理内容を見る
//#define DEBUG_MINILOAD	//デバッグ用短縮ロード

/*---駒種別用識別子---*/
enum ChessPiece{
	empty = -1,
	BKing = 0,
	BQueen,
	BRook,
	BBishop,
	BKnight,
	BPawn,
	WKing,
	WQueen,
	WRook,
	WBishop,
	WKnight,
	WPawn,
};

/*---白黒識別子---*/
enum Player{
	Yet = -1,	//未決定
	White,		//白
	Black,		//黒
	End,		//終わり
};

/*---手番識別子---*/
enum GameTurn{
	Continue = -1,	//ゲーム続行
	WhiteTurn,		//白
	BlackTurn,		//黒
	EndW,			//ゲーム終了(Whiteの勝ち)
	EndB,			//ゲーム終了(Blackの勝ち)
	EndST,			//ゲーム終了(ステイルメイト)
};

/*---各種固定変数---*/
const int BNUM = 8;					//盤の一辺の数
const int NOBJECTS = 64;			//盤オブジェクトの数
const int MODELNUM = 6;
const int SELECTIONS = 100;			//セレクションバッファのサイズ
const float BoardScale = 0.3f;		//盤の立方体ポリゴンスケール
const float PieceScale = 0.15f;		//駒のスケール

//*---盤の各マス座標を示す構造体---*/
typedef struct{
	int x;
	int y;
}Point;