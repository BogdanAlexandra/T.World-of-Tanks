#include "object2D.h"

#include <vector>
#include <cmath>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/lab3/transform2D.h"

Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    
        vertices.push_back(VertexFormat(corner, color));
        vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
        vertices.push_back(VertexFormat(corner + glm::vec3(length, length, 0), color));
        vertices.push_back(VertexFormat(corner + glm::vec3(0, length, 0), color));


    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateRectangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    
    vertices.push_back(VertexFormat(corner, color));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, width, 0), color));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, width, 0), color));

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateDiamond(
    const std::string &name,
    glm::vec3 center,
    float size,
    float rectangleWidth,
    float rectangleHeight,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Define the vertices for the diamond.
    glm::vec3 top = center + glm::vec3(0, size, 0);
    glm::vec3 right = center + glm::vec3(size, 0, 0);
    glm::vec3 bottom = center + glm::vec3(0, -size, 0);
    glm::vec3 left = center + glm::vec3(-size, 0, 0);

    // Add diamond vertices.
    vertices.emplace_back(center, color);
    vertices.emplace_back(top, color);
    vertices.emplace_back(right, color);
    vertices.emplace_back(bottom, color);
    vertices.emplace_back(left, color);

    Mesh* shape = new Mesh(name);

    if (!fill) {
        shape->SetDrawMode(GL_LINE_LOOP);
        // Define the indices for the outline of the diamond.
        indices = {0, 1, 2, 3, 4, 1};
    } else {
        // Draw 4 triangles to fill the diamond. Define the indices for the triangles.
        indices = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1};
    }

    // Add vertices and indices for the rectangle to the right.
    glm::vec3 rectTopLeft = center + glm::vec3(0, rectangleHeight / 2, 0);
    glm::vec3 rectTopRight = rectTopLeft + glm::vec3(rectangleWidth, 0, 0);
    glm::vec3 rectBottomRight = rectTopRight - glm::vec3(0, rectangleHeight, 0);
    glm::vec3 rectBottomLeft = rectTopLeft - glm::vec3(0, rectangleHeight, 0);

    vertices.emplace_back(rectTopLeft, color);
    vertices.emplace_back(rectTopRight, color);
    vertices.emplace_back(rectBottomRight, color);
    vertices.emplace_back(rectBottomLeft, color);

    if (fill) {
        // Define the indices for the rectangle.
        indices.insert(indices.end(), {5, 6, 7, 7, 8, 5});
    }

    shape->InitFromData(vertices, indices);
    return shape;
}

Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float outerRadius,
    float innerRadius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int numPoints = 5; // Number of points for the star

    for (int i = 0; i < numPoints * 2; ++i) { 
        float radius = (i % 2 == 0) ? outerRadius / 2.0f : innerRadius / 4.0f; // Outer radius and inner radius
        float angle = glm::radians(198.0f) + i * glm::radians(360.0f / (numPoints * 2));
        glm::vec3 vertexPosition = glm::vec3(radius * 5 * cos(angle), radius * 5 * sin(angle), 0.0f);
        vertices.push_back(VertexFormat(vertexPosition, color));
        indices.push_back(i);
    }

    // Connect the last and first vertices to complete the star
    indices.push_back(0);

    // Translatare pentru a plasa centrul în locul dorit
    for (auto& vertex : vertices) {
        vertex.position += center;
    }

    Mesh* star = new Mesh(name);
    if (fill) {
        star->SetDrawMode(GL_TRIANGLE_FAN);
    }
    else {
        star->SetDrawMode(GL_LINE_LOOP);
    }

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreateHexagon(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < 6; ++i) {
        float angle = i * (2 * M_PI / 6);
        glm::vec3 vertexPosition = center + glm::vec3(radius * cos(angle), radius * sin(angle), 0);
        vertices.push_back(VertexFormat(vertexPosition, color));
        indices.push_back(i);
    }

    Mesh* hexagon = new Mesh(name);

    if (!fill) {
        hexagon->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Connect the vertices to form triangles
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(0);
        indices.push_back(4);
        indices.push_back(5);
    }

    hexagon->InitFromData(vertices, indices);
    return hexagon;
}


Mesh* object2D::CreateHeart(
    const std::string &name,
    glm::vec3 center,
    float size,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Define the heart shape vertices
    float x, y;
    for (float t = 0; t <= 2 * M_PI; t += 0.01) {
        x = size * (16 * pow(sin(t), 3));
        y = -size * (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t));

        // Apply a 180-degree rotation
        glm::mat2 rotationMatrix = glm::mat2(cos(glm::pi<float>()), -sin(glm::pi<float>()), sin(glm::pi<float>()), cos(glm::pi<float>()));
        glm::vec2 rotatedPoint = rotationMatrix * glm::vec2(x, y);
        vertices.push_back(VertexFormat(glm::vec3(rotatedPoint.x, rotatedPoint.y, 0), color));
    }

    // Create indices for triangles
    for (unsigned int i = 0; i < vertices.size() - 1; ++i) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(vertices.size() / 2); // Connect to the center vertex
    }

    // Connect the last vertex to the first one
    indices.push_back(vertices.size() - 1);
    indices.push_back(0);
    indices.push_back(vertices.size() / 2); // Connect to the center vertex

    Mesh* heart = new Mesh(name);

    if (!fill) {
        heart->SetDrawMode(GL_LINE_LOOP);
    } else {
        heart->SetDrawMode(GL_TRIANGLES);
    }

    heart->InitFromData(vertices, indices);
    return heart;
}




