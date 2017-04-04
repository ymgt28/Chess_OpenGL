/*-------------------------------------------
	XLoader.h
	OpenGL用Xファイル読み込みクラスヘッダ群
	by y.y
	2015/01/28		β版
---------------------------------------------*/

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <windows.h>
#include <GL/glut.h>

//クラスプロトタイプ宣言
class XFile;
class XFace;
class XMesh;
class XMaterial;
class XModel;

/*---XVector2構造体---*/
typedef struct{
	float x, y; 
	operator float* () { return (float*)&x; }
	operator const float*() const { return (const float*)&x; }
}XVector2;

/*---XVector3構造体---*/
typedef struct{
	float x, y, z; 
	operator float*() { return (float*)&x; }
	operator const float*() const { return (const float*)&x; }
}XVector3;

/*---XColor構造体---*/
typedef struct{
	float r, g, b, a; 
	operator float*() { return (float*)&r; }
	operator const float*() const { return (const float*)&r; }
}XColor;


/*---XFileクラス---*/
class XFile{
public:
	XFile();									//コンストラクタ
	void LoadFile(const char* fileName);		//ファイル読み込み
	bool CheckToken(const char* str);			//指定された文字列とトークンをチェックする
	void GetToken(void);						//トークンを取得する
	bool GetToken(const char* token);			//トークンを取得し指定された文字列を比較を行う
	float GetFloatToken(void);					//トークンをfloat型に変換し取得する
	int GetIntToken(void);						//トークンをint型に変換し取得する
	void SkipNode(void);						//ノードを飛ばす
	bool isBufEnd(void){return (*m_buf != '\0' ? true : false);}	//バッファが終端に達したかどうか
	char* RetToken(void){return m_token;}		//トークンを返す
private:
	char* m_buf;
	char m_token[1024];
};

/*---XFaceクラス---*/
class XFace{
public :
	XFace &operator = (XFace &ob);
	XFace();
	void SetVertexIndex(int index[]);
	void SetNormalIndex(int index[]);
	void SetTexCoordIndex(int index[]);
	int element;
	int indexMaterial;
	int indexVertices[4];
	int indexNormals[4];
	int indexTexCoords[4];
};

/*---XMaterialクラス---*/
class XMaterial{
public :
	XMaterial &operator = (XMaterial &ob);
	XMaterial();
	void SetName(char *str);
	void SetTextureFileName(char *str);
	char name[MAX_PATH];
	XColor ambient;
	XColor diffuse;
	XColor specular;
	XColor emissive;
	float power;
	char textureFileName[MAX_PATH];
};

/*---XMeshクラス---*/
class XMesh{
public :
	XMesh &operator = (XMesh &ob);
	XMesh();
	int AddVertex(XVector3 &ob);
	int AddNormal(XVector3 &ob);
	int AddTexCoord(XVector2 &ob);
	int AddFace(XFace &ob);
	void Release();
	void SetName(char *str);
	char name[MAX_PATH];
	int numVertices;
	int numNormals;
	int numTexCoords;
	int numFaces;
	XVector3* vertex;
	XVector3* normal;
	XVector2* texcoord;
	XFace* face;
};

/*---XModelクラス---*/
class XModel{
public:
	bool Load(char *filename);
	void Release();
	void Render(int index, float scale=1.0f);
	void Render(float scale=1.0f);
	void Render(float r, float g, float b, float a);
	int AddMesh(XMesh ob);
	int AddMaterial(XMaterial ob);
	XModel();
	int numMeshes;
	int numMaterials;
	XMesh *mesh;
	XMaterial *material;
	XFile data;
};
