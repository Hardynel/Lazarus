#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 direction;
    vec3 position;

    float cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

out vec4 fragment_color;

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coordinates;

uniform vec3 view_position;

uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform SpotLight spot_light;

vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction);
vec3 calculate_point_light(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction);
vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main()
{
    vec3 normal_v = normalize(normal);
    vec3 view_direction = normalize(view_position - fragment_position);

    vec3 result = calculate_directional_light(directional_light, normal_v, view_direction);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        result += calculate_point_light(point_lights[i], normal_v, fragment_position, view_direction);
    }

    result += calculate_spot_light(spot_light, normal_v, fragment_position, view_direction);

    fragment_color = vec4(result, 1.0);
}

vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);

    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.diffuse, texture_coordinates).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texture_coordinates).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, texture_coordinates).rgb;

    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position); 
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragment_position);
    float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * texture(material.diffuse, texture_coordinates).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texture_coordinates).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, texture_coordinates).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    return result;
}

vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position);

    // intensity
    float theta = dot(light_direction, normalize(-light.direction));
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);

    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragment_position);
    float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * texture(material.diffuse, texture_coordinates).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texture_coordinates).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, texture_coordinates).rgb;

    ambient = ambient * attenuation * intensity;
    diffuse = diffuse * attenuation * intensity;
    specular = specular * attenuation * intensity;

    vec3 result = ambient + diffuse + specular;

    return result;
}
