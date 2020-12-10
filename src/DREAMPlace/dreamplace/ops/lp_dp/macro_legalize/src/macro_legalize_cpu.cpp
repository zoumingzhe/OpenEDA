/**
 * @file   macro_legalize.cpp
 * @author Yibo Lin
 * @date   Jun 2018
 */
#include "lp_dp/macro_legalize/src/hannan_legalize.h"
#include "lp_dp/macro_legalize/src/lp_legalize.h"
#include "lp_dp/macro_legalize/src/macro_legalize.h"
#include "utility/src/Box.h"

DREAMPLACE_BEGIN_NAMESPACE

/// @brief The macro legalization follows the way of floorplanning, 
/// because macros have quite different sizes. 
/// @return true if legal 

#define CHECK_FLAT(x) AT_ASSERTM(!x.is_cuda() && x.ndimension() == 1, #x "must be a flat tensor on CPU")
#define CHECK_EVEN(x) AT_ASSERTM((x.numel()&1) == 0, #x "must have even number of elements")
#define CHECK_CONTIGUOUS(x) AT_ASSERTM(x.is_contiguous(), #x "must be contiguous")

template <typename T>
bool check_macro_legality(LegalizationDB<T> db, const std::vector<int>& macros, bool fast_check)
{
    // check legality between movable and fixed macros 
    // for debug only, so it is slow 
    auto checkOverlap2Nodes = [&](int i, int node_id1, T xl1, T yl1, T width1, T height1, int j, int node_id2, T xl2, T yl2, T width2, T height2) {
        T xh1 = xl1 + width1; 
        T yh1 = yl1 + height1;
        T xh2 = xl2 + width2; 
        T yh2 = yl2 + height2; 
        if (std::min(xh1, xh2) > std::max(xl1, xl2) && std::min(yh1, yh2) > std::max(yl1, yl2))
        {
            dreamplacePrint((fast_check)? kWARN : kERROR, "macro %d (%g, %g, %g, %g) var %d overlaps with macro %d (%g, %g, %g, %g) var %d, fixed: %d\n", 
                    node_id1, xl1, yl1, xh1, yh1, i, 
                    node_id2, xl2, yl2, xh2, yh2, j, 
                    (int)(node_id2 >= db.num_movable_nodes)
                    ); 
            return true; 
        }
        return false; 
    };

    bool legal = true; 
    for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
    {
        int node_id1 = macros[i];
        T xl1 = db.x[node_id1];
        T yl1 = db.y[node_id1];
        T width1 = db.node_size_x[node_id1];
        T height1 = db.node_size_y[node_id1];
        // constraints with other macros 
        for (unsigned int j = i+1; j < ie; ++j)
        {
            int node_id2 = macros[j];
            T xl2 = db.x[node_id2];
            T yl2 = db.y[node_id2];
            T width2 = db.node_size_x[node_id2];
            T height2 = db.node_size_y[node_id2];

            bool overlap = checkOverlap2Nodes(i, node_id1, xl1, yl1, width1, height1, j, node_id2, xl2, yl2, width2, height2);
            if (overlap)
            {
                legal = false; 
                if (fast_check)
                {
                    return legal; 
                }
            }
        }
        // constraints with fixed macros 
        // when considering fixed macros, there is no guarantee to find legal solution 
        // with current ad-hoc constraint graphs 
        for (int j = db.num_movable_nodes; j < db.num_nodes; ++j)
        {
            int node_id2 = j; 
            T xl2 = db.init_x[node_id2];
            T yl2 = db.init_y[node_id2];
            T width2 = db.node_size_x[node_id2];
            T height2 = db.node_size_y[node_id2];

            bool overlap = checkOverlap2Nodes(i, node_id1, xl1, yl1, width1, height1, j, node_id2, xl2, yl2, width2, height2);
            if (overlap)
            {
                legal = false; 
                if (fast_check)
                {
                    return legal; 
                }
            }
        }
    }
    if (legal)
    {
        dreamplacePrint(kDEBUG, "Macro legality check PASSED\n");
    }
    else 
    {
        dreamplacePrint(kERROR, "Macro legality check FAILED\n");
    }

    return legal; 
}

template <typename T>
struct MacroLegalizeStats
{
    T total_displace; 
    T max_displace; 
    T total_weighted_displace; ///< displacement weighted by macro area ratio to average macro area 
    T max_weighted_displace; 
};

template <typename T>
MacroLegalizeStats<T> compute_displace(const LegalizationDB<T>& db, const std::vector<int>& macros)
{
    MacroLegalizeStats<T> stats; 
    stats.total_displace = 0; 
    stats.max_displace = 0; 
    stats.total_weighted_displace = 0; 
    stats.max_weighted_displace = 0; 

    for (auto node_id : macros)
    {
        T displace = std::abs(db.init_x[node_id]-db.x[node_id]) + std::abs(db.init_y[node_id]-db.y[node_id]);
        stats.total_displace += displace; 
        stats.max_displace = std::max(stats.max_displace, displace);

        displace *= db.node_weights[node_id]; 
        stats.total_weighted_displace += displace; 
        stats.max_weighted_displace = std::max(stats.max_weighted_displace, displace);
    }
    return stats; 
}

/// @brief Rough legalize some special macros 
/// 1. macros that form small clusters overlapping with each other 
/// 2. macros blocked by big ones 
/// All the other macros are regarded as fixed. 
/// @param small_clusters_flag controls whether to perform the legalization for 1 
/// @param blocked_macros_flag controls whether to perform the legalization for 2
template <typename T>
void roughLegalizeLauncher(const LegalizationDB<T>& db, const std::vector<int>& macros, const std::vector<int>& fixed_macros, 
        bool small_clusters_flag, bool blocked_macros_flag
        )
{
    std::vector<unsigned char> markers (db.num_nodes, false); 
    std::vector<int> macros_for_rough_legalize;
    std::vector<int> fixed_macros_for_rough_legalize;

    // collect small clusters 
    if (small_clusters_flag)
    {
        std::vector<std::vector<int> > clusters (macros.size()); 
        T cluster_area_ratio = 2; 
        T cluster_overlap_ratio = 0.5; 
        unsigned int cluster_macro_numbers_threshold = 2; 
        for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
        {
            int node_id1 = macros[i];
            UBox<T> box1 (db.x[node_id1], db.y[node_id1], 
                    db.x[node_id1] + db.node_size_x[node_id1], db.y[node_id1] + db.node_size_y[node_id1]);
            T a1 = box1.area();
            clusters.at(i).push_back(node_id1);
            for (unsigned int j = i + 1; j < ie; ++j)
            {
                int node_id2 = macros[j];
                UBox<T> box2 (db.x[node_id2], db.y[node_id2], 
                        db.x[node_id2] + db.node_size_x[node_id2], db.y[node_id2] + db.node_size_y[node_id2]);
                T a2 = box2.area();

                if (a1 >= a2 / cluster_area_ratio && a1 <= a2 * cluster_area_ratio)
                {
                    T overlap = std::max((T)0, std::min(box1.xh, box2.xh) - std::max(box1.xl, box2.xl)) * std::max((T)0, std::min(box1.yh, box2.yh) - std::max(box1.yl, box2.yl)); 
                    if (overlap >= std::min(a1, a2) * cluster_overlap_ratio)
                    {
                        clusters.at(i).push_back(node_id2);
                    }
                }
            }
        }
        for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
        {
            if (clusters.at(i).size() >= cluster_macro_numbers_threshold)
            {
                markers.at(macros.at(i)) = true; 
            }
        }
    }
    // collect small macros blocked by large ones 
    // If a small macro is blocked by two big macros, it is easier to move the small one around. 
    // We detect such blocks by checking whether the macro is blocked from left, right, bottom, top 4 directions. 
    // Any macro with (left, right) or (bottom, top) blocked will be collected. 
    if (blocked_macros_flag)
    {
        T blocked_macros_area_ratio = 10; // the area ratio of macros to be regarded as large 
        T blocked_macros_direct_threshold = 0.9; // determine the direction blocked 
        for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
        {
            int node_id1 = macros[i];
            if (!markers[node_id1])
            {
                UBox<T> box1 (db.x[node_id1], db.y[node_id1], 
                        db.x[node_id1] + db.node_size_x[node_id1], db.y[node_id1] + db.node_size_y[node_id1]);
                T a1 = box1.area();
                std::array<unsigned char, 4> intersect_directs; // from L, R, B, T direction, the box is overlapped 
                intersect_directs.fill(0);
                for (unsigned int j = 0; j < ie; ++j)
                {
                    int node_id2 = macros[j];
                    if (i != j && !markers[node_id2])
                    {
                        UBox<T> box2 (db.x[node_id2], db.y[node_id2], 
                                db.x[node_id2] + db.node_size_x[node_id2], db.y[node_id2] + db.node_size_y[node_id2]);
                        T a2 = box2.area();

                        if (a1 * blocked_macros_area_ratio < a2)
                        {
                            UBox<T> intersect_box (
                                    std::max(box1.xl, box2.xl), 
                                    std::max(box1.yl, box2.yl), 
                                    std::min(box1.xh, box2.xh), 
                                    std::min(box1.yh, box2.yh)
                                    );
                            if (intersect_box.xl < intersect_box.xh && intersect_box.yl < intersect_box.yh)
                            {
                                if (intersect_box.height() > box1.height() * blocked_macros_direct_threshold)
                                {
                                    if (box2.xl <= box1.xl)
                                    {
                                        intersect_directs[kXLOW] = 1; 
                                    }
                                    if (box2.xh >= box1.xh)
                                    {
                                        intersect_directs[kXHIGH] = 1; 
                                    }
                                }
                                if (intersect_box.width() > box1.width() * blocked_macros_direct_threshold)
                                {
                                    if (box2.yl <= box1.yl)
                                    {
                                        intersect_directs[kYLOW] = 1; 
                                    }
                                    if (box2.yh >= box1.yh)
                                    {
                                        intersect_directs[kYHIGH] = 1; 
                                    }
                                }

                                //if (node_id2 == 1096674)
                                if (node_id1 == 1096131 || node_id1 == 1096158)
                                {
                                    dreamplacePrint(kDEBUG, "%d (%g, %g, %g, %g) overlap %d (%g, %g, %g, %g), (%g, %g, %g, %g), (%u, %u, %u, %u)\n", 
                                            node_id1, box1.xl, box1.yl, box1.xh, box1.yh, 
                                            node_id2, box2.xl, box2.yl, box2.xh, box2.yh, 
                                            intersect_box.xl, intersect_box.yl, intersect_box.xh, intersect_box.yh, 
                                            (unsigned)intersect_directs[0], (unsigned)intersect_directs[1], (unsigned)intersect_directs[2], (unsigned)intersect_directs[3]
                                            );
                                }
                            }
                        }
                        if ((intersect_directs[kXLOW] && intersect_directs[kXHIGH])
                                || (intersect_directs[kYLOW] && intersect_directs[kYHIGH]))
                        {
                            markers[node_id1] = true; 
                            dreamplacePrint(kDEBUG, "collect %d\n", node_id1);
                            break; 
                        }
                    }
                }
            }
        }
    }

    fixed_macros_for_rough_legalize = fixed_macros;
    for (auto node_id : macros)
    {
        if (markers[node_id])
        {
            macros_for_rough_legalize.push_back(node_id);
        }
        else 
        {
            fixed_macros_for_rough_legalize.push_back(node_id);
        }
    }

    dreamplacePrint(kINFO, "Rough legalize small clusters with %lu macros\n", macros_for_rough_legalize.size());
#ifdef DEBUG
    dreamplacePrint(kDEBUG, "macros_for_rough_legalize[%lu]\n", macros_for_rough_legalize.size());
    for (auto node_id : macros_for_rough_legalize)
    {
        dreamplacePrint(kNONE, " %d", node_id);
    }
    dreamplacePrint(kNONE, "\n");
#endif
    hannanLegalizeLauncher(db, macros_for_rough_legalize, fixed_macros_for_rough_legalize, 1);
}

template <typename T>
bool macroLegalizationLauncher(LegalizationDB<T> db)
{
    // collect macros 
    std::vector<int> macros; 
    for (int i = 0; i < db.num_movable_nodes; ++i)
    {
        if (db.is_dummy_fixed(i))
        {
            // in some extreme case, some macros with 0 area should be ignored  
            T area = db.node_size_x[i] * db.node_size_y[i]; 
            if (area > 0)
            {
                macros.push_back(i);
            }
#ifdef DEBUG
            dreamplacePrint(kDEBUG, "macro %d %gx%g\n", i, db.node_size_x[i], db.node_size_y[i]);
#endif
        }
    }
    dreamplacePrint(kINFO, "Macro legalization: regard %lu cells as dummy fixed (movable macros)\n", macros.size());

    // in case there is no movable macros 
    if (macros.empty())
    {
        return true;
    }

    // fixed macros 
    std::vector<int> fixed_macros;
    fixed_macros.reserve(db.num_nodes - db.num_movable_nodes); 
    for (int i = db.num_movable_nodes; i < db.num_nodes; ++i)
    {
        // in some extreme case, some fixed macros with 0 area should be ignored  
        T area = db.node_size_x[i] * db.node_size_y[i]; 
        if (area > 0)
        {
            fixed_macros.push_back(i);
        }
    }

    // store the best legalization solution found
    std::vector<T> best_x (macros.size());
    std::vector<T> best_y (macros.size());
    MacroLegalizeStats<T> best_displace; 
    best_displace.total_displace = std::numeric_limits<T>::max();
    best_displace.max_displace = std::numeric_limits<T>::max();
    best_displace.total_weighted_displace = std::numeric_limits<T>::max();
    best_displace.max_weighted_displace = std::numeric_limits<T>::max();

    // update current best solution  
    auto update_best = [&](bool legal, const MacroLegalizeStats<T>& displace){
        if (legal && displace.total_displace < best_displace.total_displace)
        {
            for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
            {
                int macro_id = macros[i];
                best_x[i] = db.x[macro_id];
                best_y[i] = db.y[macro_id];
            }
            best_displace = displace; 
        }
    };

    // first round rough legalization with Hannan grid for clusters 
    bool small_clusters_flag = true; 
    bool blocked_macros_flag = false; 
    roughLegalizeLauncher(db, macros, fixed_macros, small_clusters_flag, blocked_macros_flag);

    // second round with LP 
    lpLegalizeGraphLauncher(db, macros, fixed_macros);
    auto displace = compute_displace(db, macros);
    dreamplacePrint(kINFO, "Macro displacement total %g, max %g, weighted total %g, max %g\n", 
            displace.total_displace, displace.max_displace, displace.total_weighted_displace, displace.max_weighted_displace);
    bool legal = check_macro_legality(db, macros, true);

    // try Hannan grid legalization if still not legal 
    if (!legal)
    {
        legal = hannanLegalizeLauncher(db, macros, fixed_macros, 10);
        displace = compute_displace(db, macros);
        dreamplacePrint(kINFO, "Macro displacement total %g, max %g, weighted total %g, max %g\n", 
                displace.total_displace, displace.max_displace, displace.total_weighted_displace, displace.max_weighted_displace);
        legal = check_macro_legality(db, macros, true);
        update_best(legal, displace);

        // refine with LP if legal 
        if (legal)
        {
            lpLegalizeLauncher(db, macros, fixed_macros);
            displace = compute_displace(db, macros);
            dreamplacePrint(kINFO, "Macro displacement total %g, max %g, weighted total %g, max %g\n", 
                    displace.total_displace, displace.max_displace, displace.total_weighted_displace, displace.max_weighted_displace);
            legal = check_macro_legality(db, macros, true);
            update_best(legal, displace);
        }

        // apply best solution 
        if (best_displace.total_displace < std::numeric_limits<T>::max())
        {
            dreamplacePrint(kINFO, "use best macro displacement total %g, max %g, weighted total %g, max %g\n", 
                    best_displace.total_displace, best_displace.max_displace, best_displace.total_weighted_displace, best_displace.max_weighted_displace);
            for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
            {
                int macro_id = macros[i];
                db.x[macro_id] = best_x[i];
                db.y[macro_id] = best_y[i];
            }
        }
    }

    dreamplacePrint(kINFO, "Align macros to site and rows\n");
    // align the lower left corner to row and site
    for (unsigned int i = 0, ie = macros.size(); i < ie; ++i)
    {
        int node_id = macros[i];
        db.x[node_id] = db.align2site(db.x[node_id], db.node_size_x[node_id]);
        db.y[node_id] = db.align2row(db.y[node_id], db.node_size_y[node_id]);
    }

    legal = check_macro_legality(db, macros, false);

    return legal; 
}

/// @brief call function for MacroLegal:run
int macroLegalizationRun(LegalizationDB<int>& db)
{
  macroLegalizationLauncher<int>(db);
  return 0;
}

DREAMPLACE_END_NAMESPACE

