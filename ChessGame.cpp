/*---------------------------------------
	ChessGame.cpp
	�`�F�X�Q�[���Ǘ��N���X
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
-----------------------------------------*/

#include <iostream>
#include <vector>
#include "ChessGame.h"

//�R���X�g���N�^
ChessGame::ChessGame(GameBoard *p){

	isChecking = false;
	m_bo = p;
	m_player = White;
	for(int i=0; i<4; ++i){
		cas[i].flag = false;		//�L���X�����O�t���O�N���A
		cas[i].oneTime = false;
	}

	//���L���O�T�C�h
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
	
	//���N�C�[���T�C�h
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

	//���L���O�T�C�h
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

	//���N�C�[���T�C�h
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

	//�A���p�b�T���Ǘ��\���̏�����
	enp.flag = false;

	//�L���O�̏����ʒu
	m_BKingX = 0;
	m_BKingY = 3;
	m_WKingX = 7;
	m_WKingY = 3;
}

//���������ꏊ���Q�[���I�ɍl����
void ChessGame::CheckMove(int x, int y){

	ChessPiece pi = m_bo->GetPiece(x,y);

	//�e��ɓ�����ꏊ��ݒ肵�Ă��炤�i�e��N���X��ok_piece/ng�����Ă��炤�j
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
						m_bo->SetOK(enp.movePawn.x+enp.takeXdiff,enp.movePawn.y);		//�A���p�b�T���t���OON�ł��̃|�[����I�����Ă���Ȃ�A���p�b�T����ɓ�����ꏊ��OK�t���O�ɓ����
			break;}
		default:
			;
	}

	//�L���X�����O�����ꏈ��
	if( (((pi == BKing || pi == WKing ) && m_bo->CheckPieceInitP(x,y))) && !GetCheckP(x,y) ){	//�L���O�������ʒu���瓮���ĂȂ���΁��L���O���`�F�b�N����Ă��Ȃ����
		CheckCastling();			//�L���X�����O�`�F�b�N;
		int seek[2];
		if( m_player == White ){		//��Ԃ����Ȃ�
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
	//���̎��_�ł̖����蕔����NG�ɕς��Ă���
	m_bo->ChangeYetNG();
	
	//�`�F�b�N�ɂ��ړ��s����icheck&checknow�j
	std::vector<Point> *okp = m_bo->GetOKpoint();
	if( okp->empty() )
		return;					//Isel�n�_���Ȃ��Ȃ�΂��̌�̏����͕s�v

	int kingX, kingY;
	if( m_player == White ){
		kingX = m_WKingX;
		kingY = m_WKingY;
	}else{
		kingX = m_BKingX;
		kingY = m_BKingY;
	}

	GameBoard tempBo = m_bo;
	tempBo.SetEmpty(x,y);								//�������̋�𓮂������ꍇ

	if( GetCheckP(kingX,kingY) || GetCheckP(kingX,kingY,&tempBo) ){		//�L���O���`�F�b�N���Ȃ� or ��𓮂��������L���O���`�F�b�N�ɂȂ�Ȃ�
		
		//�eIsel�n�_�ɂ��āA�G���h����ʒu���m�F
		std::vector<Point>::iterator okpit = okp->begin();
		if( pi == BKing || pi == WKing ){				//�I��������L���O�ł��邩�ǂ���
			while( okpit != okp->end() ){
				GameBoard tempB = m_bo;
				tempB.MovePiece(okpit->x,okpit->y,x,y);		//�L���O��Isel�n�_�ɓ������Ă݂�
				if( GetCheckP(okpit->x,okpit->y,&tempB) )	//����ł��Ȃ��L���O���`�F�b�N�����Ȃ�
					m_bo->SetCheck(okpit->x,okpit->y);
				okpit++;
			}
		}else{
			while( okpit != okp->end() ){
				GameBoard tempB = m_bo;
				tempB.MovePiece(okpit->x,okpit->y,x,y);		//���Isel�n�_�ɓ������Ă݂�
				if( GetCheckP(kingX,kingY,&tempB) )			//����ł��Ȃ��L���O���`�F�b�N�����Ȃ�
					m_bo->SetCheck(okpit->x,okpit->y);
				okpit++;
			}
		}
	}else if( pi == BKing || pi == WKing ){								//�I��������L���O�Ȃ�
		for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); okpit++){		//������ꏊ�ɓG�̃R�}�؂��������Ă邩���m�F
			GameBoard tempK = m_bo;
			tempK.MovePiece(okpit->x,okpit->y,x,y);
			if( GetCheckP(okpit->x,okpit->y,&tempK) )
				m_bo->SetCheck(okpit->x,okpit->y);
		}
	}
}

//��̈ړ�
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
	
	//�L���X�����O����
	if( pi == BKing || pi == WKing ){
		int seek[2];
		if( m_player == White ){		//��Ԃ����Ȃ�
			seek[0] = (int)WKSide;
			seek[1] = (int)WQSide;
		}else{
			seek[0] = (int)BKSide;
			seek[1] = (int)BQSide;
		}
		for(int i=0; i<2; ++i){
			if( cas[seek[i]].flag &&(afterX == cas[seek[i]].afKing.x && afterY == cas[seek[i]].afKing.y) ){		//�L���X�����O�t���O�������Ă��邩�ǂ����ƃL���X�����O����ʒu�ɃL���O�����������ǂ���
				m_bo->MovePiece(cas[seek[i]].afRook.x, cas[seek[i]].afRook.y, cas[seek[i]].bfRook.x, cas[seek[i]].bfRook.y);	//��ŃL���O�͓����̂Ń��[�N�̂ݓ�����
				cas[seek[i]].flag = false;
				for(int j=0; j<2; ++j)
					cas[seek[j]].oneTime = true;
				break;
			}
		}
	}

	//�v�����[�V��������
	if( pi == WPawn && afterX == 0){
		m_bo->Promotion(afterY,White);
	}else if( pi == BPawn && afterX == 7 ){
		m_bo->Promotion(afterY,Black);
	}

	//�A���p�b�T������
	if( enp.flag ){
		enp.flag = false;			//�A���p�b�T���̌�����1�^�[���ŏI��
		if( enp.movePawn.x+enp.takeXdiff == afterX && enp.movePawn.y == afterY )		//�A���p�b�T���̈ʒu�ɓ������Ȃ�
			m_bo->SetEmpty(enp.movePawn.x,enp.movePawn.y);				//�����|�[��������
	}
	if( (pi == WPawn || pi == BPawn) && abs(afterX-beforeX) == 2 ){	//�|�[���𓮂���������X��2�}�X��������
		Player enemyP = RetEP();
		int X = (m_player == White ? 4 : 3);	//�G�����Ȃ�X=4�̈ʒu�A���Ȃ�X=3�̈ʒu�Ƀ|�[�����Ȃ��Ƃ���
		for(int Y=0; Y<BNUM; ++Y){
			if( m_bo->GetPiece(X,Y) == (m_player == White ? BPawn : WPawn) && (Y == afterY-1 || Y == afterY+1) ){	//�|�[��������Ȃ�
				enp.flag = true;		//�A���p�b�T���t���OON
				enp.takeXdiff = (enemyP == White ? -1 : 1);			//takePawn�����Ȃ�-1,���Ȃ�+1
				enp.movePawn.x = afterX;
				enp.movePawn.y = afterY;
				enp.takePawn[0].x = X;
				enp.takePawn[0].y = Y;
				int NewY = Y+2;			//�G�|�[�������񂾌��������ɂ������̃|�[�������邩�ǂ���
				if( Y != afterY+1 && (NewY < BNUM && m_bo->GetPiece(X,NewY) == (m_player == White ? BPawn : WPawn))){	//Y==afterY+1�œ����Ă����Ȃ��������������K�v�͂Ȃ�
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
	
	//�v���C���[�̌��
	GameTurn tempTurn = CheckEnd();		//�Q�[���I�����f
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

//�L���X�����O�`�F�b�N�i���[�N�������ĂȂ����E�Ԃ��󂢂Ă��邩�E�L���O�ړ��ʒu�ɓG������Ă��Ȃ����j
void ChessGame::CheckCastling(void){
	
	int seek[2];
	if( m_player == White ){		//��Ԃ����Ȃ�
		seek[0] = (int)WKSide;
		seek[1] = (int)WQSide;
	}else{
		seek[0] = (int)BKSide;
		seek[1] = (int)BQSide;
	}
	for(int i=0; i<2; ++i){
		if( cas[seek[i]].oneTime )
			return;

		if( (m_bo->CheckPieceInitP(cas[seek[i]].bfRook.x, cas[seek[i]].bfRook.y)) ){	//���[�N�������ĂȂ���		
			//�L���O - ���[�N�Ԃ��S���󂢂Ă邩
			int X = cas[seek[i]].bfKing.x;
			int max = cas[seek[i]].bfKing.y<cas[seek[i]].bfRook.y ? cas[seek[i]].bfRook.y : cas[seek[i]].bfKing.y;
			int min = cas[seek[i]].bfKing.y>cas[seek[i]].bfRook.y ? cas[seek[i]].bfRook.y : cas[seek[i]].bfKing.y;
			//int count = 0;
			for(int j=min+1; j<max; ++j)
				if( m_bo->GetPiece(X,j) != empty )
					break;
				else if( j == max-1)
					if(!GetCheckP(cas[seek[i]].bet[0].x, cas[seek[i]].bet[0].y) && !GetCheckP(cas[seek[i]].bet[1].x, cas[seek[i]].bet[1].y))	//�L���O�ړ��ʒu�ɓG������Ă��Ȃ���
						cas[seek[i]].flag = true;

		}
	}
}

//����n�_�ɑ΂��āA�G�̋�؂������Ă邩���m�F����
bool ChessGame::GetCheckP(int x, int y){

	Player enemyP = RetEP();	//�G�v���C���[

	GameBoard tempBo = m_bo;			//�ꎞ�I�ȔՏ��𓾂�

	//�S�Տ�̓G��ɑ΂�Isel���m�F
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){
			if( tempBo.GetPiecePlayer(i,j) != enemyP )	//�G��ł͂Ȃ��Ȃ�continue
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

//GetCheck()�̈ꎞ�I�ȔՏ��w���
bool ChessGame::GetCheckP(int x, int y, GameBoard *tempBo){

	Player enemyP = RetEP();	//�G�v���C���[
	//std::cout << "x:" << x << " y:" << y << std::endl;
	//�S�Տ�̓G��ɑ΂�Isel���m�F
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){
			if( tempBo->GetPiecePlayer(i,j) != enemyP )	//�G��ł͂Ȃ��Ȃ�continue
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

//GetCheckP()�̃v���C���[�w���
bool ChessGame::GetCheckP(int x, int y, GameBoard *tempBo, Player p){

	//�S�Տ�̓G��ɑ΂�Isel���m�F
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){
			if( tempBo->GetPiecePlayer(i,j) != p )	//�v���C���[�̋�ł͂Ȃ��Ȃ�continue
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

//���������ꏊ�����̃v���C���[�Ƃ��čl����
void ChessGame::CheckMove(int x, int y, Player player, GameBoard *b){

	ChessPiece pi = b->GetPiece(x,y);

	//�e��ɓ�����ꏊ��ݒ肵�Ă��炤�i�e��N���X��ok_piece/ng�����Ă��炤�j
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
			pawn.ActPoint(x,y,player);		//�|�[���͋����ꏊ�̂�
			break;}
		default:
			;
	}
	b->ChangeOK();
}

//�Q�[���I������
GameTurn ChessGame::CheckEnd(void){
	
	Player enemyP = RetEP();	//�G�v���C���[ 
	int e_kingX, e_kingY;		//�G�̃L���O�ʒu
	if( m_player == White ){
		e_kingX = m_BKingX;
		e_kingY = m_BKingY;
	}else{
		e_kingX = m_WKingX;
		e_kingY = m_WKingY;
	}
	GameBoard tempCE = m_bo;	//�ꎞ�I�ȔՏ��
	if(isChecking)
		isChecking = false;		//�`�F�b�N�͏�Ɏ��̎�ŉ��������

	//����̃L���O���`�F�b�N���łȂ���΃X�e�C�����C�g���f�i�v���C���[�̌����z�肵����̋�����ɂ���ăX�e�C�����C�g���ꂽ���𔻒f�j
	if( !GetCheckP(e_kingX,e_kingY,&tempCE,m_player) ){
		if( CheckMoveALL(enemyP) )	//����̋��S�T������
			return Continue;
		else
			return EndST;		//�S�T�����Ă������Ȃ��ꍇ�X�e�C�����C�g����
	
	}else{
		isChecking = true;
	}

	//����̃L���O��������͈͂𒲂ׂ�
	King king(&tempCE);
	king.MovePoint(e_kingX,e_kingY,enemyP);
	tempCE.ChangeYetNG();
	std::vector<Point> *okp = tempCE.GetOKpoint();
	for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); okpit++){	//�L���O�̓�����͈͂����ׂĒ��ׂ�
		GameBoard temp = m_bo;
		temp.MovePiece(okpit->x,okpit->y,e_kingX,e_kingY);
		if( !GetCheckP(okpit->x,okpit->y,&temp,m_player) ){		//�L���O�ɓ�����ꏊ�����݂���Ȃ�
			//std::cout << "�L���O�܂��������" << std::endl;
			return Continue;									//�Q�[�����s
		}
	}

	//����̋��S�T��
	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			if( m_bo->GetPiecePlayer(i,j) != enemyP )			//����̋�łȂ��Ȃ�continue
				continue;

			GameBoard tempCEA = m_bo;
			switch(tempCEA.GetPiece(i,j)){
				case BKing:
				case WKing:
					continue;							//�L���O�͊��ɒ��ׂĂ���
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
				tempB.MovePiece(okpit->x,okpit->y,i,j);						//���Isel�n�_�ɓ������Ă݂�
				if( !GetCheckP(e_kingX,e_kingY,&tempB,m_player) ){			//����ŃL���O�̃`�F�b�N�����������Ȃ�
					//std::cout << "EPX:" << i << " EPY:" << j << std::endl;
					return Continue;
				}
			}
		}
	}
	
	//�����܂�Continue���Ԃ���Ȃ��Ȃ�Q�[���I��
	return (m_player == White ? EndW : EndB );
}

bool ChessGame::CheckMoveALL(Player player){

	for(int i=0; i<BNUM; ++i){
		for(int j=0; j<BNUM; ++j){
			if( m_bo->GetPiecePlayer(i,j) != player )		//�v���C���[�̋�łȂ����continue
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
			//���̎��_�ł̖����蕔����NG�ɕς��Ă���
			temp.ChangeYetNG();

			//�`�F�b�N�ɂ��ړ��s����icheck&checknow�j
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
			tempBo.SetEmpty(i,j);								//�������̋�𓮂������ꍇ
			Player enemyP = (player == White ? Black : White);

			if( GetCheckP(kingX,kingY,&temp,enemyP) || GetCheckP(kingX,kingY,&tempBo,enemyP) ){		//�L���O���`�F�b�N���Ȃ� or ��𓮂��������L���O���`�F�b�N�ɂȂ�Ȃ�

				//�eIsel�n�_�ɂ��āA�G���h����ʒu���m�F
				std::vector<Point>::iterator okpit = okp->begin();
				if( pi == BKing || pi == WKing ){				//�I��������L���O�ł��邩�ǂ���
					while( okpit != okp->end() ){
						GameBoard tempB = temp;
						tempB.MovePiece(okpit->x,okpit->y,i,j);		//�L���O��Isel�n�_�ɓ������Ă݂�
						if( GetCheckP(okpit->x,okpit->y,&tempB,enemyP) )	//����ł��Ȃ��L���O���`�F�b�N�����Ȃ�
							okpit = okp->erase(okpit);			//�v�f���폜
						else
							okpit++;							//�C�e���[�^���X�V����̂ō폜���Ȃ��Ƃ������C���N�������g
					}
				}else{
					while( okpit != okp->end() ){
						GameBoard tempB = temp;
						tempB.MovePiece(okpit->x,okpit->y,i,j);		//���Isel�n�_�ɓ������Ă݂�
						if( GetCheckP(kingX,kingY,&tempB,enemyP) )			//����ł��Ȃ��L���O���`�F�b�N�����Ȃ�
							okpit = okp->erase(okpit);
						else
							okpit++;
					}
				}
			}else if( pi == BKing || pi == WKing ){								//�I��������L���O�Ȃ�
				for(std::vector<Point>::iterator okpit = okp->begin(); okpit != okp->end(); /*�����ŃC���N�������g���Ȃ�*/){		//������ꏊ�ɓG�̃R�}�؂��������Ă邩���m�F
					GameBoard tempK = temp;
					tempK.MovePiece(okpit->x,okpit->y,i,j);
					if( GetCheckP(okpit->x,okpit->y,&tempK,enemyP) )
						okpit = okp->erase(okpit);
					else
						okpit++;
				}
			}
			if( !okp->empty() )		//okp����łȂ��Ȃ�
				return true;	//��������ꏊ���܂�����
		}
	}
	//�S�T�����Ă���������ꏊ��������Ȃ��Ȃ�
	return false;
}