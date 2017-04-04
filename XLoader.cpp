/*-------------------------------------
	XLoader.cpp
	OpenGL用Xファイル読み込みクラス群
	by y.y
	2015/01/28		β版
---------------------------------------*/

#include "XLoader.h"

/*---XFileクラス---*/

//コンストラクタ
XFile::XFile(){
}

//ファイル読み込み
void XFile::LoadFile(const char* fileName){

	//ファイルを読み込む
	std::ifstream ifs( fileName );

	//正常に読み込めたかチェック
	if ( ifs.fail() ){
		std::cout << "Error : 指定されたファイルの読み込みに失敗しました\n";
		std::cout << "File Name : " << fileName << std::endl;
		return;
	}

	//ファイルサイズを取得し、バッファメモリを動的確保
	ifs.seekg(0, std::fstream::end);			//ファイル末尾へ
	unsigned int endPos = ifs.tellg();			//末尾インデックス取得
	ifs.seekg(0, std::fstream::beg);			//ファイル先頭へ
	unsigned int size = endPos - ifs.tellg();	//末尾-先頭でファイルサイズ取得
	m_buf = new char[size+1];
	memset(m_buf, 0, size);
	m_buf[size] = '\0';

	//バッファに格納
	ifs.read(m_buf, size);

}

//指定された文字列とトークンをチェックする
bool XFile::CheckToken(const char* str){

	if( strcmp(m_token, str) == 0 )
		return true;
	else
		return false;
}

//トークンを取得する
void XFile::GetToken(void){
	
	char *p = m_buf;
	char *q = m_token;

	while( *p != '\0' && strchr(" \t\r\n,;\"", *p) ) 
		p++;

	if( *p == '{' || *p == '}' )
		(*q++) = (*p++);
	else
		while( *p != '\0' && !strchr(" \t\r\n,;\"{}", *p ) )
			(*q++) = (*p++);

	m_buf = p;
	*q = '\0';
}

//トークンを取得し指定された文字列を比較を行う
bool XFile::GetToken(const char* token){

	GetToken();
	if( strcmp(m_token, token) != 0 ){
		std::cout << "Error : 想定トークンと読込トークンが一致しません\n";
		std::cout << "想定トークン：" << token << std::endl;
		std::cout << "読込トークン：" << m_token << std::endl;
		return false;
	}else{
		return true;
	}
}

//トークンをfloat型に変換し取得する
float XFile::GetFloatToken(void){

	GetToken();
	return (float)atof(m_token);
}

//トークンをint型に変換し取得する
int XFile::GetIntToken(void){

	GetToken();
	return (int)atoi(m_token);
}

//ノードを飛ばす
void XFile::SkipNode(void){

	while( *m_buf != '\0' ){
		GetToken();
		if ( strchr(m_token, '{' ) )
			break;
	}
	int count = 1;
	while( *m_buf != '\0' && count > 0 ){
		GetToken();
		if ( strchr(m_token, '{' ) )
			count++;
		else if ( strchr(m_token, '}') )
			count--;
	}
	if( count > 0 ){
		std::cout << "Error : カッコが一致していません\n";
		return;
	}
}

/*---XFaceクラス---*/

//コンストラクタ
XFace::XFace(){

	element = 0;
	indexMaterial = -1;
	for(int i=0; i<4; i++){
		indexVertices[i] = -1;
		indexNormals[i] = -1;
		indexTexCoords[i] = -1;
	}
}

// operator =
XFace& XFace::operator =(XFace &ob){

	element = ob.element;
	indexMaterial = ob.indexMaterial;
	for(int i=0; i<4; i++){
		indexVertices[i] = ob.indexVertices[i];
		indexNormals[i] = ob.indexNormals[i];
		indexTexCoords[i] = ob.indexTexCoords[i];
	}
	return (*this);
}

//頂点インデックスをセットする
void XFace::SetVertexIndex(int index[]){

	for(int i=0; i<4; i++)
		indexVertices[i] = index[i];
}

//法線インデックスをセットする
void XFace::SetNormalIndex(int index[]){

	for(int i=0; i<4; i++)
		indexNormals[i] = index[i];
}

//テクスチャ座標インデックスをセットする
void XFace::SetTexCoordIndex(int index[]){

	for(int i=0; i<4; i++)
		indexTexCoords[i] = index[i];
}

/*---XMaterialクラス---*/

//コンストラクタ
XMaterial::XMaterial(){

	strcpy(name, "null");
	ambient.r = 0.0f;	ambient.g = 0.0f;	ambient.b = 0.0f;	ambient.a = 0.0f;
	diffuse.r = 0.0f;	diffuse.g = 0.0f;	diffuse.b = 0.0f;	diffuse.a = 0.0f;
	specular.r = 0.0f;	specular.g = 0.0f;	specular.b = 0.0f;	specular.a = 0.0f;
	emissive.r = 0.0f;	emissive.g = 0.0f;	emissive.g = 0.0f;	emissive.a = 0.0f;
	power = 0.0f;
	strcpy(textureFileName, "null");
}

// operator =
XMaterial& XMaterial::operator =(XMaterial &ob){

	strcpy(name, ob.name);
	ambient = ob.ambient;
	diffuse = ob.diffuse;
	specular = ob.specular;
	emissive = ob.emissive;
	power = ob.power;
	strcpy(textureFileName, ob.textureFileName);
	return (*this);
}

//名前をセットする
void XMaterial::SetName(char *str){

	strcpy(name, str);
}

//テクスチャファイル名をセットする
void XMaterial::SetTextureFileName(char *str){
	
	strcpy(textureFileName, str);
}

/*---XMeshクラス---*/

//コンストラクタ
XMesh::XMesh(){

	strcpy(name, "null");
	numVertices = 0;
	numNormals = 0;
	numTexCoords = 0;
	numFaces = 0;
	vertex = (XVector3*)malloc(1*sizeof(XVector3));
	normal = (XVector3*)malloc(1*sizeof(XVector3));
	texcoord = (XVector2*)malloc(1*sizeof(XVector2));
	face = (XFace*)malloc(1*sizeof(XFace));

}

// operator =
XMesh& XMesh::operator= (XMesh &ob){

	strcpy(name, ob.name);
	numVertices = ob.numVertices;
	numNormals = ob.numNormals;
	numTexCoords = ob.numTexCoords;
	numFaces = ob.numFaces;

	vertex = (XVector3*)malloc(numVertices*sizeof(XVector3));
	normal = (XVector3*)malloc(numNormals*sizeof(XVector3));
	texcoord = (XVector2*)malloc(numTexCoords*sizeof(XVector2));
	face = (XFace*)malloc(numFaces*sizeof(XFace));

	for(int i=0; i<numVertices; i++)
		vertex[i] = ob.vertex[i];

	for(int i=0; i<numNormals; i++)
		normal[i] = ob.normal[i];

	for(int i=0; i<numTexCoords; i++)
		texcoord[i] = ob.texcoord[i];

	for(int i=0; i<numFaces; i++)
		face[i] = ob.face[i];

	
	return (*this);
}

//頂点を追加する
int XMesh::AddVertex(XVector3 &ob){

	numVertices++;
	vertex = (XVector3*)realloc(vertex, numVertices*sizeof(XVector3));
	vertex[numVertices-1] = ob;
	return numVertices;
}

//法線を追加する
int XMesh::AddNormal(XVector3 &ob){

	numNormals++;
	normal = (XVector3*)realloc(normal, numNormals*sizeof(XVector3));
	normal[numNormals-1] = ob;
	return numNormals;
}

//テクスチャ座標を追加する
int XMesh::AddTexCoord(XVector2 &ob){

	numTexCoords++;
	texcoord = (XVector2*)realloc(texcoord, numTexCoords*sizeof(XVector2));
	texcoord[numTexCoords-1] = ob;
	return numTexCoords;
}

//面を追加する
int XMesh::AddFace(XFace &ob){

	numFaces++;
	face = (XFace*)realloc(face, numFaces*sizeof(XFace));
	face[numFaces-1] = ob;
	return numFaces;
}

//確保したメモリを解放する
void XMesh::Release(){

	if(vertex){
		free((XVector3*)vertex);
		vertex = NULL;
	}

	if(normal){
		free((XVector3*)normal);
		normal = NULL;
	}

	if(texcoord){
		free((XVector2*)texcoord);
		texcoord = NULL;
	}

	if(face){
		free((XFace*)face);
		face = NULL;
	}

	numVertices = 0;
	numNormals = 0;
	numTexCoords = 0;
	numFaces = 0;
}

//名前をセットする
void XMesh::SetName(char *str){
	strcpy(name, str);
}

/*---XModelクラス---*/

//コンストラクタ
XModel::XModel(){

	numMeshes = 0;
	numMaterials = 0;
	mesh = (XMesh*)malloc(1*sizeof(XMesh));
	material = (XMaterial*)malloc(1*sizeof(XMaterial));
}

//メッシュを追加する
int XModel::AddMesh(XMesh ob){

	numMeshes++;
	mesh = (XMesh*)realloc(mesh, numMeshes*sizeof(XMesh));
	mesh[numMeshes-1] = ob;
	return numMeshes;
}

//マテリアルを追加する
int XModel::AddMaterial(XMaterial ob){

	numMaterials++;
	material = (XMaterial*)realloc(material, numMaterials*sizeof(XMaterial));
	material[numMaterials-1] = ob;
	return numMaterials;
}

//確保したメモリを解放する
void XModel::Release(){

	if(material){
		free((XMaterial*)material);
		material = NULL;
	}
	if(mesh){
		free((XMesh*)mesh);
		mesh = NULL;
	}
	numMaterials = 0;
	numMeshes = 0;
}

//テキストフォーマットのXファイルをロードする
bool XModel::Load(char *filename){

	//カウント用変数など
	bool b_Mesh = false;
	int matCount = 0;
	int vertCount = 0;
	int faceCount = 0;
	int normCount = 0;
	int uvCount = 0;
	int meshCount = 0;
	
	XFace* tempFace;
	XMesh tempMesh;

	data.LoadFile(filename);

	while ( data.isBufEnd() ){								//バッファが'\0'に到達するまでループ

		data.GetToken();										//トークンを取得

		if(data.CheckToken("template") ){						//templateの場合

			data.SkipNode();										//スキップする

		}else if(data.CheckToken("Mesh")){						//Meshの場合

			data.GetToken();										//トークンを取得

			if(!data.CheckToken("{")){								//トークンが" { " でない場合
				tempMesh.SetName(data.RetToken());						//名前をセット
				data.GetToken("{");										//トークンを取得

			}else{													//トークンが" { "の場合
				char tempName[MAX_PATH] = "obj";						//名前を決める
				sprintf(tempName, "%s%d", tempName, meshCount+1);
				tempMesh.SetName(tempName);								//名前をセット
			}

			meshCount++;											//メッシュ数をカウントする

			if(meshCount > 1){										//カウント数が1より大きい場合
				for(int i=0; i<faceCount; i++)							//面データを追加
					tempMesh.AddFace(tempFace[i]);
				
				AddMesh(tempMesh);										//メッシュを追加

				delete [] tempFace;										//確保したメモリを解放
				tempFace = NULL;
				
				ZeroMemory(&tempMesh, sizeof(tempMesh));				//ゼロに戻す
			}

			vertCount = 0;											//頂点数のカウンターを0に戻す
			vertCount = data.GetIntToken();							//トークンから頂点数を取得
			for(int i=0; i<vertCount; i++){							//トークンから頂点データを取得
				XVector3 tempVertex;
				tempVertex.x = data.GetFloatToken();
				tempVertex.y = data.GetFloatToken();
				tempVertex.z = data.GetFloatToken();
				tempMesh.AddVertex(tempVertex);						//頂点データを追加
			}

			if(tempMesh.numVertices != vertCount){					//頂点数をチェック
				std::cout << "Error : 頂点数が一致していません\n";
				return false;
			}

			faceCount = 0;											//面数のカウンターを0に戻す
			faceCount = data.GetIntToken();							//トークンから面数を取得
			tempFace = new XFace[faceCount];						//メモリを確保
			for(int i=0; i<faceCount; i++){							//トークンから面データを取得
				int tempIndex[4] = { -1, -1, -1, -1 };
				tempFace[i].element = 0;
				tempFace[i].indexMaterial = -1;
				data.GetToken();										//トークンを取得
			
				if(data.CheckToken("3")){								//三角形の場合
					tempFace[i].element = 3;								//要素数は3にする

					for( int j=0; j<3; j++ )								//トークンから頂点インデックスを取得
						tempIndex[j] = data.GetIntToken();

					tempIndex[3] = -1;										//4番目のインデックスには-1を格納

					tempFace[i].SetVertexIndex(tempIndex);					//頂点インデックスをセット
					tempFace[i].SetTexCoordIndex(tempIndex);				//テクスチャ座標インデックスをセット
				
				}else if(data.CheckToken("4")){							//四角形の場合
					tempFace[i].element = 4;								//要素数は4にする

					for(int j=0; j<4; j++)									//トークンから頂点インデックスを取得
						tempIndex[j] = data.GetIntToken();

					tempFace[i].SetVertexIndex(tempIndex);					//頂点インデックスをセット
					tempFace[i].SetTexCoordIndex(tempIndex);				//テクスチャ座標インデックスをセット
				}
			}
		}else if(data.CheckToken("MeshNormals")){				//MeshNormalsの場合
			
			data.GetToken("{");										//トークンを取得
			normCount = 0;											//法線数のカウンターを0に戻す
			normCount = data.GetIntToken();							//トークンから法線数を取得
			for(int i=0; i<normCount; i++){							//トークンから法線データを取得
				XVector3 tempNormal;
				tempNormal.x = data.GetFloatToken();
				tempNormal.y = data.GetFloatToken();
				tempNormal.z = data.GetFloatToken();
				tempMesh.AddNormal(tempNormal);							//法線データを追加
			}

			if(tempMesh.numNormals != normCount){					//法線数をチェック
				std::cout << "Error : 法線数が一致していません\n";
				return false;
			}

			if(data.GetIntToken() != faceCount)						//法線インデックス数をチェック
			{
				std::cout << "Error : 面数と法線インデックス数が一致していません\n";
				return false;
			}

			for(int i=0; i<faceCount; i++){

				int tempIndex[4] = { -1, -1, -1, -1 };
				data.GetToken();										//トークンを取得

				if(data.CheckToken("3") ){								//三角形の場合

					for(int j=0; j<3; j++)									//トークンから法線インデックスを取得
						tempIndex[j] = data.GetIntToken();
					
					tempIndex[3] = -1;										//4番目のインデックスには-1をセット
					tempFace[i].SetNormalIndex(tempIndex);					//法線インデックスをセット
				
				}else if(data.CheckToken("4")){							//四角形の場合

					for(int j=0; j<4; j++)									//法線インデックスを取得
						tempIndex[j] = data.GetIntToken();
					
					tempFace[i].SetNormalIndex(tempIndex);					//法線インデックスをセット
				}
			}
		}else if(data.CheckToken("MeshTextureCoords")){				//MeshTextureCoordsの場合
			
			data.GetToken("{");											//トークンを取得
			uvCount = 0;												//テクスチャ座標数のカウンターを0に戻す
			uvCount = data.GetIntToken();								//トークンからテクスチャ座標数を取得
			for(int i=0; i<uvCount; i++){								//トークンからテクスチャ座標データを取得
				XVector2 tempUV;
				tempUV.x = data.GetFloatToken();
				tempUV.y = data.GetFloatToken();
				tempMesh.AddTexCoord(tempUV);								//テクスチャ座標データを追加
			}
		}else if(data.CheckToken("MeshMaterialList")){				//MeshMaterialListの場合
			
			data.GetToken("{");											//トークンを取得
			matCount = 0;												//マテリアル数のカウンターを0に戻す
			matCount = data.GetIntToken();								//トークンからマテリアル数を取得
			if(data.GetIntToken() != faceCount){						//マテリアル数をチェック
				std::cout << "Error : 面数とマテリアルリスト数が一致しません\n";
				return false;
			}
			for(int i=0; i<faceCount; i++)								//トークンからマテリアルインデックスを取得
				tempFace[i].indexMaterial = data.GetIntToken();
			
			for(int i=0; i<matCount; i++){

				XMaterial tempMaterial;
				data.GetToken("Material");									//トークンを取得
				data.GetToken();

				if(!data.CheckToken("{")){									//トークンが" { "でない場合
					
					tempMaterial.SetName(data.RetToken());						//名前をセット
					data.GetToken("{");											//トークンを取得
				
				}else{														//トークンが" { "の場合

					char tempName[MAX_PATH] = "mat";							//名前を決める
					sprintf(tempName, "%s%d", tempName, i+1);
					tempMaterial.SetName(tempName);								//名前をセット
				}

				//Ambient Color
				tempMaterial.ambient.r = 0.15f;
				tempMaterial.ambient.g = 0.15f;
				tempMaterial.ambient.g = 0.15f;
				tempMaterial.ambient.a = 1.0f;

				//Diffuse Color
				tempMaterial.diffuse.r = data.GetFloatToken();
				tempMaterial.diffuse.g = data.GetFloatToken();
				tempMaterial.diffuse.b = data.GetFloatToken();
				tempMaterial.diffuse.a = data.GetFloatToken();

				//Shiness
				tempMaterial.power = data.GetFloatToken();

				//Specular Color
				tempMaterial.specular.r = data.GetFloatToken();
				tempMaterial.specular.g = data.GetFloatToken();
				tempMaterial.specular.b = data.GetFloatToken();
				tempMaterial.specular.a = 1.0f;

				//Emissive Color
				tempMaterial.emissive.r = data.GetFloatToken();
				tempMaterial.emissive.g = data.GetFloatToken();
				tempMaterial.emissive.b = data.GetFloatToken();
				tempMaterial.emissive.a = 1.0f;

				data.GetToken();											//トークンを取得

				if(data.CheckToken("TextureFileName")){						//TextureFileNameの場合
					
					data.GetToken("{");											//トークンを取得
					data.GetToken();											//トークンを取得
					tempMaterial.SetTextureFileName(data.RetToken());			//ファイル名をセット
					data.GetToken("}");											//トークンを取得
				
				}else if(data.CheckToken("}")){								//トークンが" } "の場合
					AddMaterial(tempMaterial);									//マテリアルデータを追加
				}
			}
		}
	}

	if(meshCount >= 1){										//メッシュ数が1以上の場合
	
		for ( int i=0; i<faceCount; i++ )					//面データを追加
			tempMesh.AddFace(tempFace[i]);
		
		AddMesh(tempMesh);									//メッシュデータを追加

		delete [] tempFace;									//確保したメモリを解放
		tempFace = NULL;

	}
	return true;
}

//メッシュのインデックスを指定して描画
void XModel::Render(int index, float scale){

	int pre_mat = -1;
	int cur_mat = 0;
	bool exist_material = false;
	bool exist_texcoord = false;
	bool exist_normal = false;

	if(numMaterials > 0)
		exist_material = true;
	if(mesh[index].numTexCoords > 0)
		exist_texcoord = true;
	if(mesh[index].numNormals > 0)
		exist_normal = true;

	for(int i=0; i<mesh[index].numFaces; i++){
		
		if(exist_material){						//マテリアルが存在する場合
			
			cur_mat = mesh[index].face[i].indexMaterial;	//マテリアルインデックスを取得

			if(cur_mat != pre_mat){							//マテリアルインデックスが異なる場合
				
				//基本色
				glColor4fv(material[cur_mat].diffuse);

				//Ambient Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material[cur_mat].ambient);

				//Diffuse Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material[cur_mat].diffuse);

				//Specular Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material[cur_mat].specular);

				//Emissive Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material[cur_mat].emissive);

				//Shininess
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[cur_mat].power);

				pre_mat = cur_mat;								//マテリアルインデックスを更新
			}
		}

		if(mesh[index].face[i].element == 3)						//要素数が3の場合
			glBegin(GL_TRIANGLES);
		else if(mesh[index].face[i].element == 4)					//要素数が4の場合
			glBegin(GL_QUADS);

		for(int j=0; j<mesh[index].face[i].element; j++){
			
			if ( exist_texcoord )										//テクスチャ座標が存在する場合
				glTexCoord2fv(mesh[index].texcoord[mesh[index].face[i].indexTexCoords[j]]);

			if ( exist_normal )											//法線ベクトルが存在する場合
				glNormal3fv(mesh[index].normal[mesh[index].face[i].indexNormals[j]]);
			
			//頂点座標
			glVertex3f(
				mesh[index].vertex[mesh[index].face[i].indexVertices[j]].x * scale,
				mesh[index].vertex[mesh[index].face[i].indexVertices[j]].y * scale,
				mesh[index].vertex[mesh[index].face[i].indexVertices[j]].z * scale );
		}

		glEnd();
	}
}

//描画処理
void XModel::Render(float scale){

	for(int i=0; i<numMeshes; i++){
		Render(i, scale);
	}
}

//色を指定した描画
void XModel::Render(float r, float g, float b, float a){

	XColor tempDiffuse;
	tempDiffuse.r = r;
	tempDiffuse.g = g;
	tempDiffuse.b = b;
	tempDiffuse.a = a;
	float scale=1.0f;
	int pre_mat = -1;
	int cur_mat = 0;
	bool exist_material = false;
	bool exist_texcoord = false;
	bool exist_normal = false;

	if(numMaterials > 0)
		exist_material = true;
	if(mesh[0].numTexCoords > 0)
		exist_texcoord = true;
	if(mesh[0].numNormals > 0)
		exist_normal = true;

	for(int i=0; i<mesh[0].numFaces; i++){
		
		if(exist_material){						//マテリアルが存在する場合
			
			cur_mat = mesh[0].face[i].indexMaterial;	//マテリアルインデックスを取得

			if(cur_mat != pre_mat){							//マテリアルインデックスが異なる場合
				
				//基本色
				glColor4fv(tempDiffuse);

				//Ambient Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material[cur_mat].ambient);

				//Diffuse Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tempDiffuse);

				//Specular Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material[cur_mat].specular);

				//Emissive Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material[cur_mat].emissive);

				//Shininess
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[cur_mat].power);

				pre_mat = cur_mat;								//マテリアルインデックスを更新
			}
		}

		if(mesh[0].face[i].element == 3)						//要素数が3の場合
			glBegin(GL_TRIANGLES);
		else if(mesh[0].face[i].element == 4)					//要素数が4の場合
			glBegin(GL_QUADS);

		for(int j=0; j<mesh[0].face[i].element; j++){
			
			if ( exist_texcoord )										//テクスチャ座標が存在する場合
				glTexCoord2fv(mesh[0].texcoord[mesh[0].face[i].indexTexCoords[j]]);

			if ( exist_normal )											//法線ベクトルが存在する場合
				glNormal3fv(mesh[0].normal[mesh[0].face[i].indexNormals[j]]);
			
			//頂点座標
			glVertex3f(
				mesh[0].vertex[mesh[0].face[i].indexVertices[j]].x * scale,
				mesh[0].vertex[mesh[0].face[i].indexVertices[j]].y * scale,
				mesh[0].vertex[mesh[0].face[i].indexVertices[j]].z * scale );
		}

		glEnd();
	}
}