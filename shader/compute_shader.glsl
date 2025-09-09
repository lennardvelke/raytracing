#version 430
layout(local_size_x = 16, local_size_y = 16) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

void main() {
    // Get the current pixel coordinates
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    
    // Normalize coordinates to [0,1]
    vec2 uv = vec2(pixel_coords) / vec2(800.0, 600.0);
    
    // Simple pattern: gradient with some variation
    vec3 color = vec3(
        0.5 + 0.5 * sin(uv.x * 10.0),
        0.5 + 0.5 * cos(uv.y * 10.0),
        0.5 + 0.5 * sin(uv.x * uv.y * 20.0)
    );
    
    // Write to output image
    imageStore(img_output, pixel_coords, vec4(color, 1.0));
}
