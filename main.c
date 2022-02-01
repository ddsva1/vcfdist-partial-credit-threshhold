#include <assert.h>
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "lv89.h"
#include "ketopt.h"
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

int main(int argc, char *argv[])
{
	gzFile fp1, fp2;
	kseq_t *ks1, *ks2;
	ketopt_t o = KETOPT_INIT;
	int c, s, is_global = 0;
	uint8_t *mem;

	while ((c = ketopt(&o, argc, argv, 1, "g", 0)) >= 0) {
		if (c == 'g') is_global = 1;
	}
	if (argc - o.ind < 2) {
		fprintf(stderr, "Usage: test-ed [options] <in1.fa> <in2.fa>\n");
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "  -g    count gaps at the end of the target\n");
		return 1;
	}

	fp1 = gzopen(argv[o.ind+0], "r");
	fp2 = gzopen(argv[o.ind+1], "r");
	assert(fp1 && fp2);
	ks1 = kseq_init(fp1);
	ks2 = kseq_init(fp2);
	kseq_read(ks1);
	kseq_read(ks2);

	mem = (uint8_t*)malloc((ks1->seq.l + ks2->seq.l) * 16);
	s = lv_ed(ks1->seq.l, ks1->seq.s, ks2->seq.l, ks2->seq.s, is_global, mem);
	free(mem);
	printf("%s\t%s\t%d\n", ks1->name.s, ks2->name.s, s);

	kseq_destroy(ks1);
	kseq_destroy(ks2);
	gzclose(fp1);
	gzclose(fp2);
	return 0;
}
