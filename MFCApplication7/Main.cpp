
// Main.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication7.h"
#include "Main.h"
#include "afxdialogex.h"
#include "FreeImage.h"
#include "Triangle.h"
#include "Ray.h"
#include "Intersection.h"
#include "Material.h"
#include <memory>
#include <stack>
#include "Transform.h"
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <iostream>
#include "Node.h"
#include <iomanip>
#include "OBJ_Loader.h"
#include <direct.h>




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
int WM_UPDATE_PROG = RegisterWindowMessage(L"MYMESSAGE");
bool readvals(std::stringstream &s, const int numvals, float * values);
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// RayTracerDlg dialog



RayTracerDlg::RayTracerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION7_DIALOG, pParent)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//camera -.5 0 1 0 0 -1 0 1 0 45
CProgressCtrl * prog;
const int width = 640;
const int height = 480;
//0 -4 4 0 -1 0 0 1 1 45

vec3 eyepos = vec3(90, 80, -92.7);
vec3 lookAt = vec3(20, 25, 0);
vec3 upVec = vec3(0, 1, 0);
float fov = 45;
int currentVert = 0;
int currentVertNorm = 0;
int currentMaxVerts = 0;
int currentMaxNorms = 0;

void RayTracerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS1, RenderProgress);
	prog = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS1);
	prog->SetRange(0, height);


}

BEGIN_MESSAGE_MAP(RayTracerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &RayTracerDlg::OnBnClickedOk)

END_MESSAGE_MAP()


// RayTracerDlg message handlers

BOOL RayTracerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

									// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void RayTracerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void RayTracerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void rightmultiply(const mat4 & M, std::stack<mat4> &transfstack)
{
	mat4 &T = transfstack.top();
	T = T * M;
}

std::string path = "ViperModel\\";
std::string fname = "obj";

void renderImage(BYTE **pix) {
	BYTE *pixels = (BYTE *)malloc(3 * width * height * sizeof(BYTE));
	int k = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width * 3; j++) {
			pixels[k] = pix[i][j];
			k++;
		}
		free(pix[i]);
	}
	free(pix);
	mkdir("Output");
	FreeImage_Initialise();
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
	FreeImage_Save(FIF_PNG, img, ("output\\" + fname + ".png").c_str(), 0);
	FreeImage_DeInitialise();
	free(pixels);

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR RayTracerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//size 640 480
//output scene4 - ambient.png

Node * root;
Material * currentMat;

std::vector<Light*> lights(0);
std::vector<Material*> materials(0);

//std::vector<std::unique_ptr<SceneObject>> spheres;

// Objects to render
std::vector<SceneObject*> objects;
std::vector<Triangle*> triangles;

// Objects to visualize lights, etc. Only difference is they don't produce shadow or reflection
// This feature has been partialy removed
std::vector<SceneObject*> helpers;

std::vector<glm::vec3> vertNorms(0);
std::vector<glm::vec3> vertices(0);
glm::vec3 ambient(0, 0, 0);
std::stack<glm::mat4> transf;
vec3 attenuation(1., 0.,0.);
vec3 diffuse(.4, .1, .1);
vec3 emissive(0, 0, 0);
vec3 specular(.3,.7,0);
float transmission = 1.4;
float refractiveness = 0;
float shininess = 20;


vec3 ComputeLight(const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, Material * mat) {
	vec3 diff = mat->diffuse;
	vec3 spec = mat->specular;
	float shin = mat->shininess;

	float nDotL = glm::dot(normal, direction);
	vec3 lambert = diff * lightcolor * glm::max(nDotL, 0.f);

	float nDotH = dot(normal, halfvec);
	vec3 phong = spec * lightcolor * pow(glm::max(nDotH, 0.f), shin);

	return lambert + phong;;
}

glm::vec3 FindColor(Intersection * i, int currentDepth) {

	if (currentDepth > 4 || !i->isHit) {
		return vec3(0., 0., 0.);
	}

	vec3 mypos = i->intersectPoint;
	vec3 eyedirn = normalize(i->rayOrigin - mypos);

	vec3 normal = (i->normal);
	vec3 direction;
	vec3 lightCol(0., 0., 0.);
	vec3 lightPos;
	vec3 half;
	float atten = 1;
	float t = FLT_MAX;
	float visibility = 1;
	vec3 reflectedRayDir = i->incidentRayDirection - (2.0f * dot(i->incidentRayDirection, normal) * normal);

	for (int j = 0, len = lights.size(); j < len; j++) {
		lightPos = vec3(lights[j]->pos);

		if (lights[j]->pos.w == 0) {
			direction = normalize(lightPos);
		}
		else {
			direction = normalize(lightPos - mypos);
			t = (lightPos - mypos).x / direction.x;
			float d = glm::distance(lightPos, mypos);
			atten = (1. / (lights[j]->atten.x + lights[j]->atten.y*d + lights[j]->atten.z*d*d));
		}

		// Cast shadow ray
		Ray * shadowRay = new Ray(i->intersectPoint + direction * .01f, (direction));

		std::vector<Triangle*> currentTriangles = Node::hit(shadowRay, root, 0);

		visibility = Intersection(shadowRay, currentTriangles, t).isHit ? 0 : 1;

		half = normalize(direction + eyedirn);
		lightCol += visibility * atten * ComputeLight((direction), lights[j]->col, normal, half, i->myMat);
	}

	lightCol += i->myMat->emissive + i->myMat->ambient;

	vec3 reflectedColor;

	// Terminate the recursion if the material is not reflective
	if (dot(i->myMat->specular, vec3(1, 1, 1)) == 0.0f) {
		reflectedColor = vec3(0, 0, 0);
	}
	else {
		Ray * reflectedRay = new Ray(mypos + reflectedRayDir * .01f, reflectedRayDir);

		std::vector<Triangle*> currentTriangles = Node::hit(reflectedRay, root, 0);

		Intersection * nextIntersect = new Intersection(reflectedRay, currentTriangles, FLT_MAX);
		reflectedColor = i->myMat->specular * FindColor(nextIntersect, currentDepth + 1);
	}

	lightCol += reflectedColor;

	// uncomment the first return for interesting effect
	//return (currentDepth > 1 ? i->myMat->specular : vec3(1.)) * FindColor(nextIntersect, lightCol + colorIn, currentDepth + 1);

	// refraction calculation
	if (i->myMat->refractiveness == 0)
		return lightCol;

	float c1 = dot(normal, i->incidentRayDirection);
	float n;
	if (c1 > 0) {
		normal *= -1.0f;
		n = i->myMat->refractiveIndex / 1.0f;
	}
	else {
		n = 1.0f / i->myMat->refractiveIndex;
	}
	c1 = fabs(c1);
	float sqrtTerm = 1.0f - (n * n) * (1.0f - c1 * c1);

	if (sqrtTerm < 0)
		return lightCol;

	float c2 = sqrt(sqrtTerm);

	vec3 transmittedDirection = n * i->incidentRayDirection + (n * c1 - c2) * normal;

	Ray * transmittedRay = new Ray(mypos + transmittedDirection * .01f, transmittedDirection);

	std::vector<Triangle*> currentTriangles = Node::hit(transmittedRay, root, 0);

	Intersection * refractIntersect = new Intersection(transmittedRay, currentTriangles, FLT_MAX);

	return lightCol + (vec3(1., 1., 1.) - reflectedColor) * i->myMat->refractiveness  * FindColor(refractIntersect, currentDepth + 1);
}



void RayTracerDlg::Raytrace(Camera * cam) {

	BYTE **pixels = (BYTE **)malloc(height * sizeof(BYTE *));

	//#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < height; i++) {

		int progress = int((float(i) / float(height)) * height);
		prog->SetPos(progress);
		pixels[i] = (BYTE *)malloc(width * 3 * sizeof(BYTE));

		//#pragma omp parallel for schedule(dynamic)
		for (int j = 0; j < width * 3; j += 3) {

			// Ray origin is shot from a random location from inside the pixel 
			// with a max of .25 pixels away from the center of pixel
			int randX = rand() % 50 + 25;
			int randY = rand() % 50 + 25;

			Ray * ray = new Ray(cam, float(i) + float(randX) * .01, float(j / 3) + float(randY) * .01, width, height);

			std::vector<Triangle*> currentTriangles = Node::hit(ray, root, 0);

			Intersection * hit = new Intersection(ray, currentTriangles, FLT_MAX, helpers);
			vec3 color = FindColor(hit, 0);

			pixels[i][j] = glm::min((color.b * 255), float(255));
			pixels[i][j + 1] = glm::min((color.g * 255), float(255));
			pixels[i][j + 2] = glm::min((color.r * 255), float(255));
		}
	}
	renderImage(pixels);
}



// Read from file

void readobj3(std::string filename);
UINT MyThreadProc(LPVOID test) {
	transf.push(mat4(1.));
	std::string inputFilename = (path + fname + ".obj");
	readobj3(inputFilename);
	root = Node::buildTree(triangles, 0);
	RayTracerDlg::Raytrace(new Camera(fov, eyepos, lookAt, upVec));
	return 0;
}



void RayTracerDlg::OnBnClickedOk()
{
	CWinThread * pThread = new CWinThread();
	AfxBeginThread(MyThreadProc, pThread);
	delete pThread;
}


bool readvals(std::stringstream &s, const int numvals, float * values)
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i];
		if (s.fail()) {
			std::cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}


void readobj3(std::string filename) {
	TRACE("\n READING OBJ");

	// Give a default light, since obj doesn't seem to have one
	// directional 0 1 -1 0.2 0.2 0.2
	// point 0 0.44 -1.5 0.8 0.8 0.8
	lights.push_back(new Light(vec4(1, -1, .63, 0.), vec3(1,1,1), attenuation));
	lights.push_back(new Light(vec4(0,.5,.5,0), vec3(1, 1, 1), attenuation));
	lights.push_back(new Light(vec4(0, 50, -100, 1), vec3(1, 1, 1), attenuation));

	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile(filename.c_str());

	// Check to see if it loade
	// If so continue
	if (loadout)
	{
		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			// See if this material has already been instantiated, if not then do so
			int k = 0;
			for (; k < materials.size(); k++) {
				//TRACE("\ni: %d", i);
				if (!strcmp(curMesh.MeshMaterial.name.c_str(), materials[k]->name.c_str())) {
					currentMat = materials[k];
					break;
				}
			}
			if (k == materials.size()) {
				ambient = vec3(curMesh.MeshMaterial.Ka.X, curMesh.MeshMaterial.Ka.Y, curMesh.MeshMaterial.Ka.Z);
				diffuse = vec3(curMesh.MeshMaterial.Kd.X, curMesh.MeshMaterial.Kd.Y, curMesh.MeshMaterial.Kd.Z);
				specular = vec3(curMesh.MeshMaterial.Ks.X, curMesh.MeshMaterial.Ks.Y, curMesh.MeshMaterial.Ks.Z);
				shininess = curMesh.MeshMaterial.Ns;
				currentMat = new Material(curMesh.MeshMaterial.name, ambient, emissive, diffuse, specular, transmission, shininess, refractiveness);
				materials.push_back(currentMat);
			}
			//TRACE("\ncurrentMat name: %s ",currentMat->name.c_str());

			
			/*file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";*/

			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				vertices.resize(++currentMaxVerts);
				vertices[currentVert++] = vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);

				vertNorms.resize(++currentMaxNorms);
				vertNorms[currentVertNorm++] = normalize(vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z));
				//TODO: set texture coords
				//curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
			}

			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				int a = curMesh.Indices[j], b = curMesh.Indices[j + 1], c = curMesh.Indices[j + 2];

				vec3 vert1 = vec3(curMesh.Vertices[a].Position.X, curMesh.Vertices[a].Position.Y, curMesh.Vertices[a].Position.Z);
				vec3 vert2 = vec3(curMesh.Vertices[b].Position.X, curMesh.Vertices[b].Position.Y, curMesh.Vertices[b].Position.Z);
				vec3 vert3 = vec3(curMesh.Vertices[c].Position.X, curMesh.Vertices[c].Position.Y, curMesh.Vertices[c].Position.Z);

				vec3 norm1 = vec3(curMesh.Vertices[a].Normal.X, curMesh.Vertices[a].Normal.Y, curMesh.Vertices[a].Normal.Z);
				vec3 norm2 = vec3(curMesh.Vertices[b].Normal.X, curMesh.Vertices[b].Normal.Y, curMesh.Vertices[b].Normal.Z);
				vec3 norm3 = vec3(curMesh.Vertices[c].Normal.X, curMesh.Vertices[c].Normal.Y, curMesh.Vertices[c].Normal.Z);

				triangles.push_back(new Triangle(vert1, vert2, vert3, norm1, norm2, norm3, currentMat, transf.top(), false));
			}

		}

	}
	// If not output an error
	else
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Output Error
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

		// Close File
		file.close();
	}
}


