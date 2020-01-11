#include "CCamera.h"

CCamera* CCamera::_pCamera = nullptr;

CCamera::CCamera() {
	_type = Type::PERSPECTIVE;
	_bViewDirty = true;
	_bProjectionDirty = true;
}

void CCamera::destroyInstance()
{
	CC_SAFE_DELETE(_pCamera);
}

void CCamera::Move(Direction direction)
{
	vec4 viewDirection(_lookAt.x - _viewPosition.x, _lookAt.y - _viewPosition.y, _lookAt.z - _viewPosition.z, 0.0f);
	viewDirection = normalize(viewDirection);
	vec4 viewPosition = _viewPosition;
	vec4 lookAt = _lookAt;
	vec4 dir;
	if (direction == Direction::FORWARD)
		dir = viewDirection;
	else if (direction == Direction::BACKWARD)
		dir = -viewDirection;
	else if (direction == Direction::LEFT)
		dir = normalize(Angel::cross4(_upVector, viewDirection));
	else if (direction == Direction::RIGHT)
		dir = -normalize(Angel::cross4(_upVector, viewDirection));
	viewPosition += moveSpeed * dir;
	lookAt += moveSpeed * dir;
	if (viewPosition.z > -8.5f && viewPosition.z < 8.5f && viewPosition.x > -8.5f && viewPosition.x < 8.5f) {
		updateViewLookAt(viewPosition, lookAt);
	}
}

CCamera::~CCamera()
{

}

// �w�]�O�H�z����v�Aview �b (0,0,0) �ݦV -Z ��V
// fov = 60��
CCamera* CCamera::create()
{
	if (_pCamera == nullptr) {
		_pCamera = new (std::nothrow) CCamera;
		_pCamera->initDefault();
	}
	return _pCamera;
}

void  CCamera::initDefault()
{
	// �]�w�w�]���Y����m
	_viewPosition = vec4(0, 0, 0, 1.0f);
	_lookAt = vec4(0, 0, -1.0f, 1.0f);
	_upVector = vec4(0, 1.0f, 0, 1.0f);
	// ���� View Matrix
	_view = LookAt(_viewPosition, _lookAt, _upVector);

	// �H�`�Ϊ��j�p�}�ҳz����v�@���w�]�����Y��v�覡
	_projection = Perspective(60.0, 1.0f, 1.0, 1000.0);
	_bViewDirty = true;
	_bProjectionDirty = true;
	_viewProjection = _projection * _view;	// �i�H�����έp��
	moveSpeed = 0.1f;
}

CCamera* CCamera::getInstance()
{
	if (_pCamera == nullptr) {
		_pCamera = new (std::nothrow) CCamera;
		_pCamera->initDefault();
	}
	return _pCamera;
}

void CCamera::updatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	_projection = Perspective(60.0, 1.0f, 1.0, 1000.0);
	_bProjectionDirty = true;
	_type = Type::PERSPECTIVE;
	_viewProjection = _projection * _view; // �P�ɧ�s  viewProjection matrix (�i�H�����έp��)
}

void CCamera::updateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	_projection = Ortho(left, right, bottom, top, nearPlane, farPlane);
	_bProjectionDirty = true;
	_type = Type::ORTHOGRAPHIC;
	_viewProjection = _projection * _view; // �P�ɧ�s  viewProjection matrix (�i�H�����έp��)
}

void CCamera::updateViewPosition(vec4& vp)
{
	// ���� View Matrix
	_viewPosition = vp;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view; // �i�H�����έp��
}
void CCamera::updateLookAt(vec4& at)
{
	// ���� View Matrix
	_lookAt = at;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view;  // �i�H�����έp��
}

void CCamera::updateViewLookAt(vec4& vp, vec4& at)
{
	// ���� View Matrix
	_viewPosition = vp;
	_lookAt = at;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view; // �i�H�����έp��
}

const mat4& CCamera::getProjectionMatrix(bool& bProj) const
{
	bProj = _bProjectionDirty;
	if (_bProjectionDirty) _bProjectionDirty = false; // ���s���o �N�]�w�� false
	return _projection;
}
const mat4& CCamera::getViewMatrix(bool& bView) const
{
	bView = _bViewDirty;
	if (_bViewDirty) _bViewDirty = false; // ���s���o �N�]�w�� false
	return _view;
}

const mat4& CCamera::getViewProjectionMatrix() const
{
	return _viewProjection;
}

CCamera::Type CCamera::getProjectionType()
{
	return _type;
}