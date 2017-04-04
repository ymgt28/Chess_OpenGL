/*-------------------------------------
	main.cpp
	OpenGL�ۑ� -�`�F�X�Q�[��-
	by y.y
	2014/12/16		����
	2015/01/19		Ver1.00		������
---------------------------------------*/

#include <iostream>
#include <string>
#include <cstdlib>

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "Mouse.h"
#include "BoardAdm.h"

//�O���[�o���ϐ�
int WindowWidth = 800;
int WindowHeight = 800;
const int WindowPositionX = 100;
const int WindowPositionY = 100;
const char WindowTitle[] = "OpenGL�ۑ�";

char Version[50];
char Vender[50];
char Renderer[50];
char FpsString[50] = {0};
double CurrentTime = 0.0;
double LastTime = 0.0;
double CurrentCount = 0.0;
double LastCount = 0.0;
int FrameCount = 0;
float Fps = 0.0f;

//���p�N���X�̃C���X�^���X
ViewCamera camera(4.0);		//distance = 4.0
BoardAdm badm;

//�֐��v���g�^�C�v�錾
void Initialize(void);					//������
void Shutdown(void);					//�I�����s���֐�
void Display(void);						//�`��
void Idle(void);						//�A�C�h�������[�v
void Reshape(int, int);					//�T�C�Y�ύX
void Mouse(int, int, int, int);			//�}�E�X�C�x���g
void Motion(int, int);					//�}�E�X�h���b�O��
void PassiveMotion(int, int);			//�}�E�X����
void Keyboard(unsigned char, int, int);	//�L�[�{�[�h�C�x���g
void Special(int, int, int);			//����L�[����
void Render2D(void);					//2D�`��
void Render3D(void);					//3D�`��
void glutRenderText(void*, char*);		//ASCII������̕`��
void glutToggleFullScreen(void);		//�t���X�N���[���\��

int main(int argc, char **argv){

	//GLUT�����ݒ�
	glutInit(&argc, argv);
	glutInitWindowPosition(WindowPositionX,WindowPositionY);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(WindowTitle);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	std::cout << "Now Loading... Please wait" << std::endl;

	//������
	Initialize();

	//Loading�\��������
	system("cls");

	//���C�����[�v
	glutMainLoop();

	//�㏈��
	Shutdown();

return 0;
}

//����������
void Initialize(){

	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *vender = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);

	sprintf(Version, "OpenGL %s", version);
	sprintf(Vender, "%s", vender);
	sprintf(Renderer, "%s", renderer);

	//�J�E���^�[�̏�����
	CurrentCount = LastCount = glutGet(GLUT_ELAPSED_TIME);
	FrameCount = 0;

	//�Օ\���N���X�̏�����
	badm.Init();

	//�o�b�N�o�b�t�@���N���A����F
	glClearColor(0.3, 0.3, 1.0, 1.0);

	//�[�x�e�X�gON
	glEnable(GL_DEPTH_TEST);

	//���C�e�B���O
	float pos[4] = { 0.0f, 10.0f, 10.0f, 1.0f };
	float amb[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float dif[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spe[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glEnable(GL_NORMALIZE);

	//�E�B���h�E�\��
	glutShowWindow();
}

//�A�C�h�������[�v
void Idle(){

	//�A�C�h�����ĕ`��
	glutPostRedisplay();
}

//�T�C�Y�ύX
void Reshape(int x, int y){

	//�E�B���h�E�T�C�Y��ۑ�
	WindowWidth = x;
	WindowHeight = y;

	//�T�C�Y�`�F�b�N
	if( WindowWidth < 1 )
		WindowWidth = 1;
	if( WindowHeight < 1 )
		WindowHeight = 1;

	//�r���[�|�[�g�̐ݒ�
	glViewport(0, 0, WindowWidth, WindowHeight);

	//�ˉe�s��̐ݒ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
}

//2D�`��
void Render2D(void){

	bool isLighting = false;

	//���Ԍv����FPS�Z�o
	CurrentCount = glutGet(GLUT_ELAPSED_TIME);
	CurrentTime = (CurrentCount - LastCount)/1000.0;
	FrameCount++;
	if( (CurrentTime - LastTime) >= 1.0 ){
		Fps = FrameCount/(CurrentTime - LastTime);
		sprintf(FpsString, "%.3f FPS", Fps);
		FrameCount = 0;
		LastTime = CurrentTime;
	}

	//�����ǂ���̃^�[����
	std::string turn = badm.GetTurn();

	//3D����2D��
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight , 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if( glIsEnabled(GL_LIGHTING) ){
		isLighting = true;
		glDisable(GL_LIGHTING);
	}

	//�����̕`��
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glRasterPos2i(15, 15);
	glutRenderText(GLUT_BITMAP_HELVETICA_12, Version);
	glRasterPos2i(15, 30);
	glutRenderText(GLUT_BITMAP_HELVETICA_12 , Vender);
	glRasterPos2i(15, 45);
	glutRenderText(GLUT_BITMAP_HELVETICA_12 , Renderer);
	glRasterPos2i(15, 60);
	glutRenderText(GLUT_BITMAP_HELVETICA_12, FpsString);

	glColor4f(1.0, 0.0, 0.0, 1.0);
	glRasterPos2i(15, 120);
	glutRenderText(GLUT_BITMAP_TIMES_ROMAN_24, const_cast<char*>(turn.c_str()) );

	if( badm.GetChecking() ){
		glColor4f(0.0, 1.0, 0.0, 1.0);
		glRasterPos2i(300, 60);
		glutRenderText(GLUT_BITMAP_TIMES_ROMAN_24, "Checking NOW");
	}
	if( !glIsEnabled(GL_LIGHTING) )
		if(isLighting)
			glEnable(GL_LIGHTING);
	
	//2D����3D��
	glPopMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//3D�`��
void Render3D(void){

	badm.Draw();
}

//�E�B���h�E�ւ̕`��
void Display(void){

	//�o�b�N�o�b�t�@�N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���f���r���[�s��̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//���_�̐ݒ�
	camera.Set();

	glPushMatrix();

	//3D�`��
	Render3D();

	//2D�`��
	Render2D();

	glPopMatrix();

	//�_�u���o�b�t�@
	glutSwapBuffers();
}

//�}�E�X����
void Mouse(int button, int state, int x, int y){

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ){		//���{�^�����������Ƃ�
		badm.SetCurPoint(x,y);
	}else{
		camera.MouseInput(button, state, x, y);
	}
}

//�}�E�X�h���b�O��
void Motion(int x, int y){

	camera.MouseMotion(x, y);

}

//�}�E�X�ړ���
void PassiveMotion(int x, int y){

}

void Keyboard(unsigned char key, int x, int y){

	switch( key ){
		case '\033':
			exit(0);
			break;
		default:
			break;
	}

}

//����L�[����
void Special(int key, int x, int y){

	switch( key ){
		case GLUT_KEY_F1:
			glutToggleFullScreen();
			break;
		case GLUT_KEY_F2:
			camera.Reset();
			break;

		case GLUT_KEY_F3:
			break;

		case GLUT_KEY_F4:
			break;

		case GLUT_KEY_F5:
			break;

		case GLUT_KEY_F6:
			break;

		case GLUT_KEY_F7:
			break;

		case GLUT_KEY_F8:
			break;

		case GLUT_KEY_F9:
			break;

		case GLUT_KEY_F10:
			break;

		case GLUT_KEY_F11:
			break;

		case GLUT_KEY_F12:
			break;

		case GLUT_KEY_LEFT:
			break;

		case GLUT_KEY_RIGHT:
			break;

		case GLUT_KEY_UP:
			break;

		case GLUT_KEY_DOWN:
			break;

		case GLUT_KEY_PAGE_UP:
			break;

		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_HOME:
			break;

		case GLUT_KEY_END:
			break;

		case GLUT_KEY_INSERT:
			break;
	}

}

//�㏈��
void Shutdown(){


}

//ASCII������̕`��
void glutRenderText(void* bitmapfont, char* text){

	for(int i=0; i<(int)strlen(text); i++)
		glutBitmapCharacter(bitmapfont, (int)text[i]);

}

//�t���X�N���[����
void glutToggleFullScreen(void){

	static bool FullScreenMode = false;
	static int beforeWidth = WindowWidth;
	static int beforeHeight = WindowHeight;
	FullScreenMode = (FullScreenMode ?  false : true);
	if( FullScreenMode ){
		beforeWidth = WindowWidth;
		beforeHeight= WindowHeight;
		glutFullScreen();
	}else{
		glutPositionWindow(WindowPositionX, WindowPositionY);
		glutReshapeWindow(beforeWidth, beforeHeight);
		glutShowWindow();
	}

}
