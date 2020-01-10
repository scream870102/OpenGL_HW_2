#version 130

//#define NPR
//#define SILHOUETTE

in vec3 fN;
in vec3 fL;
in vec3 fV;

// �H�U���s�W������
uniform vec4  LightInView;        // Light's position in View Space
uniform vec4  AmbientProduct;  // light's ambient  �P Object's ambient  �P ka �����n
uniform vec4  DiffuseProduct;  // light's diffuse  �P Object's diffuse  �P kd �����n
uniform vec4  SpecularProduct; // light's specular �P Object's specular �P ks �����n
uniform float fShininess;
uniform int   iLighting;
uniform vec4  vObjectColor;    // �N���󪺳�@�C��

void main()
{
	// ���ŧi diffuse �P specular
    vec4 diffuse = vec4(0.0,0.0,0.0,1.0);
	vec4 specular = vec4(0.0,0.0,0.0,1.0);
	
	if( iLighting != 1 ) {
		gl_FragColor = vObjectColor;
	}
	else {	
		// 1. �p�� Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La = 
		vec4 ambient = AmbientProduct; // m_sMaterial.ka * m_sMaterial.ambient * vLightI;

		// ���ƶǤJ�� Normal Dir
		vec3 vN = normalize(fN); 

		// 2. ���ƶǤJ�� Light Dir
		vec3 vL = normalize(fL); // normalize light vector

		// 5. �p�� L dot N
		float fLdotN = vL.x*vN.x + vL.y*vN.y + vL.z*vN.z;
		if( fLdotN >= 0 ) { // ���I�Q�����Ө�~�ݭn�p��
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse = fLdotN * DiffuseProduct; 
#else
			if( fLdotN >= 0.85 ) diffuse = 1.0 * DiffuseProduct;
			else if( fLdotN >= 0.55 ) diffuse = 0.55 * DiffuseProduct;
			else diffuse = 0.35 * DiffuseProduct;
#endif

			// Specular color
			// Method 1: Phone Model
			// �p�� View Vector
			// ���ƶǤJ�� fV , View Direction
			vec3 vV = normalize(fV);

			//�p�� Light �� �Ϯg�� vRefL = 2 * fLdotN * vN - L
			// �P�ɧQ�� normalize �ন���V�q
			vec3 vRefL = normalize(2.0f * (fLdotN) * vN - vL);

			//   �p��  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular = SpecularProduct * pow(RdotV, fShininess); 
#else
			specular = vec4(0.0,0.0,0.0,1.0); 
#endif
		}

		gl_FragColor = ambient + diffuse + specular;  // �p���C�� ambient + diffuse + specular;
		gl_FragColor.w = 1.0;	// �]�w alpha �� 1.0
		// gl_FragColor = vec4((ambient + diffuse + specular).xyz, 1.0);
#ifdef SILHOUETTE
	vec4 edgeColor = vec4(1.0, 0.0, 0.0, 1.0);
	if( abs(dot(normalize(fN), normalize(fV))) < 0.2)  gl_FragColor = edgeColor;
#endif

	}
}
