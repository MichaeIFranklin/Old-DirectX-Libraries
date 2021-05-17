#ifndef XModel_h_
#define XModel_h_


#include "base.h"
#include "string"
#include <vector>

class XModel
{
private:
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 OrbitCenter;
	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 SelfRotation;
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX PositionMatrix;
	D3DXMATRIX OrbitCenterMatrix;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX SelfRotationMatrix;
	D3DXMATRIX WorldMatrix;
public:
	inline D3DXMATRIX* GetWorld()
	{
		return &WorldMatrix;
	};
	inline D3DXVECTOR3 GetPosition()
	{
		 return Position;
	};
	inline D3DXVECTOR3 GetSelfRotation()
	{
		return SelfRotation;
	};

	void Update();
	void Place(D3DXVECTOR3 SelfRotation = D3DXVECTOR3(0,0,0), D3DXVECTOR3 Position = D3DXVECTOR3(0,0,0), D3DXVECTOR3 Rotation = D3DXVECTOR3(0,0,0), D3DXVECTOR3 OrbitCenter = D3DXVECTOR3(0,0,0),  D3DXVECTOR3 Scale = D3DXVECTOR3(1,1,1));
	void Move(D3DXVECTOR3 TargetPosition = D3DXVECTOR3(0,0,0),bool MoveToPosition = false);
	void Resize(D3DXVECTOR3 Scale  = D3DXVECTOR3(1,1,1),bool ScaleToSize = false);
	void Rotate(D3DXVECTOR3 Rotation = D3DXVECTOR3(0,0,0),bool SelfRotation = false,bool RotationToDegree = false);
	void ChangeOrbit(D3DXVECTOR3 OrbitCenter);
	void SpinAround(D3DXVECTOR3 PointOfFocus,float SpinSpeed, bool Orbit);
	bool Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection);
	bool Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection, D3DXVECTOR3 &PickedPoint);
	bool Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection, D3DXVECTOR3 &PickedPoint, float &Distance);
	bool Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection, D3DXVECTOR3 &PickedPoint, float &Distance, D3DXVECTOR3 &TriVerticesIndices);
	D3DXVECTOR3 ScreenPos(D3DVIEWPORT9 ViewPort, D3DXMATRIX View, D3DXMATRIX Projection);
};

// used for vertex intersection info
struct D3DVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT tu, tv;

	static const DWORD FVF;
};

class XFile
{
private:
	ID3DXMesh* Mesh;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<IDirect3DTexture9*> Textures;
public:
	~XFile()
	{
		if(Mesh)
			d3d::Release<ID3DXMesh*>(Mesh);
		for(int i = 0; i < Textures.size(); i++)
			d3d::Release<IDirect3DTexture9*>(Textures[i]);
	};
	inline ID3DXMesh* GetMesh()
	{
		return Mesh;
	};
	inline std::vector<D3DMATERIAL9> GetMtrls()
	{
		return Mtrls;
	};
	inline std::vector<IDirect3DTexture9*> GetTextures()
	{
		return Textures;
	};

	bool Setup(char XFilePath[],IDirect3DDevice9* Device);
	void ChangeTexture(int Subset, char TextureFilePath[],IDirect3DDevice9* Device);
	void Draw(D3DXMATRIX WorldMatrix,IDirect3DDevice9* Device);
	void Draw(D3DXMATRIX WorldMatrix,IDirect3DTexture9* Texture, IDirect3DDevice9* Device);
};

#endif