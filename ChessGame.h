/*-------------------------------------
	ChessGame.h
	チェスゲーム管理クラスヘッダ
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
---------------------------------------*/

#pragma once

#include "Chess.h"
#include "GameBoard.h"
#include "PieceMove.h"

/*---キャスリング管理構造体---*/
typedef struct{
	bool flag;			//キャスリングが行えるか
	Point bfRook;		//キャスリングする前のルーク座標
	Point afRook;		//キャスリングした後のルーク座標
	Point bfKing;		//キャスリングする前のキング座標
	Point afKing;		//キャスリングした後のキング座標
	Point bet[2];		//ルーク・キング間にある座標かつキングが通る座標
	bool oneTime;		//キャスリングを既に行っていないか
}castling;

/*---キャスリング管理識別子---*/
enum CastlingSide{
	WKSide = 0,		//白キングサイド
	WQSide,			//白クイーンサイド
	BKSide,			//黒キングサイド
	BQSide			//黒クイーンサイド
};

/*---アンパッサン管理構造体---*/
typedef struct {
	bool flag;
	Point movePawn;					//取られるポーン位置
	Point takePawn[2];				//取るポーン位置（挟んでる場所の2つある）
	int takeXdiff;					//takePawnが動くときの差分
}EnPassant;

class ChessGame{
public:
	ChessGame(GameBoard *p);		//コンストラクタ
	void CheckMove(int x, int y);	//駒が動かせる場所をゲーム的に考える
	void Move(int afterX, int afterY, int beforeX, int beforeY);	//駒の移動
	GameTurn CheckTurn(void){return m_turn;}	//ゲーム状況表示（手番・END内容）を返す
	Player CheckPlayer(void){return m_player;}	//白黒どちらの出番かを返す
	bool CheckingTurn(void){return (isChecking ? true : false);}	//今チェックされている状態かどうかを返す

private:
	GameBoard *m_bo;				//盤情報
	Player m_player;				//手番（White/Black）
	castling cas[4];				//キャスリング管理構造体配列（白黒それぞれのクイーン・キングサイドの4つ）
	EnPassant enp;					//アンパッサン管理構造体
	int m_BKingX;					//キングの位置を捕捉しておく
	int m_BKingY;
	int m_WKingX;
	int m_WKingY;
	bool isChecking;				//今チェックされている状態かどうか
	GameTurn m_turn;				//今の手番

	/*---非公開メソッド---*/
	void CheckCastling(void);		//キャスリングが行えるかチェックしフラグ挿入
	bool GetCheckP(int x, int y);	//ある地点に対して、敵の駒筋が効いてるかを確認する
	//x,y:指定する座標
	bool GetCheckP(int x, int y, GameBoard *tempBo); //GetCheck()の一時的な盤情報指定版
	bool GetCheckP(int x, int y, GameBoard *tempBo, Player p);	//GetCheckP()のプレイヤー指定版
	void CheckMove(int x, int y, Player player, GameBoard *b);	//駒が動かせる場所をそのプレイヤーとして考える
	GameTurn CheckEnd(void);						//ゲーム終了判断
	bool CheckMoveALL(Player player);							//そのプレイヤーの駒が動かせる場所を全探索する
	
	inline Player RetEP(void){return (m_player == White ? Black : White);}
};
