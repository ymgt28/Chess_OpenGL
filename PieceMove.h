/*-------------------------------------
	PieceMove.h
	チェス駒の動き各クラスヘッダ
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
---------------------------------------*/

#pragma once

#include "Chess.h"
#include "GameBoard.h"

class King{
public:
	King(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//動ける場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
private:
	GameBoard *bo;									//情報管理クラスインスタンス
};

class Queen{
public:
	Queen(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//動ける場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
private:
	GameBoard *bo;									//情報管理クラスインスタンス
};

class Rook{
public:
	Rook(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//動ける場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
private:
	GameBoard *bo;									//情報管理クラスインスタンス
};

class Bishop{
public:
	Bishop(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//動ける場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
private:
	GameBoard *bo;									//情報管理クラスインスタンス
};

class Knight{
public:
	Knight(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//動ける場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
private:
	GameBoard *bo;									//情報管理クラスインスタンス
};

class Pawn{
public:
	Pawn(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//動ける場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
	void ActPoint(int x, int y, Player player);		//ポーンが敵駒を取れる場所をOKフラグとしてデータに入れる
	//x,y: 駒のある位置, player:プレイヤー情報
private:
	GameBoard *bo;									//情報管理クラスインスタンス
};
