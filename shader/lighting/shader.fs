#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;
    float cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

out vec4 fragColor;

in vec3 fragPos;
in vec3 Normal;

// in vec3 lightPosition;
in vec2 texCoords;

uniform float time;

uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.cutoff)
    {

        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

        // diffuse
        vec3 norm = normalize(Normal);
        //vec3 lightDir = normalize(-light.direction);              // Directional light
        float diff = max(dot(norm, lightDir), 0.0f);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoords).rgb;

        // specular
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

        // attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;


        /*
        // emission
        vec3 emission = vec3(0.0);

        if (texture(material.specular, texCoords).r == 0.0)
        {
            emission = texture(material.emission, texCoords).rgb;


            
            emission = texture(material.emission, texCoords + vec2(0.0, time * 0.5)).rgb;
            float t = emission.g;
            emission.g = emission.b; 
            emission.b = t; 

            emission = emission * (sin(time * 7.0) * 0.5 + 0.5) * 1.0;
           
        }
        */

        vec3 result = ambient + diffuse + specular;

        fragColor = vec4(result, 1.0f);
    }
    else
    {
        fragColor = vec4(light.ambient * texture(material.diffuse, texCoords).rgb, 1.0f);
    }
}
