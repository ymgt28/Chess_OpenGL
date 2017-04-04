/*-------------------------------------
	GameBoard.h
	ゲーム盤管理クラスヘッダ
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
---------------------------------------*/

#pragma once

#include <vector>
#include "Chess.h"

/*---駒が動けるかの識別子---*/
enum CanMove{
	yet = -1,		//未決定
	ok = 0,			//動ける場所
	ng,				//元々動けない（駒の現在位置・味方の駒がある場合含む）
	check			//チェックの関係で動けない
};

/*---盤の各マス実現構造体---*/
typedef struct{
	ChessPiece pi;					//このマスにある駒
	bool Dsel;						//直接セレクトされているかどうか(DirectSelected)
	bool Isel;						//間接的にセレクトされているかどうか(IndirectSelected)
	CanMove cmove;					//動けるかどうかの識別
	bool initP;						//初期位置から動いたかどうか(falseで動いた)
}CBoard;

class GameBoard{
public:
	GameBoard();																		//コンストラクタ
	GameBoard(const GameBoard *gb){this->Copy(gb);}
	void Copy(const GameBoard *gb);														//コピーメソッド
	
	void InitBoardPiece(void);															//盤の駒の初期化
	void ClearFlag(void);																//Isel,Dselのクリア("false"化)
	void MovePiece(int afterX, int afterY, int beforeX, int beforeY);					//駒の移動
	void ChangeOK(void);																//OKフラグからIsel->trueフラグへ変更
	void ChangeYetNG(void);																//YetフラグをすべてNGに変更
	void Promotion(int Y, Player player);												//プロモーション処理

	/*---Get系---*/
	ChessPiece GetPiece(int x,int y){return cbo[x][y].pi;}								//その位置の駒情報を得る
	bool GetDsel(int x, int y){return (cbo[x][y].Dsel ? true : false);}					//その位置のDsel情報を得る
	bool GetIsel(int x, int y){return (cbo[x][y].Isel ? true : false);}					//その位置のIsel情報を得る
	Player GetPiecePlayer(int x, int y);												//その位置にある駒のプレイヤー情報を得る(White/Black)
	bool CheckPieceInitP(int x, int y){return (cbo[x][y].initP ? true : false);}		//その位置にある駒が初期位置から動いたかどうか
	bool GetOK(int x, int y){return (cbo[x][y].cmove == ok ? true : false);}			//その位置にあるフラグがOKかどうか
	std::vector<Point>* GetOKpoint(void){return &OKpoint;}								//OKフラグがあるポイントを格納したvectorを参照渡しで返す

	/*---Set系---*/
	void SetDselTrue(int x, int y){	cbo[x][y].Dsel = true;}								//指定された場所のDselフラグにtrueを入れる
	void SetOK(int x, int y){		cbo[x][y].cmove = ok;}								//指定された場所へOK_piece識別子を入れる
	void SetNG(int x, int y){		cbo[x][y].cmove = ng;}								//指定された場所へNG識別子を入れる
	void SetCheck(int x, int y){	cbo[x][y].cmove = check;}							//指定された場所へcheck識別子を入れる
	void SetEmpty(int x, int y){	cbo[x][y].pi = empty;}								//指定された場所の駒を消す

	GameBoard& GameBoard::operator=(const GameBoard *gb){this->Copy(gb); return *this;}	// "="のオーバーロード（クラスコピー）

	//デバッグ
	void Print(void);

private:
	CBoard cbo[BNUM][BNUM];																//統合された盤情報
	std::vector<Point> OKpoint;															//OKフラグがある位置を格納したvector
};