#ifndef __cameraH__
#define __cameraH__
#include "base.h"
#include <d3dx9.h>

class Camera
{
public:
	enum CameraType { GROUND, AIRWATER, ORTHO };

	
	~Camera();
	void Setup(IDirect3DDevice9* Device);
	void Setup(IDirect3DDevice9* Device, CameraType cameraType);

	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	void zoom(float units);   // zoom in/out (for OTHRO only)

	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void setCameraType(CameraType cameraType);
	void getPosition(D3DXVECTOR3* pos);
	void setPosition(D3DXVECTOR3 pos);

	void setFOV(float FOV);
	void getViewport(D3DVIEWPORT9* Viewport);
	void setViewportDiamensions(float X, float Y, float Width, float Height);
	void getProjectionMatrix(D3DXMATRIX* Projection);
	void getViewMatrix(D3DXMATRIX* View);

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);
	void setLook(D3DXVECTOR3 look);

	void getYawAngle(float* angle);
	void setYawAngle(float angle);
	void getPitchAngle(float* angle);
	void setPitchAngle(float angle);

	// ORTHO functions
	void getTarget(D3DXVECTOR3* target);
	void setTarget(D3DXVECTOR3 target);
	void setTarget(D3DXMATRIX* target);
	void getZoom(float *units);
	void setZoom(float units, bool SetToUnit);
	void UpdateAnchor();
	void UpdateCamera();

private:
	IDirect3DDevice9* Device;
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
	float YawAngle;
	float PitchAngle;
	float FOV;
	D3DVIEWPORT9 Viewport;
	D3DXMATRIX Projection;
	D3DXMATRIX View;


	// ORTHO variables
	bool NewTarget;
	bool OrthoSetup;
	D3DXMATRIX* Target;
	D3DXVECTOR3 RotVec;
	D3DXMATRIX Rotation;
	D3DXMATRIX Anchor;
	D3DXMATRIX Result;
	float Zoom;
};
#endif // __cameraH__