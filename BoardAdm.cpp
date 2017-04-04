/*---------------------------------------
	Board.cpp
	盤表現クラス
	by y.y
	2014/12/16		α版
	2015/01/19		Ver1.00		仮完成
-----------------------------------------*/

#include "BoardAdm.h"

/*---オブジェクトの色---*/
GLfloat red[4] = { 256.0/256.0, 38.0/256.0, 0.0/256.0, 1.0};		//鉛丹
GLfloat black[4] = { 91.0/256.0, 66.0/256.0, 61.0/256.0, 1.0};		//鳶色
GLfloat white[4] = { 196.0/256.0, 205.0/256.0, 207.0/256.0, 1.0};	//シルバーホワイト
GLfloat pink[4] = {255.0/255.0, 20.0/255.0, 147.0/255.0, 1.0};		//ピンク

//コンストラクタ
BoardAdm::BoardAdm(){

	hits = 0;
	m_select = -1;
	mouse_state = false;
	bo = new GameBoard();
	cgame = NULL;
}

//デストラクタ
BoardAdm::~BoardAdm(){

	delete bo;
	delete cgame;
}

//全体初期化
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

//初期化（glList生成と市松模様の設定・駒を初期位置へ）
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

//駒の初期化
void BoardAdm::PieceInit(void){

#ifndef DEBUG_MINILOAD
	//モデルファイルの読み込み
	model[King].Load("Model/ChessPiece/King.x");
	model[Queen].Load("Model/ChessPiece/Queen.x");
	model[Rook].Load("Model/ChessPiece/Rook.x");
	model[Bishop].Load("Model/ChessPiece/Bishop.x");
	model[Knight].Load("Model/ChessPiece/Knight.x");
	model[Pawn].Load("Model/ChessPiece/Pawn.x");
#endif

}

//全体描画関数
void BoardAdm::Draw(void){

	DrawBoard();
	DrawPiece();
	if(mouse_state){
		DrawBoardPoint();
		mouse_state = false;
	}
}

//盤表示
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

//盤のマウスを当てられた場所を強調表示
void BoardAdm::DrawBoardPoint(void){

	//マウス座標から選択されている場所をセレクト
	SelectPoint();

	//ヒットしたオブジェクトを識別
	int hit_name = SelectHits();

	//識別したオブジェクトに対する動作
	if( hit_name != -1 ){
		SelectDo(hit_name);
	}else{
		bo->ClearFlag();		//範囲外指定なら操作のキャンセル
	}
}

//マウスが当たっている場所を判定
void BoardAdm::SelectPoint(void){

	GLint vp[4];
	hits = 0;

	/* セレクションに使うバッファの設定．これはセレクショ
	ンモード以外の時（glRenderMode(GL_SELECT) より前）
	に実行する必要がある．セレクションバッファには，入
	るだけのデータが詰め込まれる */
	glSelectBuffer(SELECTIONS, selection);

	/* レンダリングモードをセレクションモードに切替える */
	glRenderMode(GL_SELECT);

	/* セレクションバッファの初期化，これはセレクションモー
	ドになってないと無視される */
	glInitNames();

	/* ネームスタックの先頭に仮の名前を詰めておく．ネーム
	スタック自体は複数のオブジェクトが選択できるように
	スタック構造になっているが，今回は１個のオブジェク
	トしか選択しないので，ネームスタックの先頭の要素だ
	けを取り替えながら描画すればよい．そこで，あらかじ
	めネームスタックの先頭に仮の名前 (-1) に詰めておい
	て，そこを使い回す． */
	glPushName(-1);

	/* セレクションの処理は視点座標系で行う */
	glMatrixMode(GL_PROJECTION);

	/* 現在の透視変換マトリクスを保存する */
	glPushMatrix();

	/* 透視変換マトリクスを初期化する */
	glLoadIdentity();

	/* 現在のビューポート設定を得る */
	glGetIntegerv(GL_VIEWPORT, vp);

	/* 表示領域がマウスポインタの周囲だけになるように変換
	行列を設定する．マウスの座標系は，スクリーンの座標
	系に対して上下が反転しているので，それを補正する */
	gluPickMatrix(curX, vp[3] - curY, 3.0, 3.0, vp);

	/* 通常の描画と同じ透視変換マトリクスを設定する．ウィ
	ンドウ全体をビューポートにしているので，アスペクト
	比は vp[2] / vp[3] で得られる．*/
	gluPerspective(45.0, (double)vp[2] / (double)vp[3], 1.0, 100.0);

	/* モデルビューマトリクスに切替える */
	glMatrixMode(GL_MODELVIEW);

	/* もう一度シーンを描画する */
	for (int i = 0; i < NOBJECTS; i++) {
		/* ネームスタックの先頭にこれから描くオブジェクトの
		番号を設定する */
		glLoadName(i);
		/* オブジェクトを描画する（画面には表示されない）*/
		glCallList(objects + i);
	}

	/* 再び透視変換マトリクスに切替える */
	glMatrixMode(GL_PROJECTION);

	/* 透視変換マトリクスを元に戻す */
	glPopMatrix();

	/* モデルビューマトリクスに戻す */
	glMatrixMode(GL_MODELVIEW);

	/* レンダリングモードを元に戻す */
	hits = glRenderMode(GL_RENDER);

}

//ヒットしたオブジェクトを識別
int BoardAdm::SelectHits(void){

	// 最大階層数を4と仮定
	int hit_name[4]={-1, -1, -1, -1};
	float depth_min=10.0f;
	float depth_1=1.0f;
	float depth_2=1.0f;
	GLuint depth_name;
	GLuint *ptr;

	// ヒットしたデータなし
	if(hits<=0)
		return -1;

	// ポインタを作業用ptrへ渡す．
	ptr = selection;

#ifdef DEGUB_SelectHits
	std::cout << "ヒット総数: " << hits << std::endl << std::endl;;
#endif

	for(int i=0; i<hits; i++){

#ifdef DEGUB_SelectHits		
		std::cout << "ヒット: " << i+1 << std::endl;
#endif

		// 識別番号の階層の深さ
		depth_name = *ptr;

#ifdef DEGUB_SelectHits
		std::cout << "  識別番号の階層数: " << depth_name << std::endl;
#endif
		ptr++;
		depth_1 = (float) *ptr/0x7fffffff;

#ifdef DEGUB_SelectHits
		std::cout << "    デプスの最小値: " << depth_1 << std::endl;
#endif
		ptr++;
		depth_2 = (float) *ptr/0x7fffffff;

#ifdef DEGUB_SelectHits
		std::cout << "    デプスの最大値: " << depth_2 << std::endl;
#endif
		ptr++;

#ifdef DEGUB_SelectHits
		std::cout << "    識別番号: ";
#endif

		// 最小デプスの確認
		if(depth_min>depth_1){
			depth_min = depth_1;
			// 識別番号を保存
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
	std::cout << std::endl << "デプス最小の識別番号: " << std::endl;
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

//識別したオブジェクトに対する動作
void BoardAdm::SelectDo(int hit_name){

	int x = ReturnX(hit_name);
	int y = ReturnY(hit_name);

	//識別したマスが直接指定してたマスなら
	if( bo->GetDsel(x,y) ){
		bo->ClearFlag();		//操作のキャンセルとみなす
		return;
	}
	//識別したマスが間接指定してたマスなら
	if( bo->GetIsel(x,y) ){
		int tempX = ReturnX(m_select);
		int tempY = ReturnY(m_select);
		
		cgame->Move(x,y,tempX,tempY);			//駒の移動
		bo->ClearFlag();
		return;
	}
	
	//識別したマスに駒がないor相手の駒なら 何もしないor操作のキャンセル
	Player nowP = cgame->CheckPlayer();
	if( bo->GetPiecePlayer(x,y) != nowP  || nowP == EndB || nowP == EndW || nowP == EndST ){
		bo->ClearFlag();
		return;
	}

	//それ以外なら新たなフラグの設定
	m_select = hit_name;						//今セレクトされたオブジェクト番号を保持
	bo->ClearFlag();
	bo->SetDselTrue(x,y);						//選択した駒はDsel

	bo->SetNG(x,y);								//自分が居るマスには動けない
	
	cgame->CheckMove(x,y);						//ゲームルール上動けるかを判断

	bo->ChangeOK();								//最後に残ったok識別子の場所にあるIselフラグをtrueにする

}

//駒表示関数
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

// ChessPiece→ChessPieceModelの変換
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

//どちらの出番かを返す
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