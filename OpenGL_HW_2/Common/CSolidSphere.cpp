#include "CSolidSphere.h"

CSolidSphere::CSolidSphere(const GLfloat fRadius, const int iSlices,const  int iStacks)
{
    GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat) iStacks;  
    GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat) iSlices;  
    GLfloat ds = 1.0f / (GLfloat) iSlices;  
    GLfloat dt = 1.0f / (GLfloat) iStacks;  
    GLfloat t = 1.0f;      
    GLfloat s = 0.0f;  
    GLint i, j;     // Looping variables  
	int idx = 0; // �x�s vertex ���Ǫ����ޭ�

	m_fRadius = fRadius;
	m_iSlices = iSlices;
	m_iStacks = iStacks;
	m_iNumVtx = iStacks*(2*(iSlices+1));

	m_pPoints = NULL; m_pNormals = NULL; m_pTex = NULL;

	m_pPoints  = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];
	m_pColors  = new vec4[m_iNumVtx]; 
	m_pTex     = new vec2[m_iNumVtx];


	for (i = 0; i < iStacks; i++ ) {  
		GLfloat rho = (GLfloat)i * drho;  
		GLfloat srho = (GLfloat)(sin(rho));  
		GLfloat crho = (GLfloat)(cos(rho));  
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));  
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));  
		
		// Many sources of OpenGL sphere drawing code uses a triangle fan  
		// for the caps of the sphere. This however introduces texturing   
		// artifacts at the poles on some OpenGL implementations  
		s = 0.0f;  
		for ( j = 0; j <= iSlices; j++) {  
            GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;  
            GLfloat stheta = (GLfloat)(-sin(theta));  
            GLfloat ctheta = (GLfloat)(cos(theta));  
  
            GLfloat x = stheta * srho;  
            GLfloat y = ctheta * srho;  
            GLfloat z = crho;  
              
			m_pPoints[idx].x = x * m_fRadius;
			m_pPoints[idx].y = y * m_fRadius;
			m_pPoints[idx].z = z * m_fRadius;
			m_pPoints[idx].w = 1;

			m_pNormals[idx].x = x;
			m_pNormals[idx].y = y;
			m_pNormals[idx].z = z;

			m_pTex[idx].x = s;
			m_pTex[idx].y = t; // �]�w�K�Ϯy��
			idx++;

            x = stheta * srhodrho;  
            y = ctheta * srhodrho;  
            z = crhodrho;

			m_pPoints[idx].x = x * m_fRadius;
			m_pPoints[idx].y = y * m_fRadius;
			m_pPoints[idx].z = z * m_fRadius;
			m_pPoints[idx].w = 1;

			m_pNormals[idx].x = x;
			m_pNormals[idx].y = y;
			m_pNormals[idx].z = z;

			m_pTex[idx].x = s;
			m_pTex[idx].y = t - dt; // �]�w�K�Ϯy��
			idx++;
			s += ds; 
		}  
		t -= dt;  
	}  

	// �w�]�N�Ҧ��������]�w���Ǧ�
	for( int i = 0 ; i < m_iNumVtx ; i++ ) m_pColors[i] = vec4(-1.0f,-1.0f,-1.0f,1.0f);

#ifdef LIGHTING_WITHCPU
	// Default Set shader's name
	SetShaderName("vsLighting_CPU.glsl", "fsLighting_CPU.glsl");
#else // lighting with GPU
#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsLighting_GPU.glsl", "fsLighting_GPU.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif
#endif  

	// Create and initialize a buffer object �A�N���������]�w���J SetShader ��
	// CreateBufferObject();

	// �]�w����
	SetMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetKaKdKsShini(0, 0.8f, 0.2f, 1);
}

// �^�a�ۤv�g
void CSolidSphere::RenderWithFlatShading(vec4 vLightPos, color4 vLightI)
{


}
void CSolidSphere::RenderWithFlatShading(const LightSource &Lights)
{


}

void CSolidSphere::RenderWithGouraudShading(const LightSource &Lights)
{
	// Method 1 : ��C�@�� Vertex ���p���C��
	for (int i = 0; i < m_iStacks; i++) {
		for (int j = 0; j < 2 * (m_iSlices + 1); j++) {
			m_pColors[i * 2 * (m_iSlices + 1) + j] = PhongReflectionModel(m_pPoints[i * 2 * (m_iSlices + 1) + j], m_pNormals[i * 2 * (m_iSlices + 1) + j], Lights);
		}
	}

	// Method 2 : ���|�� Vertex �ϥΫe�@���p�⪺�C��
	//���p��Ĥ@�� Stack ���C��
	//for( int j = 0 ; j < 2*(m_iSlices+1) ; j++ ) {
	//	m_pColors[j] = PhongReflectionModel(m_pPoints[j], m_pNormals[j], vLightPos, vLightI);
	//}
	//// ���� Stack �� vertex �C��A�s������(�t 0) �ϥΫe�@�� stack �s��+1�� �C��
	//// �s���_�ƴN�����p���C��
	//// �C�@�� Slices �̫��� vertex ��}�Y�e��� vertex ���|�A�ҥH�ϥθӨ�� vertex ���C��
	//for (int i = 1; i < m_iStacks; i++ ) {  
	//	for( int j = 0 ; j < 2*(m_iSlices+1) - 2 ; j++ ) {
	//		if( j%2 ) m_pColors[i*2*(m_iSlices+1)+j] = PhongReflectionModel(m_pPoints[i*2*(m_iSlices+1)+j], m_pNormals[i*2*(m_iSlices+1)+j], vLightPos, vLightI);
	//		else m_pColors[i*2*(m_iSlices+1)+j] =  m_pColors[(i-1)*2*(m_iSlices+1)+j+1];		
	//	}
	//	m_pColors[(i+1)*2*(m_iSlices+1)-2] = m_pColors[i*2*(m_iSlices+1)];
	//	m_pColors[(i+1)*2*(m_iSlices+1)-1] = m_pColors[i*2*(m_iSlices+1)+1];
	//}

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void CSolidSphere::RenderWithGouraudShading(vec4 vLightPos, color4 vLightI)
{
	// Method 1 : ��C�@�� Vertex ���p���C��
	 for (int i = 0; i < m_iStacks; i++ ) {  
		 for( int j = 0 ; j < 2*(m_iSlices+1) ; j++ ) {
			 m_pColors[i * 2 * (m_iSlices + 1) + j] = PhongReflectionModel(m_pPoints[i * 2 * (m_iSlices + 1) + j], m_pNormals[i * 2 * (m_iSlices + 1) + j], vLightPos, vLightI);
		 }
	 } 

	// Method 2 : ���|�� Vertex �ϥΫe�@���p�⪺�C��
	 //���p��Ĥ@�� Stack ���C��
	//for( int j = 0 ; j < 2*(m_iSlices+1) ; j++ ) {
	//	m_pColors[j] = PhongReflectionModel(m_pPoints[j], m_pNormals[j], vLightPos, vLightI);
	//}
	//// ���� Stack �� vertex �C��A�s������(�t 0) �ϥΫe�@�� stack �s��+1�� �C��
	//// �s���_�ƴN�����p���C��
	//// �C�@�� Slices �̫��� vertex ��}�Y�e��� vertex ���|�A�ҥH�ϥθӨ�� vertex ���C��
	//for (int i = 1; i < m_iStacks; i++ ) {  
	//	for( int j = 0 ; j < 2*(m_iSlices+1) - 2 ; j++ ) {
	//		if( j%2 ) m_pColors[i*2*(m_iSlices+1)+j] = PhongReflectionModel(m_pPoints[i*2*(m_iSlices+1)+j], m_pNormals[i*2*(m_iSlices+1)+j], vLightPos, vLightI);
	//		else m_pColors[i*2*(m_iSlices+1)+j] =  m_pColors[(i-1)*2*(m_iSlices+1)+j+1];		
	//	}
	//	m_pColors[(i+1)*2*(m_iSlices+1)-2] = m_pColors[i*2*(m_iSlices+1)];
	//	m_pColors[(i+1)*2*(m_iSlices+1)-1] = m_pColors[i*2*(m_iSlices+1)+1];
	//}

	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

// ���B�ҵ��� vLightPos �����O�@�ɮy�Ъ��T�w�����m
void CSolidSphere::Update(float dt, point4 vLightPos, color4 vLightI)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated ) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if( m_iMode == FLAT_SHADING )  RenderWithGouraudShading(vLightPos, vLightI);
	else RenderWithGouraudShading(vLightPos, vLightI);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * vLightPos;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct  = m_Material.ka * m_Material.ambient  * vLightI;
	m_DiffuseProduct  = m_Material.kd * m_Material.diffuse  * vLightI;
	m_SpecularProduct = m_Material.ks * m_Material.specular * vLightI;
#endif
}

void CSolidSphere::Update(float dt, const LightSource &Lights)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING)  RenderWithGouraudShading(Lights);
	else RenderWithGouraudShading(Lights);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * Lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * Lights.ambient;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * Lights.diffuse;
	m_SpecularProduct = m_Material.ks * m_Material.specular * Lights.specular;
#endif
}

void CSolidSphere::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITView = InverseTransposeMatrix(m_mxMVFinal);
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}

void CSolidSphere::Draw()
{
	DrawingSetShader();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	for (int i = 0; i < m_iStacks; i++ ) {  
		glDrawArrays( GL_TRIANGLE_STRIP, i*(2*(m_iSlices+1)), 2*(m_iSlices+1) );
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}

void CSolidSphere::DrawW()
{
	DrawingWithoutSetShader();
	for (int i = 0; i < m_iStacks; i++ ) {  
		glDrawArrays( GL_TRIANGLE_STRIP, i*(2*(m_iSlices+1)), 2*(m_iSlices+1) );
	}
}

CSolidSphere::~CSolidSphere()
{

}