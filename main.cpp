/*-------------------------------------
	main.cpp
	OpenGL課題 -チェスゲーム-
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
---------------------------------------*/

#include <iostream>
#include <string>
#include <cstdlib>

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "Mouse.h"
#include "BoardAdm.h"

//グローバル変数
int WindowWidth = 800;
int WindowHeight = 800;
const int WindowPositionX = 100;
const int WindowPositionY = 100;
const char WindowTitle[] = "OpenGL課題";

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

//利用クラスのインスタンス
ViewCamera camera(4.0);		//distance = 4.0
BoardAdm badm;

//関数プロトタイプ宣言
void Initialize(void);					//初期化
void Shutdown(void);					//終了時行う関数
void Display(void);						//描画
void Idle(void);						//アイドル時ループ
void Reshape(int, int);					//サイズ変更
void Mouse(int, int, int, int);			//マウスイベント
void Motion(int, int);					//マウスドラッグ時
void PassiveMotion(int, int);			//マウス動き
void Keyboard(unsigned char, int, int);	//キーボードイベント
void Special(int, int, int);			//特殊キー処理
void Render2D(void);					//2D描画
void Render3D(void);					//3D描画
void glutRenderText(void*, char*);		//ASCII文字列の描画
void glutToggleFullScreen(void);		//フルスクリーン表示

int main(int argc, char **argv){

	//GLUT初期設定
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

	//初期化
	Initialize();

	//Loading表示を消す
	system("cls");

	//メインループ
	glutMainLoop();

	//後処理
	Shutdown();

return 0;
}

//初期化処理
void Initialize(){

	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *vender = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);

	sprintf(Version, "OpenGL %s", version);
	sprintf(Vender, "%s", vender);
	sprintf(Renderer, "%s", renderer);

	//カウンターの初期化
	CurrentCount = LastCount = glutGet(GLUT_ELAPSED_TIME);
	FrameCount = 0;

	//盤表現クラスの初期化
	badm.Init();

	//バックバッファをクリアする色
	glClearColor(0.3, 0.3, 1.0, 1.0);

	//深度テストON
	glEnable(GL_DEPTH_TEST);

	//ライティング
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

	//ウィンドウ表示
	glutShowWindow();
}

//アイドル時ループ
void Idle(){

	//アイドル時再描画
	glutPostRedisplay();
}

//サイズ変更
void Reshape(int x, int y){

	//ウィンドウサイズを保存
	WindowWidth = x;
	WindowHeight = y;

	//サイズチェック
	if( WindowWidth < 1 )
		WindowWidth = 1;
	if( WindowHeight < 1 )
		WindowHeight = 1;

	//ビューポートの設定
	glViewport(0, 0, WindowWidth, WindowHeight);

	//射影行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
}

//2D描画
void Render2D(void){

	bool isLighting = false;

	//時間計測とFPS算出
	CurrentCount = glutGet(GLUT_ELAPSED_TIME);
	CurrentTime = (CurrentCount - LastCount)/1000.0;
	FrameCount++;
	if( (CurrentTime - LastTime) >= 1.0 ){
		Fps = FrameCount/(CurrentTime - LastTime);
		sprintf(FpsString, "%.3f FPS", Fps);
		FrameCount = 0;
		LastTime = CurrentTime;
	}

	//白黒どちらのターンか
	std::string turn = badm.GetTurn();

	//3Dから2Dへ
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

	//文字の描画
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
	
	//2Dから3Dへ
	glPopMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//3D描画
void Render3D(void){

	badm.Draw();
}

//ウィンドウへの描画
void Display(void){

	//バックバッファクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//モデルビュー行列の設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//視点の設定
	camera.Set();

	glPushMatrix();

	//3D描画
	Render3D();

	//2D描画
	Render2D();

	glPopMatrix();

	//ダブルバッファ
	glutSwapBuffers();
}

//マウス処理
void Mouse(int button, int state, int x, int y){

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ){		//左ボタンを押したとき
		badm.SetCurPoint(x,y);
	}else{
		camera.MouseInput(button, state, x, y);
	}
}

//マウスドラッグ時
void Motion(int x, int y){

	camera.MouseMotion(x, y);

}

//マウス移動時
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

//特殊キー処理
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

//後処理
void Shutdown(){


}

//ASCII文字列の描画
void glutRenderText(void* bitmapfont, char* text){

	for(int i=0; i<(int)strlen(text); i++)
		glutBitmapCharacter(bitmapfont, (int)text[i]);

}

//フルスクリーン化
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
