#ifndef _DEF_H_
#define _DEF_H_

#include <sys/time.h>
#include <unordered_set>
#include <unordered_map>

class timer;

// misc
#define FAIL 0
#define PASS 1

#define FALSE 0
#define TRUE  1

// variant
#define TYPE_REF 0
#define TYPE_ALL 0
#define TYPE_SUB 1
#define TYPE_INS 2
#define TYPE_DEL 3
#define TYPE_CPX 4
#define TYPE_INDEL 4
#define TYPES 5

#define VARTYPE_SNP   0
#define VARTYPE_INDEL 1
#define VARTYPES      2

#define HAP1 0
#define HAP2 1
#define HAPS 2

#define BED_OUTSIDE 0
#define BED_INSIDE  1
#define BED_BORDER  2
#define BED_OFFCTG  3

#define GT_REF       0
#define GT_ALT1      1
#define GT_REF_REF   2
#define GT_REF_ALT1  3
#define GT_ALT1_REF  4
#define GT_ALT1_ALT1 5
#define GT_ALT1_ALT2 6
#define GT_ALT2_ALT1 7
#define GT_MISSING   8
#define GT_OTHER     9

#define ERRTYPE_TP 0 // true positive
#define ERRTYPE_FP 1 // false positive
#define ERRTYPE_FN 2 // false negative
#define ERRTYPE_PP 3 // partial positive (reduces ED, but not TP)
#define ERRTYPE_PE 4 // phase error (0|1 -> 1|0)
#define ERRTYPE_GE 5 // genotype error (0|1 -> 1|1)
#define ERRTYPE_UN 6 // unknown
#define ERRTYPES   7

// runtime timers
#define TIME_READ     0
#define TIME_CLUST    1
#define TIME_REALN    2
#define TIME_RECLUST  3
#define TIME_SUPCLUST 4
#define TIME_ALIGN    5
#define TIME_PHASE    6
#define TIME_WRITE    7
#define TIME_TOTAL    8

// alignment
#define QUERY    0
#define REF      1
#define TRUTH    1
#define CALLSETS 2

#define QUERY1_TRUTH1 0 // four possible alignments of truth and query haps
#define QUERY1_TRUTH2 1
#define QUERY2_TRUTH1 2
#define QUERY2_TRUTH2 3

#define PEN_SUB    0 // penalties order in boolean array *_penalties_set
#define PEN_OPEN   1
#define PEN_EXTEND 2

#define PTR_NONE  0  // backtracking pointer flags
#define PTR_INS   1
#define PTR_DEL   2
#define PTR_MAT   4
#define PTR_SUB   8
#define PTR_SWP_MAT   16
#define PTR_SWP_SUB   32  // not used atm
#define PTR_SWP_INS   64  // not used atm
#define PTR_SWP_DEL   128 // not used atm
#define PTR_LPATH 256
#define PATH      256
#define PTR_RPATH 512
#define MAIN_PATH 768
#define PTR_SYNC  1024

// 2 x N pointer array stores REF <-> QUERY/TRUTH
#define PTRS 0 // dimension for pointer position
#define FLAGS 1 // dimension for flags
#define PTR_DIMS 2
#define PTR_VARIANT 1
#define PTR_VAR_BEG 2
#define PTR_VAR_END 4

#define MAT_SUB 0 // three matrices for Smith-Waterman
#define MAT_INS 1
#define MAT_DEL 2
#define MATS    3

// phasing
#define PHASE_ORIG 0
#define PHASE_SWAP 1
#define PHASE_NONE 2

#define PHASE_PTR_KEEP 0
#define PHASE_PTR_SWAP 1

class idx1 {
public:
    int hi;  // hap idx1
    int qri; // query/ref idx1
    int ti;  // truth idx1

    idx1() : hi(0), qri(0), ti(0) {};
    idx1(int h, int q, int t) : hi(h), qri(q), ti(t) {};
    idx1(const idx1 & i2) : hi(i2.hi), qri(i2.qri), ti(i2.ti) {};

    bool operator<(const idx1 & other) const {
        if (this->hi < other.hi) return true;
        else if (this->hi == other.hi && this->qri < other.qri) return true;
        else if (this->hi == other.hi && this->qri == other.qri && this->ti < other.ti) return true;
        return false;
    }
    bool operator==(const idx1 & other) const {
        return this->hi == other.hi && this->qri == other.qri && this->ti == other.ti;
    }
    idx1 & operator=(const idx1 & other) {
        if (this == &other) return *this;
        this->hi = other.hi;
        this->qri = other.qri;
        this->ti = other.ti;
        return *this;
    }
};

namespace std {
    template<> struct hash<idx1> {
        std::uint64_t operator()(const idx1& x) const noexcept {
            return (uint64_t(x.hi) *73856093 + 0x517cc1b727220a95) ^ 
                   (uint64_t(x.qri)*19349669 + 0xd15f392b3d4704a2) ^ 
                   (uint64_t(x.ti) *83492791);
        }
    };
}

template <typename T>
bool contains(const std::unordered_set<T> & wave, const T & idx);

template <typename T, typename U>
bool contains(const std::unordered_map<T,U> & wave, const T & idx);

template <typename S, typename T>
void set(std::unordered_map<S,T> & ptrs, const S & idx, const T & flag);

template <typename S, typename T>
bool test(const std::unordered_map<S,T> & ptrs, const S & idx, const T & flag);

class idx2 {
public:
    int mi;  // matrix idx2
    int qi;  // query idx2
    int ri;  // ref idx2

    idx2() : mi(0), qi(0), ri(0) {};
    idx2(int q, int r) : mi(0), qi(q), ri(r) {};
    idx2(int m, int q, int r) : mi(m), qi(q), ri(r) {};
    idx2(const idx2 & i2) : mi(i2.mi), qi(i2.qi), ri(i2.ri) {};

    bool operator<(const idx2 & other) const {
        if (this->mi < other.mi) return true;
        else if (this->mi == other.mi && this->qi < other.qi) return true;
        else if (this->mi == other.mi && this->qi == other.qi && this->ri < other.ri) return true;
        return false;
    }
    bool operator==(const idx2 & other) const {
        return this->mi == other.mi && this->qi == other.qi && this->ri == other.ri;
    }
    idx2 & operator=(const idx2 & other) {
        if (this == &other) return *this;
        this->mi = other.mi;
        this->qi = other.qi;
        this->ri = other.ri;
        return *this;
    }
};

namespace std {
    template<> struct hash<idx2> {
        std::uint64_t operator()(const idx2& other) const noexcept {
            return uint64_t(other.mi)<<60 | uint64_t(other.qi) << 30 | uint64_t(other.ri);
        }
    };
}

// printing
#define WARN(f_, ...)                                           \
{                                                               \
    struct tm _tm123_;                                          \
    struct timeval _xxtv123_;                                   \
    gettimeofday(&_xxtv123_, NULL);                             \
    localtime_r(&_xxtv123_.tv_sec, &_tm123_);                   \
    fprintf(stderr, "\033[33m[WARN  %s %02d:%02d:%02d]\033[0m ",\
            g.PROGRAM.data(), _tm123_.tm_hour,_tm123_.tm_min,   \
            _tm123_.tm_sec);                                    \
    fprintf(stderr, (f_), ##__VA_ARGS__);                       \
    fprintf(stderr, "\n");                                      \
};

#define INFO(f_, ...)                                           \
{                                                               \
    struct tm _tm123_;                                          \
    struct timeval _xxtv123_;                                   \
    gettimeofday(&_xxtv123_, NULL);                             \
    localtime_r(&_xxtv123_.tv_sec, &_tm123_);                   \
    fprintf(stderr, "\033[32m[INFO  %s %02d:%02d:%02d]\033[0m ",\
            g.PROGRAM.data(), _tm123_.tm_hour,_tm123_.tm_min,   \
            _tm123_.tm_sec);                                    \
    fprintf(stderr, (f_), ##__VA_ARGS__);                       \
    fprintf(stderr, "\n");                                      \
};

#define ERROR(f_, ...)                                           \
{                                                                \
    struct tm _tm123_;                                           \
    struct timeval _xxtv123_;                                    \
    gettimeofday(&_xxtv123_, NULL);                              \
    localtime_r(&_xxtv123_.tv_sec, &_tm123_);                    \
    fprintf(stderr, "\033[31m[ERROR %s %02d:%02d:%02d]\033[0m ", \
            g.PROGRAM.data(), _tm123_.tm_hour,_tm123_.tm_min,    \
            _tm123_.tm_sec);                                     \
    fprintf(stderr, (f_), ##__VA_ARGS__);                        \
    fprintf(stderr, "\n");                                       \
    std::exit(1);                                                \
};

#endif
