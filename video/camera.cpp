#include "camera.h"

void Camera::Setup(IDirect3DDevice9* Device)
{
	// setup general variables
	this->Device = Device;
	OrthoSetup = false;
	_cameraType = AIRWATER;

	// setup projection variables
	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.MinZ = 0;
	Viewport.MaxZ = 1;
	Viewport.Width = WinWidth;
	Viewport.Height = WinHeight;
	FOV = 60 * (3.14/180);

	// setup view variables
	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void Camera::Setup(IDirect3DDevice9* Device, CameraType cameraType)
{
	// setup general variables
	this->Device = Device;
	OrthoSetup = false;
	_cameraType = cameraType;

	// setup projection variables
	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.MinZ = 0;
	Viewport.MaxZ = 1;
	Viewport.Width = WinWidth;
	Viewport.Height = WinHeight;
	FOV = 60 * (3.14/180);

	// setup view variables
	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// setup ORTHO camera if cameratype is set to ORTHO
	if (cameraType == ORTHO && !OrthoSetup)
	{
		Zoom = 10;
		NewTarget = false;
		Target = new D3DXMATRIX;
		D3DXMatrixTranslation(Target,0,0,1);
		D3DXMatrixTranslation(&Anchor,0,0,0-Zoom);
		RotVec = D3DXVECTOR3(0,0,0);
		D3DXMatrixRotationYawPitchRoll(&Rotation,RotVec.y,RotVec.x,RotVec.z);
		Result = Anchor * Rotation * *Target;
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
		OrthoSetup = true;
	}
}

Camera::~Camera()
{
	if (!NewTarget)
	{
		delete Target;
	}
	Target = 0;
	Device = 0;
	delete Device;
}


void Camera::getYawAngle(float* angle)
{
	*angle = YawAngle;
}
void Camera::setYawAngle(float angle)
{
	YawAngle = angle;
}

void Camera::getPitchAngle(float* angle)
{
	*angle = PitchAngle;
}
void Camera::setPitchAngle(float angle)
{
	PitchAngle = angle;
}
void Camera::setZoom(float units, bool SetToUnit)
{
	if (SetToUnit)
	{
		Zoom = units;
	}
	else
	{
		Zoom += units;
	}
	if (_cameraType == ORTHO)
	{
		// Center camera around the Target's position
		D3DXMATRIX TargTemp; // holds Target Matrix position values
		D3DXMatrixIdentity(&TargTemp); // clear temp matrix
		// input Target matrix's position values into temp matrix
		TargTemp._41 = Target->_41;
		TargTemp._42 = Target->_42;
		TargTemp._43 = Target->_43;

		D3DXMatrixTranslation(&Anchor,0,0,-1+Zoom); // set anchor matrix to new zoom level
		Result = Anchor * Rotation * TargTemp; // use new anchor matrix to calculate camera's new position
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43)); // set camera to new position
	}
}
void Camera::getZoom(float *units)
{
	*units = Zoom;
}

void Camera::setFOV(float FOV)
{
	this->FOV = FOV;
}

void Camera::getViewport(D3DVIEWPORT9* Viewport)
{
	*Viewport = this->Viewport;
}
void Camera::setViewportDiamensions(float X, float Y, float Width, float Height)
{
	Viewport.X = (DWORD)X;
	Viewport.Y = (DWORD)Y;
	Viewport.Width = (DWORD)Width;
	Viewport.Height = (DWORD)Height;
}


void Camera::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}
void Camera::setPosition(D3DXVECTOR3 pos)
{
	_pos = pos;
}

void Camera::getTarget(D3DXVECTOR3* target)
{
	target->x = Target->_41;
	target->y = Target->_42;
	target->z = Target->_43;
}
void Camera::setTarget(D3DXVECTOR3 target)
{
	if (_cameraType == ORTHO)
	{
		if (NewTarget)
		{
			Target = 0;
			NewTarget = false;
			Target = new D3DXMATRIX;
		}
		D3DXMatrixTranslation(Target,target.x,target.y,target.z);
		Result = Anchor * Rotation * *Target;
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
	}
}
void Camera::setTarget(D3DXMATRIX* target)
{
	if (_cameraType == ORTHO)
	{
		if (!NewTarget)
		{
			NewTarget = true;
			delete Target;
		}
		Target = target;
		// Center camera around the Target's position
		D3DXMATRIX TargTemp; // holds Target Matrix position values
		D3DXMatrixIdentity(&TargTemp); // clear temp matrix
		// input Target matrix's position values into temp matrix
		TargTemp._41 = Target->_41;
		TargTemp._42 = Target->_42;
		TargTemp._43 = Target->_43;


		Result = Anchor * Rotation * TargTemp; // use new temp target matrix to calculate camera's new position
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43)); // set camera to new position
	}
	
}

void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}
void Camera::setLook(D3DXVECTOR3 look)
{
	_look = look;
}

void Camera::UpdateAnchor()
{
	if (_cameraType == ORTHO)
	{
		// Center camera around the Target's position
		D3DXMATRIX TargTemp; // holds Target Matrix position values
		D3DXMatrixIdentity(&TargTemp); // clear temp matrix
		// input Target matrix's position values into temp matrix
		TargTemp._41 = Target->_41;
		TargTemp._42 = Target->_42;
		TargTemp._43 = Target->_43;

		Result = Anchor * Rotation * TargTemp;
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
	}
	
}

void Camera::UpdateCamera()
{
	// update the viewport
	Device->SetViewport(&Viewport);

	// update the Projection Matrix
	D3DXMatrixPerspectiveFovLH(&Projection, FOV, (float)Viewport.Width /  (float)Viewport.Height, 1.0f, 1000.0f);
	this->Device->SetTransform(D3DTS_PROJECTION, &Projection);

	// update the View Matrix
	getViewMatrix(&View);
	this->Device->SetTransform(D3DTS_VIEW,&View);
}

void Camera::walk(float units)
{
		if (_cameraType == ORTHO)
		{
			if (NewTarget) // if we are targeting a moving model now a constant vector
			{
				// dump model's current position into a temp matrix
				D3DXMATRIX TargTemp; // holds Target Matrix position values
				D3DXMatrixIdentity(&TargTemp); // clear temp matrix
				// input Target matrix's position values into temp matrix
				TargTemp._41 = Target->_41;
				TargTemp._42 = Target->_42;
				TargTemp._43 = Target->_43;

				// reset the Target matrix
				Target = 0;
				NewTarget = false;
				Target = new D3DXMATRIX;
				// input temp matrix values into new Target matrix
				D3DXMatrixTranslation(Target, TargTemp._41, TargTemp._42, TargTemp._43);
			}
			// move the camera on its look vector, ignoring the y axis
			D3DXMatrixTranslation(Target, Target->_41 + (_look.x * units), Target->_42, Target->_43 + (_look.z * units));
			Result = Anchor * Rotation * *Target;
			setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
		}

		// move only on xz plane for land object
		if( _cameraType == GROUND )
			_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

		if( _cameraType == AIRWATER )
			_pos += _look * units;
}

void Camera::strafe(float units)
{
	if (_cameraType == ORTHO)
	{
		if (NewTarget) // if we are targeting a moving model now a constant vector
		{
			// dump model's current position into a temp matrix
			D3DXMATRIX TargTemp; // holds Target Matrix position values
			D3DXMatrixIdentity(&TargTemp); // clear temp matrix
			// input Target matrix's position values into temp matrix
			TargTemp._41 = Target->_41;
			TargTemp._42 = Target->_42;
			TargTemp._43 = Target->_43;

			// reset the Target matrix
			Target = 0;
			NewTarget = false;
			Target = new D3DXMATRIX;
			// input temp matrix values into new Target matrix
			D3DXMatrixTranslation(Target, TargTemp._41, TargTemp._42, TargTemp._43);
		}
		// move the camera on its right vector, ignoring the y axis
		D3DXMatrixTranslation(Target, Target->_41 + (_right.x * units), Target->_42, Target->_43 + (_right.z * units));
		Result = Anchor * Rotation * *Target;
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
	}

	// move only on xz plane for land object
	if( _cameraType == GROUND )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRWATER )
		_pos += _right * units;

}

void Camera::fly(float units)
{
	if (_cameraType == ORTHO)
	{
		if (NewTarget) // if we are targeting a moving model now a constant vector
		{
			// dump model's current position into a temp matrix
			D3DXMATRIX TargTemp; // holds Target Matrix position values
			D3DXMatrixIdentity(&TargTemp); // clear temp matrix
			// input Target matrix's position values into temp matrix
			TargTemp._41 = Target->_41;
			TargTemp._42 = Target->_42;
			TargTemp._43 = Target->_43;

			// reset the Target matrix
			Target = 0;
			NewTarget = false;
			Target = new D3DXMATRIX;
			// input temp matrix values into new Target matrix
			D3DXMatrixTranslation(Target, TargTemp._41, TargTemp._42, TargTemp._43);
		}
		// move the camera on its y axis
		D3DXMatrixTranslation(Target, Target->_41, Target->_42 + units, Target->_43);
		Result = Anchor * Rotation * *Target;
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
	}

	// move only on y-axis for land object
	if( _cameraType == GROUND )
		_pos.y += units;

	if( _cameraType == AIRWATER )
		_pos += _up * units;

}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right,	angle);
	PitchAngle += angle;

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);

	if (_cameraType == ORTHO)
	{
		// Center camera around the Target's position
		D3DXMATRIX TargTemp; // holds Target Matrix position values
		D3DXMatrixIdentity(&TargTemp); // clear temp matrix
		// input Target matrix's position values into temp matrix
		TargTemp._41 = Target->_41;
		TargTemp._42 = Target->_42;
		TargTemp._43 = Target->_43;

		RotVec.x += angle; // add "angle" amount of degrees to the rotation vector around the x axis(camera's right vector)
		D3DXMatrixRotationYawPitchRoll(&Rotation,RotVec.y,RotVec.x,RotVec.z); // calculate new rotation matrix
		Result = Anchor * Rotation * TargTemp; // use new rotation and temp position matrices to calculate camera's new position
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43)); // set camera to new position
	}
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == GROUND || _cameraType == ORTHO)
	{
		D3DXMatrixRotationY(&T, angle);
		YawAngle += angle;
	}

	// rotate around own up vector for AIRWATER
	if( _cameraType == AIRWATER )
	{
		D3DXMatrixRotationAxis(&T, &_up, angle);
		YawAngle += angle;
	}
	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);

	if (_cameraType == ORTHO)
	{
		// Center camera around the Target's position
		D3DXMATRIX TargTemp; // holds Target Matrix position values
		D3DXMatrixIdentity(&TargTemp); // clear temp matrix
		// input Target matrix's position values into temp matrix
		TargTemp._41 = Target->_41;
		TargTemp._42 = Target->_42;
		TargTemp._43 = Target->_43;

		RotVec.y += angle; // add "angle" amount of degrees to the rotation vector around the y axis
		D3DXMatrixRotationYawPitchRoll(&Rotation,RotVec.y,RotVec.x,RotVec.z); // calculate new rotation matrix
		Result = Anchor * Rotation * TargTemp; // use new rotation and temp position matrices to calculate camera's new position
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43)); // set camera to new position
	}
}

void Camera::roll(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_look,	angle);

	// rotate _up and _right around _look vector
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_up,&_up, &T);
}

void Camera::getViewMatrix(D3DXMATRIX* View)
{
	// Keep camera's axes orthogonal to each other
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	(*View)(0,0) = _right.x; (*View)(0, 1) = _up.x; (*View)(0, 2) = _look.x; (*View)(0, 3) = 0.0f;
	(*View)(1,0) = _right.y; (*View)(1, 1) = _up.y; (*View)(1, 2) = _look.y; (*View)(1, 3) = 0.0f;
	(*View)(2,0) = _right.z; (*View)(2, 1) = _up.z; (*View)(2, 2) = _look.z; (*View)(2, 3) = 0.0f;
	(*View)(3,0) = x;        (*View)(3, 1) = y;     (*View)(3, 2) = z;       (*View)(3, 3) = 1.0f;
}

void Camera::getProjectionMatrix(D3DXMATRIX* Projection)
{
	*Projection = this->Projection;
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
	if (cameraType == ORTHO && !OrthoSetup)
	{
		Zoom = 10;
		Target = new D3DXMATRIX;
		NewTarget = false;
		D3DXMatrixTranslation(Target,0,0,1);
		D3DXMatrixTranslation(&Anchor,0,0,0-Zoom);
		RotVec = D3DXVECTOR3(0,0,0);
		D3DXMatrixRotationYawPitchRoll(&Rotation,RotVec.y,RotVec.x,RotVec.z);
		Result = Anchor * Rotation * *Target;
		setPosition(D3DXVECTOR3(Result._41,Result._42,Result._43));
		OrthoSetup = true;
	}
}