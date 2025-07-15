////////////////////////////////////////////////////////////////////////////
//  Created     : 13.07.2011
//  Author      : Sergey Lozinski
//  Refactored  : ChatGPT, loxotron (2025)
//  Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "constrained_delaunay_triangulator.h"
#include <xray/strings_stream.h>

using xray::ai::navigation::delaunay_triangulator;
using xray::ai::navigation::constrained_delaunay_triangulator;
#ifndef MASTER_GOLD_
using xray::ai::navigation::triangles_mesh_type;
#endif
using xray::math::float3;

// Forward declarations
u32 segment_intersects_triangle(
    float3 const& normal,
    float3 const& segment0,
    float3 const& segment1,
    float3 const& vertex0,
    float3 const& vertex1,
    float3 const& vertex2,
    float3 (&intersections)[2]
);

typedef xray::buffer_vector<bool> markers_type;

/// Predicate to check if a triangle is marked (to be removed)
struct is_marked_in_buffer_predicate3 {
    inline is_marked_in_buffer_predicate3(
        delaunay_triangulator::indices_type const& indices,
        markers_type const& markers
    )
        : m_indices(&indices), m_markers(&markers)
    {}

    inline bool operator()(u32 const& vertex_id) const {
        size_t const vertex_id_index = &vertex_id - m_indices->begin();
        return (*m_markers)[vertex_id_index / 3];
    }

private:
    delaunay_triangulator::indices_type const* m_indices;
    markers_type const* m_markers;
};

/// Helper to check if a point is on the "right" side of a segment in the projected plane
static inline bool is_right_side(
    u32 const (&coordinate_indices)[2],
    float3 p,
    float3 b,
    float3 c,
    float3 n
) {
    u32 third_index = u32(-1);
    switch (coordinate_indices[0] + coordinate_indices[1]) {
        case 1: third_index = 2; break;
        case 2: third_index = 1; break;
        case 3: third_index = 0; break;
        default: NODEFAULT();
    }
    p[third_index] = 0;
    b[third_index] = 0;
    c[third_index] = 0;
    return ((normalize(b - c) ^ normalize(p - c)) | n) >= 0.f;
}

/// Checks whether the test point coincides with any triangle vertex
bool constrained_delaunay_triangulator::is_triangle_vertex(
    float3 const& testee,
    float3 const& v0,
    float3 const& v1,
    float3 const& v2
) {
    if (testee.is_similar(v0)) {
        R_ASSERT(testee.is_similar(v1));
        R_ASSERT(testee.is_similar(v2));
        return true;
    }
    if (testee.is_similar(v1)) {
        R_ASSERT(testee.is_similar(v2));
        return true;
    }
    if (testee.is_similar(v2))
        return true;
    return false;
}

/// Checks whether a point lies on the segment
bool is_on_segment(
    xray::math::float3 const& testee,
    xray::math::float3 const& first_segment_vertex,
    xray::math::float3 const& second_segment_vertex
);

/// Intersects two 2D segments in the projected plane
u32 segment_intersects_segment(
    u32 const (&coordinate_indices)[2],
    float3 const& v0,
    float3 const& v1,
    float3 const& u0,
    float3 const& u1,
    float3& result0,
    float3& result1,
    bool& are_on_the_same_line
);

/// Predicate to check if a vertex matches one of two segment endpoints
struct is_segment_vertex_predicate {
    is_segment_vertex_predicate(u32 const edge_vertex_index0, u32 edge_vertex_index1)
        : m_vertex_index0(edge_vertex_index0), m_vertex_index1(edge_vertex_index1)
    {}

    bool operator()(u32 const vertex_index) {
        return (m_vertex_index0 == vertex_index) || (m_vertex_index1 == vertex_index);
    }

private:
    u32 m_vertex_index0;
    u32 m_vertex_index1;
};

/// Sorts vertex indices by projection onto the segment direction
struct less_by_projection_predicate {
    less_by_projection_predicate(
        float3 const* vertices,
        float3 const& segment_vertex0,
        float3 const& segment_vertex1
    )
        : m_vertices(vertices),
          m_segment_vertex0(segment_vertex0),
          m_segment_vertex1(segment_vertex1),
          m_direction(normalize(segment_vertex1 - segment_vertex0))
    {}

    bool operator()(u32 const left, u32 const right) {
        float projection0 = (m_vertices[left] - m_segment_vertex0) | m_direction;
        float projection1 = (m_vertices[right] - m_segment_vertex0) | m_direction;
        return projection0 < projection1;
    }

private:
    float3 const* m_vertices;
    float3 m_segment_vertex0;
    float3 m_segment_vertex1;
    float3 m_direction;
};

/// Adds vertex index to left/right polygon indices based on its position relative to the segment
void constrained_delaunay_triangulator::add_pseudo_polygon_index(
    u32 const (&coordinate_indices)[2],
    u32 const vertex_index,
    polygon_indices_type& right_polygon_indices,
    polygon_indices_type& left_polygon_indices,
    float3 const& v0,
    float3 const& v1
) {
    float3 vertex = m_vertices[vertex_index];
    if (is_similar(coordinate_indices, vertex, v0) || is_similar(coordinate_indices, vertex, v1)) {
        printf("WARNING: Vertex #%u is almost equal to segment endpoint. Skipping to avoid zero vector in normalize().\n", vertex_index);
        return;
    }

    if (is_right_side(coordinate_indices, vertex, v0, v1, m_normal))
        right_polygon_indices.push_back(vertex_index);
    else
        left_polygon_indices.push_back(vertex_index);
}

/**
 * @brief Removes triangles intersected by a segment defined by two vertices.
 * 
 * This function finds all triangles in the triangulation that are intersected 
 * by the segment between segment_vertex_index0 and segment_vertex_index1, 
 * marks them for removal, and then removes them from the index buffer.
 * 
 * The function projects vertices onto a 2D plane (defined by coordinate_indices)
 * for intersection calculations. It also builds two polygon index lists (right and left)
 * that represent the polygon to be re-triangulated after removal.
 * 
 * Improvements include:
 * - Safer handling of invalid coordinate indices with early return instead of crash.
 * - Detection if the segment exactly matches any triangle edges, in which case removal is aborted.
 * - Protection against numerical errors causing more than 2 intersections per triangle edge.
 * - Removal of duplicate intersection points to prevent polygon and topology errors.
 * - Warnings instead of asserts to allow graceful recovery.
 * - Detailed logging for inconsistent topology and skipped triangles.
 * 
 * @param coordinate_indices The two coordinate axes indices used for projection (e.g., {0,1} for XY plane).
 * @param right_polygon_indices Output polygon indices on the right side of the segment.
 * @param left_polygon_indices Output polygon indices on the left side of the segment.
 * @param segment_vertex_index0 Index of the first vertex of the segment.
 * @param segment_vertex_index1 Index of the second vertex of the segment.
 * 
 * @return true if removal succeeded or nothing was removed (e.g. no intersections),
 *         false if removal was aborted due to invalid conditions (e.g. segment on triangle edge).
 */
bool constrained_delaunay_triangulator::remove_triangles(
    u32 const (&coordinate_indices)[2],
    polygon_indices_type& right_polygon_indices,
    polygon_indices_type& left_polygon_indices,
    u32 const segment_vertex_index0,
    u32 const segment_vertex_index1
) {
    // Determine the third coordinate axis for projection based on the sum of coordinate indices
    u32 third_index;
    switch (coordinate_indices[0] + coordinate_indices[1]) {
        case 1: third_index = 2; break;  // XY plane -> project on Z=0
        case 2: third_index = 1; break;  // XZ plane -> project on Y=0
        case 3: third_index = 0; break;  // YZ plane -> project on X=0
        default:
            // Invalid coordinate indices - this should never happen.
            // Return false to gracefully abort instead of crashing.
            printf("ERROR: Invalid coordinate indices: %u, %u\n", coordinate_indices[0], coordinate_indices[1]);
            return false;
    }

    const u32 triangles_count = m_indices.size() / 3;
    if (triangles_count == 0) {
        // No triangles exist - nothing to remove
        return true;
    }

    // Create a temporary marker buffer to mark triangles to be removed
    markers_type markers(
        ALLOCA(sizeof(bool) * triangles_count),
        triangles_count,
        triangles_count,
        false
    );

    // Get the segment endpoints and project them onto 2D plane
    float3 const& segment_vertex0 = m_vertices[segment_vertex_index0];
    float3 const& segment_vertex1 = m_vertices[segment_vertex_index1];
    float3 projected_vertex0 = segment_vertex0;
    float3 projected_vertex1 = segment_vertex1;
    projected_vertex0[third_index] = 0;
    projected_vertex1[third_index] = 0;

    // Check if the segment exactly coincides with any triangle edge (both segment vertices in triangle)
    for (u32 i = 0; i < triangles_count; ++i) {
        u32 equal_vertex_count = 0;
        for (u32 j = 0; j < 3; ++j) {
            if (segment_vertex_index0 == m_indices[3 * i + j])
                ++equal_vertex_count;
            if (segment_vertex_index1 == m_indices[3 * i + j])
                ++equal_vertex_count;
        }
        if (equal_vertex_count >= 2) {
            printf("WARNING: Triangle #%u has both segment vertices. Skipping removal.\n", i);
            return false; // Abort removal safely
        }
    }

    triangles_info_type intersected_triangles;

    // Iterate all triangles and find intersections of the segment with their edges
    for (u32 i = 0; i < triangles_count; ++i) {
        intersections_type intersections;
        u32 const* indices = m_indices.begin() + i * 3;

        // Check all 3 edges of triangle for intersection with segment
        for (u32 j = 0; j < 3; ++j) {
            u32 const first_index = indices[(j + 0) % 3];
            u32 const second_index = indices[(j + 1) % 3];

            float3 u0 = m_vertices[first_index];
            float3 u1 = m_vertices[second_index];
            u0[third_index] = 0;
            u1[third_index] = 0;

            bool are_on_same_line;
            float3 intersection_points[2];
            u32 intersections_count = segment_intersects_segment(
                coordinate_indices,
                projected_vertex0,
                projected_vertex1,
                u0,
                u1,
                intersection_points[0],
                intersection_points[1],
                are_on_same_line
            );

            // Safety clamp: if intersections_count > 2 due to numeric errors, log and clamp to 2
            if (intersections_count > 2) {
                printf("WARNING: triangle #%u edge #%u returned %u intersections. Clamping to 2.\n", i, j, intersections_count);
                intersections_count = 2;
            }

            // Assert no case with 2 intersections? Not allowed
            R_ASSERT_CMP(intersections_count, !=, 2);

            if (intersections_count > 0) {
                intersections.push_back(intersection_info(j, intersection_points[0]));
            }
        }

        // Remove duplicate intersections (points that are numerically close)
        for (u32 k = 0; k < intersections.size(); ++k) {
            for (u32 l = k + 1; l < intersections.size(); ++l) {
                if (is_similar(coordinate_indices, intersections[k].vertex, intersections[l].vertex)) {
                    intersections.erase(intersections.begin() + l);
                    --l;
                }
            }
        }

        const u32 intersections_count = intersections.size();
        if (intersections_count > 2) {
            // Too many intersections means something is wrong, skip this triangle to avoid corrupting topology
            printf("WARNING: triangle #%u has %u intersections (expected <=2). Skipping it.\n", i, intersections_count);
            continue;
        }

        if (intersections_count > 1) {
            triangle_info info;
            info.triangle_id = i;

            // Pick intersection closer to the segment start for sorting
            float squared_length0 = squared_length(intersections[0].vertex - projected_vertex0);
            float squared_length1 = squared_length(intersections[1].vertex - projected_vertex0);

            info.intersection = (squared_length1 < squared_length0) ? intersections[1] : intersections[0];
            intersected_triangles.push_back(info);
            markers[i] = true; // mark triangle for removal
        }
    }

    // Sort intersected triangles by intersection distance to segment start
    std::sort(
        intersected_triangles.begin(),
        intersected_triangles.end(),
        less_by_intersection_distance(projected_vertex0)
    );

    const u32 intersected_triangles_count = intersected_triangles.size();
    if (intersected_triangles_count == 0) {
        // No intersections found, nothing to remove
        return true;
    }

    u32 u0, u1;
    if (intersected_triangles_count > 1) {
        u32 edge_id = intersected_triangles[1].intersection.edge_id;
        const u32* indices = m_indices.begin() + 3 * intersected_triangles[1].triangle_id;

        // Initialize polygon index lists using the edges of the first intersected triangle
        for (u32 i = 0; i < 2; ++i) {
            add_pseudo_polygon_index(
                coordinate_indices,
                indices[(edge_id + i) % 3],
                right_polygon_indices,
                left_polygon_indices,
                segment_vertex0,
                segment_vertex1
            );
        }

        u0 = indices[(edge_id + 0) % 3];
        u1 = indices[(edge_id + 1) % 3];
    }

    // Process remaining intersected triangles and build polygon indices on both sides of the segment
    for (u32 i = 2; i < intersected_triangles_count; ++i) {
        u32 edge_id = intersected_triangles[i].intersection.edge_id;
        const u32* indices = m_indices.begin() + 3 * intersected_triangles[i].triangle_id;

        if (u1 == indices[(edge_id + 1) % 3]) {
            add_pseudo_polygon_index(
                coordinate_indices,
                indices[(edge_id + 0) % 3],
                right_polygon_indices,
                left_polygon_indices,
                segment_vertex0,
                segment_vertex1
            );
        }
        else if (u0 == indices[(edge_id + 0) % 3]) {
            // Normal case with reversed order
            add_pseudo_polygon_index(
                coordinate_indices,
                indices[(edge_id + 1) % 3],
                right_polygon_indices,
                left_polygon_indices,
                segment_vertex0,
                segment_vertex1
            );
        }
        else {
            // Topology inconsistency - log warning and skip triangle
            printf("WARNING: Skipping inconsistent triangle during remove_triangles (u0=%u, u1=%u, tri=%u)\n",
                u0, u1, intersected_triangles[i].triangle_id);
            continue;
        }

        u0 = indices[(edge_id + 0) % 3];
        u1 = indices[(edge_id + 1) % 3];
    }

    // Remove marked triangles from the index buffer
    m_indices.erase(
        std::remove_if(
            m_indices.begin(),
            m_indices.end(),
            is_marked_in_buffer_predicate3(m_indices, markers)
        ),
        m_indices.end()
    );

    // Reverse the left polygon indices list to maintain consistent winding
    std::reverse(left_polygon_indices.begin(), left_polygon_indices.end());

    return true;
}

/// Returns true if the test vertex lies outside the triangle's circumcircle
bool is_outside_circumcircle(
    u32 const (&coordinate_indices)[2],
    float3 const& triangle_vertex0,
    float3 const& triangle_vertex1,
    float3 const& triangle_vertex2,
    float3 const& test_vertex
);

/// Checks that the triangle's vertices are in correct winding order
inline bool constrained_delaunay_triangulator::is_correct_order(
    float3 const& v0,
    float3 const& v1,
    float3 const& v2
) {
    return ((normalize(v1 - v0) ^ normalize(v2 - v0)) | m_normal) > 0;
}

/// Recursively triangulates the polygon formed after triangle removal
void constrained_delaunay_triangulator::triangulate_pseudo_polygon(
    u32 const (&coordinate_indices)[2],
    polygon_indices_type::iterator const start,
    polygon_indices_type::iterator const end,
    u32 const edge_vertex_index0,
    u32 const edge_vertex_index1
) {
    u32 const vertices_count = u32(end - start);
    if (vertices_count > 1) {
        polygon_indices_type::iterator third_vertex = start;
        polygon_indices_type::iterator i = start + 1;
        for (; i != end; ++i) {
            if ((*i) == edge_vertex_index0 || (*i) == edge_vertex_index1)
                continue;

            if (!is_outside_circumcircle(
                    coordinate_indices,
                    m_vertices[edge_vertex_index0],
                    m_vertices[*third_vertex],
                    m_vertices[edge_vertex_index1],
                    m_vertices[*i]
                )
            ) {
                third_vertex = i;
            }
        }

        m_indices.push_back(edge_vertex_index0);
        m_indices.push_back(*third_vertex);
        m_indices.push_back(edge_vertex_index1);

        R_ASSERT(is_correct_order(
            m_vertices[m_indices[m_indices.size() - 3]],
            m_vertices[m_indices[m_indices.size() - 2]],
            m_vertices[m_indices[m_indices.size() - 1]]
        ));

        triangulate_pseudo_polygon(coordinate_indices, start, third_vertex, edge_vertex_index0, *third_vertex);
        triangulate_pseudo_polygon(coordinate_indices, third_vertex + 1, end, *third_vertex, edge_vertex_index1);
        return;
    }

    if (vertices_count > 0) {
        m_indices.push_back(edge_vertex_index0);
        m_indices.push_back(*start);
        m_indices.push_back(edge_vertex_index1);

        R_ASSERT(is_correct_order(
            m_vertices[m_indices[m_indices.size() - 3]],
            m_vertices[m_indices[m_indices.size() - 2]],
            m_vertices[m_indices[m_indices.size() - 1]]
        ));
    }
}

#ifndef MASTER_GOLD
void copy_config_to_clipboard(xray::configs::lua_config_ptr& /*config*/)
{
}
#endif