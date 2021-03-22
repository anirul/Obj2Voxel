#include <algorithm>
#include <iostream>
#include <cassert>
#include <limits>
#include <set>
#include "../lib/Obj.h"

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << "glm::vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

const std::vector<glm::vec3> GetPointFromObj(
    const std::string& path, 
    const std::string& file)
{
    voxel::Obj obj(path, file);
    const auto& meshes = obj.GetMeshes();
    std::cout << "Found #" << meshes.size() << " meshes.\n";
    std::int64_t total_vertices = 0;
    bool initialized = false;
    glm::vec3 min;
    glm::vec3 max;
    std::vector<glm::vec3> points;
    for (const auto& mesh : meshes)
    {
        const auto& vertices = mesh.GetVertices();
        std::cout << "\tmesh has #" << vertices.size() << " vertices.\n";
        total_vertices += vertices.size();
        if (!initialized)
        {
            min = vertices[0].point;
            max = vertices[0].point;
            initialized = true;
        }
        for (const auto& element : vertices)
        {
            glm::vec3 temp = element.point;
            min = glm::min(min, temp);
            max = glm::max(max, temp);
            points.push_back(temp);
        }
    }
    std::cout << "Box: " << glm::abs(max - min) << "\n";
    return points;
}

std::pair<float, glm::vec3> GetMinDistance(
    const glm::vec3& vec, 
    const std::vector<glm::vec3>& points) 
{
    float min = std::numeric_limits<float>::max();
    glm::vec3 point;
    for (const auto& p : points)
    {
        float l = glm::length(p - vec);
        min = std::min(min, l);
        if (min == l)
        {
            point = p;
        }
    }
    return { min, point };
}

int main(int ac, char** av)
{
    try 
    {
        assert(ac == 3);
        const auto points = GetPointFromObj(av[1], av[2]);
        std::cout << "Total: #" << points.size() << "\n";
    } 
    catch(std::exception& ex) 
    {
        std::cout << "std::exception: " << ex.what() << "\n";
    }
    return 0;
}
