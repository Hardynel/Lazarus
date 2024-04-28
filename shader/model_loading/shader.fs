#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 fragment_color;

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

uniform vec3 view_position;

uniform Material material;
uniform PointLight point_light;

vec3 calculate_point_light(PointLight light,
                        vec3 normal,
                        vec3 fragment_position,
                        vec3 view_direction);

void main()
{
    vec3 normal_v = normalize(normal);
    vec3 view_direction = normalize(view_position - fragment_position);

    vec3 result = calculate_point_light(point_light, normal_v, fragment_position, view_direction);

    fragment_color = vec4(result, 1.0);
}

vec3 calculate_point_light(PointLight light,
                        vec3 normal,
                        vec3 fragment_position,
                        vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position); 

    // diffuse
    float diff = max(dot(normal, light_direction), 0.0);

    // specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragment_position);
    float attenuation =
        1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    vec3 ambient = light.ambient * texture(material.texture_diffuse1, texture_coordinates).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, texture_coordinates).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular1, texture_coordinates).rgb;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    return result;
}
