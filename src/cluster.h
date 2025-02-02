#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "variant.h"
#include "defs.h"

class ctgSuperclusters {
public:

    // constructor
    ctgSuperclusters();

    // add supercluster info
    void add_supercluster(
           std::vector<int> brks,
           int beg, int end);
    void set_phase(
           int sc_idx, int phase, 
           int orig_phase_dist, 
           int swap_phase_dist);

    // stores variant info for each contig
    // ctg_variants[truth/query][hap] -> variants
    std::vector< std::vector< std::shared_ptr<ctgVariants> > > ctg_variants;

    // cluster indices of superclusters
    // superclusters[truth/query][hap] -> cluster index
    std::vector< std::vector< std::vector<int> > > superclusters;

    int n = 0; // number of superclusters

    // helper information to find reference beg/end pos across haps, truth/query
    std::vector<int> begs, ends;

    // phasing information per supercluster
    std::vector<int> phase;
    std::vector<int> orig_phase_dist, swap_phase_dist;
};

class superclusterData {
public:
    superclusterData(
            std::shared_ptr<variantData> query_ptr,
            std::shared_ptr<variantData> truth_ptr,
            std::shared_ptr<fastaData> ref_ptr);

    void gap_supercluster();

    // data
    std::vector<std::string> contigs;
    std::vector<int> lengths;
    std::vector<int> ploidy;
    std::unordered_map<std::string, 
        std::shared_ptr<ctgSuperclusters> > ctg_superclusters;
    std::shared_ptr<fastaData> ref;
};


// for single haplotype clustering (one VCF)
void gap_cluster(std::shared_ptr<variantData> vcf, int callset);
void wf_swg_cluster(variantData * vcf, std::string ctg, int hap,
        int sub, int open, int extend);
std::vector< std::vector< std::vector<int> > > 
        sort_superclusters(std::shared_ptr<superclusterData>);

#endif
