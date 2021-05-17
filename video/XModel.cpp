#include "XModel.h"

/// XFile Functions ////////////
bool XFile::Setup(char XFilePath[],IDirect3DDevice9* Device)
{
	HRESULT hr = 0;

	//
	// Load the XFile data.
	//
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromX(XFilePath,D3DXMESH_MANAGED,Device,&adjBuffer,&mtrlBuffer,0,&numMtrls,&Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0,"D3DXLoadMeshFromX() - FAILED",0,0);
		return false;
	}

	// Extract mat and load textures
	if(mtrlBuffer !=0 && numMtrls !=0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for(int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			Mtrls.push_back(mtrls[i].MatD3D);
			if(mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(Device,mtrls[i].pTextureFilename,&tex);
				Textures.push_back(tex);
			}
			else
			{
				Textures.push_back(0);
			}
		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer);

	//Optimize mesh
	hr = Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,(DWORD*)adjBuffer->GetBufferPointer(),0,0,0);
	d3d::Release<ID3DXBuffer*>(adjBuffer);

	if(FAILED(hr))
	{
		::MessageBox(0,"OptimizeInplace() - FAILED",0,0);
		return false;
	}
	return true;
}

void XFile::ChangeTexture(int Subset,char TextureFilePath[],IDirect3DDevice9* Device)
{
	IDirect3DTexture9* tex = 0;
	D3DXCreateTextureFromFile(Device,TextureFilePath,&tex);
	Textures[Subset] = tex;
}

void XFile::Draw(D3DXMATRIX WorldMatrix, IDirect3DDevice9* Device)
{
	if( Device )
	{
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetTransform(D3DTS_WORLD,&WorldMatrix);
		
		for(int iIterator = 0; iIterator < Mtrls.size();iIterator++)
		{
			Device->SetMaterial(&Mtrls[iIterator]);
			Device->SetTexture(0,Textures[iIterator]);
			Mesh->DrawSubset(iIterator);
		}
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}

void XFile::Draw(D3DXMATRIX WorldMatrix, IDirect3DTexture9* Texture, IDirect3DDevice9* Device)
{
	if( Device )
	{
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetTransform(D3DTS_WORLD,&WorldMatrix);

		for(int iIterator = 0; iIterator < Mtrls.size();iIterator++)
		{
			if (iIterator == 0)
			{
				Device->SetRenderState(D3DRS_LIGHTING,false);
			}
			Device->SetMaterial(&Mtrls[iIterator]);
			Device->SetTexture(0,Texture);
			Mesh->DrawSubset(iIterator);
			Device->SetRenderState(D3DRS_LIGHTING,true);
		}
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}
/// End XFile Functions //////////


//// XModel Functions ////////////
void XModel::Place(D3DXVECTOR3 SelfRotation, D3DXVECTOR3 Position, D3DXVECTOR3 Rotation, D3DXVECTOR3 OrbitCenter, D3DXVECTOR3 Scale)
{
	this->SelfRotation = SelfRotation;
	this->Rotation = Rotation;
	this->Scale = Scale;
	this->Position = Position;
	this->OrbitCenter = OrbitCenter;
	D3DXMatrixRotationYawPitchRoll(&this->SelfRotationMatrix,SelfRotation.y,SelfRotation.x,SelfRotation.z);
	D3DXMatrixTranslation(&this->PositionMatrix,Position.x,Position.y,Position.z);
	D3DXMatrixTranslation(&this->OrbitCenterMatrix,OrbitCenter.x,OrbitCenter.y,OrbitCenter.z);
	D3DXMatrixRotationYawPitchRoll(&this->RotationMatrix,Rotation.y,Rotation.x,Rotation.z);
	D3DXMatrixScaling(&this->ScaleMatrix,Scale.x,Scale.y,Scale.z);
}

void XModel::Move(D3DXVECTOR3 TargetPosition,bool MoveToPosition)
{
	if(MoveToPosition)
	{
		this->Position = TargetPosition;
		D3DXMatrixTranslation(&this->PositionMatrix,TargetPosition.x,TargetPosition.y,TargetPosition.z);
	}
	else
	{
		this->Position += TargetPosition;
		D3DXMatrixTranslation(&this->PositionMatrix,this->Position.x,this->Position.y,this->Position.z);
	}
}

void XModel::Resize(D3DXVECTOR3 Scale,bool ScaleToSize)
{
	if(ScaleToSize)
	{
		this->Scale = Scale;
		D3DXMatrixScaling(&this->ScaleMatrix,1,1,1);
		D3DXMatrixScaling(&this->ScaleMatrix,Scale.x,Scale.y,Scale.z);
	}
	else
	{
		this->Scale += Scale;
		D3DXMatrixScaling(&this->ScaleMatrix,Scale.x,Scale.y,Scale.z);
	}
}

void XModel::Rotate(D3DXVECTOR3 Rotation,bool SelfRotation,bool RotationToDegree)
{
	if(RotationToDegree)
	{
		if(SelfRotation)
		{
			this->SelfRotation = Rotation;
			D3DXMatrixRotationYawPitchRoll(&this->SelfRotationMatrix,this->SelfRotation.y,this->SelfRotation.x,this->SelfRotation.z);
		}
		else
		{
			this->Rotation = Rotation;
			D3DXMatrixRotationYawPitchRoll(&this->RotationMatrix,this->Rotation.y,this->Rotation.x,this->Rotation.z);
		}
	}
	else
	{
		if(SelfRotation)
		{
			this->SelfRotation += Rotation;
			D3DXMatrixRotationYawPitchRoll(&this->SelfRotationMatrix,this->SelfRotation.y,this->SelfRotation.x,this->SelfRotation.z);
		}
		else
		{
			this->Rotation += Rotation;
			D3DXMatrixRotationYawPitchRoll(&this->RotationMatrix,this->Rotation.y,this->Rotation.x,this->Rotation.z);
		}
	}
}

void XModel::ChangeOrbit(D3DXVECTOR3 OrbitCenter)
{
	this->OrbitCenter = OrbitCenter;
	D3DXMatrixTranslation(&this->OrbitCenterMatrix,OrbitCenter.x,OrbitCenter.y,OrbitCenter.z);
}
void XModel::Update()
{
	WorldMatrix = SelfRotationMatrix * ScaleMatrix * PositionMatrix * RotationMatrix * OrbitCenterMatrix;
}


void XModel::SpinAround(D3DXVECTOR3 PointOfFocus,float SpinSpeed, bool Orbit)
{
	if(!Orbit)
	{
		float RotationSpeed;
		D3DXVECTOR3 Direction;
		Direction.x = GetWorld()->_41 - PointOfFocus.x;
		Direction.z = GetWorld()->_43 - PointOfFocus.z;
		float Angle = 0;
		D3DXVec3Normalize(&Direction,&Direction);
		if(PointOfFocus.z >= GetWorld()->_43)
		{
			Angle = atan(Direction.x/Direction.z);
		}
		else
		{
			Angle = atan(Direction.x/Direction.z);
			Angle += 3.14;
		}

		if(Angle < GetSelfRotation().y)
		{
			RotationSpeed =-SpinSpeed;
		}
		else
		{
			RotationSpeed = SpinSpeed;
		}
		Rotate(D3DXVECTOR3(0,RotationSpeed*(3.14/180),0),true,false);
	}
	else
	{
		static float Angle;
		ChangeOrbit(D3DXVECTOR3(PointOfFocus.x,0,PointOfFocus.z));
		Rotate(D3DXVECTOR3(0,SpinSpeed*(3.14/180),0),false,false);

		Angle += SpinSpeed;
	}
}

bool XModel::Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection)
{
	BOOL Hit = false; // holds if ray hit or not
	D3DXMATRIX WorldInverse; // holds the inverse of mesh's world matrix

	// invert the mesh's world matrix
	D3DXMatrixInverse(&WorldInverse,NULL,GetWorld());

	// transform the ray by the inverted matrix
	D3DXVec3TransformCoord(&RayOrigin,&RayOrigin,&WorldInverse);
	D3DXVec3TransformNormal(&RayDirection,&RayDirection,&WorldInverse);

	// cast ray
	D3DXIntersect(Mesh, &RayOrigin, &RayDirection, &Hit, NULL, NULL, NULL, NULL, NULL, NULL); // checks if the ray has intersected the mesh
	
	return Hit;
}
bool XModel::Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection, D3DXVECTOR3 &PickedPoint)
{
	DWORD FaceIndex; // index of face on mesh that was intersected
	float Vcoord; // barycentric V coordinate
	float Ucoord; // barycentric U coordinate
	float Wcoord; // barycentric W coordinate
	BOOL Hit = false; // holds if ray hit or not
	D3DXMATRIX WorldInverse; // holds the inverse of mesh's world matrix

	// invert the mesh's world matrix
	D3DXMatrixInverse(&WorldInverse,NULL,GetWorld());

	// transform the ray by the inverted matrix
	D3DXVec3TransformCoord(&RayOrigin,&RayOrigin,&WorldInverse);
	D3DXVec3TransformNormal(&RayDirection,&RayDirection,&WorldInverse);

	// cast ray
	D3DXIntersect(Mesh, &RayOrigin, &RayDirection, &Hit, &FaceIndex, &Ucoord, &Vcoord, NULL, NULL, NULL); // checks if the ray has intersected the mesh
	Wcoord = (1 - Ucoord - Vcoord); // calculate Wcord

	if (Hit)
	{
		IDirect3DIndexBuffer9 *IndexBuffer; // holds all indices of mesh
		WORD* Indices; // temp location to hold IndexBuffer's data for viewing
		short intersectedIndices[3]; // holds indices for the intersected face
		IDirect3DVertexBuffer9 *VertexBuffer; // holds all vertices of mesh
		D3DVERTEX* Vertices; // temp location to hold VertexBuffer's data for viewing

		// get indices of intersected mesh
		Mesh->GetIndexBuffer(&IndexBuffer);
		IndexBuffer->Lock(0,0,(void**)&Indices,0); // open index buffer for viewing and copy data to a temp location
		IndexBuffer->Unlock(); // close index buffer for viewing

		// copy intersected indices to array
		for (int i = 0;i < 3;i++)
		{
			intersectedIndices[i] = Indices[(FaceIndex * 3) + i];
		}

		// get vertices of interested face
		Mesh->GetVertexBuffer(&VertexBuffer); // get all vertices of mesh
		VertexBuffer->Lock(0,0,(void**)&Vertices,0); // open vertex buffer for viewing and copy data to a temp location
		VertexBuffer->Unlock(); // close vertex buffer for viewing

		// transform vertices to match mesh's transformation
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[0]].p,&Vertices[intersectedIndices[0]].p,GetWorld());
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[1]].p,&Vertices[intersectedIndices[1]].p,GetWorld());
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[2]].p,&Vertices[intersectedIndices[2]].p,GetWorld());

		// calculate picked point on mesh using barycentric coords
		PickedPoint = Vertices[intersectedIndices[0]].p * Wcoord + Vertices[intersectedIndices[1]].p* Ucoord + Vertices[intersectedIndices[2]].p * Vcoord;
	}

	return Hit;
}
bool XModel::Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection, D3DXVECTOR3 &PickedPoint, float &Distance)
{
	DWORD FaceIndex; // index of face on mesh that was intersected
	float Vcoord; // barycentric V coordinate
	float Ucoord; // barycentric U coordinate
	float Wcoord; // barycentric W coordinate
	BOOL Hit = false; // holds if ray hit or not
	D3DXMATRIX WorldInverse; // holds the inverse of mesh's world matrix

	// invert the mesh's world matrix
	D3DXMatrixInverse(&WorldInverse,NULL,GetWorld());

	// transform the ray by the inverted matrix
	D3DXVec3TransformCoord(&RayOrigin,&RayOrigin,&WorldInverse);
	D3DXVec3TransformNormal(&RayDirection,&RayDirection,&WorldInverse);

	// cast ray
	D3DXIntersect(Mesh, &RayOrigin, &RayDirection, &Hit, &FaceIndex, &Ucoord, &Vcoord, &Distance, NULL, NULL); // checks if the ray has intersected the mesh
	Wcoord = (1 - Ucoord - Vcoord); // calculate Wcord

	if (Hit)
	{
		IDirect3DIndexBuffer9 *IndexBuffer; // holds all indices of mesh
		WORD* Indices; // temp location to hold IndexBuffer's data for viewing
		short intersectedIndices[3]; // holds indices for the intersected face
		IDirect3DVertexBuffer9 *VertexBuffer; // holds all vertices of mesh
		D3DVERTEX* Vertices; // temp location to hold VertexBuffer's data for viewing

		// get indices of intersected mesh
		Mesh->GetIndexBuffer(&IndexBuffer);
		IndexBuffer->Lock(0,0,(void**)&Indices,0); // open index buffer for viewing and copy data to a temp location
		IndexBuffer->Unlock(); // close index buffer for viewing

		// copy intersected indices to array
		for (int i = 0;i < 3;i++)
		{
			intersectedIndices[i] = Indices[(FaceIndex * 3) + i];
		}

		// get vertices of interested face
		Mesh->GetVertexBuffer(&VertexBuffer); // get all vertices of mesh
		VertexBuffer->Lock(0,0,(void**)&Vertices,0); // open vertex buffer for viewing and copy data to a temp location
		VertexBuffer->Unlock(); // close vertex buffer for viewing

		// transform vertices to match mesh's transformation
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[0]].p,&Vertices[intersectedIndices[0]].p,GetWorld());
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[1]].p,&Vertices[intersectedIndices[1]].p,GetWorld());
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[2]].p,&Vertices[intersectedIndices[2]].p,GetWorld());

		// calculate picked point on mesh using barycentric coords
		PickedPoint = Vertices[intersectedIndices[0]].p * Wcoord + Vertices[intersectedIndices[1]].p* Ucoord + Vertices[intersectedIndices[2]].p * Vcoord;
	}

	return Hit;
}
bool XModel::Pick(ID3DXMesh* Mesh, D3DXVECTOR3 RayOrigin, D3DXVECTOR3 RayDirection, D3DXVECTOR3 &PickedPoint, float &Distance, D3DXVECTOR3 &TriVerticesIndices)
{
	DWORD FaceIndex; // index of face on mesh that was intersected
	float Vcoord; // barycentric V coordinate
	float Ucoord; // barycentric U coordinate
	float Wcoord; // barycentric W coordinate
	BOOL Hit = false; // holds if ray hit or not
	D3DXMATRIX WorldInverse; // holds the inverse of mesh's world matrix

	// invert the mesh's world matrix
	D3DXMatrixInverse(&WorldInverse,NULL,GetWorld());

	// transform the ray by the inverted matrix
	D3DXVec3TransformCoord(&RayOrigin,&RayOrigin,&WorldInverse);
	D3DXVec3TransformNormal(&RayDirection,&RayDirection,&WorldInverse);

	// cast ray
	D3DXIntersect(Mesh, &RayOrigin, &RayDirection, &Hit, &FaceIndex, &Ucoord, &Vcoord, &Distance, NULL, NULL); // checks if the ray has intersected the mesh
	Wcoord = (1 - Ucoord - Vcoord); // calculate Wcord

	if (Hit)
	{
		IDirect3DIndexBuffer9 *IndexBuffer; // holds all indices of mesh
		WORD* Indices; // temp location to hold IndexBuffer's data for viewing
		short intersectedIndices[3]; // holds indices for the intersected face
		IDirect3DVertexBuffer9 *VertexBuffer; // holds all vertices of mesh
		D3DVERTEX* Vertices; // temp location to hold VertexBuffer's data for viewing

		// get indices of intersected mesh
		Mesh->GetIndexBuffer(&IndexBuffer);
		IndexBuffer->Lock(0,0,(void**)&Indices,0); // open index buffer for viewing and copy data to a temp location
		IndexBuffer->Unlock(); // close index buffer for viewing

		// copy intersected indices to array
		for (int i = 0;i < 3;i++)
		{
			intersectedIndices[i] = Indices[(FaceIndex * 3) + i];
		}

		// get vertices of interested face
		Mesh->GetVertexBuffer(&VertexBuffer); // get all vertices of mesh
		VertexBuffer->Lock(0,0,(void**)&Vertices,0); // open vertex buffer for viewing and copy data to a temp location
		VertexBuffer->Unlock(); // close vertex buffer for viewing

		// transform vertices to match mesh's transformation
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[0]].p,&Vertices[intersectedIndices[0]].p,GetWorld());
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[1]].p,&Vertices[intersectedIndices[1]].p,GetWorld());
		D3DXVec3TransformCoord(&Vertices[intersectedIndices[2]].p,&Vertices[intersectedIndices[2]].p,GetWorld());

		// enter vertices positions into TriVertices vector
		TriVerticesIndices.x = intersectedIndices[0];
		TriVerticesIndices.y = intersectedIndices[1];
		TriVerticesIndices.z = intersectedIndices[2];

		// calculate picked point on mesh using barycentric coords
		PickedPoint = Vertices[intersectedIndices[0]].p * Wcoord + Vertices[intersectedIndices[1]].p* Ucoord + Vertices[intersectedIndices[2]].p * Vcoord;
	}

	return Hit;
}

D3DXVECTOR3 XModel::ScreenPos(D3DVIEWPORT9 ViewPort, D3DXMATRIX View, D3DXMATRIX Projection)
{
	D3DXVECTOR3 ScreenPosition = D3DXVECTOR3(0,0,0); // Holds result of function

	D3DXVec3Project(&ScreenPosition,&ScreenPosition,&ViewPort,&Projection,&View,GetWorld());

	return ScreenPosition;
}

/// End XModel Functions ///////