/**
Пофрагментное освещение несколькими источниками света
+ использование текстуры
*/

#version 330

uniform sampler2D diffuseTex;

struct LightInfo
{
	vec3 pos; //положение источника света в системе координат ВИРТУАЛЬНОЙ КАМЕРЫ!
	vec3 La; //цвет и интенсивность окружающего света
	vec3 Ld; //цвет и интенсивность диффузного света
	vec3 Ls; //цвет и интенсивность бликового света
};
uniform LightInfo light[3];

in vec3 normalCamSpace; //нормаль в системе координат камеры (интерполирована между вершинами треугольника)
in vec4 posCamSpace; //координаты вершины в системе координат камеры (интерполированы между вершинами треугольника)
in vec2 texCoord; //текстурные координаты (интерполирована между вершинами треугольника)


out vec4 fragColor; //выходной цвет фрагмента


const vec3 Ks = vec3(1.0, 1.0, 1.0); //Коэффициент бликового отражения
const float shininess = 128.0;

void main()
{
        vec3 diffuseColor = texture(diffuseTex, texCoord).rgb;

	vec3 normal = normalize(normalCamSpace); //нормализуем нормаль после интерполяции
	vec3 viewDirection = normalize(-posCamSpace.xyz); //направление на виртуальную камеру (она находится в точке (0.0, 0.0, 0.0))
	
	vec3 color = vec3(0.0);
	
	for (int i = 0; i < 3; i++)
	{
		vec3 lightDirCamSpace = normalize(light[i].pos - posCamSpace.xyz); //направление на источник света

                // Для того, чтобы при увеличении расстояния свет тоже отдалялся:
                //float distance = length(light[i].pos - posCamSpace.xyz);
                //float attenuation = 1.0 / (0.1 + 0.01 * distance + 0.01 * distance * distance);


		float NdotL = max(dot(normal, lightDirCamSpace.xyz), 0.0); //скалярное произведение (косинус)

		color += (light[i].La + light[i].Ld * NdotL) * diffuseColor; //* attenuation; //цвет вершины

		if (NdotL > 0.0)
		{			
			vec3 halfVector = normalize(lightDirCamSpace.xyz + viewDirection); //биссектриса между направлениями на камеру и на источник света

			float blinnTerm = max(dot(normal, halfVector), 0.0); //интенсивность бликового освещения по Блинну				
			blinnTerm = pow(blinnTerm, shininess); //регулируем размер блика
			color += light[i].Ls * Ks * blinnTerm; //* attenuation;
		}
	}

	fragColor = vec4(color, 1.0);
}
