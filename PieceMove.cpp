/*-------------------------------------
	PieceMove.cpp
	�`�F�X��̓����N���X�Q
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
---------------------------------------*/

#include "PieceMove.h"

//�L���O
void King::MovePoint(int x, int y, Player player){

	const int NMOVE = 8;			//������ő�̃}�X��

	int diff[NMOVE][2] = {		//�����f�[�^�i0��x, 1��y�j
		{0,1},{0,-1},
		{1,0},{-1,0},
		{1,1},{1,-1},
		{-1,1},{-1,-1}
	};

	for(int i=0; i<NMOVE; ++i){
		if( x+diff[i][0] < 0 || (BNUM-1) < x+diff[i][0]
			|| y+diff[i][1] < 0 || (BNUM-1) < y+diff[i][1] )	//�͈͊O�͒e��
				continue;
			if( bo->GetPiecePlayer(x+diff[i][0], y+diff[i][1]) != player )
				bo->SetOK(x+diff[i][0], y+diff[i][1]);
			else
				bo->SetNG(x+diff[i][0], y+diff[i][1]);
	}
}

//�N�C�[��
void Queen::MovePoint(int x, int y, Player player){

	const int NVECTMOVE = 8;	//�����������8����

	int diff[NVECTMOVE][2] = {	//8�����ɑ΂��鍷���f�[�^�i0��x, 1��y�j
		{0,1},{0,-1},
		{1,0},{-1,0},
		{1,1},{1,-1},
		{-1,1},{-1,-1}
	};

	int tempX, tempY;			//�v�Z��̉�XY�ʒu
	bool jump;					//����щz���Ȃ��t���O(true�ł���ȏ�i�߂��Ȃ�)

	for(int i=0; i<NVECTMOVE; ++i){		//8���ʂɑ΂��郋�[�v
		jump = false;
		tempX = x;
		tempY = y;
		for(int j=0; j<8; ++j){			//�ő��8�}�X�i��
			tempX += diff[i][0];
			tempY += diff[i][1];

			if( tempX < 0 || (BNUM-1) < tempX 
				|| tempY < 0 || (BNUM-1) < tempY)	//�͈͊O�ɓ��B����ƃ��[�v�𔲂���
				break;

			if( jump ){													//��щz�����Ȃ��t���OON
				bo->SetNG(tempX,tempY);
			}else if( bo->GetPiecePlayer(tempX,tempY) == player ){		//���̏ꏊ�ɖ����̋����ꍇ
				bo->SetNG(tempX,tempY);
				jump = true;
			}else if( bo->GetPiecePlayer(tempX,tempY) == Yet ){			//���̏ꏊ�ɂ͉����Ȃ��ꍇ
				bo->SetOK(tempX,tempY);
			}else{														//���̏ꏊ�ɓG�̋����ꍇ
				bo->SetOK(tempX,tempY);
				jump = true;
			}
		}
	}
}

//���[�N
void Rook::MovePoint(int x, int y, Player player){

	const int NVECTMOVE = 4;	//�����������4����

	int diff[NVECTMOVE][2] = {	//4�����ɑ΂��鍷���f�[�^�i0��x, 1��y�j
		{0,1},{0,-1},
		{1,0},{-1,0},
	};

	int tempX, tempY;			//�v�Z��̉�XY�ʒu
	bool jump;					//����щz���Ȃ��t���O(true�ł���ȏ�i�߂��Ȃ�)

	for(int i=0; i<NVECTMOVE; ++i){		//4���ʂɑ΂��郋�[�v
		jump = false;
		tempX = x;
		tempY = y;
		for(int j=0; j<8; ++j){			//�ő��8�}�X�i��
			tempX += diff[i][0];
			tempY += diff[i][1];

			if( tempX < 0 || (BNUM-1) < tempX 
				|| tempY < 0 || (BNUM-1) < tempY)	//�͈͊O�ɓ��B����ƃ��[�v�𔲂���
				break;

			if( jump ){													//��щz�����Ȃ��t���OON
				bo->SetNG(tempX,tempY);
			}else if( bo->GetPiecePlayer(tempX,tempY) == player ){		//���̏ꏊ�ɖ����̋����ꍇ
				bo->SetNG(tempX,tempY);
				jump = true;
			}else if( bo->GetPiecePlayer(tempX,tempY) == Yet ){			//���̏ꏊ�ɂ͉����Ȃ��ꍇ
				bo->SetOK(tempX,tempY);
			}else{														//���̏ꏊ�ɓG�̋����ꍇ
				bo->SetOK(tempX,tempY);
				jump = true;
			}
		}
	}
}

//�r�V���b�v
void Bishop::MovePoint(int x, int y, Player player){

	const int NVECTMOVE = 4;	//�����������4����

	int diff[NVECTMOVE][2] = {	//4�����ɑ΂��鍷���f�[�^�i0��x, 1��y�j
		{1,1},{1,-1},
		{-1,1},{-1,-1}
	};

	int tempX, tempY;			//�v�Z��̉�XY�ʒu
	bool jump;					//����щz���Ȃ��t���O(true�ł���ȏ�i�߂��Ȃ�)

	for(int i=0; i<NVECTMOVE; ++i){		//4���ʂɑ΂��郋�[�v
		jump = false;
		tempX = x;
		tempY = y;
		for(int j=0; j<8; ++j){			//�ő��8�}�X�i��
			tempX += diff[i][0];
			tempY += diff[i][1];

			if( tempX < 0 || (BNUM-1) < tempX 
				|| tempY < 0 || (BNUM-1) < tempY)	//�͈͊O�ɓ��B����ƃ��[�v�𔲂���
				break;

			if( jump ){													//��щz�����Ȃ��t���OON
				bo->SetNG(tempX,tempY);
			}else if( bo->GetPiecePlayer(tempX,tempY) == player ){		//���̏ꏊ�ɖ����̋����ꍇ
				bo->SetNG(tempX,tempY);
				jump = true;
			}else if( bo->GetPiecePlayer(tempX,tempY) == Yet ){			//���̏ꏊ�ɂ͉����Ȃ��ꍇ
				bo->SetOK(tempX,tempY);
			}else{														//���̏ꏊ�ɓG�̋����ꍇ
				bo->SetOK(tempX,tempY);
				jump = true;
			}
		}
	}
}

//�i�C�g
void Knight::MovePoint(int x, int y, Player player){

	const int NMOVE = 8;			//������ő�̃}�X��

	int diff[NMOVE][2] = {		//�����f�[�^�i0��x, 1��y�j
		{2,1},{2,-1},
		{1,2},{-1,2},
		{-2,1},{-2,-1},
		{1,-2},{-1,-2}
	};

	for(int i=0; i<NMOVE; ++i){
		if( x+diff[i][0] < 0 || (BNUM-1) < x+diff[i][0]
			|| y+diff[i][1] < 0 || (BNUM-1) < y+diff[i][1] )	//�͈͊O�͒e��
				continue;

			if( bo->GetPiecePlayer(x+diff[i][0], y+diff[i][1]) != player)
				bo->SetOK(x+diff[i][0], y+diff[i][1]);
			else
				bo->SetNG(x+diff[i][0], y+diff[i][1]);
	}
}

//�|�[��
void Pawn::MovePoint(int x, int y, Player player){

	if( player == White ){
		if( bo->GetPiece(x-1,y) != empty ){		//�O�ɋ����Ƃ�
			bo->SetNG(x-1,y);				//�O�ɐi�߂Ȃ�
		}else{
			bo->SetOK(x-1,y);
			if( bo->CheckPieceInitP(x,y) && bo->GetPiece(x-2,y) == empty )		//�����ʒu����2�O�ɋ�Ȃ��Ƃ�
				bo->SetOK(x-2,y);			//2�}�X�i�߂�
		}
		if( bo->GetPiecePlayer(x-1,y-1) != player && bo->GetPiecePlayer(x-1,y-1) != Yet )
			bo->SetOK(x-1,y-1);

		if( bo->GetPiecePlayer(x-1,y+1) != player && bo->GetPiecePlayer(x-1,y+1) != Yet )
			bo->SetOK(x-1,y+1);

	}else if( player == Black ){
		if( bo->GetPiece(x+1,y) != empty ){
			bo->SetNG(x+1,y);
		}else{
			bo->SetOK(x+1,y);
			if( bo->CheckPieceInitP(x,y) && bo->GetPiece(x+2,y) == empty )
				bo->SetOK(x+2,y);
		}
		if( bo->GetPiecePlayer(x+1,y+1) != player && bo->GetPiecePlayer(x+1,y+1) != Yet )
			bo->SetOK(x+1,y+1);

		if( bo->GetPiecePlayer(x+1,y-1) != player && bo->GetPiecePlayer(x+1,y-1) != Yet )
			bo->SetOK(x+1,y-1);

	}
}

//�|�[�����G�������ꏊ��ok�t���O�Ƃ��ăf�[�^�ɓ����
void Pawn::ActPoint(int x, int y, Player player){

	if( player == White ){
		if( bo->GetPiecePlayer(x-1,y-1) != player && bo->GetPiecePlayer(x-1,y-1) != Yet )
			bo->SetOK(x-1,y-1);

		if( bo->GetPiecePlayer(x-1,y+1) != player && bo->GetPiecePlayer(x-1,y+1) != Yet )
			bo->SetOK(x-1,y+1);

	}else if( player == Black ){
		if( bo->GetPiecePlayer(x+1,y+1) != player && bo->GetPiecePlayer(x+1,y+1) != Yet )
			bo->SetOK(x+1,y+1);

		if( bo->GetPiecePlayer(x+1,y-1) != player && bo->GetPiecePlayer(x+1,y-1) != Yet )
			bo->SetOK(x+1,y-1);

	}
}