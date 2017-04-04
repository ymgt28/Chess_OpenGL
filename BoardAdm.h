/*--------------------------------------
	Board.h
	�Օ\���N���X
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
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

/*---���ʗp���ʎq�i���f���f�[�^���[�h�p�j---*/
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
	BoardAdm();								//�R���X�g���N�^
	~BoardAdm();							//�f�X�g���N�^
	void Init();							//�������֐�
	void Draw(void);						//�S�̕`��֐�
	void SetCurPoint(int x, int y){curX = x; curY = y; mouse_state = true;}	//�}�E�X�𓖂Ă�ꂽ���W���󂯎��
	std::string GetTurn(void);					//�ǂ���̏o�Ԃ���Ԃ�
	//�߂�l: "��","��"
	bool GetChecking(void){return cgame->CheckingTurn();}

private:
	GameBoard *bo;							//�`�F�X�Ղ�\������C���X�^���X
	ChessGame *cgame;						//�`�F�X�Q�[�����Ǘ�����C���X�^���X
	XModel model[MODELNUM];					//���f���\���p�i��12���j
	GLuint objects;							//�f�B�X�v���C���X�g�̎��ʎq
	GLfloat *color[NOBJECTS];				//�e�I�u�W�F�N�g�̐F
	int curX;								//���݃}�E�X�����Ă��Ă�����W
	int curY;
	bool mouse_state;						//�}�E�X�����݃N���b�N����Ă邩�ǂ���
	GLuint selection[SELECTIONS];			//�Z���N�V�����o�b�t�@
	GLint hits;								//�Z���N�g���ꂽ�I�u�W�F�N�g�̐�

	int m_select;							//�Z���N�g����Ă���}�X�̃I�u�W�F�N�g�ԍ�

	/*---����J���\�b�h---*/
	void DrawBoard(void);					//�Օ\���֐�
	void DrawBoardPoint(void);				//�Ղ̃}�E�X�𓖂Ă�ꂽ�ꏊ�������\��
	//����:�}�E�X��XY���W

	void DrawPiece(void);					//��\���֐�

	void BoardInit(void);					//�Ղ̏������֐�
	void PieceInit(void);					//��̏������֐�
	void SelectPoint(void);					//�}�E�X���������Ă���ꏊ�𔻒�
	int SelectHits(void);					//�q�b�g�����I�u�W�F�N�g������
	//�߂�l:���ʂ����I�u�W�F�N�g�ԍ� or -1
	void SelectDo(int hit_name);			//���ʂ����I�u�W�F�N�g�ɑ΂��铮��
	//����:���ʂ����I�u�W�F�N�g�ԍ�
	
	ChessPieceModel RetModelNo(ChessPiece pi);	// ChessPiece��ChessPieceModel�̕ϊ�

	inline int ReturnX(int a){return a/BNUM;}	//�I�u�W�F�N�g���ʎq����X���W�֕ϊ�
	inline int ReturnY(int a){return a%BNUM;}	//�I�u�W�F�N�g���ʎq����Y���W�֕ϊ�
};
