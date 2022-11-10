#include "variant.h"
#include "print.h"
#include "globals.h"
#include "fasta.h"
#include "bed.h"
#include "dist.h"
#include "cluster.h"
#include "phase.h"

Globals g;
std::vector<std::string> type_strs = {"REF", "SUB", "INS", "DEL", "GRP"};
std::vector<std::string> error_strs = {"TP", "FP", "FN", "PP", "PE", "GE", "??"};
std::vector<std::string> gt_strs = {
".|.", "0|0", "0|1", "0|2", "1|0", "1|1", "1|2", "2|0", "2|1", "2|2", "?|?" };
std::vector<std::string> region_strs = {"OUTSIDE", "INSIDE ", "BORDER ", "OFF CTG"};
std::vector<std::string> aln_strs = {"CALLS1-TRUTH1", "CALLS1-TRUTH2", "CALLS2-TRUTH1", "CALLS2-TRUTH2"};
std::vector<std::string> phase_strs = {"=", "X", "?"};
 
int main(int argc, char **argv) {

    // parse and store command-line args
    g.parse_args(argc, argv);

    // 0. parse reference fasta
    std::shared_ptr<fastaData> ref_ptr(new fastaData(g.ref_fasta_fp));

    // 1. parse VCFs
    std::unique_ptr<variantData> calls_ptr(new variantData(g.calls_vcf_fn, ref_ptr));
    std::unique_ptr<variantData> truth_ptr(new variantData(g.truth_vcf_fn, ref_ptr));

    // 2. cluster variants
    sw_cluster(calls_ptr);
    sw_cluster(truth_ptr);

    // 3. realign variants
    variantData calls_sw = sw_realign(calls_ptr, ref_ptr);
    variantData truth_sw = sw_realign(truth_ptr, ref_ptr);

    // save old/new variants
    calls_ptr->write(g.out_prefix + "orig_calls.vcf");
    calls_sw.write(g.out_prefix + "calls.vcf");
    truth_ptr->write(g.out_prefix + "orig_truth.vcf");
    truth_sw.write(g.out_prefix + "truth.vcf");

    // 4. calculate superclusters
    std::shared_ptr<clusterData> clusterdata_ptr(new clusterData(calls_ptr, truth_ptr, ref_ptr));

    // 5. calculate edit distance and local phasing
    alignment_wrapper(clusterdata_ptr);

    // 6. calculate global phasings
    std::unique_ptr<phaseData> phasedata_ptr(new phaseData(clusterdata_ptr));

    // 7. write results in CSV format
    write_results(phasedata_ptr);

    return EXIT_SUCCESS;
}
