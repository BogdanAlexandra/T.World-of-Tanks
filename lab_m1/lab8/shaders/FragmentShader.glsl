#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int NUM_LIGHTS;
uniform vec3 light_positions[2];
uniform vec3 light_directions[2];
uniform vec3 light_colors[2];

// Spotlight parameters
uniform vec3 spotlight_direction;
uniform float spotlight_cutoff;
uniform int spotlight_mode;

uniform float cut_off_angle;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void calculateLight(int lightIndex, inout vec3 ambient_component, inout vec3 diffuse_component, inout vec3 specular_component)
{
    float ambient_light = 0.25;
    // Compute normalized light direction
    vec3 light_direction_normalized = normalize(light_positions[lightIndex] - world_position);

    // Compute normalized view direction
    vec3 view_direction_normalized = normalize(eye_position - world_position);

    // Compute normalized normal vector
    vec3 normal_vector = normalize(world_normal);

    // Compute reflection vector
    vec3 reflection_vector = reflect(-light_direction_normalized, normal_vector);

    // Ambient light component
    ambient_component += ambient_light * light_colors[lightIndex];

    // Diffuse light component
    float diffuse_intensity = max(dot(normal_vector, light_direction_normalized), 0.0);
    diffuse_component += material_kd * diffuse_intensity * light_colors[lightIndex];

    // Specular light component
    float specular_intensity = pow(max(dot(reflection_vector, view_direction_normalized), 0.0), material_shininess);
    specular_component += material_ks * specular_intensity * light_colors[lightIndex];
}

void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    float diffuse_light = 0;
    float specular_light = 0;

    vec3 ambient_component = vec3(0.0);
    vec3 diffuse_component = vec3(0.0);
    vec3 specular_component = vec3(0.0);

    // Handle the main light

     // Compute normalized light direction
    vec3 light_direction_normalized = normalize(light_position - world_position);

    // Compute normalized view direction
    vec3 view_direction_normalized = normalize(eye_position - world_position);

    // Compute normalized normal vector
    vec3 normal_vector = normalize(world_normal);

    // Compute reflection vector
    vec3 reflection_vector = reflect(-light_direction_normalized, normal_vector);

    // Define ambient light component
    ambient_component = ambient_light * object_color;

    // Define diffuse light component
    float diffuse_intensity = max(dot(normal_vector, light_direction_normalized), 0.0);
    diffuse_component = material_kd * diffuse_intensity * object_color;

    // Define specular light component
    float specular_intensity = pow(max(dot(reflection_vector, view_direction_normalized), 0.0), material_shininess);
    specular_component = material_ks * specular_intensity * object_color;


    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    if (spotlight_mode == 1) {
        float cut_off_rad = radians(cut_off_angle);
        float spotlight_factor = dot(-light_direction_normalized, spotlight_direction);
        float spot_light_limit	= cos(cut_off_rad);

        float spotlight_attenuation = smoothstep(spotlight_cutoff, spotlight_cutoff + 0.1, spotlight_factor);

        // Apply spotlight attenuation to diffuse and specular components
        diffuse_component *= spotlight_attenuation;
        specular_component *= spotlight_attenuation;
    }

    // Handle the other lights
    for (int i = 0; i < NUM_LIGHTS; ++i) {
        calculateLight(i, ambient_component, diffuse_component, specular_component);
    }

    // TODO(student): Compute the total light.
    
    vec3 total_light = ambient_component + diffuse_component + specular_component;

    // TODO(student): Write pixel out color
    out_color = vec4(total_light * object_color, 1);
}
