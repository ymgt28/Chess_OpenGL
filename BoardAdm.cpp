/*---------------------------------------
	Board.cpp
	�Օ\���N���X
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
-----------------------------------------*/

#include "BoardAdm.h"

/*---�I�u�W�F�N�g�̐F---*/
GLfloat red[4] = { 256.0/256.0, 38.0/256.0, 0.0/256.0, 1.0};		//���O
GLfloat black[4] = { 91.0/256.0, 66.0/256.0, 61.0/256.0, 1.0};		//�ΐF
GLfloat white[4] = { 196.0/256.0, 205.0/256.0, 207.0/256.0, 1.0};	//�V���o�[�z���C�g
GLfloat pink[4] = {255.0/255.0, 20.0/255.0, 147.0/255.0, 1.0};		//�s���N

//�R���X�g���N�^
BoardAdm::BoardAdm(){

	hits = 0;
	m_select = -1;
	mouse_state = false;
	bo = new GameBoard();
	cgame = NULL;
}

//�f�X�g���N�^
BoardAdm::~BoardAdm(){

	delete bo;
	delete cgame;
}

//�S�̏�����
void BoardAdm::Init(void){

	BoardInit();
	PieceInit();
	if( cgame == NULL ){
		cgame = new ChessGame(bo);
	}else{
		delete cgame;
		cgame = new ChessGame(bo);
	}
}

//�������iglList�����Ǝs���͗l�̐ݒ�E��������ʒu�ցj
void BoardAdm::BoardInit(void){

	objects = glGenLists(NOBJECTS);

	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){

			glNewList(objects + (i*BNUM+j), GL_COMPILE);
			
			glPushMatrix();
			glTranslatef((float)(i-3)*BoardScale, 0.0, (float)(j-3)*BoardScale);
			glScalef(1.0, 0.05, 1.0);

			glutSolidCube(BoardScale);
			glPopMatrix();

			glEndList();

			if( (i+j)%2 == 1 )
				color[(i*BNUM+j)] = white;
			else
				color[(i*BNUM+j)] = black;
		}
	}
	bo->InitBoardPiece();
}

//��̏�����
void BoardAdm::PieceInit(void){

#ifndef DEBUG_MINILOAD
	//���f���t�@�C���̓ǂݍ���
	model[King].Load("Model/ChessPiece/King.x");
	model[Queen].Load("Model/ChessPiece/Queen.x");
	model[Rook].Load("Model/ChessPiece/Rook.x");
	model[Bishop].Load("Model/ChessPiece/Bishop.x");
	model[Knight].Load("Model/ChessPiece/Knight.x");
	model[Pawn].Load("Model/ChessPiece/Pawn.x");
#endif

}

//�S�̕`��֐�
void BoardAdm::Draw(void){

	DrawBoard();
	DrawPiece();
	if(mouse_state){
		DrawBoardPoint();
		mouse_state = false;
	}
}

//�Օ\��
void BoardAdm::DrawBoard(void){

	int x,y;
	for (int i = 0; i < NOBJECTS; i++) {
		x = ReturnX(i);
		y = ReturnY(i);
		if( bo->GetDsel(x,y) )
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		else if( bo->GetIsel(x,y) )
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pink);
		else
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[i]);
		glCallList(objects + i);
	}
	glFlush();
}

//�Ղ̃}�E�X�𓖂Ă�ꂽ�ꏊ�������\��
void BoardAdm::DrawBoardPoint(void){

	//�}�E�X���W����I������Ă���ꏊ���Z���N�g
	SelectPoint();

	//�q�b�g�����I�u�W�F�N�g������
	int hit_name = SelectHits();

	//���ʂ����I�u�W�F�N�g�ɑ΂��铮��
	if( hit_name != -1 ){
		SelectDo(hit_name);
	}else{
		bo->ClearFlag();		//�͈͊O�w��Ȃ瑀��̃L�����Z��
	}
}

//�}�E�X���������Ă���ꏊ�𔻒�
void BoardAdm::SelectPoint(void){

	GLint vp[4];
	hits = 0;

	/* �Z���N�V�����Ɏg���o�b�t�@�̐ݒ�D����̓Z���N�V��
	�����[�h�ȊO�̎��iglRenderMode(GL_SELECT) ���O�j
	�Ɏ��s����K�v������D�Z���N�V�����o�b�t�@�ɂ́C��
	�邾���̃f�[�^���l�ߍ��܂�� */
	glSelectBuffer(SELECTIONS, selection);

	/* �����_�����O���[�h���Z���N�V�������[�h�ɐؑւ��� */
	glRenderMode(GL_SELECT);

	/* �Z���N�V�����o�b�t�@�̏������C����̓Z���N�V�������[
	�h�ɂȂ��ĂȂ��Ɩ�������� */
	glInitNames();

	/* �l�[���X�^�b�N�̐擪�ɉ��̖��O���l�߂Ă����D�l�[��
	�X�^�b�N���͕̂����̃I�u�W�F�N�g���I���ł���悤��
	�X�^�b�N�\���ɂȂ��Ă��邪�C����͂P�̃I�u�W�F�N
	�g�����I�����Ȃ��̂ŁC�l�[���X�^�b�N�̐擪�̗v�f��
	�������ւ��Ȃ���`�悷��΂悢�D�����ŁC���炩��
	�߃l�[���X�^�b�N�̐擪�ɉ��̖��O (-1) �ɋl�߂Ă���
	�āC�������g���񂷁D */
	glPushName(-1);

	/* �Z���N�V�����̏����͎��_���W�n�ōs�� */
	glMatrixMode(GL_PROJECTION);

	/* ���݂̓����ϊ��}�g���N�X��ۑ����� */
	glPushMatrix();

	/* �����ϊ��}�g���N�X������������ */
	glLoadIdentity();

	/* ���݂̃r���[�|�[�g�ݒ�𓾂� */
	glGetIntegerv(GL_VIEWPORT, vp);

	/* �\���̈悪�}�E�X�|�C���^�̎��͂����ɂȂ�悤�ɕϊ�
	�s���ݒ肷��D�}�E�X�̍��W�n�́C�X�N���[���̍��W
	�n�ɑ΂��ď㉺�����]���Ă���̂ŁC�����␳���� */
	gluPickMatrix(curX, vp[3] - curY, 3.0, 3.0, vp);

	/* �ʏ�̕`��Ɠ��������ϊ��}�g���N�X��ݒ肷��D�E�B
	���h�E�S�̂��r���[�|�[�g�ɂ��Ă���̂ŁC�A�X�y�N�g
	��� vp[2] / vp[3] �œ�����D*/
	gluPerspective(45.0, (double)vp[2] / (double)vp[3], 1.0, 100.0);

	/* ���f���r���[�}�g���N�X�ɐؑւ��� */
	glMatrixMode(GL_MODELVIEW);

	/* ������x�V�[����`�悷�� */
	for (int i = 0; i < NOBJECTS; i++) {
		/* �l�[���X�^�b�N�̐擪�ɂ��ꂩ��`���I�u�W�F�N�g��
		�ԍ���ݒ肷�� */
		glLoadName(i);
		/* �I�u�W�F�N�g��`�悷��i��ʂɂ͕\������Ȃ��j*/
		glCallList(objects + i);
	}

	/* �Ăѓ����ϊ��}�g���N�X�ɐؑւ��� */
	glMatrixMode(GL_PROJECTION);

	/* �����ϊ��}�g���N�X�����ɖ߂� */
	glPopMatrix();

	/* ���f���r���[�}�g���N�X�ɖ߂� */
	glMatrixMode(GL_MODELVIEW);

	/* �����_�����O���[�h�����ɖ߂� */
	hits = glRenderMode(GL_RENDER);

}

//�q�b�g�����I�u�W�F�N�g������
int BoardAdm::SelectHits(void){

	// �ő�K�w����4�Ɖ���
	int hit_name[4]={-1, -1, -1, -1};
	float depth_min=10.0f;
	float depth_1=1.0f;
	float depth_2=1.0f;
	GLuint depth_name;
	GLuint *ptr;

	// �q�b�g�����f�[�^�Ȃ�
	if(hits<=0)
		return -1;

	// �|�C���^����Ɨpptr�֓n���D
	ptr = selection;

#ifdef DEGUB_SelectHits
	std::cout << "�q�b�g����: " << hits << std::endl << std::endl;;
#endif

	for(int i=0; i<hits; i++){

#ifdef DEGUB_SelectHits		
		std::cout << "�q�b�g: " << i+1 << std::endl;
#endif

		// ���ʔԍ��̊K�w�̐[��
		depth_name = *ptr;

#ifdef DEGUB_SelectHits
		std::cout << "  ���ʔԍ��̊K�w��: " << depth_name << std::endl;
#endif
		ptr++;
		depth_1 = (float) *ptr/0x7fffffff;

#ifdef DEGUB_SelectHits
		std::cout << "    �f�v�X�̍ŏ��l: " << depth_1 << std::endl;
#endif
		ptr++;
		depth_2 = (float) *ptr/0x7fffffff;

#ifdef DEGUB_SelectHits
		std::cout << "    �f�v�X�̍ő�l: " << depth_2 << std::endl;
#endif
		ptr++;

#ifdef DEGUB_SelectHits
		std::cout << "    ���ʔԍ�: ";
#endif

		// �ŏ��f�v�X�̊m�F
		if(depth_min>depth_1){
			depth_min = depth_1;
			// ���ʔԍ���ۑ�
			for(unsigned int j=0; j<depth_name; j++){

#ifdef DEGUB_SelectHits
				std::cout << *ptr << " ";
#endif
				hit_name[j] = *ptr;
				ptr++;
			}
		}else{
			for(unsigned int j=0; j<depth_name; j++){

#ifdef DEGUB_SelectHits
				std::cout << *ptr << " ";
#endif
				ptr++;
			}
		}

#ifdef DEGUB_SelectHits
		std::cout << std::endl;
#endif
	}

#ifdef DEGUB_SelectHits
	std::cout << std::endl << "�f�v�X�ŏ��̎��ʔԍ�: " << std::endl;
#endif
	for(int i=0;i<4;i++){
		if(hit_name[i]==-1)
			break;

#ifdef DEGUB_SelectHits
		std::cout << hit_name[i] << " ";
#endif
	}

#ifdef DEGUB_SelectHits
	std::cout << std::endl << std::endl << std::endl;
#endif
	return hit_name[0];
}

//���ʂ����I�u�W�F�N�g�ɑ΂��铮��
void BoardAdm::SelectDo(int hit_name){

	int x = ReturnX(hit_name);
	int y = ReturnY(hit_name);

	//���ʂ����}�X�����ڎw�肵�Ă��}�X�Ȃ�
	if( bo->GetDsel(x,y) ){
		bo->ClearFlag();		//����̃L�����Z���Ƃ݂Ȃ�
		return;
	}
	//���ʂ����}�X���Ԑڎw�肵�Ă��}�X�Ȃ�
	if( bo->GetIsel(x,y) ){
		int tempX = ReturnX(m_select);
		int tempY = ReturnY(m_select);
		
		cgame->Move(x,y,tempX,tempY);			//��̈ړ�
		bo->ClearFlag();
		return;
	}
	
	//���ʂ����}�X�ɋ�Ȃ�or����̋�Ȃ� �������Ȃ�or����̃L�����Z��
	Player nowP = cgame->CheckPlayer();
	if( bo->GetPiecePlayer(x,y) != nowP  || nowP == EndB || nowP == EndW || nowP == EndST ){
		bo->ClearFlag();
		return;
	}

	//����ȊO�Ȃ�V���ȃt���O�̐ݒ�
	m_select = hit_name;						//���Z���N�g���ꂽ�I�u�W�F�N�g�ԍ���ێ�
	bo->ClearFlag();
	bo->SetDselTrue(x,y);						//�I���������Dsel

	bo->SetNG(x,y);								//����������}�X�ɂ͓����Ȃ�
	
	cgame->CheckMove(x,y);						//�Q�[�����[���㓮���邩�𔻒f

	bo->ChangeOK();								//�Ō�Ɏc����ok���ʎq�̏ꏊ�ɂ���Isel�t���O��true�ɂ���

}

//��\���֐�
void BoardAdm::DrawPiece(void){

	static const float rgb = 51.0/255.0;
	for(int i=0; i<BNUM; i++){
		for(int j=0; j<BNUM; ++j){

			if( bo->GetPiece(i,j) == empty )
				continue;

			glPushMatrix();
			glTranslatef((float)(i-3)*BoardScale, 0.0, (float)(j-3)*BoardScale);
			glScalef(PieceScale, PieceScale, PieceScale);
#ifdef DEBUG_MINILOAD
			glutSolidTeapot(0.8);
#else
			if( bo->GetPiecePlayer(i,j) == White )
				model[ RetModelNo(bo->GetPiece(i,j)) ].Render();
			else
				model[ RetModelNo(bo->GetPiece(i,j)) ].Render(rgb,rgb,rgb,1.0);
#endif			
			glPopMatrix();

		}
	}
}

// ChessPiece��ChessPieceModel�̕ϊ�
ChessPieceModel BoardAdm::RetModelNo(ChessPiece pi){
	
	switch(pi){
		case BKing:
		case WKing:
			return King;
		case BQueen:
		case WQueen:
			return Queen;
		case BRook:
		case WRook:
			return Rook;
		case BBishop:
		case WBishop:
			return Bishop;
		case BKnight:
		case WKnight:
			return Knight;
		case BPawn:
		case WPawn:
			return Pawn;
	}
}

//�ǂ���̏o�Ԃ���Ԃ�
std::string BoardAdm::GetTurn(void){

	std::string str;

	if( cgame->CheckTurn() == WhiteTurn ){
		str.append("White Turn");
	}else if( cgame->CheckTurn() == BlackTurn ){
		str.append("Black Turn");
	}else if( cgame->CheckTurn() == EndB){
		str.append("Game End. Black Win");
	}else if( cgame->CheckTurn() == EndW){
		str.append("Game End. White Win");
	}else if( cgame->CheckTurn() == EndST){
		str.append("Game End. Stalemate");
	}
	return str;
}