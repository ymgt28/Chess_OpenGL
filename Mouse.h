/*-------------------------------------
	Mouse.h
	�}�E�X�ɂ�鎋�_�ړ��N���X�w�b�_�Q
	by y.y
	2015/01/28		����
---------------------------------------*/

#pragma once

/*---�O���[�o���ϐ�---*/
const double PI = 3.14159265358979323846264338327;
template<class T> static inline T RadToDeg(T rad) { return ( (rad)*(180.0/PI) ); }
template<class T> static inline T DegToRad(T deg) { return ( (deg)*(PI/180.0) ); }

/*---�}�E�X��Ԏ��ʎq---*/
enum MState{
	Push,
	Release,
	None
};

/*---MPosition�N���X---*/
class MPosition{
public:
	double x;
	double y;

	MPosition(double nx=0.0, double ny=0.0) { x=nx; y=ny; }
	void Reset() { x = 0.0, y=0.0; }
};

/*---MouseButton�N���X---*/
class MouseButton{
public:
	MPosition before;
	MPosition current;
	MPosition after;
	MState state;

	MouseButton();
	void Reset();
};

/*---ViewCamara class---*/
class ViewCamera{
public:
	MouseButton right;
	MouseButton left;
	MouseButton middle;

	double distance;
	double angle[3];
	double position[3];
	double target[3];
	double upvector[3];
	double translate[3];

	ViewCamera(double dist=5.0);
	void Reset();
	void MouseInput(int button, int state, int x, int y);
	void MouseMotion(int x, int y);
	void Set();
};
