/*--------------------------------------
	Board.h
	盤表現クラス
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
----------------------------------------*/

#pragma once

#include <iostream>
#include <string>

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "XLoader.h"
#include "Chess.h"
#include "GameBoard.h"
#include "PieceMove.h"
#include "ChessGame.h"

/*---駒種別用識別子（モデルデータロード用）---*/
enum ChessPieceModel{
	King = 0,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};

class BoardAdm{
public:
	BoardAdm();								//コンストラクタ
	~BoardAdm();							//デストラクタ
	void Init();							//初期化関数
	void Draw(void);						//全体描画関数
	void SetCurPoint(int x, int y){curX = x; curY = y; mouse_state = true;}	//マウスを当てられた座標を受け取る
	std::string GetTurn(void);					//どちらの出番かを返す
	//戻り値: "白","黒"
	bool GetChecking(void){return cgame->CheckingTurn();}

private:
	GameBoard *bo;							//チェス盤を表現するインスタンス
	ChessGame *cgame;						//チェスゲームを管理するインスタンス
	XModel model[MODELNUM];					//モデル表示用（駒12個分）
	GLuint objects;							//ディスプレイリストの識別子
	GLfloat *color[NOBJECTS];				//各オブジェクトの色
	int curX;								//現在マウスが当てられている座標
	int curY;
	bool mouse_state;						//マウスが現在クリックされてるかどうか
	GLuint selection[SELECTIONS];			//セレクションバッファ
	GLint hits;								//セレクトされたオブジェクトの数

	int m_select;							//セレクトされているマスのオブジェクト番号

	/*---非公開メソッド---*/
	void DrawBoard(void);					//盤表示関数
	void DrawBoardPoint(void);				//盤のマウスを当てられた場所を強調表示
	//引数:マウスのXY座標

	void DrawPiece(void);					//駒表示関数

	void BoardInit(void);					//盤の初期化関数
	void PieceInit(void);					//駒の初期化関数
	void SelectPoint(void);					//マウスが当たっている場所を判定
	int SelectHits(void);					//ヒットしたオブジェクトを識別
	//戻り値:識別したオブジェクト番号 or -1
	void SelectDo(int hit_name);			//識別したオブジェクトに対する動作
	//引数:識別したオブジェクト番号
	
	ChessPieceModel RetModelNo(ChessPiece pi);	// ChessPiece→ChessPieceModelの変換

	inline int ReturnX(int a){return a/BNUM;}	//オブジェクト識別子からX座標へ変換
	inline int ReturnY(int a){return a%BNUM;}	//オブジェクト識別子からY座標へ変換
};
