uniform vec4 FogColor;
uniform float FogEnd;
uniform float FogScale;

uniform vec3 MaterialEmit;
uniform float MaterialShininess;
uniform float MaterialOpacity;

uniform vec4 LightPosition[4];
uniform vec3 LightDiffuseProduct[4];
uniform vec3 LightSpecularProduct[4];
uniform float LightConstantAttenuation[4];
uniform float LightQuadraticAttenuation[4];
uniform bool LightActive[4];

uniform sampler2D Texture[8];

varying vec3 position, normal;


void main(void)
{
	vec3 diffuse = MaterialEmit;
	vec3 N = normalize(normal);

	if(LightActive[0])
	{
		vec3 lightDir = LightPosition[0].xyz - position;
		float lightDirLength = length(lightDir);

		vec3 L = lightDir / lightDirLength;

		float lambertTerm = max(dot(N, L), 0.0);

		// use texture 0 as a ramp shader
		vec4 ramp = texture2D(Texture[0], vec2(lambertTerm, 0.5));

		float attenuation = 1.0/(LightConstantAttenuation[0] + (lightDirLength*lightDirLength)* LightQuadraticAttenuation[0]);

		diffuse = LightDiffuseProduct[0] * ramp.xyz * attenuation;
	}

	vec4 finalColor = vec4(diffuse.x, diffuse.y, diffuse.z, MaterialOpacity);
	float fogFactor = clamp((FogEnd + position.z) * FogScale, 0.0, 1.0);
	gl_FragColor = mix(FogColor, finalColor, fogFactor);
}
