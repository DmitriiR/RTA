
float CalcSpecular(float3 pos,float3 toLight_dir, float3 nrm, float4 cam_position  )
{
	toLight_dir = normalize(toLight_dir);
	float3 refl = reflect(-toLight_dir, nrm);
	float3 dir_fromCamtopoint = normalize(cam_position.xyz - pos.xyz);

	float specular = pow(saturate(dot(refl, dir_fromCamtopoint)), 128);
	return specular;
}



float4 CalcDirLight( float3 surNormals, float4 surColor, float3 pos, float3 dir_lgt_pos)
{
	float3 light_dir = dir_lgt_pos;
	float4 light_ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 light_diffuse = float4(1.0f, 0.7f, 0.3f, 1.0f);

	float lightIntensity = saturate(dot(-light_dir, surNormals));
	float4 color = saturate(light_diffuse * surColor * lightIntensity);

	return color; 
}

// point light
float4 CalcPointLight(float3 toLight, float3 surNormals, float4 surColor, float4 lightColor)//, float lightIntensity_Normals)
{
	float4 returnColor;
	
	float min_dist = 1.0f;
	float max_dist = 10.0f;
	float distance = length(toLight);
	float strength = 1 - saturate((distance - min_dist) / (max_dist - min_dist));
	
	returnColor = strength * lightColor * surColor;//*lightIntensity_Normals;

	return returnColor;
}

float4 CalcSpotLight(
	float3 normal,
	float4 baseColor,
	float3 pos_spot,
	float4 wPos,
	float4 diffuse_color,
	float4 ambient_color,
	float  range,
	float3 att,
	float3 toLight,
	float3 dir_spot,
	float cone, 
	float4 cam_position
	)
{

	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightToPixelVec = pos_spot - wPos.xyz;
	
	float d = length(lightToPixelVec);
	       lightToPixelVec /= d;

	float SURFACERATIO = saturate(dot(-lightToPixelVec, dir_spot));
	float	SPOTFACTOR = (SURFACERATIO > cone) ? 1 : 0;
	float4 finalAmbient = saturate(baseColor * ambient_color);

	if (d > range)
		return float4(finalAmbient);

		

	float specular = CalcSpecular(wPos,lightToPixelVec, normal, cam_position);

	float LIGHTRATIO =  saturate(dot( lightToPixelVec, normal.xyz));

	finalColor = SPOTFACTOR *(LIGHTRATIO * diffuse_color * baseColor + finalAmbient + diffuse_color * specular); 
	
	
	//Return Final Color
	return float4(finalColor);
}