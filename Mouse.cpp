/*-------------------------------------
	Mouse.h
	マウスによる視点移動クラス群
	by y.y
	2015/01/28		β版
---------------------------------------*/

#include <iostream>
#include <cmath>
#include <gl/glut.h>

#include "Mouse.h"


/*---MouseButtonクラス---*/

//コンストラクタ
MouseButton::MouseButton(){
	Reset();
}

//リセットする
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

//コンストラクタ
ViewCamera::ViewCamera(double dist){
	distance = dist;
	Reset();
}


//マウスの移動処理
void ViewCamera::MouseMotion( int x, int y ) {
	
	if( left.state == Push ){			//左ボタンの処理
		;//駒をクリックするため、ここでは処理しない
	}

	if( right.state == Push ){			//右ボタンの処理
	
		right.current.x = (double)x - right.before.x + right.after.x;
		right.current.y  = -(double)y -right.before.y + right.after.y;
	}

	if( middle.state == Push ){			//中ボタンの処理
	
		//　移動量を計算
		middle.current.x = (double)x - middle.before.x + middle.after.x;
		middle.current.y = (double)y - middle.before.y + middle.after.y;

		if ( middle.current.y >= 360.0 ) middle.current.y -= 360.0;
		else if ( middle.current.y < 0.0 ) middle.current.y += 360.0;
		
		angle[0] = DegToRad(angle[0] + middle.current.x);
		angle[1] = DegToRad(angle[1] + middle.current.y);

	}
}

//マウスのボタン処理
void ViewCamera::MouseInput( int button, int state, int x, int y ){ //戻り値:盤管理クラスに座標データを渡すかどうか

	switch( button ){
	//右ボタン
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

	//中ボタン
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

//パラメータをリセットする
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
		
//視点の設定
void ViewCamera::Set(){

	double zoom = distance;
	zoom += right.current.y/30.0;

	//視点位置を決定
	position[0] = sin(angle[0]) * cos(angle[1]) * zoom;
	position[1] = sin(angle[1]) * zoom;
	position[2] = cos(angle[0]) * cos(angle[1]) * zoom;

	//アップベクトルの設定
	if( angle[1] > DegToRad(90.0)  &&  angle[1] < DegToRad(270.0) )
		upvector[1] = -1.0;
	else
		upvector[1] = 1.0;

	//平行移動
	glTranslated( translate[0], translate[1], translate[2] );

	//視点位置の設定
	gluLookAt(
		position[0], position[1], position[2],
		target[0], target[1], target[2],
		upvector[0], upvector[1], upvector[2] );

}
