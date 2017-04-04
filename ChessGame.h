/*-------------------------------------
	ChessGame.h
	�`�F�X�Q�[���Ǘ��N���X�w�b�_
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
---------------------------------------*/

#pragma once

#include "Chess.h"
#include "GameBoard.h"
#include "PieceMove.h"

/*---�L���X�����O�Ǘ��\����---*/
typedef struct{
	bool flag;			//�L���X�����O���s���邩
	Point bfRook;		//�L���X�����O����O�̃��[�N���W
	Point afRook;		//�L���X�����O������̃��[�N���W
	Point bfKing;		//�L���X�����O����O�̃L���O���W
	Point afKing;		//�L���X�����O������̃L���O���W
	Point bet[2];		//���[�N�E�L���O�Ԃɂ�����W���L���O���ʂ���W
	bool oneTime;		//�L���X�����O�����ɍs���Ă��Ȃ���
}castling;

/*---�L���X�����O�Ǘ����ʎq---*/
enum CastlingSide{
	WKSide = 0,		//���L���O�T�C�h
	WQSide,			//���N�C�[���T�C�h
	BKSide,			//���L���O�T�C�h
	BQSide			//���N�C�[���T�C�h
};

/*---�A���p�b�T���Ǘ��\����---*/
typedef struct {
	bool flag;
	Point movePawn;					//�����|�[���ʒu
	Point takePawn[2];				//���|�[���ʒu�i����ł�ꏊ��2����j
	int takeXdiff;					//takePawn�������Ƃ��̍���
}EnPassant;

class ChessGame{
public:
	ChessGame(GameBoard *p);		//�R���X�g���N�^
	void CheckMove(int x, int y);	//���������ꏊ���Q�[���I�ɍl����
	void Move(int afterX, int afterY, int beforeX, int beforeY);	//��̈ړ�
	GameTurn CheckTurn(void){return m_turn;}	//�Q�[���󋵕\���i��ԁEEND���e�j��Ԃ�
	Player CheckPlayer(void){return m_player;}	//�����ǂ���̏o�Ԃ���Ԃ�
	bool CheckingTurn(void){return (isChecking ? true : false);}	//���`�F�b�N����Ă����Ԃ��ǂ�����Ԃ�

private:
	GameBoard *m_bo;				//�Տ��
	Player m_player;				//��ԁiWhite/Black�j
	castling cas[4];				//�L���X�����O�Ǘ��\���̔z��i�������ꂼ��̃N�C�[���E�L���O�T�C�h��4�j
	EnPassant enp;					//�A���p�b�T���Ǘ��\����
	int m_BKingX;					//�L���O�̈ʒu��ߑ����Ă���
	int m_BKingY;
	int m_WKingX;
	int m_WKingY;
	bool isChecking;				//���`�F�b�N����Ă����Ԃ��ǂ���
	GameTurn m_turn;				//���̎��

	/*---����J���\�b�h---*/
	void CheckCastling(void);		//�L���X�����O���s���邩�`�F�b�N���t���O�}��
	bool GetCheckP(int x, int y);	//����n�_�ɑ΂��āA�G�̋�؂������Ă邩���m�F����
	//x,y:�w�肷����W
	bool GetCheckP(int x, int y, GameBoard *tempBo); //GetCheck()�̈ꎞ�I�ȔՏ��w���
	bool GetCheckP(int x, int y, GameBoard *tempBo, Player p);	//GetCheckP()�̃v���C���[�w���
	void CheckMove(int x, int y, Player player, GameBoard *b);	//���������ꏊ�����̃v���C���[�Ƃ��čl����
	GameTurn CheckEnd(void);						//�Q�[���I�����f
	bool CheckMoveALL(Player player);							//���̃v���C���[�̋��������ꏊ��S�T������
	
	inline Player RetEP(void){return (m_player == White ? Black : White);}
};
