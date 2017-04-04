/*-------------------------------------------
	XLoader.h
	OpenGL�pX�t�@�C���ǂݍ��݃N���X�w�b�_�Q
	by y.y
	2015/01/28		����
---------------------------------------------*/

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <windows.h>
#include <GL/glut.h>

//�N���X�v���g�^�C�v�錾
class XFile;
class XFace;
class XMesh;
class XMaterial;
class XModel;

/*---XVector2�\����---*/
typedef struct{
	float x, y; 
	operator float* () { return (float*)&x; }
	operator const float*() const { return (const float*)&x; }
}XVector2;

/*---XVector3�\����---*/
typedef struct{
	float x, y, z; 
	operator float*() { return (float*)&x; }
	operator const float*() const { return (const float*)&x; }
}XVector3;

/*---XColor�\����---*/
typedef struct{
	float r, g, b, a; 
	operator float*() { return (float*)&r; }
	operator const float*() const { return (const float*)&r; }
}XColor;


/*---XFile�N���X---*/
class XFile{
public:
	XFile();									//�R���X�g���N�^
	void LoadFile(const char* fileName);		//�t�@�C���ǂݍ���
	bool CheckToken(const char* str);			//�w�肳�ꂽ������ƃg�[�N�����`�F�b�N����
	void GetToken(void);						//�g�[�N�����擾����
	bool GetToken(const char* token);			//�g�[�N�����擾���w�肳�ꂽ��������r���s��
	float GetFloatToken(void);					//�g�[�N����float�^�ɕϊ����擾����
	int GetIntToken(void);						//�g�[�N����int�^�ɕϊ����擾����
	void SkipNode(void);						//�m�[�h���΂�
	bool isBufEnd(void){return (*m_buf != '\0' ? true : false);}	//�o�b�t�@���I�[�ɒB�������ǂ���
	char* RetToken(void){return m_token;}		//�g�[�N����Ԃ�
private:
	char* m_buf;
	char m_token[1024];
};

/*---XFace�N���X---*/
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

/*---XMaterial�N���X---*/
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

/*---XMesh�N���X---*/
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

/*---XModel�N���X---*/
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
