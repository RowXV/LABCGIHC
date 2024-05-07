/*
Proyecto
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


//Texturas a utilizar en entorno opengl
//Texture "nombre";
Texture pisoTexture;
Texture banca1Texture; //textura de la banca

//Modelos a utilizar en entorno opengl
//Model "nombre";

//Objetos
Model Lifmunk;
Model PalBall;
Model Mineral;
Model FruHa;
Model Orbe;
Model Pollo;
Model Oveja;

Model Motobug;
Model Crabmeat;
Model Palmera;
Model Checkpoint;
Model Joya;
Model Cofre;
Model CajaAnillo;
Model Oro;
Model Baculo;
Model HongoM;

//Vehiculos

//Vochito
Model VCha;
Model VCof;
Model VLlDDe;
Model VLlDIz;
Model VLlTDe;
Model VLlTIz;

//Autobus
Model ACha;
Model ALlDDe;
Model ALlDIz;
Model ALlTDe;
Model ALlTIz;

//Moto
Model MCha;
Model MLl;

//Personaje
//Depresso
Model DCu;
Model DBDe;
Model DBIz;
Model DPDe;
Model DPIz;

//Sonic
Model SCu;
Model SBDe;
Model SBIz;
Model SPDe;
Model SPIz;


//Edificacion
Model PSol;
Model PLuna;
Model Altar;

//Skybox a utilizar en entorno opengl
//Skybox "nombre";
Skybox skybox;

//Materiales a utilizar en entorno opengl
//Material "nombre";
Material Material_brillante;
Material Material_opaco;

Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	//0
		10.0f, 0.0f, -10.0f,	1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	//1
		-10.0f, 0.0f, 10.0f,	0.0f, 0.0f,	0.0f, -1.0f, 0.0f,	//2
		10.0f, 0.0f, 10.0f,		1.0f, 0.0f,	0.0f, -1.0f, 0.0f	//3
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

//Creacion de banca con primitivas OpenGl
void CrearBanca1()
{
	unsigned int banca_indices[] = {

		//LADO IZQUIERDO
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,

		//LADO DERECHO
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		36, 37, 38,
		39, 40, 41,
		42, 43, 44,
		45, 46, 47,

		//PARTE DE ARRIBA
		48, 49, 50,
		51, 52, 53,
		54, 55, 56,
		57, 58, 59,
		60, 61, 62,
		63, 64, 65,
		66, 67, 68,
		69, 70, 71,

		72, 73, 74, //tapa de arriba
		75, 76, 77,
		78, 79, 80, //tapa de abajo
		81, 82, 83,
	};


	GLfloat banca_vertices[] = {
		// S y T se modifican para las coordenadas de la imagen que se mostrara 

		// LADO IZQUIERDO
		// CARA FRONTAL
		//x		y		z			S		T			NX		NY		NZ
		-4.0f,	0.0f,	1.0f,		0.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//0
		-3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		-4.0f,	3.0f,	1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2

		-3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		-4.0f,	3.0f,	1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2
		-3.0f,	3.0f,	1.0f,	    1.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//3

		// CARA IZQUIERDA
		-4.0f,	0.0f,	1.0f,		0.0f,   1.0f,		1.0f,	0.0f,	0.0f,	//0
		-4.0f,	0.0f,	-1.0f,		0.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//4
		-4.0f,	3.0f,	-1.0f,		1.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//6

		-4.0f,	3.0f,	-1.0f,		1.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//6
		-4.0f,	3.0f,	1.0f,		1.0f,	1.0f,		1.0f,	0.0f,	0.0f,	//2
		-4.0f,	0.0f,	1.0f,		0.0f,   1.0f,		1.0f,	0.0f,	0.0f,	//0

		// CARA DERECHA
		-3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//1
		-3.0f,	0.0f,	-1.0f,	    0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//5
		-3.0f,	3.0f,	-1.0f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	//7

		-3.0f,	3.0f,	-1.0f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	//7
		-3.0f,	3.0f,	1.0f,	    1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	//3
		-3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//1

		// CARA TRASERA
		-3.0f,	0.0f,	-1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5
		-4.0f,	0.0f,	-1.0f,		0.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//4
		-4.0f,	3.0f,	-1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//6

		-4.0f,	3.0f,	-1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//6
		-3.0f,	3.0f,	-1.0f,	    1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//7
		-3.0f,	0.0f,	-1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5

		// LADO DERECHO
		// CARA FRONTAL
		//x		y		z			S		T			NX		NY		NZ
		4.0f,	0.0f,	1.0f,		0.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//0
		3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		4.0f,	3.0f,	1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2

		3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		4.0f,	3.0f,	1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2
		3.0f,	3.0f,	1.0f,	    1.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//3

		// CARA IZQUIERDA
		4.0f,	0.0f,	1.0f,		0.0f,   1.0f,		-1.0f,	0.0f,	0.0f,	//0
		4.0f,	0.0f,	-1.0f,		0.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//4
		4.0f,	3.0f,	-1.0f,		1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//6

		4.0f,	3.0f,	-1.0f,		1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//6
		4.0f,	3.0f,	1.0f,		1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	//2
		4.0f,	0.0f,	1.0f,		0.0f,   1.0f,		-1.0f,	0.0f,	0.0f,	//0

		// CARA DERECHA
		3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//1
		3.0f,	0.0f,	-1.0f,	    0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//5
		3.0f,	3.0f,	-1.0f,	    0.0f,	1.0f,		1.0f,	0.0f,	0.0f,	//7

		3.0f,	3.0f,	-1.0f,	    0.0f,	1.0f,		1.0f,	0.0f,	0.0f,	//7
		3.0f,	3.0f,	1.0f,	    1.0f,	1.0f,		1.0f,	0.0f,	0.0f,	//3
		3.0f,	0.0f,	1.0f,	    1.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//1

		// CARA TRASERA
		3.0f,	0.0f,	-1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5
		4.0f,	0.0f,	-1.0f,		0.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//4
		4.0f,	3.0f,	-1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//6

		4.0f,	3.0f,	-1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//6
		3.0f,	3.0f,	-1.0f,	    1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//7
		3.0f,	0.0f,	-1.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5


		// PARTE SUPERIOR
		// CARA FRONTAL
		//x		y		z			S		T			NX		NY		NZ
		-5.0f,	3.0f,	2.0f,		0.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//0
		5.0f,	3.0f,	2.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		-5.0f,	4.0f,	2.0f,		0.0f,	0.2f,		0.0f,	0.0f,	-1.0f,	//2

		5.0f,	3.0f,	2.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		-5.0f,	4.0f,	2.0f,		0.0f,	0.2f,		0.0f,	0.0f,	-1.0f,	//2
		5.0f,	4.0f,	2.0f,	    1.0f,	0.2f,		0.0f,	0.0f,	-1.0f,	//3

		// CARA IZQUIERDA
		-5.0f,	3.0f,	2.0f,		0.0f,   1.0f,		1.0f,	0.0f,	0.0f,	//0
		-5.0f,	3.0f,	-2.0f,		0.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//4
		-5.0f,	4.0f,	-2.0f,		0.5f,	0.0f,		1.0f,	0.0f,	0.0f,	//6

		-5.0f,	4.0f,	-2.0f,		0.5f,	0.0f,		1.0f,	0.0f,	0.0f,	//6
		-5.0f,	4.0f,	2.0f,		0.5f,	1.0f,		1.0f,	0.0f,	0.0f,	//2
		-5.0f,	3.0f,	2.0f,		0.0f,   1.0f,		1.0f,	0.0f,	0.0f,	//0

		// CARA DERECHA
		5.0f,	3.0f,	2.0f,	    1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//1
		5.0f,	3.0f,	-2.0f,	    0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//5
		5.0f,	4.0f,	-2.0f,	    0.0f,	0.5f,		-1.0f,	0.0f,	0.0f,	//7

		5.0f,	4.0f,	-2.0f,	    0.0f,	0.5f,		-1.0f,	0.0f,	0.0f,	//7
		5.0f,	4.0f,	2.0f,	    1.0f,	0.5f,		-1.0f,	0.0f,	0.0f,	//3
		5.0f,	3.0f,	2.0f,	    1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//1

		// CARA TRASERA
		5.0f,	3.0f,	-2.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5
		-5.0f,	3.0f,	-2.0f,		0.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//4
		-5.0f,	4.0f,	-2.0f,		0.0f,	0.2f,		0.0f,	0.0f,	1.0f,	//6

		-5.0f,	4.0f,	-2.0f,		0.0f,	0.2f,		0.0f,	0.0f,	1.0f,	//6
		5.0f,	4.0f,	-2.0f,	    1.0f,	0.2f,		0.0f,	0.0f,	1.0f,	//7
		5.0f,	3.0f,	-2.0f,	    1.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5

		//TAPA SUPERIOR
		-5.0f,	4.0f,	2.0f,		0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//2
		-5.0f,	4.0f,	-2.0f,		1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//6
		5.0f,	4.0f,	2.0f,	    0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//3

		5.0f,	4.0f,	-2.0f,	    1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//7
		-5.0f,	4.0f,	-2.0f,		1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//6
		5.0f,	4.0f,	2.0f,	    0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//3

		//TAPA INFERIOR 
		//(aunque en realidad no es tan necesaria a menos que se quiera ver la banca desde abajo)
		-5.0f,	3.0f,	2.0f,		0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//2
		-5.0f,	3.0f,	-2.0f,		1.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//6
		5.0f,	3.0f,	2.0f,	    0.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//3

		5.0f,	3.0f,	-2.0f,	    1.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//7
		-5.0f,	3.0f,	-2.0f,		1.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//6
		5.0f,	3.0f,	2.0f,	    0.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//3
	};

	Mesh* banca1 = new Mesh();
	banca1->CreateMesh(banca_vertices, banca_indices, 768, 84);
	meshList.push_back(banca1);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	printf("Prueba repo");
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearBanca1();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	//********************************CARGA DE TEXTURAS*************************************
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	//Textura de la banca
	banca1Texture = Texture("Textures/piedra_banca.png");
	banca1Texture.LoadTextureA(); 


	//********************************CARGA DE MODELOS*************************************
	//Objetos
	Lifmunk = Model();
	Lifmunk.LoadModel("Models/Lifmunk.obj");
	
	PalBall = Model();
	PalBall.LoadModel("Models/PalBall.obj");

	Mineral = Model();
	Mineral.LoadModel("Models/Mineral.obj");

	Orbe = Model();
	Orbe.LoadModel("Models/Orbe.obj");

	Motobug = Model();
	Motobug.LoadModel("Models/ModelosAle/motobug.obj");

	
	Checkpoint = Model();
	Checkpoint.LoadModel("Models/ModelosAle/checkpoint.obj");

	Joya = Model();
	Joya.LoadModel("Models/ModelosAle/joya.obj");
	
	Cofre = Model();
	Cofre.LoadModel("Models/ModelosAle/cofre.obj");

	CajaAnillo = Model();
	CajaAnillo.LoadModel("Models/ModelosAle/ring_monitor.obj");

	Oro = Model();
	Oro.LoadModel("Models/ModelosAle/lingote_oro.obj");

	Baculo = Model();
	Baculo.LoadModel("Models/ModelosAle/baculo.obj");
	
	
	//FLORA
	FruHa = Model();
	FruHa.LoadModel("Models/FruHa.obj");

	Palmera = Model();
	Palmera.LoadModel("Models/ModelosAle/palmera.obj");

	HongoM = Model();
	HongoM.LoadModel("Models/ModelosAle/hongo_magico.obj");

	//FAUNA
	Crabmeat = Model();
	Crabmeat.LoadModel("Models/ModelosAle/crabmeat.obj");

	Pollo = Model();
	Pollo.LoadModel("Models/Pollo.obj");

	Oveja = Model();
	Oveja.LoadModel("Models/Oveja.obj");
	
	//Vehiculos
	//Vochito
	VCha = Model();
	VCha.LoadModel("Models/VCha.obj");

	VCof = Model();
	VCof.LoadModel("Models/VCof.obj");

	VLlDDe = Model();
	VLlDDe.LoadModel("Models/VLlDDe.obj");

	VLlDIz = Model();
	VLlDIz.LoadModel("Models/VLlDIz.obj");

	VLlTDe = Model();
	VLlTDe.LoadModel("Models/VLlTDe.obj");

	VLlTIz = Model();
	VLlTIz.LoadModel("Models/VLlTIz.obj");

	//Autobus
	ACha = Model();
	ACha.LoadModel("Models/ACha.obj");

	ALlDDe = Model();
	ALlDDe.LoadModel("Models/ALlDDe.obj");

	ALlDIz = Model();
	ALlDIz.LoadModel("Models/ALlDIz.obj");

	ALlTDe = Model();
	ALlTDe.LoadModel("Models/ALlTDe.obj");

	ALlTIz = Model();
	ALlTIz.LoadModel("Models/ALlTIz.obj");

	//Moto
	MCha = Model();
	MCha.LoadModel("Models/ModelosAle/moto_cuerpo.obj");

	MLl = Model();
	MLl.LoadModel("Models/ModelosAle/moto_llanta.obj");

	//Personaje
	//Depresso
	DCu = Model();
	DCu.LoadModel("Models/DCu.obj");

	DBDe = Model();
	DBDe.LoadModel("Models/DBDe.obj");

	DBIz = Model();
	DBIz.LoadModel("Models/DBIz.obj");

	DPDe = Model();
	DPDe.LoadModel("Models/DPDe.obj");

	DPIz = Model();
	DPIz.LoadModel("Models/DPIz.obj");

	//Sonic
	SCu = Model();
	SCu.LoadModel("Models/ModelosAle/sonic_cuerpo.obj");

	SBDe = Model();
	SBDe.LoadModel("Models/ModelosAle/sonic_brazo_derecho.obj");

	SBIz = Model();
	SBIz.LoadModel("Models/ModelosAle/sonic_brazo_izquierdo.obj");

	SPDe = Model();
	SPDe.LoadModel("Models/ModelosAle/sonic_pierna_derecha.obj");

	SPIz = Model();
	SPIz.LoadModel("Models/ModelosAle/sonic_pierna_izquierda.obj");

	//Edificacion
	PSol = Model();
	PSol.LoadModel("Models/PSol.obj");

	/*PLuna = Model();
	PLuna.LoadModel("Models/ModelosAle/PLuna.obj");

	Altar = Model();
	Altar.LoadModel("Models/ModelosAle/Altar.obj");*/

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//Contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Primera luz puntual
	/*pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;*/

	//Contador de luces spotlight
	unsigned int spotLightCount = 0;

	//Primera luz Spotlight
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	////Segunda luz Spotlight
	//spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
	//	1.0f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	15.0f);
	//spotLightCount++;
	
	//Continuar para más luces

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Loop mientras no se cierra la ventana
	
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 1.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		
		//**************************************************************************EDIFICACIONES**************************************************************************

		//PIRAMIDE DEL SOL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-248.0f, 0.0f, 287.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PSol.RenderModel();
		glDisable(GL_BLEND);

		//PIRAMIDE DEL SOL

		//ALTAR SOL

		//ALTAR LUNA

		//**************************************************************************PERSONAJES**************************************************************************
		
		//DEPRESSO
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-220.0f, 1.8f, 61.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DCu.RenderModel();

		//Brazo Izquierdo
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.6f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DBIz.RenderModel();

		//Brazo Derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.6f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DBDe.RenderModel();

		//Pierna Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.4f, -0.6f, 0.8f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DPIz.RenderModel();

		//Pierna Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, -0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DPDe.RenderModel();

		//SONIC
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(201.0f, 3.6f, 27.0f));
		//model = glm::translate(model, glm::vec3(mainWindow.getmovimiento(), 0.0f, 0.0f)); //para hacer que se mueva el personaje
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SCu.RenderModel(); //variable con el modelo del cuerpo de la moto
		modelaux = model;

		//Brazo izquierdo
		model = modelaux; //reinicia la matriz auxiliar para la jerarquia
		model = glm::translate(model, glm::vec3(0.1f, 0.4f, 0.8f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); //cambiar por otra articulación
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SBIz.RenderModel();

		//Brazo derecho
		model = modelaux; //reinicia la matriz auxiliar para la jerarquia
		model = glm::translate(model, glm::vec3(0.1f, 0.4f, -0.7f));
		//model = glm::rotate(model, glm::radians(-mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); //cambiar por otra articulación
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SBDe.RenderModel();

		//Pierna izquierda
		model = modelaux; //reinicia la matriz auxiliar para la jerarquia
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.3f));
		//model = glm::rotate(model, glm::radians(-mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); //cambiar por otra articulación
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SPIz.RenderModel();

		//Pierna derecha
		model = modelaux; //reinicia la matriz auxiliar para la jerarquia
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -0.3f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); //cambiar por otra articulación
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SPDe.RenderModel();


		//**************************************************************************AUTOMOVILES**************************************************************************

		//VOCHITO
		//Chasis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-356.0f, 3.0f, -42.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VCha.RenderModel();

		//Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.95f, 2.8f, -0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VCof.RenderModel();

		//Llanta Delantera Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.5f, -4.3f, 7.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VLlDIz.RenderModel();

		//Llanta Delantera Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-11.5f, -4.3f, -7.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VLlDDe.RenderModel();

		//Llanta Trasera Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(14.0f, -4.3f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VLlTIz.RenderModel();

		//Llanta Trasera Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(14.0f, -4.3f, -7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VLlTDe.RenderModel();

		//AUTOBUS
		//Chasis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(139.0f, 4.0f, 16.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ACha.RenderModel();

		//Llanta Delantera Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.43f, -0.55f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ALlDIz.RenderModel();

		//Llanta Delantera Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.43f, -0.55f, -0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ALlDDe.RenderModel();

		//Llanta Trasera Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.43f, -0.55f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ALlTIz.RenderModel();

		//Llanta Trasera Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.43f, -0.55f, -0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ALlTDe.RenderModel();

		//MOTO
		//Chasis
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, -1.0f, -85.0f));
		//model = glm::translate(model, glm::vec3(mainWindow.getmovimiento(), 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MCha.RenderModel(); //variable con el modelo del cuerpo de la moto
		modelaux = model;

		//Llanta Delantera
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, -2.4f)); //mueve la llanta hacia abajo
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f)); //articulacion de la llanta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MLl.RenderModel(); //variable con el modelo de una sola llanta

		//Llanta trasera
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 2.4f)); //mueve la llanta hacia abajo
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f)); //articulacion de la llanta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MLl.RenderModel();

		//*********************************************************************FAUNA**************************************************************************

		//Pollo (1)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-321.0f, 0.2f, 66.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pollo.RenderModel();

		//Oveja (2)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-407.0f, 0.37f, 76.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Oveja.RenderModel();

		//Crabmeat (3)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, 78.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Crabmeat.RenderModel();

		//**************************************************************************FLORA**************************************************************************

		//FrutaHabilidad
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(301.0f, 0.2f, 154.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FruHa.RenderModel();

		//Palmera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-118.0f, -0.75f, -103.0f));
		model = glm::scale(model, glm::vec3(10.5f, 10.5f, 10.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palmera.RenderModel();

		//Hongo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(475.0f, -1.0f, 145.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HongoM.RenderModel();

		//**************************************************************************OBJETOS**************************************************************************

		//Mineral (1)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-248.0f, 18.5f, 325.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mineral.RenderModel();

		//Lifmunk (2)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-147.0f, -1.0f, 62.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lifmunk.RenderModel();

		//Cofre (3)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-84.0f, -1.0f, 164.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();

		//Motobug 1 Piramide Luna (4)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(279.0f, -1.0f, -208.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Motobug.RenderModel();

		//Motobug 2 Pasando Calle
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-333.0f, -1.0f, -101.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Motobug.RenderModel();

		//Motobug 3 Calle
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, -1.0f, 66.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Motobug.RenderModel();

		//PalBall Cruzando Calle (5)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-290.0f, 0.45f, -181.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PalBall.RenderModel();

		//PalBall Calle
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(148.0f, 0.45f, 78.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PalBall.RenderModel();

		//Orbe Magico  (6)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(479.0f, -0.5f, -22.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Orbe.RenderModel();

		//Checkpoint  (7)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(143.0f, -0.0f, 237.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Checkpoint.RenderModel();

		//Joya  (8)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(244.0f, -0.0f, -114.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Joya.RenderModel();
		glDisable(GL_BLEND);

		//Caja anillo  (9)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-77.0f, -0.0f, 275.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CajaAnillo.RenderModel();

		//Oro (10)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-417.0f, -0.0f, 232.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Oro.RenderModel();

		//Baculo
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(129.0f, -0.0f, -101.0f));
		model = glm::translate(model, glm::vec3(53.0f, -1.0f, -87.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Baculo.RenderModel();
		
		//Banca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(201.0f, 0.0f, -58.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		banca1Texture.UseTexture();

		meshList[4]->RenderMesh(); //indice dentro de la meshList (IMPORTANTE:cambiar si es necesario)

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
