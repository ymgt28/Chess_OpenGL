/*-------------------------------------
	GameBoard.h
	�Q�[���ՊǗ��N���X�w�b�_
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
---------------------------------------*/

#pragma once

#include <vector>
#include "Chess.h"

/*---������邩�̎��ʎq---*/
enum CanMove{
	yet = -1,		//������
	ok = 0,			//������ꏊ
	ng,				//���X�����Ȃ��i��̌��݈ʒu�E�����̋����ꍇ�܂ށj
	check			//�`�F�b�N�̊֌W�œ����Ȃ�
};

/*---�Ղ̊e�}�X�����\����---*/
typedef struct{
	ChessPiece pi;					//���̃}�X�ɂ����
	bool Dsel;						//���ڃZ���N�g����Ă��邩�ǂ���(DirectSelected)
	bool Isel;						//�ԐړI�ɃZ���N�g����Ă��邩�ǂ���(IndirectSelected)
	CanMove cmove;					//�����邩�ǂ����̎���
	bool initP;						//�����ʒu���瓮�������ǂ���(false�œ�����)
}CBoard;

class GameBoard{
public:
	GameBoard();																		//�R���X�g���N�^
	GameBoard(const GameBoard *gb){this->Copy(gb);}
	void Copy(const GameBoard *gb);														//�R�s�[���\�b�h
	
	void InitBoardPiece(void);															//�Ղ̋�̏�����
	void ClearFlag(void);																//Isel,Dsel�̃N���A("false"��)
	void MovePiece(int afterX, int afterY, int beforeX, int beforeY);					//��̈ړ�
	void ChangeOK(void);																//OK�t���O����Isel->true�t���O�֕ύX
	void ChangeYetNG(void);																//Yet�t���O�����ׂ�NG�ɕύX
	void Promotion(int Y, Player player);												//�v�����[�V��������

	/*---Get�n---*/
	ChessPiece GetPiece(int x,int y){return cbo[x][y].pi;}								//���̈ʒu�̋���𓾂�
	bool GetDsel(int x, int y){return (cbo[x][y].Dsel ? true : false);}					//���̈ʒu��Dsel���𓾂�
	bool GetIsel(int x, int y){return (cbo[x][y].Isel ? true : false);}					//���̈ʒu��Isel���𓾂�
	Player GetPiecePlayer(int x, int y);												//���̈ʒu�ɂ����̃v���C���[���𓾂�(White/Black)
	bool CheckPieceInitP(int x, int y){return (cbo[x][y].initP ? true : false);}		//���̈ʒu�ɂ��������ʒu���瓮�������ǂ���
	bool GetOK(int x, int y){return (cbo[x][y].cmove == ok ? true : false);}			//���̈ʒu�ɂ���t���O��OK���ǂ���
	std::vector<Point>* GetOKpoint(void){return &OKpoint;}								//OK�t���O������|�C���g���i�[����vector���Q�Ɠn���ŕԂ�

	/*---Set�n---*/
	void SetDselTrue(int x, int y){	cbo[x][y].Dsel = true;}								//�w�肳�ꂽ�ꏊ��Dsel�t���O��true������
	void SetOK(int x, int y){		cbo[x][y].cmove = ok;}								//�w�肳�ꂽ�ꏊ��OK_piece���ʎq������
	void SetNG(int x, int y){		cbo[x][y].cmove = ng;}								//�w�肳�ꂽ�ꏊ��NG���ʎq������
	void SetCheck(int x, int y){	cbo[x][y].cmove = check;}							//�w�肳�ꂽ�ꏊ��check���ʎq������
	void SetEmpty(int x, int y){	cbo[x][y].pi = empty;}								//�w�肳�ꂽ�ꏊ�̋������

	GameBoard& GameBoard::operator=(const GameBoard *gb){this->Copy(gb); return *this;}	// "="�̃I�[�o�[���[�h�i�N���X�R�s�[�j

	//�f�o�b�O
	void Print(void);

private:
	CBoard cbo[BNUM][BNUM];																//�������ꂽ�Տ��
	std::vector<Point> OKpoint;															//OK�t���O������ʒu���i�[����vector
};