/*-------------------------------------
	XLoader.cpp
	OpenGL�pX�t�@�C���ǂݍ��݃N���X�Q
	by y.y
	2015/01/28		����
---------------------------------------*/

#include "XLoader.h"

/*---XFile�N���X---*/

//�R���X�g���N�^
XFile::XFile(){
}

//�t�@�C���ǂݍ���
void XFile::LoadFile(const char* fileName){

	//�t�@�C����ǂݍ���
	std::ifstream ifs( fileName );

	//����ɓǂݍ��߂����`�F�b�N
	if ( ifs.fail() ){
		std::cout << "Error : �w�肳�ꂽ�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n";
		std::cout << "File Name : " << fileName << std::endl;
		return;
	}

	//�t�@�C���T�C�Y���擾���A�o�b�t�@�������𓮓I�m��
	ifs.seekg(0, std::fstream::end);			//�t�@�C��������
	unsigned int endPos = ifs.tellg();			//�����C���f�b�N�X�擾
	ifs.seekg(0, std::fstream::beg);			//�t�@�C���擪��
	unsigned int size = endPos - ifs.tellg();	//����-�擪�Ńt�@�C���T�C�Y�擾
	m_buf = new char[size+1];
	memset(m_buf, 0, size);
	m_buf[size] = '\0';

	//�o�b�t�@�Ɋi�[
	ifs.read(m_buf, size);

}

//�w�肳�ꂽ������ƃg�[�N�����`�F�b�N����
bool XFile::CheckToken(const char* str){

	if( strcmp(m_token, str) == 0 )
		return true;
	else
		return false;
}

//�g�[�N�����擾����
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

//�g�[�N�����擾���w�肳�ꂽ��������r���s��
bool XFile::GetToken(const char* token){

	GetToken();
	if( strcmp(m_token, token) != 0 ){
		std::cout << "Error : �z��g�[�N���ƓǍ��g�[�N������v���܂���\n";
		std::cout << "�z��g�[�N���F" << token << std::endl;
		std::cout << "�Ǎ��g�[�N���F" << m_token << std::endl;
		return false;
	}else{
		return true;
	}
}

//�g�[�N����float�^�ɕϊ����擾����
float XFile::GetFloatToken(void){

	GetToken();
	return (float)atof(m_token);
}

//�g�[�N����int�^�ɕϊ����擾����
int XFile::GetIntToken(void){

	GetToken();
	return (int)atoi(m_token);
}

//�m�[�h���΂�
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
		std::cout << "Error : �J�b�R����v���Ă��܂���\n";
		return;
	}
}

/*---XFace�N���X---*/

//�R���X�g���N�^
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

//���_�C���f�b�N�X���Z�b�g����
void XFace::SetVertexIndex(int index[]){

	for(int i=0; i<4; i++)
		indexVertices[i] = index[i];
}

//�@���C���f�b�N�X���Z�b�g����
void XFace::SetNormalIndex(int index[]){

	for(int i=0; i<4; i++)
		indexNormals[i] = index[i];
}

//�e�N�X�`�����W�C���f�b�N�X���Z�b�g����
void XFace::SetTexCoordIndex(int index[]){

	for(int i=0; i<4; i++)
		indexTexCoords[i] = index[i];
}

/*---XMaterial�N���X---*/

//�R���X�g���N�^
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

//���O���Z�b�g����
void XMaterial::SetName(char *str){

	strcpy(name, str);
}

//�e�N�X�`���t�@�C�������Z�b�g����
void XMaterial::SetTextureFileName(char *str){
	
	strcpy(textureFileName, str);
}

/*---XMesh�N���X---*/

//�R���X�g���N�^
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

//���_��ǉ�����
int XMesh::AddVertex(XVector3 &ob){

	numVertices++;
	vertex = (XVector3*)realloc(vertex, numVertices*sizeof(XVector3));
	vertex[numVertices-1] = ob;
	return numVertices;
}

//�@����ǉ�����
int XMesh::AddNormal(XVector3 &ob){

	numNormals++;
	normal = (XVector3*)realloc(normal, numNormals*sizeof(XVector3));
	normal[numNormals-1] = ob;
	return numNormals;
}

//�e�N�X�`�����W��ǉ�����
int XMesh::AddTexCoord(XVector2 &ob){

	numTexCoords++;
	texcoord = (XVector2*)realloc(texcoord, numTexCoords*sizeof(XVector2));
	texcoord[numTexCoords-1] = ob;
	return numTexCoords;
}

//�ʂ�ǉ�����
int XMesh::AddFace(XFace &ob){

	numFaces++;
	face = (XFace*)realloc(face, numFaces*sizeof(XFace));
	face[numFaces-1] = ob;
	return numFaces;
}

//�m�ۂ������������������
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

//���O���Z�b�g����
void XMesh::SetName(char *str){
	strcpy(name, str);
}

/*---XModel�N���X---*/

//�R���X�g���N�^
XModel::XModel(){

	numMeshes = 0;
	numMaterials = 0;
	mesh = (XMesh*)malloc(1*sizeof(XMesh));
	material = (XMaterial*)malloc(1*sizeof(XMaterial));
}

//���b�V����ǉ�����
int XModel::AddMesh(XMesh ob){

	numMeshes++;
	mesh = (XMesh*)realloc(mesh, numMeshes*sizeof(XMesh));
	mesh[numMeshes-1] = ob;
	return numMeshes;
}

//�}�e���A����ǉ�����
int XModel::AddMaterial(XMaterial ob){

	numMaterials++;
	material = (XMaterial*)realloc(material, numMaterials*sizeof(XMaterial));
	material[numMaterials-1] = ob;
	return numMaterials;
}

//�m�ۂ������������������
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

//�e�L�X�g�t�H�[�}�b�g��X�t�@�C�������[�h����
bool XModel::Load(char *filename){

	//�J�E���g�p�ϐ��Ȃ�
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

	while ( data.isBufEnd() ){								//�o�b�t�@��'\0'�ɓ��B����܂Ń��[�v

		data.GetToken();										//�g�[�N�����擾

		if(data.CheckToken("template") ){						//template�̏ꍇ

			data.SkipNode();										//�X�L�b�v����

		}else if(data.CheckToken("Mesh")){						//Mesh�̏ꍇ

			data.GetToken();										//�g�[�N�����擾

			if(!data.CheckToken("{")){								//�g�[�N����" { " �łȂ��ꍇ
				tempMesh.SetName(data.RetToken());						//���O���Z�b�g
				data.GetToken("{");										//�g�[�N�����擾

			}else{													//�g�[�N����" { "�̏ꍇ
				char tempName[MAX_PATH] = "obj";						//���O�����߂�
				sprintf(tempName, "%s%d", tempName, meshCount+1);
				tempMesh.SetName(tempName);								//���O���Z�b�g
			}

			meshCount++;											//���b�V�������J�E���g����

			if(meshCount > 1){										//�J�E���g����1���傫���ꍇ
				for(int i=0; i<faceCount; i++)							//�ʃf�[�^��ǉ�
					tempMesh.AddFace(tempFace[i]);
				
				AddMesh(tempMesh);										//���b�V����ǉ�

				delete [] tempFace;										//�m�ۂ��������������
				tempFace = NULL;
				
				ZeroMemory(&tempMesh, sizeof(tempMesh));				//�[���ɖ߂�
			}

			vertCount = 0;											//���_���̃J�E���^�[��0�ɖ߂�
			vertCount = data.GetIntToken();							//�g�[�N�����璸�_�����擾
			for(int i=0; i<vertCount; i++){							//�g�[�N�����璸�_�f�[�^���擾
				XVector3 tempVertex;
				tempVertex.x = data.GetFloatToken();
				tempVertex.y = data.GetFloatToken();
				tempVertex.z = data.GetFloatToken();
				tempMesh.AddVertex(tempVertex);						//���_�f�[�^��ǉ�
			}

			if(tempMesh.numVertices != vertCount){					//���_�����`�F�b�N
				std::cout << "Error : ���_������v���Ă��܂���\n";
				return false;
			}

			faceCount = 0;											//�ʐ��̃J�E���^�[��0�ɖ߂�
			faceCount = data.GetIntToken();							//�g�[�N������ʐ����擾
			tempFace = new XFace[faceCount];						//���������m��
			for(int i=0; i<faceCount; i++){							//�g�[�N������ʃf�[�^���擾
				int tempIndex[4] = { -1, -1, -1, -1 };
				tempFace[i].element = 0;
				tempFace[i].indexMaterial = -1;
				data.GetToken();										//�g�[�N�����擾
			
				if(data.CheckToken("3")){								//�O�p�`�̏ꍇ
					tempFace[i].element = 3;								//�v�f����3�ɂ���

					for( int j=0; j<3; j++ )								//�g�[�N�����璸�_�C���f�b�N�X���擾
						tempIndex[j] = data.GetIntToken();

					tempIndex[3] = -1;										//4�Ԗڂ̃C���f�b�N�X�ɂ�-1���i�[

					tempFace[i].SetVertexIndex(tempIndex);					//���_�C���f�b�N�X���Z�b�g
					tempFace[i].SetTexCoordIndex(tempIndex);				//�e�N�X�`�����W�C���f�b�N�X���Z�b�g
				
				}else if(data.CheckToken("4")){							//�l�p�`�̏ꍇ
					tempFace[i].element = 4;								//�v�f����4�ɂ���

					for(int j=0; j<4; j++)									//�g�[�N�����璸�_�C���f�b�N�X���擾
						tempIndex[j] = data.GetIntToken();

					tempFace[i].SetVertexIndex(tempIndex);					//���_�C���f�b�N�X���Z�b�g
					tempFace[i].SetTexCoordIndex(tempIndex);				//�e�N�X�`�����W�C���f�b�N�X���Z�b�g
				}
			}
		}else if(data.CheckToken("MeshNormals")){				//MeshNormals�̏ꍇ
			
			data.GetToken("{");										//�g�[�N�����擾
			normCount = 0;											//�@�����̃J�E���^�[��0�ɖ߂�
			normCount = data.GetIntToken();							//�g�[�N������@�������擾
			for(int i=0; i<normCount; i++){							//�g�[�N������@���f�[�^���擾
				XVector3 tempNormal;
				tempNormal.x = data.GetFloatToken();
				tempNormal.y = data.GetFloatToken();
				tempNormal.z = data.GetFloatToken();
				tempMesh.AddNormal(tempNormal);							//�@���f�[�^��ǉ�
			}

			if(tempMesh.numNormals != normCount){					//�@�������`�F�b�N
				std::cout << "Error : �@��������v���Ă��܂���\n";
				return false;
			}

			if(data.GetIntToken() != faceCount)						//�@���C���f�b�N�X�����`�F�b�N
			{
				std::cout << "Error : �ʐ��Ɩ@���C���f�b�N�X������v���Ă��܂���\n";
				return false;
			}

			for(int i=0; i<faceCount; i++){

				int tempIndex[4] = { -1, -1, -1, -1 };
				data.GetToken();										//�g�[�N�����擾

				if(data.CheckToken("3") ){								//�O�p�`�̏ꍇ

					for(int j=0; j<3; j++)									//�g�[�N������@���C���f�b�N�X���擾
						tempIndex[j] = data.GetIntToken();
					
					tempIndex[3] = -1;										//4�Ԗڂ̃C���f�b�N�X�ɂ�-1���Z�b�g
					tempFace[i].SetNormalIndex(tempIndex);					//�@���C���f�b�N�X���Z�b�g
				
				}else if(data.CheckToken("4")){							//�l�p�`�̏ꍇ

					for(int j=0; j<4; j++)									//�@���C���f�b�N�X���擾
						tempIndex[j] = data.GetIntToken();
					
					tempFace[i].SetNormalIndex(tempIndex);					//�@���C���f�b�N�X���Z�b�g
				}
			}
		}else if(data.CheckToken("MeshTextureCoords")){				//MeshTextureCoords�̏ꍇ
			
			data.GetToken("{");											//�g�[�N�����擾
			uvCount = 0;												//�e�N�X�`�����W���̃J�E���^�[��0�ɖ߂�
			uvCount = data.GetIntToken();								//�g�[�N������e�N�X�`�����W�����擾
			for(int i=0; i<uvCount; i++){								//�g�[�N������e�N�X�`�����W�f�[�^���擾
				XVector2 tempUV;
				tempUV.x = data.GetFloatToken();
				tempUV.y = data.GetFloatToken();
				tempMesh.AddTexCoord(tempUV);								//�e�N�X�`�����W�f�[�^��ǉ�
			}
		}else if(data.CheckToken("MeshMaterialList")){				//MeshMaterialList�̏ꍇ
			
			data.GetToken("{");											//�g�[�N�����擾
			matCount = 0;												//�}�e���A�����̃J�E���^�[��0�ɖ߂�
			matCount = data.GetIntToken();								//�g�[�N������}�e���A�������擾
			if(data.GetIntToken() != faceCount){						//�}�e���A�������`�F�b�N
				std::cout << "Error : �ʐ��ƃ}�e���A�����X�g������v���܂���\n";
				return false;
			}
			for(int i=0; i<faceCount; i++)								//�g�[�N������}�e���A���C���f�b�N�X���擾
				tempFace[i].indexMaterial = data.GetIntToken();
			
			for(int i=0; i<matCount; i++){

				XMaterial tempMaterial;
				data.GetToken("Material");									//�g�[�N�����擾
				data.GetToken();

				if(!data.CheckToken("{")){									//�g�[�N����" { "�łȂ��ꍇ
					
					tempMaterial.SetName(data.RetToken());						//���O���Z�b�g
					data.GetToken("{");											//�g�[�N�����擾
				
				}else{														//�g�[�N����" { "�̏ꍇ

					char tempName[MAX_PATH] = "mat";							//���O�����߂�
					sprintf(tempName, "%s%d", tempName, i+1);
					tempMaterial.SetName(tempName);								//���O���Z�b�g
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

				data.GetToken();											//�g�[�N�����擾

				if(data.CheckToken("TextureFileName")){						//TextureFileName�̏ꍇ
					
					data.GetToken("{");											//�g�[�N�����擾
					data.GetToken();											//�g�[�N�����擾
					tempMaterial.SetTextureFileName(data.RetToken());			//�t�@�C�������Z�b�g
					data.GetToken("}");											//�g�[�N�����擾
				
				}else if(data.CheckToken("}")){								//�g�[�N����" } "�̏ꍇ
					AddMaterial(tempMaterial);									//�}�e���A���f�[�^��ǉ�
				}
			}
		}
	}

	if(meshCount >= 1){										//���b�V������1�ȏ�̏ꍇ
	
		for ( int i=0; i<faceCount; i++ )					//�ʃf�[�^��ǉ�
			tempMesh.AddFace(tempFace[i]);
		
		AddMesh(tempMesh);									//���b�V���f�[�^��ǉ�

		delete [] tempFace;									//�m�ۂ��������������
		tempFace = NULL;

	}
	return true;
}

//���b�V���̃C���f�b�N�X���w�肵�ĕ`��
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
		
		if(exist_material){						//�}�e���A�������݂���ꍇ
			
			cur_mat = mesh[index].face[i].indexMaterial;	//�}�e���A���C���f�b�N�X���擾

			if(cur_mat != pre_mat){							//�}�e���A���C���f�b�N�X���قȂ�ꍇ
				
				//��{�F
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

				pre_mat = cur_mat;								//�}�e���A���C���f�b�N�X���X�V
			}
		}

		if(mesh[index].face[i].element == 3)						//�v�f����3�̏ꍇ
			glBegin(GL_TRIANGLES);
		else if(mesh[index].face[i].element == 4)					//�v�f����4�̏ꍇ
			glBegin(GL_QUADS);

		for(int j=0; j<mesh[index].face[i].element; j++){
			
			if ( exist_texcoord )										//�e�N�X�`�����W�����݂���ꍇ
				glTexCoord2fv(mesh[index].texcoord[mesh[index].face[i].indexTexCoords[j]]);

			if ( exist_normal )											//�@���x�N�g�������݂���ꍇ
				glNormal3fv(mesh[index].normal[mesh[index].face[i].indexNormals[j]]);
			
			//���_���W
			glVertex3f(
				mesh[index].vertex[mesh[index].face[i].indexVertices[j]].x * scale,
				mesh[index].vertex[mesh[index].face[i].indexVertices[j]].y * scale,
				mesh[index].vertex[mesh[index].face[i].indexVertices[j]].z * scale );
		}

		glEnd();
	}
}

//�`�揈��
void XModel::Render(float scale){

	for(int i=0; i<numMeshes; i++){
		Render(i, scale);
	}
}

//�F���w�肵���`��
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
		
		if(exist_material){						//�}�e���A�������݂���ꍇ
			
			cur_mat = mesh[0].face[i].indexMaterial;	//�}�e���A���C���f�b�N�X���擾

			if(cur_mat != pre_mat){							//�}�e���A���C���f�b�N�X���قȂ�ꍇ
				
				//��{�F
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

				pre_mat = cur_mat;								//�}�e���A���C���f�b�N�X���X�V
			}
		}

		if(mesh[0].face[i].element == 3)						//�v�f����3�̏ꍇ
			glBegin(GL_TRIANGLES);
		else if(mesh[0].face[i].element == 4)					//�v�f����4�̏ꍇ
			glBegin(GL_QUADS);

		for(int j=0; j<mesh[0].face[i].element; j++){
			
			if ( exist_texcoord )										//�e�N�X�`�����W�����݂���ꍇ
				glTexCoord2fv(mesh[0].texcoord[mesh[0].face[i].indexTexCoords[j]]);

			if ( exist_normal )											//�@���x�N�g�������݂���ꍇ
				glNormal3fv(mesh[0].normal[mesh[0].face[i].indexNormals[j]]);
			
			//���_���W
			glVertex3f(
				mesh[0].vertex[mesh[0].face[i].indexVertices[j]].x * scale,
				mesh[0].vertex[mesh[0].face[i].indexVertices[j]].y * scale,
				mesh[0].vertex[mesh[0].face[i].indexVertices[j]].z * scale );
		}

		glEnd();
	}
}