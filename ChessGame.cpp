/*---------------------------------------
	ChessGame.cpp
	チェスゲーム管理クラス
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
-----------------------------------------*/

#include <iostream>
#include <vector>
#include "ChessGame.h"

//コンストラクタ
ChessGame::ChessGame(GameBoard *p){

	isChecking = false;
	m_bo = p;
	m_player = White;
	for(int i=0; i<4; ++i){
		cas[i].flag = false;		//キャスリングフラグクリア
		cas[i].oneTime = false;
	}

	//白キングサイド
	cas[WKSide].bfRook.x = 7;
	cas[WKSide].bfRook.y = 0;
	cas[WKSide].afRook.x = 7;
	cas[WKSide].afRook.y = 2;
	cas[WKSide].bfKing.x = 7;
	cas[WKSide].bfKing.y = 3;
	cas[WKSide].afKing.x = 7;
	cas[WKSide].afKing.y = 1;
	cas[WKSide].bet[0].x = 7;
	cas[WKSide].bet[0].y = 1;
	cas[WKSide].bet[1].x = 7;
	cas[WKSide].bet[1].y = 2;
	
	//白クイーンサイド
	cas[WQSide].bfRook.x = 7;
	cas[WQSide].bfRook.y = 7;
	cas[WQSide].afRook.x = 7;
	cas[WQSide].afRook.y = 4;
	cas[WQSide].bfKing.x = 7;
	cas[WQSide].bfKing.y = 3;
	cas[WQSide].afKing.x = 7;
	cas[WQSide].afKing.y = 5;
	cas[WQSide].bet[0].x = 7;
	cas[WQSide].bet[0].y = 4;
	cas[WQSide].bet[1].x = 7;
	cas[WQSide].bet[1].y = 5;

	//黒キングサイド
	cas[BKSide].bfRook.x = 0;
	cas[BKSide].bfRook.y = 0;
	cas[BKSide].afRook.x = 0;
	cas[BKSide].afRook.y = 2;
	cas[BKSide].bfKing.x = 0;
	cas[BKSide].bfKing.y = 3;
	cas[BKSide].afKing.x = 0;
	cas[BKSide].afKing.y = 1;
	cas[BKSide].bet[0].x = 0;
	cas[BKSide].bet[0].y = 1;
	cas[BKSide].bet[1].x = 0;
	cas[BKSide].bet[1].y = 2;

	//黒クイーンサイド
	cas[BQSide].bfRook.x = 0;
	cas[BQSide].bfRook.y = 7;
	cas[BQSide].afRook.x = 0;
	cas[BQSide].afRook.y = 4;
	cas[BQSide].bfKing.x = 0;
	cas[BQSide].bfKing.y = 3;
	cas[BQSide].afKing.x = 0;
	cas[BQSide].afKing.y = 5;
	cas[BQSide].bet[0].x = 0;
	cas[BQSide].bet[0].y = 4;
	cas[BQSide].bet[1].x = 0;
	cas[BQSide].bet[1].y = 5;

	//アンパッサン管理構造体初期化
	enp.flag = false;

	//キングの初期位置
	m_BKingX = 0;
	m_BKingY = 3;
	m_WKingX = 7;
	m_WKingY = 3;
}

//駒が動かせる場所をゲーム的に考える
void ChessGame::CheckMove(int x, int y){

	ChessPiece pi = m_bo->GetPiece(x,y);

	//各駒に動ける場所を設定してもらう（各駒クラスにok_piece/ngを入れてもらう）
	switch(pi){
		case BKing:{
		case WKing:
			King king(m_bo);
			king.MovePoint(x,y,m_player);
			break;}
		case BQueen:{
		case WQueen:
			Queen queen(m_bo);
			queen.MovePoint(x,y,m_player);
			break;}
		case BRook:{
		case WRook:
			Rook rook(m_bo);
			rook.MovePoint(x,y,m_player);
			break;}
		case BBishop:{
		case WBishop:
			Bishop bishop(m_bo);
			bishop.MovePoint(x,y,m_player);
			break;}
		case BKnight:{
		case WKnight:
			Knight knight(m_bo);
			knight.MovePoint(x,y,m_player);
			break;}
		case BPawn:{
		case WPawn:
			Pawn pawn(m_bo);
			pawn.MovePoint(x,y,m_player);
			if( enp.flag )
				for(int i=0; i<2; ++i)
					if( enp.takePawn[i].x == x && enp.takePawn[i].y == y )
						m_bo->SetOK(enp.movePawn.x+enp.takeXdiff,enp.movePawn.y);		//アンパッサンフラグONでそのポーンを選択しているならアンパッサン後に動ける場所をOKフラグに入れる
			break;}
		default:
			;
	}

	//キャスリング等特殊処理
	if( (((pi == BKing || pi == WKing ) && m_bo->CheckPieceInitP(x,y))) && !GetCheckP(x,y) ){	//キングが初期位置から動いてなければ＆キングがチェックされていなければ
		CheckCastling();			//キャスリングチェック;
		int seek[2];
		if( m_player == White ){		//手番が白なら
			seek[0] = (int)WKSide;
			seek[1] = (int)WQSide;
		}else{
			seek[0] = (int)BKSide;
			seek[1] = (int)BQSide;
		}
		for(int i=0; i<2; ++i){
			if( cas[seek[i]].flag ){
				m_bo->SetOK(cas[seek[i]].afKing.x,cas[seek[i]].afKing.y);
			}
		}
	}
	//この時点での未決定部分をNGに変えておく
	m_bo->ChangeYetNG();
	
	//チェックによる移動不可判定（check&checknow）
	std::vector<Point> *okp = m_bo->GetOKpoint();
	if( okp->empty() )
		return;					//Isel地点がないならばこの後の処理は不要

	int kingX, kingY;
	if( m_player == White ){
		kingX = m_WKingX;
		kingY = m_WKingY;
	}else{
		kingX = m_BKingX;
		kingY = m_BKingY;
	}

	GameBoard tempBo = m_bo;
	tempBo.SetEmpty(x,y);								//もしこの駒を動かした場合

	if( GetCheckP(kingX,kingY) || GetCheckP(kingX,kingY,&tempBo) ){		//キングがチェック中なら or 駒を動かした時キングがチェックになるなら
		
		//各Isel地点について、敵駒を防げる位置か確認
		std::vector<Point>::iterator okpit = okp->begin();
		if( pi == BKing || pi == WKing ){				//選択した駒がキングであるかどうか
			while( okpit != okp->end() ){
				GameBoard tempB = m_bo;
				tempB.MovePiece(okpit->x,okpit->y,x,y);		//キングをIsel地点に動かしてみる
				if( GetCheckP(okpit->x,okpit->y,&tempB) )	//それでもなおキングがチェックされるなら
					m_bo->SetCheck(okpit->x,okpit->y);
				okpit++;
			}
		}else{
			while( okpit != okp->end() ){
				GameBoard tempB = m_bo;
				tempB.MovePiece(okpit->x,okpit->y,x,y);		//駒をIsel地点に動かしてみる
				if( GetCheckP(kingX,kingY,&tempB) )			//それでもなおキングがチェックされるなら
					m_bo->SetCheck(okpit->x,okpit->y);
				okpit++;
			}
		}
	}else if( pi == BKing || pi == WKing ){								//選択した駒がキングなら
		for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); okpit++){		//動ける場所に敵のコマ筋がかかってるかを確認
			GameBoard tempK = m_bo;
			tempK.MovePiece(okpit->x,okpit->y,x,y);
			if( GetCheckP(okpit->x,okpit->y,&tempK) )
				m_bo->SetCheck(okpit->x,okpit->y);
		}
	}
}

//駒の移動
void ChessGame::Move(int afterX, int afterY, int beforeX, int beforeY){

	ChessPiece pi = m_bo->GetPiece(beforeX,beforeY);
	if( pi == BKing ){
		m_BKingX = afterX;
		m_BKingY = afterY;
	}else if( pi == WKing ){
		m_WKingX = afterX;
		m_WKingY = afterY;
	}
	m_bo->MovePiece(afterX, afterY, beforeX, beforeY);
	
	//キャスリング処理
	if( pi == BKing || pi == WKing ){
		int seek[2];
		if( m_player == White ){		//手番が白なら
			seek[0] = (int)WKSide;
			seek[1] = (int)WQSide;
		}else{
			seek[0] = (int)BKSide;
			seek[1] = (int)BQSide;
		}
		for(int i=0; i<2; ++i){
			if( cas[seek[i]].flag &&(afterX == cas[seek[i]].afKing.x && afterY == cas[seek[i]].afKing.y) ){		//キャスリングフラグが立っているかどうかとキャスリングする位置にキングが動いたかどうか
				m_bo->MovePiece(cas[seek[i]].afRook.x, cas[seek[i]].afRook.y, cas[seek[i]].bfRook.x, cas[seek[i]].bfRook.y);	//上でキングは動くのでルークのみ動かす
				cas[seek[i]].flag = false;
				for(int j=0; j<2; ++j)
					cas[seek[j]].oneTime = true;
				break;
			}
		}
	}

	//プロモーション処理
	if( pi == WPawn && afterX == 0){
		m_bo->Promotion(afterY,White);
	}else if( pi == BPawn && afterX == 7 ){
		m_bo->Promotion(afterY,Black);
	}

	//アンパッサン処理
	if( enp.flag ){
		enp.flag = false;			//アンパッサンの権利は1ターンで終了
		if( enp.movePawn.x+enp.takeXdiff == afterX && enp.movePawn.y == afterY )		//アンパッサンの位置に動いたなら
			m_bo->SetEmpty(enp.movePawn.x,enp.movePawn.y);				//取られるポーンを消す
	}
	if( (pi == WPawn || pi == BPawn) && abs(afterX-beforeX) == 2 ){	//ポーンを動かした時でXが2マス動いた時
		Player enemyP = RetEP();
		int X = (m_player == White ? 4 : 3);	//敵が黒ならX=4の位置、白ならX=3の位置にポーンがないとだめ
		for(int Y=0; Y<BNUM; ++Y){
			if( m_bo->GetPiece(X,Y) == (m_player == White ? BPawn : WPawn) && (Y == afterY-1 || Y == afterY+1) ){	//ポーンがあるなら
				enp.flag = true;		//アンパッサンフラグON
				enp.takeXdiff = (enemyP == White ? -1 : 1);			//takePawnが白なら-1,黒なら+1
				enp.movePawn.x = afterX;
				enp.movePawn.y = afterY;
				enp.takePawn[0].x = X;
				enp.takePawn[0].y = Y;
				int NewY = Y+2;			//敵ポーンを挟んだ向こう側にも自分のポーンがあるかどうか
				if( Y != afterY+1 && (NewY < BNUM && m_bo->GetPiece(X,NewY) == (m_player == White ? BPawn : WPawn))){	//Y==afterY+1で入ってきたなら向こう側を見る必要はない
					enp.takePawn[1].x = X;
					enp.takePawn[1].y = NewY;
				}else{
					enp.takePawn[1].x = -1;
					enp.takePawn[1].y = -1;
				}
				break;
			}
		}
	}
	
	//プレイヤーの交代
	GameTurn tempTurn = CheckEnd();		//ゲーム終了判断
	if( tempTurn != Continue ){
		m_player = End;
		m_turn = tempTurn;
		isChecking = false;
	}else if( m_player == White ){
		m_player = Black;
		m_turn = BlackTurn;
	}else{
		m_player = White;
		m_turn = WhiteTurn;
	}
}

//キャスリングチェック（ルークが動いてないか・間が空いているか・キング移動位置に敵駒が効いていないか）
void ChessGame::CheckCastling(void){
	
	int seek[2];
	if( m_player == White ){		//手番が白なら
		seek[0] = (int)WKSide;
		seek[1] = (int)WQSide;
	}else{
		seek[0] = (int)BKSide;
		seek[1] = (int)BQSide;
	}
	for(int i=0; i<2; ++i){
		if( cas[seek[i]].oneTime )
			return;

		if( (m_bo->CheckPieceInitP(cas[seek[i]].bfRook.x, cas[seek[i]].bfRook.y)) ){	//ルークが動いてないか		
			//キング - ルーク間が全部空いてるか
			int X = cas[seek[i]].bfKing.x;
			int max = cas[seek[i]].bfKing.y<cas[seek[i]].bfRook.y ? cas[seek[i]].bfRook.y : cas[seek[i]].bfKing.y;
			int min = cas[seek[i]].bfKing.y>cas[seek[i]].bfRook.y ? cas[seek[i]].bfRook.y : cas[seek[i]].bfKing.y;
			//int count = 0;
			for(int j=min+1; j<max; ++j)
				if( m_bo->GetPiece(X,j) != empty )
					break;
				else if( j == max-1)
					if(!GetCheckP(cas[seek[i]].bet[0].x, cas[seek[i]].bet[0].y) && !GetCheckP(cas[seek[i]].bet[1].x, cas[seek[i]].bet[1].y))	//キング移動位置に敵駒が効いていないか
						cas[seek[i]].flag = true;

		}
	}
}

//ある地点に対して、敵の駒筋が効いてるかを確認する
bool ChessGame::GetCheckP(int x, int y){

	Player enemyP = RetEP();	//敵プレイヤー

	GameBoard tempBo = m_bo;			//一時的な盤情報を得る

	//全盤上の敵駒に対しIselを確認
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){
			if( tempBo.GetPiecePlayer(i,j) != enemyP )	//敵駒ではないならcontinue
				continue;

			CheckMove(i,j,enemyP,&tempBo);
			//tempBo.Print();
			//getchar();
			if( tempBo.GetIsel(x,y) ){
				//checkEPX = i;
				//checkEPY = j;
				tempBo.ClearFlag();
				return true;
			}else{
				tempBo.ClearFlag();
			}
		}
	}

	return false;
}

//GetCheck()の一時的な盤情報指定版
bool ChessGame::GetCheckP(int x, int y, GameBoard *tempBo){

	Player enemyP = RetEP();	//敵プレイヤー
	//std::cout << "x:" << x << " y:" << y << std::endl;
	//全盤上の敵駒に対しIselを確認
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){
			if( tempBo->GetPiecePlayer(i,j) != enemyP )	//敵駒ではないならcontinue
				continue;

			CheckMove(i,j,enemyP,tempBo);
			//tempBo->Print();
			//getchar();
			if( tempBo->GetIsel(x,y) ){
				//std::cout << "PI:" << tempBo->GetPiece(i,j)  << " EPX:" << i << " EPY:" << j << std::endl;
				//tempBo->Print();
				tempBo->ClearFlag();
				return true;
			}else{
				tempBo->ClearFlag();
			}
		}
	}

	return false;
}

//GetCheckP()のプレイヤー指定版
bool ChessGame::GetCheckP(int x, int y, GameBoard *tempBo, Player p){

	//全盤上の敵駒に対しIselを確認
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){
			if( tempBo->GetPiecePlayer(i,j) != p )	//プレイヤーの駒ではないならcontinue
				continue;

			CheckMove(i,j,p,tempBo);
			if( tempBo->GetIsel(x,y) ){
				tempBo->ClearFlag();
				return true;
			}else{
				tempBo->ClearFlag();
			}
		}
	}

	return false;
}

//駒が動かせる場所をそのプレイヤーとして考える
void ChessGame::CheckMove(int x, int y, Player player, GameBoard *b){

	ChessPiece pi = b->GetPiece(x,y);

	//各駒に動ける場所を設定してもらう（各駒クラスにok_piece/ngを入れてもらう）
	switch(pi){
		case BKing:{
		case WKing:
			King king(b);
			king.MovePoint(x,y,player);
			break;}
		case BQueen:{
		case WQueen:
			Queen queen(b);
			queen.MovePoint(x,y,player);
			break;}
		case BRook:{
		case WRook:
			Rook rook(b);
			rook.MovePoint(x,y,player);
			break;}
		case BBishop:{
		case WBishop:
			Bishop bishop(b);
			bishop.MovePoint(x,y,player);
			break;}
		case BKnight:{
		case WKnight:
			Knight knight(b);
			knight.MovePoint(x,y,player);
			break;}
		case BPawn:{
		case WPawn:
			Pawn pawn(b);
			pawn.ActPoint(x,y,player);		//ポーンは駒が取れる場所のみ
			break;}
		default:
			;
	}
	b->ChangeOK();
}

//ゲーム終了判定
GameTurn ChessGame::CheckEnd(void){
	
	Player enemyP = RetEP();	//敵プレイヤー 
	int e_kingX, e_kingY;		//敵のキング位置
	if( m_player == White ){
		e_kingX = m_BKingX;
		e_kingY = m_BKingY;
	}else{
		e_kingX = m_WKingX;
		e_kingY = m_WKingY;
	}
	GameBoard tempCE = m_bo;	//一時的な盤情報
	if(isChecking)
		isChecking = false;		//チェックは常に次の手で解消される

	//相手のキングがチェック中でなければステイルメイト判断（プレイヤーの交代後を想定し相手の駒が自分によってステイルメイトされたかを判断）
	if( !GetCheckP(e_kingX,e_kingY,&tempCE,m_player) ){
		if( CheckMoveALL(enemyP) )	//相手の駒を全探索する
			return Continue;
		else
			return EndST;		//全探索しても動けない場合ステイルメイト判定
	
	}else{
		isChecking = true;
	}

	//相手のキングが動ける範囲を調べる
	King king(&tempCE);
	king.MovePoint(e_kingX,e_kingY,enemyP);
	tempCE.ChangeYetNG();
	std::vector<Point> *okp = tempCE.GetOKpoint();
	for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); okpit++){	//キングの動ける範囲をすべて調べる
		GameBoard temp = m_bo;
		temp.MovePiece(okpit->x,okpit->y,e_kingX,e_kingY);
		if( !GetCheckP(okpit->x,okpit->y,&temp,m_player) ){		//キングに動ける場所が存在するなら
			//std::cout << "キングまだ動けるよ" << std::endl;
			return Continue;									//ゲーム続行
		}
	}

	//相手の駒を全探索
	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			if( m_bo->GetPiecePlayer(i,j) != enemyP )			//相手の駒でないならcontinue
				continue;

			GameBoard tempCEA = m_bo;
			switch(tempCEA.GetPiece(i,j)){
				case BKing:
				case WKing:
					continue;							//キングは既に調べてある
				case BQueen:{
				case WQueen:
					Queen queen(&tempCEA);
					queen.MovePoint(i,j,enemyP);
					break;}
				case BRook:{
				case WRook:
					Rook rook(&tempCEA);
					rook.MovePoint(i,j,enemyP);
					break;}
				case BBishop:{
				case WBishop:
					Bishop bishop(&tempCEA);
					bishop.MovePoint(i,j,enemyP);
					break;}
				case BKnight:{
				case WKnight:
					Knight knight(&tempCEA);
					knight.MovePoint(i,j,enemyP);
					break;}
				case BPawn:{
				case WPawn:
					Pawn pawn(&tempCEA);
					pawn.ActPoint(i,j,enemyP);
					break;}
			}
			tempCEA.ChangeYetNG();
			std::vector<Point> *okp = tempCEA.GetOKpoint();
			if( okp->empty() )
				continue;

			for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); okpit++){
				GameBoard tempB = m_bo;
				tempB.MovePiece(okpit->x,okpit->y,i,j);						//駒をIsel地点に動かしてみる
				if( !GetCheckP(e_kingX,e_kingY,&tempB,m_player) ){			//それでキングのチェックが解消されるなら
					//std::cout << "EPX:" << i << " EPY:" << j << std::endl;
					return Continue;
				}
			}
		}
	}
	
	//ここまでContinueが返されないならゲーム終了
	return (m_player == White ? EndW : EndB );
}

bool ChessGame::CheckMoveALL(Player player){

	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			if( m_bo->GetPiecePlayer(i,j) != player )		//プレイヤーの駒でなければcontinue
				continue;

			GameBoard temp = m_bo;
			ChessPiece pi = temp.GetPiece(i,j);
			switch(pi){
				case BKing:{
				case WKing:
					King king(&temp);
					king.MovePoint(i,j,player);
					break;}
				case BQueen:{
				case WQueen:
					Queen queen(&temp);
					queen.MovePoint(i,j,player);
					break;}
				case BRook:{
				case WRook:
					Rook rook(&temp);
					rook.MovePoint(i,j,player);
					break;}
				case BBishop:{
				case WBishop:
					Bishop bishop(&temp);
					bishop.MovePoint(i,j,player);
					break;}
				case BKnight:{
				case WKnight:
					Knight knight(&temp);
					knight.MovePoint(i,j,player);
					break;}
				case BPawn:{
				case WPawn:
					Pawn pawn(&temp);
					pawn.MovePoint(i,j,player);
					break;}
			}
			//この時点での未決定部分をNGに変えておく
			temp.ChangeYetNG();

			//チェックによる移動不可判定（check&checknow）
			std::vector<Point> *okp =temp.GetOKpoint();
			if( okp->empty() )
				continue;

			int kingX, kingY;
			if( player == White ){
				kingX = m_WKingX;
				kingY = m_WKingY;
			}else{
				kingX = m_BKingX;
				kingY = m_BKingY;
			}
			GameBoard tempBo = temp;
			tempBo.SetEmpty(i,j);								//もしこの駒を動かした場合
			Player enemyP = (player == White ? Black : White);

			if( GetCheckP(kingX,kingY,&temp,enemyP) || GetCheckP(kingX,kingY,&tempBo,enemyP) ){		//キングがチェック中なら or 駒を動かした時キングがチェックになるなら

				//各Isel地点について、敵駒を防げる位置か確認
				std::vector<Point>::iterator okpit = okp->begin();
				if( pi == BKing || pi == WKing ){				//選択した駒がキングであるかどうか
					while( okpit != okp->end() ){
						GameBoard tempB = temp;
						tempB.MovePiece(okpit->x,okpit->y,i,j);		//キングをIsel地点に動かしてみる
						if( GetCheckP(okpit->x,okpit->y,&tempB,enemyP) )	//それでもなおキングがチェックされるなら
							okpit = okp->erase(okpit);			//要素を削除
						else
							okpit++;							//イテレータを更新するので削除しないときだけインクリメント
					}
				}else{
					while( okpit != okp->end() ){
						GameBoard tempB = temp;
						tempB.MovePiece(okpit->x,okpit->y,i,j);		//駒をIsel地点に動かしてみる
						if( GetCheckP(kingX,kingY,&tempB,enemyP) )			//それでもなおキングがチェックされるなら
							okpit = okp->erase(okpit);
						else
							okpit++;
					}
				}
			}else if( pi == BKing || pi == WKing ){								//選択した駒がキングなら
				for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); /*ここでインクリメントしない*/){		//動ける場所に敵のコマ筋がかかってるかを確認
					GameBoard tempK = temp;
					tempK.MovePiece(okpit->x,okpit->y,i,j);
					if( GetCheckP(okpit->x,okpit->y,&tempK,enemyP) )
						okpit = okp->erase(okpit);
					else
						okpit++;
				}
			}
			if( !okp->empty() )		//okpが空でないなら
				return true;	//動かせる場所がまだある
		}
	}
	//全探索しても動かせる場所が見つからないなら
	return false;
}