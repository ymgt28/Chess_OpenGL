/*-------------------------------------
	Chess.h
	�`�F�X�Q�[�����ʃw�b�_
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
---------------------------------------*/

#pragma once

//#define DEGUB_SelectHits	//SelectHits�̏������e������
//#define DEBUG_MINILOAD	//�f�o�b�O�p�Z�k���[�h

/*---���ʗp���ʎq---*/
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

/*---�������ʎq---*/
enum Player{
	Yet = -1,	//������
	White,		//��
	Black,		//��
	End,		//�I���
};

/*---��Ԏ��ʎq---*/
enum GameTurn{
	Continue = -1,	//�Q�[�����s
	WhiteTurn,		//��
	BlackTurn,		//��
	EndW,			//�Q�[���I��(White�̏���)
	EndB,			//�Q�[���I��(Black�̏���)
	EndST,			//�Q�[���I��(�X�e�C�����C�g)
};

/*---�e��Œ�ϐ�---*/
const int BNUM = 8;					//�Ղ̈�ӂ̐�
const int NOBJECTS = 64;			//�ՃI�u�W�F�N�g�̐�
const int MODELNUM = 6;
const int SELECTIONS = 100;			//�Z���N�V�����o�b�t�@�̃T�C�Y
const float BoardScale = 0.3f;		//�Ղ̗����̃|���S���X�P�[��
const float PieceScale = 0.15f;		//��̃X�P�[��

//*---�Ղ̊e�}�X���W�������\����---*/
typedef struct{
	int x;
	int y;
}Point;