/*-------------------------------------
	Mouse.h
	�}�E�X�ɂ�鎋�_�ړ��N���X�Q
	by y.y
	2015/01/28		����
---------------------------------------*/

#include <iostream>
#include <cmath>
#include <gl/glut.h>

#include "Mouse.h"


/*---MouseButton�N���X---*/

//�R���X�g���N�^
MouseButton::MouseButton(){
	Reset();
}

//���Z�b�g����
void MouseButton::Reset(){
	before.x = 0.0;
	before.y = 0.0;
	current.x = 0.0;
	current.y = 0.0;
	after.x = 0.0;
	after.y = 0.0;
	state = None;
}

/*---ViewCamara class---*/

//�R���X�g���N�^
ViewCamera::ViewCamera(double dist){
	distance = dist;
	Reset();
}


//�}�E�X�̈ړ�����
void ViewCamera::MouseMotion( int x, int y ) {
	
	if( left.state == Push ){			//���{�^���̏���
		;//����N���b�N���邽�߁A�����ł͏������Ȃ�
	}

	if( right.state == Push ){			//�E�{�^���̏���
	
		right.current.x = (double)x - right.before.x + right.after.x;
		right.current.y  = -(double)y -right.before.y + right.after.y;
	}

	if( middle.state == Push ){			//���{�^���̏���
	
		//�@�ړ��ʂ��v�Z
		middle.current.x = (double)x - middle.before.x + middle.after.x;
		middle.current.y = (double)y - middle.before.y + middle.after.y;

		if ( middle.current.y >= 360.0 ) middle.current.y -= 360.0;
		else if ( middle.current.y < 0.0 ) middle.current.y += 360.0;
		
		angle[0] = DegToRad(angle[0] + middle.current.x);
		angle[1] = DegToRad(angle[1] + middle.current.y);

	}
}

//�}�E�X�̃{�^������
void ViewCamera::MouseInput( int button, int state, int x, int y ){ //�߂�l:�ՊǗ��N���X�ɍ��W�f�[�^��n�����ǂ���

	switch( button ){
	//�E�{�^��
	case GLUT_RIGHT_BUTTON :
		if( state == GLUT_DOWN ){ 

			right.before.x = (double)x;
			right.before.y = -(double)y;
			right.state = Push;
		}
		else if( state == GLUT_UP ){			

			right.after.x = right.current.x;
			right.after.y = right.current.y;
			right.state = Release;
		}
		break;

	//���{�^��
	case GLUT_MIDDLE_BUTTON :
		if( state == GLUT_DOWN ){
			middle.before.x = (double)x;
			middle.before.y = (double)y;
			middle.state = Push;
		}
		else if( state == GLUT_UP ){	
			middle.after.x = left.current.x;
			middle.after.y = left.current.y;
			middle.state = Release;
		}
		break;
	}
}

//�p�����[�^�����Z�b�g����
void ViewCamera::Reset(){

	left.Reset();
	right.Reset();
	middle.Reset();

	angle[0] = DegToRad(90.0);
	angle[1] = DegToRad(45.0);
	angle[2] = 0.0;
	position[0] = 0.0;
	position[1] = 0.0;
	position[2] = 0.0;
	target[0] = 0.0;
	target[1] = 0.0;
	target[2] = 0.0;
	upvector[0] = 0.0;
	upvector[1] = 1.0;
	upvector[2] = 0.0;
	translate[0] = 0.125;
	translate[1] = 0.25;
	translate[2] = 0.0;
}
		
//���_�̐ݒ�
void ViewCamera::Set(){

	double zoom = distance;
	zoom += right.current.y/30.0;

	//���_�ʒu������
	position[0] = sin(angle[0]) * cos(angle[1]) * zoom;
	position[1] = sin(angle[1]) * zoom;
	position[2] = cos(angle[0]) * cos(angle[1]) * zoom;

	//�A�b�v�x�N�g���̐ݒ�
	if( angle[1] > DegToRad(90.0)  &&  angle[1] < DegToRad(270.0) )
		upvector[1] = -1.0;
	else
		upvector[1] = 1.0;

	//���s�ړ�
	glTranslated( translate[0], translate[1], translate[2] );

	//���_�ʒu�̐ݒ�
	gluLookAt(
		position[0], position[1], position[2],
		target[0], target[1], target[2],
		upvector[0], upvector[1], upvector[2] );

}
