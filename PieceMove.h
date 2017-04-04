/*-------------------------------------
	PieceMove.h
	�`�F�X��̓����e�N���X�w�b�_
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
---------------------------------------*/

#pragma once

#include "Chess.h"
#include "GameBoard.h"

class King{
public:
	King(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
private:
	GameBoard *bo;									//���Ǘ��N���X�C���X�^���X
};

class Queen{
public:
	Queen(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
private:
	GameBoard *bo;									//���Ǘ��N���X�C���X�^���X
};

class Rook{
public:
	Rook(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
private:
	GameBoard *bo;									//���Ǘ��N���X�C���X�^���X
};

class Bishop{
public:
	Bishop(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
private:
	GameBoard *bo;									//���Ǘ��N���X�C���X�^���X
};

class Knight{
public:
	Knight(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
private:
	GameBoard *bo;									//���Ǘ��N���X�C���X�^���X
};

class Pawn{
public:
	Pawn(GameBoard *p){bo = p;}
	void MovePoint(int x, int y, Player player);	//������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
	void ActPoint(int x, int y, Player player);		//�|�[�����G�������ꏊ��OK�t���O�Ƃ��ăf�[�^�ɓ����
	//x,y: ��̂���ʒu, player:�v���C���[���
private:
	GameBoard *bo;									//���Ǘ��N���X�C���X�^���X
};
