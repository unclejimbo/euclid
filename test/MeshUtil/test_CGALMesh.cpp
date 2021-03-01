#include <catch2/catch.hpp>
#include <Euclid/MeshUtil/CGALMesh.h>

#include <string>
#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <Euclid/IO/OffIO.h>

#include <config.h>

using Kernel = CGAL::Simple_cartesian<double>;
using Point_3 = typename Kernel::Point_3;

TEST_CASE("MeshUtil, CGALMesh", "[meshutil][cgalmesh]")
{
    std::vector<double> positions;
    std::vector<unsigned> indices;
    std::string file_name(DATA_DIR);
    file_name.append("bunny.off");
    Euclid::read_off<3>(file_name, positions, nullptr, &indices, nullptr);

    std::vector<Point_3> points;
    for (size_t i = 0; i < positions.size(); i += 3) {
        points.emplace_back(positions[i], positions[i + 1], positions[i + 2]);
    }

    SECTION("CGAL::Surface_mesh with raw positions")
    {
        using Mesh = CGAL::Surface_mesh<Point_3>;
        Mesh mesh;
        Euclid::make_mesh<3>(mesh, positions, indices);
        std::vector<double> new_positions;
        std::vector<unsigned> new_indices;
        Euclid::extract_mesh<3>(mesh, new_positions, new_indices);

        REQUIRE(new_positions == positions);
        REQUIRE(new_indices == indices);
    }

    SECTION("CGAL::Surface_mesh with points")
    {
        using Mesh = CGAL::Surface_mesh<Point_3>;
        Mesh mesh;
        Euclid::make_mesh<3>(mesh, points, indices);
        std::vector<Point_3> new_points;
        std::vector<unsigned> new_indices;
        Euclid::extract_mesh<3>(mesh, new_points, new_indices);

        REQUIRE(new_points == points);
        REQUIRE(new_indices == indices);
    }

    SECTION("CGAL::Polyhedron_3 with raw positions")
    {
        using Mesh =
            CGAL::Polyhedron_3<Kernel, CGAL::Polyhedron_items_with_id_3>;
        Mesh mesh;
        Euclid::make_mesh<3>(mesh, positions, indices);
        CGAL::set_halfedgeds_items_id(mesh);
        std::vector<double> new_positions;
        std::vector<unsigned> new_indices;
        Euclid::extract_mesh<3>(mesh, new_positions, new_indices);

        REQUIRE(new_positions == positions);
        REQUIRE(new_indices == indices);
    }

    SECTION("CGAL::Polyhedron_3 with points")
    {
        using Mesh =
            CGAL::Polyhedron_3<Kernel, CGAL::Polyhedron_items_with_id_3>;
        Mesh mesh;
        Euclid::make_mesh<3>(mesh, points, indices);
        CGAL::set_halfedgeds_items_id(mesh);
        std::vector<Point_3> new_points;
        std::vector<unsigned> new_indices;
        Euclid::extract_mesh<3>(mesh, new_points, new_indices);

        REQUIRE(new_points == points);
        REQUIRE(new_indices == indices);
    }
}
